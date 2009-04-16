#include <iostream>

#include "HeavyFlavorAnalysis/InclB/interface/HFDumpSignal.h"

#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAna00Event.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaTrack.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TGenCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaVertex.hh"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

// -- Yikes!
extern TAna00Event *gHFEvent;

using namespace std;
using namespace reco;
using namespace edm;


// ----------------------------------------------------------------------
HFDumpSignal::HFDumpSignal(const edm::ParameterSet& iConfig) :
  fVerbose(iConfig.getUntrackedParameter<int>("verbose", 0)),
  fMuonLabel(iConfig.getUntrackedParameter<string>("muonLabel", string("globalMuons"))) {
  using namespace std;
  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- HFDumpSignal constructor" << endl;
  cout << "--- " << fMuonLabel.c_str() << endl;
  cout << "----------------------------------------------------------------------" << endl;

}


// ----------------------------------------------------------------------
HFDumpSignal::~HFDumpSignal() {
  
}


// ----------------------------------------------------------------------
void HFDumpSignal::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  nevt++;

  edm::Handle<reco::MuonCollection> muons;
  iEvent.getByLabel( fMuonLabel.c_str(), muons);

  TAnaTrack *pTrack;

  int index = 0;
  if (fVerbose > 0) cout << "==>HFDumpSignal> nMuons =" << muons->size() << endl;
  for (  reco::MuonCollection::const_iterator muon = muons->begin(); muon != muons->end(); ++ muon ) {

    pTrack            = gHFEvent->addSigTrack();

    pTrack->fMuType   = 0;
    pTrack->fMCID     = muon->charge()*-13; 
    pTrack->fMuID     = (muon->track()).index();
    pTrack->fIndex    = index;
    pTrack->fGenIndex = -1; 
    pTrack->fQ        = muon->charge();
    pTrack->fPlab.SetPtEtaPhi(muon->pt(),
			      muon->eta(),
			      muon->phi()
			      );
    
    if (fVerbose > 0) pTrack->dump(); 
    index++;
  }

}

// ------------ method called once each job just before starting event loop  ------------
void  HFDumpSignal::beginJob(const edm::EventSetup& setup) {
  nevt=0;
}

// ------------ method called once each job just after ending the event loop  ------------
void  HFDumpSignal::endJob() { 
  cout << "HFDumpSignal> Summary: Events processed: " << nevt << endl;
}

//define this as a plug-in
//DEFINE_FWK_MODULE(HFDumpSignal);
