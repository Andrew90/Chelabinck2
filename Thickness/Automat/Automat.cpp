#include "stdafx.h"
#include <Mmsystem.h>
#include "Automat.h"
#include "SQEvents.h"
#include <windows.h>
#include "TopLabelViewer.h"
#include "BitsToTitle.hpp"
#include "ArchiveEvents.h"
#include "Controller.h"
#include "Lan.h"
#include "Compute.h"
#include "PrimaryData.h"
#include "windows.h"
#include "ThicknessData.h"
#include "PaintMaker.h"
#include "ThicknessViewer.h"
#include "EmptyWindow.h"
#include "MainWindowToolBar.h"
#include "MainLoop.h"
#include "ProcMainLoop.h"
#include "Bos.h"
#include "MainWindow.h"
#include "StoredData.h"
#include "Config.h"
#include "DialogBeforeCycle.h"
#include "ParametersCycle.h"


#include "DebugMess.h"
#ifdef XDEBUG
#define dprint debug.print
#else	
#define  dprint
#endif	
//------------------------------------------------------------------
CRITICAL_SECTION cs;
Automat automat[2];
struct Lock
{
	CRITICAL_SECTION &cs;
	Lock(CRITICAL_SECTION &cs) : cs(cs)
	{
		EnterCriticalSection(&cs);
	}
	~Lock()
	{
		LeaveCriticalSection(&cs);
	}
};

