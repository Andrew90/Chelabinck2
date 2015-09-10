#pragma once

namespace MainWindowMenu
{
struct MainFile{};
struct MainExit{};

struct MainOptionTypeSize{};
struct MainThresholdOption{};
struct MainCreateTypesize{};
struct MainDeleteTypeSize{};

struct MainOptionUnits{};
struct OptionsLan{};
struct OptionsCoefficients{};
struct AdditionalParametersTypesize{};
//struct Gang{};
struct InputsOutputs{};
struct MainAbout{};

template<>struct TopMenu<MainFile>
{
	typedef TL::MkTlst<
		MenuItem<LoadData>
		, MenuItem<StoredData>
		, Separator<0>
		, MenuItem<Recalculation>
		, Separator<1>
		, MenuItem<MainExit>
	>::Result list;
};

template<>struct SubMenu<AdditionalParametersTypesize>
{
	typedef TL::TypeToTypeLst<
		typename TL::MkTlst<MedianFiltreDlg, DeadAreaDlg, ACFBorderDlg>::Result
		, MenuItem
	>::Result list;
};
/*
template<>struct TopMenu<MainOptionTypeSize>
{
	typedef TL::MkTlst<	
	    MenuItem<MainThresholdOption>
		, SubMenu<AdditionalParametersTypesize>
		, Separator<0>
		, MenuItem<MainCreateTypesize>
		, MenuItem<MainDeleteTypeSize>
	>::Result list;
};
*/
template<>struct SubMenu<InputsOutputs>
{
	typedef TL::TypeToTypeLst<
		typename TL::MkTlst<Descriptor1730Dlg, InputsDlg, OutputsDlg>::Result 
		, MenuItem
	>::Result list;
};

template<>struct TopMenu<MainOptionUnits>
{
	typedef TL::MkTlst<	
		MenuItem<OptionsLan>
		, MenuItem<OptionsCoefficients>
		, MenuItem<SignalDlg>
		, MenuItem<PaintMakerDlg>
		, MenuItem<ColorDlg>
		, SubMenu<InputsOutputs>
		, MenuItem<AdditionalSettingsDlg>		
		, MenuItem<MedianFiltreDlg>
		, MenuItem<DeadAreaDlg>
		, MenuItem<ACFBorderDlg>
		, MenuItem<MainThresholdOption>
		, MenuItem<CoordinatesDlg>
		, Separator<0>
		, MenuItem<ManagementIOportsDlg>
		, MenuItem<StatusCycle>
	>::Result list;
};

template<>struct TopMenu<EditOperatorsWindowDlg>
{
	typedef TL::MkTlst<	
		MenuItem<EditOperatorsWindowDlg>
		, MenuItem<EditTypeSizeDlg>
		, Separator<0>
		, MenuItem<ParametesCycleTest>
	>::Result list;
};

template<>struct TopMenu<MainAbout>
{
	typedef TL::MkTlst< 
	//	MenuItem<StatusCycle>
	//	, Separator<0>
		 MenuItem<MainAbout>
	>::Result list;
};
typedef TL::MkTlst<
	TopMenu<MainFile>
	//, TopMenu<MainOptionTypeSize>
	, TopMenu<MainOptionUnits>
	, TopMenu<EditOperatorsWindowDlg>
	, TopMenu<MainAbout>
>::Result MainMenu;

    MENU_TEXT(L"Файл", TopMenu<MainFile>)
	MENU_TEXT(L"Сохранить данные", MenuItem<StoredData>)
	MENU_TEXT(L"Загрузить данные", MenuItem<LoadData>)
	MENU_TEXT(L"Перерасчёт", MenuItem<Recalculation>)
	MENU_TEXT(L"Выход", MenuItem<MainExit>)

	MENU_TEXT(L"Типоразмер", TopMenu<MainOptionTypeSize>)
    MENU_TEXT(L"Дополнительные параметры" , SubMenu<AdditionalParametersTypesize>)
	MENU_TEXT(L"Медианный фильтр" , MenuItem<MedianFiltreDlg>)
	MENU_TEXT(L"Границы АЧХ" , MenuItem<ACFBorderDlg>)
	MENU_TEXT(L"Мёртвые зоны" , MenuItem<DeadAreaDlg>)
	MENU_TEXT(L"Границы достоверности" , MenuItem<MainThresholdOption>)
	MENU_TEXT(L"Создать типоразмер", MenuItem<MainCreateTypesize>)
	MENU_TEXT(L"Удалить типоразмер", MenuItem<MainDeleteTypeSize>)

	MENU_TEXT(L"Настройки", TopMenu<MainOptionUnits>)
	MENU_TEXT(L"Настройка аналоговой платы", MenuItem<OptionsLan>)	
	MENU_TEXT(L"Коэффициенты перерасчёта", MenuItem<OptionsCoefficients>)	
	MENU_TEXT(L"Настройки сигнала", MenuItem<SignalDlg>)	
	MENU_TEXT(L"Настройки дефектоотметчика", MenuItem<PaintMakerDlg>)
	MENU_TEXT(L"Дополнительные настройки", MenuItem<AdditionalSettingsDlg>)
	MENU_TEXT(L"Цветовые настройки", MenuItem<ColorDlg>)
	MENU_TEXT(L"Сохранить координаты окна", MenuItem<CoordinatesDlg>)
	MENU_TEXT(L"Управление дискретными выходами", MenuItem<ManagementIOportsDlg>)

