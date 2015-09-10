#include "stdafx.h"
#include "ParamDlg.h"
#include "ParamDlg.hpp"
#include "Dialogs.h"
#include "ThicknessViewer.h"
#include "Emptywindow.h"
#include "Pass.h"

MIN_EQUAL_VALUE(ACFBorderLeft, 0)
MAX_EQUAL_VALUE(ACFBorderLeft, 512)

MIN_EQUAL_VALUE(ACFBorderRight, 0)
MAX_EQUAL_VALUE(ACFBorderRight, 512)

PARAM_TITLE(ACFBorderLeft, L"Отсечение левой границы АЧХ")
PARAM_TITLE(ACFBorderRight, L"Отсечение правой границы АЧХ")

void ACFBorderDlg::Do(HWND h)
{
	if(TemplDialog<ACFBorderTable>(Singleton<ACFBorderTable>::Instance()).Do(h, L"Границы АЧХ"))
	{
		RepaintWindow(thicknessViewer.hWnd);
	}
}