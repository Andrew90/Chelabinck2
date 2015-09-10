#pragma once
#include <string>
class OperatorsBaseGrid
{
public:
	struct Impl;
	Impl *impl;
public:	
	OperatorsBaseGrid();
	void Init();
	void Update();
	bool Add(HWND, HWND *);
	bool Delete(HWND, HWND *);
    wchar_t *Cell(unsigned, unsigned);
	int Count();
};