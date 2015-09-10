#pragma once
#include <string>

class TypeSizeBaseGrid
{
public:
	struct Impl;
	Impl *impl;
public:	
	TypeSizeBaseGrid();
	void Init();
	void Update();
	bool Add(HWND, HWND *);
	bool Delete(HWND, HWND *);
    wchar_t *Cell(unsigned, unsigned);
	int Count();
	template<class Dst>void GetData(Dst &dst, int row);
};

template<class Dst>void TypeSizeBaseGrid::GetData(Dst &dst, int row)
{
}

