#pragma once
struct MainFile{};
struct MainExit{};

struct MainOptionUnits{};

template<>struct TopMenu<MainFile>
{
	typedef TL::MkTlst<
		MenuItem<SaveDlg>
		, Separator<0>
		, MenuItem<MainExit>
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

#define MENU_HANDLER(title, type, proc)\
	MENU_TEXT(title, MenuItem<type>)\
	template<>struct Event<MenuItem<type> >{static void Do(HWND h){proc(h);}};

MENU_TEXT(L"Файл", TopMenu<MainFile>)
MENU_HANDLER(L"Выход", MainExit, DestroyWindow)

MENU_TEXT(L"Настройки", TopMenu<MainOptionUnits>)
MENU_HANDLER(L"Сохранить координаты окна", CoordinatesDlg, CoordinatesDlg::Do)
MENU_HANDLER(L"Сохранить сообщения", SaveDlg, SaveDlg::Do)
	
#undef MENU_HANDLER


