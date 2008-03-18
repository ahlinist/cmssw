#include <iostream>

#include "HeavyFlavorAnalysis/Examples/interface/HFDumpSignal.h"

#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAna00Event.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaTrack.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TGenCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaVertex.hh"

#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "CommonTools/Statistics/interface/ChiSquared.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/TrackReco/interface/TrackExtraFwd.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"


// -- Yikes!
extern TAna00Event *gHFEvent;

using namespace std;
using namespace reco;
using namespace edm;

#define MMUON 0.10566
#define MKAON 0.49368

// ----------------------------------------------------------------------
HFDumpSignal::HFDumpSignal(const edm::ParameterSet& iConfig) :
  fVerbose(iConfig.getUntrackedParameter<int>("verbose", 0)),
  fTracksLabel(iConfig.getUntrackedParameter<string>("tracksLabel", string("goodTracks"))), 
  // fMuonsLabel(iConfig.getUntrackedParameter<string>("muonsLabel", string("goodTracks"))) {
  fMuonsLabel(iConfig.getUntrackedParameter<edm::InputTag>("muonsLabel")) {
  using namespace std;
  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- HFDumpSignal constructor" << endl;
  cout << "--- tracksLabel:            " << fTracksLabel.c_str() << endl;
  cout << "--- muonsLabel:             " << fMuonsLabel << endl;
  cout << "----------------------------------------------------------------------" << endl;

}


// ----------------------------------------------------------------------
HFDumpSignal::~HFDumpSignal() {
  
}


// ----------------------------------------------------------------------
void HFDumpSignal::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  cout << "hallo" << endl;

  // -- get the collection of muons
  Handle<reco::MuonCollection> hMuons;
  iEvent.getByLabel(fMuonsLabel, hMuons);

  // -- get the collection of tracks
  Handle<reco::TrackCollection> hTracks;
  iEvent.getByLabel(fTracksLabel.c_str(), hTracks);  

  std::vector<const reco::Track*> recTracks; 
  TAnaTrack *pTrack;

  cout << "echo" << endl;
  
  // -- Look at muons
  if (fVerbose > 0) cout << "===> Muons " << endl;
  const reco::Track* tt = 0;
  for (reco::MuonCollection::const_iterator muon = hMuons->begin(); muon != hMuons->end(); ++muon) {

    TrackRef glbTrack = muon->combinedMuon();
    tt = &(*glbTrack);
    
    pTrack   = gHFEvent->addSigTrack();  

    pTrack->fQ = tt->charge();
    pTrack->fPlab.SetPtEtaPhi(tt->pt(),
			      tt->eta(),
			      tt->phi()
			      ); 
    
    pTrack->fIndex  = (muon->track()).index();

    recTracks.push_back(tt);

    if (fVerbose > 0) {
      pTrack->dump();
    }
  }

  // -- Choose another track close to muons
  if (recTracks.size() == 2) {
    TLorentzVector m1, m2, psi, k; 
    m1.SetPtEtaPhiM(recTracks.at(0)->pt(),
		    recTracks.at(0)->eta(),
		    recTracks.at(0)->phi(),
		    MMUON); 
    m2.SetPtEtaPhiM(recTracks.at(1)->pt(),
		    recTracks.at(1)->eta(),
		    recTracks.at(1)->phi(),
		    MMUON); 

    psi = m1 + m2; 

    for (unsigned int i = 0; i < hTracks->size(); ++i){    
      reco::TrackRef rTrack(hTracks, i);
      reco::Track track(*rTrack);

      k.SetPtEtaPhiM(track.pt(),
		     track.eta(),
		     track.phi(),
		     MKAON); 

      if (k.Perp() > 2.5 && psi.DeltaR(k) < 1.5) {
	recTracks.push_back(&track); 
	break;
      }
    }

    edm::ESHandle<TransientTrackBuilder> theB;
    iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theB);

    std::vector<reco::TransientTrack> RecoTransientTrack;
    RecoTransientTrack.clear();
    for (unsigned int i=0; i < recTracks.size(); i++ ) { 
      RecoTransientTrack.push_back(theB->build(recTracks[i]));
    }

    KalmanVertexFitter theFitter(true);
    TransientVertex TransSecVtx = theFitter.vertex(RecoTransientTrack); 
  
    if ( TransSecVtx.isValid() ) {
      cout << "==>kalmanVertexFit> KVF successful!" << endl; 

      if ( isnan(TransSecVtx.position().x()) 
	   || isnan(TransSecVtx.position().y()) 
	   || isnan(TransSecVtx.position().z()) ) {

	cout << "==>kalmanVertexFit> Something went wrong!" << endl;
	cout << "==>kalmanVertexFit> SecVtx nan - Aborting... !" << endl;
	return;
      }
    } else {
      cout << "==>kalmanVertexFit> KVF failed!" << endl;
      cout << "==>kalmanVertexFit> Aborting... !" << endl;
      return;
    }

    if (fVerbose) cout << "==>kalmanVertexFit> Filling vector SecVtx" << endl;
  
    TVector3 SecVtx; 

    SecVtx.SetX(TransSecVtx.position().x()*10);  //*10 to get mm (same unit as gen info)
    SecVtx.SetY(TransSecVtx.position().y()*10);
    SecVtx.SetZ(TransSecVtx.position().z()*10);
    
    printf ("RECO SecVtx (x, y, z) = ( %5.4f, %5.4f, %5.4f)\n", SecVtx.X(), SecVtx.Y(), SecVtx.Z());

    ChiSquared chi(TransSecVtx.totalChiSquared(), TransSecVtx.degreesOfFreedom());
    if (fVerbose) cout << "Chi2 of SecVtx-Fit: " << chi.value() << endl;
  
  } else {
    cout << "==>HFDumpSignal: Found only " << recTracks.size() << " muons, not fitting" << endl;
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
