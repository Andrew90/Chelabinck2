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
		wsprintf(buf, L"%s ������ ���� ������ %d ���\n%s ������ ���� ������ %d"
			, z.get<TresholdDlgItem<Min<T> > >().title, max
			, z.get<TresholdDlgItem<Max<T> > >().title, min
			);
		if(min > max)
		{
			MessageBox(h, buf, L"������!!!", MB_ICONEXCLAMATION);
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

X(Min<Ucal>, L"����������� ������� ����������")
X(Max<Ucal>, L"������������ ������� ����������")
X(Min<T1g>, L"����������� ������ �������")
X(Max<T1g>, L"������������ ������ �������")
X(Min<T2g>, L"����������� �������� �������")
X(Max<T2g>, L"������������ �������� �������")
X(Min<U1ajust>, L"����������� ������ ���������� �������")
X(Max<U1ajust>, L"������������ ������ ���������� �������")
X(Min<U2ajust>, L"����������� �������� ���������� �������")
X(Max<U2ajust>, L"������������ �������� ���������� �������")
X(Min<Ut>, L"����������� ������������� ����������")
X(Max<Ut>, L"������������ ������������� ����������")

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

