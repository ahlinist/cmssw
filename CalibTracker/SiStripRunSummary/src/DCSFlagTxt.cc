// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/27/07
// License: GPL

#include <ostream>

#include "CalibTracker/SiStripRunSummary/interface/ClassID.h"

#include "CalibTracker/SiStripRunSummary/interface/TECFlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/TIBFlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/TIDFlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/TOBFlagTxt.h"

#include "CalibTracker/SiStripRunSummary/interface/DCSFlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/DCSFlagXML.h"

DCSFlagTxt::DCSFlagTxt( const DCSFlagXML *poDCS_FLAGXML)
  : FlagTxt( *( dynamic_cast<const FlagXML *>( poDCS_FLAGXML) ) ) {}

Clonable *DCSFlagTxt::cloneXML() const {
  return new DCSFlagXML( this);
}

ClassIDBase::ID DCSFlagTxt::getID() const {
  return ClassID<DCSFlagTxt>::get();
}

int DCSFlagTxt::isChildValid( const FlagTxt *poCHILD_CANDIDATE) const {
  return dynamic_cast<const TECFlagTxt *>( poCHILD_CANDIDATE) ||
         dynamic_cast<const TIBFlagTxt *>( poCHILD_CANDIDATE) ||
         dynamic_cast<const TIDFlagTxt *>( poCHILD_CANDIDATE) ||
         dynamic_cast<const TOBFlagTxt *>( poCHILD_CANDIDATE) ?
         1 :
         0;
}

std::ostream &
  operator <<( std::ostream &roOut, const DCSFlagTxt &roFLAG) {

  // Print DCS info
  roOut << static_cast<int>( roFLAG.getState()) << " : "
        << roFLAG.getComment();

  // Try TEC Child
  if( TECFlagTxt *poTECFlagTxt = 
        dynamic_cast<TECFlagTxt *>( roFLAG.getChild<TECFlagTxt>()) ) {

    roOut << std::endl << *poTECFlagTxt;
  }
        
  // Try TIB Child
  if( TIBFlagTxt *poTIBFlagTxt = 
        dynamic_cast<TIBFlagTxt *>( roFLAG.getChild<TIBFlagTxt>()) ) {

    roOut << std::endl << *poTIBFlagTxt;
  }

  // Try TID Child
  if( TIDFlagTxt *poTIDFlagTxt = 
        dynamic_cast<TIDFlagTxt *>( roFLAG.getChild<TIDFlagTxt>()) ) {

    roOut << std::endl << *poTIDFlagTxt;
  }

  // Try TOB Child
  if( TOBFlagTxt *poTOBFlagTxt = 
        dynamic_cast<TOBFlagTxt *>( roFLAG.getChild<TOBFlagTxt>()) ) {

    roOut << std::endl << *poTOBFlagTxt;
  }

  return roOut;
}
