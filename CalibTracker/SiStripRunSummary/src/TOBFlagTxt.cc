// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/15/07
// License: GPL

#include "CalibTracker/SiStripRunSummary/interface/ClassID.h"

#include "CalibTracker/SiStripRunSummary/interface/TOBFlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/TOBFlagXML.h"

TOBFlagTxt::TOBFlagTxt( const TOBFlagXML *poTOB_FLAGXML)
  : FlagTxt( *( dynamic_cast<const FlagXML *>( poTOB_FLAGXML) ) ) {}

Clonable *TOBFlagTxt::cloneXML() const {
  return new TOBFlagXML( this);
}

ClassIDBase::ID TOBFlagTxt::getID() const {
  return ClassID<TOBFlagTxt>::get();
}
