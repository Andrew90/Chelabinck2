#pragma once
#include <Windows.h>
struct ParametesCycle
{
	static bool Do(HWND);
	static void Init();
};

struct ParametesCycleTest
{
	static bool Do(HWND h);
};

struct StatusCycle
{
	static void Update();
	static void Do(HWND);
};
