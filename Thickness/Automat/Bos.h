#pragma once
class Bos
{
	int count;
	unsigned delay;
	unsigned &output;
	unsigned &input;
public:
	Bos();
	void PowerOn();
	void On();
	void Off();
	void PowerDown();
};
extern Bos bos;