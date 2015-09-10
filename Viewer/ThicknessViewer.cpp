#include "stdafx.h"
#include "ThicknessViewer.h"
#include <stdio.h>
#include "EmptyWindow.h"
#include "PrimaryData.h"
#include "LabelMessage.h"
#include "DetailedWindow.h"
#include "Automat.h"
#include "DebugMess.h"
#ifdef XDEBUG
#define xprint debug.print(__FUNCTION__);
#define dprint debug.print
#define d_mess(x)
#define x_mess debug.print
#else
#define xprint
#define dprint
#endif
//------------------------------------------------------------------------------------------------------
using namespace Gdiplus;


ThicknessViewer::CursorLabel::CursorLabel(Chart &chart)
	: chart(chart)	
{
	label.fontHeight = 10;
	label.top = 2;	
}

bool ThicknessViewer::CursorLabel::Draw(TMouseMove &l, VGraphics &g)
{
	char buf[512];
	double dX = (double)(chart.rect.right - chart.rect.left - chart.offsetAxesLeft - chart.offsetAxesRight) / (chart.maxAxesX - chart.minAxesX);
	double x = -0.5 * dX + chart.rect.left + chart.offsetAxesLeft;
	int leftOffs = int((double(l.x) - x) / dX);
	int status =  currentAutomat->compute.thicknessData.status[leftOffs - 1];///////////////////////////////////////////////
	if(status)
	{		
		char *txt = StatusLabel(status).text;
		if(status == PrimaryData::Nominal || status == PrimaryData::DefectMax || status == PrimaryData::DefectMin || status == PrimaryData::DefectMinMax || status == PrimaryData::DefectDifferentWall)
		{
			double yMin = currentAutomat->compute.thicknessData.zonesMin[leftOffs - 1];////////////////////////////////////////////////////
			double yMax = currentAutomat->compute.thicknessData.zonesMax[leftOffs - 1];
			double delta = yMax - yMin;
			sprintf(buf, "<ff>Зона <0xff0000>%d <ff>Смещение <ff0000>%.2f <ff>м толщ.мин. <0xff0000>%0.2f <ff>толщ.макс. <0xff0000>%0.2f <ff>разн-ость <ff0000>%.2f %s <ff>Номин.толщ. %.2f  <ff>Откл-е минус <ff0000>%.2f%% <ff>плюс <ff0000>%.2f%% <ff>разн-ось <ff0000>%.2f%%"
				, leftOffs
				, 0.05 * leftOffs 
				, yMin
				, yMax
				, delta
				, txt
				, Singleton<ThresholdsTable>::Instance().items.get<NominalThickness>().value
				, Singleton<ThresholdsTable>::Instance().items.get<NominalPercentMin>().value
				, Singleton<ThresholdsTable>::Instance().items.get<NominalPercentMax>().value	
				, Singleton<ThresholdsTable>::Instance().items.get<DefectDifferentWallPercent>().value
				);
		}
		else
		{
			sprintf(buf, "<ff>Зона <ff0000>%d  %s  "
				, leftOffs
				, txt
				);
		}
		RECT r;
		GetClientRect(l.hwnd, &r);
		label.left = 40;
		label = buf;
		label.Draw(g());
		return true;
	}
	
	return false;
}

