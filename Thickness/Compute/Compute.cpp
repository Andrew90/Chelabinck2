#include "stdafx.h"
#include "Compute.h"
#include <Mmsystem.h>
#include "AppBase.h"
#include "ThicknessData.h"
#include "ThicknessViewer.h"
#include "StoredData.h"
#include "mainwindow.h"
#include "DetailedWindow.h"
#include "SQEvents.h"
#include "PrimaryData.h"

#include "Lan.h"

#include "Debug1730.h"
#include "DebugMess.h"
#ifdef XDEBUG
#define dprint debug.print
#else
#define dprint
#endif
//--------------------------------------------------------------------
Compute::Compute(PrimaryData &pd)
	: primaryData(pd)
    , referenceOffset1(Singleton<AdditionalSettingsTable>::Instance().items.get<ReferenceOffset1>().value)
	, referenceOffset2(Singleton<AdditionalSettingsTable>::Instance().items.get<ReferenceOffset2>().value)
    , referenceOffset3(Singleton<AdditionalSettingsTable>::Instance().items.get<ReferenceOffset3>().value)
	, referenceOffset4(Singleton<AdditionalSettingsTable>::Instance().items.get<ReferenceOffset4>().value)
	, referenceOffsetSQ3(300)
	, frameSize(Singleton<LanParametersTable>::Instance().items.get<PacketSize>().value)
	, nominalThickness(Singleton<ThresholdsTable>::Instance().items.get<NominalThickness>().value)
	, nominalPercentMin(Singleton<ThresholdsTable>::Instance().items.get<NominalPercentMin>().value)
	, nominalPercentMax(Singleton<ThresholdsTable>::Instance().items.get<NominalPercentMax>().value)
	, defectDifferentWallPercent(Singleton<ThresholdsTable>::Instance().items.get<DefectDifferentWallPercent>().value)
	, frequencyFrames(Singleton<AdditionalSettingsTable>::Instance().items.get<FrequencyFrames>().value)
	, deadAreaMM0(Singleton<DeadAreaTable>::Instance().items.get<DeadAreaMM0>().value)
	, deadAreaMM1(Singleton<DeadAreaTable>::Instance().items.get<DeadAreaMM1>().value)	
	, medianFiltreLength(Singleton<MedianFilterTable>::Instance().items.get<MedianFilter>().value)
	, acfBorderLeft(Singleton<ACFBorderTable>::Instance().items.get<ACFBorderLeft>().value)
	, acfBorderRight(Singleton<ACFBorderTable>::Instance().items.get<ACFBorderRight>().value)
{
	InitializeCriticalSection(&cs0);
	InitializeCriticalSection(&cs1);
	hEvent = CreateEvent(NULL, FALSE, FALSE, LAN_DEVICE_FRAME_READY);
	
	hThread[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc, this, CREATE_SUSPENDED, NULL);
	hThread[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc, this, CREATE_SUSPENDED, NULL);	
}
//--------------------------------------------------------------------
void Compute::MeshuringBaseStart()
{
	deltaTimeZoneStart = timeGetTime();
	lastOffset = 0;
	lastZone = 0;
	lastFrame = 0;
}
//--------------------------------------------------------------------
typedef TL::CreateNumList<TL::IntToType, count_sensors, 1 + count_sensors>::Result num_list;

template<class O, class P>struct Compute::__init_offset__
{
	void operator()(O *, P *p)
	{
		static const int i = O::value - 1;
		p->coefficientA[i] = Singleton<CoefficientParametersTable>::Instance().items.get<CoefficientA<O::value> >().value;
		p->coefficientB[i] = Singleton<CoefficientParametersTable>::Instance().items.get<CoefficientB<O::value> >().value;

		p->minOffset[i] = unsigned((Singleton<BorderCredibilityTable>::Instance().items.get<MinimumThicknessPipeWall>().value
		   - p->coefficientB[i]) / p->coefficientA[i]
		);
		//double nominalThickness = Singleton<ThresholdsTable>::Instance().items.get<NominalThickness>().value;
		//p->maxOffset[i] = unsigned(1.5 * (nominalThickness - p->coefficientB[i]) / p->coefficientA[i]);
		p->maxOffset[i] = unsigned((Singleton<BorderCredibilityTable>::Instance().items.get<MaximumThicknessPipeWall>().value
		   - p->coefficientB[i]) / p->coefficientA[i]
		);
	}
};
template<int N, class P>struct Compute::__init_energy__<MinEnergy<N>, P>
{
	void operator()(MinEnergy<N> *o, P *p)
	{
		p->minEnergy[N] = o->value;
	}
};
template<int N, class P>struct Compute::__init_energy__<MaxEnergy<N>, P>
{
	void operator()(MaxEnergy<N> *o, P *p)
	{
		p->maxEnergy[N] = o->value;
	}
};
template<int N, class P>struct Compute::__init_energy__<Peak<N>, P>
{
	void operator()(Peak<N> *o, P *p)
	{
		p->peak[N] = o->value;
	}
};

