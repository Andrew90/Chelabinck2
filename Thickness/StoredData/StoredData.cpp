#include "stdafx.h"
#include "StoredData.h"
#include "AppBase.h"
#include "TypeSizeBase.h"
#include "CycleData.hpp"
#include "Automat.h"
#include "DialogBeforeCycle.h"
//сохранение в базе результата контроля
struct ZonesMin
{
	typedef double type_value[count_zones];
	type_value value;	
	const wchar_t *name(){return L"ZonesMin";}
};
struct ZonesMax
{
	typedef double type_value[count_zones];
	type_value value;	
	const wchar_t *name(){return L"ZonesMax";}
};
struct Status
{
	typedef char type_value[count_zones];
	type_value value;	
	const wchar_t *name(){return L"Status";}
};

DEFINE_PARAM(CountZones, int, 0)
DEFINE_PARAM(CurrentDateTime, COleDateTime, 0.0)

DEFINE_PARAM_ID(OperatorsTable, int, 1)
DEFINE_PARAM_ID(TypeSizeTable, int, 1)
struct PacketTable;
DEFINE_PARAM_ID(PacketTable, int, 1)

struct PacketTable
{
	typedef TL::MkTlst<
		HeatNumber
		, PacketNumber
		, CountTubes
		, ID<OperatorsTable>
		, ID<TypeSizeTable>
		, WorkShift
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"PacketTable";}
};

struct TubesTable
{
	typedef TL::MkTlst<
		ID<PacketTable>
		, CountZones
		, CurrentDateTime
		, ZonesMin
		, ZonesMax
		, Status
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"TubesTable";}
};

struct DataBase
{
	typedef TL::MkTlst<
		NormativeTechnicalDocumentTable
		, SteelGradeTable
		, TypeSizeTable
		, OperatorsTable
		, PacketTable
		, TubesTable
	>::Result type_list;
	typedef TL::Factory<type_list> TTables;
	TTables tables;
};
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
// Singleton<Cycle>::Instance().items.get<_sycle_<WorkShift>>().value.buffer;

