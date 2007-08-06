// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/27/07
// License: GPL

#include <ostream>

#include "CalibTracker/SiStripRunSummary/interface/ClassID.h"

// Include all subflags that current one may have
#include "CalibTracker/SiStripRunSummary/interface/DAQFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/DCSFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/DQMFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/TriggerFlagXML.h"

#include "CalibTracker/SiStripRunSummary/interface/GlobalFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/GlobalFlagTxt.h"

// --[ GLOBAL FLAG XML ]-------------------------------------------------------
//                                                  --[ PUBLIC ]--
GlobalFlagXML::GlobalFlagXML( const GlobalFlagTxt *poGLOBAL_FLAGTXT)
  : FlagXML( *( dynamic_cast<const FlagTxt *>( poGLOBAL_FLAGTXT) ) ) {}

//                                                  --[ PROTECTED ]--
Clonable *GlobalFlagXML::cloneTxt() const {
  return new GlobalFlagTxt( this);
}

ClassIDBase::ID GlobalFlagXML::getID() const {
  return ClassID<GlobalFlagXML>::get();
}

int GlobalFlagXML::isChildValid( const FlagXML *poCHILD_CANDIDATE) const {
  return dynamic_cast<const DAQFlagXML     *>( poCHILD_CANDIDATE) ||
         dynamic_cast<const DCSFlagXML     *>( poCHILD_CANDIDATE) ||
         dynamic_cast<const DQMFlagXML     *>( poCHILD_CANDIDATE) ||
         dynamic_cast<const TriggerFlagXML *>( poCHILD_CANDIDATE) ?
         1 :
         0;
}

// --[ HELPER FUNCTIONS ]------------------------------------------------------
std::ostream &
  operator <<( std::ostream &roOut, const GlobalFlagXML &roFLAG) {

  // Print Global info: use Flag operator <<
  roOut << dynamic_cast<const Flag &>( roFLAG);

  // Try DAQ Child
  if( DAQFlagXML *poDAQFlagXML = 
        dynamic_cast<DAQFlagXML *>( roFLAG.getChild<DAQFlagXML>()) ) {

    roOut << std::endl << *poDAQFlagXML;
  }

  // Try DCS Child
  if( DCSFlagXML *poDCSFlagXML = 
        dynamic_cast<DCSFlagXML *>( roFLAG.getChild<DCSFlagXML>()) ) {

    roOut << std::endl << *poDCSFlagXML;
  }

  // Try DQM Child
  if( DQMFlagXML *poDQMFlagXML = 
        dynamic_cast<DQMFlagXML *>( roFLAG.getChild<DQMFlagXML>()) ) {

    roOut << std::endl << *poDQMFlagXML;
  }

  // Try Trigger Child
  if( TriggerFlagXML *poTriggerFlagXML = 
        dynamic_cast<TriggerFlagXML *>( roFLAG.getChild<TriggerFlagXML>()) ) {

    roOut << std::endl << *poTriggerFlagXML;
  }

  return roOut;
}