struct Lock
{
	CRITICAL_SECTION &critical;
	Lock(CRITICAL_SECTION &critical)
		: critical(critical)
	{
		EnterCriticalSection(&critical);
	}
    ~Lock()
	{
		LeaveCriticalSection(&critical);
	}
};

void Compute::ThreadProc(Compute *d)
{
	while(true)
	{
		switch(WaitForSingleObjectEx(d->hEvent, 1300, TRUE))
		{
		case WAIT_OBJECT_0:
			d->Calculation();
			break;
		case WAIT_TIMEOUT:
			{
				SuspendThread(GetCurrentThread());
			}
			break;
		case WAIT_IO_COMPLETION: 
			return;
		}
	}
}

void Compute::InitParam()
{
	fft.Init(frameSize);

	TL::foreach<num_list, Compute::__init_offset__>()((TL::Factory<num_list> *)0, this);

	TL::foreach<SignalParametersTable::items_list, Compute::__init_energy__>()(
		&Singleton<SignalParametersTable>::Instance().items
		, this
		);
	for(int i = 0; i < dimention_of(filtre); ++i)
	{
		filtre[i].SetLength(medianFiltreLength);
		filtre[i].Clear();
	}
	defectBorderMin = nominalThickness * (1.0 - nominalPercentMin / 100);
	defectBorderMax = nominalThickness * (1.0 + nominalPercentMax / 100);
	defectDifferentWall = defectDifferentWallPercent * nominalThickness / 100;
}
#if 1
void Compute::MeshuringBaseStop1()
{
	deltaTimePaintMarker = 0;
	deltaTimeZoneStop1 = timeGetTime();

	deltaTime = (double)zone_length * (deltaTimeZoneStop1 - deltaTimeZoneStart) / referenceOffset1;//время прохождения зоны

	primaryData.countZones = 0;
	offsetOfZonesOffset = (double)(deltaTimeZoneStop1 - deltaTimeZoneStart) * deadAreaMM0 / referenceOffset1;
	primaryData.offsetOfTime[0] =  offsetOfZonesOffset;
	stop = 0;
	for(int i = 0; i < count_sensors; ++i)
	{
	   sensorsData[i].zonesMin[0] = result_undefined;
	   sensorsData[i].zonesMax[0] = result_undefined;
	   sensorsData[i].status[0] = PrimaryData::Undefined;
	}
	thicknessData.zonesMin[0] = result_undefined;
	thicknessData.zonesMax[0] = result_undefined;
	thicknessData.status[0] = PrimaryData::Undefined;
	InitParam();
	while(ResumeThread(hThread[0]));
	Sleep(10);
	while(ResumeThread(hThread[1]));
	Sleep(10);
}
#else
void Compute::MeshuringBaseStop1()
{
	deltaTimePaintMarker = 0;
	deltaTimeZoneStop1 = timeGetTime();

	deltaTime = (double)zone_length * (deltaTimeZoneStop1 - deltaTimeZoneStart) / referenceOffset1;//время прохождения зоны

	primaryData.countZones = 0;
	offsetOfZonesOffset = 0;
	primaryData.offsetOfTime[0] = 0;
	deadAreaTime = int((double)(deltaTimeZoneStop1 - deltaTimeZoneStart) * deadAreaMM0 / referenceOffset1);
	//offsetOfZonesOffset = (double)(deltaTimeZoneStop1 - deltaTimeZoneStart) * deadAreaMM0 / referenceOffset1;
	//primaryData.offsetOfTime[0] =  offsetOfZonesOffset;
	stop = 0;
	for(int i = 0; i < count_sensors; ++i)
	{
	   sensorsData[i].zonesMin[0] = result_undefined;
	   sensorsData[i].zonesMax[0] = result_undefined;
	   sensorsData[i].status[0] = PrimaryData::Undefined;
	}
	thicknessData.zonesMin[0] = result_undefined;
	thicknessData.zonesMax[0] = result_undefined;
	thicknessData.status[0] = PrimaryData::Undefined;
	InitParam();
	while(ResumeThread(hThread[0]));
	Sleep(10);
	while(ResumeThread(hThread[1]));
	Sleep(10);
}
#endif
//----------------------------------------------------------------------
void Compute::MeshuringBaseStop2()
{
	deltaTimeZoneStop2 = timeGetTime();
	deltaTime = (double)zone_length * (deltaTimeZoneStop2 - deltaTimeZoneStart) / referenceOffset2;//время прохождения зоны
}
//-----------------------------------------------------------------------------------------
void Compute::EndAdjustmentsTube()//коррекция результата с учётом мёртвой зоны в конце трубы
{
	double offsetOfZonesBack = (double)(deltaTimeZoneStop2 - deltaTimeZoneStart) * deadAreaMM1 / referenceOffset2;
	offsetOfZonesOffset -= offsetOfZonesBack + stopMeshuring;
	for(int i = primaryData.countZones; i > 0; --i)
	{
    	if(primaryData.offsetOfTime[i] < offsetOfZonesOffset)
		{
			primaryData.countZones = i + 1;
			primaryData.offsetOfTime[i] =  offsetOfZonesOffset;
		}
	}
	UsefulPiecePipeDo();
	DetailedWindow::Instance().Update();
}
//---------------------------------------------------------------------------------------------
void Compute::StopMeshuring()
{
	stopMeshuring = timeGetTime();
}
//---------------------------------------------------------------------------------------
struct LockTry
{
	CRITICAL_SECTION &cs;
	LockTry(CRITICAL_SECTION &cs) : cs(cs) {}
	~LockTry(){LeaveCriticalSection(&cs);}
};
//----------------------------------------------------------------------------------
void SetDataStatus(double &sensorsMin, double &sensorsMax, char &sensorsStatus, char stat, double data_i)
{
	if(PrimaryData::Undefined == sensorsStatus)
	{
			switch(stat)
			{
			case PrimaryData::Nominal:
				sensorsMin = data_i;
				sensorsMax = data_i;
				sensorsStatus = PrimaryData::Nominal;
				return;
			case PrimaryData::DefectMin:
				sensorsMin = data_i;
				sensorsMax = data_i;
				sensorsStatus = PrimaryData::DefectMin;
				return;
			case PrimaryData::DefectMax:
				sensorsMin = data_i;
				sensorsMax = data_i;
				sensorsStatus = PrimaryData::DefectMax;
				return;
			}
	}
	else if(PrimaryData::Nominal == stat || PrimaryData::DefectMin == stat || PrimaryData::DefectMax == stat)
	{
		char minState = PrimaryData::Undefined;
		char maxState = PrimaryData::Undefined;

		if(sensorsMin > data_i)
		{
			sensorsMin = data_i;
			minState = stat;
		}
		if(sensorsMax < data_i)
		{
			sensorsMax = data_i;
			maxState = stat;
		}

		if(minState == PrimaryData::Undefined && maxState == PrimaryData::Undefined) return;

		if(minState == PrimaryData::Undefined) 
		{
			minState = sensorsStatus;
			if(PrimaryData::DefectMinMax == sensorsStatus) return;
		}
		if(maxState == PrimaryData::Undefined)
		{
			maxState = sensorsStatus;
			if(PrimaryData::DefectMinMax == sensorsStatus) return;
		}
		
		switch(minState)
		{
		case PrimaryData::Nominal:
			switch(maxState)
			{
			case PrimaryData::Nominal:
				sensorsStatus = PrimaryData::Nominal;
				break;
			case PrimaryData::DefectMin:
				sensorsStatus = PrimaryData::DefectMin;
				break;
			case PrimaryData::DefectMax:
				sensorsStatus = PrimaryData::DefectMax;
				break;
			}
			break;
		case PrimaryData::DefectMin:
			switch(maxState)
			{
			case PrimaryData::Nominal:
            case PrimaryData::DefectMin:
				sensorsStatus = PrimaryData::DefectMin;
				break;
			case PrimaryData::DefectMax:
				sensorsStatus = PrimaryData::DefectMinMax;
				break;
			}
			break;
		case PrimaryData::DefectMax:
			switch(maxState)
			{
			case PrimaryData::Nominal:
			case PrimaryData::DefectMax:
				sensorsStatus = PrimaryData::DefectMax;
				break;
			case PrimaryData::DefectMin:
				sensorsStatus = PrimaryData::DefectMinMax;
				break;
			}
			break;
		}
	}
}

