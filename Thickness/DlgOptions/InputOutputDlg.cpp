#include "stdafx.h"
#include "ParamDlg.h"
#include "ParamDlg.hpp"
#include "Dialogs.h"
#include "Messages.h"

template<class O, class P>struct __set__
{
	typedef typename TL::Inner<O>::Result T;
	void operator()(O *o, P *p)
	{
		wchar_t buf[128];
		GetWindowText(o->hWnd, buf, dimention_of(buf));
		o->value.value = p->Set<O>(Wchar_to<typename T::type_value>()(buf));
	}
};
template<class List>struct __data__
{
	typedef List list;
	unsigned buf[TL::Length<List>::value];
	template<class T>unsigned Set(unsigned val)
	{
		buf[TL::IndexOf<list, T>::value] = val;
		return 1 << (val - 1);
	}
};
template<class O, class P>struct __compress_bits__
{
	void operator()(O *o, P *p)
	{
		unsigned t = p->get<O>().value;
		unsigned i = 1;
		while(t >>= 1) ++i;
		o->value = i;
	}
};
struct putsDlg_OkBtn
{
	static const int width = 120;
	static const int height = 30;
	static const int ID = IDOK;
	wchar_t *Title(){return L"Применить";}
	template<class Owner>void BtnHandler(Owner &owner, HWND h)
	{
		if(TestPassword<Owner::Table>()(h))
		{
			if(!TL::find<Owner::list, __test__>()(&owner.items, &h))return;
			__data__<Owner::list> d;
			TL::foreach<Owner::list, __set__>()(&owner.items, &d);
			for(int i = 0; i < dimention_of(d.buf) - 1; ++i)
			{
				unsigned value = d.buf[i];
				for(int j = i + 1; j < dimention_of(d.buf); ++j)
				{
					if(value == d.buf[j])
					{
						MessageBox(h, L"Несколько данных ссылаются на один бит", L"Ошибка!!!", MB_ICONERROR);
						return;
					}
				}
			}
			CBase base(ParametersBase().name());
			if(base.IsOpen())
			{
				CopyFactory::Do(Singleton<Owner::Table>::Instance().items, owner.table.items);
				UpdateWhere<Owner::Table>(owner.table, base).ID(1).Execute();
			}
			EndDialog(h, TRUE);
		}
	}
};


struct DefaultBtn
{
	static const int width = 160;
	static const int height = 30;
	static const int ID = IDRETRY;
	wchar_t *Title(){return L"Встроенные настройки";}
	template<class Owner>void BtnHandler(Owner &o, HWND h)
	{
		if(TestPassword<Owner::Table>()(h))
		{
			CBase base(ParametersBase().name());
			if(base.IsOpen())
			{
				Owner::Table t;
				CopyFactory::Do(Singleton<Owner::Table>::Instance().items, t.items);
				UpdateWhere<Owner::Table>(t, base).ID(1).Execute();
			}
			EndDialog(h, TRUE);
		}
	}
};
//----------------------------------------------------------------------------
//#define CHECK(n) MIN_VALUE(n, 0) MAX_VALUE(n, 33) PARAM_TITLE(n, (wchar_t *)Mess<n>::Text())
#define CHECK(n) MIN_VALUE(n, 0) MAX_VALUE(n, 33) PARAM_TITLE(n, (wchar_t *)MessageField<n>::Text())
CHECK(PipeAfterClamp1SQ1BitIn    )
CHECK(Clamp1RetractedSQ2BitIn    )
CHECK(PipeInletSQ3BitIn          )
CHECK(OutletPipeSQ4BitIn         )
CHECK(SensorUnitRetractedSQ5BitIn)
CHECK(SensorsDivorcedSQ6BitIn      )
CHECK(PipeClamp2BeforeSQ7BitIn   )
CHECK(Clamp2RetractedSQ8BitIn    )
CHECK(PipeOnMarkerSQ9BitIn       )
CHECK(NormalAirPressurePaBitIn   )
CHECK(BlockErrorBitIn            )
//----------------------------------------------------------------------------------
void InputsDlg::Do(HWND h)
{
	InputsTable t;
	TL::foreach<InputsTable::items_list, __compress_bits__>()(&t.items, &Singleton<InputsTable>::Instance().items);
	if(TemplDialog<InputsTable, TL::MkTlst<putsDlg_OkBtn, CancelBtn, DefaultBtn>::Result>(t).Do(h, L"Смещения бит входного порта"))
	{
	}
}
//-----------------------------------------------------------------------------
CHECK(SensorUnitRemovalY1BitOut)
CHECK(SensorUnitSupplyY2BitOut )
CHECK(SensorsRemovalY3BitOut   )
CHECK(SensorsSupplyY4BitOut    )
CHECK(Clamp1SupplyY6BitOut     )
CHECK(Clamp2SupplyY7BitOut     )
CHECK(StartBitOut              )
CHECK(PipeMarkerY5BitOut       )
CHECK(PowerBitOut			   )
CHECK(GoAheadBitOut			   )
CHECK(GoBackBitOut			   )
//-----------------------------------------------------------------------------
void OutputsDlg::Do(HWND h)
{
	OutputsTable t;
	TL::foreach<OutputsTable::items_list, __compress_bits__>()(&t.items, &Singleton<OutputsTable>::Instance().items);
	if(TemplDialog<OutputsTable, TL::MkTlst<putsDlg_OkBtn, CancelBtn, DefaultBtn>::Result>(t).Do(h, L"Смещения бит выходного порта"))
	{
	}
}
//----------------------------------------------------------------------------