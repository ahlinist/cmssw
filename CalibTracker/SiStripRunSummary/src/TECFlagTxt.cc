// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/27/07
// License: GPL

#include "CalibTracker/SiStripRunSummary/interface/ClassID.h"

#include "CalibTracker/SiStripRunSummary/interface/TECFlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/TECFlagXML.h"

// --[ TEC FLAG TXT ]----------------------------------------------------------
//                                                  --[ PUBLIC ]--
TECFlagTxt::TECFlagTxt( const TECFlagXML *poTEC_FLAGXML)
  : FlagTxt( *( dynamic_cast<const FlagXML *>( poTEC_FLAGXML) ) ) {}

//                                                  --[ PROTECTED ]--
Clonable *TECFlagTxt::cloneXML() const {
  return new TECFlagXML( this);
}

ClassIDBase::ID TECFlagTxt::getID() const {
  return ClassID<TECFlagTxt>::get();
}