inline double __abs__(double x){return x >= 0 ? x : -x;}
void Compute::Calculation()
{
	unsigned stopOffset; 
	unsigned startOffset;
	for(;;)
	{		
		{
			Lock lock(cs0);
			stopOffset = primaryData.GetCurrentOffset();
			startOffset = lastOffset;
			if(stopOffset - startOffset > 300) stopOffset = startOffset + 300;
			lastOffset = stopOffset;
		}
		for(unsigned i = startOffset; i < stopOffset; ++i)
		{
			for(int sens = 0; sens < count_sensors; ++sens)
			{
				CalculationOneFrame(//вычисление кадра первичных данных(474, 986)
					sens
					, primaryData.SensorData(sens, i)
					, primaryData.result[sens][i]
				    , primaryData.status[sens][i]    
				);
			}
		}
		bool calculateFrame = startOffset < stopOffset;
		if(calculateFrame)
		{
			Lock lock(cs0);
			Point p = {startOffset, stopOffset};
			list.Add(p);
		}
		if(TryEnterCriticalSection(&cs1))
		{
			LockTry lock(cs1);
			for(;;)
			{
				unsigned start, stop;
				bool calculateZones = false;
				{
					Lock lock(cs0);					
					List<Point>::Item *i = list.items;
					while(NULL != i)
					{
						if(lastFrame == i->data.start)
						{
							start = i->data.start;
							stop = lastFrame = i->data.stop;
							calculateZones = true;
							list.Drop(i);
							break;
						}
						i = i->next;
					}
				}
				if(!calculateZones) return;
				const unsigned offs = filtre[0].length;
				for(unsigned lz = start - offs; lz < stop - offs; ++lz)
				{
					if(primaryData.countZones < dimention_of(primaryData.offsetOfTime) && lz > unsigned(primaryData.offsetOfTime[primaryData.countZones] * frequencyFrames * 0.001))
					{			
						++primaryData.countZones;	
						offsetOfZonesOffset += deltaTime;
						primaryData.offsetOfTime[primaryData.countZones] = offsetOfZonesOffset;//смещение зоны во времени				
						for(int i = 0; i < count_sensors; ++i)
						{
							sensorsData[i].zonesMin[primaryData.countZones] = result_undefined;
							sensorsData[i].zonesMax[primaryData.countZones] = result_undefined;
							sensorsData[i].status[primaryData.countZones] = PrimaryData::Undefined;
							sensorsData[i].countZones = primaryData.countZones;
						}
						thicknessData.zonesMin[primaryData.countZones] = result_undefined;
						thicknessData.zonesMax[primaryData.countZones] = result_undefined;
						thicknessData.status[primaryData.countZones] = PrimaryData::Undefined;
						thicknessData.countZones = primaryData.countZones;
						thicknessViewer.DrawSomeBar();
					}
					//if(deadAreaTime < lz)//изменено 18 11 14 если данные в мёртвой зоне вычисления не производятся
					{
						double &thicknessMin = thicknessData.zonesMin[primaryData.countZones];
						double &thicknessMax = thicknessData.zonesMax[primaryData.countZones];
						char   &thicknessStatus = thicknessData.status[primaryData.countZones];
						for(int i = 0; i < count_sensors; ++i)
						{
							double *data = &primaryData.result[i][lz];
							char *status = &primaryData.status[i][lz];
							int index = (filtre[i])(data);

							SetDataStatus(
								sensorsData[i].zonesMin[primaryData.countZones]
							, sensorsData[i].zonesMax[primaryData.countZones]
							, sensorsData[i].status[primaryData.countZones]
							, status[index]
							, data[index]
							);
							
							SetDataStatus(
								thicknessMin
								, thicknessMax
								, thicknessStatus
								, status[index]
							, data[index]
							);
						}
						if(thicknessMax - thicknessMin > defectDifferentWall) 
						{
								thicknessStatus = PrimaryData::DefectDifferentWall;
						}
					}
				}
			}
		}
		else
		{
			if(!calculateFrame)return;
		}
	}
}
//----------------------------------------------------------------------------------
struct DebugStatus
{
	double &result;
	char &status;
	DebugStatus(double &r, char &s)
		: result(r)
		, status(s)
	{}
	~DebugStatus()
	{
		dprint("result %f status %d", result, status);
		if(0 == status) Sleep(1000);
	}
};

