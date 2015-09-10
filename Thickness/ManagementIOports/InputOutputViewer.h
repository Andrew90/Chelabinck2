#pragma once
#include "InOutBits.h"
#include <GdiPlus.h>
#include "message.h"
class InputOutputViewer
{
	HANDLE hTimer;
public:
	void Size(Gdiplus::Graphics &, int, int);
	void Start(PVOID);
	void Stop();
	void MouseLDown(TLButtonDown &);
};