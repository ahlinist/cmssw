// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/15/07
// License: GPL

#include <ostream>

#include "CalibTracker/SiStripRunSummary/interface/ClassID.h"

// Include all subflags that current one may have
#include "CalibTracker/SiStripRunSummary/interface/TECFlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/TIBFlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/TIDFlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/TOBFlagTxt.h"

#include "CalibTracker/SiStripRunSummary/interface/DQMFlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/DQMFlagXML.h"

// --[ DQM FLAG TXT ]----------------------------------------------------------
//                                                  --[ PUBLIC ]--
DQMFlagTxt::DQMFlagTxt( const DQMFlagXML *poDQM_FLAGXML)
  : FlagTxt( *( dynamic_cast<const FlagXML *>( poDQM_FLAGXML) ) ) {}

//                                                  --[ PROTECTED ]--
Clonable *DQMFlagTxt::cloneXML() const {
  return new DQMFlagXML( this);
}

ClassIDBase::ID DQMFlagTxt::getID() const {
  return ClassID<DQMFlagTxt>::get();
}

int DQMFlagTxt::isChildValid( const FlagTxt *poCHILD_CANDIDATE) const {
  return dynamic_cast<const TECFlagTxt *>( poCHILD_CANDIDATE) ||
         dynamic_cast<const TIBFlagTxt *>( poCHILD_CANDIDATE) ||
         dynamic_cast<const TIDFlagTxt *>( poCHILD_CANDIDATE) ||
         dynamic_cast<const TOBFlagTxt *>( poCHILD_CANDIDATE) ?
         1 :
         0;
}

// --[ HELPER FUNCTIONS ]------------------------------------------------------
std::ostream &
  operator <<( std::ostream &roOut, const DQMFlagTxt &roFLAG) {

  // Print DQM info: use Flag operator <<
  roOut << dynamic_cast<const Flag &>( roFLAG);

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
