#pragma once
#include <windows.h>

class MainLoop
{
	void (*proc)(unsigned &, unsigned &);
	HANDLE hThread;
	HANDLE hEvent[2];
	unsigned &input;
	unsigned &output;
	bool loop;
	void Loop();
	static void Proc(MainLoop *);
public:
	MainLoop();
	void Init();
	void Destroy();
	void SetLoop(void (*)(unsigned &, unsigned &));
};

extern MainLoop mainLoop;