#include "stdafx.h"
#include "MenuItems.h"
#include "Controller.h"
#include "compute.h"
#include "TopLabelViewer.h"
#include "Lan.h"
#include "PrimaryData.h"
#include "FFT.h"
#include "ThicknessViewer.h"
#include "EmptyWindow.h"
#include "MainWindow.h"
#include "device1730.h"
#include "Automat.h"
#include "MainLoop.h"
#include "ProcMainLoop.h"
#include "Bos.h"
#include "ArchiveEvents.h"


#ifdef XDEBUG
#define xprint debug.print(__FUNCTION__);
#define dprint debug.print
#define d_mess(x)
#define x_mess debug.print
#else
#define xprint
#define dprint
#endif

bool RecalculationBool = false;
static void RecalculationThreadProc(VOID *)
{
	topLabelViewer.SetMessage(L"<ff0000>Идёт перерасчёт");
	currentAutomat->compute.MeshuringBaseStart();
	currentAutomat->compute.InitParam();
	currentAutomat->compute.Recalculation();
	topLabelViewer.SetMessage(L"");//<ff00>Перерасчёт закончен");
	RecalculationBool = false;
}

void Recalculation::Do(HWND )
{
	if(!RecalculationBool)
	{
		RecalculationBool = true;
		CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RecalculationThreadProc, NULL, 0, NULL));
	}
}
//--------------------------------------------------------------------------------------------------------
bool TestLanBool = false;
static void TestLanThreadProc(VOID *)
{
#if 1	
	bos.PowerOn();
	mainLoop.SetLoop(TestDataProc);
	topLabelViewer.SetMessage(L"<ff0000>Тестирование датчиков");	
	currentAutomat->primaryData.Clear();
	ZeroMemory(&currentAutomat->compute.thicknessData, sizeof(currentAutomat->compute.thicknessData));
	ZeroMemory(&currentAutomat->compute.sensorsData, sizeof(currentAutomat->compute.sensorsData));
	thicknessViewer.Update();
	Lan::Start();
	bos.On();
	currentAutomat->compute.MeshuringBaseStart();
	for(int i = 0; i < 50; ++i)
		if(WAIT_OBJECT_0 == WaitForSingleObject(currentAutomat->hEventStop, 100))
		{
			mainLoop.SetLoop(NoopProc);
			topLabelViewer.SetMessage(L"");
			mainWindow.BottomLabel(0, L"");
			TestLanBool = false;
			bos.PowerDown();
			Lan::Stop();
			return;
		}
	currentAutomat->compute.MeshuringBaseStop1();
	for(int i = 0; i < 100; ++i)
		if(WAIT_OBJECT_0 == WaitForSingleObject(currentAutomat->hEventStop, 100))
		{
			mainLoop.SetLoop(NoopProc);
			topLabelViewer.SetMessage(L"");
			mainWindow.BottomLabel(0, L"");
			TestLanBool = false;
			bos.PowerDown();
			Lan::Stop();
			return;
		}
	bos.PowerDown();
	unsigned &output = archiveEvents.map->output;
	device1730.Write(output);
	Lan::Stop();

	RepaintWindow(thicknessViewer.hWnd);	
	
	mainLoop.SetLoop(NoopProc);
	topLabelViewer.SetMessage(L"");
	mainWindow.BottomLabel(0, L"");
	TestLanBool = false;
#else
  unsigned t = Singleton<OutputsTable>::Instance().items.get<PowerBitOut>().value;
  device1730.Write(t);
  Sleep(3000);
 
  t |= Singleton<OutputsTable>::Instance().items.get<StartBitOut>().value;
  device1730.Write(t);
   Sleep(2000);
   device1730.Write(0);
#endif
}
//-----------------------------------------------------------------------------
void TestLan::Do(HWND )
{
	if(!TestLanBool)
	{
		TestLanBool = true;
		CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TestLanThreadProc, NULL, 0, NULL));
	}
}