void Compute::CalculationOneFrame(int sensorIndex, char *sensorData, double &result, char &status)
{
	result = result_undefined;
	double data[1024] = {};
	double t = 0;
	int j = 0;
	for(; j < frameSize; ++j)
	{
		data[j] = sensorData[j];
	}
//	t /= frameSize;

	//for(; j < fft.bufferSize; ++j)
	//{
	//	data[j] = t;
//	}
///------------------------------------------
	fft.Direct(data);
	fft.Spectrum(data);	
//--------------------------------------отсечение в частотной области
	ZeroMemory(data, sizeof(double) * acfBorderLeft);
	ZeroMemory(&data[acfBorderRight], sizeof(double) * (fft.bufferSize - acfBorderRight));
//---------------------------------------------------------------------------------------
	fft.Direct(data);
	fft.Spectrum(data);
//------------------------------------------------------------------------------
	if(data[0] < minEnergy[sensorIndex]) 
	{
		status = PrimaryData::LessMinimumEnergy;//отбраковка минимальная энергия
		return;
	}

	/* не удалять
	if(data[0] > maxEnergy[sensorIndex]) 
	{
		status = PrimaryData::GreaterMaximumEnergy;//отбраковка максимальная энергия
		return;
	}
	*/
//-----------------------------------поиск пика	
	int maxOffs = maxOffset[sensorIndex];
	int z = minOffset[sensorIndex];

	double minVal = data[z];
	double maxVal = minVal;
	double val = 0;
	int offsMin = z;
	int offsMax = z;
	int offsVal = z;

	for(; z < maxOffs; ++z)
	{
		if(minVal > data[z]) 
		{
			minVal = data[z];
			offsMin = z;
			if(offsMin > offsMax) 
			{
				minVal = maxVal = data[z];
				continue;
			}
		}
		if(maxVal < data[z])
		{
			maxVal = data[z];
			offsMax = z;
		}
		double t = maxVal - minVal;
		if(val < t) 
		{
			val = t;
			offsVal = z;
		}
	}
	if(val < data[0] * peak[sensorIndex])
	{
		 status = PrimaryData::ValueLessThreshold;//меньше допустимого порога
		 return;
	}
	result = offsVal;
	result *= coefficientA[sensorIndex];
	result += coefficientB[sensorIndex];

	char s = PrimaryData::Nominal;
	if(result > defectBorderMax + 0.1) s = PrimaryData::DefectMax;
	else if(result < defectBorderMin - 0.1) s = PrimaryData::DefectMin;

	status = s;
}
//-----------------------------------------------------------------------
VOID CALLBACK APCProc(_In_  ULONG_PTR dwParam){}

