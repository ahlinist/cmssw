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

// -- Yikes!
extern TAna00Event *gHFEvent;

using namespace std;
using namespace reco;
using namespace edm;


// ----------------------------------------------------------------------
HFDumpSignal::HFDumpSignal(const edm::ParameterSet& iConfig) :
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

  Handle<reco::MuonCollection> muons;
  iEvent.getByLabel( fMuonLabel.c_str(), muons);

  TAnaCand *pCand;
  int index=0;
  for (  reco::MuonCollection::const_iterator muon = muons->begin(); muon != muons->end(); ++ muon ) {
    
    pCand   = gHFEvent->addCand();  
    
    if (muon->charge()==1) pCand->fType  = 13;
    else if (muon->charge()==-1) pCand->fType  = -13;
    else  pCand->fType  = -9999;
      
    pCand->fMass  = mMuon;
  
    pCand->fPlab.SetPtEtaPhi(muon->pt(),
			     muon->eta(),
			     muon->phi()
			     ); 
    
    if (index==0) cout << "===> Signal " << endl;
    pCand->fSig1  = (muon->track()).index();
    pCand->dump();
    //cout << "track " << pCand->fSig1 <<  endl;
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
