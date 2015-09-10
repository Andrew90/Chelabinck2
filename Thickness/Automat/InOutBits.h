#pragma once
#include "typelist.hpp"
//E:\projects\tube_solution\templates\typelist.hpp
//E:\projects\tube_solution\Thickness\Automat\InOutBits.h
/*
#define IN_BITS(n, def) struct n##BitIn\
{\
	static const unsigned default_value = def;\
	unsigned value;\
	n##BitIn() : value(def) {}\
};
*/
#define IN_BITS(z, def_val) struct z##BitIn\
{\
	typedef unsigned type_value;\
	type_value value;\
	const type_value default_value;\
	const wchar_t *name(){return L#z;}\
	z##BitIn() : value(def_val), default_value(def_val) {}\
};

IN_BITS(PipeAfterClamp1SQ1,         1 << 0)
IN_BITS(Clamp1RetractedSQ2,         1 << 1)
IN_BITS(PipeInletSQ3,               1 << 3)
IN_BITS(OutletPipeSQ4,              1 << 2)
IN_BITS(SensorUnitRetractedSQ5,     1 << 4)
IN_BITS(SensorsDivorcedSQ6,           1 << 5)
IN_BITS(PipeClamp2BeforeSQ7,        1 << 6)
IN_BITS(Clamp2RetractedSQ8,         1 << 7)
IN_BITS(PipeOnMarkerSQ9,            1 << 8)
IN_BITS(NormalAirPressurePa,       1 << 9)
IN_BITS(BlockError,              1 << 10)
IN_BITS(Reserve1,                1 << 11)
IN_BITS(Reserve2,                1 << 12)
IN_BITS(Reserve3,                1 << 13)
IN_BITS(Reserve4,                1 << 14)
IN_BITS(Reserve5,                1 << 15)

typedef TL::MkTlst<
	PipeAfterClamp1SQ1BitIn    
	, Clamp1RetractedSQ2BitIn
	, PipeInletSQ3BitIn         
	, OutletPipeSQ4BitIn
	, SensorUnitRetractedSQ5BitIn
	, SensorsDivorcedSQ6BitIn     
	, PipeClamp2BeforeSQ7BitIn
	, Clamp2RetractedSQ8BitIn         
	, PipeOnMarkerSQ9BitIn          
	, NormalAirPressurePaBitIn
	, BlockErrorBitIn 
	//, Reserve1BitIn               
	//, Reserve2BitIn               
	//, Reserve3BitIn                
	//, Reserve4BitIn               
	//, Reserve5BitIn  
>::Result input_bits_list;
/*
#define OUT_BITS(n, def) struct n##BitOut\
{\
	static const unsigned default_value = def;\
	unsigned value;\
	n##BitOut() : value(def) {}\
};
*/
#define OUT_BITS(z, def_val) struct z##BitOut\
{\
	typedef unsigned type_value;\
	type_value value;\
	const type_value default_value;\
	const wchar_t *name(){return L#z;}\
	z##BitOut() : value(def_val), default_value(def_val) {}\
};

OUT_BITS(SensorUnitRemovalY1, 1 << 2)
OUT_BITS(SensorUnitSupplyY2, 1 << 3)
OUT_BITS(SensorsRemovalY3, 1 << 0)
OUT_BITS(SensorsSupplyY4, 1 << 1)
OUT_BITS(Clamp1SupplyY6, 1 << 5)
OUT_BITS(Clamp2SupplyY7, 1 << 4)
OUT_BITS(Start, 1 << 6)
OUT_BITS(PipeMarkerY5, 1 << 7)
OUT_BITS(Power, 1 << 8)
OUT_BITS(Reserve1, 1 << 9)
OUT_BITS(Reserve2, 1 << 10)
OUT_BITS(GoAhead, 1 << 11)
OUT_BITS(Reserve3, 1 << 12)
OUT_BITS(GoBack, 1 << 13)
OUT_BITS(Reserve4, 1 << 14)
OUT_BITS(Reserve5, 1 << 15)

typedef TL::MkTlst<
  SensorUnitRemovalY1BitOut
  , SensorUnitSupplyY2BitOut
  , SensorsRemovalY3BitOut
  , SensorsSupplyY4BitOut
  , Clamp1SupplyY6BitOut
  , Clamp2SupplyY7BitOut
  , StartBitOut
  , PipeMarkerY5BitOut
  , PowerBitOut
  //, Reserve1BitOut
  //, Reserve2BitOut
  , GoAheadBitOut
//  , Reserve3BitOut
  , GoBackBitOut
  //, Reserve4BitOut
  //, Reserve5BitOut
>::Result output_bits_list;

//extern TL::Factory<input_bits_list> input_bits;
//extern TL::Factory<output_bits_list> output_bits;

