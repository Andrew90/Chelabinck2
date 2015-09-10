#include "stdafx.h"
#include "Dialogs.h"
#include "AppBase.h"
#include "Messages.h"
#include "ArchiveEvents.h"
#include <locale.h>

void CoordinatesDlg::Do(HWND h)
{
	CBase base(ParametersBase().name());
	if(base.IsOpen())
	{
		RECT r;
		GetWindowRect(h, &r);
		ArchiveWindowTable o;
		o.items.get<OffsetWindowX>().value = r.left;
		o.items.get<OffsetWindowY>().value = r.top;
		o.items.get<WindowWidth>().value = r.right - r.left;
		o.items.get<WindowHeight>().value = r.bottom - r.top;
		UpdateWhere<ArchiveWindowTable>(o, base).ID(1).Execute();
	}
}
/*
GetModuleFileName(0, path, dimention_of(path));
	PathRemoveFileSpec(path);
	wcscat(path, L"\\ParametersBase");
	return path;

	tm *timeinfo = localtime (&map->data[i].time);
		wsprintf(buf, L"%.2d:%.2d:%.2d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

		struct Num{};
struct Time{};
struct Data{};
struct Message{};
*/
DEFINE_PARAM(Num, int, 0)
DEFINE_PARAM(TimeEvent, COleDateTime, 0.0)
DEFINE_PARAM(Data, double, 0)
DEFINE_PARAM(MessageId, int, 0)

struct TextMessage
{
	typedef Holder<128> type_value;
	type_value value;
	type_value default_value;
	const wchar_t *name(){return L"TextMessage";}
	TextMessage()
	{
		value = L"";
		default_value = L"";
	}
};

struct DataTable 
{
	typedef TL::MkTlst<	
		 Num
	   , TimeEvent
	   , Data
	   , MessageId
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"DataTable";}
};

struct TextTable 
{
	typedef TL::MkTlst<	
		Num
		, TextMessage
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"TextTable";}
};

struct SaveEvents
{
   typedef TL::MkTlst<
	  DataTable
	  , TextTable
	>::Result type_list;
	typedef TL::Factory<type_list> TTables;
   TTables tables;
   wchar_t path[256];
};

void Insert(CBase &base, int num, wchar_t *title)
{
	TextTable t;
	t.items.get<Num>().value = num;
	t.items.get<TextMessage>().value = title;
	int id = Select<TextTable>(base).eq_all<TextTable::items_list>(&t.items).Execute();
	if(0 == id)
	{
		Insert_Into<TextTable>(t, base).Execute();
	}
}

template<class O, class P>struct __insert_text_message__
{
	void operator()(O *, P *p)
	{
		Insert(*p, TL::IndexOf<message_list, O>::value, MessageField<O>::Text());
	}
};
extern ClientEvents clientEvents;
#if 0
void SaveDlg::Do(HWND h)
{
	wchar_t path[512];
	GetModuleFileName(0, path, dimention_of(path));
	PathRemoveFileSpec(path);
	wcscat(path, L"\\ArchiveEvent\\");
	CreateDirectory(path, NULL);
	int length = wcslen(path);
	time_t t = time(NULL);
	tm *timeinfo = localtime (&t);
	wsprintf(&path[length], L"%.2d%.2d%.2d", timeinfo->tm_year - 100, 1 + timeinfo->tm_mon, timeinfo->tm_mday);
	SaveEvents parameters;
	CBase base(
		path
		, CreateDataBase<SaveEvents::type_list >()
		, parameters.tables
		);

	if(base.IsOpen())
	{
		TL::foreach<message_list, __insert_text_message__>()((TL::Factory<message_list> *)0, &base);

		for(unsigned i = clientEvents.map->head; i > clientEvents.map->tail; --i)
		{
			ArchiveEvents::Data d = clientEvents.map->data[i % dimention_of(clientEvents.map->data)];

			DataTable t;
			t.items.get<Num>().value = i;
			t.items.get<TimeEvent>().value = d.time;
			t.items.get<Data>().value = d.data;
			t.items.get<MessageId>().value = d.event;

			int id = Select<DataTable>(base).eq_all<DataTable::items_list>(&t.items).Execute();
			if(0 == id)
			{
				Insert_Into<DataTable>(t, base).Execute();
			}
		}

		MessageBox(h, L"Данные сохранены", L"Сообщение", MB_ICONINFORMATION);
	}
}
#else
void SaveDlg::Do(HWND h)
{
	wchar_t path[512];
	GetModuleFileName(0, path, dimention_of(path));
	PathRemoveFileSpec(path);
	wcscat(path, L"\\ArchiveEvent\\");
	CreateDirectory(path, NULL);
	int length = wcslen(path);
	time_t t = time(NULL);
	tm *timeinfo = localtime (&t);
	wsprintf(&path[length], L"%.2d%.2d%.2d%.2d%.2d%.2d.txt"
		, timeinfo->tm_year - 100, 1 + timeinfo->tm_mon, timeinfo->tm_mday
		, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec
		);	
	FILE *f = _wfopen(path, L"w+");
	if(NULL != f)
	{
		for(unsigned i = clientEvents.map->head; i > clientEvents.map->tail; --i)
		{
			ArchiveEvents::Data d = clientEvents.map->data[i % dimention_of(clientEvents.map->data)];
			
			wchar_t buf[2048];
			wchar_t *h;
			h = buf;

			wchar_t *s = clientEvents.NumberMessage(i);
			wcscpy(h, s);
			h +=  wcslen(h);
			wcscpy(h, L"  ");
			h +=  wcslen(h);

			s = clientEvents.GetTime(i);
			wcscpy(h, s);
			h +=  wcslen(h);
			wcscpy(h, L"  ");
			h +=  wcslen(h);

			s = clientEvents.GetData(i);
			wcscpy(h, s);
			h +=  wcslen(h);
			wcscpy(h, L"  ");
			h +=  wcslen(h);

			s = clientEvents.GetText(i);
			wcscpy(h, s);
			h +=  wcslen(h);
			wcscpy(h, L"  ");
			h +=  wcslen(h);

			wcscpy(h, L"\n");

		   char w[2048];
           CharToOemW(buf, w);
           fwrite (w, 1, 1 + (h - buf), f);
		}
		fclose(f);
	}

		MessageBox(h, L"Данные сохранены", L"Сообщение", MB_ICONINFORMATION);
}
#endif