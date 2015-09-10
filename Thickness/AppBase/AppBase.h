#pragma once
#include "tables.hpp"
#include "LanParameters.h"
#include "InOutBits.h"
//-------------------------------------------------------------------------------------------
static const int count_sensors = 6;
static const int zone_length = 50;
static const int count_zones = 360;//240;
extern const double &result_undefined;
//-------------------------------------------------------------------------------------------
#define DEFINE_PARAM(z, type, def_val) struct z\
{\
	typedef type type_value;\
	type_value value;\
	const type_value default_value;\
	const wchar_t *name(){return L#z;}\
	z() : value(def_val), default_value(def_val) {}\
};

DEFINE_PARAM(NominalThickness, double, 10.0)
DEFINE_PARAM(NominalPercentMin, double, 12.5)
DEFINE_PARAM(NominalPercentMax, double, 10.5)
DEFINE_PARAM(DefectDifferentWallPercent, double, 8)

DEFINE_PARAM(MinimumThicknessPipeWall, double, 3.0)
DEFINE_PARAM(MaximumThicknessPipeWall, double, 20.0)

struct ThresholdsTable
{
	typedef TL::MkTlst<
		NominalThickness
		, NominalPercentMin
		, NominalPercentMax
		, DefectDifferentWallPercent
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
};

struct BorderCredibilityTable
{
	typedef TL::MkTlst<
		 MinimumThicknessPipeWall
		, MaximumThicknessPipeWall
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"BorderCredibilityTable";}
};

DEFINE_PARAM(MedianFilter, int, 5)

struct MedianFilterTable
{
	typedef TL::MkTlst<
		MedianFilter
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"MedianFilterTable";}
};

DEFINE_PARAM(ACFBorderLeft, int, 80)
DEFINE_PARAM(ACFBorderRight, int, 160)

struct ACFBorderTable
{
	typedef TL::MkTlst<
		ACFBorderLeft
		, ACFBorderRight
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"ACFBorderTable";}
};

struct NameParam
{
	typedef Holder<128> type_value;
	type_value value;
	type_value default_value;
	const wchar_t *name(){return L"NameParam";}
	NameParam()
	{
		value = L"NONAME";
		default_value = L"NONAME";
	}
};

template<class T>struct ID
{
	typedef typename T::__template_must_be_overridded__ noused;
};

#define DEFINE_PARAM_ID(z, type, def_val) template<> struct ID<z>\
{\
	typedef type type_value;\
	type_value value;\
	const type_value default_value;\
	const wchar_t *name(){return L#z##L"ID";}\
	ID() : value(def_val), default_value(def_val) {}\
};



DEFINE_PARAM(DeadAreaMM0, int, 50)
DEFINE_PARAM(DeadAreaMM1, int, 50)

struct DeadAreaTable
{
	typedef TL::MkTlst<
		DeadAreaMM0
		, DeadAreaMM1
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"DeadAreaTable";}
};

DEFINE_PARAM(ReferenceOffset1, unsigned, 1750)
DEFINE_PARAM(ReferenceOffset2, unsigned, 4600)

DEFINE_PARAM(ReferenceOffset3, unsigned, 1260)
DEFINE_PARAM(ReferenceOffset4, unsigned, 3030)

struct NamePlate1730
{
	typedef Holder<128> type_value;
	type_value value;
	type_value default_value;
	const wchar_t *name(){return L"NamePlate1730";}
	NamePlate1730()
	{
		default_value = L"PCIE-1730,BID#1";
		value = default_value;		
	}
};

struct NamePlate1730ParametersTable
{
	typedef TL::MkTlst<
		NamePlate1730
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"NamePlate1730ParametersTable";}
};

template<int NUM>struct CoefficientA;
template<int NUM>struct CoefficientB;

