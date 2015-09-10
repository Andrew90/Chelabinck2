#include "stdafx.h"
#include "ParamDlg.h"
#include "ParamDlg.hpp"
#include "ParametersCycle.h"
#include "ThicknessViewer.h"
#include "Emptywindow.h"
#include "Pass.h"
#include "TypeSizeBase.h"
#include "EditOperatorsWindow.h"
#include "EditTypeSizeWindow.h"
#include "BaseGridAdditional.hpp"
#include "MenuApi.h"
#include "CycleData.hpp"
#include "DialogBeforeCycle.h"
#include "EditTypeSizeViewer.hpp"
//-----------------------------------------------------------------------------------------------------------
template<class T>struct select_wapper
{
	typedef NullType Result;
};
template<template<class>class W, class T>struct select_wapper<W<T> >
{
	typedef T Result;
};

template<class List, class tmp = NullType>struct __get_no_wapper_list__;
template<class Head, class Tail, class tmp>struct __get_no_wapper_list__<Tlst<Head, Tail>, tmp>
{
	typedef typename __get_no_wapper_list__<Tail, typename TL::Append<tmp, typename select_wapper<Head>::Result>::Result>::Result Result;
};
template<class tmp>struct __get_no_wapper_list__<NullType, tmp>
{
	typedef tmp Result;
};

PARAM_TITLE(_sycle_<NormativeTechnicalDocument>, L"Нормативно-технический документ")
DO_NOT_CHECK(_sycle_<NormativeTechnicalDocument>)
template<>struct DlgSubItems<_sycle_<NormativeTechnicalDocument>, Holder<128> >: EditReadOnlyItems<_sycle_<NormativeTechnicalDocument> >{};

PARAM_TITLE(_sycle_<SteelGrade>, L"Марка стали")
DO_NOT_CHECK(_sycle_<SteelGrade>)
template<>struct DlgSubItems<_sycle_<SteelGrade>, Holder<128> >: EditReadOnlyItems<_sycle_<SteelGrade> >{};

PARAM_TITLE(_sycle_<NominalThickness>, L"Толщина стенки трубы")
DO_NOT_CHECK(_sycle_<NominalThickness>)
template<>struct DlgSubItems<_sycle_<NominalThickness>, _sycle_<NominalThickness>::type_value >: EditReadOnlyItems<_sycle_<NominalThickness> >{};

PARAM_TITLE(_sycle_<TubeDiameter>, L"Диаметр трубы")
DO_NOT_CHECK(_sycle_<TubeDiameter>)
template<>struct DlgSubItems<_sycle_<TubeDiameter>, _sycle_<TubeDiameter>::type_value >: EditReadOnlyItems<_sycle_<TubeDiameter> >{};

PARAM_TITLE(_sycle_<NominalPercentMin>, L"Допуск минус (%)")
DO_NOT_CHECK(_sycle_<NominalPercentMin>)
template<>struct DlgSubItems<_sycle_<NominalPercentMin>, _sycle_<NominalPercentMin>::type_value >: EditReadOnlyItems<_sycle_<NominalPercentMin> >{};

PARAM_TITLE(_sycle_<ZZ_NominalPercentMin>, L"Допуск минус (мм)")
DO_NOT_CHECK(_sycle_<ZZ_NominalPercentMin>)
template<>struct DlgSubItems<_sycle_<ZZ_NominalPercentMin>, _sycle_<ZZ_NominalPercentMin>::type_value >: EditReadOnlyItems<_sycle_<ZZ_NominalPercentMin> >{};

PARAM_TITLE(_sycle_<NominalPercentMax>, L"Допуск плюс (%)")
DO_NOT_CHECK(_sycle_<NominalPercentMax>)
template<>struct DlgSubItems<_sycle_<NominalPercentMax>, _sycle_<NominalPercentMax>::type_value >: EditReadOnlyItems<_sycle_<NominalPercentMax> >{};

