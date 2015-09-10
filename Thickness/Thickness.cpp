// Thickness.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Thickness.h"

#include <windows.h>
#include <commctrl.h>

#include "AppBase.h"
#include "Window\MainWindow.h"
#include "EmptyWindow.h"

#include "DebugMess.h"
#include "Automat/MainLoop.h"
#include "ArchiveEvents.h"
#include "GlobalItems.h"


HINSTANCE hInstance;	
wchar_t *typeWindow = L"thickness_120514";
int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	CreateSemaphore(0, 0, 1, typeWindow);
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		HWND h = FindWindow(typeWindow, 0);
		SendMessage(h, WM_SYSCOMMAND, SC_RESTORE, 0);
		SetForegroundWindow(h);
		return 0;
	}
	::hInstance = hInstance;
	INITCOMMONCONTROLSEX iccx;
	iccx.dwSize=sizeof(INITCOMMONCONTROLSEX);
	iccx.dwICC=0;
	InitCommonControlsEx(&iccx);

	ULONG_PTR gdiplusToken; 
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;    
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
//--------------------------------------------------------------------
#ifdef XDEBUG
	debug.Init();
	debug.print("Ok thickness");
#endif	
	Initialize initialize;
	HWND h = NULL;
	//AppBase().Init();
	InitGlobalData();
	{
		CBase base(ParametersBase().name());
		if(base.IsOpen())
		{				
			ThicknessWindowTable o;
			Select<ThicknessWindowTable>(base).ID(1).Execute(o);
			h = WindowTemplate(
				&mainWindow
				, typeWindow
				,L"Толщиномер"
				, IDI_UPLOAD
				, o.items.get<OffsetWindowX>().value
				, o.items.get<OffsetWindowY>().value
				, o.items.get<WindowWidth>().value
				, o.items.get<WindowHeight>().value
				);
			ShowWindow(h, SW_SHOWNORMAL);
		}
		else
		{
			MessageBox(0, L"Не могу открыть базу данных", L"Ошибка !!!", MB_ICONERROR);
		}
	}
	
#ifdef THREAD_PRIORITY
			SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
#endif

	MSG msg;  
	if(h)
	{
		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
//	DestroyGlobalData();
	Gdiplus::GdiplusShutdown(gdiplusToken);
	return (int) msg.wParam;
}