#pragma once

namespace ManagementIOportsMenu
{
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
		MenuItem<ManagmentOptionsCoordinatesDlg>
	>::Result list;
};

typedef TL::MkTlst<
	TopMenu<MainFile>
	, TopMenu<MainOptionUnits>
>::Result MainMenu;

MENU_TEXT(L"Файл", TopMenu<MainFile>)
MENU_TEXT(L"Выход", MenuItem<MainExit>)

MENU_TEXT(L"Настройки", TopMenu<MainOptionUnits>)
MENU_TEXT(L"Сохранить координаты окна", MenuItem<ManagmentOptionsCoordinatesDlg>)
	

template<>struct Event<MenuItem<MainExit> >{static void Do(HWND h){DestroyWindow(h);}};//выход из программы
template<>struct Event<MenuItem<ManagmentOptionsCoordinatesDlg> >{static void Do(HWND h){ManagmentOptionsCoordinatesDlg::Do(h);}};//выход из программы
}

