#include "stdafx.h"
#include "ParamDlg.h"
#include "ParamDlg.hpp"
#include "Dialogs.h"

#define PARA(n)\
MIN_VALUE(CoefficientA<n>, 0.0)\
MAX_VALUE(CoefficientA<n>, 1.0)\
MIN_VALUE(CoefficientB<n>, -3.0)\
MAX_VALUE(CoefficientB<n>, 3.0)\
PARAM_TITLE(CoefficientA<n>, L"Датчик "##L###n##L" коэффициент А")\
PARAM_TITLE(CoefficientB<n>, L"Датчик "##L###n##L" коэффициент В")

PARA(1)
PARA(2)
PARA(3)
PARA(4)
PARA(5)
PARA(6)

#undef PARA

//TABLE_ONE_ROW(CoefficientParametersTable)

void CoefficientDlg::Do(HWND h)
{
	if(TemplDialog<CoefficientParametersTable>(Singleton<CoefficientParametersTable>::Instance()).Do(h, L"Коэффициенты пересчёта"))
	{
		;
	}
}

