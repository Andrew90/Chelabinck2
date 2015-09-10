#include "stdafx.h"
#include "MainWindowToolBar.h"
#include "MainWindow.h"
#include "DetailedWindow.h"
#include "EmptyWindow.h"
#include "Automat.h"
#include "Device1730.h"
#include "MenuItems.h"
#include "ThicknessViewer.h"
#include "ProcMainLoop.h"
#include "MainLoop.h"
#include "Dialogs.h"
#include "Messages.h"
#include "Compute.h"
#include "ArchiveEvents.h"
#include "PaintMaker.h"
#include "controller.h"
#include "StoredData.h"
#include "..\TypeSizeBase\ParametersCycle.h"
//----------------------------------------------------------------------------------
void InitToolBar()
{
	SendMessage(mainWindow.hToolBar, TB_ENABLEBUTTON, IDB_SensorsUnit, MAKELONG(false, 0));
	SendMessage(mainWindow.hToolBar, TB_ENABLEBUTTON, IDB_ClampBtn, MAKELONG(false, 0));

	SendMessage(mainWindow.hToolBar, TB_ENABLEBUTTON, IDB_Clamp1OnBtn, MAKELONG(false, 0));
	SendMessage(mainWindow.hToolBar, TB_ENABLEBUTTON, IDB_Clamp2OnBtn, MAKELONG(false, 0));
}
//----------------------------------------------------------------------------------
void SycleMeashurement(HWND h)
{
	if(ParametesCycle::Do(h) && automats.StartAutomatic())
	{
		SendMessage(mainWindow.hStatusWindow, SB_SETTEXT, 0, (LONG)L"Режим циклического запуска");
		SendMessage(mainWindow.hStatusWindow, SB_SETTEXT, 1, (LONG)L"");
		thicknessViewer.openDetailedWindow = false;
		Disable3Button(h);	
		device1730.Start();
		mainLoop.SetLoop(CollectDataProc);
	}
}
//-------------------------------------------------------------------------------
void SingleMeashurement(HWND h)
{
	if(ParametesCycleTest::Do(h) && automats.StartSingle())
	{
		SendMessage(mainWindow.hStatusWindow, SB_SETTEXT, 0, (LONG)L"Режим эталона"); 
		SendMessage(mainWindow.hStatusWindow, SB_SETTEXT, 1, (LONG)L"");
		thicknessViewer.openDetailedWindow = false;
		Disable3Button(h);
		device1730.Start();
		mainLoop.SetLoop(CollectDataProc);
	}
}
void TestMeashurement(HWND h)
{
	if(ParametesCycleTest::Do(h))
	{
		SendMessage(mainWindow.hStatusWindow, SB_SETTEXT, 0, (LONG)L"Тестовый режим"); 
		SendMessage(mainWindow.hStatusWindow, SB_SETTEXT, 1, (LONG)L"");
		TestLan::Do(h);
		thicknessViewer.openDetailedWindow = false;
	}
}
void ViewerBtn(HWND)
{
	DetailedWindow::Open();
	thicknessViewer.openDetailedWindow = true;
	RepaintWindow(DetailedWindow::Instance().hWnd);
}
//-----------------------------------------------------------------------------
void StopMeashurement(HWND h)
{
	controller.workMode = false;
	automats.Stop();
	Enanle3Button(h);
	unsigned &output = archiveEvents.map->output;
	output &= ~Singleton<OutputsTable>::Instance().items.get<Clamp1SupplyY6BitOut>().value;
	output &= ~Singleton<OutputsTable>::Instance().items.get<Clamp2SupplyY7BitOut>().value;
	output |= Singleton<OutputsTable>::Instance().items.get<SensorsRemovalY3BitOut>().value;	
	device1730.Write(output);
	paintMaker.Stop();
	Sleep(500);
	output &= ~Singleton<OutputsTable>::Instance().items.get<SensorsRemovalY3BitOut>().value;
	device1730.Write(output);
	mainLoop.SetLoop(NoopProc);
	SendMessage(mainWindow.hStatusWindow, SB_SETTEXT, 0, (LONG)L""); 
	SendMessage(mainWindow.hStatusWindow, SB_SETTEXT, 1, (LONG)L""); 
#ifdef DEBUG_SAVE_DATA_BASE
	Stored().Do();
#endif
}
//-------------------------------------------------------------------------------------------
void OptionsBtn(HWND h)
{
	if(SendMessage(mainWindow.hToolBar, TB_ISBUTTONENABLED, IDB_SensorsUnit, 0))
	{
		SendMessage(mainWindow.hToolBar, TB_ENABLEBUTTON, IDB_CycleBtn, MAKELONG(true, 0));
		SendMessage(mainWindow.hToolBar, TB_ENABLEBUTTON, IDB_Refresh, MAKELONG(true, 0));
		SendMessage(mainWindow.hToolBar, TB_ENABLEBUTTON, IDB_QueryBtn, MAKELONG(true, 0));
		SendMessage(mainWindow.hToolBar, TB_ENABLEBUTTON, IDB_SensorsUnit, MAKELONG(false, 0));
		SendMessage(mainWindow.hToolBar, TB_ENABLEBUTTON, IDB_ClampBtn, MAKELONG(false, 0));
		SendMessage(mainWindow.hToolBar, TB_ENABLEBUTTON, IDB_MashBtn, MAKELONG(true, 0));
		SendMessage(mainWindow.hToolBar, TB_ENABLEBUTTON, IDB_Reset, MAKELONG(true, 0));
		SendMessage(mainWindow.hStatusWindow, SB_SETTEXT, 0, (LONG)L""); 
		SendMessage(mainWindow.hStatusWindow, SB_SETTEXT, 1, (LONG)L""); 

		SendMessage(mainWindow.hToolBar, TB_ENABLEBUTTON, IDB_Clamp1OnBtn, MAKELONG(false, 0));
	    SendMessage(mainWindow.hToolBar, TB_ENABLEBUTTON, IDB_Clamp2OnBtn, MAKELONG(false, 0));

		mainWindow.options = false;
	}
	else
	{
		SendMessage(mainWindow.hToolBar, TB_ENABLEBUTTON, IDB_CycleBtn, MAKELONG(false, 0));
		SendMessage(mainWindow.hToolBar, TB_ENABLEBUTTON, IDB_Refresh, MAKELONG(false, 0));
		SendMessage(mainWindow.hToolBar, TB_ENABLEBUTTON, IDB_QueryBtn, MAKELONG(false, 0));
		SendMessage(mainWindow.hToolBar, TB_ENABLEBUTTON, IDB_SensorsUnit, MAKELONG(true, 0));
		SendMessage(mainWindow.hToolBar, TB_ENABLEBUTTON, IDB_ClampBtn, MAKELONG(true, 0));
		SendMessage(mainWindow.hToolBar, TB_ENABLEBUTTON, IDB_MashBtn, MAKELONG(false, 0));
		SendMessage(mainWindow.hToolBar, TB_ENABLEBUTTON, IDB_Reset, MAKELONG(false, 0));

		SendMessage(mainWindow.hToolBar, TB_ENABLEBUTTON, IDB_Clamp1OnBtn, MAKELONG(true, 0));
	    SendMessage(mainWindow.hToolBar, TB_ENABLEBUTTON, IDB_Clamp2OnBtn, MAKELONG(true, 0)); 

		SendMessage(mainWindow.hStatusWindow, SB_SETTEXT, 0, (LONG)L"Режим 'Наладка'");
		mainWindow.options = true;
	}
	mainLoop.SetLoop(NoopProc);
	ManagementIOportsDlg::SendTitle();
}
#define MESS(name) (input & Singleton<InputsTable>::Instance().items.get<name>().value)\
	? MessageField<name >().Text() : L""
