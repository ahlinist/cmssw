#include <iostream>

#include "HeavyFlavorAnalysis/InclB/interface/HFDumpCandidateNew.h"

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
HFDumpCandidateNew::HFDumpCandidateNew(const edm::ParameterSet& iConfig) :
  fVerbose(iConfig.getUntrackedParameter<int>("verbose", 0)),
  fTrackPtMin(iConfig.getUntrackedParameter<double>("trackptmin", 3)),
  fMuonPtMin(iConfig.getUntrackedParameter<double>("muonptmin", 3)), 
  fMuonEta(iConfig.getUntrackedParameter<double>("muoneta", 2.1)),
  fMassLow(iConfig.getUntrackedParameter<double>("masslow", 1.7)), 
  fMassHigh(iConfig.getUntrackedParameter<double>("masshigh", 2)),
  fChi2(iConfig.getUntrackedParameter<double>("chi2", 10)),
  fDeltaR(iConfig.getUntrackedParameter<double>("deltaR", 1)),
  fMuonLabel(iConfig.getUntrackedParameter<string>("muonLabel", string("globalMuons"))),
  fTrackLabel(iConfig.getUntrackedParameter<string>("trackLabel", string("generalTracks")))
{
  using namespace std;
  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- HFDumpCandidateNew constructor" << endl;
  cout << "--- " << fMuonLabel.c_str() << endl;
  cout << "----------------------------------------------------------------------" << endl;

}


// ----------------------------------------------------------------------
HFDumpCandidateNew::~HFDumpCandidateNew() {
  
}


