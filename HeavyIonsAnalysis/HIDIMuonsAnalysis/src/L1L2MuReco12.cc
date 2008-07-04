// -*- C++ -*-
//
// Package:    L1L2MuReco12
// Class:      L1L2MuReco12
// 
/**\class L1L2MuReco12 L1L2MuReco12.cc UserCode/L1L2MuReco12/src/L1L2MuReco12.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Ji Hyun Kim
//         Created:  Thu Mar 27 14:13:56 CET 2008
// $Id$
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

// ++++++++ jkim inserts
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
#include "DataFormats/Math/interface/Point3D.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "SimDataFormats/Track/interface/SimTrackContainer.h"

#include <TH1.h>
#include <TH2F.h>
#include <TFile.h>
#include <TString.h>
#include <stdio.h>

#include <TLorentzVector.h>

using namespace reco;
using namespace std;
using namespace edm;

//
// class decleration
//

class L1L2MuReco12 : public edm::EDAnalyzer {
   public:
      explicit L1L2MuReco12(const edm::ParameterSet&);
      ~L1L2MuReco12();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------

     edm::InputTag genParticles_, l1muon_, l2muon_;

     TH1F *h_l1MuSize;
     TH1F *h_l1DiMuMass;
     TH1F *h_l1DiMuEta;
     TH1F *h_l1DiMuPt;
     TH1F *h_l1DiMuDeltaR;
     TH1F *h_l1p_pt;
     TH1F *h_l1p_eta;
     TH1F *h_l1MCDiMu_pt;
     TH1F *h_l1MCDiMu_eta;
     TH1F *h_l1PosMuDeltaR;
     TH1F *h_l1NegMuDeltaR;

     TH1F *h_l2MuSize;
     TH1F *h_l2DiMuMass;
     TH1F *h_l2DiMuEta;
     TH1F *h_l2DiMuPt;
     TH1F *h_l2DiMuDeltaR;
     TH1F *h_l2p_pt;
     TH1F *h_l2p_eta;
     TH1F *h_l2MCDiMu_pt;
     TH1F *h_l2MCDiMu_eta;
     TH1F *h_l2PosMuDeltaR;
     TH1F *h_l2NegMuDeltaR;

     TH2F *h_l1DiMuEtaPt;
     TH2F *h_l2DiMuEtaPt;
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
L1L2MuReco12::L1L2MuReco12(const edm::ParameterSet& iConfig):
genParticles_(iConfig.getUntrackedParameter<edm::InputTag>("src")),
l1muon_(iConfig.getUntrackedParameter<edm::InputTag>("L1muons")),
l2muon_(iConfig.getUntrackedParameter<edm::InputTag>("L2muons"))

{
   //now do what ever initialization is needed

}


L1L2MuReco12::~L1L2MuReco12()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called once each job just before starting event loop  ------------
void L1L2MuReco12::beginJob(const edm::EventSetup&)
{
       Service<TFileService> fs;

       h_l1MuSize      = fs->make<TH1F>("L1MuSize", "L1 muon size", 10, -0.5, 9.5);
       h_l1DiMuMass    = fs->make<TH1F>("L1DiMuMass", "L1 J/#Psi mass" , 100, 0, 10) ;
       h_l1DiMuEta     = fs->make<TH1F>("L1DiMuEta", "L1 J/#Psi #eta", 120, -6.0, 6.0);
       h_l1DiMuPt      = fs->make<TH1F>("L1DiMupT", "L1 J/#Psi p_{T}", 80, 0, 40);
       h_l1DiMuDeltaR  = fs->make<TH1F>("L1DiMuDeltaR","L1 J/#Psi DeltaR", 100, 0, 10);
       h_l1p_pt        = fs->make<TH1F>("L1MCJpsi_pt", "L1 matched MC J/Psi p_{T}", 100, 0, 40);
       h_l1p_eta       = fs->make<TH1F>("L1MCJpsi_eta", "L1 matched MC J/Psi #eta", 100, -6.0, 6.0);
       h_l1MCDiMu_pt   = fs->make<TH1F>("L1MCDiMu_pt", "L1 mached MC Dimuon p_{T}", 100, 0, 40);
       h_l1MCDiMu_eta  = fs->make<TH1F>("L1MCDiMu_eta", "L1 mached MC Dimuon #eta", 100, -6.0, 6.0);
       h_l1PosMuDeltaR = fs->make<TH1F>("L1PosMuDeltaR", "L1 Mu+ DeltaR", 100, 0, 5);
       h_l1NegMuDeltaR = fs->make<TH1F>("L1NegMuDeltaR", "L1 Mu- DeltaR", 100, 0, 5);

       h_l2MuSize      = fs->make<TH1F>( "L2MuSize", "L2 muon size", 10, -0.5, 9.5);
       h_l2DiMuMass    = fs->make<TH1F>( "L2DiMuMass", "L2 J/#Psi mass" , 100, 0, 10) ;
       h_l2DiMuEta     = fs->make<TH1F>("L2DiMuEta", "L2 J/#Psi #eta", 120, -6.0, 6.0);
       h_l2DiMuPt      = fs->make<TH1F>("L2DiMupT", "L2 J/#Psi p_{T}", 80, 0, 40);
       h_l2DiMuDeltaR  = fs->make<TH1F>("L2DiMuDeltaR","L2 J/#Psi DeltaR", 100, 0, 10);
       h_l2p_pt        = fs->make<TH1F>("L2MCJpsi_pt", "L2 matched MC J/Psi p_{T}", 100, 0, 40);
       h_l2p_eta       = fs->make<TH1F>("L2MCJpsi_eta", "L2 matched MC J/Psi #eta", 100, -6.0, 6.0);
       h_l2MCDiMu_pt   = fs->make<TH1F>("L2MCDiMu_pt", "L2 mached MC Dimuon p_{T}", 100, 0, 40);
       h_l2MCDiMu_eta  = fs->make<TH1F>("L2MCDiMu_eta", "L2 mached MC Dimuon #eta", 100, -6.0, 6.0);
       h_l2PosMuDeltaR = fs->make<TH1F>("L2PosMuDeltaR", "L2 Mu+ DeltaR", 100, 0, 5);
       h_l2NegMuDeltaR = fs->make<TH1F>("L2NegMuDeltaR", "L2 Mu- DeltaR", 100, 0, 5);

       h_l1DiMuEtaPt = fs->make<TH2F>("L1DiMuEtapT","L1 J/#Psi #eta - p_{T}", 100, -8, 8, 100, 0, 40);
       h_l2DiMuEtaPt = fs->make<TH2F>("L2DiMuEtapT","L2 J/#Psi #eta - p_{T}", 100, -8, 8, 100, 0, 40);

}

// ------------ method called to for each event  ------------
void L1L2MuReco12::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
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

   // genParticles taking
   Handle<CandidateCollection> genParticles;
   iEvent.getByLabel(genParticles_, genParticles);

   // l1muon taking 

   Handle< vector<l1extra::L1MuonParticle> > l1muons;
   iEvent.getByLabel(l1muon_, l1muons);

   unsigned int nL1Muon_ = l1muons->size();
   int a = 10000;

   TLorentzVector l1PosMCMuLV_;
   TLorentzVector l1NegMCMuLV_;
   TLorentzVector l1MuLV_i_;
   TLorentzVector l1MuLV_j_;
   TLorentzVector l1DiMu_;

   if ( nL1Muon_ > 2 )
   {
       h_l1MuSize->Fill(nL1Muon_);
   }

   if ( 1 < nL1Muon_ /*&& nL1Muon_ < 4*/  )
   {  

      //std::cout << "# of muons : " << nL1Muon_ << std::endl;
      for ( int i=0 ; i < nL1Muon_ - 1 ; ++i )
      {
         const l1extra::L1MuonParticle & l1Mu_i_ = (*l1muons)[i];
         
         for ( int j=i+1; j < nL1Muon_; ++j )
         {
            const l1extra::L1MuonParticle & l1Mu_j_ = (*l1muons)[j];

           // charge check up
           if ( i == a || j == a ) continue;
           if ( ( i != j) && ( l1Mu_i_.charge() * l1Mu_j_.charge() == -1 ) )
           {
              l1MuLV_i_.SetPtEtaPhiM( l1Mu_i_.pt(),
                               l1Mu_i_.eta(),
                               l1Mu_i_.phi(),
                               0.105658 );
              l1MuLV_j_.SetPtEtaPhiM( l1Mu_j_.pt(),
                               l1Mu_j_.eta(),
                               l1Mu_j_.phi(),
                               0.105658 );

              double recoL1MuDeltaR = l1MuLV_i_.DeltaR(l1MuLV_j_);
              h_l1DiMuDeltaR->Fill(recoL1MuDeltaR);

              l1DiMu_= l1MuLV_i_ + l1MuLV_j_;

              h_l1DiMuMass->Fill(l1DiMu_.M());
              h_l1DiMuEta->Fill(l1DiMu_.Eta());
              h_l1DiMuPt->Fill(l1DiMu_.Pt());
              h_l1DiMuEtaPt->Fill(l1DiMu_.Eta(), l1DiMu_.Pt());
              
              a = j;
              j = nL1Muon_;

              // L1 muon MC truth
              for(size_t k = 0; k < genParticles->size(); ++k ) 
              {

                 const Candidate & p = (*genParticles)[k];
                 int id = p.pdgId();
                 // int st = p.status(); 
  
                 // L1 JPsi's MCtruth
                 if(abs(id) == 443 ) 
                 {
                    double pt = p.pt(), eta = p.eta();

                    h_l1p_pt->Fill(pt); // L1 matched J/Psi_pt
                    h_l1p_eta->Fill(eta); // L1 matched J/Psi_eta
                 }

                 // DeltaR of L1 muon between MC muon
                 if (id == 13 )
                 {
                    double pt = p.pt(), eta = p.eta(), phi = p.phi(), mass = p.mass();
                    
                    h_l1MCDiMu_pt->Fill(pt);
                    h_l1MCDiMu_eta->Fill(eta);

                    l1PosMCMuLV_.SetPtEtaPhiM(pt, eta, phi, mass);

                    if ( l1Mu_i_.charge() > 0 )
                    {  
                       double l1PosMuDeltaR_ = l1MuLV_i_.DeltaR(l1PosMCMuLV_);
                       h_l1PosMuDeltaR->Fill(l1PosMuDeltaR_);
                    }

                    if ( l1Mu_j_.charge() > 0 )
                    {
                       double l1PosMuDeltaR_ = l1MuLV_j_.DeltaR(l1PosMCMuLV_);
                       h_l1PosMuDeltaR->Fill(l1PosMuDeltaR_);
                    }

                 }  

                 if (id == -13 )
                 {
                    double pt = p.pt(), eta = p.eta(), phi = p.phi(), mass = p.mass();
                    h_l1MCDiMu_pt->Fill(pt);
                    h_l1MCDiMu_eta->Fill(eta);

                    l1NegMCMuLV_.SetPtEtaPhiM(pt, eta, phi, mass);

                    if ( l1Mu_i_.charge() < 0 )
                    {
                       double l1NegMuDeltaR_ = l1MuLV_i_.DeltaR(l1NegMCMuLV_);
                       h_l1NegMuDeltaR->Fill(l1NegMuDeltaR_);
                    }

                    if ( l1Mu_j_.charge() < 0 )
                    {
                       double l1NegMuDeltaR_ = l1MuLV_j_.DeltaR(l1NegMCMuLV_);
                       h_l1NegMuDeltaR->Fill(l1NegMuDeltaR_);
                    }

                 } 

              }
             // ++++++++++++++++++++++    


            }
          }
      }
   }


   // l2muon taking 

   Handle<reco::TrackCollection> l2muons;
   iEvent.getByLabel(l2muon_, l2muons);

   unsigned int nL2Muon_ = l2muons->size();
   int b = 10000;

   TLorentzVector l2PosMCMuLV_;
   TLorentzVector l2NegMCMuLV_;
   TLorentzVector l2MuLV_i_;
   TLorentzVector l2MuLV_j_;
   TLorentzVector l2DiMu_;

   if ( nL2Muon_ > 2 )
   {
       h_l2MuSize->Fill(nL2Muon_);
   }

   if ( 1 < nL1Muon_ /*&& nL1Muon_ < 4*/ )
   {
      for ( int i=0; i < nL2Muon_ - 1 ; ++i )
      {
         const reco::Track & l2Mu_i_ = (*l2muons)[i];

         for ( int j=i+1; j < nL2Muon_; ++j )
         {
            const reco::Track & l2Mu_j_ = (*l2muons)[j];

           // charge check up
           if ( i == b || j == b ) continue;
           if ( ( i != j ) && ( l2Mu_i_.charge() * l2Mu_j_.charge() == -1 ) )
           {
              l2MuLV_i_.SetPtEtaPhiM( l2Mu_i_.pt(),
                               l2Mu_i_.eta(),
                               l2Mu_i_.phi(),
                               0.105658 );
              l2MuLV_j_.SetPtEtaPhiM( l2Mu_j_.pt(),
                               l2Mu_j_.eta(),
                               l2Mu_j_.phi(),
                               0.105658 );

              double recoL2MuDeltaR = l2MuLV_i_.DeltaR(l2MuLV_j_);
              h_l2DiMuDeltaR->Fill(recoL2MuDeltaR);

              l2DiMu_= l2MuLV_i_ + l2MuLV_j_;

              h_l2DiMuMass->Fill(l2DiMu_.M());
              h_l2DiMuEta->Fill(l2DiMu_.Eta());
              h_l2DiMuPt->Fill(l2DiMu_.Pt());
              h_l2DiMuEtaPt->Fill(l2DiMu_.Eta(), l2DiMu_.Pt());

              b = j;
              j = nL2Muon_;

              // L2 muon MC truth
              for(size_t k = 0; k < genParticles->size(); ++k )
              {

                 const Candidate & p = (*genParticles)[k];
                 int id = p.pdgId();
                 // int st = p.status(); 

                 // L2 JPsi's MC truth
                 if(abs(id) == 443 )
                 {
                    double pt = p.pt(), eta = p.eta();

                    h_l2p_pt->Fill(pt); // L2 matched J/Psi_pt
                    h_l2p_eta->Fill(eta); // L2 matched J/Psi_eta
                 }

                 // DeltaR of L2 muon between MC muon
                 if (id == 13 )
                 {
                    double pt = p.pt(), eta = p.eta(), phi = p.phi(), mass = p.mass();

                    h_l2MCDiMu_pt->Fill(pt);
                    h_l2MCDiMu_eta->Fill(eta);

                    l2PosMCMuLV_.SetPtEtaPhiM(pt, eta, phi, mass);

                    if ( l2Mu_i_.charge() > 0 )
                    {
                       double l2PosMuDeltaR_ = l2MuLV_i_.DeltaR(l2PosMCMuLV_);
                       h_l2PosMuDeltaR->Fill(l2PosMuDeltaR_);
                    }

                    if ( l2Mu_j_.charge() > 0 )
                    {
                       double l2PosMuDeltaR_ = l2MuLV_j_.DeltaR(l2PosMCMuLV_);
                       h_l2PosMuDeltaR->Fill(l2PosMuDeltaR_);
                    }

                 }

                 if (id == -13 )
                 {
                    double pt = p.pt(), eta = p.eta(), phi = p.phi(), mass = p.mass();

                    h_l2MCDiMu_pt->Fill(pt);
                    h_l2MCDiMu_eta->Fill(eta);

                    l2NegMCMuLV_.SetPtEtaPhiM(pt, eta, phi, mass);

                    if ( l2Mu_i_.charge() < 0 )
                    {
                       double l2NegMuDeltaR_ = l2MuLV_i_.DeltaR(l2NegMCMuLV_);
                       h_l2NegMuDeltaR->Fill(l2NegMuDeltaR_);
                    }

                    if ( l2Mu_j_.charge() < 0 )
                    {
                       double l2NegMuDeltaR_ = l2MuLV_j_.DeltaR(l2NegMCMuLV_);
                       h_l2NegMuDeltaR->Fill(l2NegMuDeltaR_);
                    }

                 }

              }
             // ++++++++++++++++++++++    

           }
         }
      }
   }

}



