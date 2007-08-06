// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/27/07
// License: GPL

#include "CalibTracker/SiStripRunSummary/interface/ClassID.h"

#include "CalibTracker/SiStripRunSummary/interface/TIDFlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/TIDFlagXML.h"

// --[ TID FLAG TXT ]----------------------------------------------------------
//                                                  --[ PUBLIC ]--
TIDFlagTxt::TIDFlagTxt( const TIDFlagXML *poTID_FLAGXML)
  : FlagTxt( *( dynamic_cast<const FlagXML *>( poTID_FLAGXML) ) ) {}

//                                                  --[ PROTECTED ]--
Clonable *TIDFlagTxt::cloneXML() const {
  return new TIDFlagXML( this);
}

ClassIDBase::ID TIDFlagTxt::getID() const {
  return ClassID<TIDFlagTxt>::get();
}
