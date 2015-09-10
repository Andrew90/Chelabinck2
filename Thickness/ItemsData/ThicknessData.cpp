#include "stdafx.h"
#include <math.h>
#include "ThicknessData.h"
#include "AppBase.h"
//#include "PrimaryData.h"
//---------------------------------------------------------------------------------------------------------
ThicknessData::ThicknessData()
	: countZones(0)
{
	/*
	countZones = 55;
	double dx = 2 * 3.14 / MAX_ZONES_COUNT * 5;
	for(int i = 0; i < countZones; ++ i)
	{
		zones[i] = i % 10;
	}
	*/
}
//-------------------------------------------------------------------------------------------------------
/*
#if 0
void ThicknessData::Clear()
{
	tresholdClass1  = -1;
	tresholdClass2  = -1;
	countZones = 0;
	for(int i = 0; i < MAX_ZONES_COUNT; ++ i)
	{
		for(int j = 0; j < COUNT_SENSORS; ++j) zones[j][i] = -1;
	}
}
#else
#pragma message("тестовый ThicknessData::Clear()")
void ThicknessData::Clear()
{
	countZones = 0;
	zones[0] = 0;
}
#endif
*/
//----------------------------------------------------------------------------------------------------------
//ThicknessData thicknessData;
//ThicknessData sensorsData[count_sensors];
