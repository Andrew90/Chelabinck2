#pragma once

template<class T>struct MM: T
{
	typename T::type_value value;

};
/*
#define SetMM(n) template<>struct MM<n>\
{\
    typedef typedef n::type_value type_value;\
	type_value value;\
};
*/
template<>struct MM<NominalPercentMin			>{};
template<>struct MM<NominalPercentMax	        >{};
template<>struct MM<DefectDifferentWallPercent>{};

//#undef SetMM

typedef TL::MkTlst<
NormativeTechnicalDocument
, SteelGrade				
, TubeDiameter				
, NominalThickness			
, NominalPercentMin			
, MM<NominalPercentMin>			    
, NominalPercentMax	        
, MM<NominalPercentMax>			    
, DefectDifferentWallPercent
, MM<DefectDifferentWallPercent>     
>::Result ParameterTypeSizeList;