#include "stdafx.h"
#include "EditTypeSizeWindow.h"
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
void EditTypeSizeWindow::operator()(TSize &m)
{
	if(m.resizing == SIZE_MINIMIZED || 0 == m.Width || 0 == m.Height) return;
	int y = m.Height - 80;
	grid.Size(0, 0, m.Width, y);
	y += 5;
	int left = 5;
	static const int width = 75;
	MoveWindow(editTypeSizeGrid.hEdits[0], left, y, 300 - 5, 25, TRUE);
	left += 300;										  
	MoveWindow(editTypeSizeGrid.hEdits[1], left, y, 200 - 5, 25, TRUE);
	left += 200;										  
	MoveWindow(editTypeSizeGrid.hEdits[2], left, y, width - 5, 25, TRUE);
	left += width;										  
	MoveWindow(editTypeSizeGrid.hEdits[3], left, y, width - 5, 25, TRUE);
	left += width;										  
	MoveWindow(editTypeSizeGrid.hEdits[4], left, y, width - 5, 25, TRUE);
	left += width;										  
	MoveWindow(editTypeSizeGrid.hEdits[5], left, y, width - 5, 25, TRUE);
	left += width;										  
	MoveWindow(editTypeSizeGrid.hEdits[6], left, y, width - 5, 25, TRUE);

	left += width;										  
	MoveWindow(editTypeSizeGrid.hEdits[7], left, y, width - 5, 25, TRUE);
	left += width;										  
	MoveWindow(editTypeSizeGrid.hEdits[8], left, y, width - 5, 25, TRUE);
	left += width;										  
	MoveWindow(editTypeSizeGrid.hEdits[9], left, y, width - 5, 25, TRUE);

	y += 40;
	left = 20;

	MoveWindow(hCloseBtn, left, y, 100 - 5, 25, TRUE);
	left += 110;
	MoveWindow(hAddBtn, left, y, 100 - 5, 25, TRUE);
	left += 110;
	MoveWindow(hDeleteBtn, left, y, 100 - 5, 25, TRUE);
}
//------------------------------------------------------------------------
void EditTypeSizeWindow::operator()(TCommand &m)
{
	GetMenuToolBarEvent(m);
}
//------------------------------------------------------------------------
void EditTypeSizeWindow::operator()(TGetMinMaxInfo &m)
{
	if(NULL != m.pMinMaxInfo)
	{
		m.pMinMaxInfo->ptMinTrackSize.x = 710;
		m.pMinMaxInfo->ptMinTrackSize.y = 400;
		m.pMinMaxInfo->ptMaxTrackSize.x = 1200;
		m.pMinMaxInfo->ptMaxTrackSize.y = 600;		
	}		
}
//------------------------------------------------------------------------
extern HINSTANCE hInstance;
unsigned EditTypeSizeWindow::operator()(TCreate &m)
{
	grid.Create(m, &editTypeSizeGrid);
	for(int i = 0; i < dimention_of(editTypeSizeGrid.hEdits); ++i)
	{
		editTypeSizeGrid.hEdits[i] = CreateWindowEx(WS_EX_CLIENTEDGE, L"edit", L""
			, WS_VISIBLE | WS_CHILD | ES_LEFT | WS_TABSTOP
			,0, 0, 0, 0, m.hwnd, 0, hInstance, NULL
			);
	}
	hCloseBtn = CreateWindow(L"button", L"Закрыть"
		, WS_VISIBLE | WS_CHILD | WS_TABSTOP
		,0, 0, 0, 0, m.hwnd, (HMENU)0, hInstance, NULL
		);
	SetWindowLong(hCloseBtn, GWL_USERDATA, (LONG)&EditTypeSizeWindow::CloseBtn);
	hAddBtn = CreateWindow(L"button", L"Добавить"
		, WS_VISIBLE | WS_CHILD | WS_TABSTOP
		,0, 0, 0, 0, m.hwnd, (HMENU)0, hInstance, NULL
		);
	SetWindowLong(hAddBtn, GWL_USERDATA, (LONG)&EditTypeSizeWindow::AddBtn);
	hDeleteBtn = CreateWindow(L"button", L"Удалить"
		, WS_VISIBLE | WS_CHILD | WS_TABSTOP
		,0, 0, 0, 0, m.hwnd, (HMENU)0, hInstance, NULL
		);
	SetWindowLong(hDeleteBtn, GWL_USERDATA, (LONG)&EditTypeSizeWindow::DeleteBtn);
	return 0;
}
//------------------------------------------------------------------------
void EditTypeSizeWindow::operator()(TDestroy &l)
{
	SetWindowLong(l.hwnd, GWL_USERDATA, 0);
	delete this;
}
//------------------------------------------------------------------------
unsigned EditTypeSizeWindow::operator()(TNotify &l)
{
	GridNotify *x = (GridNotify *)GetWindowLongPtr(l.pnmh->hwndFrom, GWLP_USERDATA);
	if(0 == IsBadReadPtr(x, sizeof(x)))return x->Notify(l);
	return 0;
}
//-------------------------------------------------------------------------------
void EditTypeSizeDlg::Do(HWND h)
{
	wchar_t *typeWindow = L"EditTypeSizeWindow";
	if(HWND h = FindWindow(typeWindow, 0))
	{
		SendMessage(h, WM_SYSCOMMAND, SC_RESTORE, 0);
		SetForegroundWindow(h);
	}
	else
	{
		int static const width = 1130;
		int static const height = 500;
		RECT r;
		GetClientRect(GetDesktopWindow(), &r);
		EditTypeSizeWindow *o = new EditTypeSizeWindow;
		o->hWnd = WindowTemplate(o, typeWindow, L"Редактирование типоразмеров", IDI_settings
			, (r.right - width) / 2
			, (r.bottom - height) / 2
			, width
			, height
			, (HBRUSH)(COLOR_MENU+1)
			);

		ShowWindow(o->hWnd, SW_SHOW);
		o->editTypeSizeGrid.typeSizeBaseGrid.Init();
		SetRow(o->grid.hWnd, o->editTypeSizeGrid.typeSizeBaseGrid.Count());
	}
}
//----------------------------------------------------------------------------------------
void EditTypeSizeWindow::CloseBtn(TCommand &m)
{
	DestroyWindow(m.hwnd);
}
//--------------------------------------------------------------------------------------
void EditTypeSizeWindow::AddBtn(TCommand &m)
{
	if(TypesizePasswordDlg().Do(m.hwnd))
	{
		EditTypeSizeWindow *o = (EditTypeSizeWindow *)GetWindowLongPtr(m.hwnd, GWLP_USERDATA);
		if(NULL != o)
		{
			if(o->editTypeSizeGrid.typeSizeBaseGrid.Add(m.hwnd, o->editTypeSizeGrid.hEdits))
			{
				o->editTypeSizeGrid.typeSizeBaseGrid.Update();
				SetRow(o->grid.hWnd, 1);
				ListView_Update(o->grid.hWnd, 0);
			}
		}
	}
}
//-----------------------------------------------------------------------------------------
void EditTypeSizeWindow::DeleteBtn(TCommand &m)
{
	if(TypesizePasswordDlg().Do(m.hwnd))
	{
		EditTypeSizeWindow *o = (EditTypeSizeWindow *)GetWindowLongPtr(m.hwnd, GWLP_USERDATA);
		if(NULL != o)
		{
			if(o->editTypeSizeGrid.typeSizeBaseGrid.Delete(m.hwnd, o->editTypeSizeGrid.hEdits))
			{
				o->editTypeSizeGrid.typeSizeBaseGrid.Update();
				ListView_DeleteItem(o->grid.hWnd, 0); 
				ListView_Update(o->grid.hWnd, 0);
			}
		}
	}
}
//---------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void SelectTypeSizeWindow::operator()(TSize &m)
{
	if(m.resizing == SIZE_MINIMIZED || 0 == m.Width || 0 == m.Height) return;
	int y = m.Height - 40;
	grid.Size(0, 0, m.Width, y);
	y += 5;
	int left = 40;

	MoveWindow(hCloseBtn, left, y, 100 - 5, 25, TRUE);	
}
//------------------------------------------------------------------------
void SelectTypeSizeWindow::operator()(TCommand &m)
{
	GetMenuToolBarEvent(m);
}
//------------------------------------------------------------------------
void SelectTypeSizeWindow::operator()(TGetMinMaxInfo &m)
{
	if(NULL != m.pMinMaxInfo)
	{
		m.pMinMaxInfo->ptMinTrackSize.x = 710;
		m.pMinMaxInfo->ptMinTrackSize.y = 400;
		m.pMinMaxInfo->ptMaxTrackSize.x = 1200;
		m.pMinMaxInfo->ptMaxTrackSize.y = 600;		
	}		
}
//------------------------------------------------------------------------
unsigned SelectTypeSizeWindow::operator()(TCreate &m)
{
	grid.Create(m, &editTypeSizeGrid);
	for(int i = 0; i < dimention_of(editTypeSizeGrid.hEdits); ++i)
	{
		editTypeSizeGrid.hEdits[i] = CreateWindowEx(WS_EX_CLIENTEDGE, L"edit", L""
			, WS_VISIBLE | WS_CHILD | ES_LEFT | WS_TABSTOP
			,0, 0, 0, 0, m.hwnd, 0, hInstance, NULL
			);
	}
	hCloseBtn = CreateWindow(L"button", L"Закрыть"
		, WS_VISIBLE | WS_CHILD | WS_TABSTOP
		,0, 0, 0, 0, m.hwnd, (HMENU)0, hInstance, NULL
		);
	SetWindowLong(hCloseBtn, GWL_USERDATA, (LONG)CloseBtn);
	return 0;
}
//------------------------------------------------------------------------
void SelectTypeSizeWindow::operator()(TDestroy &l)
{
	SetWindowLong(l.hwnd, GWL_USERDATA, 0);
	delete this;
}
//------------------------------------------------------------------------
unsigned SelectTypeSizeWindow::operator()(TNotify &l)
{
	GridNotify *x = (GridNotify *)GetWindowLongPtr(l.pnmh->hwndFrom, GWLP_USERDATA);
	if(0 == IsBadReadPtr(x, sizeof(x)))return x->Notify(l);
	return 0;
}
//-------------------------------------------------------------------------------
void SelectTypeSizeWindow::Do(HWND h, HWND *hEd)
{
	wchar_t *typeWindow = L"SelectTypeSizeWindow";
	if(HWND h = FindWindow(typeWindow, 0))
	{
		SendMessage(h, WM_SYSCOMMAND, SC_RESTORE, 0);
		SetForegroundWindow(h);
	}
	else
	{
		int static const width = 1130;
		int static const height = 500;
		RECT r;
		GetClientRect(GetDesktopWindow(), &r);
		SelectTypeSizeWindow *o = new SelectTypeSizeWindow;

		o->hWnd = WindowTemplate(o, typeWindow, L"Выбор типоразмера", IDI_settings
			, (r.right - width) / 2
			, (r.bottom - height) / 2
			, width
			, height
			, (HBRUSH)(COLOR_MENU+1)
			);

		ShowWindow(o->hWnd, SW_SHOW);
		o->editTypeSizeGrid.typeSizeBaseGrid.Init();
		SetRow(o->grid.hWnd, o->editTypeSizeGrid.typeSizeBaseGrid.Count());
		for(int i = 0; i < dimention_of(o->editTypeSizeGrid.hEdits); ++i)
		{
			o->editTypeSizeGrid.hEdits[i] = hEd[i];
		}
	}
}
//----------------------------------------------------------------------------------------
void SelectTypeSizeWindow::CloseBtn(TCommand &m)
{
	DestroyWindow(m.hwnd);
}
//--------------------------------------------------------------------------------------
