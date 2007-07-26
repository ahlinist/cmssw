// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/15/07
// License: GPL

#include "CalibTracker/SiStripRunSummary/interface/ClassID.h"

#include "CalibTracker/SiStripRunSummary/interface/TIBFlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/TIBFlagXML.h"

TIBFlagTxt::TIBFlagTxt( const TIBFlagXML *poTIB_FLAGXML)
  : FlagTxt( *( dynamic_cast<const FlagXML *>( poTIB_FLAGXML) ) ) {}

Clonable *TIBFlagTxt::cloneXML() const {
  return new TIBFlagXML( this);
}

ClassIDBase::ID TIBFlagTxt::getID() const {
  return ClassID<TIBFlagTxt>::get();
}
