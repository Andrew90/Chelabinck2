#include "stdafx.h"
#include "DialogBeforeCycle.h"
#include "MainWindow.h"
#include "MainWindowToolBar.h"
#include "StoredData.h"
#include "AppBase.h"
//------------------------------------------------------------------------------------
int sycles;
int current;
void DialogBeforeCycle::Init(int countCycles)
{
	sycles = countCycles;
	current = CountTubesInPacket();
}
//------------------------------------------------------------------------------------
void DialogBeforeCycle::Test()
{
	if(++current >= sycles)
	{
		StopMeashurement(mainWindow.hWnd);
		SendMessage(mainWindow.hStatusWindow, SB_SETTEXT, 1, (LONG)L""); 
	}
	wchar_t buf[256];
	wsprintf(buf, L"труб в пачке %d осталось труб %d", sycles, sycles - current);
	SendMessage(mainWindow.hStatusWindow, SB_SETTEXT,2, (LONG)buf); 
}
//------------------------------------------------------------------------------------
