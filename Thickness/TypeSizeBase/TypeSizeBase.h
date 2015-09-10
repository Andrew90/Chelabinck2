#pragma once
#include "AppBase.h"

struct NormativeTechnicalDocument
{
	typedef Holder<128> type_value;
	type_value value;
	type_value default_value;
	const wchar_t *name(){return L"NormativeTechnicalDocument";}
	NormativeTechnicalDocument()
	{
		default_value = L"etalon";
		value = default_value;
	}
};
struct SteelGrade
{
	typedef Holder<128> type_value;
	type_value value;
	type_value default_value;
	const wchar_t *name(){return L"SteelGrade";}
	SteelGrade()
	{
		default_value = L"steel";
		value = default_value;
	}
};
DEFINE_PARAM(TubeDiameter, double, 114)
//----------------------------------------------------------------------------------------
struct NormativeTechnicalDocumentTable
{
	typedef TL::MkTlst<
		NormativeTechnicalDocument
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"NormativeTechnicalDocumentTable";}
};
struct SteelGradeTable
{
	typedef TL::MkTlst<
		SteelGrade
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"SteelGradeTable";}
};

DEFINE_PARAM_ID(NormativeTechnicalDocument, int, 1)
DEFINE_PARAM_ID(SteelGrade, int, 1)

//DEFINE_PARAM(NormativeTechnicalDocumentID, int, 1)
//DEFINE_PARAM(SteelGradeID, int, 1)

struct TypeSizeTable
{
	typedef TL::MkTlst<
		  ID<NormativeTechnicalDocument>
		, ID<SteelGrade>				 
		, NominalThickness	   
		, TubeDiameter		   
		, NominalPercentMin
		, NominalPercentMax
		, DefectDifferentWallPercent
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"TypeSizeTable";}
};

struct NameOperator
{
	typedef Holder<128> type_value;
	type_value value;
	type_value default_value;
	const wchar_t *name(){return L"NameOperator";}
	NameOperator()
	{
		default_value = L"nobody";
		value = default_value;
	}
};
DEFINE_PARAM(PersonnelNumber, int, 0)

struct OperatorsTable
{
	typedef TL::MkTlst<
		NameOperator
		, PersonnelNumber
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"OperatorsTable";}
};

struct TypeSizeBase
{
   typedef TL::MkTlst<
		NormativeTechnicalDocumentTable
		, SteelGradeTable
		, TypeSizeTable
		, OperatorsTable
   >::Result type_list;
  typedef TL::Factory<type_list> TTables;
   TTables tables;
   wchar_t path[512];
   const wchar_t *name()
   {
	   GetModuleFileName(0, path, dimention_of(path));
	   PathRemoveFileSpec(path);
	   wcscat(path, L"\\TypeSizeBase");
	   return path;
   }
};
