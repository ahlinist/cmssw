// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/27/07
// License: GPL

#include "CalibTracker/SiStripRunSummary/interface/DAQFlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/DAQFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/DCSFlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/DCSFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/DQMFlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/DQMFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/TECFlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/TECFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/TIDFlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/TIDFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/TIBFlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/TIBFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/TOBFlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/TOBFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/TriggerFlagTxt.h"
#include "CalibTracker/SiStripRunSummary/interface/TriggerFlagXML.h"

#include "CalibTracker/SiStripRunSummary/interface/getFlagTree.h"

GlobalFlagTxt getTXTFlagTree() {
  GlobalFlagTxt oGlobalFlag;
  oGlobalFlag.setComment( "This is Global Flag TXT");

  // DAQ
  if( FlagTxt *poDAQFlag = oGlobalFlag.createChild<DAQFlagTxt>()) {
    poDAQFlag->setComment( "GlobalFlagTxt -> DAQFlagTxt");

    if( FlagTxt *poFlag = poDAQFlag->createChild<TECFlagTxt>()) {
      poFlag->setState( Flag::OK);
      poFlag->setComment( "DAQFlagTxt -> TECFlagTxt");
    }

    if( FlagTxt *poFlag = poDAQFlag->createChild<TIBFlagTxt>()) {
      poFlag->setState( Flag::OK);
      poFlag->setComment( "DAQFlagTxt -> TIBFlagTxt");
    }

    if( FlagTxt *poFlag = poDAQFlag->createChild<TIDFlagTxt>()) {
      poFlag->setState( Flag::OK);
      poFlag->setComment( "DAQFlagTxt -> TIDFlagTxt");
    }

    if( FlagTxt *poFlag = poDAQFlag->createChild<TOBFlagTxt>()) {
      poFlag->setState( Flag::OK);
      poFlag->setComment( "DAQFlagTxt -> TOBFlagTxt");
    }
  } // End DAQ

  // DCS
  if( FlagTxt *poDCSFlag = oGlobalFlag.createChild<DCSFlagTxt>()) {
    poDCSFlag->setComment( "GlobalFlagTxt -> DCSFlagTxt");

    if( FlagTxt *poFlag = poDCSFlag->createChild<TECFlagTxt>()) {
      poFlag->setState( Flag::OK);
      poFlag->setComment( "DCSFlagTxt -> TECFlagTxt");
    }

    if( FlagTxt *poFlag = poDCSFlag->createChild<TIBFlagTxt>()) {
      poFlag->setState( Flag::OK);
      poFlag->setComment( "DCSFlagTxt -> TIBFlagTxt");
    }

    if( FlagTxt *poFlag = poDCSFlag->createChild<TIDFlagTxt>()) {
      poFlag->setState( Flag::OK);
      poFlag->setComment( "DCSFlagTxt -> TIDFlagTxt");
    }

    if( FlagTxt *poFlag = poDCSFlag->createChild<TOBFlagTxt>()) {
      poFlag->setState( Flag::OK);
      poFlag->setComment( "DCSFlagTxt -> TOBFlagTxt");
    }
  } // End DCS

  // DQM
  if( FlagTxt *poDQMFlag = oGlobalFlag.createChild<DQMFlagTxt>()) {
    poDQMFlag->setComment( "GlobalFlagTxt -> DQMFlagTxt");

    if( FlagTxt *poFlag = poDQMFlag->createChild<TECFlagTxt>()) {
      poFlag->setState( Flag::OK);
      poFlag->setComment( "DQMFlagTxt -> TECFlagTxt");
    }

    if( FlagTxt *poFlag = poDQMFlag->createChild<TIBFlagTxt>()) {
      poFlag->setState( Flag::OK);
      poFlag->setComment( "DQMFlagTxt -> TIBFlagTxt");
    }

    if( FlagTxt *poFlag = poDQMFlag->createChild<TIDFlagTxt>()) {
      poFlag->setState( Flag::OK);
      poFlag->setComment( "DQMFlagTxt -> TIDFlagTxt");
    }

    if( FlagTxt *poFlag = poDQMFlag->createChild<TOBFlagTxt>()) {
      poFlag->setState( Flag::OK);
      poFlag->setComment( "DQMFlagTxt -> TOBFlagTxt");
    }
  } // End DQM

  // Trigger
  if( FlagTxt *poTriggerFlag = oGlobalFlag.createChild<TriggerFlagTxt>()) {
    poTriggerFlag->setState( Flag::ERROR);
    poTriggerFlag->setComment( "GlobalFlagTxt -> TriggerFlagTxt");
  } // End Trigger

  return oGlobalFlag;
}

