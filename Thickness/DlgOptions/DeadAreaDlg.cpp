#include "stdafx.h"
#include "ParamDlg.h"
#include "ParamDlg.hpp"
#include "Dialogs.h"
#include "Pass.h"

MIN_VALUE(DeadAreaMM0, 0)
MAX_VALUE(DeadAreaMM0, 500)

MIN_VALUE(DeadAreaMM1, 0)
MAX_VALUE(DeadAreaMM1, 500)

PARAM_TITLE(DeadAreaMM0, L"Начало трубы")
PARAM_TITLE(DeadAreaMM1, L"Конец трубы")

void DeadAreaDlg::Do(HWND h)
{
	if(TemplDialog<DeadAreaTable>(Singleton<DeadAreaTable>::Instance()).Do(h, L"Мёртвые зоны"))
	{
	}
}

