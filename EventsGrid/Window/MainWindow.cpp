#include "stdafx.h"
#include "MainWindow.h"
#include <commctrl.h>
#include "MenuApi.h"
#include "Dialogs.h"
//#include "MenuItems.h"
#include "Pass.h"
#include "MainWindowMenu.hpp"
//#include "MainWindowToolBar.h"
#include "InitToolBar.hpp"
#include "EmptyWindow.h"
//#include "TopLabelViewer.h"
//#include "ThicknessViewer.h"
#include "ToolBArWidth.hpp"
#include "MainGrid.h"

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
	grid.Size(0, 0, m.Width, m.Height);
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
		m.pMinMaxInfo->ptMinTrackSize.x = 710;
		m.pMinMaxInfo->ptMinTrackSize.y = 400;
		m.pMinMaxInfo->ptMaxTrackSize.x = 900;
		m.pMinMaxInfo->ptMaxTrackSize.y = 600;		
		RepaintWindow(m.hwnd);
	}		
}
//------------------------------------------------------------------------
unsigned MainWindow::operator()(TNotify &m)
{
	GridNotify *x = (GridNotify *)GetWindowLongPtr(m.pnmh->hwndFrom, GWLP_USERDATA);
	if(0 == IsBadReadPtr(x, sizeof(x)))return x->Notify(m);
	return 0;
}
//------------------------------------------------------------------------
unsigned MainWindow::operator()(TCreate &m)
{
	Menu<MainMenu>().Init(m.hwnd);
	grid.Create(m, &mainGrid);
	mainGrid.Start(grid.hWnd);
	return 0;
}
//------------------------------------------------------------------------
void MainWindow::operator()(TDestroy &m)
{
	mainGrid.Stop();
	SetWindowLongPtr(m.hwnd, GWLP_USERDATA, NULL);
	PostQuitMessage(0);
}
//------------------------------------------------------------------------
unsigned MainWindow::operator()(TMessage &)
{
	return 0;
}
//------------------------------------------------------------------------
MainWindow mainWindow;