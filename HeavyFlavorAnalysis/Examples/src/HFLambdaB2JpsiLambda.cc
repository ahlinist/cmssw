#include <iostream>

#include "HeavyFlavorAnalysis/Examples/interface/HFLambdaB2JpsiLambda.h"

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
#define MJPSI 3.096916
#define MPION 0.139570
#define MPROTON 0.93827
#define MLAMBDA_B 5.6202
#define MLAMBDA_0 1.115683
#define MKSTAR 0.8960
#define MKSHORT 0.497614
#define MB_0 5.27953
#define MBPLUS 5.27927


// ----------------------------------------------------------------------
HFLambdaB2JpsiLambda::HFLambdaB2JpsiLambda(const edm::ParameterSet& iConfig) :
  fVerbose(iConfig.getUntrackedParameter<int>("verbose", 0)),
  fTracksLabel(iConfig.getUntrackedParameter<string>("tracksLabel", string("goodTracks"))), 
  fPrimaryVertexLabel(iConfig.getUntrackedParameter<string>("PrimaryVertexLabel", string("offlinePrimaryVertices"))),
  fMuonsLabel(iConfig.getUntrackedParameter<InputTag>("muonsLabel")),
  fMuonPt(iConfig.getUntrackedParameter<double>("muonPt", 1.0)), 
  fProtonPt(iConfig.getUntrackedParameter<double>("protonPt", 1.0)), 
  fPionPt(iConfig.getUntrackedParameter<double>("pionPt", 0.4)), 
  fDeltaR(iConfig.getUntrackedParameter<double>("deltaR", 99.)) {
  using namespace std;
  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- HFLambdaB2JpsiLambda constructor" << endl;
  cout << "---  tracksLabel:              " << fTracksLabel.c_str() << endl;
  cout << "---  muonsLabel:               " << fMuonsLabel << endl;
  cout << "---  muonPt:                   " << fMuonPt << endl;
  cout << "---  protonPt:                 " << fProtonPt << endl;
  cout << "---  pionPt:                   " << fPionPt << endl;
  cout << "---  deltaR:                   " << fDeltaR << endl;
  cout << "----------------------------------------------------------------------" << endl;

}


// ----------------------------------------------------------------------
HFLambdaB2JpsiLambda::~HFLambdaB2JpsiLambda() {
  
}


