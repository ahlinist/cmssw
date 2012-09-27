// Package:    DTL1SimOperation
// Class:      DTL1SimOperation
//
// Original Author:  Ignazio Lazzizzera
//         Created:  Thu Jul 30 11:56:13 CEST 2009
// $Id$
//
//

#include "FWCore/PluginManager/interface/ModuleDef.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "L1TriggerConfig/DTTPGConfig/interface/DTConfigManager.h"
#include "L1TriggerConfig/DTTPGConfig/interface/DTConfigManagerRcd.h"

#include "SLHCUpgradeSimulations/L1DTTrigger/interface/DTL1SimOperations.h"

using namespace std;
using namespace edm;


//------------------------------------------------------------------------------
DTL1SimOperations::DTL1SimOperations(const edm::ParameterSet& pset):
  pSet(pset)
{
  //****************************************************************************  
  //    From cfg file
  // ***************************************************************************
  interestingToMe    = 
    pset.getUntrackedParameter<std::string>("interestingToMe");
  debug_bti          = -
    pset.getUntrackedParameter<bool>("debug_bti", true);
  debug_tsphi          = -
    pset.getUntrackedParameter<bool>("debug_tsphi", true);
  debug_tstheta          = -
    pset.getUntrackedParameter<bool>("debug_tstheta", true);
  debug_dtmatch      = 
    pset.getUntrackedParameter<bool>("debug_dtmatch", true);
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
  use_TSTheta =
    pset.getUntrackedParameter<bool>("use_TSTheta", false);
  use_roughTheta =
    pset.getUntrackedParameter<bool>("use_roughTheta", false);
  some_more_debug = 
    pset.getUntrackedParameter<bool>("some_more_debug", false);
  even_more_debug = 
    pset.getUntrackedParameter<bool>("even_more_debug", false);
  debug = // needed by DTTrig stuff: required!!! (Ignazio)
    pset.getUntrackedParameter<bool>("debug");

  // the outputs: **************************************************************
  theOutputDir = pset.getUntrackedParameter<string>("outputDir", "");
  if(theOutputDir != string("") && theOutputDir[theOutputDir.size()] != '/')
    theOutputDir +=  string("/");

  theAsciiFileName = pset.getUntrackedParameter<string>("asciiFileName", "");
  if(theAsciiFileName != string("")) {
    theAsciiFileName = theOutputDir + theAsciiFileName;
    // cout << "\nopening ascii file " << theAsciiFileName << endl;
    outAscii.open(theAsciiFileName.c_str());
  }
  // PLZ 1/12/2009 begin *******************************************************
  // define output root file
  theRootFileNameHis =  pset.getUntrackedParameter<string>("rootFileNameHis", "");    
  if(theRootFileNameHis != string("")) {
    theRootFileNameHis = theOutputDir + theRootFileNameHis;
    cout << "\nAdditional histos " << theRootFileNameHis << endl;
    book_histos();
  }	
  // PLZ end *******************************************************************
		
  thePatternAsciiFileName = 
    pset.getUntrackedParameter<string>("patternAsciiFileName", "");
  if(thePatternAsciiFileName != string("")) {
    thePatternAsciiFileName = theOutputDir + thePatternAsciiFileName;
    // cout << "\nopening file " << thePatternAsciiFileName << endl;
    patternAscii.open(thePatternAsciiFileName.c_str());
  }

  cout << endl;
  //****************************************************************************
  theDTSectorFormat = true; // AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA!!!!!
}



//------------------------------------------------------------------------------
DTL1SimOperations::~DTL1SimOperations() {
  
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}



