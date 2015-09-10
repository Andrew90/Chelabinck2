#include "stdafx.h"
#include "EditOperatorsViewer.h"
#include <strsafe.h>
#include "Grid.h"
#include "typelist.hpp"
#include "ArchiveEvents.h"
#include "TypeSizeBase.h"

#include "DebugMess.h"
#ifdef XDEBUG
#define dprint debug.print
#else	
#define  dprint
#endif	

template<class T>struct header_table;
#define Z(T, w, txt)template<>struct header_table<T>\
{\
	LPWSTR name(){return L###txt;}\
	static const int width = w;\
};

Z(NameOperator   , 300, Имя оператора)
Z(PersonnelNumber, 200, Табельный номер)
#undef Z

typedef TL::MkTlst<
   NameOperator
   , PersonnelNumber
>::Result ParameterNameList;

#pragma warning(disable : 4996)
//--------------------------------------------------------------------------------
void EditOperatorsGrid::SetHeaderToGrid(HWND h)
{
	SetGridHeader<ParameterNameList> x(h);
}
//--------------------------------------------------------------------------------
void EditOperatorsGrid::SetDataToGrid(LV_DISPINFO *d)
{	
	int i = d->item.iItem;
	if(i >= 0 && i < typeSizeBaseGrid.Count())
	{
		wchar_t *s = typeSizeBaseGrid.Cell((unsigned)i, d->item.iSubItem);
		if(0 != *s) StringCchCopy(d->item.pszText, wcslen(s) + 1, s);
	}
}
//------------------------------------------------------------------------------------
void EditOperatorsGrid::LClick(LPNMITEMACTIVATE d)
{
	if(d->iItem >= 0 && d->iSubItem >= 0 && d->iSubItem < dimention_of(hEdits))
	{
		wchar_t buffer[256];
		ListView_GetItemText(d->hdr.hwndFrom, d->iItem, d->iSubItem, buffer, 256);
		SetWindowText(hEdits[d->iSubItem], buffer); 
	}
}
//---------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
void SelectOperatorGrid::LClick(LPNMITEMACTIVATE d)
{
	if(d->iItem >= 0 && d->iSubItem >= 0 && d->iSubItem < dimention_of(hEdits))
	{
		wchar_t buffer[256];
		ListView_GetItemText(d->hdr.hwndFrom, d->iItem, 0, buffer, dimention_of(buffer));
		SetWindowText(hEdits[0], buffer);
		ListView_GetItemText(d->hdr.hwndFrom, d->iItem, 1, buffer, dimention_of(buffer));
		SetWindowText(hEdits[1], buffer);
	}
}
//----------------------------------------------------------------------------------------

