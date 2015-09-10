#include "stdafx.h"
#include "ParamDlg.h"
#include "ParamDlg.hpp"
#include "Dialogs.h"
#include "Pass.h"

MIN_VALUE(SprayTime, 0)
MAX_VALUE(SprayTime, 1000)

MIN_VALUE(CorrectionOffsetTick, -5000)
MAX_VALUE(CorrectionOffsetTick, 5000)

PARAM_TITLE(SprayTime, L"Время распыления")
PARAM_TITLE(CorrectionOffsetTick, L"Корректировка смещения по времени")

//TABLE_ONE_ROW(PaintMakerTable)

void PaintMakerDlg::Do(HWND h)
{
	if(TemplDialog<PaintMakerTable>(Singleton<PaintMakerTable>::Instance()).Do(h, L"Дефектоотметчик"))
	{
	}
}

