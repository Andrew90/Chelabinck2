#pragma once
#include <windows.h>
#include "FFT.h"
#include "AppBase.h"
#include "PrimaryData.h"
#include "MedianFiltre.h"
#include "List.hpp"
class Compute
{
public:
	static const int FirstClassPipe       = 1;
	static const int TubeOfTheSecondClass = 2;
	static const int DefectivePipe        = 3;
	template<class , class >struct __init_offset__;
	template<class , class >struct __init_energy__;
	struct Point{unsigned start, stop;};
	List<Point> list;
	static const int countTreshold = 2;
	PrimaryData &primaryData;
	HANDLE hThread[countTreshold];
    double deltaTime;
	unsigned deltaTimePaintMarker;
	//double deltaOffset;
	unsigned lastOffset;
	unsigned lastZone; //
	unsigned lastFrame;
	const unsigned &referenceOffset1;
	const unsigned &referenceOffset2;
	const unsigned &referenceOffset3;
	const unsigned &referenceOffset4;
	const unsigned referenceOffsetSQ3;
	const int &frequencyFrames;
	const int &frameSize;
	HANDLE hEvent;
	CRITICAL_SECTION cs0, cs1;
	CFFT fft;
	ThicknessData thicknessData;
	ThicknessData sensorsData[count_sensors];
	unsigned minOffset[count_sensors];
	unsigned maxOffset[count_sensors];
	double minEnergy[count_sensors];
	double maxEnergy[count_sensors];
public:
	double peak[count_sensors];
public:
	double coefficientA[count_sensors];
	double coefficientB[count_sensors];
public:
	double &nominalThickness;
	double &nominalPercentMin;
	double &nominalPercentMax;
	double &defectDifferentWallPercent;
	int &deadAreaMM0;
	int &deadAreaMM1;
	//int &countZones;
	double offsetOfZonesOffset;
	//double (&offsetOfTime)[400];
	MedianFiltre filtre[count_sensors];
	const int &medianFiltreLength;
	const int &acfBorderLeft;
	const int &acfBorderRight;
	int stop;
	int cuttingZone0;
	int cuttingZone1;
	//int deadAreaTime;
	//int usefulPiecePipe;
	char usefulPiecePipeOk;
	double defectBorderMin;
	double defectBorderMax;
	double defectDifferentWall;
	static void ThreadProc(Compute *);	
public:
	
	Compute(PrimaryData &);
	unsigned deltaTimeZoneStart;
	unsigned deltaTimeZoneStop1;
	unsigned deltaTimeZoneStop2;
	unsigned stopMeshuring;
	void MeshuringBaseStart();
	void EndAdjustmentsTube();

	void InitParam();

	void MeshuringBaseStop1();
	void MeshuringBaseStop2();

	void Calculation();
	void CalculationOneFrame(int sensor, char *data, double &result, char &status);
	void Stop();
	void Destroy();

	void Recalculation();
	void UsefulPiecePipeDo();
	void StopMeshuring();
};

//extern Compute compute;