#define DEFINE_PARAM_NUM(z, n, type, def_val) template<>struct z<n>\
{\
	typedef type type_value;\
	type_value value;\
	const type_value default_value;\
	const wchar_t *name(){return L###z##L###n;}\
	z() : value(def_val), default_value(def_val) {}\
};

DEFINE_PARAM_NUM(CoefficientA, 1, double, 6.12e-002)
DEFINE_PARAM_NUM(CoefficientB, 1, double, 0.3398)
DEFINE_PARAM_NUM(CoefficientA, 2, double, 6.12e-002)
DEFINE_PARAM_NUM(CoefficientB, 2, double, 0.3398)
DEFINE_PARAM_NUM(CoefficientA, 3, double, 6.12e-002)
DEFINE_PARAM_NUM(CoefficientB, 3, double, 0.3398)
DEFINE_PARAM_NUM(CoefficientA, 4, double, 6.12e-002)
DEFINE_PARAM_NUM(CoefficientB, 4, double, 0.3398)
DEFINE_PARAM_NUM(CoefficientA, 5, double, 6.12e-002)
DEFINE_PARAM_NUM(CoefficientB, 5, double, 0.3398)
DEFINE_PARAM_NUM(CoefficientA, 6, double, 6.12e-002)
DEFINE_PARAM_NUM(CoefficientB, 6, double, 0.3398)

struct CoefficientParametersTable
{
	typedef TL::MkTlst<		
		CoefficientA<1>
		, CoefficientB<1>
		, CoefficientA<2>
		, CoefficientB<2>
		, CoefficientA<3>
		, CoefficientB<3>
		, CoefficientA<4>
		, CoefficientB<4>
		, CoefficientA<5>
		, CoefficientB<5>
		, CoefficientA<6>
		, CoefficientB<6>
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"CoefficientParametersTable";}
};

template<int>struct MinEnergy;
template<int>struct MaxEnergy;
template<int>struct Peak;

#define PEAK(n)\
DEFINE_PARAM_NUM(MinEnergy, n, double, 0.1)\
DEFINE_PARAM_NUM(MaxEnergy, n, double, 1000)\
DEFINE_PARAM_NUM(Peak, n, double, 0.2)

PEAK(0)
PEAK(1)
PEAK(2)
PEAK(3)
PEAK(4)
PEAK(5)

#undef PEAK

#define PEAK(n) MinEnergy<n>, MaxEnergy<n>, Peak<n>
struct SignalParametersTable
{
	typedef TL::MkTlst<		
		PEAK(0), PEAK(1), PEAK(2), PEAK(3), PEAK(4), PEAK(5)
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"SignalParametersTable";}
};
#undef PEAK

DEFINE_PARAM(SprayTime, int, 200)
DEFINE_PARAM(CorrectionOffsetTick, int, 0)
struct PaintMakerTable
{
	typedef TL::MkTlst<		
		SprayTime//импульс распыления краски дефектоотметчика
		, CorrectionOffsetTick
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"PaintMakerTable";}
};
//-------------------------------------------------------------------------------------------
//DEFINE_PARAM(UsefulPiecePipe, int, 130)
DEFINE_PARAM(SupplySensorDelay, int, 150)
DEFINE_PARAM(RemoveSensorDelay, int,  150)
DEFINE_PARAM(FrequencyFrames, int,  100)
struct AdditionalSettingsTable
{
	typedef TL::MkTlst<	
	//	UsefulPiecePipe
		 ReferenceOffset1
		, ReferenceOffset2
		, ReferenceOffset3
		, ReferenceOffset4
		, SupplySensorDelay
		, RemoveSensorDelay
		, FrequencyFrames
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"AdditionalSettingsTable";}
};
//--------------------------------------------------------------------------------
DEFINE_PARAM(LessMinimumEnergy, int, 0xff555555)
DEFINE_PARAM(ValueLessThreshold, int, 0xff555555)
DEFINE_PARAM(ThicknessLessThreshold, int, 0xff555555)
//DEFINE_PARAM(ThicknessGreaterThreshold, int, 0xff555555)
DEFINE_PARAM(Undefined, int, 0xff555555)

DEFINE_PARAM(Nominal, int, 0xff00ff00)
DEFINE_PARAM(DefectMax, int, 0xff0000ff)
DEFINE_PARAM(DefectMin, int, 0xffff0000)
DEFINE_PARAM(DefectMinMax, int, 0xffffff00)
DEFINE_PARAM(DefectDifferentWall, int, 0xff00ffff)

