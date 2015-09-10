#pragma once

#include "typelist.hpp"
#include "AppBase.h"
class OutBitDown
{
public:
	static const int delay = 500;
	template<class T>struct Bit
	{
		unsigned time;
		const unsigned &bit;
		Bit()
			: bit(Singleton<OutputsTable>::Instance().items.get<T>().value)
		{}
	};
	
	typedef TL::MkTlst<
		SensorUnitRemovalY1BitOut, SensorUnitSupplyY2BitOut
		, SensorsRemovalY3BitOut, SensorsSupplyY4BitOut
		//, PipeMarkerY5BitOut
	>::Result list;
	typedef TL::TypeToTypeLst<list, Bit>::Result items_list;
	unsigned &output;
	unsigned msk;
	unsigned bitsUp;
	TL::Factory<items_list> items;
public:
	OutBitDown();
	void Init();
	void Do();
};

extern OutBitDown outBitDown;
