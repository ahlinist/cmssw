#include <iostream>

#include "HeavyFlavorAnalysis/Examples/interface/HFDumpSignal.h"

#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAna00Event.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaTrack.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TGenCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaVertex.hh"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/Candidate/interface/Candidate.h"

// -- Yikes!
extern TAna00Event *gHFEvent;

using namespace std;
using namespace reco;
using namespace edm;


// ----------------------------------------------------------------------
HFDumpSignal::HFDumpSignal(const edm::ParameterSet& iConfig) :
  fTracksLabel(iConfig.getUntrackedParameter<string>("tracksLabel", string("goodTracks"))) {
  using namespace std;
  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- HFDumpSignal constructor" << endl;
  cout << "--- " << fTracksLabel.c_str() << endl;
  cout << "----------------------------------------------------------------------" << endl;

}


// ----------------------------------------------------------------------
HFDumpSignal::~HFDumpSignal() {
  
}


// ----------------------------------------------------------------------
void HFDumpSignal::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  Handle<reco::CandidateCollection> trackCands;
  iEvent.getByLabel(fTracksLabel.c_str(), trackCands);

  for (unsigned int i = 0; i < trackCands->size(); ++i) {
    const Candidate &cand = (*trackCands)[i];
    cout << i << "  " << cand.pt() << endl;
  }

}

// ------------ method called once each job just before starting event loop  ------------
void  HFDumpSignal::beginJob(const edm::EventSetup& setup) {
}

// ------------ method called once each job just after ending the event loop  ------------
void  HFDumpSignal::endJob() {
}

//define this as a plug-in
//DEFINE_FWK_MODULE(HFDumpSignal);
