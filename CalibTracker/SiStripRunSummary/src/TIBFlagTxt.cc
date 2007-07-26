// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/15/07
// License: GPL

#include "interface/ClassID.h"

#include "interface/TIBFlagTxt.h"
#include "interface/TIBFlagXML.h"

TIBFlagTxt::TIBFlagTxt( const TIBFlagXML *poTIB_FLAGXML)
  : FlagTxt( *( dynamic_cast<const FlagXML *>( poTIB_FLAGXML) ) ) {}

Clonable *TIBFlagTxt::cloneXML() const {
  return new TIBFlagXML( this);
}

ClassIDBase::ID TIBFlagTxt::getID() const {
  return ClassID<TIBFlagTxt>::get();
}
