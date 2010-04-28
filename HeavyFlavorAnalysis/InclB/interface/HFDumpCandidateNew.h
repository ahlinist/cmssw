
// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "HeavyFlavorAnalysis/InclB/rootio/TAna00Event.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TAnaTrack.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TAnaCand.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TGenCand.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TAnaVertex.hh"

class TFile;
class TTree;
class TAna00Event;



// ----------------------------------------------------------------------
class HFDumpCandidateNew : public edm::EDAnalyzer {
 public:
  explicit HFDumpCandidateNew(const edm::ParameterSet&);
  ~HFDumpCandidateNew();
  
 private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  int GetSlowPion(const edm::Event& iEvent,TVector3 muonvector, int muontrack_index, int pion_index, int pion_charge);
  int GetHighestPtGlobalMuon(const edm::Event& iEvent,double ptmin_muon, double eta_muon);

  int         fVerbose;
  double      fTrackPtMin;
  double      fMuonPtMin;
  double      fMuonEta;
  double      fMassLow;
  double      fMassHigh;
  double      fChi2;
  double      fDeltaR; 
  std::string fMuonLabel;
  std::string fTrackLabel; 
 
  
  int nevt;

};
