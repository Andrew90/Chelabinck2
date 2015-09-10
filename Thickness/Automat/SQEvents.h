#pragma once
#include "AppBase.h"

class SQEvents
{
public:
public:
	unsigned &input__;
	unsigned input;
	unsigned last;
	unsigned msk;
public:
	template<class T>struct Event;
	template<class T>struct On{};
	template<class T>struct Off{};

///\brief если поднимается бит - срабатывает событие с соответствующем именем 

#define EVENT_ON(name)template<>struct Event<On<name> >\
	{\
	typedef name inner_type;\
	HANDLE hEvent;\
	unsigned &bit;\
	Event(): bit(Singleton<InputsTable>::Instance().items.get<name>().value){}\
	wchar_t *Name(){return L#name##L"ON";}\
	void Do(){SetEvent(hEvent);}\
	};

///\brief если падает бит - срабатывает событие с соответствующем именем 

#define EVENT_OFF(name)template<>struct Event<Off<name> >\
	{\
	HANDLE hEvent;\
	unsigned &bit;\
	Event(): bit(Singleton<InputsTable>::Instance().items.get<name>().value){}\
	wchar_t *Name(){return L#name##L"OFF";}\
	void Do(){SetEvent(hEvent);}\
	};
#define EVENT_(name) EVENT_ON(name) EVENT_OFF(name)

		EVENT_(PipeAfterClamp1SQ1BitIn )
		EVENT_(PipeInletSQ3BitIn       )
		EVENT_(OutletPipeSQ4BitIn      )
		EVENT_(PipeClamp2BeforeSQ7BitIn)
	//	EVENT_(PipeOnMarkerSQ9BitIn    )
	    EVENT_OFF(PipeOnMarkerSQ9BitIn)
#undef EVENT_ON
#undef EVENT_OFF
///\brief переопределён метод Do - если срабатывает бит - вызывается метод Do
		template<>struct Event<On<PipeOnMarkerSQ9BitIn> > 
		{
			typedef PipeOnMarkerSQ9BitIn inner_type;
			struct Inner;
			Inner *inner;
			HANDLE hEvent;
			unsigned &bit;
			Event()
				: inner(NULL)
				, bit(Singleton<InputsTable>::Instance().items.get<PipeOnMarkerSQ9BitIn>().value)
			{}
			wchar_t *Name(){return L"PipeOnMarkerSQ9BitInON";}
			void Do();			
		};
#undef EVENT_
#define USER_EVENT(name)struct name{};\
		template<>struct Event<name>\
		{\
		HANDLE hEvent;\
		wchar_t *Name(){return L#name;}\
		};
		USER_EVENT(AutomaticMode)
		USER_EVENT(SingleMode)
#undef USER_EVENT
	typedef TL::MkTlst<
		Event<On<PipeAfterClamp1SQ1BitIn > >
		, Event<On<PipeInletSQ3BitIn       > >
		, Event<On<OutletPipeSQ4BitIn      > >
		, Event<On<PipeClamp2BeforeSQ7BitIn> >
		, Event<On<PipeOnMarkerSQ9BitIn    > >

		, Event<Off<PipeAfterClamp1SQ1BitIn > >
		, Event<Off<PipeInletSQ3BitIn       > >
		, Event<Off<OutletPipeSQ4BitIn      > >
		, Event<Off<PipeClamp2BeforeSQ7BitIn> >
		, Event<Off<PipeOnMarkerSQ9BitIn    > >
		, Event<AutomaticMode    > 
		, Event<SingleMode    > 
	>::Result type_items;
	TL::Factory<type_items> items;
public:
	SQEvents();
	void Init();
	void Destroy();
	void Do();
	void Reset();
};

extern SQEvents sQEvents;