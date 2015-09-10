#include "stdafx.h"
#include "Dialogs.h"
#include "AppBase.h"

void CoordinatesDlg::Do(HWND h)
{
	CBase base(ParametersBase().name());
	if(base.IsOpen())
	{
		RECT r;
		GetWindowRect(h, &r);
		InputOutputWindowTable o;
		o.items.get<OffsetWindowX>().value = r.left;
		o.items.get<OffsetWindowY>().value = r.top;
		UpdateWhere<InputOutputWindowTable>(o, base).ID(1).Execute();
	}
}