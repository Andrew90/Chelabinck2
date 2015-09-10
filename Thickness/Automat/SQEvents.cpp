#include "stdafx.h"
#include "SQEvents.h"
#include "ArchiveEvents.h"

#include "DebugMess.h"
#ifdef XDEBUG
#define dprint debug.print
#else	
#define  dprint
#endif	

namespace 
{
	template<class O, class P>struct __init__
	{
		void operator()(O *o, P *p)
		{
			o->hEvent = CreateEvent(NULL, FALSE, TRUE, o->Name());
		}
	};
//------------------------------------------------------------------------------------
	template<class List>struct __on_list__;
	template<class Head, class Tail>struct __on_list__<Tlst<SQEvents::Event<SQEvents::On<Head> >, Tail> >
	{
		typedef Tlst<SQEvents::Event<SQEvents::On<Head> >, typename __on_list__<Tail>::Result> Result;
	};	
	template<class Head, class Tail, template<class >class Wapper>struct __on_list__<Tlst<SQEvents::Event<Wapper<Head> >, Tail> >
	{
		typedef typename __on_list__<Tail>::Result Result;
	};
	template<class Head, class Tail>struct __on_list__<Tlst<Head, Tail> >
	{
		typedef typename __on_list__<Tail>::Result Result;
	};
	template<>struct __on_list__<NullType>
	{
		typedef NullType Result;
	};
#if 0
	template<class O, class P>struct __do__
	{
		void operator()(O *o, P *p)
		{
			if(o->bit & p->msk)
			{
				if(o->bit & p->input) 
				{
					SetEvent(o->hEvent);
					p->last |= o->bit;
				}
				else
				{
					typedef SQEvents::Event<SQEvents::Off<typename O::inner_type> > T;
					SetEvent(p->items.get<T>().hEvent);
					p->last &= ~o->bit;
				}
			}
		}
	};
#else
	template<class O, class P>struct __do__
	{
		void operator()(O *o, P *p)
		{
			if(o->bit & p->msk)
			{
				if(o->bit & p->input) 
				{
					o->Do();
					p->last |= o->bit;
				}
				else
				{
					typedef SQEvents::Event<SQEvents::Off<typename O::inner_type> > T;
					p->items.get<T>().Do();
					p->last &= ~o->bit;
				}
			}
		}
	};
#endif
}
//------------------------------------------------------------------------------------
SQEvents::SQEvents(): input__(archiveEvents.map->input) {}
//----------------------------------------------------------------------------------
void SQEvents::Init()
{
	TL::foreach<type_items, __init__>()(&items, (int *)0);
}
//----------------------------------------------------------------------------------
void SQEvents::Destroy()
{
}
//----------------------------------------------------------------------------------
void SQEvents::Do()
{
	{
		input = input__;
		msk = input ^ last;
		if(0 != msk)
		{
			TL::foreach<__on_list__<type_items>::Result, __do__>()(&items, this);
		}
	}
}
//----------------------------------------------------------------------------------
void SQEvents::Reset()
{
	last = 0;
}
//------------------------------------------------------------------------------------
SQEvents sQEvents;