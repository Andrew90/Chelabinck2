#include "stdafx.h"
#include "EditTypeSizeViewer.h"
#include <strsafe.h>
#include "Grid.h"
#include "typelist.hpp"
#include "ArchiveEvents.h"
#include "TypeSizeBase.h"
#include "EditTypeSizeViewer.hpp"

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

Z(NormativeTechnicalDocument, 300, Нормативно-технический документ)
Z(SteelGrade				, 200, Марка стали)
Z(TubeDiameter				, 75, Диаметр трубы)
Z(NominalThickness			, 75, Толщ.стенки)
Z(NominalPercentMin			, 75, Доп.-(%))
Z(MM<NominalPercentMin>			    , 75, Доп.-(мм))
Z(NominalPercentMax	        , 75, Доп.+(%))
Z(MM<NominalPercentMax>			    , 75, Доп.+(мм))
Z(DefectDifferentWallPercent, 75, Разн.(%))
Z(MM<DefectDifferentWallPercent>     , 75, Разн.(мм))
#undef Z


#pragma warning(disable : 4996)
//--------------------------------------------------------------------------------
void EditTypeSizeGrid::SetHeaderToGrid(HWND h)
{
	SetGridHeader<ParameterTypeSizeList> x(h);
	int count = typeSizeBaseGrid.Count();
}
//--------------------------------------------------------------------------------
void EditTypeSizeGrid::SetDataToGrid(LV_DISPINFO *d)
{	
	int i = d->item.iItem;
	if(i >= 0 && i < typeSizeBaseGrid.Count())
	{
		wchar_t *s = typeSizeBaseGrid.Cell((unsigned)i, d->item.iSubItem);
		if(0 != *s) StringCchCopy(d->item.pszText, wcslen(s) + 1, s);
	}
}
//-----------------------------------------------------------------------------------
void EditTypeSizeGrid::LClick(LPNMITEMACTIVATE d)
{
	if(d->iItem >= 0 && d->iSubItem >= 0 && d->iSubItem < dimention_of(hEdits))
	{
		wchar_t buffer[256];
		ListView_GetItemText(d->hdr.hwndFrom, d->iItem, d->iSubItem, buffer,  dimention_of(buffer));
		SetWindowText(hEdits[d->iSubItem], buffer); 
	}
}
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
void SelectTypeSizeGrid::LClick(LPNMITEMACTIVATE d)
{
	wchar_t buffer[256];
	if(d->iItem >= 0)
	{
		for(int i = 0; i < dimention_of(hEdits); ++i)
		{
			ListView_GetItemText(d->hdr.hwndFrom, d->iItem, i, buffer, dimention_of(buffer));
			SetWindowText(hEdits[i], buffer); 
		}
	}
}
//-------------------------------------------------------------------------------------------

