// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/15/07
// License: GPL

#include <ostream>

#include "CalibTracker/SiStripRunSummary/interface/ClassID.h"

// Include all subflags that current one may have
#include "CalibTracker/SiStripRunSummary/interface/TECFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/TIBFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/TIDFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/TOBFlagXML.h"

#include "CalibTracker/SiStripRunSummary/interface/DQMFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/DQMFlagTxt.h"

// --[ DQM FLAG XML ]----------------------------------------------------------
//                                                  --[ PUBLIC ]--
DQMFlagXML::DQMFlagXML( const DQMFlagTxt *poDQM_FLAGTXT)
  : FlagXML( *( dynamic_cast<const FlagTxt *>( poDQM_FLAGTXT) ) ) {}

//                                                  --[ PROTECTED ]--
Clonable *DQMFlagXML::cloneTxt() const {
  return new DQMFlagTxt( this);
}

ClassIDBase::ID DQMFlagXML::getID() const {
  return ClassID<DQMFlagXML>::get();
}

int DQMFlagXML::isChildValid( const FlagXML *poCHILD_CANDIDATE) const {
  return dynamic_cast<const TECFlagXML *>( poCHILD_CANDIDATE) ||
         dynamic_cast<const TIBFlagXML *>( poCHILD_CANDIDATE) ||
         dynamic_cast<const TIDFlagXML *>( poCHILD_CANDIDATE) ||
         dynamic_cast<const TOBFlagXML *>( poCHILD_CANDIDATE) ?
         1 :
         0;
}

// --[ HELPER FUNCTIONS ]------------------------------------------------------
std::ostream &
  operator <<( std::ostream &roOut, const DQMFlagXML &roFLAG) {

  // Print DQM info: use Flag operator <<
  roOut << dynamic_cast<const Flag &>( roFLAG);

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