PARAM_TITLE(_sycle_<ZZ_NominalPercentMax>, L"Допуск плюс (мм)")
DO_NOT_CHECK(_sycle_<ZZ_NominalPercentMax>)
template<>struct DlgSubItems<_sycle_<ZZ_NominalPercentMax>, _sycle_<ZZ_NominalPercentMax>::type_value >: EditReadOnlyItems<_sycle_<ZZ_NominalPercentMax> >{};

PARAM_TITLE(_sycle_<DefectDifferentWallPercent>, L"Разностенность (%)")
DO_NOT_CHECK(_sycle_<DefectDifferentWallPercent>)
template<>struct DlgSubItems<_sycle_<DefectDifferentWallPercent>, _sycle_<DefectDifferentWallPercent>::type_value >: EditReadOnlyItems<_sycle_<DefectDifferentWallPercent> >{};

PARAM_TITLE(_sycle_<ZZ_DefectDifferentWallPercent>, L"Разностенность (мм)")
DO_NOT_CHECK(_sycle_<ZZ_DefectDifferentWallPercent>)
template<>struct DlgSubItems<_sycle_<ZZ_DefectDifferentWallPercent>, _sycle_<ZZ_DefectDifferentWallPercent>::type_value >: EditReadOnlyItems<_sycle_<ZZ_DefectDifferentWallPercent> >{};

template<class T, int edit_width = 240>struct ButtonItems:T
{
	HWND Init(HWND h, int &width, int &dy, T &t)
	{		
		HWND hWnd = CreateWindow(L"button", ParamTitle<T>()()
				, WS_VISIBLE | WS_CHILD | WS_TABSTOP
				, 40, dy + 3, edit_width, 25, h, (HMENU)T::ID, hInstance, NULL
				);
		
		dy += 35;
		return hWnd;
	}
};

template<class List>struct NoWapper;
template<class Head, class Tail>struct NoWapper<Tlst<Head, Tail> >
{
	typedef Tlst<typename TL::NoWapperType<Head>::Result, typename NoWapper<Tail>::Result> Result;
};
template<>struct NoWapper<NullType>
{
	typedef NullType Result;
};

template<> struct _sycle_<DocumentBtn>:DocumentBtn{};
PARAM_TITLE(DocumentBtn, L"Выбор типоразмера")
DO_NOT_CHECK(DocumentBtn)
template<>struct DlgSubItems<DocumentBtn, int >: ButtonItems<DocumentBtn>{};

PARAM_TITLE(_sycle_<HeatNumber>, L"Номер плавки")
DO_NOT_CHECK(_sycle_<HeatNumber>)
template<>struct DlgSubItems<_sycle_<HeatNumber>, _sycle_<HeatNumber>::type_value>: EditItems<_sycle_<HeatNumber> >{};

PARAM_TITLE(_sycle_<PacketNumber>, L"Номер пакета")
DO_NOT_CHECK(_sycle_<PacketNumber>)
template<>struct DlgSubItems<_sycle_<PacketNumber>, _sycle_<PacketNumber>::type_value>: EditItems<_sycle_<PacketNumber> >{};

PARAM_TITLE(_sycle_<CountTubes>, L"Количество труб в пакете")
DO_NOT_CHECK(_sycle_<CountTubes>)
template<>struct DlgSubItems<_sycle_<CountTubes>, _sycle_<CountTubes>::type_value>: EditItems<_sycle_<CountTubes> >{};

PARAM_TITLE(_sycle_<NameOperator>, L"Имя оператора")
DO_NOT_CHECK(_sycle_<NameOperator>)
template<>struct DlgSubItems<_sycle_<NameOperator>, _sycle_<NameOperator>::type_value>: EditReadOnlyItems<_sycle_<NameOperator> >{};

PARAM_TITLE(_sycle_<PersonnelNumber>, L"Табельный номер")
DO_NOT_CHECK(_sycle_<PersonnelNumber>)
template<>struct DlgSubItems<_sycle_<PersonnelNumber>, _sycle_<PersonnelNumber>::type_value>: EditReadOnlyItems<_sycle_<PersonnelNumber> >{};


