// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 05/31/07
// License: GPL

#include "CalibTracker/SiStripRunSummary/interface/FlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/FlagTxt.h"

// --[ FLAGXML ]---------------------------------------------------------------
//                                                  --[ PUBLIC ]--
FlagXML::~FlagXML() {
  // Memory Clean Up: remove all children
  for( MChildFlags::const_iterator oIter = oMChildFlags_.begin();
       oIter != oMChildFlags_.end();
       ++oIter) {

    delete oIter->second;
  }
}

bool FlagXML::setState( const State &reSTATE) {
  bool bResult = Flag::setState( reSTATE);

  // Update parent value only in case child state modified and parent exists.
  if( bResult && poParentFlag_) {
    poParentFlag_->updateState( reSTATE);
  }

  return bResult;
}

//                                                  --[ PROTECTED ]--
FlagXML::FlagXML( const FlagXML &roFLAGXML)
  : Flag( roFLAGXML)
{
  poParentFlag_ = 0;

  // Clone all children.
  for( MChildFlags::const_iterator oIter = roFLAGXML.oMChildFlags_.begin();
       oIter != roFLAGXML.oMChildFlags_.end();
       ++oIter) {

    FlagXML *poFlagClone = dynamic_cast<FlagXML *>( oIter->second->clone());

    // Parent will be set by addChild(...)
    addChild( poFlagClone);
  }
}

FlagXML::FlagXML( const FlagTxt &roFLAGTXT)
  : Flag( roFLAGTXT),
    poParentFlag_( 0) {

  // Take care of Children
  for( FlagTxt::MChildFlags::const_iterator oIter = 
         roFLAGTXT.oMChildFlags_.begin();
       oIter != roFLAGTXT.oMChildFlags_.end();
       ++oIter) {

    FlagXML *poNewChild = dynamic_cast<FlagXML *>( oIter->second->cloneXML());

    // Parent will be set by addChild(...)
    addChild( poNewChild);
  }
}

//                                                  --[ PRIVATE ]--
void FlagXML::updateState( const State &reCHILD_STATE) {
  switch( reCHILD_STATE) {
    case UNKNOWN:
      // Do nothing: Unknown state does not affect child nor parent
      break;
    case ERROR:
      // Child changed to ERROR state
      if( ERROR != eState_) {
        setState( ERROR);
      }
      break;
    case OK:
    {
      // Child changed to OK state
      State eNewState = OK;

      // Loop over children and check if all of them are OK now or UNKNOWN.
      for( MChildFlags::const_iterator oIter = oMChildFlags_.begin();
           oIter != oMChildFlags_.end();
           ++oIter) {

        if( ERROR == oIter->second->getState()) {
          eNewState = ERROR;

          // One ERROR child flag is enough for parent to be set into the same
          // state
          break;
        }
      } // End loop over children

      // So, did state change?
      if( eNewState != eState_) {
        setState( eNewState);
      }
      break;
    }
    default:
      // Unsupported State
      break;
  }
}

void FlagXML::addChild( FlagXML *poSubFlag) {
  poSubFlag->poParentFlag_ = this;
  oMChildFlags_.insert( std::make_pair( poSubFlag->getID(), poSubFlag));
}
