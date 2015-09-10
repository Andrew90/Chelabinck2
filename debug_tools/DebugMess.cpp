#include "stdafx.h"
#include "DebugMess.h"
#ifdef XDEBUG
#include <stdio.h>
#define d_mess debug.print
wchar_t name[] = L"Parallel hatch in space";
CRITICAL_SECTION critical;
struct InitCriticalSectionDebug
{
	InitCriticalSectionDebug(){InitializeCriticalSection(&critical);}
	~InitCriticalSectionDebug(){EnterCriticalSection(&critical);(&critical);}
} initCriticalSection_Debug;

struct LockDebug
{
	LockDebug(){EnterCriticalSection(&critical);}
	~LockDebug(){LeaveCriticalSection(&critical);}
};
void DebugMess::Init()
{
	hMapFile = CreateFileMapping(
                 INVALID_HANDLE_VALUE,    // use paging file
                 NULL,                    // default security 
                 PAGE_READWRITE,          // read/write access
                 0,                       // max. object size 
                 sizeof(TMapData),                // buffer size  
                 name);                 // name of mapping object
 
   if (hMapFile == NULL || hMapFile == INVALID_HANDLE_VALUE) 
   { 
      d_mess("Could not create file mapping object (%d).\n", 
             GetLastError());
      return;
   }
   map = (TMapData *) MapViewOfFile(hMapFile,   // handle to map object
                        FILE_MAP_ALL_ACCESS, // read/write permission
                        0,                   
                        0,                   
                        sizeof(TMapData));           
 
   if (map == NULL) 
   { 
      d_mess("Could not map view of file (%d).\n", 
             GetLastError()); 
      return;
   }
   map->head = 0;
   map->tail = 0;
}

DebugMess::~DebugMess(void)
{
   UnmapViewOfFile(map);
   CloseHandle(hMapFile);
}
#pragma warning(disable : 4996)
void DebugMess::print(char *c, ...)
{
	LockDebug lock;
	if(NULL != map)
	{
		char *b = map->data[map->head];
		vsprintf(b, c, (char *)&c + 4);
		size_t len = strlen(b);
		b[len] = '\0';
		++map->head;
	}
}
//---------------------------------------------------------------------------------------
ViewerDebugMess::ViewerDebugMess() : map(NULL)
{	
	hMapFile = CreateFileMapping(
                 INVALID_HANDLE_VALUE,    // use paging file
                 NULL,                    // default security 
                 PAGE_READWRITE,          // read/write access
                 0,                       // max. object size 
                 sizeof(TMapData),                // buffer size  
                 name);                 // name of mapping object
	if (hMapFile == NULL) 
   { 
      d_mess("Could not open file mapping object (%d).\n", 
             GetLastError());
      return;
   } 
 
   map = (TMapData *)MapViewOfFile(hMapFile,    // handle to mapping object
               FILE_MAP_ALL_ACCESS,  // read/write permission
               0,                    
               0,                    
               sizeof(TMapData));                   
 
   if (map == NULL) 
   { 
      d_mess("Could not map view of file (%d).\n", 
             GetLastError()); 
      return;
   }
}
//----------------------------------------------------------------------------
ViewerDebugMess::~ViewerDebugMess()
{
   UnmapViewOfFile(map);
   CloseHandle(hMapFile);
}
//----------------------------------------------------------------------------
char *ViewerDebugMess::get()
{
	if(map && map->tail != map->head)
	{
		static char b[512];		
        CharToOemA(map->data[map->tail], b);
		++map->tail;
		return b;
	}
	return NULL;
}
//-----------------------------------------------------------------------------
DebugMess debug;
#endif
