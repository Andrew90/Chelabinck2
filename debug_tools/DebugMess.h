#pragma once
#define XDEBUG
#ifdef XDEBUG
#include <windows.h>
struct TMapData
{
	unsigned char head;
	unsigned char tail;
	char data[256][512];
};
class DebugMess
{
	HANDLE hMapFile;
    TMapData *map;
public:
	~DebugMess(void);
	void Init();
	void print(char *, ...);
};

class ViewerDebugMess
{
	HANDLE hMapFile;
    TMapData *map;
public:
	ViewerDebugMess(void);
public:
	~ViewerDebugMess(void);
	char *get();
};

extern DebugMess debug;
#define DBG(x) x
#else
//int noused_proc(...);
//#define dprint sizeof noused_proc
#define DBG(x)
#endif
