#pragma once
//#include "MenuApi.h"
struct MainFile{};
struct MainExit{};

struct MainOptionUnits{};

template<>struct TopMenu<MainFile>
{
	typedef TL::MkTlst<
		MenuItem<MainExit>
	>::Result list;
};

template<>struct TopMenu<MainOptionUnits>
{
	typedef TL::MkTlst<
		MenuItem<CoordinatesDlg>
	>::Result list;
};

typedef TL::MkTlst<
	TopMenu<MainFile>
	, TopMenu<MainOptionUnits>
>::Result MainMenu;

MENU_TEXT(L"����", TopMenu<MainFile>)
MENU_TEXT(L"�����", MenuItem<MainExit>)

MENU_TEXT(L"���������", TopMenu<MainOptionUnits>)
MENU_TEXT(L"��������� ���������� ����", MenuItem<CoordinatesDlg>)
	

template<>struct Event<MenuItem<MainExit> >{static void Do(HWND h){DestroyWindow(h);}};//����� �� ���������
template<>struct Event<MenuItem<CoordinatesDlg> >{static void Do(HWND h){CoordinatesDlg::Do(h);}};//����� �� ���������



