#include "stdafx.h"
#include "AppBase.h"
#include "ArchiveEvents.h"
#include "..\TypeSizeBase\ParametersCycle.h"

#include "DebugMess.h"
#ifdef XDEBUG
#define dprint debug.print
#else	
#define  dprint
#endif	
//-------------------------------------------------------------------
template<class O, class P>struct app_base_init
{
	void operator()(O *, P *p)
	{
	   Select<O>(*p).ID(1).Execute(Singleton<O>::Instance());
	   dprint(__FUNCTION__);
	}
};

template<class O, class P>struct multiply_base_init
{
	void operator()(O *, P *)
	{
	}
};

template<class X, class P>struct multiply_base_init<ID<X>, P>
{
	typedef ID<X> O;
	void operator()(O *, P *p)
	{
		 Select<X>(p->base).ID(p->param.items.get<O>().value).Execute(Singleton<X>::Instance());
		 dprint("%s\n", __FUNCTION__);
	}
};
//---------------------------------------------------------------------------------------------
void AppBase::Init()
{
    ParametersBase parameters;
	CBase base(
		parameters.name()
		, CreateDataBase<ParametersBase::type_list, SetDefault<ParametersBase::type_list> >()
		, parameters.tables
		);

  if(base.IsOpen())
  {
	  TL::foreach<ParametersBase::one_row_table_list, app_base_init>()((TL::Factory<ParametersBase::type_list> *)0, &base);
  }
  //ParametesCycle::Init();
}
//------------------------------------------------------------------------
const wchar_t *ParametersBase::name()
{
	GetModuleFileName(0, path, dimention_of(path));
	PathRemoveFileSpec(path);
	wcscat(path, L"\\EventGrid");
	return path;
}
//-------------------------------------------------------------------------	
const double &result_undefined = Singleton<ThresholdsTable>::Instance().items.get<NominalThickness>().value;





