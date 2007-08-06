// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/15/07
// License: GPL

#include "CalibTracker/SiStripRunSummary/interface/ClassID.h"

#include "CalibTracker/SiStripRunSummary/interface/TIBFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/TIBFlagTxt.h"

// --[ TIB FLAG XML ]----------------------------------------------------------
//                                                  --[ PUBLIC ]--
TIBFlagXML::TIBFlagXML( const TIBFlagTxt *poTIB_FLAGTXT)
  : FlagXML( *( dynamic_cast<const FlagTxt *>( poTIB_FLAGTXT) ) ) {}

//                                                  --[ PROTECTED ]--
Clonable *TIBFlagXML::cloneTxt() const {
  return new TIBFlagTxt( this);
}

ClassIDBase::ID TIBFlagXML::getID() const {
  return ClassID<TIBFlagXML>::get();
}
