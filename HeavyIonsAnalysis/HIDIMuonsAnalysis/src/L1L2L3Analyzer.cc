// -*- C++ -*-
//
// Package:    L1L2L3Analyzer
// Class:      L1L2L3Analyzer
// vesrion v13 -
/**\class L1L2L3Analyzer L1L2L3Analyzer.cc UserCode/L1L2L3Analyzer/src/L1L2L3Analyzer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Dong Ho Moon
//         Created:  Fri Nov 21 17:27:33 CET 2008
// $Id: L1L2L3Analyzer.cc,v 1.3 2009/02/20 00:39:56 dmoon Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

// added by moon

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include <vector>
#include <iostream>

#include "PhysicsTools/CandUtils/interface/CandCombiner.h"
#include "PhysicsTools/CandUtils/interface/CandCombiner.h"
#include "DataFormats/Candidate/interface/CompositeRefCandidate.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTReadoutRecord.h"
#include "DataFormats/L1Trigger/interface/L1MuonParticle.h"
#include "L1Trigger/L1ExtraFromDigis/interface/L1ExtraParticlesProd.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"

#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Math/interface/Point3D.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"

#include "DataFormats/Math/interface/Point3D.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Common/interface/Ref.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"

#include <TH1.h>
#include <TFile.h>
#include <TString.h>
#include <stdio.h>
#include <TH2F.h>
#include <TNtuple.h>

#include <TLorentzVector.h>


//
// class decleration
//

class L1L2L3Analyzer : public edm::EDAnalyzer {
   public:
      explicit L1L2L3Analyzer(const edm::ParameterSet&);
      ~L1L2L3Analyzer();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------
   edm::InputTag l1muon_, l2muon_, l3muon_, l2muonCand_, l3muonCand_;
   int muTracks_;
   //double min_Pt_;
   std::string fOutputFileName_;
   	
   TFile *hOutputFile;
   TNtuple *nL1Muon, *nL2Muon, *nL3Muon;
   TNtuple *nL2MuonCand, *nL3MuonCand, *nL3DiMuCand;
   TNtuple *nL1DoubleMuOpenFil, *nL2MuonFil, *nL3MuonFil;
   TNtuple *nL1DoubleMu3Fil, *nL2Muon3Fil, *nL3Muon3Fil;

   TNtuple *nL2DoubleMuOpenFil, *nL3DoubleMuOpenFil;
   TNtuple *nL2DoubleMu3Fil, *nL3DoubleMu3Fil;
   
   TNtuple *nL2DoubleMuOpenGenMuon, *nL2DoubleMuOpenGenJpsi, *nL2DoubleMuOpenGenJpsiToMM, *nL2DoubleMuOpenGenMuFromJpsi;
   TNtuple *nL2DoubleMu3GenMuon, *nL2DoubleMu3GenJpsi, *nL2DoubleMu3GenJpsiToMM, *nL2DoubleMu3GenMuFromJpsi;
   TNtuple *nL3DoubleMuOpenGenMuon, *nL3DoubleMuOpenGenJpsi, *nL3DoubleMuOpenGenJpsiToMM, *nL3DoubleMuOpenGenMuFromJpsi;
   TNtuple *nL3DoubleMu3GenMuon, *nL3DoubleMu3GenJpsi, *nL3DoubleMu3GenJpsiToMM, *nL3DoubleMu3GenMuFromJpsi;

   TNtuple *nGenJpsi, *nGenMuon;
   TNtuple *nGenMuFromJpsi, *nGenJpsiToMM;
   TNtuple *nNL2Cand, *nNL3Cand; 
   
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
L1L2L3Analyzer::L1L2L3Analyzer(const edm::ParameterSet& iConfig):
l1muon_(iConfig.getUntrackedParameter<edm::InputTag>("L1muons")),
l2muon_(iConfig.getUntrackedParameter<edm::InputTag>("L2muons")),
l3muon_(iConfig.getUntrackedParameter<edm::InputTag>("L3muons")),
l2muonCand_(iConfig.getUntrackedParameter<edm::InputTag>("L2muonCands")),
l3muonCand_(iConfig.getUntrackedParameter<edm::InputTag>("L3muonCands")),
muTracks_(iConfig.getUntrackedParameter("muTracks",1)),
//min_Pt_(iConfig.getParameter<double> ("MinPt")),
fOutputFileName_(iConfig.getUntrackedParameter<std::string>("HistOutFile"))
{
   //now do what ever initialization is needed
}


L1L2L3Analyzer::~L1L2L3Analyzer()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called to for each event  ------------
void
L1L2L3Analyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace reco;
   using namespace std;

#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
	// genParticleCandidates taking
   Handle<GenParticleCollection> genParticles;
   iEvent.getByLabel("genParticles", genParticles);
   
   Handle< vector<l1extra::L1MuonParticle> > l1muons;
   iEvent.getByLabel(l1muon_, l1muons);
   
   l1extra::L1MuonParticleCollection::const_iterator muItr = l1muons->begin() ;
   l1extra::L1MuonParticleCollection::const_iterator muEnd = l1muons->end() ;

   double l3mucand_char=0., l3mucand_pt=0., l3mucand_eta=0., l3mucand_phi=0.;
   double l2mucand_char=0., l2mucand_pt=0., l2mucand_eta=0., l2mucand_phi=0.;
   double l1mu_char=0., l1mu_pt=0., l1mu_eta=0., l1mu_phi=0.;
   int pL1DM3 = 0, pL1DM0 = 0, pL2DM3 = 0, pL2DM0 = 0, pL3DM3 = 0, pL3DM0 = 0;
  
   int nL1Muon_ = l1muons->size();
   // L1 muons
   if ( nL1Muon_ > 0 ){
   	for ( int i=0 ; i < nL1Muon_; i++ ){
           // when using trigger
	   const l1extra::L1MuonParticle & l1mu1 = (*l1muons)[i];
	   	
	   	 l1mu_char=l1mu1.charge(), l1mu_pt=l1mu1.pt(), l1mu_eta=l1mu1.eta(), l1mu_phi=l1mu1.phi();
	   	 nL1Muon->Fill(l1mu1.pt(),l1mu1.eta(),l1mu1.phi());
	}
	   	 
	  if(nL1Muon_>= 2){
  	    for( size_t i = 0 ; muItr != muEnd ; ++muItr, ++i )
      	    {
		if((*muItr).pt()>0 && abs((*muItr).eta())<2.5){
			unsigned int qual = muItr->gmtMuonCand().quality() ;
			if( qual ==2 || qual == 3 || qual == 5 || qual == 6 || qual == 7 )
			{
	           		nL1DoubleMuOpenFil->Fill((*muItr).pt(),(*muItr).eta(),(*muItr).phi());
		   		pL1DM0++;
			}
            
			if(qual == 3 || qual == 5 || qual == 6 || qual == 7 ){
				if((*muItr).pt()>=3){
					nL1DoubleMu3Fil->Fill((*muItr).pt(),(*muItr).eta(),(*muItr).phi());
					pL1DM3++;
				}
			}
		}
	    }
          }
        
  }
   

   // L2 muon candidates
   Handle<RecoChargedCandidateCollection> l2muonCands;
   iEvent.getByLabel(l2muonCand_, l2muonCands);
   int NL2 = 0;
   int nL2MuonC_ = l2muonCands->size();

   nNL2Cand->Fill(nL2MuonC_);
    
   if(nL2MuonC_>0){
	 for ( int i=0 ; i < nL2MuonC_ ; i++ ){
           const RecoChargedCandidate & l2muCand1 = (*l2muonCands)[i];
	     
	   l2mucand_char=l2muCand1.charge(), l2mucand_pt=l2muCand1.pt(), l2mucand_eta=l2muCand1.eta(), l2mucand_phi=l2muCand1.phi();
	   NL2++;
	   nL2MuonCand->Fill(l2muCand1.pt(),l2muCand1.eta(),l2muCand1.phi(),NL2);
		if(nL2MuonC_>= 2 && fabs(l2mucand_eta)<2.5 ) {
		if(l2mucand_pt>0) {pL2DM0++;}//for // L2 DoubleMuOpen Filtered Generation info
		if(l2mucand_pt>=3){pL2DM3++;}
		}
	 }		
   }

   if(pL2DM3 >= 2){
        int numOfgenPaticles = genParticles->size();
        for( int k = 0; k < numOfgenPaticles; k++ ){ //for
                const Candidate & p = (*genParticles)[k];
                int id = p.pdgId();
                        if(abs(id) == 13) {// if 1
                                nL2DoubleMu3GenMuon->Fill(p.pt(),p.eta(),p.phi(),p.mass());
                                if(id == 13){
                                int n=p.numberOfMothers();
                                for(int j=0; j<n ; j++){
                                        const Candidate * m = p.mother(j);
                                        int mj_id = m->pdgId();
                                        if(mj_id == 443){
                                                nL2DoubleMu3GenJpsiToMM->Fill(m->pt(),m->eta(),m->phi(),m->mass());
                                        }
                                }
                                }
                        }//if -1

                        if( id == 443 )
                        {//if 2
                                nL2DoubleMu3GenJpsi->Fill(p.pt(),p.eta(),p.phi(),p.mass());
                                int n=p.numberOfDaughters();
                                for(int j=0; j<n ; j++){//for 2
                                        const Candidate * d = p.daughter(j);
                                        int dj_id = d->pdgId();
                                        if(abs(dj_id) == 13){//if 3
                                                nL2DoubleMu3GenMuFromJpsi->Fill(d->pt(),d->eta(),d->phi(),d->mass());
                                        }//if -3
                                }//for 2
                        }//if 2
        }// L2 DoubleMu3 Filtered Generation info
   }

  if(pL2DM0 >= 2){
        int numOfgenPaticles = genParticles->size();
        for( int k = 0; k < numOfgenPaticles; k++ ){ //for
                const Candidate & p = (*genParticles)[k];
                int id = p.pdgId();
                        if(abs(id) == 13) {// if 1
                                nL2DoubleMuOpenGenMuon->Fill(p.pt(),p.eta(),p.phi(),p.mass());
                                if(id == 13){
                                int n=p.numberOfMothers();
                                for(int j=0; j<n ; j++){
                                        const Candidate * m = p.mother(j);
                                        int mj_id = m->pdgId();
                                        if(mj_id == 443){
                                                nL2DoubleMuOpenGenJpsiToMM->Fill(m->pt(),m->eta(),m->phi(),m->mass());
                                        }
                                }
                                }
                        }//if -1

                        if( id == 443 )
                        {//if 2
                                nL2DoubleMuOpenGenJpsi->Fill(p.pt(),p.eta(),p.phi(),p.mass());
                                int n=p.numberOfDaughters();
                                for(int j=0; j<n ; j++){//for 2
                                        const Candidate * d = p.daughter(j);
                                        int dj_id = d->pdgId();
                                        if(abs(dj_id) == 13){//if 3
                                                nL2DoubleMuOpenGenMuFromJpsi->Fill(d->pt(),d->eta(),d->phi(),d->mass());
                                        }//if -3
                                }//for 2
                        }//if
        }// L2 DoubleMuOpen Filtered Generation info
   }

   
   
   	// L3 muon candidates
   	Handle<RecoChargedCandidateCollection> l3muonCands; // for l3 muon cand
   	//Handle<reco::TrackCollection> l3muonCands; //for l3 pixel3ProtoTracks
   	iEvent.getByLabel(l3muonCand_, l3muonCands);
	int nL3MuonC_ = l3muonCands->size();

	double const MuMass = 0.106;
	double const MuMass2 = MuMass*MuMass;
	double e1,e2;
	Particle::LorentzVector p,p1,p2;
	// for l3 muon cand
	RecoChargedCandidateCollection::const_iterator cand1;
	RecoChargedCandidateCollection::const_iterator cand2;
	// for l3 pixel3ProtoTracks
	//reco::TrackCollection::const_iterator cand1;
	//reco::TrackCollection::const_iterator cand2;
	nNL3Cand->Fill(nL3MuonC_);
   
	//int nL3m3Gen = 0, nL3m0Gen = 0, 
	int NL3 = 0; 
   	if(nL3MuonC_>0){
	for ( int i=0 ; i < nL3MuonC_ ; i++ ){
	   
	const RecoChargedCandidate & l3muCand1 = (*l3muonCands)[i]; // for l3 muon cand
	//const reco::Track & l3muCand1 = (*l3muonCands)[i]; // for l3 pixelProtoTracks
	l3mucand_char=l3muCand1.charge(), l3mucand_pt=l3muCand1.pt(), l3mucand_eta=l3muCand1.eta(), l3mucand_phi=l3muCand1.phi();
	NL3++;
	nL3MuonCand->Fill(l3muCand1.pt(),l3muCand1.eta(),l3muCand1.phi(),NL3);

		// L3 DoubleMuOpen filtered Generation info

		if(nL3MuonC_>= 2 && fabs(l3mucand_eta)<2.5 ) {
		if(l3mucand_pt>0){pL3DM0++;}
		if(l3mucand_pt>=3){pL3DM3++;}
		}
    	}
   	}	
		
	if(nL3MuonC_ >= 2 && pL1DM0 >= 2 && pL2DM0 >= 2 && pL3DM0 >= 2) {
        for (cand1=l3muonCands->begin(); cand1!=l3muonCands->end(); cand1++) {

        TrackRef tk1 = cand1->get<TrackRef>(); //for l3 muon cand
        if(abs(cand1->eta())>2.5) continue;
        	cand2 = cand1; cand2++;
                for (; cand2!=l3muonCands->end(); cand2++) {
                        TrackRef tk2 = cand2->get<TrackRef>(); // for l3 muon cand
                        
                        e1 = sqrt(tk1->momentum().Mag2()+MuMass2);
                        e2 = sqrt(tk2->momentum().Mag2()+MuMass2);
                        p1 = Particle::LorentzVector(tk1->px(),tk1->py(),tk1->pz(),e1);
                        p2 = Particle::LorentzVector(tk2->px(),tk2->py(),tk2->pz(),e2);
			
                        /*
                        // for l3 MuL3
                        if(abs(cand2->eta()>2.5)) continue;
                        e1 = sqrt(cand1->momentum().Mag2()+MuMass2);
                        e2 = sqrt(cand2->momentum().Mag2()+MuMass2);
                        p1 = Particle::LorentzVector(cand1->px(),cand1->py(),cand1->pz(),e1);
                        p2 = Particle::LorentzVector(cand2->px(),cand2->py(),cand2->pz(),e2);
			*/
			
                        p = p1+p2;
                        //double pt12 = p.pt();
                        double invmass = abs(p.mass());
                        nL3DiMuCand->Fill(p.pt(),p.eta(),p.phi(),p.mass());
                }
        }
        }


	if(pL1DM3 >= 2 && pL2DM3 >= 2){
		for ( int i=0 ; i < l2muonCands->size() ; i++ ){
		const RecoChargedCandidate & l2muCand1 = (*l2muonCands)[i];
			nL2DoubleMu3Fil->Fill(l2muCand1.pt(),l2muCand1.eta(),l2muCand1.phi());
		}
	}
	if(pL1DM0 >= 2 && pL2DM0 >= 2){
		for ( int i=0 ; i < l2muonCands->size() ; i++ ){
			const RecoChargedCandidate & l2muCand1 = (*l2muonCands)[i];
			nL2DoubleMuOpenFil->Fill(l2muCand1.pt(),l2muCand1.eta(),l2muCand1.phi());
		}
	}

	if(pL1DM3 >= 2 && pL2DM3 >= 2 && pL3DM3 >= 2){
		for ( int i=0 ; i < l3muonCands->size() ; i++ ){
                        const RecoChargedCandidate & l3muCand1 = (*l3muonCands)[i];
                        nL3DoubleMu3Fil->Fill(l3muCand1.pt(),l3muCand1.eta(),l3muCand1.phi());
                }
	}

	if(pL1DM0 >= 2 && pL2DM0 >= 2 && pL3DM0 >= 2){
		for ( int i=0 ; i < l3muonCands->size() ; i++ ){
                        const RecoChargedCandidate & l3muCand1 = (*l3muonCands)[i];
                        nL3DoubleMuOpenFil->Fill(l3muCand1.pt(),l3muCand1.eta(),l3muCand1.phi());
		}
	}


