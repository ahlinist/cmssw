
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
class HFDumpCandidate : public edm::EDAnalyzer {
 public:
  explicit HFDumpCandidate(const edm::ParameterSet&);
  ~HFDumpCandidate();
  
 private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  int GetSlowPion(const edm::Event& iEvent,TVector3 muonvector, int muontrack_index, int pion_index, int pion_charge);
  int GetWrongSignPion(const edm::Event& iEvent,TVector3 muonvector, int muontrack_index, int kaon_index, int pion_charge);

  int         fVerbose;
  double      fTrackPtMin;
  double      fDeltaR; 
  std::string fMuonLabel;
  std::string fTrackLabel; 
 
  
  int nevt;

};
