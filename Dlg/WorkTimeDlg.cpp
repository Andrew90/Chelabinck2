#include "stdafx.h"
#include "ParamDlg.h"
#include "ParamDlg.hpp"
#include "Dialogs.h"
#include <commctrl.h>
#include "MainWindow.h"
#include "DlgItems.h"
#include "DebugMess.h"
#ifdef XDEBUG
#define dprint debug.print
#else	
#define  dprint
#endif
#if 0
template<template<class>class Wapper, class Z>struct Skip<Wapper<OperatorName>, Z>
{
	typedef Wapper<OperatorName> O;
	template<class P>bool operator()(O *o, P *p)
	{
		wchar_t buf[128];
		ComboBox_GetText(o->hWnd, buf, dimention_of(buf));
		if(0 == wcslen(buf))
		{
			MessageBox(*p, L"Недопустимое имя оператора", L"Предупреждение", MB_ICONWARNING);
			return false;
		}
		return true;
	}
};

PARAM_TITLE(OperatorName, L"Имя оператора")

template<>struct DlgSubItems<OperatorName, Holder<128> >: ComboEditSubItem<OperatorName>{};

template<>struct FillComboboxList<OperatorName>
{
	void operator()(HWND h, OperatorName &t)
	{
		CBase base(ParametersBase().name());
		ADODB::_RecordsetPtr rec;
		Select<WorkTimeTable>(base).Execute(rec);
		OperatorName::type_value tmp;
		while (!rec->EndOfFile) 
		{
			tmp = _bstr_t(rec->Fields->GetItem(t.name())->GetValue());
			ComboBox_AddString(h, tmp.buffer);
			rec->MoveNext(); 
		}					
	}
};

template<>struct CurrentValue<OperatorName>
{
	void operator()(HWND h, OperatorName &t)
	{
		ComboBox_SetText(h, t.value.buffer);
	}
};

template<>struct __ok_table_btn__<WorkTimeTable, ParametersBase::simple_table_list>
{
	typedef WorkTimeTable Table;
	template<class T>bool operator()(HWND h, T &t)
	{
		if(!TL::find<typename T::list, __test__>()(&t.items, &h))return false;
		CBase base(ParametersBase().name());
		if(base.IsOpen())
		{
			__update_data__<Table> _data(base);
			TL::foreach<T::list, __ok_btn__>()(&t.items, &_data);
			unsigned id = Select<Table>(base).eq<OperatorName>(t.table.items.get<OperatorName>().value).Execute();
		    if(0 == id)
			{
				Insert_Into<Table>(t.table, base).Execute();
				id = Select<Table>(base).eq<OperatorName>(t.table.items.get<OperatorName>().value).Execute();
			}
			Update<CurrentParametersTable>(base).set<CurrentOperatorID>(id).Where().ID(1).Execute();
		}
		WorkTimeTable &s = Singleton<WorkTimeTable>::Instance();
		wchar_t buffer[128];
		wchar_t *_0 = s.items.get<OperatorName>().value;
		wsprintf(buffer, L"Оператор %s", _0);// смена %d", _0, _1);
		SendMessage(mainWindow.hStatusWindow, SB_SETTEXT, 2, (LONG)buffer);
		return true;
	}
};

struct WorkOkBtn
{
	static const int width = 120;
	static const int height = 30;
	static const int ID = IDOK;
	wchar_t *Title(){return L"Применить";}
	template<class Owner>void BtnHandler(Owner &owner, HWND h)
	{
		if(__ok_table_btn__<
			Owner::Table
			, typename TL::SubListFromMultyList<ParametersBase::multy_type_list, Owner::Table>::Result
		>()(h, owner))
		{
			EndDialog(h, TRUE);
		}
	}
};

struct WorkCancelBtn
{
	static const int width = 120;
	static const int height = 30;
	static const int ID = IDCANCEL;
	wchar_t *Title(){return L"Закрыть";}
	template<class Owner>void BtnHandler(Owner &o, HWND h)
	{
	    WorkTimeTable &s = Singleton<WorkTimeTable>::Instance();
		wchar_t *_0 = s.items.get<OperatorName>().value;
		if(wcslen(_0) > 0)
		{			
			wchar_t buffer[128];
//			int _1 = s.items.get<WorkTime>().value;
			wsprintf(buffer, L"Оператор %s", _0);// смена %d", _0, _1);
			SendMessage(mainWindow.hStatusWindow, SB_SETTEXT, 2, (LONG)buffer);
			EndDialog(h, FALSE);
		}
		else
		{
			MessageBox(h, L"Недопустимое имя оператора", L"Предупреждение", MB_ICONWARNING);
		}	    
	}
};

struct WorkDeleteBtn
{
	static const int width = 160;
	static const int height = 30;
	static const int ID = IDRETRY;
	wchar_t *Title(){return L"Удалить";}
	template<class Owner>void BtnHandler(Owner &o, HWND h)
	{
		HWND hW = o.items.get<DlgItem<OperatorName> >().hWnd;
		if(ComboBox_GetCount(hW) > 1)
		{
			wchar_t buf[128];
			ComboBox_GetText(hW, buf, dimention_of(buf));
			int index = ComboBox_FindString(hW, 0, buf);
			if(ComboBox_DeleteString(hW, index))
			{
				CBase base(ParametersBase().name());
				if(base.IsOpen())
				{
					OperatorName::type_value val;
					val = buf;
					Delete<WorkTimeTable>(base).eq<OperatorName>(val).Execute();
					ComboBox_SetCurSel(hW, 0);
					__ok_table_btn__<WorkTimeTable, ParametersBase::simple_table_list>()(h, o);
				}
			}
		}
		else
		{
			MessageBox(GetParent(h), L"Должно быть больше 1 оператора", L"Предупреждение", MB_ICONWARNING);
		}
	}
};
#endif
void WorkTimeDlg::Do(HWND h)
{
	//if(TemplDialog<WorkTimeTable, TL::MkTlst<WorkOkBtn, WorkCancelBtn, WorkDeleteBtn>::Result>(Singleton<WorkTimeTable>::Instance()).Do(h, L"Операторы"))
	{
	}
}