template<> struct _sycle_<NameOperatorBtn>:NameOperatorBtn{};
PARAM_TITLE(NameOperatorBtn, L"Выбор оператора")
DO_NOT_CHECK(NameOperatorBtn)
template<>struct DlgSubItems<NameOperatorBtn, int >: ButtonItems<NameOperatorBtn>{};

wchar_t *shift_data[] = {L"A", L"B", L"C", L"D"};

template<>struct CurrentValue<_sycle_<WorkShift> >
{
	void operator()(HWND h, WorkShift &t);
};
template<>struct FillComboboxList<_sycle_<WorkShift> >
{
	void operator()(HWND h, WorkShift &t)
	{
		for(int i = 0; i < dimention_of(shift_data); ++i)
		{
            ComboBox_AddString(h, shift_data[i]);
		}
	}
};
PARAM_TITLE(_sycle_<WorkShift>, L"Смена")
DO_NOT_CHECK(_sycle_<WorkShift>)
template<>struct DlgSubItems<_sycle_<WorkShift>, _sycle_<WorkShift>::type_value>: ComboBoxSubItem<_sycle_<WorkShift> >{};

struct CycleTest
{
	typedef TL::MkTlst<
		  _sycle_<NormativeTechnicalDocument   >
		, _sycle_<SteelGrade				   >
		, _sycle_<TubeDiameter				   >
		, _sycle_<NominalThickness			   >
		, _sycle_<NominalPercentMin			   >
		, _sycle_<ZZ_NominalPercentMin		   >
		, _sycle_<NominalPercentMax	    	   >
		, _sycle_<ZZ_NominalPercentMax		   >
		, _sycle_<DefectDifferentWallPercent   >
		, _sycle_<ZZ_DefectDifferentWallPercent>
		, DocumentBtn                  
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
};

void CurrentValue<_sycle_<WorkShift> >::operator()(HWND h, WorkShift &t)
{
	wchar_t *x = Singleton<Cycle>::Instance().items.get<_sycle_<WorkShift>>().value.buffer;
	for(int i = 0; i < dimention_of(shift_data); ++i)
	{
		if(0 == wcscmp(x, shift_data[i]))
		{
			ComboBox_SetCurSel(h, i);
			return;
		}
	}
	ComboBox_SetCurSel(h, 0);
}

template<class T>struct GreaterZero
{
	bool operator()(wchar_t *v)
	{
		return Wchar_to<T>()(v) > 0;
	}
};
template<int N>struct GreaterZero<Holder<N> >
{
	bool operator()(wchar_t *v)
	{
		return true;
	}
};

template<class O, class P>struct __test_sycle_data__
{
	bool operator()(O *o, P *p)
	{
		return true;
	}
};

template<template<class>class W, class X, class P>struct __test_sycle_data__<DlgItem<W<X> >, P>
{
	typedef DlgItem<W<X> >O;
	bool operator()(O *o, P *p)
	{
		wchar_t buffer[256];
		GetWindowText(o->hWnd, buffer, dimention_of(buffer));
		typedef TL::NoWapperType<O>::Result T;
		if(!(Test<typename T::type_value>()(buffer) && GreaterZero<typename T::type_value>()(buffer)))
		{
			wsprintf(buffer, L"Поле \"%s\" заполнено не корректно", ParamTitle<W<T> >()());
			MessageBox(*p, buffer, L"Ошибка !!!", MB_ICONERROR);
			return false;
		}
		return true;
	}
};

template<class P>struct __test_sycle_data__<DlgItem<_sycle_<WorkShift> >, P>
{
	typedef DlgItem<_sycle_<WorkShift> > O;
	bool operator()(O *o, P *p)
	{
		return true;
	}
};

template<class O, class P>struct __set_sycle_data__
{
	void operator()(O *, P *){}
};

