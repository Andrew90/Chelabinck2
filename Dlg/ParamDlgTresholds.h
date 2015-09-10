#pragma once
#include "BaseParams.h"
#include <typeinfo>
#include "typelist.hpp"
#include "..\AppBaseResult\ParamItemTable.hpp"

struct BaseParamItem
{
	const wchar_t *title;
	int &value;
	HWND hWnd;
	BaseParamItem(int &value, const wchar_t *title): value(value), title(title){}
	void Init(HWND, int &, int &);
};

template<class T>struct TresholdDlgItemTest
{
	template<class Z, class V>bool operator()(HWND hDld, Z &z, V &v)
	{	
		wchar_t buf[256];
		GetWindowText(z.get<TresholdDlgItem<T> >().hWnd, buf, dimention_of(buf));
		v = _wtoi(buf);
		return true;
	}
};
template<class T>struct TresholdDlgItemTest<Min<T> >
{
	template<class Z, class V>bool operator()(HWND h, Z &z, V &v)
	{
		wchar_t buf[256];
		GetWindowText(z.get<TresholdDlgItem<Min<T> > >().hWnd, buf, dimention_of(buf));
		int min = _wtoi(buf);
		v = min;
		GetWindowText(z.get<TresholdDlgItem<Max<T> > >().hWnd, buf, dimention_of(buf));
		int max = _wtoi(buf);
		wsprintf(buf, L"%s должно быть меньше %d или\n%s должно быть больше %d"
			, z.get<TresholdDlgItem<Min<T> > >().title, max
			, z.get<TresholdDlgItem<Max<T> > >().title, min
			);
		if(min > max)
		{
			MessageBox(h, buf, L"Ошибка!!!", MB_ICONEXCLAMATION);
			return false;
		}
		return true;
	}
};
template<class T>struct TresholdDlgItemTxt;
template<class T>struct TresholdDlgItem : BaseParamItem
{
	template<class Z>TresholdDlgItem(Z &p) : BaseParamItem(p.get<T>().value, TresholdDlgItemTxt<T>()()){}
	template<class Z, class V>bool Test(HWND h, Z &z, V &value){return TresholdDlgItemTest<T>()(h, z, value);}
};
#define X(n, txt)template<>struct TresholdDlgItemTxt<n>\
{\
	const wchar_t *operator()(){return txt;}\
};

X(Min<Ucal>, L"Минимальное опорное напряжение")
X(Max<Ucal>, L"Максимальное опорное напряжение")
X(Min<T1g>, L"Минимальный прямой счётчик")
X(Max<T1g>, L"Максимальный прямой счётчик")
X(Min<T2g>, L"Минимальный обратный счётчик")
X(Max<T2g>, L"Максимальный обратный счётчик")
X(Min<U1ajust>, L"Минимальное прямое напряжение досчёта")
X(Max<U1ajust>, L"Максимальное прямое напряжение досчёта")
X(Min<U2ajust>, L"Минимальное обратное напряжение досчёта")
X(Max<U2ajust>, L"Максимальное обратное напряжение досчёта")
X(Min<Ut>, L"Минимальное температурное напряжение")
X(Max<Ut>, L"Максимальное температурное напряжение")

typedef TL::MkTlst<
TresholdDlgItem<Min<Ucal> >
, TresholdDlgItem<Max<Ucal> >
, TresholdDlgItem<Min<T1g> >
, TresholdDlgItem<Max<T1g> >
, TresholdDlgItem<Min<T2g> >
, TresholdDlgItem<Max<T2g> >
, TresholdDlgItem<Min<U1ajust> >
, TresholdDlgItem<Max<U1ajust> >
, TresholdDlgItem<Min<U2ajust> >
, TresholdDlgItem<Max<U2ajust> >
, TresholdDlgItem<Min<Ut> >
, TresholdDlgItem<Max<Ut> >
>::Result TresholdParametersList;

#undef X

