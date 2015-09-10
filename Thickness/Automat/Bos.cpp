#include "stdafx.h"
#include "Bos.h"
#include <MMSystem.h>
#include "ArchiveEvents.h"
#include "InOutBits.h"
#include "AppBase.h"
#include "DebugMess.h"
#ifdef XDEBUG
#define dprint debug.print
#else	
#define  dprint
#endif	
//---------------------------------------------------------------------------
Bos::Bos()
	: count(0)
	, delay(0)
	, output(archiveEvents.map->output)
	, input(archiveEvents.map->input)
{}
//------------------------------------------------------------------------
void Bos::PowerOn()
{
	if(0 >= count && 0 == delay)
	{
		dprint("PowerBitOut On");
		output |= Singleton<OutputsTable>::Instance().items.get<PowerBitOut>().value;
        delay = timeGetTime() + 1000;
	}
}
//------------------------------------------------------------------------
void Bos::On()
{
	if(0 >= count)
	{
		
		dprint("PowerBitOut On");
		count = 0;
		int t = -1000;
		if(0 == (input & Singleton<InputsTable>::Instance().items.get<BlockErrorBitIn>().value))
		{
			t = timeGetTime() - delay;
		}
		output |= Singleton<OutputsTable>::Instance().items.get<PowerBitOut>().value;
		if(t < 0) 
		{
			Sleep(-t);	
		    dprint("sleep %d", -t);
		}
	}
	++count;
	output |= Singleton<OutputsTable>::Instance().items.get<StartBitOut>().value;
	dprint("StartBitOut On");
}
//------------------------------------------------------------------------
void Bos::Off()
{
	--count;
	if(0 >= count)
	{
		delay = 0;
        output &= ~Singleton<OutputsTable>::Instance().items.get<StartBitOut>().value;
		Sleep(1000);
		output &= ~Singleton<OutputsTable>::Instance().items.get<PowerBitOut>().value;
		dprint("PowerBitOut OFF");
	}
	output &= ~Singleton<OutputsTable>::Instance().items.get<StartBitOut>().value;
	dprint("StartBitOut OFF");
}
//------------------------------------------------------------------------
void Bos::PowerDown()
{
	delay = 0;
	count = 0;
	output &= ~Singleton<OutputsTable>::Instance().items.get<StartBitOut>().value;
	dprint("StartBitOut OFF");
	Sleep(1000);
	output &= ~Singleton<OutputsTable>::Instance().items.get<PowerBitOut>().value;
	dprint("PowerBitOut OFF");
}
//-----------------------------------------------------------------------------
Bos bos;