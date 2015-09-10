#include "stdafx.h"
#include <windows.h>
#include "ParamDlg.h"
#include "ParamDlg.hpp"
#include "Dialogs.h"
#include "AppBase.h"
#include "DlgFiles.h"
#include "DebugMess.h"

void __color_wm_paint(HWND hwnd, int color)
{
	PAINTSTRUCT p;
	HDC hdc = BeginPaint(hwnd, &p);
	{		
		Gdiplus::Graphics g(hdc);
		RECT r;
		GetClientRect(hwnd, &r);
		g.FillRectangle(&Gdiplus::SolidBrush(Gdiplus::Color(color)), 0, 0, r.right, r.bottom);
	}
	EndPaint(hwnd, &p);
}

int __color_l_button_down(HWND hwnd)
{
	dprint("%s", __FUNCTION__);
	ColorDialog c(hwnd);
	char d[4] = {-1};
	if(c())
	{
		dprint("%s %x", __FUNCTION__, c.color.rgbResult);

		*(int *)d = c.color.rgbResult | 0xff000000;
		char t = d[0];
		d[0] = d[2];
		d[2] = t;
		InvalidateRect(hwnd, NULL, true);
	}
	return *(int *)d;
}

template<class T>struct __color__
{	
	static LRESULT CALLBACK Proc_(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch(msg)
		{
		case WM_PAINT:
			{
				T *t = (T *)GetWindowLong(hwnd, GWL_USERDATA);
				if(t)
				{
					__color_wm_paint(hwnd, t->value);
				}
			}
			return 0;
		case WM_LBUTTONDBLCLK:
		case WM_LBUTTONDOWN:
			{
				T *t = (T *)GetWindowLong(hwnd, GWL_USERDATA);
				if(t)
				{
			    t->value = __color_l_button_down(hwnd);
				}
			}
			return 0;
		}
		return CallWindowProc(OldWndProc, hwnd, msg, wParam, lParam);
	}
	static WNDPROC OldWndProc;
};

 template<class T>WNDPROC __color__<T>::OldWndProc;

template<class T>struct ColorButtonItem
{
	HWND Init(HWND h, int &width, int &dy, T &t)
	{
		HWND hWnd = CreateWindow(L"button", L""
			, WS_VISIBLE | WS_CHILD | WS_TABSTOP
			,10, dy, 100, 30, h, (HMENU)0, hInstance, 0
			);
		__color__<T>::OldWndProc = (WNDPROC)GetWindowLong(hWnd, GWL_WNDPROC);
		SetWindowLong(hWnd, GWL_WNDPROC, (long ) __color__<T>::Proc_);
		SetWindowLong(hWnd, GWL_USERDATA, (long )&t);
		CreateWindow(L"static", ParamTitle<T>()()
			, WS_VISIBLE | WS_CHILD
			, 100 + 20, dy + 7, dlg_width, 20, h, 0, hInstance, NULL
			);
		dy += 35;

		return hWnd;
	}
};

#define DLG_SUB(n)template<>struct DlgSubItems<n, int>: ColorButtonItem<n>{};\
template<>struct __data_from_widget__<DlgItem<n>, int>\
{\
    int operator()(DlgItem<n> &o)\
	{\
		return o.value.value;\
	}\
};

DLG_SUB(LessMinimumEnergy)
//DLG_SUB(GreaterMaximumEnergy)
DLG_SUB(ValueLessThreshold)
DLG_SUB(ThicknessLessThreshold)
//DLG_SUB(ThicknessGreaterThreshold)
DLG_SUB(Undefined)
DLG_SUB(Nominal)
DLG_SUB(DefectMin)
DLG_SUB(DefectMax)
DLG_SUB(DefectMinMax)
DLG_SUB(DefectDifferentWall)
//------------------------------------------------------------------------------------------------
PARAM_TITLE(LessMinimumEnergy, L"Энергия меньше минимума")
//PARAM_TITLE(GreaterMaximumEnergy, L"Энергия больше максимума")
PARAM_TITLE(ValueLessThreshold, L"Импульс меньше порога")
PARAM_TITLE(ThicknessLessThreshold, L"Измеренная величина меньше минимальной")
//PARAM_TITLE(ThicknessGreaterThreshold, L"Измеренная величина больше максимальной")
PARAM_TITLE(Undefined, L"Результат не определён")
PARAM_TITLE(Nominal, L"Номинальная толщина")
PARAM_TITLE(DefectMin, L"Брак минус")
PARAM_TITLE(DefectMax, L"Брак плюс")
PARAM_TITLE(DefectMinMax, L"Брак плюс-минус")
PARAM_TITLE(DefectDifferentWall, L"Брак разностенность")

DO_NOT_CHECK(LessMinimumEnergy)
//DO_NOT_CHECK(GreaterMaximumEnergy)
DO_NOT_CHECK(ValueLessThreshold)
DO_NOT_CHECK(ThicknessLessThreshold)
//DO_NOT_CHECK(ThicknessGreaterThreshold)
DO_NOT_CHECK(Undefined)
DO_NOT_CHECK(Nominal)
DO_NOT_CHECK(DefectMin)
DO_NOT_CHECK(DefectMax)
DO_NOT_CHECK(DefectMinMax)
DO_NOT_CHECK(DefectDifferentWall)

struct DefaultBtn
{
	static const int width = 160;
	static const int height = 30;
	static const int ID = IDRETRY;
	wchar_t *Title(){return L"Встроенные настройки";}
	template<class Owner>void BtnHandler(Owner &o, HWND h)
	{
		dprint(__FUNCTION__"test");
		if(TestPassword<Owner::Table>()(h))
		{
			CBase base(ParametersBase().name());
			if(base.IsOpen())
			{
				ColorTable color;
				UpdateWhere<ColorTable>(color, base).ID(1).Execute();
				CopyFactory::Do(Singleton<ColorTable>::Instance().items, color.items);
			}
			EndDialog(h, FALSE);
		}
	}
};
void ColorDlg::Do(HWND h)
{
	ColorTable color;
	CopyFactory::Do(color.items, Singleton<ColorTable>::Instance().items);
	if(TemplDialog<ColorTable, TL::MkTlst<OkBtn, CancelBtn, DefaultBtn>::Result>(color).Do(h, L"Цвета"))
	{
		CopyFactory::Do(Singleton<ColorTable>::Instance().items, color.items);
	}
}

