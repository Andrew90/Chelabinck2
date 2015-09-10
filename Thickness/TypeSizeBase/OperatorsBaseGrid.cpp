#include "stdafx.h"
#include "OperatorsBaseGrid.h"
#include "TypeSizeBase.h"
#include <vector>
#include "BaseGridAdditional.hpp"

#include "DebugMess.h"
#ifdef XDEBUG
#define dprint debug.print
#else	
#define  dprint
#endif

struct OperatorsBaseGrid::Impl
{
	typedef OperatorsTable __Table__;
	template<class T>struct __item_grid__
	{
		typedef T Type;
        typename TL::GetInnerType<T>::Result::type_value value;
	};
	typedef TL::TypeToTypeLst<typename __Table__::items_list, __item_grid__>::Result items_list; 
	typedef TL::Factory<items_list> Items;
	typedef TL::TypeToTypeLst<items_list, __set_data_to_grid__>::Result proc_list;	
	std::vector<Items> data;
	TL::Factory<proc_list> proc;
	TL::ArrayFunc<proc_list, Items, wchar_t *> __array__;
	Impl(): __array__(proc)
	{}
};

OperatorsBaseGrid::Impl localImpl;

OperatorsBaseGrid::OperatorsBaseGrid() : impl(&localImpl){}
//--------------------------------------------------------------------------------------
void OperatorsBaseGrid::Init()
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
			cmd->CommandText = L"SELECT NameOperator, PersonnelNumber "\
				L"FROM OperatorsTable "\
				L"ORDER BY NameOperator, PersonnelNumber"
				;

			_variant_t rowsAffected; 
			rec = cmd->Execute( &rowsAffected, 0, ADODB::adCmdText);
			if(rec)
			{
				Impl::Items items;
				while (!rec->EndOfFile) 
				{
					TL::foreach<Impl::items_list, __set_to_item__>()(&items, rec.GetInterfacePtr());
					impl->data.push_back(items);
					rec->MoveNext(); 
				}
			}
		}
	}
}
void OperatorsBaseGrid::Update()
{
	impl->data.clear();
	Init();
}
//----------------------------------------------------------------
template<>struct ___error_message_dlg__<NameOperator>
{
	wchar_t *operator()(){return L"Поле \"Имя оператора\" должено быть заполнено";}
};
template<>struct ___error_message_dlg__<PersonnelNumber>
{
	wchar_t *operator()(){return L"Поле \"Табельный номер\" должено быть заполнено";}
};

template<>struct TypeToTable<NormativeTechnicalDocument>{typedef NormativeTechnicalDocumentTable Result;};
template<>struct TypeToTable<SteelGrade>{typedef SteelGradeTable Result;};


bool OperatorsBaseGrid::Add(HWND ownerHWND, HWND *hEdit)
{	
	Impl::Items items;
	if(TL::find<Impl::items_list, __testing_input_date__>()(&items, &__testing_input_date_data__<Impl>(hEdit, ownerHWND)))
	{
		CBase base(TypeSizeBase().name());
		if(base.IsOpen())
		{
			Impl::__Table__ ts;
			TL::foreach<Impl::__Table__::items_list, __set_date_to__>()(&ts.items, &__base__table_data__<Impl>(base, items));
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
bool OperatorsBaseGrid::Delete(HWND ownerHWND, HWND *hEdit)
{
	Impl::Items items;
	if(TL::find<Impl::items_list, __testing_input_date__>()(&items, &__testing_input_date_data__<Impl>(hEdit, ownerHWND)))
	{
		CBase base(TypeSizeBase().name());
		if(base.IsOpen())
		{
			Impl::__Table__ ts;	
			TL::foreach<Impl::__Table__::items_list, __insert_from_other_tables__>()(&ts.items, &__insert_from_other_tables_data__<Impl>(items, base));
			if(Select<Impl::__Table__>(base).eq_all<Impl::__Table__::items_list>(&ts.items).Execute())
			{
				::Delete<Impl::__Table__>(base).eq_all<Impl::__Table__::items_list>(&ts.items).Execute();
				if(0 == Select<Impl::__Table__>(base).eq_all<Impl::__Table__::items_list>(&ts.items).Execute())
				{
					TL::foreach<Impl::__Table__::items_list, __delete_from_tables__>()(&ts.items, &__base__table_data__<Impl>(base, items));
					return true;
				}
			}
		}
	}
	return false;
}
//---------------------------------------------------------------------
wchar_t *OperatorsBaseGrid::Cell(unsigned row, unsigned col)
{
	return (impl->__array__[col])(impl->data[row]);
}
 //------------------------------------------------------------------------
 int OperatorsBaseGrid::Count()
 {
	 return impl->data.size();
 }
 //-------------------------------------------------------------------------