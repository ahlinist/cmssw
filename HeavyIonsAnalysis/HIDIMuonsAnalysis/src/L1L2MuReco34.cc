// -*- C++ -*-
//
// Package:    L1L2MuReco34
// Class:      L1L2MuReco34
// 
/**\class L1L2MuReco34 L1L2MuReco34.cc UserCode/L1L2MuReco34/src/L1L2MuReco34.cc

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

class L1L2MuReco34 : public edm::EDAnalyzer {
   public:
      explicit L1L2MuReco34(const edm::ParameterSet&);
      ~L1L2MuReco34();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------

     edm::InputTag genParticles_, l1muon_, l2muon_;

     TH1F *h_st3DiMuMass;
     TH1F *h_st3DiMuEta;
     TH1F *h_st3DiMuPt;
     TH2F *h_st3DiMuEtaPt;
     TH1F *h_st3p_pt;
     TH1F *h_st3p_eta;
     TH1F *h_st3PosMuDeltaR;
     TH1F *h_st3NegMuDeltaR;

     TH1F *h_st4DiMuMass;
     TH1F *h_st4DiMuEta;
     TH1F *h_st4DiMuPt;
     TH2F *h_st4DiMuEtaPt;
     TH1F *h_st4p_pt;
     TH1F *h_st4p_eta;
     TH1F *h_st4PosMuDeltaR;
     TH1F *h_st4NegMuDeltaR;

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
L1L2MuReco34::L1L2MuReco34(const edm::ParameterSet& iConfig):
genParticles_(iConfig.getUntrackedParameter<edm::InputTag>("src")),
l1muon_(iConfig.getUntrackedParameter<edm::InputTag>("L1muons")),
l2muon_(iConfig.getUntrackedParameter<edm::InputTag>("L2muons"))

{
   //now do what ever initialization is needed

}


L1L2MuReco34::~L1L2MuReco34()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called once each job just before starting event loop  ------------
void L1L2MuReco34::beginJob(const edm::EventSetup&)
{
       Service<TFileService> fs;

       h_st3DiMuMass    = fs->make<TH1F>( "St3DiMuMass", "ST3 J/#Psi mass" , 100, 0, 10) ;
       h_st3DiMuEta     = fs->make<TH1F>("St3DiMuEta", "ST3 J/#Psi #eta", 120, -6.0, 6.0);
       h_st3DiMuPt      = fs->make<TH1F>("St3DiMupT", "ST3 J/#Psi p_{T}", 80, 0, 40);
       h_st3p_pt        = fs->make<TH1F>("St3MCJpsi_pt", "ST3 matched MC J/Psi p_{T}", 100, 0, 40);
       h_st3p_eta       = fs->make<TH1F>("St3MCJpsi_eta", "ST3 matched MC J/Psi #eta", 100, -6.0, 6.0);
       h_st3PosMuDeltaR = fs->make<TH1F>("St3PosMuDeltaR", "ST3 Mu+ DeltaR", 100, 0, 5);
       h_st3NegMuDeltaR = fs->make<TH1F>("St3NegMuDeltaR", "ST3 Mu- DeltaR", 100, 0, 5);

       h_st4DiMuMass    = fs->make<TH1F>( "St4DiMuMass", "ST4 J/#Psi mass" , 100, 0, 10) ;
       h_st4DiMuEta     = fs->make<TH1F>("St4DiMuEta", "ST4 J/#Psi #eta", 120, -6.0, 6.0);
       h_st4DiMuPt      = fs->make<TH1F>("St4DiMupT", "ST4 J/#Psi p_{T}", 80, 0, 40);
       h_st4p_pt        = fs->make<TH1F>("St4MCJpsi_pt", "ST4 matched MC J/Psi p_{T}", 100, 0, 40);
       h_st4p_eta       = fs->make<TH1F>("St4MCJpsi_eta", "ST4 matched MC J/Psi #eta", 100, -6.0, 6.0);
       h_st4PosMuDeltaR = fs->make<TH1F>("St4PosMuDeltaR", "ST4 Mu+ DeltaR", 100, 0, 5);
       h_st4NegMuDeltaR = fs->make<TH1F>("St4NegMuDeltaR", "ST4 Mu- DeltaR", 100, 0, 5);

       h_st3DiMuEtaPt = fs->make<TH2F>("St3DiMuEtapT","ST3 J/#Psi #eta - p_{T}", 100, -8, 8, 100, 0, 40);
       h_st4DiMuEtaPt = fs->make<TH2F>("St4DiMuEtapT","ST4 J/#Psi #eta - p_{T}", 100, -8, 8, 100, 0, 40);

}

// ------------ method called to for each event  ------------
void L1L2MuReco34::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{


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
   bool l1ST_ = false;
   double l1Mu_i_eta = 0.0;
   double l1Mu_j_eta = 0.0;
   double l1Mu_i_pt = 0.0;
   double l1Mu_j_pt = 0.0;
   double l1Mu_i_phi = 0.0;
   double l1Mu_j_phi = 0.0;
   int l1Mu_i_charge = 0;
   int l1Mu_j_charge = 0;


   if ( 1 < nL1Muon_ /*&& nL1Muon_ < 4*/ )
   {
      for ( int i=0; i < nL1Muon_ - 1 ; ++i )
      {
         const l1extra::L1MuonParticle & l1Mu_i_ = (*l1muons)[i];
         
         for ( int j=i+1; j < nL1Muon_; ++j )
         {
            const l1extra::L1MuonParticle & l1Mu_j_ = (*l1muons)[j];

            // charge check up
            if ( i == a || j == a ) continue;
            if ( ( i != j) && ( l1Mu_i_.charge() * l1Mu_j_.charge() == -1 ) )
            {  
              l1ST_ = true;
              l1Mu_i_eta = l1Mu_i_.eta();
              l1Mu_j_eta = l1Mu_j_.eta();
              l1Mu_i_pt = l1Mu_i_.pt();
              l1Mu_j_pt = l1Mu_j_.pt();
              l1Mu_i_phi = l1Mu_i_.phi();
              l1Mu_j_phi = l1Mu_j_.phi();
              l1Mu_i_charge = l1Mu_i_.charge();
              l1Mu_j_charge = l1Mu_j_.charge();

              a = j;
              j = nL1Muon_;
            }

          }
      }
   }


   // l2muon taking 

   //   Handle<TrackCollection> l2muons;
   Handle<reco::TrackCollection> l2muons;
   iEvent.getByLabel(l2muon_, l2muons);

   unsigned int nL2Muon_ = l2muons->size();
   int b = 10000;
   bool l2ST_ = false;
   double l2Mu_i_eta = 0.0;
   double l2Mu_j_eta = 0.0;
   double l2Mu_i_pt = 0.0;
   double l2Mu_j_pt = 0.0;
   double l2Mu_i_phi = 0.0;
   double l2Mu_j_phi = 0.0;
   int l2Mu_i_charge = 0;
   int l2Mu_j_charge = 0;

   if ( 1< nL2Muon_ /*&& nL2Muon_ < 4*/ )
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
              l2ST_= true;
              l2Mu_i_eta = l2Mu_i_.eta();
              l2Mu_j_eta = l2Mu_j_.eta();
              l2Mu_i_pt = l2Mu_i_.pt();
              l2Mu_j_pt = l2Mu_j_.pt();
              l2Mu_i_phi = l2Mu_i_.phi();
              l2Mu_j_phi = l2Mu_j_.phi();
              l2Mu_i_charge = l2Mu_i_.charge();
              l2Mu_j_charge = l2Mu_j_.charge();

              b = j;
              j = nL2Muon_;
            }

          }
      }
   }

   // ST3
   if ( l1ST_ && l2ST_)
   {
       TLorentzVector st3PosMCMuLV_;
       TLorentzVector st3NegMCMuLV_;
 
       TLorentzVector l2MuLV_i_;
       TLorentzVector l2MuLV_j_;
       TLorentzVector l2DiMu_;

       l2MuLV_i_.SetPtEtaPhiM( l2Mu_i_pt, l2Mu_i_eta, l2Mu_i_phi, 0.105658 );
       l2MuLV_j_.SetPtEtaPhiM( l2Mu_j_pt, l2Mu_j_eta,l2Mu_j_phi, 0.105658 );

       l2DiMu_= l2MuLV_i_ + l2MuLV_j_;

       h_st3DiMuMass->Fill(l2DiMu_.M());
       h_st3DiMuEta->Fill(l2DiMu_.Eta());
       h_st3DiMuPt->Fill(l2DiMu_.Pt());
       h_st3DiMuEtaPt->Fill(l2DiMu_.Eta(), l2DiMu_.Pt());

       // For ST4 +++++++++++++++++++++++
       h_st4DiMuMass->Fill(l2DiMu_.M());
       h_st4DiMuEta->Fill(l2DiMu_.Eta());
       h_st4DiMuPt->Fill(l2DiMu_.Pt());
       h_st4DiMuEtaPt->Fill(l2DiMu_.Eta(), l2DiMu_.Pt());
       // +++++++++++++++++++++++++++++++

       // ST3 & ST4  muon MC truth
       for(size_t k = 0; k < genParticles->size(); ++k )
       {

           const Candidate & p = (*genParticles)[k];
           int id = p.pdgId();
           // int st = p.status(); 

           // ST3 JPsi's MCtruth
           if(abs(id) == 443 )
           {
               double pt = p.pt(), eta = p.eta();

               h_st3p_pt->Fill(pt); // ST3 matched J/Psi_pt
               h_st3p_eta->Fill(eta); // ST3 matched J/Psi_eta

               // For ST4 +++++++++
               h_st4p_pt->Fill(pt); // ST4 matched J/Psi_pt
               h_st4p_eta->Fill(eta); // ST4 matched J/Psi_eta
               // +++++++++++++++++
            }

           // DeltaR of ST3 muon between MC muon
           if (id == 13 )
           {
               double pt = p.pt(), eta = p.eta(), phi = p.phi(), mass = p.mass();
               st3PosMCMuLV_.SetPtEtaPhiM(pt, eta, phi, mass);

               if ( l2Mu_i_charge > 0 )
               {
                   double st3PosMuDeltaR_ = l2MuLV_i_.DeltaR(st3PosMCMuLV_);
                   h_st3PosMuDeltaR->Fill(st3PosMuDeltaR_);
                   h_st4PosMuDeltaR->Fill(st3PosMuDeltaR_); // For ST4

               }

               if ( l2Mu_j_charge > 0 )
               {
                   double st3PosMuDeltaR_ = l2MuLV_j_.DeltaR(st3PosMCMuLV_);
                   h_st3PosMuDeltaR->Fill(st3PosMuDeltaR_);
                   h_st4PosMuDeltaR->Fill(st3PosMuDeltaR_); // For ST4 
               }
            }

            if (id == -13 )
            {
                double pt = p.pt(), eta = p.eta(), phi = p.phi(), mass = p.mass();
                st3NegMCMuLV_.SetPtEtaPhiM(pt, eta, phi, mass);

                if ( l2Mu_i_charge < 0 )
                {
                    double st3NegMuDeltaR_ = l2MuLV_i_.DeltaR(st3NegMCMuLV_);
                    h_st3NegMuDeltaR->Fill(st3NegMuDeltaR_);
                    h_st4NegMuDeltaR->Fill(st3NegMuDeltaR_); // For ST4
                }

                if ( l2Mu_j_charge < 0 )
                {
                    double st3NegMuDeltaR_ = l2MuLV_j_.DeltaR(st3NegMCMuLV_);
                    h_st3NegMuDeltaR->Fill(st3NegMuDeltaR_);
                    h_st4NegMuDeltaR->Fill(st3NegMuDeltaR_); // For ST4
                 }
             }

         }
         // ++++++++++++++++++++++    

   }

   // ST4
   if ( l1ST_ || l2ST_)
   {
       TLorentzVector st4PosMCMuLV_;
       TLorentzVector st4NegMCMuLV_;

       TLorentzVector l1MuLV_i_;
       TLorentzVector l1MuLV_j_;
       TLorentzVector l2MuLV_i_;
       TLorentzVector l2MuLV_j_;
       TLorentzVector l1DiMu_;
       TLorentzVector l2DiMu_;

       if ( l1ST_== true && l2ST_ == false )
       {

          l1MuLV_i_.SetPtEtaPhiM( l1Mu_i_pt, l1Mu_i_eta, l1Mu_i_phi, 0.105658 );
          l1MuLV_j_.SetPtEtaPhiM( l1Mu_j_pt, l1Mu_j_eta, l1Mu_j_phi, 0.105658 );

          l1DiMu_= l1MuLV_i_ + l1MuLV_j_;

          h_st4DiMuMass->Fill(l1DiMu_.M());
          h_st4DiMuEta->Fill(l1DiMu_.Eta());
          h_st4DiMuPt->Fill(l1DiMu_.Pt());
          h_st4DiMuEtaPt->Fill(l1DiMu_.Eta(), l1DiMu_.Pt());

          // ST4  muon MC truth
          for(size_t k = 0; k < genParticles->size(); ++k )
          {

              const Candidate & p = (*genParticles)[k];
              int id = p.pdgId();
              // int st = p.status(); 

              // ST4 JPsi's MCtruth
              if(abs(id) == 443 )
              {
                  double pt = p.pt(), eta = p.eta();

                  h_st4p_pt->Fill(pt); // ST4 matched J/Psi_pt
                  h_st4p_eta->Fill(eta); // ST4 matched J/Psi_eta
               }
 
              // DeltaR of ST3 muon between MC muon
              if (id == 13 )
              {
                  double pt = p.pt(), eta = p.eta(), phi = p.phi(), mass = p.mass();
                  st4PosMCMuLV_.SetPtEtaPhiM(pt, eta, phi, mass);

                  if ( l1Mu_i_charge > 0 )
                  {
                      double st4PosMuDeltaR_ = l1MuLV_i_.DeltaR(st4PosMCMuLV_);
                      h_st4PosMuDeltaR->Fill(st4PosMuDeltaR_);

                  }

                  if ( l1Mu_j_charge > 0 )
                  {
                      double st4PosMuDeltaR_ = l1MuLV_j_.DeltaR(st4PosMCMuLV_);
                      h_st4PosMuDeltaR->Fill(st4PosMuDeltaR_);  
                  }
               }

               if (id == -13 )
               {
                   double pt = p.pt(), eta = p.eta(), phi = p.phi(), mass = p.mass();
                   st4NegMCMuLV_.SetPtEtaPhiM(pt, eta, phi, mass);

                   if ( l1Mu_i_charge < 0 )
                   {
                       double st4NegMuDeltaR_ = l1MuLV_i_.DeltaR(st4NegMCMuLV_);
                       h_st4NegMuDeltaR->Fill(st4NegMuDeltaR_); 
                   }

                   if ( l1Mu_j_charge < 0 )
                   {
                       double st4NegMuDeltaR_ = l1MuLV_j_.DeltaR(st4NegMCMuLV_);
                       h_st4NegMuDeltaR->Fill(st4NegMuDeltaR_); 
                   }
                }

            }
            // ++++++++++++++++++++++    


       }

       if ( l1ST_== false && l2ST_ == true )
       {

          l2MuLV_i_.SetPtEtaPhiM( l2Mu_i_pt, l2Mu_i_eta, l2Mu_i_phi, 0.105658 );
          l2MuLV_j_.SetPtEtaPhiM( l2Mu_j_pt, l2Mu_j_eta, l2Mu_j_phi, 0.105658 );

          l2DiMu_= l2MuLV_i_ + l2MuLV_j_;

          h_st4DiMuMass->Fill(l2DiMu_.M());
          h_st4DiMuEta->Fill(l2DiMu_.Eta());
          h_st4DiMuPt->Fill(l2DiMu_.Pt());
          h_st4DiMuEtaPt->Fill(l2DiMu_.Eta(), l2DiMu_.Pt());

          // ST4  muon MC truth
          for(size_t k = 0; k < genParticles->size(); ++k )
          {

              const Candidate & p = (*genParticles)[k];
              int id = p.pdgId();
              // int st = p.status(); 

              // ST4 JPsi's MCtruth
              if(abs(id) == 443 )
              {
                  double pt = p.pt(), eta = p.eta();

                  h_st4p_pt->Fill(pt); // ST4 matched J/Psi_pt
                  h_st4p_eta->Fill(eta); // ST4 matched J/Psi_eta
               }

              // DeltaR of ST3 muon between MC muon
              if (id == 13 )
              {
                  double pt = p.pt(), eta = p.eta(), phi = p.phi(), mass = p.mass();
                  st4PosMCMuLV_.SetPtEtaPhiM(pt, eta, phi, mass);

                  if ( l2Mu_i_charge > 0 )
                  {
                      double st4PosMuDeltaR_ = l2MuLV_i_.DeltaR(st4PosMCMuLV_);
                      h_st4PosMuDeltaR->Fill(st4PosMuDeltaR_);

                  }

                  if ( l2Mu_j_charge > 0 )
                  {
                      double st4PosMuDeltaR_ = l2MuLV_j_.DeltaR(st4PosMCMuLV_);
                      h_st4PosMuDeltaR->Fill(st4PosMuDeltaR_);           
                  }
               }

               if (id == -13 )
               {
                   double pt = p.pt(), eta = p.eta(), phi = p.phi(), mass = p.mass();
                   st4NegMCMuLV_.SetPtEtaPhiM(pt, eta, phi, mass);

                   if ( l2Mu_i_charge < 0 )
                   {
                       double st4NegMuDeltaR_ = l2MuLV_i_.DeltaR(st4NegMCMuLV_);
                       h_st4NegMuDeltaR->Fill(st4NegMuDeltaR_);
                   }

                   if ( l2Mu_j_charge < 0 )
                   {
                       double st4NegMuDeltaR_ = l2MuLV_j_.DeltaR(st4NegMCMuLV_);
                       h_st4NegMuDeltaR->Fill(st4NegMuDeltaR_);
                   }
                }

            }
            // ++++++++++++++++++++++    


       }

/*      if ( l1ST_ && l2ST_)
      {
          TLorentzVector l2MuLV_i_;
          TLorentzVector l2MuLV_j_;
          TLorentzVector l2DiMu_;

          l2MuLV_i_.SetPtEtaPhiM( l2Mu_i_pt, l2Mu_i_eta, l2Mu_i_phi, 0.105658 );
          l2MuLV_j_.SetPtEtaPhiM( l2Mu_j_pt, l2Mu_j_eta, l2Mu_j_phi, 0.105658 );

          l2DiMu_= l2MuLV_i_ + l2MuLV_j_;


          h_st4DiMuMass->Fill(l2DiMu_.M());
          h_st4DiMuEta->Fill(l2DiMu_.Eta());
          h_st4DiMuPt->Fill(l2DiMu_.Pt());
          h_st4DiMuEtaPt->Fill(l2DiMu_.Eta(), l2DiMu_.Pt());

      }
*/ //Did in ST3

   }

}