// ------------ method called once each job just after ending the event loop  ------------
void L1L2MuReco12::endJob() {

        std::cout << "===== Start writing user histograms =====" << std::endl;

        
        h_l1DiMuMass->GetXaxis()->SetTitle("Mass (GeV/c^{2})");
        h_l1DiMuEta->GetXaxis()->SetTitle("#eta");
        h_l1DiMuPt->GetXaxis()->SetTitle("p_{T} (GeV/c)");
        h_l1p_pt->GetXaxis()->SetTitle("p_{T} (GeV/c)");
        h_l1p_eta->GetXaxis()->SetTitle("#eta");
        h_l1MCDiMu_eta->GetXaxis()->SetTitle("#eta");  
        h_l1MCDiMu_pt->GetXaxis()->SetTitle("p_{T} (GeV/c)");   
 
        h_l2DiMuMass->GetXaxis()->SetTitle("Mass (GeV/c^{2})");
        h_l2DiMuEta->GetXaxis()->SetTitle("#eta");
        h_l2DiMuPt->GetXaxis()->SetTitle("p_{T} (GeV/c)");
        h_l2p_pt->GetXaxis()->SetTitle("p_{T} (GeV/c)");
        h_l2p_eta->GetXaxis()->SetTitle("#eta");
        h_l2MCDiMu_eta->GetXaxis()->SetTitle("#eta");
        h_l2MCDiMu_pt->GetXaxis()->SetTitle("p_{T} (GeV/c)");

        h_l1DiMuEtaPt->GetXaxis()->SetTitle("#eta");
        h_l1DiMuEtaPt->GetYaxis()->SetTitle("p_{T} (GeV/c)");

        h_l2DiMuEtaPt->GetXaxis()->SetTitle("#eta");
        h_l2DiMuEtaPt->GetYaxis()->SetTitle("p_{T} (GeV/c)");


        std::cout << "===== End writing user histograms =======" << std::endl;

}

//define this as a plug-in
DEFINE_FWK_MODULE(L1L2MuReco12);
