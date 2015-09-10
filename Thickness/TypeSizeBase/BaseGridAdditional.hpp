#pragma once
#include "DebugMess.h"
#ifdef XDEBUG
#define dprint debug.print
#else	
#define  dprint
#endif	
template<class T>struct __local_buffer__;
template<>struct __local_buffer__<double>
{
	wchar_t buf[64];
	wchar_t *operator()(double data)
	{
		char c[64];
		_gcvt_s(c, 64, data, 5);
		int len = strlen(c) - 1;
		if('.' == c[len]) c[len] = 0;
		size_t ConvertedChars;
		mbstowcs_s(&ConvertedChars, buf, c, 60);
		return buf;
	}
};
template<>struct __local_buffer__<int>
{
	wchar_t buf[64];
	wchar_t *operator()(int data)
	{
		_itow(data, buf, 10);
		return buf;
	}
};
template<int N>struct __local_buffer__<Holder<N> >
{
	wchar_t *operator()(Holder<N> &buf){return buf.buffer;}
};

template<class T>struct __set_data_to_grid__
{
	typename __local_buffer__<typename TL::NoWapperType<T>::Result::type_value> buffer;

   template<class Z>wchar_t *operator()(Z &z)
   {
	   return buffer(z.get<T>().value);
   }
};

template<int N>struct from_variant<Holder<N> >
{
	wchar_t *operator()(_variant_t &v)
	{
		return v.bstrVal;
	}
};
struct __set_to_item_data__
{
	ADODB::_RecordsetPtr rec;
};
template<class O, class P>struct __set_to_item__
{
	void operator()(O *o, P *p)
	{
		typedef typename TL::GetInnerType<typename TL::GetInnerType<O>::Result>::Result T;
		o->value = from_variant<typename T::type_value>()(p->Fields->GetItem(T().name())->GetValue());
	}
};

template<int N>struct Test<Holder<N> >
{
	bool operator()(wchar_t *value)
	{
		return 0 != value[0];
	}
};

template<class T>struct __testing_input_date_data__
{
	typedef T Type;
	HWND *hEdit;
	HWND ownerHWND;	
	__testing_input_date_data__(HWND *h, HWND o) :hEdit(h), ownerHWND(o){}
};
template<class T> T &__convert__(T &t){return t;}
inline wchar_t *__convert__(std::wstring &t){return (wchar_t *)t.c_str();}

template<int N> struct Wchar_to<Holder<N> >
{
	wchar_t *operator()(wchar_t *data)
	{
		return data;
	}
};

template<class O, class P>struct __testing_input_date__
{
	bool operator()(O *o, P *p)
	{
		static const int i = TL::IndexOf<typename P::Type::items_list, O>::value;
		wchar_t buffer[256];
		GetWindowText(p->hEdit[i], buffer, 256);
		typedef typename TL::GetInnerType<typename TL::GetInnerType<O>::Result>::Result::type_value type_value;
		if(Test<type_value>()(buffer))
		{
			o->value = Wchar_to<type_value>()(buffer);
		}
		else
		{
			MessageBox(p->ownerHWND, ___error_message_dlg__<typename TL::GetInnerType<O>::Result>()(), L"Ошибка", MB_ICONERROR);
			return false;
		}
		return true;
	}
};
template<class T>struct ___error_message_dlg__
{
	wchar_t *operator()(){return L"Поле должно быть заполнено";}
};

template<class T>struct TypeToTable;

template<class T>struct __base__table_data__
{
	typedef T Type;
	CBase &base;
	typename T::Items &items;
	__base__table_data__(CBase &base, typename T::Items &items): base(base), items(items){}
};
template<class O, class P>struct __set_date_to__
{
	void operator()(O *o, P *p)
	{
		o->value = p->items.get<P::Type::__item_grid__<O> >().value;
	}
};
template<class X, class P>struct __set_date_to__<ID<X>, P>
{
	typedef ID<X> O;
	void operator()(O *o, P *p)
	{
		typedef TypeToTable<X>::Result Table;
		Table table;
		table.items.get<X>().value = __convert__(p->items.get<P::Type::__item_grid__<O> >().value);
		int id = Select<Table>(p->base).eq<X>(table.items.get<X>().value).Execute();
		if(0 == id)
		{
			Insert_Into<Table>(table, p->base).Execute();
			id = Select<Table>(p->base).eq<X>(table.items.get<X>().value).Execute();
		}
		o->value = id;
	}
};

template<class O, class P>struct __delete_from_tables__{void operator()(O *, P *){}};

template<class X, class P>struct __delete_from_tables__<ID<X>, P>
{
	typedef ID<X> O;
	void operator()(O *o, P *p)
	{
		ADODB::_RecordsetPtr rec;
		Select<TypeSizeTable>(p->base).eq<O>(o->value).Execute(rec);
		if(rec->EndOfFile)
		{
			Delete<typename TypeToTable<X>::Result>(p->base).ID(o->value).Execute();
		}
	}
};
template<class T>struct __insert_from_other_tables_data__
{
	typedef T Type;
	typename T::Items &items;
	CBase &base;
	__insert_from_other_tables_data__(typename T::Items &items, CBase &base) : items(items), base(base){}
};
template<class O, class P>struct __insert_from_other_tables__
{
	void operator()(O *o, P *p)
	{
		o->value = p->items.get<P::Type::__item_grid__<O> >().value;
	}
};
template<class X, class P>struct __insert_from_other_tables__<ID<X>, P>
{
	typedef ID<X> O;
	void operator()(O *o, P *p)
	{
		o->value = Select<typename TypeToTable<X>::Result>(p->base).eq<X>(p->items.get<P::Type::__item_grid__<O> >().value).Execute();
	}
};
