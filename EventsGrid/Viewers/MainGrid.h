#pragma once
#include "GridItems.h"
class MainGrid : public GridHandlers
{
	HANDLE hTimer;
public:	
	int start;
	MainGrid();

	void SetHeaderToGrid(HWND);
	void SetDataToGrid(LV_DISPINFO *);
	void SetColorToGrid(NMLVCUSTOMDRAW *);

	void Start(HWND);
	void Stop();
};

extern MainGrid mainGrid;