#include "stdafx.h"
#include "TypeSizeBaseGrid.h"
#include "TypeSizeBase.h"
#include <vector>
#include "BaseGridAdditional.hpp"
#include "EditTypeSizeViewer.hpp"

#include "DebugMess.h"
#ifdef XDEBUG
#define dprint debug.print
#else	
#define  dprint
#endif
struct TypeSizeBaseGrid::Impl
{
	typedef TypeSizeTable __Table__;
	template<class T>struct __item_grid__
	{
		typedef T Type;
        typename TL::NoWapperType<T>::Result::type_value value;
	};
	//typedef TL::NoWapperList<ParameterNameList>::Result base_list;
	typedef TL::TypeToTypeLst<typename ParameterTypeSizeList, __item_grid__>::Result items_list; 
	typedef TL::Factory<items_list> Items;
	typedef TL::TypeToTypeLst<items_list, __set_data_to_grid__>::Result proc_list;	
	std::vector<Items> data;
	TL::Factory<proc_list> proc;
	TL::ArrayFunc<proc_list, Items, wchar_t *> __array__;
	Impl(): __array__(proc)
	{}
};

TypeSizeBaseGrid::Impl localImpl;

TypeSizeBaseGrid::TypeSizeBaseGrid() : impl(&localImpl){}

template<template<class>class Z, template<class>class MM, class X, class P>struct __set_to_item__<Z<MM<X> >, P>
{
	typedef Z<MM<X> > O;
	void operator()(O *o, P *){}
};
template<class O, class P>struct __set_mm_item__{void operator()(O *, P *){}};
template<template<class>class Z, template<class>class MM, class X, class P>struct __set_mm_item__<Z<MM<X> >, P>
{
	typedef Z<MM<X> > O;
	void operator()(O *o, P *p)
	{
		o->value = p->get< Z<NominalThickness> >().value * p->get<Z<X> >().value / 100;
	}
};