// ----------------------------------------------------------------------
void HFDumpCandidateNew::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  nevt++;

  edm::Handle<reco::MuonCollection> muons;
  iEvent.getByLabel( fMuonLabel.c_str(), muons);
  
  const reco::MuonCollection & muonsproduct = *(muons.product());
         
  Handle<reco::TrackCollection> recTrks;
  iEvent.getByLabel(fTrackLabel.c_str(), recTrks); 
   
  TAnaCand *pCand;
 
  int i = 0;

  //cout << "==========> Event " << nevt << endl;

  //highest pt reco global muon
  int muon_index = GetHighestPtGlobalMuon(iEvent,fMuonPtMin, fMuonEta);
  if (muon_index>-1) {
    reco::Muon muon = muonsproduct[muon_index]; 
    int muontrack_index = (muon.track()).index();
    if (fVerbose > 0)
      cout << "Muon Candidate: pt " << muon.pt() << " phi " << muon.phi() << " eta " << muon.eta() << endl;
       
    //track with pt> ptmin, eta<2.1, deltaR(mu,track)>1, assumed to be kaon
    int k_index=0;
    for (reco::TrackCollection::const_iterator kaon=recTrks->begin(); kaon < recTrks->end(); ++kaon){ 
      //+pt cut, eta cut 
      if (k_index != muontrack_index && kaon->pt() > fTrackPtMin && fabs(kaon->eta())<fMuonEta ) {
	
	//dr cut 
	TVector3 muvect; muvect.SetPtEtaPhi(muon.pt(),muon.eta(),muon.phi());
	TLorentzVector kaonvect; kaonvect.SetPtEtaPhiM(kaon->pt(),kaon->eta(),kaon->phi(),mkaon);
	//cout << "deltaR " <<  muvect.DeltaR((kaonvect.Vect())) << endl; 
	if (muvect.DeltaR((kaonvect.Vect())) > fDeltaR) { 
	  if (fVerbose > 0)
	  cout << "Kaon Candidate " << k_index << ": pt " << kaon->pt() << " phi " << kaon->phi() << " eta " << kaon->eta() << endl; 
	 	  
	  //second track with opposite charge (=pion from D0 decay) with pt> ptmin, eta<2.1, deltaR(mu,track)>1
	  int p_index=0;
	  for (reco::TrackCollection::const_iterator pion=recTrks->begin(); pion < recTrks->end(); ++pion){ 
	    //+pt cut, eta cut, charge
	    if (p_index != muontrack_index && p_index != k_index && pion->pt() > fTrackPtMin && fabs(pion->eta())<fMuonEta && pion->charge()!= kaon->charge()) {
	      
	      //dr cut  
	      TLorentzVector pionvect; pionvect.SetPtEtaPhiM(pion->pt(),pion->eta(),pion->phi(),mpion);	
	      if (muvect.DeltaR((pionvect.Vect())) > fDeltaR) { 
		if (fVerbose > 0)
		  cout << "Pion Candidate " << p_index << ": pt " << pion->pt() << " phi " << pion->phi() << " eta " << pion->eta() << endl;  
		
		double invmass = (kaonvect+pionvect).M();
		if (fVerbose)
		  cout << "mass_d0 " << invmass << endl;
		if (invmass > fMassLow && invmass < fMassHigh) {
		  
		  //secondary vertex  
		  //*******************************************
		  //calculate secondary vertex of pion and kaon
		  //*******************************************
		  // -- Kalman Vertex Fit
		  std::vector<reco::TransientTrack> RecoTransientTrack;
		  RecoTransientTrack.clear();
		  TAnaVertex *pVtx;
		  pVtx = new TAnaVertex();
			    
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
		  
		  if (goodvertex) {
		    
		    pVtx->fPoint.SetXYZ(TransSecVtx.position().x()*10,TransSecVtx.position().y()*10,TransSecVtx.position().z()*10);//*10 to get mm (same unit as gen info)
		    pVtx->setInfo(TransSecVtx.normalisedChiSquared(), TransSecVtx.degreesOfFreedom(), -1., -1, -1);
		  	    
		  }
		  else {
		    pVtx->fPoint.SetXYZ(-9999.,-9999.,-9999.);
		    pVtx->setInfo(-1., -1, -1., -1, -1);
		  }
		  
		  if (fVerbose)
		    cout << "chi2_pionkaon " << pVtx->fChi2 << endl;
		  //*******************************************
		  //******************************************* 
		  if (pVtx->fChi2 < fChi2 ) {
		    //search for slow pion 
		    
		    //slow pion with pt> ptmin, eta<2.1, deltaR(mu,track)>1
		    int pslow_index=0;
		    for (reco::TrackCollection::const_iterator pionslow=recTrks->begin(); pionslow < recTrks->end(); ++pionslow){ 
		      //+pt cut, eta cut, charge
		      if (pslow_index != muontrack_index && pslow_index != k_index && pslow_index != p_index && pionslow->pt() > fTrackPtMin && fabs(pionslow->eta())<fMuonEta ) {
			
			//dr cut  
			TLorentzVector pionslowvect; pionslowvect.SetPtEtaPhiM(pionslow->pt(),pionslow->eta(),pionslow->phi(),mpion);
			if (muvect.DeltaR((pionslowvect.Vect())) > fDeltaR) { 
			  if (fVerbose > 0)
			    cout << "PionSLow Candidate " << pslow_index << ": pt " << pionslow->pt() << " phi " << pionslow->phi() << " eta " << pionslow->eta() << " charge " << pionslow->charge() << endl;
			  
			  //**********************************************
			  //calculate secondary vertex of D0 and slow pion
			  //**********************************************
			  double chi2_kaonpionpion = -1;
			  if (fVerbose > 0)
			    cout << "==>vertex of D0 candidate and slow pion " << endl;
			  RecoTransientTrack.clear();
			  
			  //get transient track 
			  RecoTransientTrack.push_back( (theB->build(&(*kaon)  )) );
			  RecoTransientTrack.push_back( (theB->build(&(*pion)  )) ); 
			  RecoTransientTrack.push_back( (theB->build(&(*pionslow)  )) );
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
			    chi2_kaonpionpion = TransSecVtx1.normalisedChiSquared();
			  			    
			  }
	 
			  if (fVerbose)
			    cout << "chi2_kaonpionpion " << chi2_kaonpionpion << endl;
			  if (chi2_kaonpionpion<fChi2) {

			    //-->add cand
			    if (fVerbose > 0)
			      cout << "Add Candidate " << i << endl;
			    pCand            = gHFEvent->addD0Cand();
			    pCand->fMCID     = 1;                    //type of DO cand
			    pCand->fType     = muon_index;            //muon index
			    pCand->fIndex    = i;                     //cand index
			    pCand->fSig1     = k_index;               //kaon index
			    pCand->fSig2     = p_index;               //pion index  
			    pCand->fSig3     = pslow_index;           //pion index 
			    pCand->fv1       = chi2_kaonpionpion;
			    pCand->fPlab.SetPtEtaPhi((kaonvect+pionvect).Pt(),(kaonvect+pionvect).Eta(),(kaonvect+pionvect).Phi());
			    pCand->fMass     = invmass;
			    pCand->fVtx = *pVtx;
			    i++;
			  } 
			}
			
		      }
		      pslow_index++;
		    }
		  } 
		}
	      }
	    }
	    p_index++;
	  }

	}
      
      }
      k_index++;
    }

  }
  
  
}

// ----------------------------------------------------------------------
int HFDumpCandidateNew::GetHighestPtGlobalMuon(const edm::Event& iEvent,double ptmin_muon, double eta_muon) { 

  edm::Handle<reco::MuonCollection> muons;
  iEvent.getByLabel( fMuonLabel.c_str(), muons);

  int index = -9999;
  double ptmax = ptmin_muon;
  int i = 0;
  for (  reco::MuonCollection::const_iterator muon = muons->begin(); muon != muons->end(); ++ muon ) {  
   
    if ( muon->isGlobalMuon() && muon->pt()>ptmax && fabs(muon->eta()) < eta_muon) {
      ptmax = muon->pt();
      index = i;
    }
    i++;
  }
  return index;
}

int HFDumpCandidateNew::GetSlowPion(const edm::Event& iEvent,TVector3 muonvector, int muontrack_index, int pion_index, int pion_charge) {


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

// ------------ method called once each job just before starting event loop  ------------
//void  HFDumpCandidateNew::beginJob(const edm::EventSetup& setup) {
void  HFDumpCandidateNew::beginJob() {
  nevt=0;
}

// ------------ method called once each job just after ending the event loop  ------------
void  HFDumpCandidateNew::endJob() { 
  cout << "HFDumpCandidateNew> Summary: Events processed: " << nevt << endl;
}

//define this as a plug-in
DEFINE_FWK_MODULE(HFDumpCandidateNew);
