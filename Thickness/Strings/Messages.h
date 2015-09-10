#pragma once
#include <Windows.h>
#include "typelist.hpp"
#pragma warning(disable : 4995)

template<class T>struct MessageField
{
	typedef typename T::__template_must_be_overridded__ noused; 
};

#define ms(n, text) \
struct n;\
template<>struct MessageField<n>\
{\
	static wchar_t *Text(){return text;}\
};

struct NotMessage{};

ms(NotMessage    , L"")
ms(PipeAfterClamp1SQ1BitIn    , L"����� ����� ������� 1(SQ1)")
ms(Clamp1RetractedSQ2BitIn    , L"������ 1 ������(SQ2)")
ms(PipeInletSQ3BitIn          , L"����� �� ����� ��(SQ3)")
ms(OutletPipeSQ4BitIn         , L"����� �� ������ ��(SQ4)")
ms(SensorUnitRetractedSQ5BitIn, L"���� �������� ������(SQ5)")
ms(SensorsDivorcedSQ6BitIn    , L"������� ��������(SQ6)")
ms(PipeClamp2BeforeSQ7BitIn   , L"������� ����� ������� 2(SQ7)")
ms(Clamp2RetractedSQ8BitIn    , L"������ 2 ������(SQ8)")
ms(PipeOnMarkerSQ9BitIn       , L"����� �� ����������������(SQ9)")
ms(NormalAirPressurePaBitIn   , L"�������� ������� � �����")
ms(BlockErrorBitIn            , L"������ ���")
ms(SensorUnitRemovalY1BitOut  , L"���� �������� �����(Y1)")
ms(SensorUnitSupplyY2BitOut   , L"���� �������� ������(Y2)")
ms(SensorsRemovalY3BitOut     , L"������� �����(Y3)")
ms(SensorsSupplyY4BitOut      , L"������� ������(Y4)")
ms(Clamp1SupplyY6BitOut       , L"������ 1 ������(Y6)")
ms(Clamp2SupplyY7BitOut       , L"������ 2 ������(Y7)")
ms(StartBitOut                , L"����� ��")
ms(PipeMarkerY5BitOut         , L"���������������(Y5)")
ms(PowerBitOut                , L"������� ��")
ms(GoAheadBitOut              , L"���������� �������� �����")
ms(GoBackBitOut               , L"���������� �������� �����") 

#undef ms

template<class T>struct DataField
{
	typedef typename T::__template_must_be_overridded__ noused; 
};

#define ms_data(n) template<>struct DataField<n>\
{\
	static wchar_t *Text(double d)\
    {\
	   return 0.0 != d ? L"���" : L"����";\
    }\
};

template<>struct DataField<NotMessage>
{
	static wchar_t *Text(double d)
    {
	   return L"";
    }
};

ms_data(PipeAfterClamp1SQ1BitIn    )
ms_data(Clamp1RetractedSQ2BitIn    )
ms_data(PipeInletSQ3BitIn          )
ms_data(OutletPipeSQ4BitIn         )
ms_data(SensorUnitRetractedSQ5BitIn)
ms_data(SensorsDivorcedSQ6BitIn      )
ms_data(PipeClamp2BeforeSQ7BitIn   )
ms_data(Clamp2RetractedSQ8BitIn    )
ms_data(PipeOnMarkerSQ9BitIn       )
ms_data(NormalAirPressurePaBitIn   )
ms_data(BlockErrorBitIn            )
ms_data(SensorUnitRemovalY1BitOut  )
ms_data(SensorUnitSupplyY2BitOut   )
ms_data(SensorsRemovalY3BitOut     )
ms_data(SensorsSupplyY4BitOut      )
ms_data(Clamp1SupplyY6BitOut       )
ms_data(Clamp2SupplyY7BitOut       )
ms_data(StartBitOut                )
ms_data(PipeMarkerY5BitOut         )
ms_data(PowerBitOut                )
ms_data(GoAheadBitOut              )
ms_data(GoBackBitOut               )

#undef ms_data

typedef TL::MkTlst<
 NotMessage
, PipeAfterClamp1SQ1BitIn    
, Clamp1RetractedSQ2BitIn    
, PipeInletSQ3BitIn          
, OutletPipeSQ4BitIn         
, SensorUnitRetractedSQ5BitIn
, SensorsDivorcedSQ6BitIn      
, PipeClamp2BeforeSQ7BitIn   
, Clamp2RetractedSQ8BitIn    
, PipeOnMarkerSQ9BitIn       
, NormalAirPressurePaBitIn   
, BlockErrorBitIn            
, SensorUnitRemovalY1BitOut  
, SensorUnitSupplyY2BitOut   
, SensorsRemovalY3BitOut     
, SensorsSupplyY4BitOut      
, Clamp1SupplyY6BitOut       
, Clamp2SupplyY7BitOut       
, StartBitOut                
, PipeMarkerY5BitOut         
, PowerBitOut                
, GoAheadBitOut              
, GoBackBitOut               
>::Result message_list; 

void EventLogInputPort();
void EventLogOutputPort();










