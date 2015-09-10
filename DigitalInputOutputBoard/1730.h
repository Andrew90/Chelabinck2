#pragma once

class DigitInOut
{
	unsigned value;
public:
	DigitInOut();
	void Init();
	void Destroy();
	unsigned Input();
	void Output(unsigned);
	void SetBits(unsigned);
	void ClearBit(unsigned);
};

extern DigitInOut digitInOut;