template<template<class>class W, class X, class P>struct __set_sycle_data__<DlgItem<W<X> >, P>
{
	typedef DlgItem<W<X> >O;
	void operator()(O *o, P *)
	{
		wchar_t buffer[256];
		GetWindowText(o->hWnd, buffer, dimention_of(buffer));
		typedef TL::NoWapperType<O>::Result T;
		o->value.value = Wchar_to<typename T::type_value>()(buffer);
	}
};

template<class P>struct __set_sycle_data__<DlgItem<_sycle_<CountTubes> >, P>
{
	typedef DlgItem<_sycle_<CountTubes> > O;
	void operator()(O *o, P *)
	{
		wchar_t buffer[256];
		GetWindowText(o->hWnd, buffer, dimention_of(buffer));
		typedef TL::NoWapperType<O>::Result T;
		o->value.value = Wchar_to<typename T::type_value>()(buffer);
		DialogBeforeCycle::Init(o->value.value);
	}
};

struct CycleOkBtn
{
	static const int width = 120;
	static const int height = 30;
	static const int ID = IDOK;
	wchar_t *Title(){return L"Применить";}
	template<class Owner>void BtnHandler(Owner &owner, HWND h)
	{
		if(TL::find<typename Owner::list, __test_sycle_data__>()(&owner.items, &h))
		{
			TL::foreach<typename Owner::list, __set_sycle_data__>()(&owner.items, (int *)0);
			StatusCycle::Update();
			EndDialog(h, TRUE);
		}
	}
};

struct CycleCancelBtn
{
	static const int width = 120;
	static const int height = 30;
	static const int ID = IDCANCEL;
	wchar_t *Title(){return L"Отмена";}
	template<class Owner>void BtnHandler(Owner &o, HWND h)
	{
		EndDialog(h, FALSE);
	}
};

template<class X, class P>struct __make_btn__<_sycle_<X>, P>
{
	void operator()(_sycle_<X> *, P *)
	{
		dprint("%s", __FUNCTION__);
	}
};

void PersentToMM()
{
	Cycle &x = Singleton<Cycle>::Instance();
	double thickness = x.items.get<_sycle_<NominalThickness> >().value;
    x.items.get<_sycle_<ZZ_NominalPercentMin> >().value = thickness * x.items.get<_sycle_<NominalPercentMin> >().value / 100;
	x.items.get<_sycle_<ZZ_NominalPercentMax> >().value = thickness * x.items.get<_sycle_<NominalPercentMax> >().value / 100;
	x.items.get<_sycle_<ZZ_DefectDifferentWallPercent> >().value = thickness * x.items.get<_sycle_<DefectDifferentWallPercent> >().value / 100;
}

bool ParametesCycle::Do(HWND h)
{
	PersentToMM();
	return TemplDialog<
		Cycle
		, TL::MkTlst<
		    CycleOkBtn
		   , CycleCancelBtn
		   , _sycle_<DocumentBtn>
		   , _sycle_<NameOperatorBtn>
		>::Result>(Singleton<Cycle>::Instance()).Do(h, L"Настройки цикла");
}
template<class O, class P>struct ParametesCycleTest_to
{
	void operator()(O *o, P *p)
	{
		o->value = p->get<O>().value;
		dprint("ParametesCycleTest_to %s", typeid(O).name());
	}
};

template<class O, class P>struct ParametesCycleTest_from
{
	void operator()(O *o, P *p)
	{
		p->get<O>().value = o->value;
	}
};

template<class List>struct _no_w_;
template<class Head, class Tail>struct _no_w_<Tlst<Head, Tail> >
{
	typedef typename _no_w_<Tail>::Result Result;
};
template<class Head, class Tail, template<class >class W>struct _no_w_<Tlst<W<Head>, Tail> >
{
	typedef Tlst<W<Head>, typename _no_w_<Tail>::Result> Result;
};
template<>struct _no_w_<NullType>
{
	typedef NullType Result;
};

