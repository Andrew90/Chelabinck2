#pragma once

namespace TestStateSpace
{
template<class T, bool B>struct bit
{
	typedef T type;
	unsigned offset;
	static const bool on = B;
	bit(): offset(Singleton<InputsTable>::Instance().items.get<T>().value){}
};

template<class list>struct TestState
{
	unsigned numerator, denominator;
	//int offset;
//	unsigned value;
	TL::Factory<list> items;
//	wchar_t buffer[512];
	template<class O, class P>struct num
	{
		void operator()(O *o, P *p)
		{
			if(O::on) *p |= o->offset;
		}
	};
	template<class O, class P>struct denom
	{
		void operator()(O *o, P *p)
		{
			*p |= o->offset;
		}
	};
	
	TestState()
	{
		numerator = 0;
		denominator = 0;
		TL::foreach<list, num>()(&items, &numerator);
		TL::foreach<list, denom>()(&items, &denominator);
	}
	
	bool IsOk(unsigned val){return numerator == (val & denominator);}
};
}