// L3 DoubleMu3 Gen info
   if( pL1DM3 >= 2 && pL2DM3 >= 2 && pL3DM3 >= 2 ){
        int numOfgenPaticles = genParticles->size();
        for( int k = 0; k < numOfgenPaticles; k++ ){ //for
                const Candidate & p = (*genParticles)[k];
                int id = p.pdgId();
                        if(abs(id) == 13 && abs(p.eta())<2.5) {// if 1
                                nL3DoubleMu3GenMuon->Fill(p.pt(),p.eta(),p.phi(),p.mass());
				if(id == 13){
                                int n=p.numberOfMothers();
				
                                for(int j=0; j<n ; j++){
                                        const Candidate * m = p.mother(j);
                                        int mj_id = m->pdgId();
                                        if(mj_id == 443){
                                                nL3DoubleMu3GenJpsiToMM->Fill(m->pt(),m->eta(),m->phi(),m->mass(),n);
                                        }
                                }
				}
                        }//if -1

                        if( id == 443 )
                        {//if 2
                                nL3DoubleMu3GenJpsi->Fill(p.pt(),p.eta(),p.phi(),p.mass());
                                int n=p.numberOfDaughters();
                                for(int j=0; j<n ; j++){//for 2
                                        const Candidate * d = p.daughter(j);
                                        int dj_id = d->pdgId();
                                	if(abs(dj_id) == 13 && abs(d->eta())<2.5){//if 3
                                        	nL3DoubleMu3GenMuFromJpsi->Fill(d->pt(),d->eta(),d->phi(),d->mass(),n);
                                	}//if -3
                        	}//for
        		}//if 2 
	}// L2 DoubleMu3 Filtered Generation info
   }

