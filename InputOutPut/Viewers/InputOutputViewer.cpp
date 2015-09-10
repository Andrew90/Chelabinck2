#include "stdafx.h"
#include "InputOutputViewer.h"
#include "ArchiveEvents.h"
#include "ColorLabel.h"
#include "Messages.h"
#include "AppBase.h"
#include "../Window/MainWindow.h"
#include "Chart.h"
#include "DebugMess.h"

#ifdef XDEBUG
#define dprint debug.print
#else	
#define  dprint
#endif
ClientEvents clientEvents;
using namespace Gdiplus;
//-----------------------------------------------------------------------------
struct __draw_data__
{
	int x, dY;
	Graphics &g;
	int colorOn;
	int value;
	__draw_data__(int x, Graphics &g, int colorOn, int value)  : x(x), dY(15), g(g), colorOn(colorOn), value(value) {}
};

template<class O, class P>struct __draw__
{
	void operator()(O *o, P *p)
	{
		ColorLabel l;
		l.left = p->x;
		l.top = p->dY;
		wsprintf(l.buffer, L"<55>%s",  MessageField<O>::Text());
		l.Draw(p->g);
		int color = 0xffcccccc;
	//	if(o->value & p->value) color = p->colorOn;
		p->g.FillRectangle(&SolidBrush(Color(color)), p->x - 20, p->dY - 3, 15, 15);
		p->dY += 20;
	}
};

void InputOutputViewer::Size(Graphics &g, int width, int height)
{
	g.FillRectangle(&SolidBrush(Color(0xffaaaaaa)), 0, 0, width, height); 
	TL::foreach<input_bits_list, __draw__>()(&Singleton<InputsTable>::Instance().items, &__draw_data__(40, g, 0xff0000ff, clientEvents.map->input));
	TL::foreach<output_bits_list, __draw__>()(&Singleton<OutputsTable>::Instance().items, &__draw_data__(230, g, 0xffff0000, clientEvents.map->output));
}
//------------------------------------------------------------------------------------
struct __update_data__
{
	int x, dY;
	HDCGraphics &g;
	int colorOn;
	int value;
	__update_data__(int x, HDCGraphics &g, int colorOn, int value)  : x(x), dY(15), g(g), colorOn(colorOn), value(value) {}
};
template<class O, class P>struct __update__
{
	void operator()(O *o, P *p)
	{
		int color = 0xffcccccc;
		if(o->value & p->value) color = p->colorOn;
		p->g.graphics.FillRectangle(&SolidBrush(Color(color)), p->x - 20, p->dY - 3, 15, 15);
		p->dY += 20;
	//	dprint("color ----------------------- %d %s", color, __FUNCTION__);
	}
};
VOID NTAPI Update(PVOID o, BOOLEAN)
{
	//Graphics g(((MainWindow *)o)->backScreen);
	MainWindow *mw = (MainWindow *)o;
	HDCGraphics g(mw->hWnd, mw->backScreen);
	TL::foreach<input_bits_list, __update__>()(&Singleton<InputsTable>::Instance().items, &__update_data__(40, g, 0xff0000ff, clientEvents.map->input));
	TL::foreach<output_bits_list, __update__>()(&Singleton<OutputsTable>::Instance().items, &__update_data__(230, g, 0xffff0000, clientEvents.map->output));
}
//--------------------------------------------------------------------------------------
void InputOutputViewer::Start(PVOID o)
{
	CreateTimerQueueTimer(&hTimer, NULL, Update, o, 2000, 500, WT_EXECUTEDEFAULT);
}
//------------------------------------------------------------------------------------------
void InputOutputViewer::Stop()
{
	if(hTimer)DeleteTimerQueueTimer( NULL, hTimer, NULL );
}
//--------------------------------------------------------------------------------------------