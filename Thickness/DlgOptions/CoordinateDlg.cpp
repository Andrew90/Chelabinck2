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
		ThicknessWindowTable o;
		o.items.get<OffsetWindowX>().value = r.left;
		o.items.get<OffsetWindowY>().value = r.top;
		o.items.get<WindowWidth>().value = r.right - r.left;
		o.items.get<WindowHeight>().value = r.bottom - r.top;
		UpdateWhere<ThicknessWindowTable>(o, base).ID(1).Execute();
	}
}