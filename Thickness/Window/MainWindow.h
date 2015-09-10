#pragma once

#include <windows.h>
#include "message.h"
#include "SelectTypeSize.h"
class MainWindow
{
public:
	//SelectTypeSize select;
	bool sizingWindow;
public:
	HWND hWnd;
	HWND hStatusWindow;
	HWND hToolBar;	
	bool options;
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	unsigned operator()(TCreate &);
	void operator()(TDestroy &);
	void operator()(TMouseWell &);
	void operator()(TSizing &);
	void operator()(TKeyDown &);
	void DisableMemuItemFile();
	void EnableMemuItemFile();
	void BottomLabel(int, wchar_t *);
};
extern MainWindow mainWindow;
