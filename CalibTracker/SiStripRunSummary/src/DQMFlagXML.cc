// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/15/07
// License: GPL

#include <ostream>

#include "CalibTracker/SiStripRunSummary/interface/ClassID.h"

#include "CalibTracker/SiStripRunSummary/interface/TIBFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/TOBFlagXML.h"

#include "CalibTracker/SiStripRunSummary/interface/DQMFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/DQMFlagTxt.h"

DQMFlagXML::DQMFlagXML( const DQMFlagTxt *poDQM_FLAGTXT)
  : FlagXML( *( dynamic_cast<const FlagTxt *>( poDQM_FLAGTXT) ) ) {}

Clonable *DQMFlagXML::cloneTxt() const {
  return new DQMFlagTxt( this);
}

ClassIDBase::ID DQMFlagXML::getID() const {
  return ClassID<DQMFlagXML>::get();
}

int DQMFlagXML::isChildValid( const FlagXML *poCHILD_CANDIDATE) const {
  return dynamic_cast<const TIBFlagXML *>( poCHILD_CANDIDATE) ||
         dynamic_cast<const TOBFlagXML *>( poCHILD_CANDIDATE) ?
         1 :
         0;
}

std::ostream &
  operator <<( std::ostream &roOut, const DQMFlagXML &roFLAG) {

  // Print DQM info
  roOut << static_cast<int>( roFLAG.getState()) << " : "
        << roFLAG.getComment();
        
  // Try TIB Child
  if( FlagXML *poTIBFlagXML = roFLAG.getChild<TIBFlagXML>()) {
    roOut << std::endl << *poTIBFlagXML;
  }

  // Try TOB Child
  if( FlagXML *poTOBFlagXML = roFLAG.getChild<TOBFlagXML>()) {
    roOut << std::endl << *poTOBFlagXML;
  }

  return roOut;
}
