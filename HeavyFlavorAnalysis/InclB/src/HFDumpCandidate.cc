#include <iostream>

#include "HeavyFlavorAnalysis/InclB/interface/HFDumpCandidate.h"

#include "HeavyFlavorAnalysis/InclB/rootio/TAna00Event.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TAnaTrack.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TAnaCand.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TGenCand.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TAnaVertex.hh"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

#include "DataFormats/JetReco/interface/BasicJetCollection.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "DataFormats/GeometryVector/interface/GlobalVector.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/IPTools/interface/IPTools.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"

#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "DataFormats/Candidate/interface/Candidate.h"

// -- Yikes!
extern TAna00Event *gHFEvent;

using namespace std;
using namespace reco;
using namespace edm;

const double mkaon = 0.493;
const double mpion = 0.140;


// ----------------------------------------------------------------------
HFDumpCandidate::HFDumpCandidate(const edm::ParameterSet& iConfig) :
  fVerbose(iConfig.getUntrackedParameter<int>("verbose", 0)),
  fTrackPtMin(iConfig.getUntrackedParameter<double>("trackptmin", 1)),
  fDeltaR(iConfig.getUntrackedParameter<double>("deltaR", 1.5)),
  fMuonLabel(iConfig.getUntrackedParameter<string>("muonLabel", string("globalMuons"))),
  fTrackLabel(iConfig.getUntrackedParameter<string>("trackLabel", string("generalTracks")))
{
  using namespace std;
  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- HFDumpCandidate constructor" << endl;
  cout << "--- " << fMuonLabel.c_str() << endl;
  cout << "----------------------------------------------------------------------" << endl;

}


// ----------------------------------------------------------------------
HFDumpCandidate::~HFDumpCandidate() {
  
}