// ------------ method called once each job just after ending the event loop  ------------
void L1L2MuReco34::endJob() {

        std::cout << "===== Start writing user histograms =====" << std::endl;

        
        h_st3DiMuMass->GetXaxis()->SetTitle("Mass (GeV/c^{2})");
        h_st3DiMuEta->GetXaxis()->SetTitle("#eta");
        h_st3DiMuPt->GetXaxis()->SetTitle("p_{T} (GeV/c)");
        h_st3p_pt->GetXaxis()->SetTitle("p_{T} (GeV/c)");
        h_st3p_eta->GetXaxis()->SetTitle("#eta");

        h_st4DiMuMass->GetXaxis()->SetTitle("Mass (GeV/c^{2})");
        h_st4DiMuEta->GetXaxis()->SetTitle("#eta");
        h_st4DiMuPt->GetXaxis()->SetTitle("p_{T} (GeV/c)");
        h_st4p_pt->GetXaxis()->SetTitle("p_{T} (GeV/c)");
        h_st4p_eta->GetXaxis()->SetTitle("#eta");

        h_st3DiMuEtaPt->GetXaxis()->SetTitle("#eta");
        h_st3DiMuEtaPt->GetYaxis()->SetTitle("p_{T} (GeV/c)");

        h_st4DiMuEtaPt->GetXaxis()->SetTitle("#eta");
        h_st4DiMuEtaPt->GetYaxis()->SetTitle("p_{T} (GeV/c)");


        std::cout << "===== End writing user histograms =======" << std::endl;

}

//define this as a plug-in
DEFINE_FWK_MODULE(L1L2MuReco34);
