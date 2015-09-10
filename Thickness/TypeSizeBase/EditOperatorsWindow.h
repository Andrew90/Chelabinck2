#pragma once
#include <windows.h>
#include "message.h"
#include "Grid.h"
#include "EditOperatorsViewer.h"
class EditOperatorsWindow
{
public:
	HWND hWnd, hCloseBtn, hAddBtn, hDeleteBtn;
	EditOperatorsGrid gridViewer;
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

class SelectOperatorsWindow
{
public:
	HWND hWnd, hCloseBtn;
	SelectOperatorGrid gridViewer;
	GridNotify grid;
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	unsigned operator()(TCreate &);
	void operator()(TDestroy &);
	unsigned operator()(TNotify &);
	static void CloseBtn(TCommand &);

	static void Do(HWND, HWND (&)[2]);
};