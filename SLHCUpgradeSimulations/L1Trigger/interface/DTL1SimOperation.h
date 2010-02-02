// -*- C++ -*-
//
// Package:    DTL1SimOperation
// Class:      DTL1SimOperation
// 
/*
 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Ignazio Lazzizzera
//         Created:  Thu Jul 30 11:56:13 CEST 2009
// $Id$
//
//
#ifndef __DTL1SimOperation__
#define __DTL1SimOperation__

// system include files
#include <memory>

// Collaborating Class Header
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/GenericHandle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/TriggerNames.h"
          

// STL
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <iterator>
#include <vector>
#include <set>
#include <algorithm>
#include <string>
#include <iomanip>

//----------------------------------------------------------------------------------
#include "L1Trigger/DTTrigger/interface/DTTrig.h"
//#include "SLHCUpgradeSimulations/L1Trigger/interface/DTBtiTrigger.h"
#include "SimDataFormats/SLHC/interface/DTBtiTrigger.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTTrackCand.h"
#include "L1Trigger/DTTrackFinder/interface/L1MuDTTrack.h"

#include "SimDataFormats/SLHC/interface/StackedTrackerTypes.h"

#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/DTGeometry/interface/DTGeometry.h"
#include "Geometry/DTGeometry/interface/DTLayer.h"
#include "Geometry/DTGeometry/interface/DTTopology.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"

#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTTrackContainer.h"

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/SiPixelDigi/interface/PixelDigi.h"

#include "SLHCUpgradeSimulations/Utilities/interface/StackedTrackerGeometry.h"
#include "SLHCUpgradeSimulations/Utilities/interface/StackedTrackerGeometryRecord.h"
#include "SLHCUpgradeSimulations/Utilities/interface/StackedTrackerDetUnit.h"
#include "SLHCUpgradeSimulations/Utilities/interface/StackedTrackerDetId.h"
#include "SLHCUpgradeSimulations/Utilities/interface/classInfo.h" 

//#include "SLHCUpgradeSimulations/L1Trigger/interface/DTStubMatch.h"
//#include "SLHCUpgradeSimulations/L1Trigger/interface/DTTSPhiTrigger.h"
//#include "SLHCUpgradeSimulations/L1Trigger/interface/DTTSThetaTrigger.h"
//#include "SLHCUpgradeSimulations/L1Trigger/interface/DTTrackerStub.h"
//#include "SLHCUpgradeSimulations/L1Trigger/interface/DTStubMatchesCollection.h"
//#include "SLHCUpgradeSimulations/L1Trigger/interface/DTSeededTracklet.h"
#include "SimDataFormats/SLHC/interface/DTStubMatch.h"
#include "SimDataFormats/SLHC/interface/DTTSPhiTrigger.h"
#include "SimDataFormats/SLHC/interface/DTTSThetaTrigger.h"
#include "SimDataFormats/SLHC/interface/DTTrackerStub.h"
#include "SimDataFormats/SLHC/interface/DTStubMatchesCollection.h"
#include "SimDataFormats/SLHC/interface/DTSeededTracklet.h"


using namespace std;
//using namespace edm;
using namespace cmsUpgrades;

// *****************************************************************************

typedef 
std::map< DetId, vector<const GlobalStub_PixelDigi_ *> > DigiGlobalStubsMap_t;

// *****************************************************************************

typedef std::vector<const L1MuDTTrack*>  L1DTTracksCollection;

// *****************************************************************************


class DTL1SimOperation 
{
  
public:
  
  //----------------------------------------------------------------------------
  
  DTL1SimOperation(const edm::ParameterSet&);
  void Init(const edm::EventSetup&);
  int Do(edm::Event&, const edm::EventSetup&);
  void End_of_Operations();

  ~DTL1SimOperation();

  //----------------------------------------------------------------------------

  const edm::ParameterSet pSet;

  void getDTSimTrigger(edm::Event& event, const edm::EventSetup& eventSetup);

  bool match(DTBtiTrigger const bti, DTChambPhSegm const tsphi)
  {
    return (tsphi.wheel()  == bti.wheel() && 
	    tsphi.station()== bti.station() && 
	    tsphi.sector() == bti.sector() && 
	    tsphi.step()   == bti.step() && 
	    2              == bti.btiSL());
  }
  bool match(DTChambThSegm const tstheta, DTChambPhSegm const tsphi)
  {
    return (tsphi.wheel()  == tstheta.ChamberId().wheel() && 
	    tsphi.station()== tstheta.ChamberId().station() && 
	    tsphi.sector() == tstheta.ChamberId().sector() && 
	    tsphi.step()   == tstheta.step());
  }
  void getTrackerGlobalStubs(edm::Event& event, const edm::EventSetup& eventSetup);
  void getDTPrimitivesToTrackerStubsMatches();
  void setDTSeededTrackletRefCollection(edm::Event& event); 

protected:

  edm::ESHandle<DTGeometry> muonGeom;
  const TrackerGeometry* theTracker;  
  DTTrig* theTrigger;
  bool    theTriggerOK;
  // BX offset used to correct DTTPG output
  int     theBXoffset;
  // Sector Format Flag: true=[0-11] false=[1-12]
  bool    theDTSectorFormat;

  const StackedTrackerGeometry*    theStackedTracker;
  StackedTrackerGeometry::StackContainerIterator StackedTrackerIterator;
  DigiGlobalStubsMap_t DigiGlobalStubs;
  DigiGlobalStubsMap_t::const_iterator DigiGlobalStubsIter;
  // time to TDC_time conversion
  static const double TtoTDC;
 
  // -------- to control some debugging ------------------------------------
  std::string interestingToMe;

  // bool debug_tracks_and_vertices;
  bool debug_bti;
  bool debug_traco;
  bool debug_tsphi;
  bool debug_tstheta;
  bool USE_TSTheta;
  bool debug_dtmatch;
  bool debug_global;
  bool debug_stubs;
  bool debug_dttrackmatch;
  bool debug_dttrackmatch_extrapolation;
  bool debug_dttf;


  //----------- products ---------------------------------------------------
  BtiTrigsCollection*           BtiTrigs;
  TSPhiTrigsCollection*         TSPhiTrigs;
  TSThetaTrigsCollection*       TSThetaTrigs;
  DTStubMatchesCollection*      DTStubMatches;
  L1DTTracksCollection*         L1MuDTTracks;
  DTSeededTrackletsCollection*  DTSeededTracklets;

  // --------- useful member data ------------------------------------------
  std::ofstream outAscii; 
  // counters
  size_t EvtCounter; 
  size_t theSimMuonsTotal;
  // Outputs
  std::string theOutputDir;
  std::string theAsciiFileName; 
  std::string theRootFileName; 
  // Labels
  std::string theGlobalMuonLabel;
  std::string theSeedCollectionLabel;
};


#endif