// ----------------------------------------------------------------------
void HFDumpCandidate::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  nevt++;

  edm::Handle<reco::MuonCollection> muons;
  iEvent.getByLabel( fMuonLabel.c_str(), muons);


  Handle<reco::TrackCollection> recTrks;
  iEvent.getByLabel(fTrackLabel.c_str(), recTrks); 
   
  TAnaCand *pCand;
 
  int index = 0;
  int i = 0;
  if (fVerbose > 0) cout << "==>HFDumpCandidate> nMuons =" << muons->size() << endl;
  for (  reco::MuonCollection::const_iterator muon = muons->begin(); muon != muons->end(); ++ muon ) { 
    if (muon->isGlobalMuon()) {
      int index_muontrack = (muon->track()).index();
      
      //pCand->fType = index : sagt zu welchen muon der Cand gehoert.
      int k_index=0;
      for (reco::TrackCollection::const_iterator kaon=recTrks->begin(); kaon < recTrks->end(); ++kaon){ 
	//track with same charge as muon (=kaon from D0 decay)
	//+pt cut
	if (k_index != index_muontrack && muon->charge() == kaon->charge() && kaon->pt() > fTrackPtMin) { 
	  //dr cut 
	  TVector3 muvect; muvect.SetPtEtaPhi(muon->pt(),muon->eta(),muon->phi());
	  TVector3 kaonvect; kaonvect.SetPtEtaPhi(kaon->pt(),kaon->eta(),kaon->phi());
	  if (muvect.DeltaR(kaonvect)<fDeltaR) {
	    
	    //second track with opposite charge (=pion from D0 decay)
	    //all possible combinations
	    int p_index=0;
	    for (reco::TrackCollection::const_iterator pion=recTrks->begin(); pion < recTrks->end(); ++pion){ 
	      //+pt cut
	      if (muon->charge() != pion->charge() && pion->pt() > fTrackPtMin) {
		//dr cut  
		TVector3 pionvect; pionvect.SetPtEtaPhi(pion->pt(),pion->eta(),pion->phi());
		if (muvect.DeltaR(pionvect)<fDeltaR) {
		  //-->add cand
		  
		  //cout << "Candidate_____________________" << i << endl;
		  pCand            = gHFEvent->addD0Cand();
		  pCand->fMCID     = 0;                    //type of DO cand
		  pCand->fType     = index;                 //muon index
		  pCand->fIndex    = i;                     //cand index
		  pCand->fSig1     = k_index;               //kaon index
		  pCand->fSig2     = p_index;               //pion index
		  
		  //*******************************************
		  //calculate secondary vertex of pion and kaon
		  //*******************************************
		  // -- Kalman Vertex Fit
		  std::vector<reco::TransientTrack> RecoTransientTrack;
		  RecoTransientTrack.clear();
		  
		  try { 
		    
		    if (fVerbose > 0)
		      cout << "==>vertex of D0 candidate (pion and kaon) " << endl;
		    
		    edm::ESHandle<TransientTrackBuilder> theB;
		    iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theB);
		    //get transient track 
		    RecoTransientTrack.push_back( (theB->build(&(*kaon)  )) );
		    RecoTransientTrack.push_back( (theB->build(&(*pion)  )) );
		    //get secondary vertex 
		    if (fVerbose) cout << "==>kalmanVertexFit> Vertexing with " << RecoTransientTrack.size() << " trans. tracks " << endl;
		    KalmanVertexFitter theFitter(true);
		    
		    if (fVerbose) cout << "==>kalmanVertexFit> Starting fitter TransSecVtx" << endl;
		    TransientVertex TransSecVtx = theFitter.vertex(RecoTransientTrack); 
		    
		    bool goodvertex = false;
		    if ( TransSecVtx.isValid() ) {
		      if (fVerbose) 
			cout << "==>kalmanVertexFit> KVF successful!" << endl; 
		      
		      if ( isnan(TransSecVtx.position().x()) || isnan(TransSecVtx.position().y()) || isnan(TransSecVtx.position().z()) ) {
			if (fVerbose) 
			  cout << "==>kalmanVertexFit> Something went wrong! SecVtx nan - Aborting... !" << endl;
		      }
		      else {
			goodvertex = true;
		      }
		    } else { 
		      if (fVerbose) 
			cout << "==>kalmanVertexFit> KVF failed! Aborting... !" << endl;
		    }
		    
		    TAnaVertex *pVtx;
		    pVtx = new TAnaVertex();
		    
		    if (goodvertex) {
		      
		      pVtx->fPoint.SetXYZ(TransSecVtx.position().x()*10,TransSecVtx.position().y()*10,TransSecVtx.position().z()*10);//*10 to get mm (same unit as gen info)
		      pVtx->setInfo(TransSecVtx.normalisedChiSquared(), TransSecVtx.degreesOfFreedom(), -1., -1, -1);
		      
		    }
		    else {
		      pVtx->fPoint.SetXYZ(-9999.,-9999.,-9999.);
		      pVtx->setInfo(-1., -1, -1., -1, -1);
		      
		    }
		    
		    pCand->fVtx = *pVtx;
		    
		    
		  } catch (Exception event) {
		    
		    cout << "%% -- No TransientTrackRecord (1) with label TransientTrackBuilder" << endl;
		    //return -99.;
		  }
		  //*******************************************
		  //*******************************************
		  
		  //*******************************************
		  //calculate invariant mass
		  //******************************************* 
		  TLorentzVector kaonvector; kaonvector.SetXYZM(kaon->px(),kaon->py(),kaon->pz(),mkaon);
		  TLorentzVector pionvector; pionvector.SetXYZM(pion->px(),pion->py(),pion->pz(),mpion);
		  
		  pCand->fPlab.SetPtEtaPhi((kaonvector+pionvector).Pt(),(kaonvector+pionvector).Eta(),(kaonvector+pionvector).Phi());
		  pCand->fMass   = (kaonvector+pionvector).M();
		  
		  //*******************************************
		  //*******************************************
		  if (fVerbose > 0) pCand->dump(); 
		  
		  //*******************************************************************************
		  //search for additional pion
		  //******************************************************************************* 
		  //use fSig3 for index of slow pion track
		  pCand->fSig3      = -9999;
		  pCand->fv1        = -9999;
		  pCand->fv2        = -9999;
		  pCand->fv3        = -9999;
		  pCand->fv4        = -9999; 
		  
		  
		  pCand->fSig3  = GetSlowPion(iEvent,muvect, (muon->track()).index(), p_index, pion->charge());
		  if (pCand->fSig3>-1) { 
		    
		    reco::TrackRef slowpion(recTrks, pCand->fSig3);
		    if (fVerbose > 0) {
		      cout << "==>found a slow pion " << pCand->fSig3 << endl;
		      cout << "muon charge  " << muon->charge() << endl;
		      cout << "kaon charge  " << kaon->charge() << endl;
		      cout << "pion charge  " << pion->charge() << endl;
		      cout << "spion charge " << slowpion->charge() << endl << endl;
		      cout << "muon index  " << index_muontrack << endl;
		      cout << "kaon index  " << k_index << endl;
		      cout << "pion index  " << p_index << endl;
		    }
		    
		    //**********************************************
		    //calculate secondary vertices
		    //**********************************************
		    //try {
		    
		    edm::ESHandle<TransientTrackBuilder> theB;
		    iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theB);
		    
		    //**********************************************
		    //calculate secondary vertex of D0 and slow pion
		    //**********************************************
		    if (fVerbose > 0)
		      cout << "==>vertex of D0 candidate and slow pion " << endl;
		    RecoTransientTrack.clear();
		    
		    //get transient track 
		    RecoTransientTrack.push_back( (theB->build(&(*kaon)  )) );
		    RecoTransientTrack.push_back( (theB->build(&(*pion)  )) ); 
		    RecoTransientTrack.push_back( (theB->build(&(*slowpion)  )) );
		    //get secondary vertex 
		    if (fVerbose) cout << "==>kalmanVertexFit> Vertexing with " << RecoTransientTrack.size() << " trans. tracks " << endl;
		    KalmanVertexFitter theFitter(true);
		    
		    if (fVerbose) cout << "==>kalmanVertexFit> Starting fitter TransSecVtx1" << endl;
		    TransientVertex TransSecVtx1 = theFitter.vertex(RecoTransientTrack); 
		    
		    bool goodvertex = false;
		    if ( TransSecVtx1.isValid() ) {
		      if (fVerbose) 
			cout << "==>kalmanVertexFit> KVF successful!" << endl; 
		      
		      if ( isnan(TransSecVtx1.position().x()) || isnan(TransSecVtx1.position().y()) || isnan(TransSecVtx1.position().z()) ) {
			if (fVerbose) 
			  cout << "==>kalmanVertexFit> Something went wrong! SecVtx nan - Aborting... !" << endl;
		      }
		      else {
			goodvertex = true;
		      }
		    } else { 
		      if (fVerbose) 
			cout << "==>kalmanVertexFit> KVF failed! Aborting... !" << endl;
		    }
		    
		    if (goodvertex) {
		      pCand->fv1 = TransSecVtx1.normalisedChiSquared();
		      if (fVerbose > 0)
			cout << "==>chi2 " << pCand->fv1 << endl;
		      
		    }
		    //**********************************************
		    //calculate secondary vertex of D0 and muon
		    //**********************************************
		    if (fVerbose > 0)
		      cout << "==>vertex of D0 candidate and muon " << endl;
		    RecoTransientTrack.clear();
		    		    
		    //get transient track 
		    RecoTransientTrack.push_back( (theB->build(&(*kaon)  )) );
		    RecoTransientTrack.push_back( (theB->build(&(*pion)  )) ); 
		    RecoTransientTrack.push_back( (theB->build(&(*(muon->track()))))); 
		    //get secondary vertex 
		    if (fVerbose) cout << "==>kalmanVertexFit> Vertexing with " << RecoTransientTrack.size() << " trans. tracks " << endl;
		    
		    if (fVerbose) cout << "==>kalmanVertexFit> Starting fitter TransSecVtx2" << endl;
		    TransientVertex TransSecVtx2 = theFitter.vertex(RecoTransientTrack); 
		    
		    goodvertex = false;
		    if ( TransSecVtx2.isValid() ) {
		      if (fVerbose) 
			cout << "==>kalmanVertexFit> KVF successful!" << endl; 
		      
		      if ( isnan(TransSecVtx2.position().x()) || isnan(TransSecVtx2.position().y()) || isnan(TransSecVtx2.position().z()) ) {
			if (fVerbose) 
			  cout << "==>kalmanVertexFit> Something went wrong! SecVtx nan - Aborting... !" << endl;
		      }
		      else {
			goodvertex = true;
		      }
		    } else { 
		      if (fVerbose) 
			cout << "==>kalmanVertexFit> KVF failed! Aborting... !" << endl;
		    }
		    
		    if (goodvertex) {
		      pCand->fv2 = TransSecVtx2.normalisedChiSquared();
		      if (fVerbose > 0)
			cout << "==>chi2 " << pCand->fv2 << endl;
		    }
		    
		    //**********************************************
		    //calculate secondary vertex of D0, muon and slow pion
		    //**********************************************
		    if (fVerbose > 0)
		      cout << "==>vertex of D0, muon and slow pion " << endl;
		    RecoTransientTrack.clear();
		 
		    //get transient track 
		    RecoTransientTrack.push_back( (theB->build(&(*kaon)  )) );
		    RecoTransientTrack.push_back( (theB->build(&(*pion)  )) ); 
		    RecoTransientTrack.push_back( (theB->build(&(*(muon->track())))));
		    RecoTransientTrack.push_back( (theB->build(&(*slowpion)  )) );
		    //get secondary vertex 
		    if (fVerbose) cout << "==>kalmanVertexFit> Vertexing with " << RecoTransientTrack.size() << " trans. tracks " << endl;
		    if (fVerbose) cout << "==>kalmanVertexFit> Starting fitter TransSecVtx3" << endl;
		    TransientVertex TransSecVtx3 = theFitter.vertex(RecoTransientTrack); 
		    
		    goodvertex = false;
		    if ( TransSecVtx3.isValid() ) {
		      if (fVerbose) 
			cout << "==>kalmanVertexFit> KVF successful!" << endl; 
		      
		      if ( isnan(TransSecVtx3.position().x()) || isnan(TransSecVtx3.position().y()) || isnan(TransSecVtx3.position().z()) ) {
			if (fVerbose) 
			  cout << "==>kalmanVertexFit> Something went wrong! SecVtx nan - Aborting... !" << endl;
		      }
		      else {
			goodvertex = true;
		      }
		    } else { 
		      if (fVerbose) 
			cout << "==>kalmanVertexFit> KVF failed! Aborting... !" << endl;
		    }
	    
		    if (goodvertex) {
		      pCand->fv3 = TransSecVtx3.normalisedChiSquared();
		      if (fVerbose > 0)
			cout << "==>chi2 " << pCand->fv3 << endl;
		    }

		    //**********************************************
		    //calculate secondary vertex of muon and slow pion
		    //**********************************************
		    if (fVerbose > 0)
		      cout << "==>vertex of muon and slow pion" << endl;
		    RecoTransientTrack.clear();
		 
		    //get transient track 
		    RecoTransientTrack.push_back( (theB->build(&(*(muon->track())))));
		    RecoTransientTrack.push_back( (theB->build(&(*slowpion)  )) );
		    //get secondary vertex 
		    if (fVerbose) cout << "==>kalmanVertexFit> Vertexing with " << RecoTransientTrack.size() << " trans. tracks " << endl;
		    if (fVerbose) cout << "==>kalmanVertexFit> Starting fitter TransSecVtx4" << endl;
		    TransientVertex TransSecVtx4 = theFitter.vertex(RecoTransientTrack); 
		    
		    goodvertex = false;
		    if ( TransSecVtx4.isValid() ) {
		      if (fVerbose) 
			cout << "==>kalmanVertexFit> KVF successful!" << endl; 
		      
		      if ( isnan(TransSecVtx4.position().x()) || isnan(TransSecVtx4.position().y()) || isnan(TransSecVtx4.position().z()) ) {
			if (fVerbose) 
			  cout << "==>kalmanVertexFit> Something went wrong! SecVtx nan - Aborting... !" << endl;
		      }
		      else {
			goodvertex = true;
		      }
		    } else { 
		      if (fVerbose) 
			cout << "==>kalmanVertexFit> KVF failed! Aborting... !" << endl;
		    }
	    
		    if (goodvertex) {
		      pCand->fv4 = TransSecVtx4.normalisedChiSquared();
		      if (fVerbose > 0)
			cout << "==>chi2 " << pCand->fv4 << endl;
		    }
		    
		    
		    
		    //  } 
		    // catch (Exception event) {
		    //  cout << "%% -- No TransientTrackRecord (2) with label TransientTrackBuilder" << endl;
		    //  }
		    //*************************************
		    
		    
		  }
		  //*******************************************slow pion end
		  
		  
		  //*******************************************************************************
		  //search for wrong sign pion
		  //******************************************************************************* 
		  //use fSig4 for index of wrong sign pion track
		  pCand->fSig4      = -9999;
		  pCand->fv5        = -9999;
		  pCand->fv6        = -9999;
		  pCand->fv7        = -9999;
		  pCand->fv8        = -9999; 
		  
		  
		  pCand->fSig4      = GetWrongSignPion(iEvent,muvect, (muon->track()).index(), k_index, pion->charge());
		  if (pCand->fSig4>-1) { 
		    
		    reco::TrackRef wrongpion(recTrks, pCand->fSig4);
		    if (fVerbose > 0) {
		      cout << "==>found a wrong sign pion " << pCand->fSig4 << endl;
		      cout << "muon charge  " << muon->charge() << endl;
		      cout << "kaon charge  " << kaon->charge() << endl;
		      cout << "pion charge  " << pion->charge() << endl;
		      cout << "wpion charge " << wrongpion->charge() << endl << endl;
		      cout << "muon index  " << index_muontrack << endl;
		      cout << "kaon index  " << k_index << endl;
		      cout << "pion index  " << p_index << endl;
		      
		    }
		    
		    //**********************************************
		    //calculate secondary vertices
		    //**********************************************
		    try {
		      
		      edm::ESHandle<TransientTrackBuilder> theB;
		      iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theB);
		      
		      //**********************************************
		      //calculate secondary vertex of D0 and wrong pion
		      //**********************************************
		      if (fVerbose > 0)
			cout << "==>vertex of D0 candidate and wrong pion " << endl;
		      RecoTransientTrack.clear();
		      
		      //get transient track 
		      RecoTransientTrack.push_back( (theB->build(&(*kaon)  )) );
		      RecoTransientTrack.push_back( (theB->build(&(*pion)  )) ); 
		      RecoTransientTrack.push_back( (theB->build(&(*wrongpion)  )) );
		      //get secondary vertex 
		      if (fVerbose) cout << "==>kalmanVertexFit> Vertexing with " << RecoTransientTrack.size() << " trans. tracks " << endl;
		      KalmanVertexFitter theFitter(true);
		      
		      if (fVerbose) cout << "==>kalmanVertexFit> Starting fitter TransSecVtx1" << endl;
		      TransientVertex TransSecVtx1 = theFitter.vertex(RecoTransientTrack); 
		      
		      bool goodvertex = false;
		      if ( TransSecVtx1.isValid() ) {
			if (fVerbose) 
			  cout << "==>kalmanVertexFit> KVF successful!" << endl; 
			
			if ( isnan(TransSecVtx1.position().x()) || isnan(TransSecVtx1.position().y()) || isnan(TransSecVtx1.position().z()) ) {
			  if (fVerbose) 
			    cout << "==>kalmanVertexFit> Something went wrong! SecVtx nan - Aborting... !" << endl;
			}
			else {
			  goodvertex = true;
			}
		      } else { 
			if (fVerbose) 
			  cout << "==>kalmanVertexFit> KVF failed! Aborting... !" << endl;
		      }
		      
		      if (goodvertex) {
			pCand->fv5 = TransSecVtx1.normalisedChiSquared();
			if (fVerbose > 0)
			  cout << "==>chi2 " << pCand->fv5 << endl;
			
		      }
		      //**********************************************
		      //calculate secondary vertex of D0, muon and wrong pion
		      //**********************************************
		      if (fVerbose > 0)
			cout << "==>vertex of D0, muon and wrong pion " << endl;
		      RecoTransientTrack.clear();
		      
		      //get transient track 
		      RecoTransientTrack.push_back( (theB->build(&(*kaon)  )) );
		      RecoTransientTrack.push_back( (theB->build(&(*pion)  )) ); 
		      RecoTransientTrack.push_back( (theB->build(&(*(muon->track())))));
		      RecoTransientTrack.push_back( (theB->build(&(*wrongpion)  )) );
		      //get secondary vertex 
		      if (fVerbose) cout << "==>kalmanVertexFit> Vertexing with " << RecoTransientTrack.size() << " trans. tracks " << endl;
		      if (fVerbose) cout << "==>kalmanVertexFit> Starting fitter TransSecVtx3" << endl;
		      TransientVertex TransSecVtx3 = theFitter.vertex(RecoTransientTrack); 
		      
		      goodvertex = false;
		      if ( TransSecVtx3.isValid() ) {
			if (fVerbose) 
			  cout << "==>kalmanVertexFit> KVF successful!" << endl; 
			
			if ( isnan(TransSecVtx3.position().x()) || isnan(TransSecVtx3.position().y()) || isnan(TransSecVtx3.position().z()) ) {
			if (fVerbose) 
			  cout << "==>kalmanVertexFit> Something went wrong! SecVtx nan - Aborting... !" << endl;
			}
			else {
			  goodvertex = true;
			}
		      } else { 
			if (fVerbose) 
			  cout << "==>kalmanVertexFit> KVF failed! Aborting... !" << endl;
		      }
		      
		      if (goodvertex) {
			pCand->fv6 = TransSecVtx3.normalisedChiSquared();
			if (fVerbose > 0)
			  cout << "==>chi2 " << pCand->fv6 << endl;
		      }
		      
		      //**********************************************
		      //calculate secondary vertex of muon and wrong pion
		      //**********************************************
		      if (fVerbose > 0)
			cout << "==>vertex of muon and wrong pion" << endl;
		      RecoTransientTrack.clear();
		      
		      //get transient track 
		      RecoTransientTrack.push_back( (theB->build(&(*(muon->track())))));
		      RecoTransientTrack.push_back( (theB->build(&(*wrongpion)  )) );
		      //get secondary vertex 
		      if (fVerbose) cout << "==>kalmanVertexFit> Vertexing with " << RecoTransientTrack.size() << " trans. tracks " << endl;
		      if (fVerbose) cout << "==>kalmanVertexFit> Starting fitter TransSecVtx4" << endl;
		      TransientVertex TransSecVtx4 = theFitter.vertex(RecoTransientTrack); 
		      
		      goodvertex = false;
		      if ( TransSecVtx4.isValid() ) {
			if (fVerbose) 
			  cout << "==>kalmanVertexFit> KVF successful!" << endl; 
			
			if ( isnan(TransSecVtx4.position().x()) || isnan(TransSecVtx4.position().y()) || isnan(TransSecVtx4.position().z()) ) {
			  if (fVerbose) 
			    cout << "==>kalmanVertexFit> Something went wrong! SecVtx nan - Aborting... !" << endl;
			}
			else {
			  goodvertex = true;
			}
		      } else { 
			if (fVerbose) 
			  cout << "==>kalmanVertexFit> KVF failed! Aborting... !" << endl;
		      }
		      
		      if (goodvertex) {
			pCand->fv7 = TransSecVtx4.normalisedChiSquared();
			if (fVerbose > 0)
			  cout << "==>chi2 " << pCand->fv7 << endl;
		      }
		      
		    } 
		    catch (Exception event) {
		      cout << "%% -- No TransientTrackRecord (3) with label TransientTrackBuilder" << endl;
		    }
		    //*************************************
		    
		    
		  }
		  //*******************************************end wrong sign pion end
		  i++;
		}
	      }
	      
	      p_index++;
	    }
	  }
	  
	}
	
	k_index++;
      }
      
      
      index++;
    }
    
  }
  
}


