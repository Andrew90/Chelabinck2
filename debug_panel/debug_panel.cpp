// debug_panel.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DebugMess.h"
#include <clocale>


int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_CTYPE, "");
	printf("консоль для вывода сообщений");
	ViewerDebugMess m;
	while(true)
	{
		char *c = m.get();
		if(NULL != c)printf("%s\n", c);
		Sleep(10);
	}
	getchar();
	return 0;
}