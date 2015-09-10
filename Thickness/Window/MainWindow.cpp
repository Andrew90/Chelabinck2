#include "stdafx.h"
#include "MainWindow.h"
#include <commctrl.h>
#include "MenuApi.h"
#include "Dialogs.h"
#include "MenuItems.h"
#include "Pass.h"
#include "SaveLoadData.h"
#include "..\TypeSizeBase\ParametersCycle.h"
#include "MainWindowMenu.hpp"
#include "MainWindowToolBar.h"
#include "InitToolBar.hpp"
#include "EmptyWindow.h"
#include "TopLabelViewer.h"
#include "ThicknessViewer.h"
#include "ToolBArWidth.hpp"
#include "GlobalItems.h"

#include "DebugMess.h"
#ifdef XDEBUG
#define xprint debug.print(__FUNCTION__);
#define dprint debug.print
#define d_mess(x)
#define x_mess debug.print
#else
#define xprint
#define dprint
#endif
//------------------------------------------------------------------------
void MainWindow::operator()(TSize &m)
{
	if(m.resizing == SIZE_MINIMIZED || 0 == m.Width || 0 == m.Height) return;
	MoveWindow(hToolBar, 0, 0, 0, 0, false);
	MoveWindow(hStatusWindow, 0, 0, 0, 0, false);

	RECT rt;
	GetWindowRect(hToolBar, &rt);
    RECT rs;
	GetClientRect(hStatusWindow, &rs);
	RECT r;
	GetClientRect(m.hwnd, &r);	

	//select.Size((TL::Length<tool_button_list>::value - 2) * 64 + 32, 5, 400);
//	static const int width = __tool_bar_width__<tool_button_list>::value;
//	select.Size(/*600*/width, 5, 400);

	static const int topLabelHeight = 40;
	int y = rt.bottom - rt.top - 1;
	int t = (r.bottom - rs.bottom - rt.bottom + rt.top + 2 - topLabelHeight);
	MoveWindow(topLabelViewer.hWnd , 0, y, r.right, topLabelHeight, true);
	y += topLabelHeight;
	MoveWindow(thicknessViewer.hWnd , 0, y, r.right, t, true);

	//-------------------test
	//topLabelViewer.SetMessage(L"<ff>TopLabel");
}
//------------------------------------------------------------------------
void MainWindow::operator()(TCommand &m)
{
	GetMenuToolBarEvent(m);
}
//------------------------------------------------------------------------
void MainWindow::operator()(TGetMinMaxInfo &m)
{
	if(NULL != m.pMinMaxInfo)
	{
		m.pMinMaxInfo->ptMinTrackSize.x = 600;
		m.pMinMaxInfo->ptMinTrackSize.y = 300;
		m.pMinMaxInfo->ptMaxTrackSize.x = 2000;
		m.pMinMaxInfo->ptMaxTrackSize.y = 500;		
		RepaintWindow(m.hwnd);
	}		
}
//------------------------------------------------------------------------
unsigned MainWindow::operator()(TCreate &m)
{
	sizingWindow = true;
	Menu<MainWindowMenu::MainMenu>().Init(m.hwnd);
	hToolBar = InitToolbar<tool_button_list>()(m.hwnd);
	InitToolBar();
	//select.Create(hToolBar);
	hStatusWindow = CreateStatusWindow(WS_CHILD | WS_VISIBLE, NULL, m.hwnd, 0);
	int pParts[] = {550,900, 3000};
	SendMessage(hStatusWindow, SB_SETPARTS, 3, (LPARAM)pParts);
	topLabelViewer.hWnd = CreateChildWindow(m.hwnd, (WNDPROC)&Viewer<TopLabelViewer>::Proc, L"TopLabelWindow", &topLabelViewer);
	thicknessViewer.hWnd = CreateChildWindow(m.hwnd, (WNDPROC)&Viewer<ThicknessViewer>::Proc, L"ThicknessWindow", &thicknessViewer);
	/*
	wchar_t buffer[128];
	WorkTimeTable &s = Singleton<WorkTimeTable>::Instance();
	wchar_t *_0 = s.items.get<OperatorName>().value;
//	int _1 = s.items.get<WorkTime>().value;
	wsprintf(buffer, L"Оператор %s", _0);// смена %d", _0, _1);
	SendMessage(hStatusWindow, SB_SETTEXT, 2, (LONG)buffer);
	*/
	if(!initGlobalUnitOk)
	{
		DisableAllButton();
	}
	return 0;
}
//-------------------------------------------------------------------------
void MainWindow::operator()(TSizing &l)
{
	if(!sizingWindow)
	{
		RECT r;
		GetWindowRect(l.hwnd, &r);
		*l.pRect = r;
	}
}
//------------------------------------------------------------------------
void MainWindow::operator()(TDestroy &)
{
	dprint("TDestroy");
	DestroyGlobalData();
	Sleep(1000);
	PostQuitMessage(0);
}
//--------------------------------------------------------------------------
void MainWindow::operator()(TKeyDown &l)
{
	l.hwnd = thicknessViewer.hWnd; 
	SendMessage(MESSAGE(l));
}
//------------------------------------------------------------------------
struct EnableMenuAll
{
	template<class O, class P>struct loc
	{
		void operator()(O *, P *p)
		{
           EnableMenu<O>(p->h, p->en);
		}
	};
	HWND h;
	bool en;
	explicit EnableMenuAll(HWND h, bool en) : h(h), en(en)
	{
		TL::foreach<MainWindowMenu::MainMenu, loc>()((TL::Factory<MainWindowMenu::MainMenu> *)0, this);
	}
};
void MainWindow::DisableMemuItemFile()
{
	EnableMenuAll(hWnd, false);
	//EnableWindow(select.hWnd, false);
}
//-------------------------------------------------------------------------
void MainWindow::EnableMemuItemFile()
{
	EnableMenuAll(hWnd, true);
//	EnableWindow(select.hWnd, true);
}
//--------------------------------------------------------------------------
void MainWindow::BottomLabel(int n, wchar_t *text)
{
	SendMessage(mainWindow.hStatusWindow, SB_SETTEXT, n, (LONG)text);
}
//---------------------------------------------------------------------------
void MainWindow::operator()(TMouseWell &l)
{
	l.hwnd = thicknessViewer.hWnd; 
	SendMessage(MESSAGE(l));
}

MainWindow mainWindow;