#include "stdafx.h"
#include "ParamDlg.h"
#include "ParamDlg.hpp"
#include "Dialogs.h"
#include "AppBase.h"
/*
MIN_VALUE(UsefulPiecePipe, 100)
MAX_VALUE(UsefulPiecePipe, 500)
PARAM_TITLE(UsefulPiecePipe, L"Пригодная длина трубы(в зонах)")
*/
MIN_VALUE(ReferenceOffset1, 500)
MAX_VALUE(ReferenceOffset1, 15000)
PARAM_TITLE(ReferenceOffset1, L"Базовое расстояние 1 (мм)")

MIN_VALUE(ReferenceOffset2, 200)
MAX_VALUE(ReferenceOffset2, 15000)
PARAM_TITLE(ReferenceOffset2, L"Базовое расстояние 2 (мм)")

MIN_VALUE(ReferenceOffset3, 500)
MAX_VALUE(ReferenceOffset3, 15000)
PARAM_TITLE(ReferenceOffset3, L"Базовое расстояние 3 (мм)")

MIN_VALUE(ReferenceOffset4, 200)
MAX_VALUE(ReferenceOffset4, 15000)
PARAM_TITLE(ReferenceOffset4, L"Базовое расстояние 4 (мм)")

MIN_VALUE(SupplySensorDelay, 0)
MAX_VALUE(SupplySensorDelay, 99000)
PARAM_TITLE(SupplySensorDelay, L"Задержка подвода датчика")

MIN_VALUE(RemoveSensorDelay, 0)
MAX_VALUE(RemoveSensorDelay, 99000)
PARAM_TITLE(RemoveSensorDelay, L"Задержка отвода датчика")

MIN_VALUE(FrequencyFrames, 10)
MAX_VALUE(FrequencyFrames, 1000)
PARAM_TITLE(FrequencyFrames, L"Частота кадров БОС (Гц)")

void AdditionalSettingsDlg::Do(HWND h)
{
	if(TemplDialog<AdditionalSettingsTable>(Singleton<AdditionalSettingsTable>::Instance()).Do(h, L"Дополнительные настройки"))
	{
	}
}


