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

MENU_TEXT(L"Файл", TopMenu<MainFile>)
MENU_TEXT(L"Выход", MenuItem<MainExit>)

MENU_TEXT(L"Настройки", TopMenu<MainOptionUnits>)
MENU_TEXT(L"Сохранить координаты окна", MenuItem<CoordinatesDlg>)
	

template<>struct Event<MenuItem<MainExit> >{static void Do(HWND h){DestroyWindow(h);}};//выход из программы
template<>struct Event<MenuItem<CoordinatesDlg> >{static void Do(HWND h){CoordinatesDlg::Do(h);}};//выход из программы



