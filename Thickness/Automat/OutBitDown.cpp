#include "stdafx.h"
#include "OutBitDown.h"
#include <MMSystem.h>
#include "ArchiveEvents.h"
namespace OutBitDownSpace
{
	template<class O, class P>struct __init__
	{
		void operator()(O *o, P *p)
		{
			o->time = 0;
			p->msk |= o->bit;
		}
	};
	template<class O, class P>struct __do__
	{
		void operator()(O *o, P *p)
		{
			if(p->bitsUp & o->bit)
			{
				unsigned t = timeGetTime();
				if(0 == o->time) o->time = t + OutBitDown::delay;
				else if(t > o->time)
				{
					o->time = 0;
					p->output &= ~o->bit;
				}
			}			
		}
	};
}

OutBitDown::OutBitDown()
	: output(archiveEvents.map->output)
{}

void OutBitDown::Init()
{
	msk = 0;
	TL::foreach<items_list, OutBitDownSpace::__init__>()(&items, this);
}
void OutBitDown::Do()
{
	bitsUp = output & msk;
	if(bitsUp)
	{
		TL::foreach<items_list,  OutBitDownSpace::__do__>()(&items, this);
	}
}