void ExitTubeBtn(HWND h)
{
	const unsigned pathClear = 
		Singleton<InputsTable>::Instance().items.get  <PipeAfterClamp1SQ1BitIn >().value
		| Singleton<InputsTable>::Instance().items.get<PipeInletSQ3BitIn       >().value
		| Singleton<InputsTable>::Instance().items.get<OutletPipeSQ4BitIn      >().value
		| Singleton<InputsTable>::Instance().items.get<PipeClamp2BeforeSQ7BitIn>().value
		;
	unsigned input = device1730.Read();
	unsigned &output = archiveEvents.map->output;
	output &= ~Singleton<OutputsTable>::Instance().items.get<Clamp1SupplyY6BitOut>().value;
	output &= ~Singleton<OutputsTable>::Instance().items.get<Clamp2SupplyY7BitOut>().value;
	device1730.Write(output);
#pragma message("Востановить проверку наличия трубы на блоке датчиков")
	///*
	if(0 != (input & pathClear))
	{
		wchar_t buf[1024];
		wsprintf(buf, L"Сработали датчики:\n%s\n%s\n%s\n%s"
			, MESS(PipeAfterClamp1SQ1BitIn )
			, MESS(PipeInletSQ3BitIn       )
			, MESS(OutletPipeSQ4BitIn      )
			, MESS(PipeClamp2BeforeSQ7BitIn)
			);
		MessageBox(h, buf, L"Предупреждение", MB_ICONWARNING);
		return;
	}	

	if(input & Singleton<InputsTable>::Instance().items.get<SensorUnitRetractedSQ5BitIn>().value)
	{
		output |= Singleton<OutputsTable>::Instance().items.get<SensorUnitSupplyY2BitOut>().value;
		output |= Singleton<OutputsTable>::Instance().items.get<SensorsRemovalY3BitOut>().value;
	}
	else
	{
		output |= Singleton<OutputsTable>::Instance().items.get<SensorUnitRemovalY1BitOut>().value;
	}
	device1730.Write(output);
	Sleep(500);
	output &= ~Singleton<OutputsTable>::Instance().items.get<SensorUnitSupplyY2BitOut>().value;
	output &= ~Singleton<OutputsTable>::Instance().items.get<SensorsRemovalY3BitOut>().value;
	output &= ~Singleton<OutputsTable>::Instance().items.get<SensorUnitRemovalY1BitOut>().value;
	device1730.Write(output);
}
#undef MESS
void ClampingBtn(HWND h)
{
	unsigned input = device1730.Read();
	unsigned &output = archiveEvents.map->output;
	if(input & Singleton<InputsTable>::Instance().items.get<SensorsDivorcedSQ6BitIn>().value)
	{
		output |= Singleton<OutputsTable>::Instance().items.get<SensorsSupplyY4BitOut>().value;
	}
	else
	{
		output |= Singleton<OutputsTable>::Instance().items.get<SensorsRemovalY3BitOut>().value;
	}
	device1730.Write(output);
	Sleep(500);
	output &= ~Singleton<OutputsTable>::Instance().items.get<SensorsSupplyY4BitOut>().value;
	output &= ~Singleton<OutputsTable>::Instance().items.get<SensorsRemovalY3BitOut>().value;
	device1730.Write(output);
}

