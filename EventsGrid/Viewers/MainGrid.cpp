#include "stdafx.h"
#include "MainGrid.h"
#include <strsafe.h>
#include "Grid.h"
#include "typelist.hpp"
#include "ArchiveEvents.h"

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
struct Num{};
struct Time{};
struct Data{};
struct Message{};

Z(Num    , 100, №)
Z(Time   , 130, Время)
Z(Data   , 120, Данные)
Z(Message, 470, Сообщение)
#undef Z

typedef TL::MkTlst<
 Num    
 , Time   
 , Data   
 , Message
>::Result ParameterNameList;
//-----------------------------------------------------------------------------------
template<class T>struct DataToGrid
{
	typedef typename T::__template_must_be_overridded__ noused;
};
//------------------------------------------------------------------------------------
extern ClientEvents clientEvents;
//-------------------------------------------------------------------------------------
#pragma warning(disable : 4996)
template<>struct DataToGrid<Num>
{
	wchar_t *operator()(unsigned i)
	{
		return  clientEvents.NumberMessage(i);
	}
};
//-----------------------------------------------------------------------------------------
template<>struct DataToGrid<Message>
{
	wchar_t *operator()(unsigned i)
	{
		return clientEvents.GetText(i);
	}
};
//-----------------------------------------------------------------------------------------
template<>struct DataToGrid<Time>
{
	wchar_t *operator()(unsigned i)
	{
		return clientEvents.GetTime(i);
	}
};
//---------------------------------------------------------------------------------------
template<>struct DataToGrid<Data>
{
	wchar_t *operator()(unsigned i)
	{
		return clientEvents.GetData(i);
	}
};
//--------------------------------------------------------------------------------------
template<class >struct ColorToGrid
{
	void operator()(ClientEvents::DataColor &d)
	{
		d.d->clrText = 0;;
		d.d->clrTextBk = -1;
	}
};
template<>struct ColorToGrid<Data>
{
	void operator()(ClientEvents::DataColor &d)
	{
		clientEvents.GetDataColor(d);
	}
};
template<>struct ColorToGrid<Message>
{
	void operator()(ClientEvents::DataColor &d)
	{
		clientEvents.GetTextColor(d);
	}
};
//---------------------------------------------------------------------------------------
typedef TL::ListToWapperList<ParameterNameList, DataToGrid>::Result __proc_list__;
TL::Factory<__proc_list__> rows;
TL::ArrayFunc<__proc_list__, unsigned, wchar_t *> dataFunc(rows);

typedef TL::ListToWapperList<ParameterNameList, ColorToGrid>::Result __proc_color__list__;
TL::Factory<__proc_color__list__> color_rows;
TL::ArrayFunc<__proc_color__list__, ClientEvents::DataColor &, void> dataColorFunc(color_rows);
//--------------------------------------------------------------------------------
VOID WINAPI UpdateListView(HWND h, bool)
{
	if(mainGrid.start != clientEvents.map->head)
	{
		mainGrid.start = clientEvents.map->head;
		ListView_Update(h, 0);
	}
}
//--------------------------------------------------------------------------------
MainGrid::MainGrid() :hTimer(NULL)
{
	start = 0;
}
//--------------------------------------------------------------------------------
void MainGrid::SetHeaderToGrid(HWND h)
{
	SetGridHeader<ParameterNameList> x(h);
	SetRow(h, 1000);
}
//--------------------------------------------------------------------------------
void MainGrid::SetDataToGrid(LV_DISPINFO *d)
{	
	unsigned i = start - d->item.iItem;
	//dprint("%d %d", d->item.iSubItem, i);
	wchar_t *c = dataFunc[d->item.iSubItem](i);
	StringCchCopy(d->item.pszText, wcslen(c) + 1, c);		
}
//-----------------------------------------------------------------------------------
void MainGrid::SetColorToGrid(NMLVCUSTOMDRAW *d)
{
	int t = start - d->nmcd.dwItemSpec;
	//dprint("__%d", t);
	if(t > 0)
	dataColorFunc[d->iSubItem](ClientEvents::DataColor(d, start - d->nmcd.dwItemSpec));
}
//------------------------------------------------------------------------------------
void MainGrid::Start(HWND h)
{
	CreateTimerQueueTimer(&hTimer, NULL, (WAITORTIMERCALLBACK)UpdateListView, h, 1000, 1000, WT_EXECUTEDEFAULT);
}
//------------------------------------------------------------------------------------------
void MainGrid::Stop()
{
	if(hTimer)DeleteTimerQueueTimer( NULL, hTimer, NULL );
}
//--------------------------------------------------------------------------------------------


