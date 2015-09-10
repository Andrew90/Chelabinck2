#pragma once
class Controller
{
	unsigned delayClamp;
	bool boolClamp;
	bool bosOn;
	//bool bosOff;
	unsigned delayBOS;
	unsigned &input, &output;
public:
	bool setupMode;
	bool workMode;
	bool exitPipe;
	bool supplyClamp1;
	bool supplyClamp2;
	const unsigned &SQ1;
	const unsigned &SQ2;
	const unsigned &SQ3;
	const unsigned &SQ4;
	const unsigned &SQ5;
	const unsigned &SQ7;
	const unsigned &SQ8;
	const unsigned &SQ9;
	const unsigned &Pa;
	const unsigned &goAheadBitOut;       
	const unsigned &goBackBitOut;		  
	const unsigned &clamp1SupplyY6BitOut;
	const unsigned &clamp2SupplyY7BitOut;
	const unsigned &sensorsSupplyY4BitOut; 
	const unsigned &sensorsRemovalY3BitOut;
    const int &supplySensorDelay;
    const int &removeSensorDelay;
public:
	Controller();
	void Do();
//	void BosOn();
//	void BosOff();
	//void BosPowerOn();
	//void BosStartBitOff();
};

extern Controller controller;