//------------------------------------------------------------
#define SINGLE_MODE_WaitFor(name, OnOff)\
{\
	dprint("before %s", #name);\
	HANDLE hEvents[] = {\
	  sQEvents.items.get<SQEvents::Event<SQEvents::OnOff<name> > >().hEvent\
	  , hEventStop\
    };\
	ResetEvent(hEvents[0]);\
  switch(WaitForMultipleObjects(2, hEvents, FALSE, 10 * 60 * 1000))\
  {\
  case WAIT_OBJECT_0: dprint(#name);break;\
  case 1 + WAIT_OBJECT_0:\
    {\
	   Lock lock(cs);\
	   workMode = eNone;\
	   bos.Off();\
	   Lan::Stop();\
    }\
	goto MAIN_LOOP;\
  case WAIT_TIMEOUT:\
      {\
	    Lock lock(cs);\
		topLabelViewer.SetMessage(timeExceeded);\
	    workMode = eNone;\
	    bos.Off();\
		Lan::Stop();\
		dprint("timeout");\
      }\
	goto MAIN_LOOP;\
  }\
}

#define SINGLE_MODE_WaitForNoSubType(name)\
{\
	dprint("before %s", #name);\
	HANDLE hEvents[] = {\
	  sQEvents.items.get<SQEvents::Event<SQEvents::name> >().hEvent\
	  , hEventStop\
    };\
	ResetEvent(hEvents[0]);\
  switch(WaitForMultipleObjects(2, hEvents, FALSE, 10 * 60 * 1000))\
  {\
  case WAIT_OBJECT_0: dprint(#name);break;\
  case 1 + WAIT_OBJECT_0:\
    {\
	   Lock lock(cs);\
	   workMode = eNone;\
	   bos.Off();\
	   Lan::Stop();\
    }\
	goto MAIN_LOOP;\
  case WAIT_TIMEOUT:\
      {\
	    Lock lock(cs);\
		topLabelViewer.SetMessage(timeExceeded);\
	    workMode = eNone;\
	    bos.Off();\
		Lan::Stop();\
      }\
	goto MAIN_LOOP;\
  }\
}

#define SINGLE_MODE_TEXT(name, text) template<>struct Text<name>{wchar_t *operator()(){return text;}};

namespace SingleModeSpace
{
wchar_t *timeExceeded = L"<ff0000>Превышено максимальное время измерения";
wchar_t *userInterruptedWork = L"<ff0000>Оператор прервал работу установки";
wchar_t *modeSingleMeasurement = L"<ff0000>Установка находится в режиме одиночного измерения";
wchar_t *modeAutomaticMeasurement = L"<ff0000>Установка находится в режиме циклического измерения";
wchar_t *modeAutomaticWaitingPipe = L"<ff>Автоматический режим. Ожидание трубы";
wchar_t *modeSingleWaitingPipe = L"<ff>Одиночный режим. Ожидание трубы";
wchar_t *computeComplected = L"<ааff00>Вычисление завершено";
wchar_t *pipeOut = L"<55ddff>Труба вышла из установки";
wchar_t *resetText = L"";//<ff>Выход из режима измерений";
wchar_t *collectionDate = L"<ff>Сбор данных";

typedef TL::MkTlst<
	//	  TestStateSpace::bit<PipeAfterClamp1SQ1BitIn    , false>
		  TestStateSpace::bit<Clamp1RetractedSQ2BitIn    , true>
		, TestStateSpace::bit<PipeInletSQ3BitIn          , false>
		, TestStateSpace::bit<OutletPipeSQ4BitIn         , false>
		, TestStateSpace::bit<SensorUnitRetractedSQ5BitIn, false>
		, TestStateSpace::bit<SensorsDivorcedSQ6BitIn    , true> //проверить отведены д
	//	, TestStateSpace::bit<PipeClamp2BeforeSQ7BitIn   , false>
	//	, TestStateSpace::bit<Clamp2RetractedSQ8BitIn    , true>
		, TestStateSpace::bit<NormalAirPressurePaBitIn   , true>
	>::Result start_single_state_list;

template<class T>struct Text;
SINGLE_MODE_TEXT(PipeAfterClamp1SQ1BitIn    , L"<ff0000>Ошибка! Сработал датчик \"Труба после прижима 1(SQ1)\"")
SINGLE_MODE_TEXT(Clamp1RetractedSQ2BitIn    , L"<ff0000>Ошибка! Не сработал датчик \"Прижим 1 отведён(SQ2)\" ")
SINGLE_MODE_TEXT(PipeInletSQ3BitIn          , L"<ff0000>Ошибка! Сработал датчик \"Труба на входе СУ(SQ3)\" ")
SINGLE_MODE_TEXT(OutletPipeSQ4BitIn         , L"<ff0000>Ошибка! Сработал датчик \"Труба на выходе СУ(SQ4)\" ")
SINGLE_MODE_TEXT(SensorUnitRetractedSQ5BitIn, L"<ff0000>Ошибка! Сработал датчик \"Блок датчиков отведён(SQ5)\" ")
SINGLE_MODE_TEXT(SensorsDivorcedSQ6BitIn    , L"<ff0000>Ошибка! Не сработал датчик \"Датчики отведены(SQ6)\" ")
SINGLE_MODE_TEXT(PipeClamp2BeforeSQ7BitIn   , L"<ff0000>Ошибка! Сработал датчик \"Датчики после прижима 2(SQ7)\" ")
SINGLE_MODE_TEXT(Clamp2RetractedSQ8BitIn    , L"<ff0000>Ошибка! Не сработал датчик \"Прижим 2 отведён(SQ8)\" ")
SINGLE_MODE_TEXT(NormalAirPressurePaBitIn   , L"<ff0000>Ошибка! Не сработал датчик \"Давление воздуха в норме\" ")

class ErrorMessage
{
	unsigned input;	
	template<class O, class P> struct __inner__
	{
		bool operator()(O *o, P *p)
		{
			if((0 != (p->input & o->offset)) != o->on)
			{
				p->text = Text<typename O::type>()();
				return false;
			}
			return true;
		}
	};
public:
	wchar_t *text;
	template<class Items>void operator()(Items &items, unsigned val)
	{
		input = val;
		text = L"";
		TL::find<TL::Inner<Items>::Result, __inner__>()(&items, this);
	}
};
}
#pragma message("Добавить параметр смещение датчика SQ3 вход в блок датчиков")
Automat::Automat()
	: input(archiveEvents.map->input)
	, output(archiveEvents.map->output)
	//, referenceOffseSQ3(300)
	, compute(primaryData)
	, distanceClampSensor(Singleton<AdditionalSettingsTable>::Instance().items.get<SupplySensorDelay>().value)
{}
void Automat::Init()
{
	hEventSingle = CreateEvent(NULL, FALSE, FALSE, L"hEventSingle");
	hEventAutomatic = CreateEvent(NULL, FALSE, FALSE, L"hEventAutomatic");
	hEventStop = CreateEvent(NULL, TRUE, FALSE, L"hEventStop");
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Automat::Proc, this, CREATE_SUSPENDED, NULL);
	SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST);
	ResumeThread(hThread);
}
void Automat::Destroy()
{
	TerminateThread(hThread, 0);
}
//-----------------------------------------------------------------------------
void AutomatCollectDataSpeedPipeText(wchar_t *buf, int speed)
{
	wsprintf(&buf[wcslen(buf)], L"%d ", speed);
	SendMessage(mainWindow.hStatusWindow, SB_SETTEXT, 1, (LONG)buf); 
}
//----------------------------------------------------------------------------------
struct SQEvents::Event<SQEvents::On<PipeOnMarkerSQ9BitIn> >::Inner
{		
	Automat &automat;
	double &speed;	
	void(Inner::*ptr)();
	wchar_t(&bufferSpeed)[512];
	Inner(Automat &a, double &s, wchar_t(&b)[512])
		: automat(a)
		, speed(s)
		, ptr(&Inner::Noop)
		, bufferSpeed(b)
	{}
	void Start()
	{
		ptr = &Inner::ChangeBit;
	}
	void Noop(){}
	void ChangeBit()
	{
		ptr = &Inner::Noop;

		automat.compute.MeshuringBaseStop2();

		paintMaker.Start(automat.compute.deltaTimeZoneStop2
			, automat.compute.primaryData.offsetOfTime
			, currentAutomat->compute.thicknessData.status			
			);
		speed = (1000.0 * automat.compute.referenceOffset2 /(automat.compute.deltaTimeZoneStop2 - automat.compute.deltaTimeZoneStart));
		AutomatCollectDataSpeedPipeText(bufferSpeed, (int)speed);
	}
};
void SQEvents::Event<SQEvents::On<PipeOnMarkerSQ9BitIn> >::Do()
{
	if(inner)(inner->*inner->ptr)();
}
//----------------------------------------------------------------------------------
void Automat::Do()
{
	using namespace SingleModeSpace;
	workMode = eNone;
	HANDLE h[] = {hEventSingle, hEventAutomatic};
	double speed = 0;
	wchar_t bufferSpeed[512];
	SQEvents::Event<SQEvents::On<PipeOnMarkerSQ9BitIn> >::Inner inner(*this, speed, bufferSpeed);
	sQEvents.items.get<SQEvents::Event<SQEvents::On<PipeOnMarkerSQ9BitIn > > >().inner = &inner;
	for(;;)
	{		        
MAIN_LOOP:
		dprint("MAIN_LOOP");

		ResetEvent(automat[1].hEventStop);
		switch(WaitForMultipleObjects(2, h, FALSE, 20 * 60 * 1000))
		{
		case WAIT_OBJECT_0: 
			topLabelViewer.SetMessage(modeSingleWaitingPipe);
			{
				Lock lock(cs);
				workMode = eSingleMode;
			}
			break;
		case 1 + WAIT_OBJECT_0: 
			topLabelViewer.SetMessage(modeAutomaticWaitingPipe);
			{
				Lock lock(cs);
				workMode = eAutomaticMode;
			}
			break;
			case WAIT_TIMEOUT:
				{
					Lock lock(cs);
					topLabelViewer.SetMessage(L"");
					Enanle3Button(NULL);
					workMode = eNone;
					bos.PowerDown();
					Lan::Stop();
				}
				goto MAIN_LOOP;
		}
				
		
		TestStateSpace::TestState<start_single_state_list> testState;
#pragma message("Востановить после отладки")
	//	/*
		if(!testState.IsOk(input))
		{
			ErrorMessage e;
			e(testState.items, input);
			workMode = eNone;
			topLabelViewer.SetMessage(e.text);
			continue;
		}
	//	*/

		//SQ1------------------------------------------------------------------------------------------------
		bos.PowerOn();//питание бос
	//	sQEvents.ResetBit<SQEvents::Event<SQEvents::On<PipeAfterClamp1SQ1BitIn> > >();
	//	SINGLE_MODE_WaitFor(PipeAfterClamp1SQ1BitIn, On);
		
		//SQ3------------------------------------------------------------------------------------------------		
		SINGLE_MODE_WaitFor(PipeInletSQ3BitIn, On);
		topLabelViewer.SetMessage(collectionDate);
#pragma message("Проверить SensorsRemovalY3BitOut")
	//	output |= Singleton<OutputsTable>::Instance().items.get<SensorsRemovalY3BitOut>().value;	//отвод датчиков 

		currentAutomat = this;
		unsigned timeSQ3 = timeGetTime();
		primaryData.Clear();
	
		ZeroMemory(&compute.thicknessData, sizeof(currentAutomat->compute.thicknessData));
		ZeroMemory(&compute.sensorsData, sizeof(currentAutomat->compute.sensorsData));
		
		thicknessViewer.Update();
				
		bufferSpeed[0] = 0;
		
	    //SQ4-----смыкание датчиков-------------------------------------------------------------------------------------------
		SINGLE_MODE_WaitFor(OutletPipeSQ4BitIn, On);
		unsigned timeSQ4 = timeGetTime();
		compute.MeshuringBaseStart();
		Lan::Start();
		bos.On();//стробы бос
		speed = 1000.0 * 150 / (timeSQ4 - timeSQ3);
		unsigned sleep = unsigned(
			1000.0 
			* Singleton<AdditionalSettingsTable>::Instance().items.get<SupplySensorDelay>().value 
			 / speed);//задержка смыкания
		Sleep(sleep);

		output |= Singleton<OutputsTable>::Instance().items.get<SensorsSupplyY4BitOut>().value;	//подвод датчиков

		//SQ7----------------------------------------------------------------------------------------------
		SINGLE_MODE_WaitFor(PipeClamp2BeforeSQ7BitIn, On);
		unsigned timeSQ7 = timeGetTime();
		compute.MeshuringBaseStop1();
		speed = (1000.0 * compute.referenceOffset1 /(compute.deltaTimeZoneStop1 - compute.deltaTimeZoneStart));
		AutomatCollectDataSpeedPipeText(bufferSpeed, (int)speed);
		dprint("time base SQ3-SQ7 %d", timeSQ7 - timeSQ3);
		//SQ9----------------------------------------------------------------------------------------------
		/*
		SINGLE_MODE_WaitFor(PipeOnMarkerSQ9BitIn, On);
		unsigned timeSQ9 = timeGetTime();
		compute.MeshuringBaseStop2();

		paintMaker.Start(compute.deltaTimeZoneStop2
			, compute.primaryData.offsetOfTime
			, currentAutomat->compute.thicknessData.status			
			);
		speed = (1000.0 * compute.referenceOffset2 /(compute.deltaTimeZoneStop2 - compute.deltaTimeZoneStart));
		AutomatCollectDataSpeedPipeText(bufferSpeed, (int)speed);
		dprint("time base SQ3-SQ9 %d", timeSQ9 - timeSQ3);
		*/
		inner.Start();
		//notSQ1------------------------------------------------------------------------------
		SINGLE_MODE_WaitFor(PipeAfterClamp1SQ1BitIn, Off);
		unsigned timeNotSQ1 = timeGetTime();
		//notSQ3------------------------------------------------------------------------------
		SINGLE_MODE_WaitFor(PipeInletSQ3BitIn, Off);// размыкание датчиков
		speed = 1000.0 * compute.referenceOffset3 / (timeGetTime() - timeNotSQ1);
		//paintMaker.offsetTime = speed * zone_length;
		AutomatCollectDataSpeedPipeText(bufferSpeed, (int)speed);
		sleep = unsigned(
			1000.0 
			* Singleton<AdditionalSettingsTable>::Instance().items.get<RemoveSensorDelay>().value 
			 / speed);//задержка размыкания
		Sleep(sleep);
		dprint("<<<<<<<<<<CLAMP OFFFF sleep %d", sleep);
//#ifdef CLAMP_WORK		
		output |= Singleton<OutputsTable>::Instance().items.get<SensorsRemovalY3BitOut>().value;
//#endif
		///*
		sleep = unsigned(
			1000.0 
			* Singleton<DeadAreaTable>::Instance().items.get<DeadAreaMM1>().value 
			 / speed);//задержка размыкания
		Sleep(sleep);//для мёртвой зоны в конце трубы
		//*/
		//SINGLE_MODE_WaitFor(OutletPipeSQ4BitIn, Off);
		compute.StopMeshuring();
		{
			Lock lock(cs);
			bos.Off();
			Lan::Stop();
		}
		//Вычисление данных завершено------------------------------------------------------
		compute.EndAdjustmentsTube();
		{
			Lock lock(cs);
			if(eAutomaticMode == workMode)
			{
				Stored().Do();
				SetEvent(hEventAutomatic);
			}
		}
		topLabelViewer.SetMessage(computeComplected);
		//NotSQ7----------------------------------------------------------------------------------------------
		SINGLE_MODE_WaitFor(PipeClamp2BeforeSQ7BitIn, Off);
		speed = 1000.0 * compute.referenceOffset4 / (timeGetTime() - timeNotSQ1);
		AutomatCollectDataSpeedPipeText(bufferSpeed, (int)speed);
		//NotSQ9----------------------------------------------------------------------------------------------
		SINGLE_MODE_WaitFor(PipeOnMarkerSQ9BitIn, Off);
		RepaintWindow(thicknessViewer.hWnd);	
		paintMaker.Stop();
		
		{
			Lock lock(cs);
			if(workMode == eSingleMode)
			{
				Enanle3Button(NULL);
				mainLoop.SetLoop(NoopProc);
				topLabelViewer.SetMessage(pipeOut);				
			}
			if(eAutomaticMode == workMode)
			{
				DialogBeforeCycle::Test();//В режиме АВТОМАТ считает количество обработанных труб
			}
			workMode = eNone;
		}		
	}
}
void Automat::Proc(Automat *automat)
{
	automat->Do();
}
//---------------------------------------------------------------------------------------------
void Automats::Init()
{
	InitializeCriticalSection(&cs);
	currentAutomat = &automat[0];
	automat[0].num = 0;
	automat[0].Init();	
	automat[1].num = 1;
	automat[1].Init();	
}	
//----------------------------------------------------------------------------------------------
void Automats::Destroy()
{
	automat[0].Destroy();
	automat[1].Destroy();
	DeleteCriticalSection(&cs);
}
//--------------------------------------------------------------------------------------------
bool Automats::StartAutomatic()
{
	{
		Lock lock(cs);
		if(automat[0].eNone == automat[0].workMode && automat[1].eNone == automat[1].workMode)
		{
			sQEvents.Reset();
			SetEvent(automat[0].hEventAutomatic);
			return true;
		}
	}
	return false;
}
//---------------------------------------------------------------------------------------------------
bool Automats::StartSingle()
{
	{
		Lock lock(cs);
		if(automat[0].eNone == automat[0].workMode && automat[1].eNone == automat[1].workMode)
		{
			sQEvents.Reset();
			SetEvent(automat[0].hEventSingle);
			return true;
		}
	}
	return false;
}
//----------------------------------------------------------------------------------------------
void Automats::Stop()
{
	bos.PowerDown();
	PulseEvent(automat[0].hEventStop);
	topLabelViewer.SetMessage(SingleModeSpace::resetText);
}
//------------------------------------------------------------------------------------------------
#undef SINGLE_MODE_WaitFor
#undef SINGLE_MODE_TEXT

Automat *currentAutomat;