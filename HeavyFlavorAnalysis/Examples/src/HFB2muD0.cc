#include <iostream>

#include "HeavyFlavorAnalysis/Examples/interface/HFB2muD0.hh"

#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAna01Event.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaTrack.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TGenCand.hh"

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "RecoVertex/VertexTools/interface/VertexDistance3D.h"
#include "RecoVertex/VertexTools/interface/VertexDistanceXY.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "CommonTools/Statistics/interface/ChiSquared.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/TrackReco/interface/TrackExtraFwd.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"

// -- Yikes!
extern TAna01Event *gHFEvent;

using namespace std;
using namespace reco;
using namespace edm;

#define MMUON 0.10566
#define MKAON 0.49368
#define MPION 0.139570
#define MB_0 5.27953
#define MBPLUS 5.27927
#define MD_0 1.86484
#define MD_S 2.01027


// ----------------------------------------------------------------------
HFB2muD0::HFB2muD0(const edm::ParameterSet& iConfig) :
  fVerbose(iConfig.getUntrackedParameter<int>("verbose", 0)),
  fTracksLabel(iConfig.getUntrackedParameter<string>("tracksLabel", string("goodTracks"))),
  fPrimaryVertexLabel(iConfig.getUntrackedParameter<string>("PrimaryVertexLabel", string("offlinePrimaryVertices"))),
  fMuonsLabel(iConfig.getUntrackedParameter<InputTag>("muonsLabel")),
  fMuonPt(iConfig.getUntrackedParameter<double>("muonPt", 1.0)),
  fProtonPt(iConfig.getUntrackedParameter<double>("protonPt", 1.0)),
  fPionPt(iConfig.getUntrackedParameter<double>("pionPt", 0.4)),
  fDeltaR(iConfig.getUntrackedParameter<double>("deltaR", 99.)),
  fTrackPt(iConfig.getUntrackedParameter<double>("trackPt", 1.0)),
  fDeltaMD0(iConfig.getUntrackedParameter<double>("deltaMD0", 0.4)),
  fDeltaMDs(iConfig.getUntrackedParameter<double>("deltaMDs", 0.4)),
  fSlowPt(iConfig.getUntrackedParameter<double>("slowPt", 0.1)) {
  using namespace std;
  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- HFB2muD0 constructor" << endl;
  cout << "---  tracksLabel:              " << fTracksLabel.c_str() << endl;
  cout << "---  muonsLabel:               " << fMuonsLabel << endl;
  cout << "---  muonPt:                   " << fMuonPt << endl;
  cout << "---  protonPt:                 " << fProtonPt << endl;
  cout << "---  trackPt:                  " << fTrackPt << endl;
  cout << "---  slowPt:                   " << fSlowPt << endl;
  cout << "---  pionPt:                   " << fPionPt << endl;
  cout << "---  deltaR:                   " << fDeltaR << endl;
  cout << "---  deltaMD0:                 " << fDeltaMD0 << endl;
  cout << "---  deltaMDs:                 " << fDeltaMDs << endl;
  cout << "----------------------------------------------------------------------" << endl;

}


// ----------------------------------------------------------------------
HFB2muD0::~HFB2muD0() {
  
}


