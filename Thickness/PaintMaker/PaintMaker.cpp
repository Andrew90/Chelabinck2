#include "stdafx.h"
#include "PaintMaker.h"
#include "PrimaryData.h"
#include "ThicknessData.h"
#include "InOutBits.h"
#include "AppBase.h"
#include <Mmsystem.h>
#include "Compute.h"
#include "ArchiveEvents.h"
#include "Device1730.h"
#include "DebugMess.h"
#ifdef XDEBUG
#define dprint debug.print
#else	
#define  dprint
#endif
#if 1
//------------------------------------------------------------------------
PaintMaker::PaintMaker()
	: outputBit(Singleton<OutputsTable>::Instance().items.get<PipeMarkerY5BitOut>().value)
	, output(archiveEvents.map->output)
	, sprayTime(Singleton<PaintMakerTable>::Instance().items.get<SprayTime>().value)
	, correctionTick(Singleton<PaintMakerTable>::Instance().items.get<CorrectionOffsetTick>().value)
	, startTime(0)
	, bitDown(0)
	, status(NULL)
{}
//------------------------------------------------------------------------
void PaintMaker::Do()
{
	if(startTime)
	{
		unsigned t = timeGetTime();
		if(currentTime < t)
		{
			char st = status[currentOffset];
			if(
				PrimaryData::DefectMin == st
				|| PrimaryData::DefectMax == st
				|| PrimaryData::DefectMinMax == st
				|| PrimaryData::DefectDifferentWall == st
				)
			{
				output |= outputBit;
				bitDown = t + sprayTime;
			}
			++currentOffset;
			currentTime = (unsigned)offsetTime[currentOffset] + startTime + correctionTick;
		}
		else if(bitDown && bitDown < t)
		{
			output &= ~outputBit;
			bitDown = 0;
		}
		else if(0 == status[currentOffset])
		{
			output &= ~outputBit;
		}
	}
	if(NULL != status && 0 == status[currentOffset])
	{
		output &= ~outputBit;
		bitDown = 0;
	}
}
//-------------------------------------------------------------------------
void PaintMaker::Start(unsigned delay, double *offs, char *stat)
{
	status = stat;
	offsetTime = offs;
	currentOffset = 0;
	currentTime = (unsigned)offsetTime[0] + delay + correctionTick;
	startTime = delay;
}
void PaintMaker::Stop()
{
	startTime = 0;
	bitDown = 0;
	unsigned &output = archiveEvents.map->output;
	output &= ~outputBit;
	device1730.Write(output);
}
#else
//------------------------------------------------------------------------
PaintMaker::PaintMaker()
	: outputBit(Singleton<OutputsTable>::Instance().items.get<PipeMarkerY5BitOut>().value)
	, output(archiveEvents.map->output)
	, sprayTime(Singleton<PaintMakerTable>::Instance().items.get<SprayTime>().value)
	, correctionTick(Singleton<PaintMakerTable>::Instance().items.get<CorrectionOffsetTick>().value)
	, startTime(0)
	, bitDown(0)
	, status(NULL)
{}
//------------------------------------------------------------------------
void PaintMaker::Do()
{
	if(startTime)
	{
		unsigned t = timeGetTime();
		if(currentTime < t)
		{
			if(
				PrimaryData::DefectMin == status[currentOffset]
			|| PrimaryData::DefectMax == status[currentOffset]
			)
			{
				output |= outputBit;
				bitDown = t + sprayTime;
			}
			++currentOffset;
			currentTime__ += offsetTime;
			currentTime = currentTime__ + startTime + correctionTick;
		}
		else if(bitDown && bitDown < t)
		{
			output &= ~outputBit;
			bitDown = 0;
		}
		else if(0 == status[currentOffset])
		{
			output &= ~outputBit;
		}
	}
	if(NULL != status && 0 == status[currentOffset])
	{
		output &= ~outputBit;
		bitDown = 0;
	}
}
//-------------------------------------------------------------------------
void PaintMaker::Start(unsigned delay, char *stat)//, int *count_)
{
	status = stat;
	currentOffset = 0;
	currentTime__ = offsetTime;
	currentTime = currentTime__ + delay + correctionTick;
	startTime = delay;
}
void PaintMaker::Stop()
{
	startTime = 0;
	bitDown = 0;
	unsigned &output = archiveEvents.map->output;
	output &= ~outputBit;
	device1730.Write(output);
}
#endif
PaintMaker paintMaker;