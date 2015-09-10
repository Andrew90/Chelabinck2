#include "stdafx.h"
#include "Controller.h"
#include "InOutBits.h"
#include "AppBase.h"
#include <MMSystem.h>
#include "ArchiveEvents.h"
#include "DebugMess.h"
#include "Config.h"
#ifdef XDEBUG
#define dprint debug.print
#else	
#define  dprint
#endif	
//-----------------------------------------------------------------------------------------
Controller::Controller()
	: SQ1(Singleton<InputsTable>::Instance().items.get<PipeAfterClamp1SQ1BitIn>().value)
	, SQ2(Singleton<InputsTable>::Instance().items.get<Clamp1RetractedSQ2BitIn>().value)
	, SQ3(Singleton<InputsTable>::Instance().items.get<PipeInletSQ3BitIn>().value)
	, SQ4(Singleton<InputsTable>::Instance().items.get<OutletPipeSQ4BitIn>().value)
	, SQ5(Singleton<InputsTable>::Instance().items.get<SensorUnitRetractedSQ5BitIn>().value)
	, SQ7(Singleton<InputsTable>::Instance().items.get<PipeClamp2BeforeSQ7BitIn>().value)
	, SQ8(Singleton<InputsTable>::Instance().items.get<Clamp2RetractedSQ8BitIn>().value)
	, SQ9(Singleton<InputsTable>::Instance().items.get<PipeOnMarkerSQ9BitIn>().value)
	, Pa(Singleton<InputsTable>::Instance().items.get<NormalAirPressurePaBitIn>().value)
	, goAheadBitOut       (Singleton<OutputsTable>::Instance().items.get<GoAheadBitOut>().value)
	, goBackBitOut		  (Singleton<OutputsTable>::Instance().items.get<GoBackBitOut>().value)
	, clamp1SupplyY6BitOut(Singleton<OutputsTable>::Instance().items.get<Clamp1SupplyY6BitOut>().value)
	, clamp2SupplyY7BitOut(Singleton<OutputsTable>::Instance().items.get<Clamp2SupplyY7BitOut>().value)
	, sensorsSupplyY4BitOut (Singleton<OutputsTable>::Instance().items.get<SensorsSupplyY4BitOut>().value)	
	, sensorsRemovalY3BitOut(Singleton<OutputsTable>::Instance().items.get<SensorsRemovalY3BitOut>().value)
	, supplySensorDelay(Singleton<AdditionalSettingsTable>::Instance().items.get<SupplySensorDelay>().value)
	, removeSensorDelay(Singleton<AdditionalSettingsTable>::Instance().items.get<RemoveSensorDelay>().value)
	, delayClamp(0)
	, delayBOS(0)
	, bosOn(false)
	//, bosOff(false)
	, input(archiveEvents.map->input)
	, output(archiveEvents.map->output)
{
}
//------------------------------------------------------------------------------------
//bool clamp1_tmp = false;
//bool clamp2_tmp = false;
//unsigned result_tmp = 0;
void Controller::Do()//смотри тех. задание
{
	bool tubeAbsent = ((SQ2|SQ8) == (input &(SQ1|SQ2|SQ3|SQ4|SQ7|SQ8)));//отсутствие трубы
	bool offPosition = 0 != (input &SQ5);// СУ в нерабочем состоянии
	bool pressure = 0 != (input &Pa); //давление воздуха

	if(!pressure)
	{
			return;//нет давления воздуха
	}

	bool goAhead = //разрешение движения вперёд
		workMode 
		|| (setupMode && exitPipe)
		|| tubeAbsent
		|| offPosition
		;

	bool goBack = //разрешение движения назад
		tubeAbsent
		|| offPosition
		;

	bool clamp1 = //Прижима 1 подвод
		(workMode && ((SQ1|SQ3) == (input&(SQ1|SQ3))))//|SQ9))))
		;
	
	unsigned const msk_clamp2 = SQ4|SQ7;
	bool clamp2 = //Прижима 2 подвод
		(workMode && (msk_clamp2 == (input & msk_clamp2)))
		;	
//---------------------------------------------------------------
	if(goAhead) output |= Singleton<OutputsTable>::Instance().items.get<GoAheadBitOut>().value;
	else output &= ~Singleton<OutputsTable>::Instance().items.get<GoAheadBitOut>().value;

	if(goBack)  output |= Singleton<OutputsTable>::Instance().items.get<GoBackBitOut>().value;
	else output &= ~Singleton<OutputsTable>::Instance().items.get<GoBackBitOut>().value;

	if(clamp1)  output |= Singleton<OutputsTable>::Instance().items.get<Clamp1SupplyY6BitOut>().value;
	else output &= ~Singleton<OutputsTable>::Instance().items.get<Clamp1SupplyY6BitOut>().value;

	if(clamp2)  output |= Singleton<OutputsTable>::Instance().items.get<Clamp2SupplyY7BitOut>().value;
	else output &= ~Singleton<OutputsTable>::Instance().items.get<Clamp2SupplyY7BitOut>().value;
}
//-------------------------------------------------------------------------------------