//bool  ThicknessViewer::CursorLabel::GetColorBar(int zone, double &data_, unsigned &color)
bool ThicknessViewer::CursorLabel::GetColorBar(int zone, double &data_, unsigned &color, double &data_1, unsigned &color1)
{
#if 0
	if(zone)
	{
		int i = zone - 1;
		data_ = currentAutomat->compute.thicknessData.zonesMin[i];///////////////////////////////////////
		ColorBar(data_, color, currentAutomat->compute.thicknessData.status[i]);////////////////////////////////////////
		return 0 != currentAutomat->compute.thicknessData.status[i];//////////////////////////////////////////////////////////
	}
	data_ = 0;
	return true;
#else
	if(zone)
	{
		int i = zone - 1;
		data_1 = currentAutomat->compute.thicknessData.zonesMin[i];
		data_ = currentAutomat->compute.thicknessData.zonesMax[i];
		ColorBar(data_, color1, currentAutomat->compute.thicknessData.status[i]);
		
		color = color1;
		unsigned char *x = (unsigned char *) &color;
		x[0] = unsigned char(3.0 * x[0] / 4);
		x[1] = unsigned char(3.0 * x[1] / 4);
		x[2] = unsigned char(3.0 * x[2] / 4);
		return 0 != currentAutomat->compute.thicknessData.status[i];
	}
	data_ = 0;
	return true;
#endif
}
//-----------------------------------------------------------------------------
ThicknessViewer::ThicknessViewer()
	: backScreen(NULL)
	, chart(backScreen)
	, cursor(chart)
	, openDetailedWindow(false)
	, painting(true)
	, mouseMove(true)
	, cursorLabel(chart)
{
	chart.rect.left = 10;
	chart.rect.top = 17;
	chart.offsetAxesBottom = 10;
	
	chart.minAxesX = 0;
	chart.maxAxesX = count_zones + 1 ;
	chart.minAxesY = 0;

	cursor.SetMouseMoveHandler(&cursorLabel, &CursorLabel::Draw);
	chart.items.get<BarSeriesDouble>().SetColorBarHandler(&cursorLabel, &ThicknessViewer::CursorLabel::GetColorBar);

	chart.items.get<BottomAxesMeters>().minBorder = 0;
	chart.items.get<BottomAxesMeters>().maxBorder = 0.001 * count_zones * zone_length;
}
//----------------------------------------------------------------------------------------------------
#pragma warning(disable : 4996)
void ThicknessViewer::operator()(TSize &l)
{
	if(l.resizing == SIZE_MINIMIZED || 0 == l.Width || 0 == l.Height) return;	
	
	if(NULL != backScreen)
	{
		if(backScreen->GetWidth() < l.Width || backScreen->GetHeight() < l.Height)
		{
			delete backScreen;
		    backScreen = new Bitmap(l.Width, l.Height);
		}
	}
	else if(l.Width > 0 && l.Height > 0)
	{
		backScreen = new Bitmap(l.Width, l.Height);
	}
	else
	{
		return;
	}
    Graphics g(backScreen);
	SolidBrush solidBrush(Color(0xffaaaaaa));
	g.FillRectangle(&solidBrush, 0, 0, 10, l.Height);   
	g.FillRectangle(&solidBrush, 0, 1, l.Width, 29);  
	chart.minAxesY = Singleton<BorderCredibilityTable>::Instance().items.get<MinimumThicknessPipeWall>().value;
	chart.maxAxesY = Singleton<BorderCredibilityTable>::Instance().items.get<MaximumThicknessPipeWall>().value;
	chart.rect.right = l.Width - 10;
	chart.rect.bottom = l.Height - 10;
	chart.Draw(g);
	g.DrawLine(&Pen(-1, 1), 0, 0, l.Width, 0);
	g.DrawLine(&Pen(0xff000000, 1), 0, l.Height - 1, l.Width, l.Height - 1);
}
//----------------------------------------------------------------------------------------------------
void ThicknessViewer::operator()(TPaint &l)
{
	PAINTSTRUCT p;
	HDC hdc = BeginPaint(l.hwnd, &p);
	{		
		Graphics g(hdc);		
		g.DrawCachedBitmap(&CachedBitmap(backScreen, &g), 0, 0);
		cursor.VerticalCursor(storedMouseMove, PaintGraphics(g));
	}
	EndPaint(l.hwnd, &p);
}
//-----------------------------------------------------------------------------------------------------
void ThicknessViewer::operator()(TMouseMove &l)
{
#if 1
	if(mouseMove)
	{
		if(cursor.VerticalCursor(l, HDCGraphics(l.hwnd, backScreen)))
		{
			storedMouseMove = l;
		}
	}
#else
	if(mouseMove)
	{
		for(int i = 0; i < dimention_of(thicknessViewer); ++i)
		{
			l.hwnd = thicknessViewer[i]->hWnd;
			thicknessViewer[i]->cursor.VerticalCursor(l, HDCGraphics(thicknessViewer[i]->hWnd, thicknessViewer[i]->backScreen));
			thicknessViewer[i]->storedMouseMove = l;
		}
	}
#endif
}
//----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
void ThicknessViewer::operator()(TLButtonDown &l)
{
	mouseMove = false;
}
//--------------------------------------------------------------------------
void ThicknessViewer::operator()(TLButtonDbClk &l)
{
#if 1
	mouseMove = true;
	if(cursor.VerticalCursor(*(TMouseMove *)&l, HDCGraphics(l.hwnd, backScreen)))
	{
		storedMouseMove.x = l.x;
	}
#else
	for(int i = 0; i < dimention_of(thicknessViewer); ++i)
	{
		thicknessViewer[i]->mouseMove = true;
		l.hwnd = thicknessViewer[i]->hWnd;
		thicknessViewer[i]->cursor.VerticalCursor(*(TMouseMove *)&l, HDCGraphics(thicknessViewer[i]->hWnd, thicknessViewer[i]->backScreen));
		thicknessViewer[i]->storedMouseMove.x = l.x;
	}
#endif
}
void ThicknessViewer::operator()(TMouseWell &l)
{
#if 1
	RECT r;
	GetWindowRect(l.hwnd, &r);
	if(InRect(l.x, l.y, r))
	{
		mouseMove = false;

		chart.items.get<BottomAxesMeters>().OffsetToPixel(storedMouseMove.x, l.delta / 120);
		cursor.VerticalCursor(storedMouseMove, HDCGraphics(storedMouseMove.hwnd, backScreen));
	}
#else
	mouseMove = false;
	storedMouseMove.hwnd = l.hwnd;
	storedMouseMove.y = (WORD)(chart.rect.top + chart.rect.bottom)/2;
	chart.items.get<BottomAxesMeters>().OffsetToPixel(storedMouseMove.x, l.delta / 120);
	cursor.VerticalCursor(storedMouseMove, HDCGraphics(l.hwnd, backScreen));
#endif
}
void ThicknessViewer::operator()(TKeyDown &l)
{	
	int offs = VK_RIGHT == l.VirtKey ? -1 : VK_LEFT == l.VirtKey ? 1 : 0;
	if(offs)
	{
		mouseMove = false;
		chart.items.get<BottomAxesMeters>().OffsetToPixel(storedMouseMove.x, offs);
		cursor.VerticalCursor(storedMouseMove, HDCGraphics(storedMouseMove.hwnd, backScreen));
	}
}
//-----------------------------------------------------------------------------------------------------
void ThicknessViewer::Update()
{
	RepaintWindow(hWnd);
}
//------------------------------------------------------------------------------------------------------
void ThicknessViewer::DrawSomeBar()
{
	static unsigned lastTime;
	unsigned t = GetTickCount();
	if(t - lastTime > 1000)
	{
		lastTime = t;
		RepaintWindow(hWnd);
	}
}
//------------------------------------------------------------------------------------------------------
ThicknessViewer thicknessViewer;
