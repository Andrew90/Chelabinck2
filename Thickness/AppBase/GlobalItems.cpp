#include "stdafx.h"
#include "GlobalItems.h"
#include "ArchiveEvents.h"
#include "MainLoop.h"
#include "AppBase.h"
#include "Automat.h"
#include "Device1730.h"
#include "Lan.h"
#include "OutBitDown.h"
#include "SQEvents.h"
#include "Controller.h"
#include "Config.h"

bool InitGlobalData()
{
	AppBase().Init();	
	
	initGlobalUnitOk = true;
	if(!device1730.Init(Singleton<NamePlate1730ParametersTable>::Instance().items.get<NamePlate1730>().value))
	{
		MessageBox(0, L"Не могу инициировать плату 1730", L"Ошибка !!!", MB_ICONERROR);
		initGlobalUnitOk = false;
		return false;
	}
#if 0
	unsigned t = 0x100;
	device1730.Write(t);
	Sleep(3000);
	t = 0x140;
	device1730.Write(t);
	Sleep(3000);
#endif	
	device1730.Write(0);
	unsigned res = Lan::Init();
	if(0 != res)
	{
		wchar_t buf[256];
		wsprintf(buf, L"Не могу инициировать плату Lan10m8-100 номер %d", res);
		MessageBox(0, buf, L"Ошибка !!!", MB_ICONERROR);
		initGlobalUnitOk = false;
		return false;
	}
	sQEvents.Init();
	automats.Init();
	mainLoop.Init();	
	outBitDown.Init();
	return true;
}

void DestroyGlobalData()
{
	HWND hh = FindWindow(L"ManagementIOportsDlg", 0);
	if(NULL != hh)
	{
	  DestroyWindow(hh);
	}
	device1730.Write(0);
	mainLoop.Destroy();
	automats.Destroy();
	Lan::Destroy();
	device1730.Destroy();	
}

ArchiveEvents archiveEvents;
MainLoop mainLoop;
OutBitDown outBitDown;
Automats automats;
Controller controller;
#ifndef DEBUG_ITEMS
 Device1730 device1730;
#else
 Debug1730 device1730;
#endif

bool initGlobalUnitOk;