int HFDumpCandidate::GetSlowPion(const edm::Event& iEvent,TVector3 muonvector, int muontrack_index, int pion_index, int pion_charge) {


  Handle<reco::TrackCollection> recTrks;
  iEvent.getByLabel(fTrackLabel.c_str(), recTrks);
  
  int index=-9999;
  int p2_index=0;
  double drmin = 9999;
  for (reco::TrackCollection::const_iterator pion2=recTrks->begin(); pion2 < recTrks->end(); ++pion2){ 
    //+pt cut
    if (pion_index != p2_index && muontrack_index != p2_index && pion_charge == pion2->charge() && pion2->pt() > fTrackPtMin) {
      //dr cut  
      TVector3 pion2vect; pion2vect.SetPtEtaPhi(pion2->pt(),pion2->eta(),pion2->phi());
      if (muonvector.DeltaR(pion2vect)<drmin && muonvector.DeltaR(pion2vect)<fDeltaR ) {
	
	index = p2_index;
	drmin = muonvector.DeltaR(pion2vect);
      }
    }
    
    p2_index++;
  }
  
  return index;
}

int HFDumpCandidate::GetWrongSignPion(const edm::Event& iEvent,TVector3 muonvector, int muontrack_index, int kaon_index, int pion_charge) {


  Handle<reco::TrackCollection> recTrks;
  iEvent.getByLabel(fTrackLabel.c_str(), recTrks);
  
  int index=-9999;
  int p2_index=0;
  double drmin = 9999;
  for (reco::TrackCollection::const_iterator pion2=recTrks->begin(); pion2 < recTrks->end(); ++pion2){ 
    //+pt cut
    if (kaon_index != p2_index && muontrack_index != p2_index && pion_charge != pion2->charge() && pion2->pt() > fTrackPtMin) {
      //dr cut  
      TVector3 pion2vect; pion2vect.SetPtEtaPhi(pion2->pt(),pion2->eta(),pion2->phi());
      if (muonvector.DeltaR(pion2vect)<drmin && muonvector.DeltaR(pion2vect)<fDeltaR ) {
	
	index = p2_index;
	drmin = muonvector.DeltaR(pion2vect);
      }
    }
    
    p2_index++;
  }
  
  return index;
}

// ------------ method called once each job just before starting event loop  ------------
//void  HFDumpCandidate::beginJob(const edm::EventSetup& setup) {
void  HFDumpCandidate::beginJob() {
  nevt=0;
}

// ------------ method called once each job just after ending the event loop  ------------
void  HFDumpCandidate::endJob() { 
  cout << "HFDumpCandidate>  Summary: Events processed: " << nevt << endl; 
  
}

//define this as a plug-in
DEFINE_FWK_MODULE(HFDumpCandidate);
