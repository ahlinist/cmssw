// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/27/07
// License: GPL

#include <ostream>

#include "CalibTracker/SiStripRunSummary/interface/ClassID.h"

#include "CalibTracker/SiStripRunSummary/interface/DAQFlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/DCSFlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/DQMFlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/TriggerFlagTxt.h"

#include "CalibTracker/SiStripRunSummary/interface/GlobalFlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/GlobalFlagXML.h"

GlobalFlagTxt::GlobalFlagTxt( const GlobalFlagXML *poGLOBAL_FLAGXML)
  : FlagTxt( *( dynamic_cast<const FlagXML *>( poGLOBAL_FLAGXML) ) ) {}

Clonable *GlobalFlagTxt::cloneXML() const {
  return new GlobalFlagXML( this);
}

ClassIDBase::ID GlobalFlagTxt::getID() const {
  return ClassID<GlobalFlagTxt>::get();
}

int GlobalFlagTxt::isChildValid( const FlagTxt *poCHILD_CANDIDATE) const {
  return dynamic_cast<const DAQFlagTxt     *>( poCHILD_CANDIDATE) ||
         dynamic_cast<const DCSFlagTxt     *>( poCHILD_CANDIDATE) ||
         dynamic_cast<const DQMFlagTxt     *>( poCHILD_CANDIDATE) ||
         dynamic_cast<const TriggerFlagTxt *>( poCHILD_CANDIDATE) ?
         1 :
         0;
}

std::ostream &
  operator <<( std::ostream &roOut, const GlobalFlagTxt &roFLAG) {

  // Print Global info
  roOut << static_cast<int>( roFLAG.getState()) << " : "
        << roFLAG.getComment();

  // Try DAQ Child
  if( DAQFlagTxt *poDAQFlagTxt = 
        dynamic_cast<DAQFlagTxt *>( roFLAG.getChild<DAQFlagTxt>()) ) {

    roOut << std::endl << *poDAQFlagTxt;
  }

  // Try DCS Child
  if( DCSFlagTxt *poDCSFlagTxt = 
        dynamic_cast<DCSFlagTxt *>( roFLAG.getChild<DCSFlagTxt>()) ) {

    roOut << std::endl << *poDCSFlagTxt;
  }

  // Try DQM Child
  if( DQMFlagTxt *poDQMFlagTxt = 
        dynamic_cast<DQMFlagTxt *>( roFLAG.getChild<DQMFlagTxt>()) ) {

    roOut << std::endl << *poDQMFlagTxt;
  }

  // Try Trigger Child
  if( TriggerFlagTxt *poTriggerFlagTxt = 
        dynamic_cast<TriggerFlagTxt *>( roFLAG.getChild<TriggerFlagTxt>()) ) {

    roOut << std::endl << *poTriggerFlagTxt;
  }

  return roOut;
}
