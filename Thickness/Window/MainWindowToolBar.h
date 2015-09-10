#pragma once
#include "../Resource.h"
#include "InitToolBar.hpp"

void InitToolBar();

void SycleMeashurement(HWND h);//���������� ��� ������ "����������� ���������"
void SingleMeashurement(HWND h);//���������� ��� ������ "��������� ���������"
void TestMeashurement(HWND);
void ViewerBtn(HWND);
void StopMeashurement(HWND h);//���������� ��� ������ "��������� ���������"
void OptionsBtn(HWND h);//���������� "����� �������"
void ExitTubeBtn(HWND h);//���������� ���������� ������� ����� ��������
void ClampingBtn(HWND h);//�������� ��������
void Clamp1On(HWND);
void Clamp2On(HWND);

#define TOOL_LIP_TEXT(name, text)struct name{static wchar_t *Do(){return text;}};
#if 0
TOOL_LIP_TEXT(ToolLipCycleBtn     , L"����������� ����� ������")
TOOL_LIP_TEXT(ToolLipRefresh  	  , L"������ ���������� ���������")
TOOL_LIP_TEXT(ToolLipQueryBtn  	  , L"������������ ��������")
TOOL_LIP_TEXT(ToolLipMashBtn  	  , L"��������� �������� ����������� ���������")
TOOL_LIP_TEXT(ToolLipReset  	  , L"����� �� ������ ���������")
TOOL_LIP_TEXT(ToolLipOptionsBtn	  , L"����� ������ ������ ���������")
TOOL_LIP_TEXT(ToolLipExitTubeBtn  , L"���������� ���������� ����� ��������")
TOOL_LIP_TEXT(ToolLipClampingBtn  , L"����-������ ��������")

TOOL_LIP_TEXT(ToolLipClamp1OnBtn  , L"������ 1")
TOOL_LIP_TEXT(ToolLipClamp2OnBtn  , L"������ 2")
#else
TOOL_LIP_TEXT(ToolLipCycleBtn     , L"����")
TOOL_LIP_TEXT(ToolLipRefresh  	  , L"������")
TOOL_LIP_TEXT(ToolLipQueryBtn  	  , L"����")
TOOL_LIP_TEXT(ToolLipMashBtn  	  , L"��������")
TOOL_LIP_TEXT(ToolLipReset  	  , L"����")
TOOL_LIP_TEXT(ToolLipOptionsBtn	  , L"�������")
TOOL_LIP_TEXT(ToolLipExitTubeBtn  , L"���� ��������")
TOOL_LIP_TEXT(ToolLipClampingBtn  , L"������ ��������")

TOOL_LIP_TEXT(ToolLipClamp1OnBtn  , L"������ 1")
TOOL_LIP_TEXT(ToolLipClamp2OnBtn  , L"������ 2")
#endif
#undef TOOL_LIP_TEXT

typedef TL::MkTlst<
  SeparatorToolbar<0>
  , ButtonToolbar<IDB_CycleBtn, SycleMeashurement, ToolLipCycleBtn>  
  , ButtonToolbar<IDB_Refresh, SingleMeashurement, ToolLipRefresh>  
  , ButtonToolbar<IDB_QueryBtn, TestMeashurement , ToolLipQueryBtn> 
  , ButtonToolbar<IDB_Reset, StopMeashurement    , ToolLipReset>  
  , ButtonToolbar<IDB_MashBtn, ViewerBtn         , ToolLipMashBtn>  
  , SeparatorToolbar<1>							 
  , ButtonToolbar<IDB_OptionsBtn, OptionsBtn     , ToolLipOptionsBtn>  
  , ButtonToolbar<IDB_SensorsUnit, ExitTubeBtn   , ToolLipExitTubeBtn> 
  , ButtonToolbar<IDB_ClampBtn, ClampingBtn   , ToolLipClampingBtn> 
  //IDB_SensorsUnit
  , ButtonToolbar<IDB_Clamp1OnBtn, Clamp1On, ToolLipClamp1OnBtn>
  , ButtonToolbar<IDB_Clamp2OnBtn, Clamp2On, ToolLipClamp2OnBtn>
  , SeparatorToolbar<2>
>::Result tool_button_list;
//IDB_ExitTubeBtn

void Disable3Button(HWND );
void Enanle3Button(HWND );

void DisableAllButton();



