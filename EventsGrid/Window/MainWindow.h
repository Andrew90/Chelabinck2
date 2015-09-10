#pragma once

#include <windows.h>
#include "message.h"
#include "Grid.h"
class MainWindow
{
public:
//	static const int width = 710;
//	static const int height = 400;
public:
	HWND hWnd;
	HWND hStatusWindow;
	GridNotify grid;
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	unsigned operator()(TNotify &);
	unsigned operator()(TCreate &);
	void operator()(TDestroy &);
	unsigned operator()(TMessage &);
};
extern MainWindow mainWindow;
