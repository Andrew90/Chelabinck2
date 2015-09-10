#include "stdafx.h"
#include "EditOperatorsWindow.h"
#include "Dialogs.h"
#include "EmptyWindow.h"
#include "MenuAPI.h"
#include <WindowsX.h>
#include "Pass.h"

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
//-----------------------------------------------------------------------------
void EditOperatorsWindow::operator()(TSize &m)
{
	if(m.resizing == SIZE_MINIMIZED || 0 == m.Width || 0 == m.Height) return;
	int y = m.Height - 80;
	grid.Size(0, 0, m.Width, y);
	y += 5;
	int left = 5;
	MoveWindow(gridViewer.hEdits[0], left, y, 300 - 5, 25, TRUE);
	left += 300;										  
	MoveWindow(gridViewer.hEdits[1], left, y, 200 - 5, 25, TRUE);

	y += 40;
	left = 20;

	MoveWindow(hCloseBtn, left, y, 100 - 5, 25, TRUE);
	left += 110;
	MoveWindow(hAddBtn, left, y, 100 - 5, 25, TRUE);
	left += 110;
	MoveWindow(hDeleteBtn, left, y, 100 - 5, 25, TRUE);
}
//------------------------------------------------------------------------
void EditOperatorsWindow::operator()(TCommand &m)
{
	GetMenuToolBarEvent(m);
}
//------------------------------------------------------------------------
void EditOperatorsWindow::operator()(TGetMinMaxInfo &m)
{
	if(NULL != m.pMinMaxInfo)
	{
		m.pMinMaxInfo->ptMinTrackSize.x = 550;
		m.pMinMaxInfo->ptMinTrackSize.y = 400;
		m.pMinMaxInfo->ptMaxTrackSize.x = 550;
		m.pMinMaxInfo->ptMaxTrackSize.y = 600;		
	}		
}
//------------------------------------------------------------------------
extern HINSTANCE hInstance;
unsigned EditOperatorsWindow::operator()(TCreate &m)
{
	grid.Create(m, &gridViewer);
	for(int i = 0; i < dimention_of(gridViewer.hEdits); ++i)
	{
		gridViewer.hEdits[i] = CreateWindowEx(WS_EX_CLIENTEDGE, L"edit", L""
			, WS_VISIBLE | WS_CHILD | ES_LEFT | WS_TABSTOP
			,0, 0, 0, 0, m.hwnd, 0, hInstance, NULL
			);
	}
	hCloseBtn = CreateWindow(L"button", L"Закрыть"
		, WS_VISIBLE | WS_CHILD | WS_TABSTOP
		,0, 0, 0, 0, m.hwnd, (HMENU)0, hInstance, NULL
		);
	SetWindowLong(hCloseBtn, GWL_USERDATA, (LONG)&EditOperatorsWindow::CloseBtn);
	hAddBtn = CreateWindow(L"button", L"Добавить"
		, WS_VISIBLE | WS_CHILD | WS_TABSTOP
		,0, 0, 0, 0, m.hwnd, (HMENU)0, hInstance, NULL
		);
	SetWindowLong(hAddBtn, GWL_USERDATA, (LONG)&EditOperatorsWindow::AddBtn);
	hDeleteBtn = CreateWindow(L"button", L"Удалить"
		, WS_VISIBLE | WS_CHILD | WS_TABSTOP
		,0, 0, 0, 0, m.hwnd, (HMENU)0, hInstance, NULL
		);
	SetWindowLong(hDeleteBtn, GWL_USERDATA, (LONG)&EditOperatorsWindow::DeleteBtn);
	return 0;
}
//------------------------------------------------------------------------
void EditOperatorsWindow::operator()(TDestroy &l)
{
	SetWindowLong(l.hwnd, GWL_USERDATA, 0);
	delete this;
}
//------------------------------------------------------------------------
unsigned EditOperatorsWindow::operator()(TNotify &l)
{
	GridNotify *x = (GridNotify *)GetWindowLongPtr(l.pnmh->hwndFrom, GWLP_USERDATA);
	if(0 == IsBadReadPtr(x, sizeof(x)))return x->Notify(l);
	return 0;
}
//-------------------------------------------------------------------------------
void EditOperatorsWindowDlg::Do(HWND h)
{
	wchar_t *typeWindow = L"EditOperatorsWindowDlg";
	if(HWND h = FindWindow(typeWindow, 0))
	{
		SendMessage(h, WM_SYSCOMMAND, SC_RESTORE, 0);
		SetForegroundWindow(h);
	}
	else
	{
		int static const width = 550;
		int static const height = 500;
		RECT r;
		GetClientRect(GetDesktopWindow(), &r);
		EditOperatorsWindow *o = new EditOperatorsWindow;
		o->hWnd = WindowTemplate(o, typeWindow, L"Редактирование операторов", IDI_settings
			, (r.right - width) / 2
			, (r.bottom - height) / 2
			, width
			, height
			, (HBRUSH)(COLOR_MENU+1)
			);

		ShowWindow(o->hWnd, SW_SHOW);
		o->gridViewer.typeSizeBaseGrid.Init();
		SetRow(o->grid.hWnd, o->gridViewer.typeSizeBaseGrid.Count());
		ListView_Update(o->grid.hWnd, 0);
	}
}
//----------------------------------------------------------------------------------------
void EditOperatorsWindow::CloseBtn(TCommand &m)
{
	DestroyWindow(m.hwnd);
}
//--------------------------------------------------------------------------------------
void EditOperatorsWindow::AddBtn(TCommand &m)
{
	if(TypesizePasswordDlg().Do(m.hwnd))
	{
		EditOperatorsWindow *o = (EditOperatorsWindow *)GetWindowLongPtr(m.hwnd, GWLP_USERDATA);
		if(NULL != o)
		{
			if(o->gridViewer.typeSizeBaseGrid.Add(m.hwnd, o->gridViewer.hEdits))
			{
				o->gridViewer.typeSizeBaseGrid.Update();
				SetRow(o->grid.hWnd, 1);
				ListView_Update(o->grid.hWnd, 0);
			}
		}
	}
}
//-----------------------------------------------------------------------------------------
void EditOperatorsWindow::DeleteBtn(TCommand &m)
{
	if(TypesizePasswordDlg().Do(m.hwnd))
	{
		EditOperatorsWindow *o = (EditOperatorsWindow *)GetWindowLongPtr(m.hwnd, GWLP_USERDATA);
		if(NULL != o)
		{
			if(o->gridViewer.typeSizeBaseGrid.Delete(m.hwnd, o->gridViewer.hEdits))
			{
				o->gridViewer.typeSizeBaseGrid.Update();
				ListView_DeleteItem(o->grid.hWnd, 0); 
				ListView_Update(o->grid.hWnd, 0);
			}
		}
	}
}
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void SelectOperatorsWindow::operator()(TSize &m)
{
	if(m.resizing == SIZE_MINIMIZED || 0 == m.Width || 0 == m.Height) return;
	int y = m.Height - 40;
	grid.Size(0, 0, m.Width, y);
	y += 5;
	int left = 40;
	MoveWindow(hCloseBtn, left, y, 100 - 5, 25, TRUE);
}
//------------------------------------------------------------------------
void SelectOperatorsWindow::operator()(TCommand &m)
{
	GetMenuToolBarEvent(m);
}
//------------------------------------------------------------------------
void SelectOperatorsWindow::operator()(TGetMinMaxInfo &m)
{
	if(NULL != m.pMinMaxInfo)
	{
		m.pMinMaxInfo->ptMinTrackSize.x = 550;
		m.pMinMaxInfo->ptMinTrackSize.y = 400;
		m.pMinMaxInfo->ptMaxTrackSize.x = 550;
		m.pMinMaxInfo->ptMaxTrackSize.y = 600;		
	}		
}
//------------------------------------------------------------------------
unsigned SelectOperatorsWindow::operator()(TCreate &m)
{
	grid.Create(m, &gridViewer);
	for(int i = 0; i < dimention_of(gridViewer.hEdits); ++i)
	{
		gridViewer.hEdits[i] = CreateWindowEx(WS_EX_CLIENTEDGE, L"edit", L""
			, WS_VISIBLE | WS_CHILD | ES_LEFT | WS_TABSTOP
			,0, 0, 0, 0, m.hwnd, 0, hInstance, NULL
			);
	}
	hCloseBtn = CreateWindow(L"button", L"Закрыть"
		, WS_VISIBLE | WS_CHILD | WS_TABSTOP
		,0, 0, 0, 0, m.hwnd, (HMENU)0, hInstance, NULL
		);
	SetWindowLong(hCloseBtn, GWL_USERDATA, (LONG)&SelectOperatorsWindow::CloseBtn);
	return 0;
}
//------------------------------------------------------------------------
void SelectOperatorsWindow::operator()(TDestroy &l)
{
	SetWindowLong(l.hwnd, GWL_USERDATA, 0);
	delete this;
}
//------------------------------------------------------------------------
unsigned SelectOperatorsWindow::operator()(TNotify &l)
{
	GridNotify *x = (GridNotify *)GetWindowLongPtr(l.pnmh->hwndFrom, GWLP_USERDATA);
	if(0 == IsBadReadPtr(x, sizeof(x)))return x->Notify(l);
	return 0;
}
//-------------------------------------------------------------------------------
void SelectOperatorsWindow::Do(HWND h, HWND (&hRec)[2])
{
	wchar_t * typeWindow = L"SelectOperatorsWindow";
	if(HWND h = FindWindow(typeWindow, 0))
	{
		SendMessage(h, WM_SYSCOMMAND, SC_RESTORE, 0);
		SetForegroundWindow(h);
	}
	else
	{
		int static const width = 550;
		int static const height = 500;
		RECT r;
		GetClientRect(GetDesktopWindow(), &r);
		SelectOperatorsWindow *o = new SelectOperatorsWindow;	
		o->hWnd = WindowTemplate(o, typeWindow, L"Выбор оператора", IDI_settings
			, (r.right - width) / 2
			, (r.bottom - height) / 2
			, width
			, height
			, (HBRUSH)(COLOR_MENU+1)
			);

		ShowWindow(o->hWnd, SW_SHOW);
		o->gridViewer.typeSizeBaseGrid.Init();
		SetRow(o->grid.hWnd, o->gridViewer.typeSizeBaseGrid.Count());
		ListView_Update(o->grid.hWnd, 0);
		for(int i = 0; i < dimention_of(o->gridViewer.hEdits); ++i)o->gridViewer.hEdits[i] = hRec[i];
	}
}
//----------------------------------------------------------------------------------------
void SelectOperatorsWindow::CloseBtn(TCommand &m)
{
	DestroyWindow(m.hwnd);
}
//--------------------------------------------------------------------------------------

