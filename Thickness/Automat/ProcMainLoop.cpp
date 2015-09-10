#include "stdafx.h"
#include "ProcMainLoop.h"
#include "Device1730.h"
#include "Automat.h"
#include "PaintMaker.h"
#include "InOutBits.h"
#include "AppBase.h"
#include "Controller.h"
#include "SQEvents.h"
#include "OutBitDown.h"
#include "Controller.h"
#include "Messages.h"

#include "DebugMess.h"
#ifdef XDEBUG
#define dprint debug.print
#else
#define dprint
#endif
//--------------------------------------------------------------------------------------------------
unsigned const &SQ6 = Singleton<InputsTable>::Instance().items.get<SensorsDivorcedSQ6BitIn>().value;
//--------------------------------------------------------------------------------------------------
void NoopProc(unsigned &input, unsigned &output)
{
	/*
	controller.workMode = false;
	Sleep(100);
	input = device1730.Read();
	controller.Do();
	outBitDown.Do();	
	device1730.Write();//output);
	*/
	controller.workMode = false;
	input = device1730.Read();	
//	sQEvents.Do();
//	automat.Do(input);
	//controller.Do();
	//paintMaker.Do();
	//outBitDown.Do();	
	//device1730.Write();
	output = device1730.ReadOutput();
	EventLogInputPort();
	EventLogOutputPort();
	Sleep(100);
}
//--------------------------------------------------------------------------------------------------
#ifdef XDEBUG
unsigned lastOutput;
#endif
void CollectDataProc(unsigned &input, unsigned &output)
{
	controller.workMode = true;
	input = device1730.Read();
	sQEvents.Do();
//	automat.Do(input);
	controller.Do();
	paintMaker.Do();
	outBitDown.Do();	
	device1730.Write();
	EventLogInputPort();
	EventLogOutputPort();
#ifdef XDEBUG
	if(lastOutput != output)
	{
		lastOutput = output;
		dprint("out %x", output);
	}
#endif
}
void TestDataProc(unsigned &input, unsigned &output)
{
	controller.workMode = false;
	input = device1730.Read();
//	sQEvents.Do();
//	automat.Do(input);
	controller.Do();
//	paintMaker.Do(output);
	outBitDown.Do();	
	device1730.Write();//output);
	EventLogInputPort();
	EventLogOutputPort();
#ifdef XDEBUG
	if(lastOutput != output)
	{
		lastOutput = output;
		dprint("out %x", output);
	}
#endif
}
//--------------------------------------------------------------------------------------------------
void RemovePipeProc(unsigned &input, unsigned &output)
{
	/*
	input = device1730.Read();
	ñontroller.setupMode = true;
	ñontroller.workMode = false;
	ñontroller.Do(input, output);
	if(0 == (input & SQ6))
	{		
		output |= Singleton<OutputsTable>::Instance().items.get<SensorsRemovalY3BitOut>().value;
		output &= ~Singleton<OutputsTable>::Instance().items.get<SensorsSupplyY4BitOut>().value;		
	}
	automat.outBitDown.Do();
	device1730.Write(output);
#ifdef XDEBUG
	if(lastOutput != output)
	{
		lastOutput = output;
		dprint("out %x", output);
	}
#endif
	*/
	dprint(__FUNCTION__);
	Sleep(100);
}
//--------------------------------------------------------------------------------------------------