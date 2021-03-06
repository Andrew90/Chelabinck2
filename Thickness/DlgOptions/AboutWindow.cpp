#include "stdafx.h"
#include "Dialogs.h"

static const wchar_t *about_text[] = {
	L"��� ��� �� '����'"
	, L"���� 2014 �."
	, L"������ 5.3"
};
extern HINSTANCE hInstance;
LRESULT CALLBACK Proc(HWND h, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_COMMAND:
		switch(wParam)
		{
		case IDOK:
			{
				EndDialog(h, TRUE);
			}
			return TRUE;
		}
		break;
	case WM_INITDIALOG:
		{			
			SetWindowLong(h, GWL_USERDATA, lParam);
			int width = 450;
			int dy = 10;

			for(int i = 0; i < dimention_of(about_text); ++i)
			{
				CreateWindow(L"static", about_text[i]
					, WS_VISIBLE | WS_CHILD
					, 10, dy + 3, width, 20, h, 0, hInstance, NULL
					);
				dy += 20;
			}

			int offs = width / 2 - 55;
			dy += 5;
			CreateWindow(L"button", L"�������"
				, WS_VISIBLE | WS_CHILD | WS_TABSTOP
				,offs, dy, 110, 30, h, (HMENU)IDOK, hInstance, NULL
				);
			RECT r;
			GetWindowRect(GetDesktopWindow(), &r);
			
			dy += 73;
			int x = r.left +(r.right - r.left - width) / 2;
			int y = r.top +(r.bottom - r.top - dy) / 2;
			MoveWindow(h, x, y, width, dy, FALSE);
		}
		return TRUE;
	}
	return FALSE;
}

#pragma warning(disable : 4996)
void AboutWindow::Do(HWND hWnd)
{
	char *p = (char *)LocalAlloc(LPTR, 2048);
	DLGTEMPLATE &d = *(DLGTEMPLATE *)p;
	d.style = DS_MODALFRAME | WS_POPUP | WS_CAPTION;
	d.dwExtendedStyle = 0;
	d.cdit = 0;
	wchar_t *c = (wchar_t *)&p[sizeof(DLGTEMPLATE) + 4];
    wcscpy(c, L"� ���������");
	
	DialogBoxIndirectParam(hInstance, &d, hWnd, (DLGPROC)Proc, NULL);
    LocalFree((HLOCAL)p);
}