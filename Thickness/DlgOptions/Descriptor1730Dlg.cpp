#include "stdafx.h"
#include "ParamDlg.h"
#include "ParamDlg.hpp"
#include "Dialogs.h"
#include "Device1730.h"

DO_NOT_CHECK(NamePlate1730)

PARAM_TITLE(NamePlate1730, L"Дескриптор")

template<>struct DlgSubItems<NamePlate1730, Holder<128> >: EditItems<NamePlate1730>{};

void Descriptor1730Dlg::Do(HWND h)
{
	if(TemplDialog<NamePlate1730ParametersTable>(Singleton<NamePlate1730ParametersTable>::Instance()).Do(h, L"Дескриптор дискретной платы"))
	{
		device1730.Init(Singleton<NamePlate1730ParametersTable>::Instance().items.get<NamePlate1730>().value);
	}
}
