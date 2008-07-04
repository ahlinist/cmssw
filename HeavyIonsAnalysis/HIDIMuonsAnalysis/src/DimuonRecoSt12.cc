// -*- C++ -*-
//
// Package:    DimuonRecoSt12
// Class:      DimuonRecoSt12
// 
/**\class DimuonRecoSt12 DimuonRecoSt12.cc UserCode/DimuonRecoSt12/src/DimuonRecoSt12.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Dong Ho Moon
//         Created:  Sun Feb 17 15:20:13 CET 2008
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

//added include by moon

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
#include <TFile.h>
#include <TString.h>
#include <stdio.h>
#include <TH2F.h>

#include <TLorentzVector.h>
 
//
// class decleration
//

class DimuonRecoSt12 : public edm::EDAnalyzer {
   public:
      explicit DimuonRecoSt12(const edm::ParameterSet&);
      ~DimuonRecoSt12();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------
	  edm::InputTag l1muon_, l2muon_; 
	  int muTracks_;
	  std::string fOutputFileName_;

	  TFile *hOutputFile;
	  TH1F *h_st1l1muon_size;
	  TH1F *h_st1l1Muon_Size;
	  TH1F *h_st1l1muon_energy;
	  TH1F *h_st1l1muon_momentum;
	  TH1F *h_st1l1muon_pt;
	  TH1F *h_st1l1muon_eta;
	  TH1F *h_st1l1muon_phi;

	  TH1F *h_st2l2muon_size;
	  TH1F *h_st2l2Muon_Size;
	  TH1F *h_st2l2muon_energy;
	  TH1F *h_st2l2muon_momentum;
	  TH1F *h_st2l2muon_pt;
	  TH1F *h_st2l2muon_eta;
	  TH1F *h_st2l2muon_phi;
	  //TH1F *h_st2l2muon_energy;
	  //TH1F *h_st2l2muon_momentum;

	  TH1F *h_st1l1dimuon_charge;
	  TH1F *h_st1l1dimuon_mass;
	  TH1F *h_st1l1dimuon_likesign_mass;
	  TH1F *h_st1l1dimuon_pt;
	  TH1F *h_st1l1dimuon_eta;
	  TH1F *h_st1l1dimuon_DeltaR;

	  TH1F *h_st2l2dimuon_charge;
      TH1F *h_st2l2dimuon_mass;
      TH1F *h_st2l2dimuon_likesign_mass;
      TH1F *h_st2l2dimuon_pt;
      TH1F *h_st2l2dimuon_eta;
      TH1F *h_st2l2dimuon_DeltaR;
      
      TH2F *h_st1dimuon_eta_pt;
	  TH2F *h_st2dimuon_eta_pt;
	  
	  double ptMax_;
	  
	  
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
DimuonRecoSt12::DimuonRecoSt12(const edm::ParameterSet& iConfig):
l1muon_(iConfig.getUntrackedParameter<edm::InputTag>("L1muons")),
l2muon_(iConfig.getUntrackedParameter<edm::InputTag>("L2muons")),
muTracks_(iConfig.getUntrackedParameter("muTracks",1)),
fOutputFileName_(iConfig.getUntrackedParameter<std::string>("HistOutFile"))
{
		//now do what ever initialization is needed

}


DimuonRecoSt12::~DimuonRecoSt12()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void DimuonRecoSt12::beginJob(const edm::EventSetup&){

		hOutputFile   = new TFile( fOutputFileName_.c_str(), "RECREATE" ) ;

	    h_st1l1muon_size = new TH1F( "h_st1l1muon_size", "st1 l1muon size", 10, -0.5, 9.5);
	    h_st1l1Muon_Size = new TH1F( "h_st1l1Muon_Size", "st1 l1muon size > 2", 10, -0.5, 9.5);
	    h_st1l1muon_pt = new TH1F( "h_st1l1muon_pt" , "st1 l1muon p_{t}", 200, 0., 200.);
	    h_st1l1muon_eta = new TH1F( "h_st1l1muon_eta", "st1 l1muon #eta", 60, -3, 3);
	    h_st1l1muon_phi = new TH1F( "h_st1l1muon_phi", "st1 l1muon #phi", 80, -4, 4);
	    h_st1l1muon_energy = new TH1F( "h_st1l1muon_energy", "st1 l1muon energy", 50, 0, 50);
	    h_st1l1muon_momentum = new TH1F( "h_st1l1muon_mementum","st1 l1muon mementum", 50, 0, 50);

        h_st2l2muon_size = new TH1F( "h_st2l2muon_size", "st2 l2muon size", 10, -0.5, 9.5);
        h_st2l2Muon_Size = new TH1F( "h_st2l2Muon_Size", "st2 l2muon size > 2", 10, -0.5, 9.5);
        h_st2l2muon_pt = new TH1F( "h_st2l2muon_pt" , "st2 l2muon p_{t}", 100, 0., 40.);
	    h_st2l2muon_eta = new TH1F( "h_st2l2muon_eta", "st2 l2muon #eta", 60, -6, 6);
	    h_st2l2muon_phi = new TH1F( "h_st2l2muon_phi", "st2 l2muon #phi", 80, -4, 4);
		h_st2l2muon_energy = new TH1F( "h_st2l2muon_energy", "st2 l2muon energy", 50, 0, 50);
		h_st2l2muon_momentum = new TH1F( "h_st2l2muon_mementum","st2 l2muon mementum", 50, 0, 50);
        
		h_st1l1dimuon_mass = new TH1F( "h_st1l1dimuon_mass", "st1 l1dimuon mass" , 100, 0, 10);
	    h_st1l1dimuon_charge = new TH1F( "h_st1l1dimuon_charge","st1 l1dimuon charge", 4, -2,2);
	    h_st1l1dimuon_likesign_mass = new TH1F( "h_st1l1dimuon_likesign_mass", "st1 l1dimuon likesign mass" , 100, 0, 10);
	    h_st1l1dimuon_eta = new TH1F("h_st1l1dimuon_eta", "st1 l1dimuon eta", 120, -6.0, 6.0);
	    h_st1l1dimuon_pt = new TH1F("h_st1l1dimuon_pt", "st1 l1dimuon pt", 100, 0, 50);
	    h_st1l1dimuon_DeltaR = new TH1F("h_st1l1dimuon_DeltaR","st1 l1dimuon DeltaR", 100, 0, 2);

		h_st2l2dimuon_mass = new TH1F( "h_st2l2dimuon_mass", "st2 l2dimuon mass" , 100, 0, 10);
	    h_st2l2dimuon_charge = new TH1F( "h_st2l2dimuon_charge","st2 l2dimuon charge", 4, -2,2);
		h_st2l2dimuon_likesign_mass = new TH1F( "h_st2l2dimuon_likesign_mass", "st2 l2dimuon likesign mass" , 100, 0, 10);
		h_st2l2dimuon_eta = new TH1F("h_st2l2dimuon_eta", "st2 l2dimuon eta", 120, -6.0, 6.0);
		h_st2l2dimuon_pt = new TH1F("h_st2l2dimuon_pt", "st2 l2dimuon pt", 100, 0, 50);
		h_st2l2dimuon_DeltaR = new TH1F("h_st2l2dimuon_DeltaR","st2 l2dimuon DeltaR", 100, 0, 2);
		
		h_st1dimuon_eta_pt = new TH2F("st1_dimuon_eta_pt","St1 J/#Psi #eta - p_{T}", 100, -8, 8, 100, 0, 40);
		h_st2dimuon_eta_pt = new TH2F("st2_dimuon_eta_pt","St2 J/#Psi #eta - p_{T}", 100, -8, 8, 100, 0, 40);

}

									  
void
DimuonRecoSt12::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
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

   // l1muon taking
   Handle< vector<l1extra::L1MuonParticle> > l1muons;
   iEvent.getByLabel(l1muon_, l1muons);
   LogInfo("Muoninfo") << "number of l1muons : "<<l1muons->size();
   h_st1l1muon_size->Fill(l1muons->size());
   
   int nL1Muon_ = l1muons->size();
   int a = 10000;
   
   if ( nL1Muon_ == 3 || nL1Muon_ == 4 )
   {
       h_st1l1Muon_Size->Fill(nL1Muon_);
   }
   
   if ( nL1Muon_ > muTracks_ ){
   	std::cout << "# of muons : " << nL1Muon_ << std::endl;
   	for ( int i=0 ; i < nL1Muon_ - 1 ; ++i ){
      
//   if(l1muons->size() >1){
		   // when using trigger
		   const l1extra::L1MuonParticle & l1mu1 = (*l1muons)[i];
		for ( int j=i+1; j < nL1Muon_; ++j ){
		   const l1extra::L1MuonParticle & l1mu2 = (*l1muons)[j];
		   	if ( i == a || j == a ) continue;
		   	
		   double l1charge1 = l1mu1.charge(), l1pt1 = l1mu1.pt(), l1eta1 = l1mu1.eta(), l1phi1 = l1mu1.phi(), l1E1 = l1mu1.energy(), l1p1 = l1mu1.p();
		   double l1charge2 = l1mu2.charge(), l1pt2 = l1mu2.pt(), l1eta2 = l1mu2.eta(), l1phi2 = l1mu2.phi(), l1E2 = l1mu2.energy(), l1p2 = l1mu2.p();
		   
		   if ( ( i != j)&& ( l1charge1 * l1charge2 == -1 )){	

		   h_st1l1muon_pt->Fill(l1pt1);
		   h_st1l1muon_pt->Fill(l1pt2);
		   h_st1l1muon_eta->Fill(l1eta1);
		   h_st1l1muon_eta->Fill(l1eta2);
		   h_st1l1muon_phi->Fill(l1phi1);
		   h_st1l1muon_phi->Fill(l1phi2);
		   h_st1l1muon_energy->Fill(l1E1);
		   h_st1l1muon_energy->Fill(l1E2);
		   h_st1l1muon_momentum->Fill(l1p1);
		   h_st1l1muon_momentum->Fill(l1p2);
		   h_st1l1dimuon_charge->Fill(l1charge1*l1charge2);

		   TLorentzVector l1dimuon;
		   TLorentzVector l1muon1;
		   TLorentzVector l1muon2;

		   l1muon1.SetPxPyPzE(l1mu1.px(), l1mu1.py(), l1mu1.pz(), l1mu1.energy());
		   l1muon2.SetPxPyPzE(l1mu2.px(), l1mu2.py(), l1mu2.pz(), l1mu2.energy());

		   double recoL1MuDeltaR = l1muon1.DeltaR(l1muon2);
		   h_st1l1dimuon_DeltaR->Fill(recoL1MuDeltaR);

		   l1dimuon = l1muon1 + l1muon2;
		   double l1dimuonmass = l1dimuon.M();
		   
				   LogInfo("Jinfo")<<"dimuon mass = "<< l1dimuonmass;
				   h_st1l1dimuon_mass->Fill(l1dimuonmass);
				   h_st1l1dimuon_eta->Fill(l1dimuon.Eta());
				   h_st1l1dimuon_pt->Fill(l1dimuon.Pt());
		   
		   h_st1dimuon_eta_pt->Fill(l1dimuon.Eta(), l1dimuon.Pt());


		   if(l1charge1*l1charge2 == 1) h_st1l1dimuon_likesign_mass -> Fill(l1dimuonmass);
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
/*
   edm::Handle<RecoChargedCandidateCollection> l2muons;
   iEvent.getByLabel(l2muon_, l2muons);
   LogInfo("Trackinfo")<<"number of l2muons : "<< l2muons->size();
*/

   int nL2Muon_ = l2muons->size();
   int b = 10000;
   
   h_st2l2muon_size->Fill (nL2Muon_);
   if ( nL2Muon_ == 3 || nL2Muon_ == 4 )
   {
       h_st2l2Muon_Size->Fill(nL2Muon_);
   }
   
   if ( nL2Muon_ > muTracks_ ){
   	std::cout << "# of muons : " << nL2Muon_ << std::endl;
   	for ( int i=0 ; i < nL2Muon_ - 1 ; ++i ){
//   if(l2muons->size() >1){

		   const reco::Track & l2mu1 = (*l2muons)[i];
		   for ( int j=i+1; j < nL2Muon_; ++j ){
		   		   	
		   const reco::Track & l2mu2 = (*l2muons)[j];
		   
		   if ( i == b || j == b ) continue;	
		   
		   
		   //const RecoChargedCandidates & l2mu1 = (*l2muons)[0];
		   //const RecoChargedCandidates & l2mu2 = (*l2muons)[1];

		   double l2charge1 = l2mu1.charge(), l2pt1 = l2mu1.pt(), l2eta1 = l2mu1.eta(), l2phi1 = l2mu1.phi(), l2p1 = l2mu1.p();
           double l2charge2 = l2mu2.charge(), l2pt2 = l2mu2.pt(), l2eta2 = l2mu2.eta(), l2phi2 = l2mu2.phi(), l2p2 = l2mu2.p();
           
           if ( ( i != j)&& ( l2charge1 * l2charge2 == -1 )){	
         //  LogInfo("MuonInfo") << " Mu1 " << l2charge1 << " " << l2pt1 << " " << l2eta1 << " " << l2E1 << endl;
         //  LogInfo("MuonInfo") << " Mu2 " << l2charge2 << " " << l2pt2 << " " << l2eta2 << " " << l2E2 << endl;

		   h_st2l2muon_pt->Fill(l2pt1);
           h_st2l2muon_pt->Fill(l2pt2);
           h_st2l2muon_eta->Fill(l2eta1);
           h_st2l2muon_eta->Fill(l2eta2);
           h_st2l2muon_phi->Fill(l2phi1);
           h_st2l2muon_phi->Fill(l2phi2);
           h_st2l2muon_momentum->Fill(l2p1);
           h_st2l2muon_momentum->Fill(l2p2);
           h_st2l2dimuon_charge->Fill(l2charge1*l2charge2);

		   TLorentzVector l2dimuon;
		   TLorentzVector l2muon1;
		   TLorentzVector l2muon2;

		   l2muon1.SetPtEtaPhiM( l2pt1, l2eta1, l2phi1, 0.105658 );
		   l2muon2.SetPtEtaPhiM( l2pt2, l2eta2, l2phi2, 0.105658 );
		   
		   double recoL2MuDeltaR = l2muon1.DeltaR(l2muon2);
		   h_st2l2dimuon_DeltaR->Fill(recoL2MuDeltaR);
		   
		   l2dimuon = l2muon1 + l2muon2;
           double l2dimuonmass = l2dimuon.M();


				   LogInfo("Jinfo")<<"dimuon mass = "<< l2dimuonmass;
                   h_st2l2dimuon_mass->Fill(l2dimuonmass);
                   h_st2l2dimuon_eta->Fill(l2dimuon.Eta());
                   h_st2l2dimuon_pt->Fill(l2dimuon.Pt());
                   
           h_st2dimuon_eta_pt->Fill(l2dimuon.Eta(), l2dimuon.Pt());

		   if(l2charge1*l2charge2 == 1) h_st2l2dimuon_likesign_mass -> Fill(l2dimuonmass);		
		   b= j;
           j = nL1Muon_;   
		  }
		}
	}
 }
}


