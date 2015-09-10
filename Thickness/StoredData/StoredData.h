#pragma once
#include <atlcomtime.h>
//сохранение результата в базе данных
class Stored
{
public:
	void Do();
protected:
	bool Save(wchar_t *); //сохранить в файле ГГММ.mdb(ГГ-год, ММ-месяц)	
};
int CountTubesInPacket();