bool ParametesCycleTest::Do(HWND h)
{
	PersentToMM();
	CycleTest x;
	typedef _no_w_<CycleTest::items_list>::Result x_list;
	TL::foreach<x_list, ParametesCycleTest_to>()(&x.items, &Singleton<Cycle>::Instance().items);
	if(
		TemplDialog<
		CycleTest
		, TL::MkTlst<
		CycleOkBtn
		, CycleCancelBtn
		, _sycle_<DocumentBtn>
		>::Result>(x).Do(h, L"Настройки порогов")
		)
	{
		TL::foreach<x_list, ParametesCycleTest_from>()(&x.items, &Singleton<Cycle>::Instance().items);
		StatusCycle::Update();
		return true;
	}
	return false;
}

template<class O, class P>struct __set_0__
{
	void operator()(O *o, P *){}
};

template<class T>void set_null__(T &t){t = 0;}
template<int N>void set_null__(Holder<N> &t){t = L"";}

template<template<class>class W, class X, class P>struct __set_0__<W<X>, P>
{
	typedef W<X> O;
	void operator()(O *o, P *)
	{
		set_null__(o->value);
	}
};

void ParametesCycle::Init()
{
	TL::foreach<Cycle::items_list, __set_0__>()(&Singleton<Cycle>::Instance().items, (int *)0);
}

class StatusCycleWindow
{
public:
	template<class T>struct __handler_HWND__
	{
		HWND hWnd;
	};
	typedef __get_no_wapper_list__<Cycle::items_list>::Result list;
	typedef TL::TypeToTypeLst<list, __handler_HWND__>::Result items_list;
	int height, width, left, top;
	TL::Factory<items_list> items;
public:
	HWND hWnd, hCloseBtn;
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	unsigned operator()(TCreate &);
	void operator()(TDestroy &);
	unsigned operator()(TNotify &);
	static void Coordinates(HWND h);
	static void Do(HWND h);
	void Update();
};

namespace StatusCycleWindowMenu
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
		MenuItem<StatusCycleWindow>
	>::Result list;
};

typedef TL::MkTlst<
	TopMenu<MainFile>
	, TopMenu<MainOptionUnits>
>::Result MainMenu;

MENU_TEXT(L"Файл", TopMenu<MainFile>)
MENU_TEXT(L"Выход", MenuItem<MainExit>)

MENU_TEXT(L"Настройки", TopMenu<MainOptionUnits>)
MENU_TEXT(L"Сохранить координаты окна", MenuItem<StatusCycleWindow>)
	