GlobalFlagXML getXMLFlagTree() {
  GlobalFlagXML oGlobalFlag;
  oGlobalFlag.setComment( "This is Global Flag XML");
  
  // DAQ
  if( FlagXML *poDAQFlag = oGlobalFlag.createChild<DAQFlagXML>()) {
    poDAQFlag->setComment( "GlobalFlagXML -> DAQFlagXML");

    if( FlagXML *poFlag = poDAQFlag->createChild<TECFlagXML>()) {
      poFlag->setState( Flag::OK);
      poFlag->setComment( "DAQFlagXML -> TECFlagXML");
    }

    if( FlagXML *poFlag = poDAQFlag->createChild<TIBFlagXML>()) {
      poFlag->setState( Flag::OK);
      poFlag->setComment( "DAQFlagXML -> TIBFlagXML");
    }

    if( FlagXML *poFlag = poDAQFlag->createChild<TIDFlagXML>()) {
      poFlag->setState( Flag::OK);
      poFlag->setComment( "DAQFlagXML -> TIDFlagXML");
    }

    if( FlagXML *poFlag = poDAQFlag->createChild<TOBFlagXML>()) {
      poFlag->setState( Flag::OK);
      poFlag->setComment( "DAQFlagXML -> TOBFlagXML");
    }
  } // End DAQ

  // DCS
  if( FlagXML *poDCSFlag = oGlobalFlag.createChild<DCSFlagXML>()) {
    poDCSFlag->setComment( "GlobalFlagXML -> DCSFlagXML");

    if( FlagXML *poFlag = poDCSFlag->createChild<TECFlagXML>()) {
      poFlag->setState( Flag::OK);
      poFlag->setComment( "DCSFlagXML -> TECFlagXML");
    }

    if( FlagXML *poFlag = poDCSFlag->createChild<TIBFlagXML>()) {
      poFlag->setState( Flag::OK);
      poFlag->setComment( "DCSFlagXML -> TIBFlagXML");
    }

    if( FlagXML *poFlag = poDCSFlag->createChild<TIDFlagXML>()) {
      poFlag->setState( Flag::OK);
      poFlag->setComment( "DCSFlagXML -> TIDFlagXML");
    }

    if( FlagXML *poFlag = poDCSFlag->createChild<TOBFlagXML>()) {
      poFlag->setState( Flag::OK);
      poFlag->setComment( "DCSFlagXML -> TOBFlagXML");
    }
  } // End DCS

  // DQM
  if( FlagXML *poDQMFlag = oGlobalFlag.createChild<DQMFlagXML>()) {
    poDQMFlag->setComment( "GlobalFlagXML -> DQMFlagXML");

    if( FlagXML *poFlag = poDQMFlag->createChild<TECFlagXML>()) {
      poFlag->setState( Flag::OK);
      poFlag->setComment( "DQMFlagXML -> TECFlagXML");
    }

    if( FlagXML *poFlag = poDQMFlag->createChild<TIBFlagXML>()) {
      poFlag->setState( Flag::OK);
      poFlag->setComment( "DQMFlagXML -> TIBFlagXML");
    }

    if( FlagXML *poFlag = poDQMFlag->createChild<TIDFlagXML>()) {
      poFlag->setState( Flag::OK);
      poFlag->setComment( "DQMFlagXML -> TIDFlagXML");
    }

    if( FlagXML *poFlag = poDQMFlag->createChild<TOBFlagXML>()) {
      poFlag->setState( Flag::OK);
      poFlag->setComment( "DQMFlagXML -> TOBFlagXML");
    }
  } // End DQM

  // Trigger
  if( FlagXML *poTriggerFlag = oGlobalFlag.createChild<TriggerFlagXML>()) {
    poTriggerFlag->setState( Flag::ERROR);
    poTriggerFlag->setComment( "GlobalFlagXML -> TriggerFlagXML");
  } // End Trigger

  return oGlobalFlag;
}
