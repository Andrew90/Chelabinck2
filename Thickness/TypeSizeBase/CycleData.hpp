#pragma once
#include "ParamDlg.hpp"
#include "EditTypeSizeViewer.hpp"

template<class T>struct _sycle_:T{};

struct ZZ_NominalPercentMin:NominalPercentMin{};
struct ZZ_NominalPercentMax:NominalPercentMax{};
struct ZZ_DefectDifferentWallPercent:DefectDifferentWallPercent{};
struct DocumentBtn
{
	typedef TL::TypeToTypeLst<
		TL::MkTlst<
		NormativeTechnicalDocument
		, SteelGrade				
		, TubeDiameter			
		, NominalThickness			
		, NominalPercentMin			
		, ZZ_NominalPercentMin
		, NominalPercentMax	    
		, ZZ_NominalPercentMax		  
		, DefectDifferentWallPercent
		, ZZ_DefectDifferentWallPercent
		>::Result
		, _sycle_>::Result items_list;	
	typedef  TL::TypeToTypeLst<
	  items_list
		, DlgItem
	>::Result x_list;
	typedef int type_value;

	static const int width = 0;
	static const int ID = 100;
	template<class O, class P>struct __set_HWND__
	{
		void operator()(O *o, P *p)
		{
			p[TL::IndexOf<x_list, O>::value] = o->hWnd;
		}
	};
	template<class Owner>void BtnHandler(Owner &o, HWND h)
	{
		HWND hh[TL::Length<x_list>::value];
		TL::foreach<x_list, __set_HWND__>()(&o.items, hh);
		SelectTypeSizeWindow::Do(h, hh);
	}
};
struct NameOperatorBtn
{
	typedef int type_value;

	static const int width = 0;
	static const int ID = 110;
	template<class Owner>void BtnHandler(Owner &o, HWND h)
	{
		HWND hh[2] = {
			o.items.get<DlgItem<_sycle_<NameOperator> > >().hWnd
			, o.items.get<DlgItem<_sycle_<PersonnelNumber> > >().hWnd
		};
		SelectOperatorsWindow::Do(h, hh);
	}
};

//DEFINE_PARAM(HeatNumber, int, 0)
DEFINE_PARAM(PacketNumber, int, 0)
DEFINE_PARAM(CountTubes, int, 0)

struct WorkShift
{
	typedef Holder<2> type_value;
	type_value value;
	type_value default_value;
	const wchar_t *name(){return L"WorkShift";}
	WorkShift()
	{
		default_value = L"A";
		value = default_value;
	}
};
struct HeatNumber
{
	typedef Holder<16> type_value;
	type_value value;
	type_value default_value;
	const wchar_t *name(){return L"HeatNumber";}
	HeatNumber()
	{
		default_value = L"";
		value = default_value;
	}
};

struct Cycle
{	
	typedef TL::AppendList<
		DocumentBtn::items_list
		, TL::MkTlst<
		DocumentBtn
		, _sycle_<HeatNumber>
		, _sycle_<PacketNumber>
		, _sycle_<CountTubes>
		, _sycle_<NameOperator>
		, _sycle_<PersonnelNumber>
		, NameOperatorBtn
		, _sycle_<WorkShift>
		>::Result
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
};