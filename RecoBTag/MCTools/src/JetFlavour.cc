#include "RecoBTag/MCTools/interface/JetFlavour.h"

JetFlavour::JetFlavour()
{
  m_mainFlavour          = 0 ;
  m_heaviestFlavour      = 0 ;
  m_minimumDeltaRFlavour = 0 ;

  m_mainOrigFlavour      = 0 ;
  
  m_originFlavour        = 0 ;

  m_initialFlavour        = 0 ;

  m_initialPartonSplitsToC = false ;
  m_initialPartonSplitsToB = false ;
  
  m_pMainParton         =    0.0 ;       
  m_pClosestParton      =    0.0 ;       
  m_pHeaviestParton     =    0.0 ;       
  m_deltaRMainParton    =  999.0 ;  
  m_deltaRClosestParton =  999.0 ;

  m_Vec4MainParton.SetPx ( 0.0 ) ;
  m_Vec4MainParton.SetPy ( 0.0 ) ;
  m_Vec4MainParton.SetPz ( 0.0 ) ;
  m_Vec4MainParton.SetE  ( 0.0 ) ;
  m_Vec4ClosestParton.SetPx ( 0.0 )  ;
  m_Vec4ClosestParton.SetPy ( 0.0 )  ;
  m_Vec4ClosestParton.SetPz ( 0.0 )  ;
  m_Vec4ClosestParton.SetE  ( 0.0 )  ;
  m_Vec4HeaviestParton.SetPx ( 0.0 )  ;
  m_Vec4HeaviestParton.SetPy ( 0.0 )  ;
  m_Vec4HeaviestParton.SetPz ( 0.0 )  ;
  m_Vec4HeaviestParton.SetE  ( 0.0 )  ;

  m_Vec4SummedPartons.SetPx ( 0.0 ) ;
  m_Vec4SummedPartons.SetPy ( 0.0 ) ;
  m_Vec4SummedPartons.SetPz ( 0.0 ) ;
  m_Vec4SummedPartons.SetE  ( 0.0 ) ;
  
  m_Vec4OriginParton.SetPx ( 0.0 ) ;
  m_Vec4OriginParton.SetPy ( 0.0 ) ;
  m_Vec4OriginParton.SetPz ( 0.0 ) ;
  m_Vec4OriginParton.SetE  ( 0.0 ) ;
  
  m_hasDown   = false ;
  m_hasUp     = false ;
  m_hasStrange= false ;
  m_hasCharm  = false ;
  m_hasBottom = false ;
  m_hasGluon  = false ;

  m_nDown   = 0 ;
  m_nUp     = 0 ;
  m_nStrange= 0 ;
  m_nCharm  = 0 ;
  m_nBottom = 0 ;
  m_nGluon  = 0 ;

  m_numberOfSources = 0 ;

  m_flavourSources.erase ( m_flavourSources.begin() , m_flavourSources.end() ) ;
}
