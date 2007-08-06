// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/27/07
// License: GPL

#include "CalibTracker/SiStripRunSummary/interface/ClassID.h"

#include "CalibTracker/SiStripRunSummary/interface/TriggerFlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/TriggerFlagXML.h"

// --[ TRIGGER FLAG TXT ]------------------------------------------------------
//                                                  --[ PUBLIC ]--
TriggerFlagTxt::TriggerFlagTxt( const TriggerFlagXML *poTRIGGER_FLAGXML)
  : FlagTxt( *( dynamic_cast<const FlagXML *>( poTRIGGER_FLAGXML) ) ) {}

//                                                  --[ PROTECTED ]--
Clonable *TriggerFlagTxt::cloneXML() const {
  return new TriggerFlagXML( this);
}

ClassIDBase::ID TriggerFlagTxt::getID() const {
  return ClassID<TriggerFlagTxt>::get();
}
