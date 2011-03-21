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
  use_TSTheta =
    pset.getUntrackedParameter<bool>("use_TSTheta", false);
  use_roughTheta =
    pset.getUntrackedParameter<bool>("use_roughTheta", false);
  //****************************************************************************
  // the outputs:
  theOutputDir = pset.getUntrackedParameter<string>("outputDir");
  if(theOutputDir != string(""))
    theOutputDir +=  string("/");

  theAsciiFileName = pset.getUntrackedParameter<string>("asciiFileName");
  if(theAsciiFileName != string("")) {
    theAsciiFileName += theOutputDir;// + theAsciiFileName
    // pset.getUntrackedParameter<string>("asciiFileName");
    cout << "\nopening ascii file " << theAsciiFileName << endl;
    outAscii.open(theAsciiFileName.c_str());
  }
	
  thePatternAsciiFileName = 
    pset.getUntrackedParameter<string>("patternAsciiFileName");
  if(thePatternAsciiFileName != string("")) {
    thePatternAsciiFileName += theOutputDir;
    cout << "\nopening file " << thePatternAsciiFileName << endl;
    patternAscii.open(thePatternAsciiFileName.c_str());
  }

  //****************************************************************************
  theDTSectorFormat = true; // AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA!!!!!

}



//------------------------------------------------------------------------------
DTL1SimOperations::~DTL1SimOperations() {

	
	// do anything here that needs to be done at desctruction time
	// (e.g. close files, deallocate resources etc.)

	
}




int DTL1SimOperations::InitDTL1SimOperations(const edm::EventSetup& eventSetup)
{
  //*** Initialize counters ****************************************************
  EvtCounter = 0;
  theSimMuonsTotal = 0;
  //*** get the DT trigger *****************************************************
  theDTTrigger = 0; // to have then a check that the trigger is created indeed
  theDTTriggerOK = false;
  theDTTrigger = new DTTrig(pSet); // 1_8_4 confManager.product());
  theDTTrigger->createTUs(eventSetup);
  if(theDTTrigger)
    theDTTriggerOK = true;
  //cout << "the DTTrigger OK" << endl;
  //*** get DTTrig and DTConfigManager *****************************************
  ESHandle<DTConfigManager> confManager;
  eventSetup.get<DTConfigManagerRcd>().get( confManager );
  theBXoffset = confManager->getBXOffset();

  /*
  cout << "DTConfigManagerRcd : Print some Config stuff" << endl;
  DTBtiId btiid(1,1,1,1,1);
  DTTracoId tracoid(1,1,1,1);
  DTChamberId chid(1,1,1);
  DTSectCollId scid(1,1);
  cout 	<< "BtiMap & TracoMap Size for chamber (1,1,1):" 
	<< confManager->getDTConfigBtiMap(chid).size() << " " 
	<< confManager->getDTConfigTracoMap(chid).size() << endl;
  confManager->getDTConfigBti(btiid)->print();
  confManager->getDTConfigTraco(tracoid)->print();
  confManager->getDTConfigTSTheta(chid)->print();
  confManager->getDTConfigTSPhi(chid)->print();
  */

  //*** get digi geometry tracker **********************************************
  // edm::Handle<edm::SimTrackContainer>,
  // edm::Handle<edm::SimVertexContainer>
  // the above are not needed! (Ignazio - Feb22.2010)
  theTracker=0;
  theTrackerOk = false;
  ESHandle<TrackerGeometry> trkGeomHandle;
  eventSetup.get<TrackerDigiGeometryRecord>().get(trkGeomHandle);
  theTracker = trkGeomHandle.product();
  if(theTracker)
  theTrackerOk = true;
  // cout << "the Tracker OK" << endl;

  //*** get geometry tracker ***************************************************
  theStackedTracker = 0;
  theStackedTrackerOk = false;
  ESHandle<StackedTrackerGeometry> StackedTrackerGeomHandle;
  eventSetup.get<StackedTrackerGeometryRecord>().get(StackedTrackerGeomHandle);
  theStackedTracker = StackedTrackerGeomHandle.product();
  if(theStackedTracker)
    theStackedTrackerOk = true;
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

  return 0;
}




//------------------------------------------------------------------------------
int DTL1SimOperations::DoDTL1SimOperations(edm::Event& event, 
					   const edm::EventSetup& eventSetup)
{
  /*
  ESHandle<MagneticField> bField_handle;
  eventSetup.get<IdealMagneticFieldRecord>().get(bField_handle);
  if(bField_handle.isValid()) {
    const MagneticField* theField = &(*bField_handle);
    GlobalPoint P = GlobalPoint(0.,0.,0.);
    cout << "-----------------> " << theField->inTesla(P).z() << " in Tesla" << endl;
  }
  */

  DTStubMatches->clear(); // to be safe with counters!

  ++EvtCounter;
    if(debug_tsphi || debug_bti || debug_tstheta){
       if(theAsciiFileName != string("")) 
          outAscii << "\n===================================================\n"
	           << "   Run: " << event.id().run() << " Event: " << event.id().event() 
	           << "\n===================================================\n"
	           << endl;
  
       if((EvtCounter%10) == 0)
           cout << "current evt No " << EvtCounter << "\r" << flush;
          // cout << endl;
     }
     
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
  // ...then get BTI - TS phi and theta matches and store in DTStubMatchesCollection;
  // moreover CMS (global) coordinates are also set:
  getDTSimTrigger(event, eventSetup);
  // Next, order DT triggers in ascending order (the lower the best) by: 
  // 1.higher code, 2.lower phib
  DTStubMatches->orderDTTriggers();
  // Extrapolate each DTStubMatch to each Tracker layer
  DTStubMatches->extrapolateDTToTracker();
  // Get rid of redundancies (in fact set a rejection flag)
  DTStubMatches->removeRedundantDTStubMatch();
  // Get Tracker Global Stubs, also setting a tracker Pt threshold flag
  getTrackerGlobalStubs(event, eventSetup);
  // Find Tracker Stubs matching selected DT primitives 
	getDTPrimitivesToTrackerStubsMatches();

  return 0;
}


// ------ method called once each job just after ending the event loop  ------------
void DTL1SimOperations::EndDTL1SimOperations() 
{
  //cout << "\nend_of_all called" << endl;
  if (theDTTrigger != 0) delete theDTTrigger;
  cout << "*************************\ntotal of events " << EvtCounter << endl;
  //cout << "theSimMuonsTotal " << theSimMuonsTotal << endl;
  cout << "*************************" << endl;
  if(theAsciiFileName != string(""))
    outAscii.close();
  if(thePatternAsciiFileName != string(""))
    patternAscii.close();
  delete TSThetaTrigs;
}




