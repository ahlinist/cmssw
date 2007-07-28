// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/27/07
// License: GPL

#include <ostream>

#include "CalibTracker/SiStripRunSummary/interface/ClassID.h"

#include "CalibTracker/SiStripRunSummary/interface/TECFlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/TIBFlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/TIDFlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/TOBFlagTxt.h"

#include "CalibTracker/SiStripRunSummary/interface/DAQFlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/DAQFlagXML.h"

DAQFlagTxt::DAQFlagTxt( const DAQFlagXML *poDAQ_FLAGXML)
  : FlagTxt( *( dynamic_cast<const FlagXML *>( poDAQ_FLAGXML) ) ) {}

Clonable *DAQFlagTxt::cloneXML() const {
  return new DAQFlagXML( this);
}

ClassIDBase::ID DAQFlagTxt::getID() const {
  return ClassID<DAQFlagTxt>::get();
}

int DAQFlagTxt::isChildValid( const FlagTxt *poCHILD_CANDIDATE) const {
  return dynamic_cast<const TECFlagTxt *>( poCHILD_CANDIDATE) ||
         dynamic_cast<const TIBFlagTxt *>( poCHILD_CANDIDATE) ||
         dynamic_cast<const TIDFlagTxt *>( poCHILD_CANDIDATE) ||
         dynamic_cast<const TOBFlagTxt *>( poCHILD_CANDIDATE) ?
         1 :
         0;
}

std::ostream &
  operator <<( std::ostream &roOut, const DAQFlagTxt &roFLAG) {

  // Print DAQ info
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
