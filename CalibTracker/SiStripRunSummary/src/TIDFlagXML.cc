// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/27/07
// License: GPL

#include "CalibTracker/SiStripRunSummary/interface/ClassID.h"

#include "CalibTracker/SiStripRunSummary/interface/TIDFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/TIDFlagTxt.h"

TIDFlagXML::TIDFlagXML( const TIDFlagTxt *poTID_FLAGTXT)
  : FlagXML( *( dynamic_cast<const FlagTxt *>( poTID_FLAGTXT) ) ) {}

Clonable *TIDFlagXML::cloneTxt() const {
  return new TIDFlagTxt( this);
}

ClassIDBase::ID TIDFlagXML::getID() const {
  return ClassID<TIDFlagXML>::get();
}
