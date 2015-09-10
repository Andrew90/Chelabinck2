#pragma once
#include <Windows.h>
#include "PrimaryData.h"
#include "Compute.h"

class Automat
{
public:
	unsigned &input;
	unsigned &output;
	int num;
	PrimaryData primaryData;
	Compute compute;
public:
//	unsigned referenceOffseSQ3;
	int	&distanceClampSensor;
	HANDLE hThread;
	HANDLE hEventSingle, hEventAutomatic, hEventStop;
	static void Proc(Automat *);
public:
	enum {eNone, eAutomaticMode, eSingleMode} workMode;

	Automat();
	void Init();
	void Destroy();
	void Do();
};
extern Automat automat[2];
class Automats
{		
public:	
	void Init();
	void Destroy();
	bool StartAutomatic();
	bool StartSingle();
	void Stop();
};

extern Automats automats;
extern Automat *currentAutomat;