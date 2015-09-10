#pragma once

#include <windows.h>
#include "message.h"
#include "InputOutputViewer.h"
class ManagementIOports
{
public:
	static const int width = 400;
	static const int height = 300;
	Gdiplus::Bitmap *backScreen;
	InputOutputViewer viewer;
public:
	HWND hWnd;
	ManagementIOports();
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	unsigned operator()(TNotify &);
	unsigned operator()(TCreate &);
	void operator()(TDestroy &);
	unsigned operator()(TMessage &);
	void operator()(TPaint &l);
	void operator()(TLButtonDown &);
};
extern ManagementIOports managementIOports;
