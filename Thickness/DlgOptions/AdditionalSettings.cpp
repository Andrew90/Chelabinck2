#include "stdafx.h"
#include "ParamDlg.h"
#include "ParamDlg.hpp"
#include "Dialogs.h"
#include "AppBase.h"
/*
MIN_VALUE(UsefulPiecePipe, 100)
MAX_VALUE(UsefulPiecePipe, 500)
PARAM_TITLE(UsefulPiecePipe, L"��������� ����� �����(� �����)")
*/
MIN_VALUE(ReferenceOffset1, 500)
MAX_VALUE(ReferenceOffset1, 15000)
PARAM_TITLE(ReferenceOffset1, L"������� ���������� 1 (��)")

MIN_VALUE(ReferenceOffset2, 200)
MAX_VALUE(ReferenceOffset2, 15000)
PARAM_TITLE(ReferenceOffset2, L"������� ���������� 2 (��)")

MIN_VALUE(ReferenceOffset3, 500)
MAX_VALUE(ReferenceOffset3, 15000)
PARAM_TITLE(ReferenceOffset3, L"������� ���������� 3 (��)")

MIN_VALUE(ReferenceOffset4, 200)
MAX_VALUE(ReferenceOffset4, 15000)
PARAM_TITLE(ReferenceOffset4, L"������� ���������� 4 (��)")

MIN_VALUE(SupplySensorDelay, 0)
MAX_VALUE(SupplySensorDelay, 99000)
PARAM_TITLE(SupplySensorDelay, L"�������� ������� �������")

MIN_VALUE(RemoveSensorDelay, 0)
MAX_VALUE(RemoveSensorDelay, 99000)
PARAM_TITLE(RemoveSensorDelay, L"�������� ������ �������")

MIN_VALUE(FrequencyFrames, 10)
MAX_VALUE(FrequencyFrames, 1000)
PARAM_TITLE(FrequencyFrames, L"������� ������ ��� (��)")

void AdditionalSettingsDlg::Do(HWND h)
{
	if(TemplDialog<AdditionalSettingsTable>(Singleton<AdditionalSettingsTable>::Instance()).Do(h, L"�������������� ���������"))
	{
	}
}


