// -*- C++ -*-
//
// Package:    DimuonRecoSt34
// Class:      DimuonRecoSt34
// 
/**\class DimuonRecoSt34 DimuonRecoSt34.cc UserCode/DimuonRecoSt34/src/DimuonRecoSt34.cc

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
#include <TH2F.h>
#include <TFile.h>
#include <TString.h>
#include <stdio.h>

#include <TLorentzVector.h>
 
//
// class decleration
//

class DimuonRecoSt34 : public edm::EDAnalyzer {
   public:
      explicit DimuonRecoSt34(const edm::ParameterSet&);
      ~DimuonRecoSt34();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------
	  edm::InputTag l1muon_, l2muon_; 
	  int muTracks_;
	  std::string fOutputFileName_;

	  TFile *hOutputFile;
	  TH2F *h_st3dimuon_eta_pt;
	  TH2F *h_st4dimuon_eta_pt;

	  TH1F *h_st3l1Muon_Size;
	  TH1F *h_st3muon_eta;
      TH1F *h_st3muon_pt;
      TH1F *h_st3muon_p;
      TH1F *h_st3dimuon_eta;
      TH1F *h_st3dimuon_pt;
      TH1F *h_st3dimuon_p;
      TH1F *h_st3dimuon_mass;
      
	  TH1F *h_st4l2Muon_Size;
	  TH1F *h_st4muon_eta;
      TH1F *h_st4muon_pt;
      TH1F *h_st4muon_p;
      TH1F *h_st4dimuon_eta;
      TH1F *h_st4dimuon_pt;
      TH1F *h_st4dimuon_p;
      TH1F *h_st4dimuon_mass;
	  
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
DimuonRecoSt34::DimuonRecoSt34(const edm::ParameterSet& iConfig):
l1muon_(iConfig.getUntrackedParameter<edm::InputTag>("L1muons")),
l2muon_(iConfig.getUntrackedParameter<edm::InputTag>("L2muons")),
muTracks_(iConfig.getUntrackedParameter("muTracks",1)),
fOutputFileName_(iConfig.getUntrackedParameter<std::string>("HistOutFile"))
{
		//now do what ever initialization is needed

}


DimuonRecoSt34::~DimuonRecoSt34()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void DimuonRecoSt34::beginJob(const edm::EventSetup&){

		hOutputFile   = new TFile( fOutputFileName_.c_str(), "RECREATE" ) ;
		

		h_st3l1Muon_Size = new TH1F( "h_st3l1Muon_Size", "st3 l1muon size > 2", 10, -0.5, 9.5);
		h_st3muon_eta      = new TH1F( "st3_muon_eta", "St3 muon #eta", 60, -6, 6);
		h_st3muon_pt       = new TH1F( "st3_muon_pt" , "St3 muon p_{T}", 80, 0., 40.);
		h_st3muon_p        = new TH1F( "st3_muon_p" , "St3 muon momentum", 80, 0., 40.);

		h_st3dimuon_eta    = new TH1F( "st3_dimuon_eta", "St3 J/#Psi #eta", 120, -6, 6); 
		h_st3dimuon_pt     = new TH1F( "st3_dimuon_pt" , "St3 J/#Psi p_{T}", 100, 0., 50.); 
		h_st3dimuon_p      = new TH1F( "st3_dimuon_p" , "St3 J/#Psi momentum", 80, 0., 40.);
        h_st3dimuon_mass   = new TH1F( "st3_dimuon_mass", "St3 J/#Psi mass" , 100, 0, 10);

		h_st4l2Muon_Size = new TH1F( "h_st4l2Muon_Size", "st4 muon size > 2", 10, -0.5, 9.5);
		h_st4muon_eta      = new TH1F( "st4_muon_eta", "St4 muon #eta", 60, -6, 6);
		h_st4muon_pt       = new TH1F( "st4_muon_pt" , "St4 muon p_{T}", 80, 0., 40.);
		h_st4muon_p        = new TH1F( "st4_muon_p" , "St4 muon momentum", 80, 0., 40.);

		h_st4dimuon_eta    = new TH1F( "st4_dimuon_eta", "St4 J/#Psi #eta", 120, -6, 6);
		h_st4dimuon_pt     = new TH1F( "st4_dimuon_pt" , "St4 J/#Psi p_{T}", 100, 0., 50.);
        h_st4dimuon_p      = new TH1F( "st4_dimuon_p" , "St4 J/#Psi momentum", 80, 0., 40.);
		h_st4dimuon_mass   = new TH1F( "st4_dimuon_mass", "St4 J/#Psi mass" , 100, 0, 10);

		h_st3dimuon_eta_pt = new TH2F("st3_dimuon_eta_pt","St3 J/#Psi #eta - p_{T}", 100, -8, 8, 100, 0, 40);
		h_st4dimuon_eta_pt = new TH2F("st4_dimuon_eta_pt","St4 J/#Psi #eta - p_{T}", 100, -8, 8, 100, 0, 40);
		
}

									  
void
DimuonRecoSt34::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
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


   
   bool l1STP_ = false;
   bool l2STP_ = false;
   int l1stp = 0;
   int l2stp = 0;
   
   // l1muon taking
   Handle< vector<l1extra::L1MuonParticle> > l1muons;
   iEvent.getByLabel(l1muon_, l1muons);
   LogInfo("Muoninfo") << "number of l1muons : "<<l1muons->size();
//   h_l1muon_size->Fill(l1muons->size());

   double l1_1_charge = 0.0;
   double l1_2_charge = 0.0;
   double l1_1_pt     = 0.0;
   double l1_2_pt     = 0.0;
   double l1_1_eta    = 0.0;
   double l1_2_eta    = 0.0;
   double l1_1_phi    = 0.0;
   double l1_2_phi    = 0.0;
   double l1_1_e      = 0.0;
   double l1_2_e      = 0.0;
   double l1_1_p      = 0.0;
   double l1_2_p      = 0.0;
   double l1_1_px     = 0.0;
   double l1_2_px     = 0.0;
   double l1_1_py     = 0.0;
   double l1_2_py     = 0.0;
   double l1_1_pz     = 0.0;
   double l1_2_pz     = 0.0;

//   if(l1muons->size() >1){
   int nL1Muon_ = l1muons->size();
   int a = 10000;
   
   if ( nL1Muon_ == 3 || nL1Muon_ == 4 )
   {
       h_st3l1Muon_Size->Fill(nL1Muon_);
   }
   
   if ( nL1Muon_ > muTracks_ ){
   	std::cout << "# of muons : " << nL1Muon_ << std::endl;
   	for ( int i=0 ; i < nL1Muon_ - 1 ; ++i ){
     
      // when using trigger
      const l1extra::L1MuonParticle & l1mu1 = (*l1muons)[i];
      
      for ( int j=i+1; j < nL1Muon_; ++j ){
      const l1extra::L1MuonParticle & l1mu2 = (*l1muons)[j];
      if ( i == a || j == a ) continue;

      l1_1_charge = l1mu1.charge();
      l1_2_charge = l1mu2.charge();
      
      if ( ( i != j)&& ( l1_1_charge * l1_2_charge == -1 )){	
      	
      l1_1_pt     = l1mu1.pt();
      l1_2_pt     = l1mu2.pt();
      l1_1_eta    = l1mu1.eta();
      l1_2_eta    = l1mu2.eta();
      l1_1_phi    = l1mu1.phi();
      l1_2_phi    = l1mu2.phi();
      l1_1_e      = l1mu1.energy();
      l1_2_e      = l1mu2.energy();
      l1_1_p      = l1mu1.p();
      l1_2_p      = l1mu2.p();
	  l1_1_px     = l1mu1.px();
	  l1_2_px     = l1mu2.px();
	  l1_1_py     = l1mu1.py();
	  l1_2_py     = l1mu2.py();
	  l1_1_pz     = l1mu1.pz();
	  l1_2_pz     = l1mu1.pz();
     
			  l1STP_ = true;
			  l1stp = 1;
	  a = j;
      j = nL1Muon_;
	  }
   }
  }
}
   


   // l2muon taking 
   
   double l2_1_charge = 0.0;
   double l2_2_charge = 0.0;
   double l2_1_pt     = 0.0;
   double l2_2_pt     = 0.0;
   double l2_1_eta    = 0.0;
   double l2_2_eta    = 0.0;
   double l2_1_phi    = 0.0;
   double l2_2_phi    = 0.0;
   double l2_1_p      = 0.0;
   double l2_2_p      = 0.0;
   
   Handle<reco::TrackCollection> l2muons;
   iEvent.getByLabel(l2muon_, l2muons);
//   h_l2muon_size->Fill (l2muons->size());
   //if( l2muons->size() > 1 ){
   int nL2Muon_ = l2muons->size();
   int b = 10000;
   
   if ( nL2Muon_ == 3 || nL2Muon_ == 4 )
   {
       h_st4l2Muon_Size->Fill(nL2Muon_);
   }
   if ( nL2Muon_ > muTracks_ ){
   	std::cout << "# of muons : " << nL2Muon_ << std::endl;
   	for ( int i=0 ; i < nL2Muon_ - 1 ; ++i ){
   
      const reco::Track & l2mu1 = (*l2muons)[i];
      	
      for ( int j=i+1; j < nL2Muon_; ++j ){
      const reco::Track & l2mu2 = (*l2muons)[j];
      if ( i == b || j == b ) continue;	
      	
      l2_1_charge = l2mu1.charge();
      l2_2_charge = l2mu2.charge();
      
      if ( ( i != j)&& ( l2_1_charge * l2_2_charge == -1 )){	
      
      l2_1_pt     = l2mu1.pt();
      l2_2_pt     = l2mu2.pt();
      l2_1_eta    = l2mu1.eta();
      l2_2_eta    = l2mu2.eta();
      l2_1_phi    = l2mu1.phi();
      l2_2_phi    = l2mu2.phi();
      l2_1_p      = l2mu1.p();
      l2_2_p      = l2mu2.p();
   
			  l2STP_ = true;
			  l2stp = 1;
	  }
   }
  }
}

   // ST3 section
   if ( l1STP_ && l2STP_ ) {
      TLorentzVector l1_1_muon, l1_2_muon, l2_1_muon, l2_2_muon, l1_dimuon, l2_dimuon;
	  //l1_1_muon.SetPxPyPzE( l1_1_px, l1_1_py, l1_1_pz, l1_1_e);
	  //l1_2_muon.SetPxPyPzE( l1_2_px, l1_2_py, l1_2_pz, l1_2_e );
	  l1_1_muon.SetPtEtaPhiM( l1_1_pt, l1_1_eta, l1_1_phi, 0.105658 );
	  l1_2_muon.SetPtEtaPhiM( l1_2_pt, l1_2_eta, l1_2_phi, 0.105658 );
	  l2_1_muon.SetPtEtaPhiM( l2_1_pt, l2_1_eta, l2_1_phi, 0.105658 );
	  l2_2_muon.SetPtEtaPhiM( l2_2_pt, l2_2_eta, l2_2_phi, 0.105658 );
	  
	  l1_dimuon = l1_1_muon + l1_2_muon;
	  l2_dimuon = l2_1_muon + l2_2_muon;
	  
	  h_st3muon_eta->Fill(l2_1_eta);
	  h_st3muon_eta->Fill(l2_2_eta);
	  h_st3muon_pt->Fill(l2_1_pt);
	  h_st3muon_pt->Fill(l2_2_pt);
	  h_st3muon_p->Fill(l2_1_p);
	  h_st3muon_p->Fill(l2_2_p);

	  h_st3dimuon_mass->Fill(l2_dimuon.M());
//	  h_st3dimuon_mass->Fill(l1_dimuon.M());
	  h_st3dimuon_eta->Fill(l2_dimuon.Eta());
//	  h_st3dimuon_eta->Fill(l1_dimuon.Eta());
	  h_st3dimuon_pt->Fill(l2_dimuon.Pt());
//	  h_st3dimuon_pt->Fill(l1_dimuon.Pt());
	  h_st3dimuon_p->Fill(l2_dimuon.P());
//	  h_st3dimuon_p->Fill(l1_dimuon.P());

	  h_st3dimuon_eta_pt->Fill(l2_dimuon.Eta(), l2_dimuon.Pt());
	  
   }
   // ST4 section
   if ( l1STP_ || l2STP_ ) {
	  TLorentzVector l1_1_muon, l1_2_muon, l2_1_muon, l2_2_muon, l1_dimuon, l2_dimuon;
	  l1_1_muon.SetPxPyPzE( l1_1_px, l1_1_py, l1_1_pz, l1_1_e );
	  l1_2_muon.SetPxPyPzE( l1_2_px, l1_2_py, l1_2_pz, l1_2_e );
	  l2_1_muon.SetPtEtaPhiM( l2_1_pt, l2_1_eta, l2_1_phi, 0.105658 );
	  l2_2_muon.SetPtEtaPhiM( l2_2_pt, l2_2_eta, l2_2_phi, 0.105658 );

	  l1_dimuon = l1_1_muon + l1_2_muon;
	  l2_dimuon = l2_1_muon + l2_2_muon;
	  
	  if(l2STP_ ){
	  h_st4muon_eta->Fill(l2_1_eta);
	  h_st4muon_eta->Fill(l2_2_eta);
      h_st4muon_pt->Fill(l2_1_pt);
	  h_st4muon_pt->Fill(l2_2_pt);
	  h_st4muon_p->Fill(l2_1_p);
	  h_st4muon_p->Fill(l2_2_p);
	  
	  h_st4dimuon_mass->Fill(l2_dimuon.M());
	  h_st4dimuon_eta->Fill(l2_dimuon.Eta());
	  h_st4dimuon_pt->Fill(l2_dimuon.Pt());
	  h_st4dimuon_p->Fill(l2_dimuon.P());

	  h_st4dimuon_eta_pt->Fill(l1_dimuon.Eta(), l1_dimuon.Pt());
	  
      }else if(l1STP_){
	  h_st4muon_eta->Fill(l1_1_eta);
	  h_st4muon_eta->Fill(l1_2_eta);
	  h_st4muon_pt->Fill(l1_1_pt);
	  h_st4muon_pt->Fill(l1_2_pt);
	  h_st4muon_p->Fill(l1_1_p);
	  h_st4muon_p->Fill(l1_2_p);

	  h_st4dimuon_mass->Fill(l1_dimuon.M());
	  h_st4dimuon_eta->Fill(l1_dimuon.Eta());
	  h_st4dimuon_pt->Fill(l1_dimuon.Pt());
	  h_st4dimuon_p->Fill(l1_dimuon.P());
	  
	  h_st4dimuon_eta_pt->Fill(l2_dimuon.Eta(), l2_dimuon.Pt());
	  }

  
  }
}

// ------------ method called once each job just before starting event loop  ------------

// ------------ method called once each job just after ending the event loop  ------------
void 
DimuonRecoSt34::endJob() {
		
		std::cout << "===== Start writing user histograms =====" << std::endl;
		
        hOutputFile->SetCompressionLevel(2);
        hOutputFile->cd();

		h_st3muon_eta->GetXaxis()->SetTitle("pseudorapidity #eta (rad)");
		h_st3muon_eta->SetLineWidth(2);
		h_st3muon_eta->SetFillColor(42);
		
		h_st3muon_pt->GetXaxis()->SetTitle("p_{T} (GeV/c)");
		h_st3muon_pt->SetLineWidth(2);
	    h_st3muon_pt->SetFillColor(42);
		
		h_st3muon_p->GetXaxis()->SetTitle("p (GeV/c)");
		h_st3muon_p->SetLineWidth(2);
		h_st3muon_p->SetFillColor(42);

		h_st3dimuon_eta->GetXaxis()->SetTitle("pseudorapidity #eta (rad)");
		h_st3dimuon_eta->SetLineWidth(2);
		h_st3dimuon_eta->SetFillColor(42);

		h_st3dimuon_pt->GetXaxis()->SetTitle("p_{T} (GeV/c)");
		h_st3dimuon_pt->SetLineWidth(2);
		h_st3dimuon_pt->SetFillColor(42);
		
		h_st3dimuon_p->GetXaxis()->SetTitle("p (GeV/c)");
		h_st3dimuon_p->SetLineWidth(2);
		h_st3dimuon_p->SetFillColor(42);

		h_st3dimuon_mass->GetXaxis()->SetTitle("Mass (GeV/c^{2})");
		h_st3dimuon_mass->SetLineWidth(2);
		h_st3dimuon_mass->SetFillColor(42);
		
		h_st4muon_eta->GetXaxis()->SetTitle("pseudorapidity #eta (rad)");
		h_st4muon_eta->SetLineWidth(2);
		h_st4muon_eta->SetFillColor(42);
		
		h_st4muon_pt->GetXaxis()->SetTitle("p_{T} (GeV/c)");
		h_st4muon_pt->SetLineWidth(2);
		h_st4muon_pt->SetFillColor(42);
		
		h_st4muon_p->GetXaxis()->SetTitle("p (GeV/c)");
		h_st4muon_p->SetLineWidth(2);
		h_st4muon_p->SetFillColor(42);

		h_st4dimuon_eta->GetXaxis()->SetTitle("pseudorapidity #eta (rad)");
		h_st4dimuon_eta->SetLineWidth(2);
		h_st4dimuon_eta->SetFillColor(42);
		
		h_st4dimuon_pt->GetXaxis()->SetTitle("p_{T} (GeV/c)");
		h_st4dimuon_pt->SetLineWidth(2);
		h_st4dimuon_pt->SetFillColor(42);
		
		h_st4dimuon_p->GetXaxis()->SetTitle("p (GeV/c)");
		h_st4dimuon_p->SetLineWidth(2);
		h_st4dimuon_p->SetFillColor(42);
		
		h_st4dimuon_mass->GetXaxis()->SetTitle("Mass (GeV/c^{2})");
		h_st4dimuon_mass->SetLineWidth(2);
		h_st4dimuon_mass->SetFillColor(42);

		h_st3dimuon_eta_pt->GetYaxis()->SetTitle("p_{T} (GeV/c)");
		h_st3dimuon_eta_pt->GetXaxis()->SetTitle("pseudorapidity #eta (rad)");
		h_st4dimuon_eta_pt->GetYaxis()->SetTitle("p_{T} (GeV/c)");
		h_st4dimuon_eta_pt->GetXaxis()->SetTitle("pseudorapidity #eta (rad)");

		hOutputFile->Write();
        hOutputFile->Close();
		std::cout << "===== End writing user histograms =======" << std::endl;
}

//define this as a plug-in
DEFINE_FWK_MODULE(DimuonRecoSt34);