void Clamp1On(HWND)
{
	unsigned input = device1730.Read();
	unsigned &output = archiveEvents.map->output;
	if(input & Singleton<InputsTable>::Instance().items.get<Clamp1RetractedSQ2BitIn>().value)
	{
		output |= Singleton<OutputsTable>::Instance().items.get<Clamp1SupplyY6BitOut>().value;
	}
	else
	{
		output &= ~Singleton<OutputsTable>::Instance().items.get<Clamp1SupplyY6BitOut>().value;
	}
	device1730.Write(output);
}
void Clamp2On(HWND)
{
	unsigned input = device1730.Read();
	unsigned &output = archiveEvents.map->output;
	if(input & Singleton<InputsTable>::Instance().items.get<Clamp2RetractedSQ8BitIn>().value)
	{
		output |= Singleton<OutputsTable>::Instance().items.get<Clamp2SupplyY7BitOut>().value;
	}
	else
	{
		output &= ~Singleton<OutputsTable>::Instance().items.get<Clamp2SupplyY7BitOut>().value;
	}
	device1730.Write(output);
}

void Disable3Button(HWND h)
{
	mainWindow.sizingWindow = false; 
	SendMessage(mainWindow.hToolBar, TB_ENABLEBUTTON, IDB_QueryBtn, MAKELONG(false, 0));
	SendMessage(mainWindow.hToolBar, TB_ENABLEBUTTON, IDB_MashBtn, MAKELONG(false, 0));
	SendMessage(mainWindow.hToolBar, TB_ENABLEBUTTON, IDB_OptionsBtn, MAKELONG(false, 0));
    mainWindow.DisableMemuItemFile();
}
void Enanle3Button(HWND h)
{
	mainWindow.sizingWindow = true; 
	SendMessage(mainWindow.hToolBar, TB_ENABLEBUTTON, IDB_QueryBtn, MAKELONG(true, 0));
	SendMessage(mainWindow.hToolBar, TB_ENABLEBUTTON, IDB_MashBtn, MAKELONG(true, 0));
	SendMessage(mainWindow.hToolBar, TB_ENABLEBUTTON, IDB_OptionsBtn, MAKELONG(true, 0));
	mainWindow.EnableMemuItemFile();
}
void DisableAllButton()
{
	SendMessage(mainWindow.hToolBar, TB_ENABLEBUTTON, IDB_CycleBtn, MAKELONG(false, 0));
	SendMessage(mainWindow.hToolBar, TB_ENABLEBUTTON, IDB_Refresh, MAKELONG(false, 0));
	SendMessage(mainWindow.hToolBar, TB_ENABLEBUTTON, IDB_QueryBtn, MAKELONG(false, 0));
	SendMessage(mainWindow.hToolBar, TB_ENABLEBUTTON, IDB_SensorsUnit, MAKELONG(false, 0));
	SendMessage(mainWindow.hToolBar, TB_ENABLEBUTTON, IDB_ClampBtn, MAKELONG(false, 0));
	SendMessage(mainWindow.hToolBar, TB_ENABLEBUTTON, IDB_MashBtn, MAKELONG(false, 0));
	SendMessage(mainWindow.hToolBar, TB_ENABLEBUTTON, IDB_Reset, MAKELONG(false, 0));

	SendMessage(mainWindow.hToolBar, TB_ENABLEBUTTON, IDB_Clamp1OnBtn, MAKELONG(false, 0));
	SendMessage(mainWindow.hToolBar, TB_ENABLEBUTTON, IDB_Clamp2OnBtn, MAKELONG(false, 0));

	SendMessage(mainWindow.hToolBar, TB_ENABLEBUTTON, IDB_OptionsBtn, MAKELONG(false, 0));
//	SendMessage(mainWindow.hToolBar, TB_ENABLEBUTTON, IDB_SensorsUnit, MAKELONG(false, 0));
//	SendMessage(mainWindow.hToolBar, TB_ENABLEBUTTON, IDB_ClampBtn, MAKELONG(false, 0));
}
/* не стирать 
DWORD WINAPI WorkItemFunc(MainWindow *self)
{
	Sleep(5000);
	self->StringMessage(L"<ff0000>Stored");
	self->DisableBtn(IDB_MeshureBtn, true);
	self->DisableBtn(IDB_MashBtn, true);
	return 0;
}
void MainWindow::LoadTestProgram(HWND h)
{
	StringMessage(L"<ff>MainWindow::LoadTestProgram");
    DisableBtn(IDB_MeshureBtn, false);
	DisableBtn(IDB_MashBtn, false);
    QueueUserWorkItem((PTHREAD_START_ROUTINE)WorkItemFunc, this, WT_EXECUTEDEFAULT);
}
DWORD WINAPI WorkItemFuncLoadWorkProgram(MainWindow *self)
{
	Sleep(5000);
	self->StringMessage(L"<ff00ff>Stored WorkItemFuncLoadWorkProgram");
	self->DisableBtn(IDB_MoveBtn, true);
	return 0;
}
void MainWindow::LoadWorkProgram(HWND h)
{
	StringMessage(L"<ff5555>MainWindow::LoadWorkProgram");
	DisableBtn(IDB_MoveBtn, false);
    QueueUserWorkItem((PTHREAD_START_ROUTINE)WorkItemFuncLoadWorkProgram, this, WT_EXECUTEDEFAULT);
}
void MainWindow::StoreToBase(HWND h){}
void MainWindow::StringMessage(wchar_t *txt)
{
	/*
	topLabel.text = txt;
	RECT r;
	GetClientRect(hPaint, &r);
	SendMessage(hPaint, WM_SIZE, 0, r.right | (r.bottom << 16));
	*/
