#pragma once
#include <windows.h>
#include "message.h"
#include "../resource.h"
#include "WindowsEventTemplate.hpp"
//---------------------------------------------------------------------------------
ATOM MyRegisterClass(WNDPROC WndProc
	, const wchar_t *windowClassName
	, int style = CS_HREDRAW | CS_VREDRAW
	, int idIcon = 0
	, int idSmIcon = 0
	, HBRUSH backGround = NULL					 
	);

HWND MyCreateWindow(const wchar_t *windowClass, const wchar_t *title = NULL
	, void *data = NULL
	, int style = WS_OVERLAPPEDWINDOW
	, HWND hParent = NULL
	, int x = CW_USEDEFAULT
	, int y = CW_USEDEFAULT
	, int width = CW_USEDEFAULT
	, int height = CW_USEDEFAULT
	);

HWND CreateChildWindow(HWND hParent, WNDPROC WndProc, wchar_t *classStr, void *data);

template<class T>HWND WindowTemplate(
	T *self
	, wchar_t *className
	, wchar_t *title
	, int styleS = IDI_UPLOAD
	, int x = CW_USEDEFAULT
	, int y = CW_USEDEFAULT
	, int width = CW_USEDEFAULT
	, int height = CW_USEDEFAULT
	, HBRUSH backGround = NULL	
	)
{
	MyRegisterClass(Viewer<T>::Proc, className,  CS_HREDRAW | CS_VREDRAW, styleS, styleS, backGround);
	HWND hWnd = MyCreateWindow(className, title, self, WS_OVERLAPPEDWINDOW, NULL, x, y, width, height);
	return hWnd;
}

void RepaintWindow(HWND h);


