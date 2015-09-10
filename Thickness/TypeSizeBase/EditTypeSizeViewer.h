#pragma once
#include <Windows.h>
#include "GridItems.h"
#include "TypeSizeBaseGrid.h"

class EditTypeSizeGrid : public GridHandlers
{
public:
	TypeSizeBaseGrid typeSizeBaseGrid;
public:
	HWND hEdits[10];	
public:	
	void SetHeaderToGrid(HWND);
	void SetDataToGrid(LV_DISPINFO *);
	void LClick(LPNMITEMACTIVATE );
};

class SelectTypeSizeGrid : public EditTypeSizeGrid
{
public:
	void LClick(LPNMITEMACTIVATE );
};