struct ColorTable
{
	typedef TL::MkTlst<	
		LessMinimumEnergy
		, ValueLessThreshold
		, ThicknessLessThreshold
		//, ThicknessGreaterThreshold
		, Nominal
		, DefectMin
		, DefectMax
		, DefectMinMax
		, DefectDifferentWall
		, Undefined
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"ColorTable";}
};
//-----------------------------------------------------------------------------------
struct InputsTable
{
	typedef input_bits_list items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"InputsTable";}
};
struct OutputsTable
{
	typedef output_bits_list items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"OutputsTable";}
};
//-----------------------------------------------------------------------------------
DEFINE_PARAM(OffsetWindowX, int, 0)
DEFINE_PARAM(OffsetWindowY, int, 0)

DEFINE_PARAM(WindowWidth, int, 750)
DEFINE_PARAM(WindowHeight, int, 450)

struct InputOutputWindowTable
{
	typedef TL::MkTlst<	
		OffsetWindowX
		, OffsetWindowY
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"InputOutputWindowTable";}
};

struct ArchiveWindowTable
{
	typedef TL::MkTlst<	
		OffsetWindowX
		, OffsetWindowY
		, WindowWidth
		, WindowHeight
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"ArchiveWindowTable";}
};
struct ThicknessWindowTable
{
	typedef TL::MkTlst<	
		OffsetWindowX
		, OffsetWindowY
		, WindowWidth
		, WindowHeight
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"ThicknessWindowTable";}
};
struct ViewerWindowTable
{
	typedef TL::MkTlst<	
		OffsetWindowX
		, OffsetWindowY
		, WindowWidth
		, WindowHeight
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"ViewerWindowTable";}
};
struct ManagementIOportsTable
{
	typedef TL::MkTlst<	
		OffsetWindowX
		, OffsetWindowY
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"ManagementIOportsTable";}
};
struct StatusWindowTable
{
	typedef TL::MkTlst<	
		OffsetWindowX
		, OffsetWindowY
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"StatusWindowTable";}
};
//-----------------------------------------------------------------------------------
struct ParametersBase
{
   typedef TL::MkTlst<
		 DeadAreaTable
		, MedianFilterTable
		, ACFBorderTable
		, LanParametersTable
		, CoefficientParametersTable
		, SignalParametersTable
		, PaintMakerTable
		, AdditionalSettingsTable
		, ColorTable
		, InputsTable
		, OutputsTable	
		, NamePlate1730ParametersTable
		, ViewerWindowTable
		, ManagementIOportsTable	
		, StatusWindowTable
		, BorderCredibilityTable
	>::Result one_row_table_list;
   typedef TL::MkTlst<
	   InputOutputWindowTable
	   , ArchiveWindowTable
	   , ThicknessWindowTable  	   
   >::Result no_init_table_list;
	typedef TL::MkTlst<
		 one_row_table_list
		, no_init_table_list
	>::Result multy_type_list; 
	typedef TL::MultyListToList<multy_type_list>::Result type_list;
	typedef TL::Factory<type_list> TTables;
   TTables tables;
   wchar_t path[256];
   const wchar_t *name();
};

struct AppBase
{
	void Init();
};

template<class T>int CurrentId(CBase &base)
{
	  CurrentParametersTable current;
	  Select<CurrentParametersTable>(base).ID(1).Execute(current);
	  ParametersTable param;
	  Select<ParametersTable>(base).ID(current.items.get<CurrentID>().value).Execute(param);
	  return param.items.get<T>().value;
}

template<class T>int CountId(CBase &base, int num)
{
	ADODB::_RecordsetPtr rec;
	Select<ParametersTable>(base).eq<T>(num).Execute(rec);
	int i = 0;
	while (!rec->EndOfFile) 
	{			
		++i;
		rec->MoveNext(); 
	}
	return i;
}

template<class T>void UpdateId(CBase &base, int num)
{
   CurrentParametersTable current;
   Select<CurrentParametersTable>(base).ID(1).Execute(current);
   Update<ParametersTable>(base).set<T>(num).Where().ID(current.items.get<CurrentID>().value).Execute();
}

template<class T>struct Singleton
{
	static T& Instance(){static T x; return x;}
};