// ------------ method called once each job just before starting event loop  ------------

// ------------ method called once each job just after ending the event loop  ------------
void 
DimuonRecoSt12::endJob() {
		
		std::cout << "===== Start writing user histograms =====" << std::endl;
		
        hOutputFile->SetCompressionLevel(2);
        hOutputFile->cd();
		
		h_st1l1muon_pt->GetXaxis()->SetTitle("p_{T} (GeV/c)");
        h_st1l1muon_pt->GetYaxis()->SetTitle("Events/10 GeV/c");
	    h_st1l1muon_phi->GetXaxis()->SetTitle("#Delta#phi (rad)");
	    h_st1l1muon_phi->GetYaxis()->SetTitle("Events/0.1 rad");
	    h_st1l1muon_eta->GetXaxis()->SetTitle("pseudorapidity #eta (rad)");
	    h_st1l1muon_eta->GetYaxis()->SetTitle("Events/0.1 rad");

		h_st2l2muon_pt->GetXaxis()->SetTitle("p_{T} (GeV/c)");
		h_st2l2muon_pt->GetYaxis()->SetTitle("Events/10 GeV/c");
		h_st2l2muon_phi->GetXaxis()->SetTitle("#Delta#phi (rad)");
	    h_st2l2muon_phi->GetYaxis()->SetTitle("Events/0.1 rad");
	    h_st2l2muon_eta->GetXaxis()->SetTitle("pseudorapidity #eta (rad)");
		h_st2l2muon_eta->GetYaxis()->SetTitle("Events/0.1 rad");
												
	    h_st1l1dimuon_mass->GetXaxis()->SetTitle("Mass (GeV/c^{2})");
	    h_st1l1dimuon_mass->GetYaxis()->SetTitle("Events/10 GeV/c^{2}");
	    
	    h_st1l1dimuon_pt->GetXaxis()->SetTitle("p_{T} (GeV/c)");
	    h_st1l1dimuon_pt->SetLineWidth(2);
	    h_st1l1dimuon_pt->SetFillColor(42);
	    
	    h_st1l1dimuon_eta->GetXaxis()->SetTitle("pseudorapidity #eta (rad)");
	    h_st1l1dimuon_eta->SetLineWidth(2);
	    h_st1l1dimuon_eta->SetFillColor(42);
	    
	    h_st2l2dimuon_pt->GetXaxis()->SetTitle("p_{T} (GeV/c)");
	    h_st2l2dimuon_pt->SetLineWidth(2);
	    h_st2l2dimuon_pt->SetFillColor(42);
	    
	    h_st2l2dimuon_eta->GetXaxis()->SetTitle("pseudorapidity #eta (rad)");
	    h_st2l2dimuon_eta->SetLineWidth(2);
	    h_st2l2dimuon_eta->SetFillColor(42);
	    	    
	    h_st1l1dimuon_likesign_mass->GetXaxis()->SetTitle("Mass (GeV/c^{2})");
	    h_st1l1dimuon_likesign_mass->GetYaxis()->SetTitle("Events/10 GeV/c^{2}");
        
	    h_st2l2dimuon_mass->GetXaxis()->SetTitle("Mass (GeV/c^{2})");
	    h_st2l2dimuon_mass->GetYaxis()->SetTitle("Events/10 GeV/c^{2}");
	    h_st2l2dimuon_likesign_mass->GetXaxis()->SetTitle("Mass (GeV/c^{2})");
	    h_st2l2dimuon_likesign_mass->GetYaxis()->SetTitle("Events/10 GeV/c^{2}");
	    
	    h_st1dimuon_eta_pt->GetYaxis()->SetTitle("p_{T} (GeV/c)");
		h_st1dimuon_eta_pt->GetXaxis()->SetTitle("pseudorapidity #eta (rad)");
		h_st2dimuon_eta_pt->GetYaxis()->SetTitle("p_{T} (GeV/c)");
		h_st2dimuon_eta_pt->GetXaxis()->SetTitle("pseudorapidity #eta (rad)");
		
		hOutputFile->Write();
        hOutputFile->Close();
		std::cout << "===== End writing user histograms =======" << std::endl;
}

//define this as a plug-in
DEFINE_FWK_MODULE(DimuonRecoSt12);
