#ifndef RecoLocalMuon_CSCRecHitComparator_H
#define RecoLocalMuon_CSCRecHitComparator_H

/** \class CSCRecHitReader
 *  Basic analyzer class which accesses 2D CSCRecHits
 *  and plot resolution comparing rechits produced using different 
 *  processes (e.g. CSCRecHit vs CSCRecHitB)
 *
 *  Author: D. Fortin  - UC Riverside
 */

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "DataFormats/Common/interface/Handle.h"

#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"

#include "DataFormats/MuonDetId/interface/CSCDetId.h"

#include "Geometry/CSCGeometry/interface/CSCGeometry.h"
#include <Geometry/CSCGeometry/interface/CSCChamber.h>
#include <Geometry/CSCGeometry/interface/CSCLayer.h>

#include "CSCRecHitHistograms.h"

#include <vector>
#include <map>
#include <string>
#include "TH1F.h"

namespace edm {
  class ParameterSet;
  class Event;
  class EventSetup;
}

class PSimHit;
class TFile;
class CSCLayer;
class CSCDetId;

class CSCRecHitComparator : public edm::EDAnalyzer {
public:
  /// Constructor
  CSCRecHitComparator(const edm::ParameterSet& pset);

  /// Destructor
  virtual ~CSCRecHitComparator();

  // Operations

  /// Perform the real analysis
  void analyze(const edm::Event & event, const edm::EventSetup& eventSetup);


protected:

private: 

  // Histograms
  H2DRecHit *hRHPME1a;
  H2DRecHit *hRHPME1b;
  H2DRecHit *hRHPME12;
  H2DRecHit *hRHPME13;
  H2DRecHit *hRHPME21;
  H2DRecHit *hRHPME22;
  H2DRecHit *hRHPME31;
  H2DRecHit *hRHPME32;
  H2DRecHit *hRHPME4;


  std::map<std::string, int> segMap1;
  std::map<std::string, int> chaMap1;


  // The file which will store the histos
  TFile *theFile;
  TH1F *heff0;
  // Switch for debug output
  bool debug;
  // Root file name
  std::string rootFileName;
  std::string simHitLabel;
  std::string recHitLabel;
  std::string stripdigiLabel;
  std::string wiredigiLabel;

  double maxRechitDistance;
  int maxRechitLayer;
  int maxSimhitLayer;

  int maxRechitLayerGanged;

  int WhichEndCap;

};


#endif




