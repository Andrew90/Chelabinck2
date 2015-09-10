#include "stdafx.h"
#include "Debug1730.h"
#include <MMSystem.h>
#include "Automat.h"
#include "AppBase.h"
#ifdef DEBUG_ITEMS
#include "InOutBits.h"
#include "ArchiveEvents.h"
#include "DebugMess.h"
#ifdef XDEBUG
#define dprint debug.print
#else	
#define  dprint
#endif
#define inp(n) input_bits.get<n>().value



//-----------------------------------------------------------------------
void Debug1730::Start()
{
	t_output = 0;
}
//------------------------------------------------------------------------

//------------------------------------------------------------------------
void Debug1730::Write()
{
	if(output != t_output)
	{
	t_output = output;
	}
}
void Debug1730::Write(unsigned d)
{
	output =d;
	if(output != t_output)
	{
	//dprint("Debug1730::Write %.8x", d);
	t_output = output;
	}
}
unsigned Debug1730::ReadOutput()
{
	return output;
}
//------------------------------------------------------------------------
//#define ON(n) input |= Singleton<InputsTable>::Instance().items.get<n>().value;
//#define OFF(n) input &= ~Singleton<InputsTable>::Instance().items.get<n>().value;

typedef bool(*Tptr)();
unsigned g_input = 0;
unsigned g_last = 0;
unsigned Debug1730::Read()
{
	Do();
	if(g_input != g_last)
	{
		dprint("input %x", g_input);
		g_last = g_input;
	}
	return g_input;
}
template<bool b, class T, unsigned delay>struct xState
{
	unsigned time;
	bool(xState::*ptr)();
	xState()
	{
       ptr = &xState::Before;
	}
	bool Before()
	{
		time = timeGetTime();
		time += delay;
		ptr = &xState::After;
		if(b)
		{
			g_input |= Singleton<InputsTable>::Instance().items.get<T>().value;
		}
		else
		{
			g_input &= ~Singleton<InputsTable>::Instance().items.get<T>().value;
		}
		return false;
	}
	bool After()
	{
		if(time > timeGetTime())return false;
		ptr = &xState::Before;
		return true;
	}
	static bool Do()
	{
		static xState x;
		return (x.*x.ptr)();
	}
};

template<unsigned delay>struct xSleep
{
	unsigned time;
	bool(xSleep::*ptr)();
	xSleep()
	{
       ptr = &xSleep::Before;
	}
	bool Before()
	{
		time = timeGetTime();
		time += delay;
		ptr = &xSleep::After;
		return false;
	}
	bool After()
	{
		if(time > timeGetTime())return false;
		ptr = &xSleep::Before;
		return true;
	}
	static bool Do()
	{
		static xSleep x;
        return (x.*x.ptr)();
	}
};

typedef TL::MkTlst<
	        xSleep<5000>
			, xState<true, PipeAfterClamp1SQ1BitIn, 400>
			, xState<true, PipeInletSQ3BitIn, 1400>
			, xState<true, OutletPipeSQ4BitIn, 1200>
			, xState<true, PipeClamp2BeforeSQ7BitIn, 5000>
			, xState<true, PipeOnMarkerSQ9BitIn, 8000>

			, xState<false, PipeAfterClamp1SQ1BitIn, 400>
			, xState<false, PipeInletSQ3BitIn, 1400>
			, xState<false, OutletPipeSQ4BitIn, 1200>
			, xState<false, PipeClamp2BeforeSQ7BitIn, 5000>
			, xState<false, PipeOnMarkerSQ9BitIn, 10>
>::Result list;

template<class O, class P>struct __init_test_array__
{
	void operator()(O *, P *p)
	{
		p[TL::IndexOf<list, O>::value] = O::Do;
	}
};
Tptr arr[TL::Length<list>::value];
int offset = TL::Length<list>::value;
void Debug1730::Do()
{	
	if(offset < TL::Length<list>::value)
	{
		if((arr[offset])()) ++offset;
	}
	else
	{
	//	if(automat.automatMode || automat.manualMode) offset = 0;
	}
	Sleep(10);
}
//-------------------------------------------------------------------------
#define ON(n) g_input |= Singleton<InputsTable>::Instance().items.get<n##BitIn>().value;
Debug1730::Debug1730()
	: contine(true)
	, input(archiveEvents.map->input)
	, output(archiveEvents.map->output)
{
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc, this, 0, NULL);
	TL::foreach<list, __init_test_array__>()((TL::Factory<list> *)0, arr);
	ON(Clamp1RetractedSQ2)
	ON(Clamp2RetractedSQ8)
	ON(NormalAirPressurePa)
	ON(SensorsDivorcedSQ6)
}
//------------------------------------------------------------------------
void Debug1730::ThreadProc(Debug1730 *d)
{
	d->Do();
}
//---------------------------------------------------------------------------
void Debug1730::Destroy()
{
	contine = false;
	CloseHandle(hThread);
}
//---------------------------------------------------------------------------
#endif