// ----------------------------------------------------------------------
void HFB2muD0::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  // -- get the primary vertex
  
  edm::Handle<VertexCollection> recoPrimaryVertexCollection;
  iEvent.getByLabel(fPrimaryVertexLabel.c_str(), recoPrimaryVertexCollection);
  if(!recoPrimaryVertexCollection.isValid()) {
    cout << "==>HFB2muD0> No primary vertex found, skipping" << endl;
    return;
  }
  const reco::VertexCollection vertices = *(recoPrimaryVertexCollection.product());
  if (vertices.size() == 0) {
    cout << "==>HFB2muD0> No primary vertex found, skipping" << endl;
    return;
  }
  fPV = vertices[0]; // ???
  
  // -- get the collection of muons
  Handle<MuonCollection> hMuons;
  iEvent.getByLabel(fMuonsLabel, hMuons);
  if (!hMuons.isValid()) {
    cout << "==>HFB2muD0> No valid MuonCollection with label " << fMuonsLabel << " found, skipping" << endl;
    return;
  }
  
  // -- get the collection of tracks
  edm::Handle<edm::View<reco::Track> > hTracks;
  iEvent.getByLabel(fTracksLabel.c_str(), hTracks);
  if(!hTracks.isValid()) {
    cout << "==>HFB2muD0> No valid TrackCollection with label " << fTracksLabel.c_str()
	 << " found, skipping" << endl;
    return;
  }

  if (hTracks->size() > 100) {
    cout << "==>HFB2muD0> TrackCollection with label " << fTracksLabel.c_str()
	 << " has " << hTracks->size() << " entries, skipping event!!" << endl;
  }
 
  // -- Fill muons into muonTracks
  if (fVerbose > 1)
    cout << "==>HFB2muD0> nMuons = " << hMuons->size() << endl;


  // -- get the collection of muons and store their corresponding track indices
  vector<int> muonIndices;
  for (MuonCollection::const_iterator muon = hMuons->begin(); muon != hMuons->end(); ++muon) {
    int im = muon->track().index();
    if (im > 0) muonIndices.push_back(im);
  }
  if (fVerbose > 1)
    cout << "==>HFB2muD0> nMuonIndices = " << muonIndices.size() << endl;

  if (muonIndices.size() < 1) return;

  // =========================================================================================================

  // go through muon track list
  TLorentzVector vmuon, vtrack1, vtrack2, vd0, vkaon, vpion;
  for (unsigned int imuon = 0; imuon < muonIndices.size(); imuon++) {
    if (fVerbose > 1)
      cout << "==>HFB2muD0> Checking Muon #" << imuon << endl;

    TrackBaseRef muTrackView(hTracks, muonIndices[imuon]); // <------------------------------------------- ???
    Track muon(*muTrackView);

    vmuon.SetPtEtaPhiM(muon.pt(), muon.eta(), muon.phi(), MMUON);

    if (muon.pt() < fMuonPt) {
      if (fVerbose > 1)
	cout << "==>HFB2muD0> muon.pt() < " << fMuonPt << endl;
      continue;
    }
    
    vector<unsigned int> track_list;
    vector<unsigned int> slow_list;
    for (unsigned int itrack = 0; itrack < hTracks->size(); itrack++) {
      if (itrack == imuon)
	continue;
      
      TrackBaseRef tr1TrackView(hTracks, itrack);
      Track track(*tr1TrackView);

      vtrack1.SetPtEtaPhiM(track.pt(), track.eta(), track.phi(), MPION);
      
      if (vmuon.DeltaR(vtrack1) < fDeltaR) {
	if (track.pt() > fTrackPt)
	  track_list.push_back(itrack);
	if (track.pt() > fSlowPt)
	  slow_list.push_back(itrack);
      }
    }
    
    if (fVerbose > 1)
      cout << "==>HFB2muD0> track_list size = " << track_list.size() << endl;

    if (track_list.size() < 2) {
      if (fVerbose > 1)
	cout << "==>HFB2muD0> Too little tracks in muon cone!" << endl;
      continue;
    }
    
    iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder", fTTB);
    if (!fTTB.isValid()) {
      cout << " -->HFB2muD0: Error: no TransientTrackBuilder found." << endl;
      return;
    }
    
    // D0 reconstruction #####################################################################################

    // tracks (kaon + pion) which form a D0 candidate
    // d0tracks[2n    ] is the kaon of the n'th D0
    // d0tracks[2n + 1] is the pion of the n'th D0
    vector<unsigned int> d0tracks;

    vector<unsigned int>::iterator ia, ib;
    for (ia = track_list.begin(); ia != track_list.end(); ++ia) {
      if (*ia == imuon)
	continue;
      for (ib = ia + 1; ib != track_list.end(); ++ib) {
	if (*ib == imuon)
	  continue;
	TrackBaseRef tr1TrackView(hTracks, *ia);
	Track a(*tr1TrackView);
	TrackBaseRef tr2TrackView(hTracks, *ib);
	Track b(*tr2TrackView);

	if (a.charge() * b.charge() < 0) {
	  Track kaon, pion;
	  unsigned int ikaon, ipion;
	  if (a.charge() * muon.charge() > 0) {
	    // a is the kaon
	    kaon = a;
	    pion = b;
	    ikaon = *ia;
	    ipion = *ib;
	    vkaon.SetPtEtaPhiM(a.pt(), a.eta(), a.phi(), MKAON);
	    vpion.SetPtEtaPhiM(b.pt(), b.eta(), b.phi(), MPION);
	  } else {
	    // b is the kaon
	    kaon = b;
	    pion = a;
	    ikaon = *ib;
	    ipion = *ia;
	    vpion.SetPtEtaPhiM(a.pt(), a.eta(), a.phi(), MPION);
	    vkaon.SetPtEtaPhiM(b.pt(), b.eta(), b.phi(), MKAON);
	  }
	  
	  vd0 = vkaon + vpion;
	  if (TMath::Abs(vd0.M()- MD_0) < fDeltaMD0) {
	    if (fVerbose > 2)
	      cout << "==>HFB2muD0> D0 candidate found! Mass: " << vd0.M() << endl;

	    d0tracks.push_back(ikaon);
	    d0tracks.push_back(ipion);

	    std::vector<reco::Track> fitTracks;
	    fitTracks.clear();
	    fitTracks.push_back(kaon);
	    fitTracks.push_back(pion);

	    //m1.SetXYZM(tMuon1.px(), tMuon1.py(), tMuon1.pz(), MMUON);
	    //m2.SetXYZM(tMuon2.px(), tMuon2.py(), tMuon2.pz(), MMUON);

	    TAnaVertex D0Vtx;
	    D0Vtx = DoVertexFit(fitTracks);
	    D0Vtx.addTrack(ikaon);
	    D0Vtx.addTrack(ipion);

 	    //vkaon.SetXYZM(fitTracks[0].px(), fitTracks[0].py(), fitTracks[0].pz(), MKAON);
 	    //vpion.SetXYZM(fitTracks[1].px(), fitTracks[1].py(), fitTracks[1].pz(), MPION);
 	    //vd0 = m1 + m2;

	    TAnaCand *pCand;
	    pCand        = gHFEvent->addCand();
	    pCand->fPlab = vd0.Vect();
	    pCand->fMass = vd0.M();
	    pCand->fType = 421;
	    pCand->fVtx  = D0Vtx;
	    //pCand->fMom  = -1;
	    pCand->fSig1 = gHFEvent->nSigTracks();
	    pCand->fSig2 = pCand->fSig1 + 1;

	    TAnaTrack *pTrack;
	    pTrack            = gHFEvent->addSigTrack();
	    pTrack->fMCID     = kaon.charge() * 321;
	    pTrack->fGenIndex = -1;
	    pTrack->fQ        = kaon.charge();
	    pTrack->fPlab.SetPtEtaPhi(vkaon.Pt(), vkaon.Eta(), vkaon.Phi());
	    pTrack->fIndex  = ikaon;

	    pTrack            = gHFEvent->addSigTrack();
	    pTrack->fMCID     = pion.charge() * 211;
	    pTrack->fGenIndex = -1;
	    pTrack->fQ        = pion.charge();
	    pTrack->fPlab.SetPtEtaPhi(vpion.Pt(), vpion.Eta(), vpion.Phi());
	    pTrack->fIndex  = ipion;
	  } // mass cut
	} // charge cut
      } // track b (D0 reco)
    } // track a (D0 reco)

    // D* reconstruction #####################################################################################

    // looking for (slow) pion which together with the D0 is a daughter of D*
    TLorentzVector vslow, vds;
    vector<unsigned int>::iterator islow;
    for (islow = slow_list.begin(); islow != slow_list.end(); ++islow) {
      TrackBaseRef tr1TrackView(hTracks, *islow);
      Track slow(*tr1TrackView);

      // pion has opposite muon charge
      if (muon.charge() * slow.charge() < 0) {

	// go through D0 candidates
	vector<unsigned int>::iterator id0;
	for (id0 = d0tracks.begin(); id0 != d0tracks.end(); id0 += 2) {
	  unsigned int ikaon = *id0;
	  unsigned int ipion = *(id0 + 1);
	  // slow pion should not be the kaon
	  if (ipion == *islow || ikaon == *islow)
	    continue;

	  TrackBaseRef tr1TrackView(hTracks, ikaon);
	  Track kaon(*tr1TrackView);
	  TrackBaseRef tr2TrackView(hTracks, ipion);
	  Track pion(*tr2TrackView);

	  // maybe take refitted tracks instead? <-------------------------------------------------------- !!!
	  vkaon.SetPtEtaPhiM(kaon.pt(), kaon.eta(), kaon.phi(), MKAON);
	  vpion.SetPtEtaPhiM(pion.pt(), pion.eta(), pion.phi(), MPION);

	  // mass cut
	  vslow.SetPtEtaPhiM(slow.pt(), slow.eta(), slow.phi(), MPION);
	  vds = vslow + vkaon + vpion;
	  if (TMath::Abs(vds.M() - MD_S) < fDeltaMDs) {
	    if (fVerbose > 2)
	      cout << "==>HFB2muD0> D* candidate found! Mass: " << vds.M() << endl;

	    // Fill TAnaCand
	    std::vector<reco::Track> fitTracks;
	    fitTracks.clear();
	    fitTracks.push_back(muon);
	    fitTracks.push_back(slow);
	    // fitTracks.push_back(D0 candidate);  <------------------------------------------------------ !!!

	    //m1.SetXYZM(tMuon1.px(), tMuon1.py(), tMuon1.pz(), MMUON);
	    //m2.SetXYZM(tMuon2.px(), tMuon2.py(), tMuon2.pz(), MMUON);

	    TAnaVertex DsVtx;
	    DsVtx = DoVertexFit(fitTracks);
	    DsVtx.addTrack(*islow);
	    DsVtx.addTrack(imuon);

 	    //vkaon.SetXYZM(fitTracks[0].px(), fitTracks[0].py(), fitTracks[0].pz(), MKAON);
 	    //vpion.SetXYZM(fitTracks[1].px(), fitTracks[1].py(), fitTracks[1].pz(), MPION);
 	    //vd0 = m1 + m2;

	    TAnaCand *pCand;
	    pCand        = gHFEvent->addCand();
	    pCand->fPlab = vds.Vect();
	    pCand->fMass = vds.M();
	    pCand->fType = 413;
	    pCand->fVtx  = DsVtx;
	    //pCand->fMom  = -1;
	    pCand->fDau1  = (id0 - d0tracks.begin()) / 2;
	    pCand->fDau2  = pCand->fDau1;
	    pCand->fSig1 = gHFEvent->nSigTracks();
	    pCand->fSig2 = pCand->fSig1 + 0; // <- change when adding D0 to fitTracks -------------------- !!!

	    TAnaTrack *pTrack;
	    pTrack            = gHFEvent->addSigTrack();
	    pTrack->fMCID     = slow.charge() * 211;
	    pTrack->fGenIndex = -1;
	    pTrack->fQ        = slow.charge();
	    pTrack->fPlab.SetPtEtaPhi(vslow.Pt(), vslow.Eta(), vslow.Phi());
	    pTrack->fIndex  = *islow;

	    // pTrack            = gHFEvent->addSigTrack(); // <- add one for D0 ------------------------- !!!

	  } // mass cut
	} // d0 candidates
      } // charge cut
    } // track (D* reco)
  } // muons

  if (fVerbose > 0) {
    cout << "==>HFB2muD0> " << gHFEvent->nCands() << " D0/D* candidates found." << endl;
  }

  return;
}


