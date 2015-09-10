#pragma once
#include "AppBase.h"
class ThicknessData
{
public:
	ThicknessData();
	static const int MAX_ZONES_COUNT = 400;
	int countZones;
	double zonesMin[MAX_ZONES_COUNT];
	double zonesMax[MAX_ZONES_COUNT];
	char   status[MAX_ZONES_COUNT];
};

//extern ThicknessData thicknessData;
//extern ThicknessData sensorsData[count_sensors];