#include "stdafx.h"
#include "ParamDlg.h"
#include "ParamDlg.hpp"
#include "Dialogs.h"
#include "Pass.h"

//MIN_VALUE(MedianFilter, -1)
//MAX_VALUE(MedianFilter, 31)
/*
#define MIN_VALUE(sub_type, value)template<>struct MinT<sub_type>\
{sub_type::type_value operator()(){return value;}};

#define MAX_VALUE(sub_type, value)template<>struct MaxT<sub_type>\
{sub_type::type_value operator()(){return value;}};
template<class T>struct allowable_limit_on_the_parameter
{
	bool operator()(typename T::type_value &t, HWND h)
	{
		if(t > MinT<T>()() && t < MaxT<T>()()) return true;
		ErrMess<T>()(t, h);
		return false;
	}	
};
template<class T>struct ErrMess
{
	void operator()(typename T::type_value &t, HWND h)
	{
        wchar_t buf[256];
		wsprintf(buf, L"�������� \"%s\" ������ ���� ������ %s � ������ %s"
			, ParamTitle<T>()()
			, Wchar_from<T::type_value>(MinT<T>()())()
			, Wchar_from<T::type_value>(MaxT<T>()())()
			);
		MessageBox(h, buf, L"������!!!", MB_ICONEXCLAMATION);
	}
};
*/
#if 1
template<>struct allowable_limit_on_the_parameter<MedianFilter>
{
	typedef MedianFilter T;
	bool operator()(T::type_value &t, HWND h)
	{
		if(t < 2)
		{
			unsigned res = MessageBox(h, L"��������� ������ ����� ��������", L"���������", MB_ICONEXCLAMATION | MB_YESNO);
			if(res == IDYES)
			{
				t = 0;
				return true;
			}
			return false;
		}
		if(0 ==(t & 1))
		{
			unsigned res = MessageBox(h, L"������ ���������� ������� ������ ���� ��������", L"���������", MB_ICONEXCLAMATION | MB_YESNO);
			if(res == IDYES)
			{
				++t;
				return true;
			}
			return false;
		}
		if(t < 31) return true;
		MessageBox(h, L"�������� \"������ �������\" ������ ������ 30", L"������!!!", MB_ICONEXCLAMATION);
		return false;
	}	
};
#endif

PARAM_TITLE(MedianFilter, L"������ �������")

void MedianFiltreDlg::Do(HWND h)
{
	if(TemplDialog<MedianFilterTable>(Singleton<MedianFilterTable>::Instance()).Do(h, L"��������� ������"))
	{
	}
}