//}
//--------------------------------------------------------------------------------------------------
/*
void MainWindow::DisableBtn(int id, bool enable)
{
	if(SendMessage(hToolBar, TB_ENABLEBUTTON, id, MAKELONG(enable, 0)))
	{
		wchar_t buf[128];
		if(!enable)wsprintf(buf, L"<ff00>button pressed %d", id);
		else wsprintf(buf, L"<ff00ff>button unpressed %d", id);
		StringMessage(buf);
	}
}
//-----------------------------------------------------------------------
void MainWindow::MeshureBtn(HWND h)
{
	if(!SendMessage(hToolBar, TB_ISBUTTONCHECKED, IDB_MeshureBtn, 0))
	{
		SendMessage(hToolBar, TB_CHECKBUTTON, IDB_MeshureBtn, MAKELONG(true, 0));
		SendMessage(hToolBar, TB_CHECKBUTTON, IDB_MashBtn, MAKELONG(false, 0));
		SendMessage(hToolBar, TB_CHECKBUTTON, IDB_MoveBtn, MAKELONG(false, 0));
	//	screen.SetMeshure();
      //  storedCursor = (HCURSOR)SetClassLong(hPaint, GCL_HCURSOR, (LONG)noCursor);
	}
}
void MainWindow::ScaleBtn(HWND h)
{
	if(!SendMessage(hToolBar, TB_ISBUTTONCHECKED, IDB_MashBtn, 0))
	{
		SendMessage(hToolBar, TB_CHECKBUTTON, IDB_MeshureBtn, MAKELONG(false, 0));
		SendMessage(hToolBar, TB_CHECKBUTTON, IDB_MashBtn, MAKELONG(true, 0));
		SendMessage(hToolBar, TB_CHECKBUTTON, IDB_MoveBtn, MAKELONG(false, 0));
	//	screen.SetScale();
	//	if(NULL != storedCursor)SetClassLong(hPaint, GCL_HCURSOR, (LONG)storedCursor);
	}
}
void MainWindow::MoveBtn(HWND h)
{
	if(!SendMessage(hToolBar, TB_ISBUTTONCHECKED, IDB_MoveBtn, 0))
	{
		SendMessage(hToolBar, TB_CHECKBUTTON, IDB_MeshureBtn, MAKELONG(false, 0));
		SendMessage(hToolBar, TB_CHECKBUTTON, IDB_MashBtn, MAKELONG(false, 0));
		SendMessage(hToolBar, TB_CHECKBUTTON, IDB_MoveBtn, MAKELONG(true, 0));
	//	screen.SetMove();
	//    if(NULL != storedCursor)SetClassLong(hPaint, GCL_HCURSOR, (LONG)storedCursor);
	}
}
void MainWindow::RefreshBtn(HWND h)
{
	//screen.SetRestore(h);
}
//---------------------------------------------------------------------------------
*/