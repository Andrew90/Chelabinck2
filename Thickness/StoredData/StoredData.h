#pragma once
#include <atlcomtime.h>
//���������� ���������� � ���� ������
class Stored
{
public:
	void Do();
protected:
	bool Save(wchar_t *); //��������� � ����� ����.mdb(��-���, ��-�����)	
};
int CountTubesInPacket();