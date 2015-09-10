#include "stdafx.h"
#include "PrimaryData.h"
#include "Lan.h"
#include "Debug1730.h"
#include <math.h>
#include "config.h"
#ifdef XDEBUG
#define dprint debug.print
#else
#define dprint
#endif
#ifdef DEBUG_ITEMS
#include <Windows.h>
int start_fill_debug = 0;
struct DebugCS
{
	CRITICAL_SECTION cs;
	DebugCS()
	{
		InitializeCriticalSection(&cs);
	}
	static DebugCS &Instance(){static DebugCS x; return x;};
};
struct FillDebug
{
	int start, stop;
	HANDLE hThread;
	PrimaryData &p;
	static void Proc(FillDebug *t)
	{
		t->Do();
	}
	FillDebug(PrimaryData &p)
		: start(0)
		, stop(0)
		, p(p)
	{
		CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Proc, this, 0, NULL));
	}
	void Do()
	{
		while(true)
		{
			EnterCriticalSection(&DebugCS::Instance().cs);
			start = start_fill_debug;
			stop = start + 200;
			start_fill_debug = stop;
			LeaveCriticalSection(&DebugCS::Instance().cs);
			if(stop > PrimaryData::sensorBuffSize)
			{
				delete this;
				return;
			}
			double dw = 2.0 * 3.14 / 500 * 4;
			double dy = 4.0 / 500;
			int z = start * count_sensors * 474;
			double ddw = dw;
			for(int i = start; i < stop; ++i)
			{				
				for(int j = 0; j < count_sensors; ++j)
				{
					double x = 0;
					ddw *= 1.001;
					double dddw = ddw + 0.02 * j;
					for(int k = 0; k < 474; ++k)
					{
						double e = exp(-x);
						p.buffer[z] = char(sin(0.7 * dddw * k)*cos(4 * dddw * k) * e * 128);
						x += dy;
						++z;
					}
				}
		}
		}
	}
};
#endif
char PrimaryData::buffer[PrimaryData::buffSize];
//----------------------------------------------------------------------------------------------
PrimaryData::PrimaryData()
	: bufSize(Lan::bufSize)
	, frameSize(Singleton<LanParametersTable>::Instance().items.get<PacketSize>().value)
{
////////////debug
#pragma message("заполнение для отладки--------------------------------------------")
//	if(false)
//	{
		/*
		double dw = 2.0 * 3.14 / 500 * 4;
		double dy = 4.0 / 500;
		int z = 0;
		for(int i = 0; i < sensorBuffSize; ++i)
		{
			double ddw = dw;
			for(int j = 0; j < count_sensors; ++j)
			{
				double x = 0;
				ddw *= 1.3;
				for(int k = 0; k < 474; ++k)
				{
					double e = exp(-x);
					buffer[z] = char(sin(0.7 * dw * k)*cos(4 * dw * k) * e * 128);
					x += dy;
					++z;
				}
			}
		}
		*/
		
//	}
#ifdef DEBUG_ITEMS
	/*
	new FillDebug(*this);
	new FillDebug(*this);
	new FillDebug(*this);
	new FillDebug(*this);
	*/
#endif
///debug
}
//---------------------------------------------------------------------------------------------
void PrimaryData::Clear()
{
	current__ = 0;
	ZeroMemory(status, sizeof(status));
	ZeroMemory(result, sizeof(result));
}
//---------------------------------------------------------------------------------------------
char *PrimaryData::CurrentData()
{
	return &buffer[current__ * bufSize];
}
//---------------------------------------------------------------------------------------------
bool PrimaryData::IncrementOffset()
{
	if(((1 + current__) * bufSize) < buffSize)
	{
		++current__;
		//dprint("current %d", current__);
		return true;
	}
	//dprint("current %d  false", current__);
	return false;
}
//---------------------------------------------------------------------------------------------
char *PrimaryData::SensorData(int sensor, int offset)
{
	return &buffer[(offset * count_sensors + sensor) * frameSize];
}
//-------------------------------------------------------------------------------------
int PrimaryData::GetCurrentOffset() const
{
	return current__ * Singleton<LanParametersTable>::Instance().items.get<NumberPackets>().value;
}
//-------------------------------------------------------------------------------------
void PrimaryData::SetCurrentOffset(int d)
{
	//current__ = d / bufSize;// / Singleton<LanParametersTable>::Instance().items.get<NumberPackets>().value;
	int bufSize = Singleton<LanParametersTable>::Instance().items.get<NumberPackets>().value 
	   * Singleton<LanParametersTable>::Instance().items.get<PacketSize>().value * count_sensors;
	current__ = d / bufSize;
}
//--------------------------------------------------------------------------------------
int PrimaryData::Filling()
{
  return current__ * Singleton<LanParametersTable>::Instance().items.get<NumberPackets>().value 
	   * Singleton<LanParametersTable>::Instance().items.get<PacketSize>().value * count_sensors;
}
//----------------------------------------------------------------------------------------
PrimaryData primaryData;