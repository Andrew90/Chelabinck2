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
//#include "MainGrid.h"
#include "AppBase.h"

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
using namespace Gdiplus;
//------------------------------------------------------------------------
MainWindow::MainWindow() : backScreen(NULL){}
//-------------------------------------------------------------------------
void MainWindow::operator()(TSize &l)
{
	if(l.resizing == SIZE_MINIMIZED || l.Height < 100) return;	
	
	if(NULL != backScreen)
	{
		if(backScreen->GetWidth() < l.Width || backScreen->GetHeight() < l.Height)
		{
			delete backScreen;
		    backScreen = new Bitmap(l.Width, l.Height);
		}
	}
	else if(l.Width > 0 && l.Height > 0)
	{
		backScreen = new Bitmap(l.Width, l.Height);
	}
	else
	{
		return;
	}
	
	dprint("TSize");
	Graphics g(backScreen);
	viewer.Size(g, l.Width, l.Height);
}
//------------------------------------------------------------------------
void MainWindow::operator()(TCommand &m)
{
	dprint("TCommand");
	GetMenuToolBarEvent(m);
}
//------------------------------------------------------------------------
void MainWindow::operator()(TGetMinMaxInfo &m)
{
	if(NULL != m.pMinMaxInfo)
	{
		m.pMinMaxInfo->ptMinTrackSize.x = 400;
		m.pMinMaxInfo->ptMinTrackSize.y = 300;
		m.pMinMaxInfo->ptMaxTrackSize.x = 400;
		m.pMinMaxInfo->ptMaxTrackSize.y = 300;		
		dprint("TGetMinMaxInfo");
		RepaintWindow(m.hwnd);
	}		
}
//------------------------------------------------------------------------
unsigned MainWindow::operator()(TNotify &m)
{
	return 0;
}
//------------------------------------------------------------------------
unsigned MainWindow::operator()(TCreate &m)
{
	Menu<MainMenu>().Init(m.hwnd);
	viewer.Start((PVOID)this);
	return 0;
}
//------------------------------------------------------------------------
void MainWindow::operator()(TDestroy &m)
{
	dprint("TDestroy");
	viewer.Stop();
	SetWindowLongPtr(m.hwnd, GWLP_USERDATA, NULL);
	PostQuitMessage(0);
}
//------------------------------------------------------------------------
unsigned MainWindow::operator()(TMessage &)
{
	dprint("TMessage");
	return 0;
}
//------------------------------------------------------------------------
void MainWindow::operator()(TPaint &l)
{
	PAINTSTRUCT p;
	HDC hdc = BeginPaint(l.hwnd, &p);
	{		
		Graphics g(hdc);		
		g.DrawCachedBitmap(&CachedBitmap(backScreen, &g), 0, 0);		
	}
	EndPaint(l.hwnd, &p);
}
MainWindow mainWindow;
