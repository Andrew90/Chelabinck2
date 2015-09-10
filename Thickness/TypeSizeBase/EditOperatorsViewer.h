#pragma once
#include <Windows.h>
#include "GridItems.h"
#include "OperatorsBaseGrid.h"
class EditOperatorsGrid : public GridHandlers
{
public:
	OperatorsBaseGrid typeSizeBaseGrid;
public:
	HWND hEdits[2];	
public:	
	void SetHeaderToGrid(HWND);
	void SetDataToGrid(LV_DISPINFO *);
	void LClick(LPNMITEMACTIVATE );
};

class SelectOperatorGrid : public EditOperatorsGrid
{
public:	
	void LClick(LPNMITEMACTIVATE );
};