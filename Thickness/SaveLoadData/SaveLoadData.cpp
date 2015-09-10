#include "stdafx.h"
#include "SaveLoadData.h"
#include "DlgFiles.h"
#include "Automat.h"
//------------------------------------------------------------------------
struct HeadStoredData
{
	int countZones;
	int countSensors;
};

struct close_file
{
	FILE *f;
	close_file(FILE *f) : f(f){}
	~close_file(){if(NULL != f)fclose(f);}
};
#if 0
void StoredData::Do(HWND h)
{
	SaveData o(h);
	if(o())
	{
		FILE *f = _wfopen(o.sFile, L"wb+");
		close_file c_f(f);
		if(NULL != f)
		{
			HeadStoredData head;
			head.countZones = primaryData.countZones;
			head.countSensors = count_sensors;
			if(1 == fwrite(&head, sizeof(head), 1, f))
			{
				if(1 == fwrite(primaryData.offsetOfZones, sizeof(primaryData.offsetOfZones), 1, f))
				{
					char buf[1024];
					int k = 0;
					const int count = primaryData.Filling();
					for(int i = 0; i < int(count / dimention_of(buf)); ++i)
					{
						for(int j = 0; j < dimention_of(buf); ++j)
						{
							buf[j] = (char)primaryData.buffer[k++];
						}
						fwrite(buf, 1, sizeof(buf), f);
					}
					int z = 0;
					for(; k < count; ++z, ++k)
					{
						buf[z] = (char)primaryData.buffer[k];
					}

					if(z > 0) fwrite(buf, 1, z, f);
					return;
				}
			}
		}
		MessageBox(0, L"Файл не сохранён", L"Ошибка !!!", MB_ICONERROR);
	}
}
#else
void StoredData::Do(HWND h)
{
	SaveData o(h);
	if(o())
	{
		FILE *f = _wfopen(o.sFile, L"wb+");
		close_file c_f(f);
		if(NULL != f)
		{
			HeadStoredData head;
			head.countZones = currentAutomat->primaryData.countZones;
			head.countSensors = count_sensors;
			if(1 == fwrite(&head, sizeof(head), 1, f))
			{
				if(1 == fwrite(currentAutomat->primaryData.offsetOfTime, sizeof(currentAutomat->primaryData.offsetOfTime), 1, f))
				{
					char buf[1024];
					int k = 0;
					const int count = currentAutomat->primaryData.Filling();
					for(int i = 0; i < int(count / dimention_of(buf)); ++i)
					{
						for(int j = 0; j < dimention_of(buf); ++j)
						{
							buf[j] = (char)currentAutomat->primaryData.buffer[k++];
						}
						fwrite(buf, 1, sizeof(buf), f);
					}
					int z = 0;
					for(; k < count; ++z, ++k)
					{
						buf[z] = (char)currentAutomat->primaryData.buffer[k];
					}

					if(z > 0) fwrite(buf, 1, z, f);
					return;
				}
			}
		}
		MessageBox(0, L"Файл не сохранён", L"Ошибка !!!", MB_ICONERROR);
	}
}
#endif
//------------------------------------------------------------------------
#if 0
bool LoadData::Do(HWND h)
{
	OpenData o(h);
	if(o())
	{
		FILE *f = _wfopen(o.sFile, L"rb");
		close_file c_f(f);
		if(NULL != f)
		{
			HeadStoredData head;
			if(1 == fread(&head, sizeof(head), 1, f))
			{
				primaryData.countZones = head.countZones;
				int count = fread(primaryData.offsetOfZones, sizeof(primaryData.offsetOfZones[0]), dimention_of(primaryData.offsetOfZones), f);
				char buf[1024];
				//int &k = const_cast<int &>(primaryData.currentOffset);
				int k = 0;//primaryData.GetCurrentOffset();
				for(;;)
				{
					int count = fread(buf, 1, sizeof(buf), f);
					if(0 == count) break;
					{
						for(int i = 0; i < count; ++i)
						{
							primaryData.buffer[k++] = buf[i];
						}
					}
				}
				/*
				primaryData.SetCurrentOffset(k / (
					count_sensors 
					* Singleton<LanParametersTable>::Instance().items.get<NumberPackets>().value 
	                * Singleton<LanParametersTable>::Instance().items.get<PacketSize>().value
					)
					);
					*/
				primaryData.SetCurrentOffset(k);
				if(k)return true;
			}
		}
	}
	MessageBox(0, L"Файл не загружен", L"Ошибка !!!", MB_ICONERROR);
	return false;
}
#else
bool LoadData::Do(HWND h)
{
	OpenData o(h);
	if(o())
	{
		FILE *f = _wfopen(o.sFile, L"rb");
		close_file c_f(f);
		if(NULL != f)
		{
			HeadStoredData head;
			if(1 == fread(&head, sizeof(head), 1, f))
			{
				currentAutomat->primaryData.countZones = head.countZones;
				int count = fread(currentAutomat->primaryData.offsetOfTime, sizeof(currentAutomat->primaryData.offsetOfTime), 1, f);
				char buf[1024 * 16];
				int k = 0;
				for(;;)
				{
					int count = fread(buf, 1, sizeof(buf), f);
					if(0 == count) break;
					{
						for(int i = 0; i < count; ++i)
						{
							currentAutomat->primaryData.buffer[k++] = buf[i];
						}
					}
				}
				currentAutomat->primaryData.SetCurrentOffset(k);
				if(k)return true;
			}
		}
	}
	MessageBox(0, L"Файл не загружен", L"Ошибка !!!", MB_ICONERROR);
	return false;
}
#endif
//------------------------------------------------------------------------