#pragma once
#include "stdafx.h"
#include "Dialogs.h"
#include "ManagementIOports.h"
#include "EmptyWindow.h"
#include "AppBase.h"
#include "mainwindow.h"
#include "Pass.h"
#include <CommCtrl.h>
#include <WindowsX.h>
#if 0
void ManagementIOportsDlg::Do(HWND h)
{
	HWND hh = FindWindow(L"ManagementIOportsDlg", 0);
	if(NULL != hh)
	{
		SendMessage(hh, WM_SYSCOMMAND, SC_RESTORE, 0);
		SetForegroundWindow(hh);
		return;
	}
	if(SendMessage(mainWindow.hToolBar, TB_ISBUTTONENABLED, IDB_SensorsUnit, 0))
	{
		if(OptionPasswordDlg().Do(h))
		{
			CBase base(ParametersBase().name());
			if(base.IsOpen())
			{		
				ManagementIOportsTable o;
				Select<ManagementIOportsTable>(base).ID(1).Execute(o);
				h = WindowTemplate(
					&managementIOports
					, L"ManagementIOportsDlg"
					, L"Управление дискретными выходами"
					, IDI_settings
					, o.items.get<OffsetWindowX>().value
					, o.items.get<OffsetWindowY>().value
					, managementIOports.width
					, managementIOports.height
					);
				ShowWindow(h, SW_SHOWNORMAL);
			}
		}
	}
	else
	{
		MessageBox(h, L"Необходимо перейти в режим \"Наладка\"", L"Сообщение", MB_ICONINFORMATION);
	}
}
#else
void ManagementIOportsDlg::Do(HWND h)
{
	HWND hh = FindWindow(L"ManagementIOportsDlg", 0);
	if(NULL != hh)
	{
		SendMessage(hh, WM_SYSCOMMAND, SC_RESTORE, 0);
		SetForegroundWindow(hh);
	}
	else
	{
		ManagementIOportsTable &o = Singleton<ManagementIOportsTable>::Instance();
		h = WindowTemplate(
			&managementIOports
			, L"ManagementIOportsDlg"
			, L""
			, IDI_settings
			, o.items.get<OffsetWindowX>().value
			, o.items.get<OffsetWindowY>().value
			, managementIOports.width
			, managementIOports.height
			);
		ShowWindow(h, SW_SHOWNORMAL);
	}
	SendTitle();
}
void ManagementIOportsDlg::SendTitle()
{
	HWND hh = FindWindow(L"ManagementIOportsDlg", 0);
	if(NULL != hh)
	{
		wchar_t *mess = L"Просмотр дискретных входов-выходов";
		if(SendMessage(mainWindow.hToolBar, TB_ISBUTTONENABLED, IDB_SensorsUnit, 0))
		{
			mess = L"Управление дискретными выходами";
		}
		SetWindowText(hh, mess);
		SendMessage(hh, WM_SYSCOMMAND, SC_RESTORE, 0);
		SetForegroundWindow(hh);
	}
}
#endif