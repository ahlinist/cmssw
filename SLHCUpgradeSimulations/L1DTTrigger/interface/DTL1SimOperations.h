// -*- C++ -*-
//
// Package:    DTL1SimOperations
// Class:      DTL1SimOperations
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
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/GenericHandle.h"
#include "FWCore/Framework/interface/ESHandle.h"

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

#include "SimDataFormats/SLHC/interface/StackedTrackerTypes.h" // 4_2_3 (Ignazio)
#include "SimDataFormats/SLHC/interface/L1TkTrack.h"
#include "SimDataFormats/SLHC/interface/L1TkTracklet.h"
#include "SimDataFormats/SLHC/interface/L1TkStub.h"


#include "SimDataFormats/SLHC/interface/DTMatch.h"
#include "SimDataFormats/SLHC/interface/DTTSPhiTrigger.h"
#include "SimDataFormats/SLHC/interface/DTTSThetaTrigger.h"
#include "SimDataFormats/SLHC/interface/DTTrackerStub.h"
#include "SimDataFormats/SLHC/interface/DTTrackerTracklet.h"
#include "SimDataFormats/SLHC/interface/DTTrackerTrack.h"
#include "SimDataFormats/SLHC/interface/DTMatchesCollection.h"
#include "SimDataFormats/SLHC/interface/DTSeededStubTrack.h"

#include "SimDataFormats/Track/interface/SimTrackContainer.h"

#include "SLHCUpgradeSimulations/L1DTTrigger/interface/DTL1OnTheFlyHistos.h"

using namespace std;
//using namespace edm;
using namespace cmsUpgrades;

// *****************************************************************************

typedef map<DetId, vector<const L1TkStub_PixelDigi_ *> > DigiGlobalStubsMap_t;

typedef map<DetId, vector<const L1TkTracklet_PixelDigi_ *> > DigiTrackletsMap_t;

typedef map<DetId, vector<const L1TkTrack_PixelDigi_ *> > DigiTracksMap_t;


// *****************************************************************************

typedef std::vector<const L1MuDTTrack*>  L1DTTracksCollection;

// *****************************************************************************


class DTL1SimOperations: public DTL1OnTheFlyHistos
{

  // 6.5.2010 PLZ : to use Stacked Tracker PTFlag 
  // 21.10.2011 Ignazio to 4_2_3
  typedef L1TkStub< Ref_PixelDigi_ >  GlobalStubRefType;
  typedef L1TkTracklet< Ref_PixelDigi_ > TrackletRefType;
  typedef L1TkTrack< Ref_PixelDigi_ > TrackRefType;

 public:

  //----------------------------------------------------------------------------
  
  DTL1SimOperations(const edm::ParameterSet&);
  int InitDTL1SimOperations(const edm::EventSetup&);
  int DoDTL1SimOperations(edm::Event&, const edm::EventSetup&);
  void EndDTL1SimOperations();

  ~DTL1SimOperations();

  //----------------------------------------------------------------------------

  const edm::ParameterSet pSet;

  void getDTSimTrigger(edm::Event& event, const edm::EventSetup& eventSetup);

  bool match(DTBtiTrigger const bti, DTChambPhSegm const tsphi) {
    return (tsphi.wheel()  == bti.wheel() && 
	    tsphi.station()== bti.station() && 
	    tsphi.sector() == bti.sector() && 
	    tsphi.step()   == bti.step() && 
	    2              == bti.btiSL());
  }

  bool match(DTChambThSegm const tstheta, DTChambPhSegm const tsphi) {
    return (tsphi.wheel()  == tstheta.ChamberId().wheel() && 
	    tsphi.station()== tstheta.ChamberId().station() && 
	    tsphi.sector() == tstheta.ChamberId().sector() && 
	    tsphi.step()   == tstheta.step());
  }

