#pragma once
//#include <windows.h>
#include "message.h"
#if 0
class SelectTypeSizeNew
{
public:
	static void Do(HWND);
};
class SelectTypeSizeDelete
{
public:
	static void Do(HWND);
};

class SelectTypeSize
{
public:
   HWND hWnd;
public:   
	void Create(HWND);
	void Size(int x, int y, int width);
	static void Do(TCommand &);
};
#endif