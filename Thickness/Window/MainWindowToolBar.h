#pragma once
#include "../Resource.h"
#include "InitToolBar.hpp"

void InitToolBar();

void SycleMeashurement(HWND h);//обработчик для кнопки "Циклическое измерение"
void SingleMeashurement(HWND h);//обработчик для кнопки "Одиночное измерение"
void TestMeashurement(HWND);
void ViewerBtn(HWND);
void StopMeashurement(HWND h);//обработчик для кнопки "Остановка измерения"
void OptionsBtn(HWND h);//обработчик "Режим налатка"
void ExitTubeBtn(HWND h);//Управление механизмом подъёма блока датчиков
void ClampingBtn(HWND h);//Смыкание датчиков
void Clamp1On(HWND);
void Clamp2On(HWND);

#define TOOL_LIP_TEXT(name, text)struct name{static wchar_t *Do(){return text;}};
#if 0
TOOL_LIP_TEXT(ToolLipCycleBtn     , L"Циклический режим работы")
TOOL_LIP_TEXT(ToolLipRefresh  	  , L"Запуск одиночного измерения")
TOOL_LIP_TEXT(ToolLipQueryBtn  	  , L"Тестирование датчиков")
TOOL_LIP_TEXT(ToolLipMashBtn  	  , L"Подробный просмотр результатов измерений")
TOOL_LIP_TEXT(ToolLipReset  	  , L"Выход из режима измерений")
TOOL_LIP_TEXT(ToolLipOptionsBtn	  , L"Смена режима работы установки")
TOOL_LIP_TEXT(ToolLipExitTubeBtn  , L"Управление механизмом блока датчиков")
TOOL_LIP_TEXT(ToolLipClampingBtn  , L"Свод-развод датчиков")

TOOL_LIP_TEXT(ToolLipClamp1OnBtn  , L"Прижим 1")
TOOL_LIP_TEXT(ToolLipClamp2OnBtn  , L"Прижим 2")
#else
TOOL_LIP_TEXT(ToolLipCycleBtn     , L"Цикл")
TOOL_LIP_TEXT(ToolLipRefresh  	  , L"Эталон")
TOOL_LIP_TEXT(ToolLipQueryBtn  	  , L"Тест")
TOOL_LIP_TEXT(ToolLipMashBtn  	  , L"Просмотр")
TOOL_LIP_TEXT(ToolLipReset  	  , L"Стоп")
TOOL_LIP_TEXT(ToolLipOptionsBtn	  , L"Наладка")
TOOL_LIP_TEXT(ToolLipExitTubeBtn  , L"Блок датчиков")
TOOL_LIP_TEXT(ToolLipClampingBtn  , L"Развод датчиков")

TOOL_LIP_TEXT(ToolLipClamp1OnBtn  , L"Прижим 1")
TOOL_LIP_TEXT(ToolLipClamp2OnBtn  , L"Прижим 2")
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



