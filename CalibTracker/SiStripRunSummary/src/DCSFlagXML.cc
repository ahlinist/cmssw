// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/27/07
// License: GPL

#include <ostream>

#include "CalibTracker/SiStripRunSummary/interface/ClassID.h"

#include "CalibTracker/SiStripRunSummary/interface/TECFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/TIBFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/TIDFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/TOBFlagXML.h"

#include "CalibTracker/SiStripRunSummary/interface/DCSFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/DCSFlagTxt.h"

DCSFlagXML::DCSFlagXML( const DCSFlagTxt *poDCS_FLAGTXT)
  : FlagXML( *( dynamic_cast<const FlagTxt *>( poDCS_FLAGTXT) ) ) {}

Clonable *DCSFlagXML::cloneTxt() const {
  return new DCSFlagTxt( this);
}

ClassIDBase::ID DCSFlagXML::getID() const {
  return ClassID<DCSFlagXML>::get();
}

int DCSFlagXML::isChildValid( const FlagXML *poCHILD_CANDIDATE) const {
  return dynamic_cast<const TECFlagXML *>( poCHILD_CANDIDATE) ||
         dynamic_cast<const TIBFlagXML *>( poCHILD_CANDIDATE) ||
         dynamic_cast<const TIDFlagXML *>( poCHILD_CANDIDATE) ||
         dynamic_cast<const TOBFlagXML *>( poCHILD_CANDIDATE) ?
         1 :
         0;
}

std::ostream &
  operator <<( std::ostream &roOut, const DCSFlagXML &roFLAG) {

  // Print DCS info
  roOut << static_cast<int>( roFLAG.getState()) << " : "
        << roFLAG.getComment();

  // Try TEC Child
  if( TECFlagXML *poTECFlagXML = dynamic_cast<TECFlagXML *>( roFLAG.getChild<TECFlagXML>())) {
    roOut << std::endl << *poTECFlagXML;
  }
        
  // Try TIB Child
  if( TIBFlagXML *poTIBFlagXML = dynamic_cast<TIBFlagXML *>( roFLAG.getChild<TIBFlagXML>())) {
    roOut << std::endl << *poTIBFlagXML;
  }

  // Try TID Child
  if( TIDFlagXML *poTIDFlagXML = dynamic_cast<TIDFlagXML *>( roFLAG.getChild<TIDFlagXML>())) {
    roOut << std::endl << *poTIDFlagXML;
  }

  // Try TOB Child
  if( TOBFlagXML *poTOBFlagXML = dynamic_cast<TOBFlagXML *>( roFLAG.getChild<TOBFlagXML>())) {
    roOut << std::endl << *poTOBFlagXML;
  }

  return roOut;
}
