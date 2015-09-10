#pragma once
#include "AppBase.h"
#include "ThicknessData.h"
class PrimaryData
{
	int current__;
public:
	template<int NUM>struct Mess{static const int value = NUM;};
	static const int LessMinimumEnergy = 1;
	static const int DefectDifferentWall = 2;
	static const int ValueLessThreshold = 3;
	static const int ThicknessLessThreshold = 4;
//	static const int ThicknessGreaterThreshold = 5;
	static const int Nominal = 6;
	static const int DefectMin = 7;
	static const int DefectMax = 8;
	static const int DefectMinMax = 9;
	static const int Undefined = 10;
	

	typedef TL::MkTlst<
		Mess<LessMinimumEnergy>
		, Mess<DefectDifferentWall>
		, Mess<ValueLessThreshold>
	//	, Mess<ThicknessGreaterThreshold>
		, Mess<Nominal>
		, Mess<DefectMin>		
		, Mess<DefectMax>
		, Mess<DefectMinMax>
		, Mess<Undefined>
	>::Result message_list;

	static const int buffSize = 500 * 1024 * 1024;
	static const int sensorBuffSize = buffSize / 474 / count_sensors;
	const size_t &bufSize;
	const int &frameSize;
	static char buffer[buffSize];
	double result[count_sensors][sensorBuffSize];//результат вычислений
	char status[count_sensors][sensorBuffSize];//тег вычислений
	int countZones;
	double offsetOfTime[ThicknessData::MAX_ZONES_COUNT];//временные метки
	PrimaryData();
	short *operator[](int );
	void Clear();
	char *CurrentData();
	bool IncrementOffset();
	char *SensorData(int sensor, int offset);
	int GetCurrentOffset() const;
	void SetCurrentOffset(int);
	int Filling();//заполнение буфера в short
};