//------------------------------------------------------------------------------
int DTL1SimOperations::InitDTL1SimOperations(const edm::EventSetup& eventSetup)
{
  //*** Initialize counters ****************************************************
  EvtCounter = 0;
  theSimMuonsTotal = 0;
  //*** get the DT trigger *****************************************************
  theDTTrigger = 0; 
  theDTTriggerOK = false;
  theDTTrigger = new DTTrig(pSet); 
  theDTTrigger->createTUs(eventSetup);
  if(theDTTrigger) 
    theDTTriggerOK = true;
  if(some_more_debug)
    cout << (theDTTriggerOK? "theDTTrigger OK": "error: theDTTrigger") << endl;
  //*** get DTConfigManager ****************************************************
  ESHandle<DTConfigManager> confManager;
  eventSetup.get<DTConfigManagerRcd>().get( confManager );
  theBXoffset = confManager->getBXOffset();
  if(even_more_debug) {
    cout << "DTConfigManagerRcd : Print some Config stuff" << endl;
    DTBtiId btiid(1,1,1,1,1);
    DTTracoId tracoid(1,1,1,1);
    DTChamberId chid(1,1,1);
    DTSectCollId scid(1,1);
    cout << "BtiMap & TracoMap Size for chamber (1,1,1):" 
	 << confManager->getDTConfigBtiMap(chid).size() << " " 
	 << confManager->getDTConfigTracoMap(chid).size() << endl;
    confManager->getDTConfigBti(btiid)->print();
    confManager->getDTConfigTraco(tracoid)->print();
    confManager->getDTConfigTSTheta(chid)->print();
    confManager->getDTConfigTSPhi(chid)->print();
  }
  //*** get digi geometry tracker ***********************************************
  theTracker=0;
  theTrackerOk = false;
  ESHandle<TrackerGeometry> trkGeomHandle;
  eventSetup.get<TrackerDigiGeometryRecord>().get(trkGeomHandle);
  theTracker = trkGeomHandle.product();
  if(theTracker) 
    theTrackerOk = true;
  if(some_more_debug)
    cout << (theTrackerOk ? "theTracker OK" : "error: theTracker") << endl;
  //*** get geometry of the tracker *********************************************
  theStackedTracker = 0;
  theStackedTrackerOk = false;
  ESHandle<StackedTrackerGeometry> StackedTrackerGeomHandle;
  eventSetup.get<StackedTrackerGeometryRecord>().get(StackedTrackerGeomHandle);
  theStackedTracker = StackedTrackerGeomHandle.product();
  if(theStackedTracker)
    theStackedTrackerOk = true;
  if(some_more_debug) {
    cout 
      << (theStackedTrackerOk ? "theStackedTrackers OK" : "error theStackedTrackers")
      << endl;
    if(theStackedTrackerOk && even_more_debug) {
      StackedTrackerDetId id;
      for(StackedTrackerIterator = theStackedTracker->stacks().begin();
	  StackedTrackerIterator != theStackedTracker->stacks().end();
	  ++StackedTrackerIterator ) 
	cout << (id = (*StackedTrackerIterator)->Id()) << endl;
    }
  } 
  // get muon geometry: needed for the TStheta trigger
  eventSetup.get<MuonGeometryRecord>().get(muonGeom);
  cout << endl;
  return 0;
}




//------------------------------------------------------------------------------
int DTL1SimOperations::DoDTL1SimOperations(edm::Event& event, 
					   const edm::EventSetup& eventSetup)
{
  if(even_more_debug) {
    ESHandle<MagneticField> bField_handle;
    eventSetup.get<IdealMagneticFieldRecord>().get(bField_handle);
    if(bField_handle.isValid()) {
      const MagneticField* theField = &(*bField_handle);
      GlobalPoint P = GlobalPoint(0.,0.,0.);
      cout << "-----------------> " << theField->inTesla(P).z() << " in Tesla" << endl;
    }
    else 
      cout << "Unable to get valid bField_handle" << endl;
  } // end if even_more_debug

  DTMatches->clear(); // to be safe with counters!

  ++EvtCounter;
  
  if(theAsciiFileName != string("")) 
    outAscii << "\n===================================================\n"
	     << "   Run: " << event.id().run() << " Event: " << event.id().event() 
	     << "\n===================================================\n"
	     << endl;

  if((EvtCounter%10) == 0)
    cout << "Current evt No " << EvtCounter << "\r" << flush;

  //****************************************************************************
  //   Trigger operations
  //****************************************************************************
  if(!theDTTriggerOK) 
    {
      cout << "\nInvalid L1Trigger reconstruction... exiting\n" << endl;
      return 1;
    }
  // Run the whole Trigger reconstruction...
  theDTTrigger->triggerReco(event, eventSetup);
  // ...then get BTI - TS phi and theta matches and store in DTMatchesCollection;
  // moreover CMS (global) coordinates are also set:
  getDTSimTrigger(event, eventSetup);
  // Next, order DT triggers in ascending order (the lower the best) by: 
  // 1.higher code, 2.lower phib
  DTMatches->orderDTTriggers();
  // Extrapolate each DTMatch to each Tracker layer
  DTMatches->extrapolateDTToTracker();
  // Get rid of redundancies (in fact set a rejection flag)
  DTMatches->removeRedundantDTMatch();
  // Get Tracker Global Stubs, also setting a tracker Pt threshold flag
  getTrackerGlobalStubs(event, eventSetup);
  // Get Tracker Tracklets, also setting a tracker Pt threshold flag
  getTrackerTracklets(event, eventSetup);
  // Get Tracker L1 Tracks
  getTrackerTracks(event, eventSetup);
  // Find Tracker Objects matching selected DT primitives 
  getDTPrimitivesToTrackerObjectsMatches();
  
  //PLZ begin
  // Analyze DT primitives - Tracker Stubs matches 
  if(theRootFileNameHis != "")
    analyze_DTTPGStubMatches(event, eventSetup, theRootFileNameHis);
  //PLZ end

  return 0;
}
 

// ------ method called once each job just after ending the event loop  ------------
void DTL1SimOperations::EndDTL1SimOperations() 
{
  if(theRootFileNameHis != "") 
    write_out_histos(theRootFileNameHis);
  if(some_more_debug) 
    cout << "\nend_of_all called" << endl;
  if (theDTTrigger != 0) delete theDTTrigger;
  cout << "*************************\ntotal of events " << EvtCounter << endl;
  if(some_more_debug) 
    cout << "theSimMuonsTotal " << theSimMuonsTotal << endl;
  cout << "*************************" << endl;
  if(theAsciiFileName != string(""))
    outAscii.close();
  if(thePatternAsciiFileName != string(""))
    patternAscii.close();
  delete TSThetaTrigs;
}




