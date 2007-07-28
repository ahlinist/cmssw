// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/27/07
// License: GPL

#include "CalibTracker/SiStripRunSummary/interface/ClassID.h"

#include "CalibTracker/SiStripRunSummary/interface/TECFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/TECFlagTxt.h"

TECFlagXML::TECFlagXML( const TECFlagTxt *poTEC_FLAGTXT)
  : FlagXML( *( dynamic_cast<const FlagTxt *>( poTEC_FLAGTXT) ) ) {}

Clonable *TECFlagXML::cloneTxt() const {
  return new TECFlagTxt( this);
}

ClassIDBase::ID TECFlagXML::getID() const {
  return ClassID<TECFlagXML>::get();
}
