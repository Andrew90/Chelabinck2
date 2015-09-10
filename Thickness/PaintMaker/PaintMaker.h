#pragma once
#if 1
class PaintMaker
{
	int currentOffset;
	unsigned currentTime;
	unsigned startTime;
	int &correctionTick;
	int &sprayTime;
	unsigned &outputBit;
	unsigned &output;
	double *offsetTime;
	char *status;
	unsigned bitDown;
public:	
	PaintMaker();
	void Do();
	void Start(unsigned delay, double *, char *);
	void Stop();
};
#else
class PaintMaker
{
	int currentOffset;
	double currentTime;
	double currentTime__;
	unsigned startTime;
	int &correctionTick;
	int &sprayTime;
	unsigned &outputBit;
	unsigned &output;
	
	char *status;
	unsigned bitDown;
public:	
	double offsetTime;
	PaintMaker();
	void Do();
	void Start(unsigned delay, char *);
	void Stop();
};
#endif
extern PaintMaker paintMaker;
