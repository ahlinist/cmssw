// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/27/07
// License: GPL

#include "CalibTracker/SiStripRunSummary/interface/ClassID.h"

#include "CalibTracker/SiStripRunSummary/interface/TriggerFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/TriggerFlagTxt.h"

// --[ TRIGGER FLAG XML ]------------------------------------------------------
//                                                  --[ PUBLIC ]--
TriggerFlagXML::TriggerFlagXML( const TriggerFlagTxt *poTRIGGER_FLAGTXT)
  : FlagXML( *( dynamic_cast<const FlagTxt *>( poTRIGGER_FLAGTXT) ) ) {}

//                                                  --[ PROTECTED ]--
Clonable *TriggerFlagXML::cloneTxt() const {
  return new TriggerFlagTxt( this);
}

ClassIDBase::ID TriggerFlagXML::getID() const {
  return ClassID<TriggerFlagXML>::get();
}