//--------------------------------------------------------------------------------------------
template<class T, class Dst>void __set_val__(Dst &dst)
{
	dst.items.get<T>().value = Singleton<Cycle>::Instance().items.get<_sycle_<T>>().value;
}
template<class O, class P>struct __set_items__
{
	void operator()(O *o, P *p)
	{
		o->value = Singleton<Cycle>::Instance().items.get<_sycle_<O> >().value;
	}
};
template<class T, class List, class Dst>void __set_ID__(Dst &dst, CBase &base)
{
	TL::Factory<List> items;
	TL::foreach<List, __set_items__>()(&items, &Singleton<Cycle>::Instance().items);
	if(0 == (dst.items.get<ID<T> >().value = Select<T>(base).eq_all<List>(&items).Execute()))
	{
		T table;
		TL::foreach<typename T::items_list, __set_items__>()(&table.items, &Singleton<Cycle>::Instance().items);
		Insert_Into<T>(table, base).Execute();
		dst.items.get<ID<T> >().value = Select<T>(base).eq_all<List>(&items).Execute();
	}
}
template<class T, class Dst>int __set_ID_one_item_(Dst &dst, CBase &base)
{
	T item;
	item.value = Singleton<Cycle>::Instance().items.get<_sycle_<T> >().value;
	int id = Select<Dst>(base).eq<T>(item.value).Execute();
	if(0 == id)
	{
		dst.items.get<T>().value = Singleton<Cycle>::Instance().items.get<_sycle_<T> >().value;
        Insert_Into<Dst>(dst, base).Execute();
		id = Select<Dst>(base).eq<T>(item.value).Execute();
	}
	return id;
}
//кнопка СТОП в отладочном режиме вызывает эту функцию
bool Stored::Save(wchar_t *path)
{
	DataBase parameters;
	
	CBase base(
		path
		, CreateDataBase<DataBase::type_list/*, SetDefault<DataBase::type_list>*/ >()
		, parameters.tables
		);
	if(base.IsOpen())
	{
		TypeSizeTable typeSizeTable;
		
		NormativeTechnicalDocumentTable normativeTechnicalDocumentTable;
		typeSizeTable.items.get<ID<NormativeTechnicalDocument> >().value = __set_ID_one_item_<NormativeTechnicalDocument>(normativeTechnicalDocumentTable, base);
	
		SteelGradeTable steelGradeTable;
		typeSizeTable.items.get<ID<SteelGrade> >().value = __set_ID_one_item_<SteelGrade>(steelGradeTable, base);

		__set_val__<NominalThickness	        >(typeSizeTable);
		__set_val__<TubeDiameter		   		>(typeSizeTable);
		__set_val__<NominalPercentMin			>(typeSizeTable);
		__set_val__<NominalPercentMax			>(typeSizeTable);
		__set_val__<DefectDifferentWallPercent>(typeSizeTable);

		int typeSizeTableID = Select<TypeSizeTable>(base).eq_all<TypeSizeTable::items_list>(&typeSizeTable.items).Execute();
		if(0 == typeSizeTableID)
		{
			Insert_Into<TypeSizeTable>(typeSizeTable, base).Execute();
			typeSizeTableID = Select<TypeSizeTable>(base).eq_all<TypeSizeTable::items_list>(&typeSizeTable.items).Execute();
		}
		
		PacketTable packetTable;
		__set_val__<HeatNumber>(packetTable);
		__set_val__<PacketNumber>(packetTable);
		__set_val__<CountTubes>(packetTable);
		__set_val__<WorkShift>(packetTable);

		__set_ID__<OperatorsTable, TL::MkTlst<NameOperator>::Result>(packetTable, base);
		
		packetTable.items.get<ID<TypeSizeTable> >().value = typeSizeTableID;

		int packetTableID = Select<PacketTable>(base).eq_all<PacketTable::items_list>(&packetTable.items).Execute();
		if(0 == packetTableID)
		{
			Insert_Into<PacketTable>(packetTable, base).Execute();
			packetTableID = Select<PacketTable>(base).eq_all<PacketTable::items_list>(&packetTable.items).Execute();
		}

		TubesTable tubesTable;

		tubesTable.items.get<ID<PacketTable> >().value = packetTableID;

		int size = currentAutomat->compute.thicknessData.countZones < count_zones ?  currentAutomat->compute.thicknessData.countZones : count_zones;

		tubesTable.items.get<CountZones>().value = size;
		tubesTable.items.get<CurrentDateTime>().value = COleDateTime::GetCurrentTime();
		
		memmove(tubesTable.items.get<ZonesMin>().value, currentAutomat->compute.thicknessData.zonesMin, size * sizeof(double));
		memmove(tubesTable.items.get<ZonesMax>().value, currentAutomat->compute.thicknessData.zonesMax, size * sizeof(double));
		memmove(tubesTable.items.get<Status>().value  , currentAutomat->compute.thicknessData.status  , size * sizeof(char));

	    Insert_Into<TubesTable>(tubesTable, base).Execute();

		return true;
	}
	return false;
}
//--------------------------------------------------------------------------------------------
void Stored::Do()
{
	wchar_t path[512];	
	GetModuleFileName(0, path, dimention_of(path));
	PathRemoveFileSpec(path);
	int length = wcslen(path);
	wcscpy(&path[length], L"/Stored/");
	CreateDirectory(path, NULL);
	wcscpy(&path[length], L"/Stored/Base/");
	CreateDirectory(path, NULL);

	COleDateTime time = COleDateTime::GetCurrentTime();	
	wsprintf(&path[length], L"/Stored/Base/%02d%02d", time.GetYear() - 2000, time.GetMonth());

	if(Save(path))
	{
		
	}
	else
	{
		
	}
}
//-------------------------------------------------------------------------------------------------
int CountTubesInPacket()
{
	wchar_t path[512];	
	GetModuleFileName(0, path, dimention_of(path));
	PathRemoveFileSpec(path);
	int length = wcslen(path);
	wcscpy(&path[length], L"/Stored/");
	CreateDirectory(path, NULL);
	wcscpy(&path[length], L"/Stored/Base/");
	CreateDirectory(path, NULL);

	COleDateTime time = COleDateTime::GetCurrentTime();	
	wsprintf(&path[length], L"/Stored/Base/%02d%02d", time.GetYear() - 2000, time.GetMonth());
	DataBase parameters;
	
	CBase base(
		path
		, CreateDataBase<DataBase::type_list/*, SetDefault<DataBase::type_list>*/ >()
		, parameters.tables
		);
	int i = 0;
	if(base.IsOpen())
	{
		int packetNumber = Singleton<Cycle>::Instance().items.get<_sycle_<PacketNumber>>().value;
		int packetNumberID = Select<PacketTable>(base).eq<PacketNumber>(packetNumber).Execute();
		ADODB::_RecordsetPtr rec;
		Select<TubesTable>(base).eq<ID<PacketTable> >(packetNumberID).Execute(rec);
		
		while (!rec->EndOfFile) 
		{			
			++i;
			rec->MoveNext(); 
		}
	}
	return i;
}