// L3 DoubleMuOpen Gen info
   if(pL1DM0 >= 2 && pL2DM0 >= 2 && pL3DM0 >= 2){
        int numOfgenPaticles = genParticles->size();
        for( int k = 0; k < numOfgenPaticles; k++ ){ //for
                const Candidate & p = (*genParticles)[k];
                int id = p.pdgId();
                        if(abs(id) == 13 && abs(p.eta())<2.5) {// if 1
                                nL3DoubleMuOpenGenMuon->Fill(p.pt(),p.eta(),p.phi(),p.mass());
				if(id == 13){
                                int n=p.numberOfMothers();
                                for(int j=0; j<n ; j++){
                                        const Candidate * m = p.mother(j);
                                        int mj_id = m->pdgId();
                                        if(mj_id == 443){
                                                nL3DoubleMuOpenGenJpsiToMM->Fill(m->pt(),m->eta(),m->phi(),m->mass());
                                        }
                                }
				}
                        }//if -1

                        if( id == 443 )
                        {//if 2
                                nL3DoubleMuOpenGenJpsi->Fill(p.pt(),p.eta(),p.phi(),p.mass());
                                int n=p.numberOfDaughters();
                                for(int j=0; j<n ; j++){//for 2
                                        const Candidate * d = p.daughter(j);
                                        int dj_id = d->pdgId();
                                	if(abs(dj_id) == 13 && abs(d->eta())<2.5){//if 3
                                        	nL3DoubleMuOpenGenMuFromJpsi->Fill(d->pt(),d->eta(),d->phi(),d->mass());
                                	}//if -3
                        	}//for
        		}//if 2 
	}//for L3 DoubleMuOpen Filtered Generation info
   }


}
     




