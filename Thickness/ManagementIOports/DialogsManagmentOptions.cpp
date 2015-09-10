#include "stdafx.h"
#include "DialogsManagmentOptions.h"
#include "AppBase.h"
#include "Pass.h"
void ManagmentOptionsCoordinatesDlg::Do(HWND h)
{	
	CBase base(ParametersBase().name());
	if(base.IsOpen())
	{
		RECT r;
		GetWindowRect(h, &r);
		ManagementIOportsTable &o = Singleton<ManagementIOportsTable>::Instance();
		o.items.get<OffsetWindowX>().value = r.left;
		o.items.get<OffsetWindowY>().value = r.top;
		UpdateWhere<ManagementIOportsTable>(o, base).ID(1).Execute();
	}
}