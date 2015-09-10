#pragma once
//E:\projects\tube_solution\templates\typelist.hpp
//E:\projects\tube_solution\Lan\LanParameters.h
#include "typelist.hpp"

#define DEFINE_PARAM(z, type, def_val) struct z\
{\
	typedef type type_value;\
    type_value value;\
	const type_value default_value;\
	const wchar_t *name(){return L#z;}\
	z() : value(def_val), default_value(def_val) {}\
};

DEFINE_PARAM(PacketSize, int, 986)//474)//986
DEFINE_PARAM(Gain0, int, 100)
DEFINE_PARAM(SyncLevel, int, 33)
DEFINE_PARAM(SyncGain, int, 10)
DEFINE_PARAM(StartDelay, int, 160)
//----------------------------------------------------------------
DEFINE_PARAM(Frequency, int, 25) //50
DEFINE_PARAM(NumberPackets, int, 100)

DEFINE_PARAM(SyncInput, bool, false)
DEFINE_PARAM(MeasurementInput, bool, false)
DEFINE_PARAM(SynchronizationEdge, bool, true)


struct LanParametersTable
{
	typedef TL::MkTlst<
		PacketSize
		, Gain0
		, SyncLevel
		, SyncGain
		, StartDelay
		, Frequency
		, NumberPackets
		, SyncInput
		, MeasurementInput
		, SynchronizationEdge
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"LanParameters";}
};
#undef DEFINE_PARAM