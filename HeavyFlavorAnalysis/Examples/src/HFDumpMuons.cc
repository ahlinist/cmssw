#include <iostream>

#include "HeavyFlavorAnalysis/Examples/interface/HFDumpMuons.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Common/interface/Handle.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

#include "DataFormats/TrackerRecHit2D/interface/SiTrackerGSRecHit2D.h" 

#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "DataFormats/TrackReco/interface/TrackExtraFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/MuonDetId/interface/MuonSubdetId.h"

#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAna01Event.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaTrack.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TGenCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaVertex.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaMuon.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TTrgObj.hh"

#include <TFile.h>
#include <TH1.h>

// -- Yikes!
extern TAna01Event *gHFEvent;
extern TFile       *gHFFile;

using namespace std;
using namespace edm;
using namespace reco;


// ----------------------------------------------------------------------
HFDumpMuons::HFDumpMuons(const edm::ParameterSet& iConfig):
  fMuonsLabel(iConfig.getUntrackedParameter<InputTag>("muonsLabel")),
  fVerbose(iConfig.getUntrackedParameter<int>("verbose", 0)),
  fDoTruthMatching(iConfig.getUntrackedParameter<int>("doTruthMatching", 1)) {
  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- HFDumpMuons constructor" << endl;
  cout << "---  fMuonsLabel:             " << fMuonsLabel.encode() << endl;
  cout << "---  fDoTruthMatching:        " << fDoTruthMatching << endl;  // 0 = nothing, 1 = TrackingParticles, 2 = FAMOS
  cout << "---  fVerbose:                " << fVerbose << endl;
  cout << "----------------------------------------------------------------------" << endl;
}


// ----------------------------------------------------------------------
HFDumpMuons::~HFDumpMuons() {

}


// ----------------------------------------------------------------------
void HFDumpMuons::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  // -- global muons
  Handle<MuonCollection> hMuons;
  cout << "==> HFDumpTracks> " << fMuonsLabel << endl;
  iEvent.getByLabel(fMuonsLabel, hMuons);

  for (reco::MuonCollection::const_iterator iMuon = hMuons->begin(); iMuon != hMuons->end();  iMuon++) {
    TrackRef gTrack = iMuon->globalTrack();
    TrackRef iTrack = iMuon->innerTrack();

    Track trkView(*gTrack);
    
    fillMuon(trkView, 1); 
  }

}



// ----------------------------------------------------------------------
void HFDumpMuons::fillMuon(const reco::Track& tr, int type) {

  //  TAnaMuon *pM = gHFEvent->addMuon();
  TAnaTrack *pM = gHFEvent->addRecTrack();    

  pM->fPlab.SetPtEtaPhi(tr.pt(), tr.eta(), tr.phi());

}

// ----------------------------------------------------------------------
vector<unsigned int> HFDumpMuons::muonStatHits(const reco::Track& tr) {
  vector<unsigned int> theMuonHits;
  unsigned int nRecHitDT(0), nRecHitCSC(0), nRecHitRPC(0);

  for (trackingRecHit_iterator recHit = tr.recHitsBegin(); recHit != tr.recHitsEnd(); ++recHit){
    DetId detIdHit = (*recHit)->geographicalId();
    if (detIdHit.det() == DetId::Muon ){
      if (detIdHit.subdetId() == MuonSubdetId::DT ) nRecHitDT++;
      else if (detIdHit.subdetId() == MuonSubdetId::CSC ) nRecHitCSC++;
      else if (detIdHit.subdetId() == MuonSubdetId::RPC ) nRecHitRPC++;
    }
  }
  
  theMuonHits.push_back(nRecHitDT); 
  theMuonHits.push_back(nRecHitCSC);
  theMuonHits.push_back(nRecHitRPC);
  return theMuonHits; 
}


// ------------ method called once each job just before starting event loop  ------------
void  HFDumpMuons::beginJob(const edm::EventSetup& setup) {

  gHFFile->cd();
  TH1D *h1 = new TH1D("h2", "h2", 20, 0., 20.);

}

// ------------ method called once each job just after ending the event loop  ------------
void  HFDumpMuons::endJob() {
}

//define this as a plug-in
//DEFINE_FWK_MODULE(HFDumpMuons);
