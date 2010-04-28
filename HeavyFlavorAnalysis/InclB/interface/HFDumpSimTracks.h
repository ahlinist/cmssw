#ifndef _HFDUMPSIMTRACKS_h_
#define _HFDUMPSIMTRACKS_h_

#include <memory>
#include <utility>
#include <vector>
#include <memory>
#include <algorithm>
#include <map>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"


#include <Math/DistFunc.h>
#include "Math/Vector3D.h"
#include "TMath.h"

class TFile;
class TTree;
class TAna00Event;

class TrackAssociatorBase;

// ----------------------------------------------------------------------
class HFDumpSimTracks : public edm::EDAnalyzer {
 public:
  explicit HFDumpSimTracks(const edm::ParameterSet&);
  ~HFDumpSimTracks();
  
 private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  std::string          fTracksLabel; 

  typedef math::XYZPoint Point; 
  void selectVertices ( const reco::VertexCollection &vtxs, std::vector<Point> &points);
  
  int                  nevt; 
  int                  fVerbose;

  TrackAssociatorBase *fAssociator;
};

#endif