template<>struct Event<MenuItem<MainExit> >{static void Do(HWND h){DestroyWindow(h);}};//выход из программы
template<>struct Event<MenuItem<StatusCycleWindow> >{static void Do(HWND h){StatusCycleWindow::Coordinates(h);}};
}
//------------------------------------------------------------------------
void StatusCycleWindow::operator()(TCommand &m)
{
	GetMenuToolBarEvent(m);
}
//------------------------------------------------------------------------
void StatusCycleWindow::operator()(TGetMinMaxInfo &m)
{
	if(NULL != m.pMinMaxInfo)
	{
		m.pMinMaxInfo->ptMinTrackSize.x = width;
		m.pMinMaxInfo->ptMinTrackSize.y = height;
		m.pMinMaxInfo->ptMaxTrackSize.x = width;
		m.pMinMaxInfo->ptMaxTrackSize.y = height;	
	}		
}
//------------------------------------------------------------------------
struct StatusCycleWindow_Create_Data
{
	HWND h;
	int width;
	int dy;
	StatusCycleWindow_Create_Data(HWND h) : h(h), width(200), dy(10) {}
};
template<class O, class P>struct StatusCycleWindow_Create
{
	void operator()(O *o, P *p)
	{
		typedef TL::NoWapperType<O>::Result T;
		o->hWnd = EditReadOnlyItems<_sycle_<T> >().Init(p->h, p->width, p->dy, Singleton<Cycle>::Instance().items.get<_sycle_<T>>());
	}
};
unsigned StatusCycleWindow::operator()(TCreate &m)
{
	PersentToMM();
	Menu<StatusCycleWindowMenu::MainMenu>().Init(m.hwnd);
	dprint("StatusCycleWindow %s", typeid(list).name());
	StatusCycleWindow_Create_Data data(m.hwnd);
	TL::foreach<items_list, StatusCycleWindow_Create>()(&items, &data);
	height = data.dy + 80;
	width = 500;
	MoveWindow(m.hwnd, left, top, width, height, TRUE);
	return 0;
}
//------------------------------------------------------------------------
template<class O, class P>struct StatusCycleWindow_Update
{
	void operator()(O *o, P *)
	{
		typedef TL::NoWapperType<O>::Result T;
		wchar_t *s = Wchar_from<typename T::type_value>(Singleton<Cycle>::Instance().items.get<_sycle_<T>>().value)();
        SetWindowText(o->hWnd, s);
	}
};
void StatusCycleWindow::Update()
{
	TL::foreach<items_list, StatusCycleWindow_Update>()(&items, (int *)0);
}
//------------------------------------------------------------------------
void StatusCycleWindow::operator()(TDestroy &l)
{
	SetWindowLong(l.hwnd, GWL_USERDATA, 0);
	delete this;
}
//------------------------------------------------------------------------
unsigned StatusCycleWindow::operator()(TNotify &l)
{
	GridNotify *x = (GridNotify *)GetWindowLongPtr(l.pnmh->hwndFrom, GWLP_USERDATA);
	if(0 == IsBadReadPtr(x, sizeof(x)))return x->Notify(l);
	return 0;
}
//-------------------------------------------------------------------------------
void StatusCycleWindow::Do(HWND h)
{
	wchar_t *typeWindow =  L"StatusCycleWindow";
	if(HWND h = FindWindow(typeWindow, 0))
	{
		SendMessage(h, WM_SYSCOMMAND, SC_RESTORE, 0);
		SetForegroundWindow(h);
	}
	else
	{
		int static const width = 500;
		int static const height = 500;
		RECT r;
		GetClientRect(GetDesktopWindow(), &r);
		StatusCycleWindow *o = new StatusCycleWindow;
		StatusWindowTable &x = Singleton<StatusWindowTable>::Instance();
		o->hWnd = WindowTemplate(o, typeWindow, L"Параметры цикла", IDI_settings
			, o->left = x.items.get<OffsetWindowX>().value
			, o->top = x.items.get<OffsetWindowY>().value
			, width
			, height
			, (HBRUSH)(COLOR_MENU+1)
			);

		ShowWindow(o->hWnd, SW_SHOW);
	}
}
//----------------------------------------------------------------------------------------
void StatusCycleWindow::Coordinates(HWND h)
{
	RECT r;
	GetWindowRect(h, &r);
	StatusWindowTable &x = Singleton<StatusWindowTable>::Instance();
	x.items.get<OffsetWindowX>().value = r.left;
	x.items.get<OffsetWindowY>().value = r.top;
	CBase base(ParametersBase().name());
	if(base.IsOpen())
	{
		UpdateWhere<StatusWindowTable>(x, base).ID(1).Execute();
	}
}
//--------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
template<class O, class P>struct StatusCycle_Update
{
	void operator()(O *o, P *p)
	{
		o->value = p->get<_sycle_<O> >().value;
	}
};
void StatusCycle::Update()
{
	HWND h = FindWindow(L"StatusCycleWindow", NULL);
	if(NULL != h)
	{
		StatusCycleWindow *o = (StatusCycleWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
		o->Update();
	}
	TL::foreach<ThresholdsTable::items_list, StatusCycle_Update>()(&Singleton<ThresholdsTable>::Instance().items, &Singleton<Cycle>::Instance().items);
}
void StatusCycle::Do(HWND h)
{
	StatusCycleWindow::Do(h);
}