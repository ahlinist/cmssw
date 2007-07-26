// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/15/07
// License: GPL

#include "interface/ClassID.h"

#include "interface/TIBFlagXML.h"
#include "interface/TIBFlagTxt.h"

TIBFlagXML::TIBFlagXML( const TIBFlagTxt *poTIB_FLAGTXT)
  : FlagXML( *( dynamic_cast<const FlagTxt *>( poTIB_FLAGTXT) ) ) {}

Clonable *TIBFlagXML::cloneTxt() const {
  return new TIBFlagTxt( this);
}

ClassIDBase::ID TIBFlagXML::getID() const {
  return ClassID<TIBFlagXML>::get();
}