void Compute::Stop()
{
  // EnterCriticalSection(&cs0);
  // stop = dimention_of(hThread);
   //LeaveCriticalSection(&cs0);
}
//---------------------------------------------------------------------------
void Compute::Destroy()
{
	TerminateThread(hThread[0], 0);
	TerminateThread(hThread[1], 0);
}
//-------------------------------------------------------------------------------
struct __Recalculation__
{
	Compute &owner;
	int start, stop;
	__Recalculation__(Compute &o, int start, int stop)
		: owner(o)
	    , start(start)
		, stop(stop)
	{}
	void Do()
	{
		for(int i = start; i < stop; ++i)
		{
			for(int sens = 0; sens < count_sensors; ++sens)
			{
				owner.CalculationOneFrame(//вычисление кадра первичных данных(486, 984)
					sens
					, owner.primaryData.SensorData(sens, i)
					, owner.primaryData.result[sens][i]
				, owner.primaryData.status[sens][i]    
				);
			}
		}
	}
	static void Proc(__Recalculation__ *d){d->Do();}
};

void Compute::Recalculation()
{
	defectBorderMin = nominalThickness * (1 - nominalPercentMin / 100);
	defectBorderMax = nominalThickness * (1 + nominalPercentMax / 100);
	defectDifferentWall = defectDifferentWallPercent * nominalThickness / 100;

	ZeroMemory(&thicknessData, sizeof(thicknessData));
	ZeroMemory(&sensorsData, sizeof(sensorsData));
	ZeroMemory(primaryData.result, sizeof(primaryData.result));
	ZeroMemory(primaryData.status, sizeof(primaryData.status));
	
	int count = primaryData.GetCurrentOffset() / 4;
	int o = 0;
	__Recalculation__ _0(*this, o, o + count);
	o += count;
	__Recalculation__ _1(*this, o, o + count);
	o += count;
	__Recalculation__ _2(*this, o, o + count);
	o += count;
	__Recalculation__ _3(*this, o, primaryData.GetCurrentOffset());

	HANDLE h[] = {
         CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)__Recalculation__::Proc, &_0, 0,NULL)
		 ,  CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)__Recalculation__::Proc, &_1, 0,NULL)
		 ,  CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)__Recalculation__::Proc, &_2, 0,NULL)
	};
	_3.Do();
	WaitForMultipleObjects(dimention_of(h), h, TRUE, INFINITE);

	for(int i = 0; i < dimention_of(h); ++i) CloseHandle(h[i]);

	unsigned offs = 0;
	for(int z = 0; z <= primaryData.countZones && z < dimention_of(primaryData.offsetOfTime); ++z)
	{
		thicknessData.countZones = z;
		thicknessData.zonesMin[z] = result_undefined;
		thicknessData.zonesMax[z] = result_undefined;
		thicknessData.status[z] = PrimaryData::Undefined;
		for(int i = 0; i < count_sensors; ++i)
		{
			sensorsData[i].zonesMin[z] = result_undefined;
			sensorsData[i].zonesMax[z] = result_undefined;
			sensorsData[i].status[z] = PrimaryData::Undefined;
		}
		unsigned last = unsigned(primaryData.offsetOfTime[z] * frequencyFrames * 0.001);
		for(unsigned j = offs; j < last; ++j)
		{
			double &thicknessMin = thicknessData.zonesMin[z];
			double &thicknessMax = thicknessData.zonesMax[z];
			char   &thicknessStatus = thicknessData.status[z];
			for(int i = 0; i < count_sensors; ++i)
			{
				double *data = &primaryData.result[i][j];
				char *status = &primaryData.status[i][j];
				int index = (filtre[i])(data);

				SetDataStatus(
					sensorsData[i].zonesMin[z]
				, sensorsData[i].zonesMax[z]
				, sensorsData[i].status[z]
				, status[index]
				, data[index]
				);

				SetDataStatus(
					thicknessMin
					, thicknessMax
					, thicknessStatus
					, status[index]
				, data[index]
				);				
			}
			if(thicknessMax - thicknessMin > defectDifferentWall) 
			{
					thicknessStatus = PrimaryData::DefectDifferentWall;
			}
		}		
		offs = last;//int(primaryData.offsetOfTime[z] * frequencyFrames * 0.001);
		
		thicknessViewer.Update();
	}
	thicknessData.countZones = primaryData.countZones;
	for(int i = 0; i < count_sensors; ++i)
	{
		sensorsData[i].countZones = primaryData.countZones;
	}
}
//------------------------------------------------------------------------------------
void Compute::UsefulPiecePipeDo()
{
	cuttingZone0 = 0;
	cuttingZone1 = 0;
	int tail = 0;
	int maxLength = 0;
	int i = 0;
	for(; i < primaryData.countZones; ++i)
	{		
		if(thicknessData.status[i] == PrimaryData::DefectMin || thicknessData.status[i] == PrimaryData::DefectMax)
		{
			int len = i - tail;
			if(len > maxLength)
			{
				cuttingZone1 = i; 
				cuttingZone0 = tail;
			}
			tail = i;
		}
	}
	int len = i - tail;
	if(len > maxLength)
	{
		cuttingZone1 = i; 
		cuttingZone0 = tail;
	}
}
//-------------------------------------------------------------------------------------