// ----------------------------------------------------------------------
void HFLambdaB2JpsiLambda::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  // -- get the primary vertex
  
  edm::Handle<VertexCollection> recoPrimaryVertexCollection;
  iEvent.getByLabel(fPrimaryVertexLabel.c_str(), recoPrimaryVertexCollection);
  if(!recoPrimaryVertexCollection.isValid()) {
    cout << "==>HFLambdaB2JpsiLambda> No primary vertex found, skipping" << endl;
    return;
  }
  const reco::VertexCollection vertices = *(recoPrimaryVertexCollection.product());
  if (vertices.size() == 0) {
    cout << "==>HFLambdaB2JpsiLambda> No primary vertex found, skipping" << endl;
    return;
  }
  fPV = vertices[0]; // ???
  
  // -- get the collection of muons
  Handle<MuonCollection> hMuons;
  iEvent.getByLabel(fMuonsLabel, hMuons);
  if (!hMuons.isValid()) {
  cout << "==>HFLambdaB2JpsiLambda> No valid MuonCollection with label "<< fMuonsLabel <<" found, skipping" << endl;
    return;
  }
  
  // -- get the collection of tracks
  edm::Handle<edm::View<reco::Track> > hTracks;
  iEvent.getByLabel(fTracksLabel.c_str(), hTracks);
  if(!hTracks.isValid()) {
    cout << "==>HFLambdaB2JpsiLambda> No valid TrackCollection with label "<<fTracksLabel.c_str() <<" found, skipping" << endl;
    return;
  }
 
  // -- Fill muons into muonTracks
  if (fVerbose > 0) cout << "==>HFLambdaB2JpsiLambda> nMuons = " << hMuons->size() << endl;


  // -- get the collection of muons and store their corresponding track indices
  vector<int> muonIndices;
  for (MuonCollection::const_iterator muon = hMuons->begin(); muon != hMuons->end(); ++muon) {
    int im = muon->track().index(); 
    if (im > 0) muonIndices.push_back(im);
  }
  if (fVerbose > 0) {
    cout << "==>HFLambdaB2JpsiLambda> nMuons = " << hMuons->size() << endl;
    cout << "==>HFLambdaB2JpsiLambda> nMuonIndices = " << muonIndices.size() << endl;
  }

  if (muonIndices.size() < 2) return;

  // -- reconstruct J/Psi
  TLorentzVector psi, cpsi, m1, m2;
  double dmass(1000.0);

  Track tMuon1, tMuon2; 
  int iMuon1(-1), iMuon2(-1); 
  for (unsigned int imuon1 = 0; imuon1 < muonIndices.size()-1; ++imuon1) {
    TrackBaseRef mu1TrackView(hTracks, muonIndices[imuon1]);
    Track muon1(*mu1TrackView);
    if (muon1.pt() < fMuonPt)  {
      continue;
    }

    for (unsigned int imuon2 = imuon1 + 1; imuon2 < muonIndices.size(); ++imuon2) {
      TrackBaseRef mu2TrackView(hTracks, muonIndices[imuon2]);
      Track muon2(*mu2TrackView);
      if (muon2.pt() < fMuonPt)  {
	continue;
      }

      m1.SetPtEtaPhiM(muon1.pt(),
		      muon1.eta(),
		      muon1.phi(),
		      MMUON); 
      m2.SetPtEtaPhiM(muon2.pt(),
		      muon2.eta(),
		      muon2.phi(),
		      MMUON); 
      psi = m1 + m2; 

      if ((TMath::Abs(psi.M()-MJPSI) < dmass) 
	  && (muon1.charge()*muon2.charge() < 0) 
	  ) {
	dmass  = TMath::Abs(psi.M()-MJPSI);
	tMuon1 = muon1;
	iMuon1 = muonIndices[imuon1]; 
	tMuon2 = muon2;
	iMuon2 = muonIndices[imuon2]; 
      }
    }
  }

  if (iMuon1 < 0 || iMuon2 < 0) {
    if (fVerbose > 0) cout << " --> HFLambdaB2JpsiLambda: No unlike charge dimuons found, dmass = " << dmass << endl;
    return;
  }
  if (TMath::Abs(psi.M() - MJPSI) >  0.5) {
    if (fVerbose > 0) cout << " --> HFLambdaB2JpsiLambda: No J/Psi candidate found: " << psi.M() << endl;
    return;
  }

  iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder", fTTB);
  if (!fTTB.isValid()) {
    cout << " -->HFLambdaB2JpsiLambda: Error: no TransientTrackBuilder found."<<endl;
    return;
  }
  std::vector<reco::Track> fitTracks;

  TAnaVertex jpsiVtx, lambdaVtx;
  TAnaTrack *pTrack; 

  // -- Fit J/Psi
  fitTracks.clear();
  fitTracks.push_back(tMuon1);
  fitTracks.push_back(tMuon2);

  m1.SetXYZM(tMuon1.px(), tMuon1.py(), tMuon1.pz(), MMUON); 
  m2.SetXYZM(tMuon2.px(), tMuon2.py(), tMuon2.pz(), MMUON); 

  jpsiVtx = DoVertexFit(fitTracks);
  jpsiVtx.addTrack(iMuon1);
  jpsiVtx.addTrack(iMuon2);

  m1.SetXYZM(fitTracks[0].px(), fitTracks[0].py(), fitTracks[0].pz(), MMUON); 
  m2.SetXYZM(fitTracks[1].px(), fitTracks[1].py(), fitTracks[1].pz(), MMUON); 
  psi = m1 + m2;

  // -- Lambda reconstruction 
  fitTracks.clear();

  // -- Proton!
  for (unsigned int iProton = 0; iProton < hTracks->size(); ++iProton){    
    if (static_cast<int>(iProton) == iMuon1 || static_cast<int>(iProton) == iMuon2) continue; 
    TrackBaseRef rTrackView(hTracks, iProton);
    Track proton(*rTrackView);

    if (proton.pt() < fProtonPt) continue;
    
    
    // -- Pion!
    for (unsigned int iPion = 0; iPion < hTracks->size(); ++iPion){    
      if (iPion == iProton) continue; 
      if (static_cast<int>(iPion) == iMuon1 || static_cast<int>(iPion) == iMuon2) continue; 

      TrackBaseRef piTrackView(hTracks, iPion);
      Track pion(*piTrackView);

      if (pion.pt() < fPionPt) continue;
      if (pion.pt() > proton.pt()) continue;
      if (pion.charge()*proton.charge() > 0) continue;
	
      TLorentzVector pi, p, Lambda, cLambda, LambdaB; 
      
      p.SetXYZM(proton.px(), proton.py(), proton.pz(), MPROTON); 
      pi.SetXYZM(pion.px(), pion.py(), pion.pz(), MPION); 

      Lambda = p + pi;
      if (TMath::Abs(Lambda.M() - MLAMBDA_0) > 0.3) { 
	fitTracks.clear();
	fitTracks.push_back(proton);
	fitTracks.push_back(pion);
	lambdaVtx = DoVertexFit(fitTracks);
	lambdaVtx.addTrack(iProton);
	lambdaVtx.addTrack(iPion);

      
	p.SetXYZM(fitTracks[0].px(), fitTracks[0].py(), fitTracks[0].pz(), MPROTON); 
	pi.SetXYZM(fitTracks[1].px(), fitTracks[1].py(), fitTracks[1].pz(), MPION); 
	Lambda = p + pi;

	if (TMath::Abs(Lambda.M() - MLAMBDA_0) > 0.20) { 
	  
	  cLambda.SetXYZM(Lambda.Px(), Lambda.Py(), Lambda.Pz(), MLAMBDA_0); 
	  cpsi.SetXYZM(psi.Px(), psi.Py(), psi.Pz(), MJPSI); 
	  LambdaB = cLambda + cpsi; 

	  if (fVerbose > 0) {
	    cout << "muon1: " << iMuon1 << " " << m1.Pt() << ", " << m1.Phi() << ", " << m1.Eta() << endl;
	    cout << "muon2: " << iMuon2 << " " << m2.Pt() << ", " << m2.Phi() << ", " << m2.Eta() << endl;
	    cout << "proton: " << iProton << " " << p.Pt() << ", " << p.Phi() << ", " << p.Eta() << endl;
	    cout << "pion: " << iPion << " " << pi.Pt() << ", " << pi.Phi() << ", " << pi.Eta() << endl;
	    cout << "J/psi mass = " << psi.M()
		 << " and chi2: " << jpsiVtx.fChi2 << endl
		 << " at vertex: (" << jpsiVtx.fPoint.X() << "," << jpsiVtx.fPoint.Y() << "," << jpsiVtx.fPoint.Z() << ")" 
		 << " and distances: " << jpsiVtx.fDxy << "+/-" << jpsiVtx.fDxyE << " " << jpsiVtx.fD3d << "+/-" << jpsiVtx.fD3dE
		 <<	endl;
	    cout << "LambdaB mass = " << LambdaB.M()
		 << " and chi2: " << lambdaVtx.fChi2 << endl
		 << " at vertex: (" << lambdaVtx.fPoint.X() << "," << lambdaVtx.fPoint.Y() << "," << lambdaVtx.fPoint.Z() << ")" 
		 << " and distances: " << lambdaVtx.fDxy << "+/-" << lambdaVtx.fDxyE << " " << lambdaVtx.fD3d << "+/-" << lambdaVtx.fD3dE
		 <<	endl;
	  }

	  if (TMath::Abs(LambdaB.M() - MLAMBDA_B) >  1.3) { 
	    // -- fill candidates

	    // LambdaB
	    int lind = gHFEvent->nCands();
	    TAnaCand  *pCand = gHFEvent->addCand();
 
	    pCand->fPlab = LambdaB.Vect();
	    pCand->fMass = LambdaB.M();
	    pCand->fType = 5122;
	    pCand->fDau1 = lind+1;
	    pCand->fDau2 = lind+2;

	    // -- Distance between J/psi and Lambda vertex -- schoe waer's, he?!
	    // 	    TAnaVertex anaVt;
	    // 	    anaVt.fPoint.SetXYZ(99,99,99);
	    // 	    VertexDistanceXY axy;
	    // 	    anaVt.fDxy   = axy.distance(lambdaVtx->vertexState(), jpsiVtx->vertexState()).value();
	    // 	    anaVt.fDxyE  = axy.distance(lambdaVtx->vertexState(), jpsiVtx->vertexState()).error();
	    // 	    VertexDistance3D a3d;
	    // 	    anaVt.fD3d   = a3d.distance(lambdaVtx->vertexState(), jpsiVtx->vertexState()).value();
	    // 	    anaVt.fD3dE  = a3d.distance(lambdaVtx->vertexState(), jpsiVtx->vertexState()).error();
	    // 	    pCand->fVtx  = anaVt;    

	    // Lambda with proton and pion
	    pCand        = gHFEvent->addCand();
	    pCand->fPlab = Lambda.Vect();
	    pCand->fMass = Lambda.M();
	    pCand->fType = 3122;
	    pCand->fVtx  = lambdaVtx;    
	    pCand->fMom  = lind;
	    pCand->fSig1 = gHFEvent->nSigTracks();
	    pCand->fSig2 = pCand->fSig1 + 1;

	    pTrack            = gHFEvent->addSigTrack();
	    pTrack->fMCID     = proton.charge()*2212; 
	    pTrack->fGenIndex = -1; 
	    pTrack->fQ        = proton.charge();
	    pTrack->fPlab.SetPtEtaPhi(p.Pt(),
				      p.Eta(),
				      p.Phi()
				      ); 
	    pTrack->fIndex  = iProton;

	    pTrack            = gHFEvent->addSigTrack();
	    pTrack->fMCID     = pion.charge()*211; 
	    pTrack->fGenIndex = -1; 
	    pTrack->fQ        = pion.charge();
	    pTrack->fPlab.SetPtEtaPhi(pi.Pt(),
				      pi.Eta(),
				      pi.Phi()
				      ); 
	    pTrack->fIndex  = iPion;

	    // J/psi
	    pCand        = gHFEvent->addCand();
	    pCand->fPlab = psi.Vect();
	    pCand->fMass = psi.M();
	    pCand->fType = 443;
	    pCand->fVtx  = jpsiVtx;    
	    pCand->fMom  = lind;
	    pCand->fSig1 = gHFEvent->nSigTracks();
	    pCand->fSig2 = pCand->fSig1 + 1;

	    pTrack            = gHFEvent->addSigTrack();
	    pTrack->fMCID     = tMuon1.charge()*13; 
	    pTrack->fGenIndex = -1; 
	    pTrack->fQ        = tMuon1.charge();
	    pTrack->fPlab.SetPtEtaPhi(m1.Pt(),
				      m1.Eta(),
				      m1.Phi()
				      ); 
	    pTrack->fIndex  = iMuon1;

	    pTrack            = gHFEvent->addSigTrack();
	    pTrack->fMCID     = tMuon2.charge()*13; 
	    pTrack->fGenIndex = -1; 
	    pTrack->fQ        = tMuon2.charge();
	    pTrack->fPlab.SetPtEtaPhi(m2.Pt(),
				      m2.Eta(),
				      m2.Phi()
				      ); 
	    pTrack->fIndex  = iMuon2;

	  }
	  
	}
      }

    }
  }

  
}


// ----------------------------------------------------------------------
TAnaVertex HFLambdaB2JpsiLambda::DoVertexFit(std::vector<reco::Track> &Tracks){
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
	cout << "==>HFLambdaB2JpsiLambda> Something went wrong! SecVtx nan - Aborting... !" << endl;
        anaVt.fPoint.SetXYZ(0,0,0);
	return anaVt;
      }
  } else {
      cout << "==>HFLambdaB2JpsiLambda> KVF failed! Aborting... !" << endl;
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
void  HFLambdaB2JpsiLambda::beginJob(const edm::EventSetup& setup) {
}


// ------------ method called once each job just after ending the event loop  ------------
void  HFLambdaB2JpsiLambda::endJob() {
}


//define this as a plug-in
//DEFINE_FWK_MODULE(HFLambdaB2JpsiLambda);
