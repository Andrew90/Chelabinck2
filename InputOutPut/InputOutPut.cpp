// InputOutPut.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "resource.h"

#include <windows.h>
#include <commctrl.h>

#include "Window\MainWindow.h"
#include "EmptyWindow.h"

#include "AppBase.h"

#include "DebugMess.h"
#ifdef XDEBUG
#define dprint debug.print
#else	
#define  dprint
#endif	

HINSTANCE hInstance;	
wchar_t *typeWindow = L"InputOutput_090714";
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

#ifdef XDEBUG
	debug.Init();
	debug.print("Ok thickness");
#endif	

	Initialize initialize;
	HWND h = NULL;
	AppBase().Init();
	{
		CBase base(ParametersBase().name());
		if(base.IsOpen())
		{		
			InputOutputWindowTable o;
			Select<InputOutputWindowTable>(base).ID(1).Execute(o);
			h = WindowTemplate(
				&mainWindow
				, typeWindow
				, L"Дискретные входы-выходы"
				, IDI_settings
				, o.items.get<OffsetWindowX>().value
				, o.items.get<OffsetWindowY>().value
				, mainWindow.width
				, mainWindow.height
				);
			ShowWindow(h, SW_SHOWNORMAL);
		}
	}
	
	MSG msg;
	if(h)
	{
		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	else
	{
		MessageBox(0, L"Не могу открыть базу данных", L"Ошибка !!!", MB_ICONERROR);
	}
	Gdiplus::GdiplusShutdown(gdiplusToken);
	return (int) msg.wParam;
}