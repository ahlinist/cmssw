// Package:    DTL1SimOperation
// Class:      DTL1SimOperation
//
// Original Author:  Ignazio Lazzizzera
//         Created:  Thu Jul 30 11:56:13 CEST 2009
// $Id: DTL1SimOperation.cc,v 1.1 2010/02/02 18:44:30 arose Exp $
//
//

#include "FWCore/PluginManager/interface/ModuleDef.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "L1TriggerConfig/DTTPGConfig/interface/DTConfigManager.h"
#include "L1TriggerConfig/DTTPGConfig/interface/DTConfigManagerRcd.h"

#include "SLHCUpgradeSimulations/L1Trigger/interface/DTL1SimOperation.h"

using namespace std;
using namespace edm;


//------------------------------------------------------------------------------
DTL1SimOperation::DTL1SimOperation(const edm::ParameterSet& pset):
  pSet(pset)
{

  //****************************************************************************  
  //    From cfg file
  // ***************************************************************************

  interestingToMe    = 
    pset.getUntrackedParameter<std::string>("interestingToMe");
  debug_bti          = 
    pset.getUntrackedParameter<bool>("debug_bti", true);
  debug_dtmatch      = 
    pset.getUntrackedParameter<bool>("debug_dtmatch", true);
  debug_global       = 
    pset.getUntrackedParameter<bool>("debug_global", true);
  debug_stubs        = 
    pset.getUntrackedParameter<bool>("debug_stubs", true);
  debug_dttrackmatch = 
    pset.getUntrackedParameter<bool>("debug_dttrackmatch", true);
  debug_dttrackmatch_extrapolation = 
    pset.getUntrackedParameter<bool>("debug_dttrackmatch_extrapolation", false);
  debug_dttrackmatch_extrapolation = 
    pset.getUntrackedParameter<bool>("debug_dttf", false);
  theGlobalMuonLabel = 
    pset.getUntrackedParameter<string>("TrackCollectionLabel");
  theSeedCollectionLabel = 
    pset.getUntrackedParameter<string>("MuonSeedCollectionLabel");
  USE_TSTheta =
    pset.getUntrackedParameter<bool>("use_TSTheta", false);

  //****************************************************************************
  // the outputs:
  theOutputDir = pset.getUntrackedParameter<string>("outputDir");
  if(theOutputDir != string(""))
    theOutputDir +=  string("/");
  theAsciiFileName = theOutputDir +
    pset.getUntrackedParameter<string>("asciiFileName");
  if(theAsciiFileName != string("")) {
    cout << "\nopening ascii file " << theAsciiFileName << endl;
    outAscii.open(theAsciiFileName.c_str());
  }
  //****************************************************************************
  theDTSectorFormat = true; // AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA!!!!!

}



//------------------------------------------------------------------------------
DTL1SimOperation::~DTL1SimOperation() {}




void DTL1SimOperation::Init(const edm::EventSetup& eventSetup)
{

  // Counters ******************************************************************
  EvtCounter = 0;
  theSimMuonsTotal = 0;
  // cout << "Init called" << endl;
  theTriggerOK = false;
  //*** get DTConfigManager 
  theTrigger = new DTTrig(pSet); // 1_8_4 confManager.product());
  ESHandle<DTConfigManager> confManager;
  try { 
    eventSetup.get<DTConfigManagerRcd>().get( confManager );
  } 
  catch(...) { 
    cout << "\nException getting DTConfigManagerRcd" << endl; 
    return;
  }
  theBXoffset = confManager->getBXOffset();
  theTrigger->createTUs(eventSetup);
  theTriggerOK = true;
  // cout << "the Trigger OK" << endl;

  //*** get digi geometry tracker **********************************************
  ESHandle<TrackerGeometry> trkGeomHandle;
  try {
    eventSetup.get<TrackerDigiGeometryRecord>().get(trkGeomHandle);
  }
  catch(...) {
    cout << "\nException from eventSetup.get for TrackerGeometry handle" << endl;
    return;
  }  
  theTracker = trkGeomHandle.product();
  // cout << "the Tracker OK" << endl;

  //*** get geometry tracker ***************************************************
  ESHandle<StackedTrackerGeometry> StackedTrackerGeomHandle;
  try {
    eventSetup.get<StackedTrackerGeometryRecord>().get(StackedTrackerGeomHandle);
  }
  catch(...) {
    cout << "\nException from eventSetup.get for StackedTrackerGeometry handle" << endl;
    return;
  }
  theStackedTracker = StackedTrackerGeomHandle.product();
  // cout << "the Stacked Trackers OK" << endl;
  /*
  StackedTrackerDetId id;
  for(StackedTrackerIterator = theStackedTracker->stacks().begin();
      StackedTrackerIterator != theStackedTracker->stacks().end();
      ++StackedTrackerIterator ) 
    cout << (id = (*StackedTrackerIterator)->Id()) << endl;
  */

  // get muon geometry: needed for TStheta trigger use
  eventSetup.get<MuonGeometryRecord>().get(muonGeom);

  return;
}



//------------------------------------------------------------------------------
int DTL1SimOperation::Do(edm::Event& event, const edm::EventSetup& eventSetup)
{

  BtiTrigs            = new BtiTrigsCollection();
  TSPhiTrigs          = new TSPhiTrigsCollection();
  TSThetaTrigs        = new TSThetaTrigsCollection();
  L1MuDTTracks        = new L1DTTracksCollection();
  DTStubMatches       = new DTStubMatchesCollection();
  DTSeededTracklets   = new DTSeededTrackletsCollection();

  DTStubMatches->clear(); // to be safe with counters!

  ++EvtCounter;
  
  if(theAsciiFileName != string("")) 
    outAscii << "\n===================================================\n"
	     << "   Run: " << event.id().run() << " Event: " << event.id().event() 
	     << "\n===================================================\n"
	     << endl;
  
  if((EvtCounter%10) == 0)
    cout << "current evt No " << EvtCounter << "\r" << flush;
  //  cout << endl;

  //****************************************************************************
  //   Trigger operations
  //****************************************************************************
  if(!theTriggerOK) 
    {
      cout << "\nInvalid L1Trigger reconstruction... exiting\n" << endl;
      return 1;
    }
  // Run Trigger reconstruction...
  theTrigger->triggerReco(event, eventSetup);
  // ...then get BTI - TS phi and theta matches and store in DTStubMatchesCollection;
  // also CMS (global) coordinates are set:
  getDTSimTrigger(event, eventSetup);
  // Next, order DT triggers in ascending order (the lower the best) by: 
  // 1.higher code, 2.lower phib
  DTStubMatches->orderDTTriggers();
  // Extrapolate each DTStubMatch to each Tracker layer
  DTStubMatches->extrapolateDTToTracker();
  // Get Tracker Global Stubs into DTStubMatchesCollection
  getTrackerGlobalStubs(event, eventSetup);
  // Find DT primitives - Tracker Stubs matches 
  getDTPrimitivesToTrackerStubsMatches();
  return 0;
}


// ------ method called once each job just after ending the event loop  ------------
void DTL1SimOperation::End_of_Operations() 
{
  cout << "\nend_of_all called" << endl;
  if (theTrigger != 0) delete theTrigger;
  cout << "*************************\ntotal of events " << EvtCounter << endl;
  //cout << "theSimMuonsTotal " << theSimMuonsTotal << endl;
  cout << "*************************" << endl;
  if(theAsciiFileName != string(""))
    outAscii.close();
  delete TSThetaTrigs;
}





