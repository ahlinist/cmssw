// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/15/07
// License: GPL

#include "CalibTracker/SiStripRunSummary/interface/ClassID.h"

#include "CalibTracker/SiStripRunSummary/interface/TOBFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/TOBFlagTxt.h"

TOBFlagXML::TOBFlagXML( const TOBFlagTxt *poTOB_FLAGTXT)
  : FlagXML( *( dynamic_cast<const FlagTxt *>( poTOB_FLAGTXT) ) ) {}

Clonable *TOBFlagXML::cloneTxt() const {
  return new TOBFlagTxt( this);
}

ClassIDBase::ID TOBFlagXML::getID() const {
  return ClassID<TOBFlagXML>::get();
}
