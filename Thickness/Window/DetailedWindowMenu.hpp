namespace DetailedWindowMenu
{
struct MainFile{};
struct MainExit{};

struct MainOptionUnits{};
struct OptionsCoordinates
{
	static void Do(HWND h)
	{
		RECT r;
		GetWindowRect(h, &r);
		ViewerWindowTable &t = Singleton<ViewerWindowTable>::Instance();
		t.items.get<OffsetWindowX>().value = r.left;
		t.items.get<OffsetWindowY>().value = r.top;
		t.items.get<WindowWidth>().value = r.right - r.left;
		t.items.get<WindowHeight>().value = r.bottom - r.top;
		CBase base(ParametersBase().name());
		if(base.IsOpen())
		{
			UpdateWhere<ViewerWindowTable>(t, base).ID(1).Execute();
		}
	}
};

template<>struct TopMenu<MainFile>
{
	typedef TL::MkTlst<
		MenuItem<MainExit>
	>::Result list;
};

template<>struct TopMenu<MainOptionUnits>
{
	typedef TL::MkTlst<
		MenuItem<OptionsCoordinates>
	>::Result list;
};

typedef TL::MkTlst<
	TopMenu<MainFile>
	, TopMenu<MainOptionUnits>
>::Result MainMenu;

MENU_TEXT(L"Файл", TopMenu<MainFile>)
MENU_TEXT(L"Выход", MenuItem<MainExit>)

MENU_TEXT(L"Настройки", TopMenu<MainOptionUnits>)
MENU_TEXT(L"Сохранить координаты окна", MenuItem<OptionsCoordinates>)
	
template<>struct Event<MenuItem<MainExit> >{static void Do(HWND h){DestroyWindow(h);}};
template<>struct Event<MenuItem<OptionsCoordinates> >{static void Do(HWND h){OptionsCoordinates::Do(h);}};
}