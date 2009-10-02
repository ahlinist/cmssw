#ifndef _HFLAMBDAB2JPSILAMBDA_h_
#define _HFLAMBDAB2JPSILAMBDA_h_

#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaVertex.hh"
#include "DataFormats/VertexReco/interface/Vertex.h"

// ----------------------------------------------------------------------

class HFLambdaB2JpsiLambda : public edm::EDAnalyzer {
 public:
  explicit HFLambdaB2JpsiLambda(const edm::ParameterSet&);
  ~HFLambdaB2JpsiLambda();

  TAnaVertex DoVertexFit(std::vector<reco::Track> &Tracks);
  bool KineCut(reco::TrackCollection::const_iterator iTrack);
  bool massCut(TLorentzVector Track, double mass, double sigma = 0.4);
  
 private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  int           fVerbose; 
  std::string   fTracksLabel, fPrimaryVertexLabel;
  edm::InputTag fMuonsLabel;

  double        fMuonPt, fProtonPt, fPionPt, fDeltaR;

  reco::Vertex  fPV;

  edm::ESHandle<TransientTrackBuilder> fTTB;
};

#endif
