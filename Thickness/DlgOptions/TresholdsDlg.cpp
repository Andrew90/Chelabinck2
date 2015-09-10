#include "stdafx.h"
#include "ParamDlg.h"
#include "ParamDlg.hpp"
#include "Dialogs.h"
#include "ThicknessViewer.h"
#include "Emptywindow.h"
#include "Pass.h"
/*
MIN_EQUAL_VALUE(NominalThreshold, 1.0)
MAX_EQUAL_VALUE(NominalThreshold, 30.0)

MIN_EQUAL_VALUE(NominalPercentMin, 1.0)
MAX_EQUAL_VALUE(NominalPercentMin, 30.0)

MIN_EQUAL_VALUE(NominalPercentMax, 1.0)
MAX_EQUAL_VALUE(NominalPercentMax, 30.0)

MIN_EQUAL_VALUE(DefectDifferentWallThreshold, 0.01)
MAX_EQUAL_VALUE(DefectDifferentWallThreshold, 100.0)
*/
MIN_EQUAL_VALUE(MinimumThicknessPipeWall, 1.2)
MAX_EQUAL_VALUE(MinimumThicknessPipeWall, 30.0)

MIN_EQUAL_VALUE(MaximumThicknessPipeWall, 3.0)
MAX_EQUAL_VALUE(MaximumThicknessPipeWall, 30.0)

//PARAM_TITLE(NominalThreshold, L"����������� �������")
//PARAM_TITLE(NominalPercentMin, L"����� ����� �����")
//PARAM_TITLE(NominalPercentMax, L"����� ����� ����")

//PARAM_TITLE(DefectDifferentWallThreshold, L"����� ��������������")

PARAM_TITLE(MinimumThicknessPipeWall, L"����������� ������� ������ �����")
PARAM_TITLE(MaximumThicknessPipeWall, L"������������ ������� ������ �����")

void ThresholdsParamDlg::Do(HWND h)
{
	if(TemplDialog<BorderCredibilityTable>(Singleton<BorderCredibilityTable>::Instance()).Do(h, L"������� ������������� ������ �����"))
	{
		RepaintWindow(thicknessViewer.hWnd);
	}
}

