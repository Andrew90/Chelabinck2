#pragma once
#include <windows.h>
#include "message.h"
#include "Grid.h"
#include "EditTypeSizeViewer.h"
class EditTypeSizeWindow
{
public:
	HWND hWnd, hCloseBtn, hAddBtn, hDeleteBtn;
	EditTypeSizeGrid editTypeSizeGrid;
	GridNotify grid;
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	unsigned operator()(TCreate &);
	void operator()(TDestroy &);
	unsigned operator()(TNotify &);
	static void CloseBtn(TCommand &);
	static void AddBtn(TCommand &);
	static void DeleteBtn(TCommand &);
};

class SelectTypeSizeWindow
{
public:
	HWND hWnd, hCloseBtn;
	SelectTypeSizeGrid editTypeSizeGrid;
	GridNotify grid;
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	unsigned operator()(TCreate &);
	void operator()(TDestroy &);
	unsigned operator()(TNotify &);
	static void CloseBtn(TCommand &);

	static void Do(HWND h, HWND *);
};