// ------------ method called once each job just before starting event loop  ------------
void 
L1L2L3Analyzer::beginJob(const edm::EventSetup&)
{
	hOutputFile    = new TFile( fOutputFileName_.c_str(), "RECREATE" );
	nL1Muon        = new TNtuple("nL1Muon", "L1 Muon ","l1pt:l1eta:l1phi");
	nL2Muon        = new TNtuple("nL2Muon", "L2 Muon","l2pt:l2eta:l2phi");
	nL3Muon        = new TNtuple("nL3Muon", "L3 Muon","l3pt:l3eta:l3phi");
	nL2MuonCand    = new TNtuple("nL2MuonCand", "L2 Muon candidate","l2pt:l2eta:l2phi:NL2");
	nL3MuonCand    = new TNtuple("nL3MuonCand", "L3 Muon candidate","l3pt:l3eta:l3phi:NL3");
	nL3DiMuCand       = new TNtuple("nL3DiMuCand","L3 Reco Jpsi","jpsi_pt:jpsi_eta:jpsi_phi:jpsi_mass");
	nL1DoubleMuOpenFil    = new TNtuple("nL1DoubleMuOpenFil", "L1 Muon Open Filtered","l1pt:l1eta:l1phi");
	nL1DoubleMu3Fil    = new TNtuple("nL1DoubleMu3Fil", "L1 Muon 3 Filtered","l1pt:l1eta:l1phi");
	nL2DoubleMuOpenFil    = new TNtuple("nL2DoubleMuOpenFil", "L2 Cand Muon Filtered","l2pt:l2eta:l2phi:nL2m0Gen");
	nL2DoubleMu3Fil    = new TNtuple("nL2DoubleMu3Fil", "L2 Cand Muon 3 Filtered","l2pt:l2eta:l2phi:nL2m3Gen");
	nL3DoubleMuOpenFil    = new TNtuple("nL3DoubleMuOpenFil", "L3 Cand Muon Filtered","l3pt:l3eta:l3phi:nL3m0Gen");
	nL3DoubleMu3Fil    = new TNtuple("nL3DoubleMu3Fil", "L3 Cand Muon 3 Filtered","l3pt:l3eta:l3phi:nL3m3Gen");
	nGenJpsi       = new TNtuple("nGenJpsi","Gen Jpsi","jpsi_pt:jpsi_eta:jpsi_phi:jpsi_mass");
	nGenMuon       = new TNtuple("nGenMuon","Gen Muon","muon_pt:muon_eta:muon_phi:muon_mass");
	nGenMuFromJpsi = new TNtuple("nGenMuFromJpsi","Gen Jpsi to muon","muon_pt:muon_eta:muon_phi:muon_mass:muon_p");
	nGenJpsiToMM     = new TNtuple("nGenJpsiToMM","Gen Jpsi MM","jpsi_pt:jpsi_eta:jpsi_phi:jpsi_mass");
	
	nL2DoubleMuOpenGenJpsi       = new TNtuple("nL2DoubleMuOpenGenJpsi","Gen Jpsi","jpsi_pt:jpsi_eta:jpsi_phi:jpsi_mass");
        nL2DoubleMuOpenGenMuon       = new TNtuple("nL2DoubleMuOpenGenMuon","Gen Muon","muon_pt:muon_eta:muon_phi:muon_mass");
        nL2DoubleMuOpenGenMuFromJpsi = new TNtuple("nL2DoubleMuOpenGenMuFromJpsi","Gen Jpsi to muon","muon_pt:muon_eta:muon_phi:muon_mass:muon_p");
        nL2DoubleMuOpenGenJpsiToMM     = new TNtuple("nL2DoubleMuOpenGenJpsiToMM","Gen Jpsi MM","jpsi_pt:jpsi_eta:jpsi_phi:jpsi_mass");

	nL3DoubleMuOpenGenJpsi       = new TNtuple("nL3DoubleMuOpenGenJpsi","Gen Jpsi","jpsi_pt:jpsi_eta:jpsi_phi:jpsi_mass");
        nL3DoubleMuOpenGenMuon       = new TNtuple("nL3DoubleMuOpenGenMuon","Gen Muon","muon_pt:muon_eta:muon_phi:muon_mass");
        nL3DoubleMuOpenGenMuFromJpsi = new TNtuple("nL3DoubleMuOpenGenMuFromJpsi","Gen Jpsi to muon","muon_pt:muon_eta:muon_phi:muon_mass:muon_p");
        nL3DoubleMuOpenGenJpsiToMM     = new TNtuple("nL3DoubleMuOpenGenJpsiToMM","Gen Jpsi MM","jpsi_pt:jpsi_eta:jpsi_phi:jpsi_mass");
	
	nL2DoubleMu3GenJpsi       = new TNtuple("nL2DoubleMu3GenJpsi","Gen Jpsi","jpsi_pt:jpsi_eta:jpsi_phi:jpsi_mass");
        nL2DoubleMu3GenMuon       = new TNtuple("nL2DoubleMu3GenMuon","Gen Muon","muon_pt:muon_eta:muon_phi:muon_mass");
        nL2DoubleMu3GenMuFromJpsi = new TNtuple("nL2DoubleMu3GenMuFromJpsi","Gen Jpsi to muon","muon_pt:muon_eta:muon_phi:muon_mass:muon_p");
        nL2DoubleMu3GenJpsiToMM     = new TNtuple("nL2DoubleMu3GenJpsiToMM","Gen Jpsi MM","jpsi_pt:jpsi_eta:jpsi_phi:jpsi_mass");

        nL3DoubleMu3GenJpsi       = new TNtuple("nL3DoubleMu3GenJpsi","Gen Jpsi","jpsi_pt:jpsi_eta:jpsi_phi:jpsi_mass");
        nL3DoubleMu3GenMuon       = new TNtuple("nL3DoubleMu3GenMuon","Gen Muon","muon_pt:muon_eta:muon_phi:muon_mass");
        nL3DoubleMu3GenMuFromJpsi = new TNtuple("nL3DoubleMu3GenMuFromJpsi","Gen Jpsi to muon","muon_pt:muon_eta:muon_phi:muon_mass:muon_p:num");
        nL3DoubleMu3GenJpsiToMM     = new TNtuple("nL3DoubleMu3GenJpsiToMM","Gen Jpsi MM","jpsi_pt:jpsi_eta:jpsi_phi:jpsi_mass:num");
	
	nNL2Cand = new TNtuple("nNL2Cand","nNL2Cand","nL2cand");
	nNL3Cand = new TNtuple("nNL3Cand","nNL3Cand","nL3cand");
	
}

// ------------ method called once each job just after ending the event loop  ------------
void 
L1L2L3Analyzer::endJob() {
		std::cout << "===== Start writing user histograms =====" << std::endl;
		
    hOutputFile->SetCompressionLevel(2);
    hOutputFile->cd();
		
	hOutputFile->Write();
    hOutputFile->Close();
	std::cout << "===== End writing user histograms =======" << std::endl;	
}

//define this as a plug-in
DEFINE_FWK_MODULE(L1L2L3Analyzer);
