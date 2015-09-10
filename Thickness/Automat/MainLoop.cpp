#include "stdafx.h"
#include "MainLoop.h"
#include <Mmsystem.h>
#include "Automat.h"
#include "Device1730.h"
#include "Lan.h"
#include "PaintMaker.h"
#include "ArchiveEvents.h"
#include "ProcMainLoop.h"
#include "DebugMess.h"
#include "MainWindowToolBar.h"
#include "MainWindow.h"
#include "TopLabelViewer.h"
#ifdef XDEBUG
#define dprint debug.print
#else
#define dprint
#endif
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
MainLoop::MainLoop()
	: hThread(NULL)
	, loop(true)
	, input(archiveEvents.map->input)
	, output(archiveEvents.map->output)
	, proc(NoopProc)
{
	memset(hEvent, 0, sizeof(hEvent));
}
//-------------------------------------------------------------------------------------
void MainLoop::Init()
{
	if(NULL == hEvent[0]) hEvent[0] = CreateEvent(NULL, false, false, LAN_DEVICE_FRAME_READY);
	if(NULL == hEvent[1]) hEvent[1] = CreateEvent(NULL, false, false, LAN_DEVICE_TIME_OUT);

	if(NULL == hThread)
	{
		hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Proc, this, CREATE_SUSPENDED, NULL);
#ifdef THREAD_PRIORITY
		SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST);
#endif
		ResumeThread(hThread);
	}
}
//-------------------------------------------------------------------------------------
void MainLoop::Destroy()
{
	loop = false;
	if(NULL != hThread) //WaitForSingleObject(hThread, 10000);
		TerminateThread(hThread, 0);

}
//-------------------------------------------------------------------------------------
void MainLoop::Loop()
{
	while(loop)
	{
		if((1 + WAIT_OBJECT_0) == WaitForMultipleObjects(2, hEvent, false, 10))//todo если сработал LAN_DEVICE_TIME_OUT
		{
			//currentAutomat->Reset();
			topLabelViewer.SetMessage(L"<ff0000>Нет синхроимпульсов");
			proc = NoopProc;
			Enanle3Button(NULL);
		}
		proc(input, output);//если сработало LAN_DEVICE_FRAME_READY
#ifdef XDEBUG
		static unsigned tme = 0;
		unsigned t = GetTickCount();
		//dprint("MainLoop delay %d", t - tme);
		tme = t;
#endif		
	}
}
//---------------------------------------------------------------------------------------
void MainLoop::Proc(MainLoop *d)
{
#if 1
	d->Loop();
#else
	while(d->loop)
	{
		/*
		unsigned inputBits = device1730.Read();
		unsigned outputBits = automat.Do(inputBits);
		outputBits |= paintMaker.Do();
		device1730.Write(outputBits);
		*/
		d->input = device1730.Read();
		d->output = automat.Do(d->input);
		d->output |= paintMaker.Do();
		device1730.Write(d->output);
	}
#endif
}
//-----------------------------------------------------------------------------------
void MainLoop::SetLoop(void (*p)(unsigned &, unsigned &))
{
	proc = p;
}
//----------------------------------------------------------------------
