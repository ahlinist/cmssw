// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/15/07
// License: GPL

#include <ostream>

#include "interface/ClassID.h"

#include "interface/TIBFlagTxt.h"
#include "interface/TOBFlagTxt.h"

#include "interface/DQMFlagTxt.h"
#include "interface/DQMFlagXML.h"

DQMFlagTxt::DQMFlagTxt( const DQMFlagXML *poDQM_FLAGXML)
  : FlagTxt( *( dynamic_cast<const FlagXML *>( poDQM_FLAGXML) ) ) {}

Clonable *DQMFlagTxt::cloneXML() const {
  return new DQMFlagXML( this);
}

ClassIDBase::ID DQMFlagTxt::getID() const {
  return ClassID<DQMFlagTxt>::get();
}

int DQMFlagTxt::isChildValid( const FlagTxt *poCHILD_CANDIDATE) const {
  return dynamic_cast<const TIBFlagTxt *>( poCHILD_CANDIDATE) ||
         dynamic_cast<const TOBFlagTxt *>( poCHILD_CANDIDATE) ?
         1 :
         0;
}

std::ostream &
  operator <<( std::ostream &roOut, const DQMFlagTxt &roFLAG) {

  // Print DQM info
  roOut << static_cast<int>( roFLAG.getState()) << " : "
        << roFLAG.getComment();
        
  // Try TIB Child
  if( FlagTxt *poTIBFlagTxt = roFLAG.getChild<TIBFlagTxt>()) {
    roOut << std::endl << *poTIBFlagTxt;
  }

  // Try TOB Child
  if( FlagTxt *poTOBFlagTxt = roFLAG.getChild<TOBFlagTxt>()) {
    roOut << std::endl << *poTOBFlagTxt;
  }

  return roOut;
}