  bool flagStubThreshold(const GlobalStubRefType& aStub,  
			 const cmsUpgrades::StackedTrackerGeometry* theStackedTracker, 
			 double mMagneticFieldStrength, 
			 double mPtThreshold ); /* 
						   This is not needed starting 
						   with 4_2_3 any more 
						*/
  // Notice: starting from 4_2_3 "Global" just refers to CMS (gloabal) coordinates!
  void getTrackerGlobalStubs(edm::Event& event, const edm::EventSetup& eventSetup);
  void getTrackerTracklets(edm::Event& event, const edm::EventSetup& eventSetup);
  void getTrackerTracks(edm::Event& event, const edm::EventSetup& eventSetup);
  void getDTPrimitivesToTrackerObjectsMatches();
  void getClosestTrackerStub(DTMatch* dtmatch, int dm);
  void setDTSeededStubTracks();
  void getClosestTrackerTracklet(DTMatch* dtmatch, int dm);
  void getMatchingTrackerTrack(DTMatch* dtmatch, int dm);
  void setDTSeededTrackletRefCollection(edm::Event& event); 
  void choosePtValue(); // PZ 100513
  void assignPtBin();   // PZ 100513
  
  //PLZ begin
  void analyze_DTTPGStubMatches(edm::Event& event, 
				const edm::EventSetup& eventSetup, 
				string &theRootFileNameHis);
  //PLZ end
  
 protected:

  int InitError;
  edm::ESHandle<DTGeometry> muonGeom;
  DTTrig* theDTTrigger;
  bool    theDTTriggerOK;
  const TrackerGeometry* theTracker;
  bool    theTrackerOk;
  // BX offset used to correct DTTPG output
  int     theBXoffset;
  // Sector Format Flag: true=[0-11] false=[1-12]
  bool    theDTSectorFormat;
  const StackedTrackerGeometry* theStackedTracker;
  bool    theStackedTrackerOk;
  StackedTrackerGeometry::StackContainerIterator StackedTrackerIterator;
  DigiGlobalStubsMap_t DigiGlobalStubs; 
  DigiGlobalStubsMap_t::const_iterator DigiGlobalStubsIter;
  // time to TDC_time conversion 
  static const double TtoTDC;
 
  bool use_TSTheta;
  bool use_roughTheta;
  // -------- to control some debugging ------------------------------------
  std::string interestingToMe;
  // bool debug_tracks_and_vertices;
  bool debug_bti;
  bool debug_traco;
  bool debug_tsphi;
  bool debug_tstheta; 
  bool debug_dtmatch;
  bool debug_stubs;
  bool debug_dttrackmatch;
  bool debug_dttrackmatch_extrapolation;
  bool debug_dttf;
  bool even_more_debug;
  bool debug;  // needed by DTTrig stuff: required!!!
  //----------- products ---------------------------------------------------

  BtiTrigsCollection*            BtiTrigs;
  TSPhiTrigsCollection*          TSPhiTrigs;
  TSThetaTrigsCollection*        TSThetaTrigs;
  DTMatchesCollection*       DTMatches;
  L1DTTracksCollection*          L1MuDTTracks;
  DTSeededStubTracksCollection*  DTSeededStubTracks;

  // --------- useful data member ------------------------------------------
  std::ofstream outAscii, patternAscii; 
  // counters
  size_t EvtCounter; 
  size_t theSimMuonsTotal;

  // Outputs
  std::string theOutputDir;
  std::string theAsciiFileName; 
  std::string theRootFileName; 
  std::string thePatternAsciiFileName;
  //PLZ 1/12/2009 - begin
  std::string theRootFileNameHis; 
  //end
  
  // Labels
  std::string theGlobalMuonLabel;
  std::string theSeedCollectionLabel;

  //----------------------------------------------------------------------------
  struct LessById 
  { 
    // From pat::GenPlusSimParticleProducer,
    // used to sort a SimTrackContainer object and alike.
    bool operator()(const SimTrack &tk1, const SimTrack &tk2) const { 
      return (tk1.trackId() < tk2.trackId()); 
    }
    bool operator()(const SimTrack &tk1, unsigned int id ) const { 
      return (tk1.trackId() < id);            
    }
    bool operator()(unsigned int id, const SimTrack &tk2) const { 
      return (id < tk2.trackId()); 
    }
  };
  //----------------------------------------------------------------------------
};


#endif