	MENU_TEXT(L"База", TopMenu<EditOperatorsWindowDlg>)
	MENU_TEXT(L"Редактирование операторов", MenuItem<EditOperatorsWindowDlg>)	
	
	MENU_TEXT(L"Редактирование типоразмеров", MenuItem<EditTypeSizeDlg>)
	MENU_TEXT(L"Выбор порогов", MenuItem<ParametesCycleTest>)

	MENU_TEXT(L"О программе", TopMenu<MainAbout>)
	MENU_TEXT(L"Статус цикла", MenuItem<StatusCycle>)
	MENU_TEXT(L"О программе", MenuItem<MainAbout>)

    MENU_TEXT(L"Порты ввода-вывода", SubMenu<InputsOutputs>)
	MENU_TEXT(L"Входной порт", MenuItem<InputsDlg>)
	MENU_TEXT(L"Выходной порт", MenuItem<OutputsDlg>)
	MENU_TEXT(L"Дескриптор платы 1730", MenuItem<Descriptor1730Dlg>)

#define TYPE_SIZE_PASS(h, x) if(TypesizePasswordDlg().Do(h)) x(h);
#define OPTION_PASS(h, x) if(OptionPasswordDlg().Do(h)) x(h);
// #define NO_PASS(h, x) x(h);

template<>struct Event<MenuItem<MainExit> >{static void Do(HWND h){DestroyWindow(h);}};//выход из программы
template<>struct Event<MenuItem<Recalculation> >{static void Do(HWND h){Recalculation::Do(h);}};//пересчёт
template<>struct Event<MenuItem<MainAbout> >{static void Do(HWND h){AboutWindow::Do(h);}};
template<>struct Event<MenuItem<StatusCycle> >{static void Do(HWND h){StatusCycle::Do(h);}};
template<>struct Event<MenuItem<MainThresholdOption> >{static void Do(HWND h){ThresholdsParamDlg::Do(h);}};//настройка порогов
//template<>struct Event<MenuItem<MainCreateTypesize> >{static void Do(HWND h){TYPE_SIZE_PASS(h,SelectTypeSizeNew::Do);}};//добавить типоразмер
//template<>struct Event<MenuItem<MainDeleteTypeSize> >{static void Do(HWND h){TYPE_SIZE_PASS(h,SelectTypeSizeDelete::Do);}};//удалить типоразмер
template<>struct Event<MenuItem<OptionsLan> >{static void Do(HWND h){LanParamDlg::Do(h);}};//настройка Lan10m8
template<>struct Event<MenuItem<OptionsCoefficients> >{static void Do(HWND h){CoefficientDlg::Do(h);}};//
template<>struct Event<MenuItem<SignalDlg> >{static void Do(HWND h){SignalDlg::Do(h);}};//

template<>struct Event<MenuItem<PaintMakerDlg> >{static void Do(HWND h){PaintMakerDlg::Do(h);}};//
template<>struct Event<MenuItem<MedianFiltreDlg> >{static void Do(HWND h){MedianFiltreDlg::Do(h);}};//
template<>struct Event<MenuItem<ACFBorderDlg> >{static void Do(HWND h){ACFBorderDlg::Do(h);}};//
template<>struct Event<MenuItem<DeadAreaDlg> >{static void Do(HWND h){DeadAreaDlg::Do(h);}};//
template<>struct Event<MenuItem<AdditionalSettingsDlg> >{static void Do(HWND h){AdditionalSettingsDlg::Do(h);}};//
template<>struct Event<MenuItem<CoordinatesDlg> >{static void Do(HWND h){CoordinatesDlg::Do(h);}};//

template<>struct Event<MenuItem<EditOperatorsWindowDlg> >{static void Do(HWND h){EditOperatorsWindowDlg::Do(h);}};//

template<>struct Event<MenuItem<EditTypeSizeDlg> >{static void Do(HWND h){EditTypeSizeDlg::Do(h);}};//
template<>struct Event<MenuItem<ParametesCycleTest> >{static void Do(HWND h){ParametesCycleTest::Do(h);}};//
template<>struct Event<MenuItem<ColorDlg> >{static void Do(HWND h){ColorDlg::Do(h);}};//

template<>struct Event<MenuItem<InputsDlg> >{static void Do(HWND h){InputsDlg::Do(h);}};//
template<>struct Event<MenuItem<OutputsDlg> >{static void Do(HWND h){OutputsDlg::Do(h);}};//
template<>struct Event<MenuItem<Descriptor1730Dlg> >{static void Do(HWND h){Descriptor1730Dlg::Do(h);}};//

template<>struct Event<MenuItem<StoredData> >{static void Do(HWND h){StoredData::Do(h);}};//
template<>struct Event<MenuItem<LoadData> >{static void Do(HWND h){if(LoadData::Do(h))Recalculation::Do(h);}};//

template<>struct Event<MenuItem<ManagementIOportsDlg> >{static void Do(HWND h){ManagementIOportsDlg::Do(h);}};//

}
