#pragma once
#include "Config.h"
#ifdef DEBUG_ITEMS
#include "windows.h"
class Debug1730
{
	unsigned input;
	unsigned output;
	unsigned t_output;
	HANDLE hThread;
	bool contine;
	static void ThreadProc(Debug1730 *);
	void Do();
public:
	Debug1730();
	void Start();
	bool Init(wchar_t *){return true;};
	void Destroy();
	bool IsOpen();
	unsigned Read();
	void Write();
	void Write(unsigned);
	unsigned ReadOutput();
};

#endif