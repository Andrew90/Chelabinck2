#include "stdafx.h"
#include "ManagementIOports.h"
#include <commctrl.h>
#include "MenuApi.h"
#include "DialogsManagmentOptions.h"
#include "Pass.h"
#include "ManagementIOportsMenu.hpp"
#include "InitToolBar.hpp"
#include "EmptyWindow.h"
#include "ToolBArWidth.hpp"
#include "AppBase.h"
#include "mainwindow.h"


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
ManagementIOports::ManagementIOports() : backScreen(NULL){}
//-------------------------------------------------------------------------
void ManagementIOports::operator()(TSize &l)
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
	
	Graphics g(backScreen);
	viewer.Size(g, l.Width, l.Height);
}
//------------------------------------------------------------------------
void ManagementIOports::operator()(TCommand &m)
{
	GetMenuToolBarEvent(m);
}
//------------------------------------------------------------------------
void ManagementIOports::operator()(TGetMinMaxInfo &m)
{
	if(NULL != m.pMinMaxInfo)
	{
		m.pMinMaxInfo->ptMinTrackSize.x = 400;
		m.pMinMaxInfo->ptMinTrackSize.y = 300;
		m.pMinMaxInfo->ptMaxTrackSize.x = 400;
		m.pMinMaxInfo->ptMaxTrackSize.y = 300;		
		RepaintWindow(m.hwnd);
	}		
}
//------------------------------------------------------------------------
unsigned ManagementIOports::operator()(TNotify &m)
{
	return 0;
}
//------------------------------------------------------------------------
unsigned ManagementIOports::operator()(TCreate &m)
{
	Menu<ManagementIOportsMenu::MainMenu>().Init(m.hwnd);
	viewer.Start((PVOID)this);
	return 0;
}
//------------------------------------------------------------------------
void ManagementIOports::operator()(TDestroy &m)
{
	dprint("TDestroy");
	viewer.Stop();
	SetWindowLongPtr(m.hwnd, GWLP_USERDATA, NULL);
}
//------------------------------------------------------------------------
unsigned ManagementIOports::operator()(TMessage &)
{
	dprint("TMessage");
	return 0;
}
//------------------------------------------------------------------------
void ManagementIOports::operator()(TLButtonDown &l)
{
	if(mainWindow.options)viewer.MouseLDown(l);
}
//------------------------------------------------------------------------
void ManagementIOports::operator()(TPaint &l)
{
	PAINTSTRUCT p;
	HDC hdc = BeginPaint(l.hwnd, &p);
	{		
		Graphics g(hdc);		
		g.DrawCachedBitmap(&CachedBitmap(backScreen, &g), 0, 0);		
	}
	EndPaint(l.hwnd, &p);
}
ManagementIOports managementIOports;