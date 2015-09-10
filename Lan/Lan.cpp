#include "stdafx.h"
#include "Lan.h"
#include <windows.h>

#include "PrimaryData.h"

#include "IRshDevice.h"
#include "RshDllClient.h"
#include "RshError.h"
#include "config.h"
#include <MMSystem.h>
#include "Automat.h"

#include "DebugMess.h"
#ifdef XDEBUG
#define dprint debug.print
#else
#define dprint
#endif
//-----------------------------------------------------------------------------------
#define BOARD_NAME "LAn10M8PCI"
namespace Lan
{	
//-----------------------------------------------------------------------------------
size_t ___bufSize___;
const size_t &bufSize = ___bufSize___;
//-----------------------------------------------------------------------------------
struct LanBuffer: public RshBaseType
{
	char *ptr;
	size_t size;
	size_t psize;
	LanBuffer()
	{
		_type = rshBufferTypeS8;//повторение кода платы лан10....(не изменять)
		_typeSize = 20;
	}
};
struct LanDevice
{
	int num;
	CRITICAL_SECTION &cs;
	HANDLE hThread;
	HANDLE hEventTimeOut;
	HANDLE hFrameReady;
	IRshDevice* device; 
	unsigned status;
	bool &loop;
	unsigned Init(RshDllClient &, int);
	void Do();
	unsigned SetParams();
	static void Proc(LanDevice *d){d->Do();}
	LanDevice(CRITICAL_SECTION &cs, bool &loop) : cs(cs), loop(loop), hThread(NULL){}
};
struct Inner
{
	CRITICAL_SECTION cs;
	RshDllClient client;
	LanDevice device1, device2;
	bool loop;
	Inner()
		: device1(cs, loop)
		, device2(cs, loop)
		, loop(true)
	{
		InitializeCriticalSection(&cs);
	}
	static Inner &Instance(){static Lan::Inner x; return x;}
};
//--------------------------------------------------------------------------------------
unsigned LanDevice::SetParams()
{
	RshInitMemory  p; 
	p.startType = RshInitMemory::External;
	p.controlSynchro = 0;//Синхронизация по фронту
	if(Singleton<LanParametersTable>::Instance().items.get<SynchronizationEdge>().value)p.controlSynchro = RshInitMemory::SlopeDecline;
	p.bufferSize = Singleton<LanParametersTable>::Instance().items.get<PacketSize>().value;
	p.frequency = 1e6 * Singleton<LanParametersTable>::Instance().items.get<Frequency>().value;
	p.channels[0].control = RshChannel::Used; 
	if(Singleton<LanParametersTable>::Instance().items.get<MeasurementInput>().value)p.channels[0].control |= RshChannel::AC;
	p.channels[0].gain = Singleton<LanParametersTable>::Instance().items.get<Gain0>().value;
	
	p.packetNumber = Singleton<LanParametersTable>::Instance().items.get<NumberPackets>().value * count_sensors;

	p.channelSynchro.gain = Singleton<LanParametersTable>::Instance().items.get<SyncGain>().value;
	p.channelSynchro.control = RshSynchroChannel::DC;
	if(Singleton<LanParametersTable>::Instance().items.get<SyncInput>().value)p.channelSynchro.control = RshSynchroChannel::AC;
	p.threshold = Singleton<LanParametersTable>::Instance().items.get<SyncLevel>().value;
	p.startDelay = Singleton<LanParametersTable>::Instance().items.get<StartDelay>().value;
#ifndef DEBUG_ITEMS
	return device->Init(&p);
#else
	return RSH_API_SUCCESS;
#endif
}
//--------------------------------------------------------------------------------------
unsigned LanDevice::Init(RshDllClient &client, int num)
{
	this->num = num;
	hEventTimeOut = CreateEvent(NULL, false, false, LAN_DEVICE_TIME_OUT);
	hFrameReady   = CreateEvent(NULL, false, false, LAN_DEVICE_FRAME_READY);
	unsigned st = RSH_API_SUCCESS;
#ifndef DEBUG_ITEMS
	RshDllInterfaceKey DEV_KEY(BOARD_NAME,(void**)&device); 
	st = client.GetDeviceInterface(DEV_KEY); 
	if(st != RSH_API_SUCCESS) return st;
	RshDeviceKey key(num);
	st = device->Connect(&key);
	if(st != RSH_API_SUCCESS) return st;

	st = SetParams();
	if(st != RSH_API_SUCCESS) return st;
#endif
	if(NULL == hThread)
	{
			hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Proc, this, CREATE_SUSPENDED, NULL);
#ifdef THREAD_PRIORITY
			SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST);
#endif
	}
	return st;
}
//------------------------------------------------------------------------------------
#ifndef DEBUG_ITEMS
//unsigned last = 0;
void LanDevice::Do()
{	
	RSH_U32 waitTime = 5000;
	LanBuffer buf;
	while(loop)
	{
		status = device->Start();
		unsigned res = device->Get(RSH_GET_WAIT_BUFFER_READY_EVENT, &waitTime );
		if(res == RSH_API_SUCCESS ) 
		{   
			device->Stop();
			EnterCriticalSection(&cs);
			buf.ptr = (char *)currentAutomat->primaryData.CurrentData();
			buf.psize = bufSize;
			currentAutomat->primaryData.IncrementOffset();
			LeaveCriticalSection(&cs);
			status = device->GetData(&buf);
			SetEvent(hFrameReady);
		//	unsigned t = timeGetTime();
		//	dprint("frame ok %d  %x %d %d cur %d", res, res, t - last, num, currentAutomat);
		//	last = t;
		}
		else
		{
			SetEvent(hEventTimeOut);
			dprint("hEventTimeOut %d  %x %d", res, res, num);
		//	wchar_t err[128];
		//	RshError::GetErrorDescriptionUTF16(res, err, 128, RSH_LANGUAGE_RUSSIAN);
		//	dprint("time out d=%d  h=%x", res, res);
		}
		
	}
}
#else
void LanDevice::Do()
{	
	while(loop)
	{
		Sleep(10);
		EnterCriticalSection(&cs);
		currentAutomat->primaryData.IncrementOffset();
		LeaveCriticalSection(&cs);
		SetEvent(hFrameReady);
	}
}
#endif
//----------------------------------------------------------------------------------------
unsigned Init()
{
	Lan::Inner &i = Lan::Inner::Instance();
	if(RSH_API_SUCCESS != i.device1.Init(i.client, 1)) return 1;
	if(RSH_API_SUCCESS != i.device2.Init(i.client, 2)) return 2;
	___bufSize___ = count_sensors * Singleton<LanParametersTable>::Instance().items.get<NumberPackets>().value
		* Singleton<LanParametersTable>::Instance().items.get<PacketSize>().value;
	return 0;
}
//-------------------------------------------------------------------------------------
void Destroy()
{
	Lan::Inner &i = Lan::Inner::Instance();
	i.device1.loop = false;
	i.device2.loop = false;

	//WaitForSingleObject(i.device1.hThread, 300);
	//WaitForSingleObject(i.device2.hThread, 300);
	TerminateThread(i.device1.hThread, 0);
	TerminateThread(i.device2.hThread, 0);
}
//-------------------------------------------------------------------------------------
unsigned SetParams()
{
	Lan::Inner &i = Lan::Inner::Instance();

	unsigned st = i.device1.SetParams();
	if(st != RSH_API_SUCCESS) return st;
	return i.device2.SetParams();
}
//---------------------------------------------------------------------------------
void Start()
{
	___bufSize___ = Singleton<LanParametersTable>::Instance().items.get<NumberPackets>().value
		* Singleton<LanParametersTable>::Instance().items.get<PacketSize>().value * count_sensors;
	Lan::Inner &i = Lan::Inner::Instance();
	dprint("Lan Start  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	while(0 < ResumeThread(i.device1.hThread));
	while(0 < ResumeThread(i.device2.hThread));
}
//------------------------------------------------------------------------------------
void Stop()
{
	Lan::Inner &i = Lan::Inner::Instance();
	SuspendThread(i.device1.hThread);
	SuspendThread(i.device2.hThread);
	dprint("Lan Stop <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
#ifndef DEBUG_ITEMS
	i.device1.device->Stop();
	i.device2.device->Stop();
#endif
}
//------------------------------------------------------------------------------------
}