// ----------------------------------------------------------------------
TAnaVertex HFB2muD0::DoVertexFit(std::vector<reco::Track> &Tracks){
  KalmanVertexFitter theFitter(true);
  std::vector<reco::TransientTrack> TransTracks;
  for(reco::TrackCollection::const_iterator iTrack=Tracks.begin(); iTrack!=Tracks.end(); iTrack++)
    TransTracks.push_back(fTTB->build(*iTrack));

  TransientVertex TransSecVtx = theFitter.vertex(TransTracks);
  TAnaVertex anaVt;
  if ( TransSecVtx.isValid() ) {
    if ( isnan(TransSecVtx.position().x())
	 || isnan(TransSecVtx.position().y())
	 || isnan(TransSecVtx.position().z()) ) {
      cout << "==>HFB2muD0> Something went wrong! SecVtx nan - Aborting... !" << endl;
      anaVt.fPoint.SetXYZ(0,0,0);
      return anaVt;
    }
  } else {
    cout << "==>HFB2muD0> KVF failed! Aborting... !" << endl;
    anaVt.fPoint.SetXYZ(0,0,0);
    return anaVt;
  }

  // -- Give back refitted tracks
  Tracks.clear();
  std::vector<reco::TransientTrack> refTT = TransSecVtx.refittedTracks();
  for(vector<reco::TransientTrack>::const_iterator i = refTT.begin(); i != refTT.end(); i++) {
    const Track & ftt = i->track();
    Tracks.push_back(ftt);
  }

  ChiSquared chi(TransSecVtx.totalChiSquared(), TransSecVtx.degreesOfFreedom());

  // -- Build vertex for ntuple
  anaVt.setInfo(chi.value(), int(chi.degreesOfFreedom()), chi.probability(), 1, 0);
  anaVt.fPoint.SetXYZ(TransSecVtx.position().x(),
		      TransSecVtx.position().y(),
		      TransSecVtx.position().z());
 
  // -- Distance to primary vertex

  VertexDistanceXY axy;
  double dXY      = axy.distance(fPV, TransSecVtx).value();
  double dXYE     = axy.distance(fPV, TransSecVtx).error();
 
  VertexDistance3D a3d;
  double d3d      = a3d.distance(fPV, TransSecVtx).value();
  double d3dE     = a3d.distance(fPV, TransSecVtx).error();
 
  anaVt.fDxy  = dXY;
  anaVt.fDxyE = dXYE;

  anaVt.fD3d  = d3d;
  anaVt.fD3dE = d3dE;

  return anaVt;
}


// ------------ method called once each job just before starting event loop  ------------
void  HFB2muD0::beginJob(const edm::EventSetup& setup) {
}


// ------------ method called once each job just after ending the event loop  ------------
void  HFB2muD0::endJob() {
}


//define this as a plug-in
//DEFINE_FWK_MODULE(HFB2muD0);