//--------------------------------------------------------------------------------------
void TypeSizeBaseGrid::Init()
{
	if(impl->data.empty())
	{
		TypeSizeBase parameters;
		CBase base(
			parameters.name()
			, CreateDataBase<TypeSizeBase::type_list, SetDefault<TypeSizeBase::type_list> >()
			, parameters.tables
			);
		if(base.IsOpen())
		{
			ADODB::_RecordsetPtr rec;
			ADODB::_CommandPtr cmd;
			cmd.CreateInstance(__uuidof(ADODB::Command));
			cmd->ActiveConnection = base.conn;
			cmd->CommandType = ADODB::adCmdText;
			cmd->CommandText = L"SELECT ntd.NormativeTechnicalDocument, sg.SteelGrade, ts.NominalThickness, ts.TubeDiameter, ts.NominalPercentMin, ts.NominalPercentMax, ts.DefectDifferentWallPercent "\
				L"FROM NormativeTechnicalDocumentTable AS ntd, SteelGradeTable AS sg, TypeSizeTable AS ts "\
				L"WHERE ntd.ID = ts.NormativeTechnicalDocumentID AND sg.ID = ts.SteelGradeID "\
				L"ORDER BY ntd.NormativeTechnicalDocument, sg.SteelGrade, ts.NominalThickness, ts.TubeDiameter, ts.NominalPercentMin, ts.NominalPercentMax, ts.DefectDifferentWallPercent"
				;

			_variant_t rowsAffected; 
			rec = cmd->Execute( &rowsAffected, 0, ADODB::adCmdText);
			if(rec)
			{
				Impl::Items items;
				while (!rec->EndOfFile) 
				{
					TL::foreach<Impl::items_list, __set_to_item__>()(&items, rec.GetInterfacePtr());
					TL::foreach<Impl::items_list, __set_mm_item__>()(&items, &items);
					impl->data.push_back(items);
					rec->MoveNext(); 
				}
			}
		}
	}
}
void TypeSizeBaseGrid::Update()
{
	impl->data.clear();
	Init();
}
//----------------------------------------------------------------
/*
template<>struct ___error_message_dlg__<NormativeTechnicalDocument>
{
	wchar_t *operator()(){return L"Поле \"Нормативно-технический документ\" должено быть заполнено";}
};
*/
#define E_MESS(type, txt)template<>struct ___error_message_dlg__<type>\
{\
	wchar_t *operator()(){return L"Поле \""##txt##L"\" должено быть заполнено";}\
};
/*
Z(NormativeTechnicalDocument, 300, Нормативно-технический документ)
Z(SteelGrade				, 200, Марка стали)
Z(TubeDiameter				, 75, Диаметр трубы)
Z(NominalThickness			, 75, Толщина стенки)
Z(NominalPercentMin			, 75, Допуск -(%))
Z(MM<NominalPercentMin>			    , 75, Допуск -(мм))
Z(NominalPercentMax	        , 75, Допуск +(%))
Z(MM<NominalPercentMax>			    , 75, Допуск +(мм))
Z(DefectDifferentWallPercent, 75, Разностенность(%))
Z(MM<DefectDifferentWallPercent>     , 75, Разностенность(мм))
*/
E_MESS(NormativeTechnicalDocument    , L"Нормативно-технический документ")
E_MESS(SteelGrade				      , L"Марка стали")
E_MESS(TubeDiameter			      , L"Диаметр трубы")
E_MESS(NominalThickness		      , L"Толщина стенки")
E_MESS(NominalPercentMin		      , L"Допуск -(%)")
E_MESS(MM<NominalPercentMin>	      , L"Допуск-(мм)")
E_MESS(NominalPercentMax	          , L"Допуск +(%)")
E_MESS(MM<NominalPercentMax>	      , L"Допуск +(мм)")
E_MESS(DefectDifferentWallPercent    , L"Разностенность(%)")
E_MESS(MM<DefectDifferentWallPercent>, L"Разностенность(мм)")

#undef E_MESS



template<>struct TypeToTable<NormativeTechnicalDocument>{typedef NormativeTechnicalDocumentTable Result;};
template<>struct TypeToTable<SteelGrade>{typedef SteelGradeTable Result;};


template<class O, class P>struct TypeSizeBaseGrid_test
{
	bool operator()(O *o, P *p)
	{
        return __testing_input_date__<O, P>()(o, p);
	};
};

template<class O, class P>struct TypeSizeBaseGrid_test__
{
	bool operator()(O *o, P *p)
	{
        static const int i = TL::IndexOf<typename P::Type::items_list, O>::value;
		wchar_t buffer[256];
		GetWindowText(p->hEdit[i], buffer, 256);
		typedef typename TL::GetInnerType<typename TL::GetInnerType<O>::Result>::Result T;

		if(Test<T::type_value>()(buffer))
		{
			o->value = Wchar_to<T::type_value>()(buffer);
			return true;
		}
		else 
		{
			typedef typename TypeSizeBaseGrid::Impl::__item_grid__<MM<T> > mm_item;
			static const int i = TL::IndexOf<typename P::Type::items_list, mm_item>::value;
			GetWindowText(p->hEdit[i], buffer, 256);
			if(Test<T::type_value>()(buffer))
			{
				double x = p->items.get<typename TypeSizeBaseGrid::Impl::__item_grid__<NominalThickness> >().value; 
				double z = Wchar_to<T::type_value>()(buffer);
				p->items.get<mm_item>().value = z;
				if(0 != x)
				{
					o->value = 100.0 * z / x;
					return true;
				}
			}
		}
		wchar_t buf[1024];
		wsprintf(buf, L"%s\nили\n%s"
		 , ___error_message_dlg__<T>()()
		 , ___error_message_dlg__<MM<T> >()()
		 );
		MessageBox(p->ownerHWND, buf, L"Ошибка", MB_ICONERROR);
		return false;
	};
};
#define TYPE_SIZE(type)template<class P>struct TypeSizeBaseGrid_test<TypeSizeBaseGrid::Impl::__item_grid__<type>, P> :TypeSizeBaseGrid_test__<TypeSizeBaseGrid::Impl::__item_grid__<type>, P>{};
TYPE_SIZE(NominalPercentMin			)
TYPE_SIZE(NominalPercentMax			)
TYPE_SIZE(DefectDifferentWallPercent)
#undef TYPE_SIZE

#define SKIP__(type)template<class P>struct TypeSizeBaseGrid_test<TypeSizeBaseGrid::Impl::__item_grid__<MM<type> >, P>{bool operator()(TypeSizeBaseGrid::Impl::__item_grid__<MM<type> >*, P *){return true;}};
SKIP__(NominalPercentMin			)
SKIP__(NominalPercentMax			)
SKIP__(DefectDifferentWallPercent)
#undef SKIP__

struct __TypeSizeBaseGrid_testing_input_date_data__
{
	typedef TypeSizeBaseGrid::Impl Type;
	HWND *hEdit;
	HWND ownerHWND;	
	TypeSizeBaseGrid::Impl::Items &items;
	__TypeSizeBaseGrid_testing_input_date_data__(HWND *h, HWND o, TypeSizeBaseGrid::Impl::Items &items) :hEdit(h), ownerHWND(o), items(items){}
};
template<class O, class P>struct __tst__
{
	void operator()(O *o, P *p)
	{
		dprint("_tst_ %s", typeid(O).name());
	}
};
template<class O, class P>struct __set_date_to__zz
{
	void operator()(O *o, P *p)
	{
		o->value = p->items.get<P::Type::__item_grid__<O> >().value;
	}
};
template<class X, class P>struct __set_date_to__zz<ID<X>, P>
{
	typedef ID<X> O;
	void operator()(O *o, P *p)
	{
		typedef TypeToTable<X>::Result Table;
		Table table;
		table.items.get<X>().value = p->items.get<typename P::Type::__item_grid__<X> >().value;
		int id = Select<Table>(p->base).eq<X>(table.items.get<X>().value).Execute();
		if(0 == id)
		{
			Insert_Into<Table>(table, p->base).Execute();
			id = Select<Table>(p->base).eq<X>(table.items.get<X>().value).Execute();
		}
		o->value = id;
	}
};
bool TypeSizeBaseGrid::Add(HWND ownerHWND, HWND *hEdit)
{	
	TypeSizeBaseGrid::Impl::Items items;
	if(TL::find<TypeSizeBaseGrid::Impl::items_list, TypeSizeBaseGrid_test>()(&items, &__TypeSizeBaseGrid_testing_input_date_data__(hEdit, ownerHWND, items)))
	{
		CBase base(TypeSizeBase().name());
		if(base.IsOpen())
		{
			Impl::__Table__ ts;
			TL::foreach<Impl::__Table__::items_list, __set_date_to__zz>()(&ts.items, &__base__table_data__<typename TypeSizeBaseGrid::Impl>(base, items));
			int id = Select<Impl::__Table__>(base).eq_all<Impl::__Table__::items_list>(&ts.items).Execute();
			if(0 == id)
			{
				Insert_Into<Impl::__Table__>(ts, base).Execute();
				return true;
			}
		}		
	}
	return false;
}
//-----------------------------------------------------------------------
template<class O, class P>struct __insert_from_other_tables__zz
{
	void operator()(O *o, P *p)
	{
		o->value = p->items.get<P::Type::__item_grid__<O> >().value;
	}
};
template<class X, class P>struct __insert_from_other_tables__zz<ID<X>, P>
{
	typedef ID<X> O;
	void operator()(O *o, P *p)
	{
		X x;
		x.value = p->items.get<P::Type::__item_grid__<X> >().value;
		//o->value = Select<typename TypeToTable<X>::Result>(p->base).eq<X>(p->items.get<P::Type::__item_grid__<O> >().value).Execute();
		o->value = Select<typename TypeToTable<X>::Result>(p->base).eq<X>(x.value).Execute();
	}
};
bool TypeSizeBaseGrid::Delete(HWND ownerHWND, HWND *hEdit)
{
	TypeSizeBaseGrid::Impl::Items items;
	if(TL::find<TypeSizeBaseGrid::Impl::items_list, __testing_input_date__>()(&items, &__testing_input_date_data__<TypeSizeBaseGrid::Impl>(hEdit, ownerHWND)))
	{
		CBase base(TypeSizeBase().name());
		if(base.IsOpen())
		{
			Impl::__Table__ ts;	
			TL::foreach<Impl::__Table__::items_list, __insert_from_other_tables__zz>()(&ts.items, &__insert_from_other_tables_data__<TypeSizeBaseGrid::Impl>(items, base));
			if(Select<Impl::__Table__>(base).eq_all<Impl::__Table__::items_list>(&ts.items).Execute())
			{
				if(::Delete<Impl::__Table__>(base).eq_all<Impl::__Table__::items_list>(&ts.items).Execute())
				{
					if(0 == Select<Impl::__Table__>(base).eq_all<Impl::__Table__::items_list>(&ts.items).Execute())
					{
						TL::foreach<Impl::__Table__::items_list, __delete_from_tables__>()(&ts.items, &__base__table_data__<TypeSizeBaseGrid::Impl>(base, items));
						return true;
					}
				}
			}
		}
	}
	return false;
}
//---------------------------------------------------------------------
wchar_t *TypeSizeBaseGrid::Cell(unsigned row, unsigned col)
{
	return (impl->__array__[col])(impl->data[row]);
}
 //------------------------------------------------------------------------
 int TypeSizeBaseGrid::Count()
 {
	 return impl->data.size();
 }
 //-------------------------------------------------------------------------