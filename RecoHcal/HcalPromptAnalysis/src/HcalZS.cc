// -*- C++ -*-
//
// Package:    HcalZS
// Class:      HcalZS
// 
/**\class HcalZS HcalZS.cc RecoHcal/HcalPromptAnalysis/src/HcalZS.cc

 SIMPLE EXAMPLE CODE demonstrating the access to officially reconstructed global run data.
 The config file is under the test directory.
 The config file should read the officially reconstructed root file in castor or 
 The output from Configuration/GlobalRuns/data/recoT0.cfg (official reco cfg). 

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Francesco Santanastasio, Sinjini Sengupta
// $Id: HcalZS.cc,v 1.1 2009/02/12 14:38:05 ssengupt Exp $
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

#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"
#include "CondFormats/HcalObjects/interface/HcalQIECoder.h"
#include "CalibFormats/HcalObjects/interface/HcalCalibrations.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "DataFormats/CaloTowers/interface/CaloTowerDetId.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"

#include "DataFormats/Candidate/interface/Candidate.h"

//HCAL ZS
#include "SimCalorimetry/HcalZeroSuppressionAlgos/interface/HcalZSAlgoRealistic.h"
#include "SimCalorimetry/HcalZeroSuppressionAlgos/interface/HcalZeroSuppressionAlgo.h"


//TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

//
#include "TH1.h"
#include "TH2.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include <TROOT.h>
#include <TSystem.h>
#include "TFile.h"
#include <TCanvas.h>
#include <cmath>

#include "RecoHcal/HcalPromptAnalysis/interface/HcalZS.h"
#include "SimCalorimetry/HcalZeroSuppressionAlgos/interface/HcalZSAlgoRealistic.h"

//
// constants, enums and typedefs
//
// enum ZSMode { zs_SingleChannel=0, zs_TriggerTowerOR=1, zs_AllDepthsOR=2 };

//
// static data member definitions
//


//
// constructors and destructor
//
HcalZS::HcalZS(const edm::ParameterSet& iConfig)

{
  //now do what ever initialization is needed
  NtotEvents = 0;

  thresholdHB_ = iConfig.getUntrackedParameter < int > ("thresholdHB", 8);
  thresholdHE_ = iConfig.getUntrackedParameter < int > ("thresholdHE", 8);
  thresholdHO_ = iConfig.getUntrackedParameter < int > ("thresholdHO", 8);
  thresholdHF_ = iConfig.getUntrackedParameter < int > ("thresholdHF", 9);

  cout << "using digi thresholdHB_ = " << thresholdHB_ << endl;
  cout << "using digi thresholdHE_ = " << thresholdHE_ << endl;
  cout << "using digi thresholdHO_ = " << thresholdHO_ << endl;
  cout << "using digi thresholdHF_ = " << thresholdHF_ << endl;

}


HcalZS::~HcalZS()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//


namespace ZSRealistic_impl {

  template <class DIGI> 
  bool keepMe(const DIGI& inp, int threshold) {
    bool keepIt=false;

    //determine the sum of 2 timeslices

    for (int i=0; i< inp.size()-1 && !keepIt; i++) {
      int sum=0;
      for (int j=i; j<(i+2); j++){
	sum+=inp[j].adc();
	//pedsum+=pedave;
      }
      if (sum>=threshold) keepIt=true;
    }
    return keepIt;
  }
}


// ------------ method called to for each event  ------------
void
HcalZS::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;
   using namespace reco;

   //## Declare digi collections

   //hcal digis (HBHE)
   Handle<HBHEDigiCollection> hbhe_digi_h; 
   //iEvent.getByLabel("hcalZeroSuppressedDigis",hbhe_digi_h);
   iEvent.getByLabel("hcalDigis",hbhe_digi_h);
   const HBHEDigiCollection* hbhe_digi = hbhe_digi_h.failedToGet () ? 0 : &*hbhe_digi_h;
   
   //## ZS Mark&Pass vs ZS Emulation

   if (hbhe_digi) { // object is available 
     
     //      cout << "hbhe_digi is available" << endl;
     //      cout << "hbhe_digi->size(): " << hbhe_digi->size() << endl;
     
     for (HBHEDigiCollection::const_iterator j=hbhe_digi->begin(); j!=hbhe_digi->end(); j++)
       {
	 const HBHEDataFrame digi = (const HBHEDataFrame)(*j);
	 HcalDetId id = digi.id();
	 bool agree = false;
	 
	 //-- Mark&Pass bit in the data
	 //	 cout << "digi.zsMarkAndPass(): " << digi.zsMarkAndPass() << endl;
	 
	 //-- ZS Emulation
	 if ( id.subdet() == HcalBarrel ) 
	   {
	     h_hbdigi_zsEmulation->Fill( ZSRealistic_impl::keepMe<HBHEDataFrame>(digi,thresholdHB_) );
	     // 	     cout << "ZSRealistic_impl::keepMe<HBHEDataFrame>(digi,thresholdHB_): "
	     // 		  << ZSRealistic_impl::keepMe<HBHEDataFrame>(digi,thresholdHB_) << endl;
	     
	     if( ZSRealistic_impl::keepMe<HBHEDataFrame>(digi,thresholdHB_) ==  digi.zsMarkAndPass() )
	       agree = true;
	   }
	 else
	   { 
	     h_hedigi_zsEmulation->Fill( ZSRealistic_impl::keepMe<HBHEDataFrame>(digi,thresholdHE_) );
	     // 	     cout << "ZSRealistic_impl::keepMe<HBHEDataFrame>(digi,thresholdHE_): "
	     // 		  << ZSRealistic_impl::keepMe<HBHEDataFrame>(digi,thresholdHE_) << endl;
	    
	     if( ZSRealistic_impl::keepMe<HBHEDataFrame>(digi,thresholdHE_) ==  digi.zsMarkAndPass() )
	       agree = true;
	   }
	 
	 //## Fill directly into Histogram?
	 if ( agree == true ){

	   if (id.subdet() == 1){
	     if (id.depth() == 1)
	       h_hbhf_d1->Fill(id.ieta(),id.iphi());
	     if (id.depth() == 2)
	       h_hbhf_d2->Fill(id.ieta(),id.iphi());
	   }
	 
	   if (id.subdet() == 2){
	     if (id.depth() == 1)
	       h_he_d1->Fill(id.ieta(),id.iphi());
	     if (id.depth() == 2)
	       h_he_d2->Fill(id.ieta(),id.iphi());
	     if (id.depth() == 3)
	       h_he_d3->Fill(id.ieta(),id.iphi());
	   }
	 } // end fill eta-phi histograms

       }// end loop of HBHE digi
   }//end check if hbhe_digi exists



   //hcal digis (HO)
   Handle<HODigiCollection> ho_digi_h; 
   iEvent.getByLabel("hcalDigis",ho_digi_h);
   const HODigiCollection* ho_digi = ho_digi_h.failedToGet () ? 0 : &*ho_digi_h;

   if (ho_digi) { // object is available 
     
     //      cout << "ho_digi is available" << endl;
     //      cout << "ho_digi->size(): " << ho_digi->size() << endl;
     
     for (HODigiCollection::const_iterator j=ho_digi->begin(); j!=ho_digi->end(); j++)
       {
	 const HODataFrame digi = (const HODataFrame)(*j);
	 HcalDetId id = digi.id();
	 bool agree = false;
	 
	 //-- Mark&Pass bit in the data
	 //	 cout << "digi.zsMarkAndPass(): " << digi.zsMarkAndPass() << endl;
	 
	 //-- ZS Emulation
	 h_hodigi_zsEmulation->Fill( ZSRealistic_impl::keepMe<HODataFrame>(digi,thresholdHO_) );
	 // 	 cout << "ZSRealistic_impl::keepMe<HODataFrame>(digi,thresholdHO_): "
	 // 	      << ZSRealistic_impl::keepMe<HODataFrame>(digi,thresholdHO_) << endl;

	 if ( ZSRealistic_impl::keepMe<HODataFrame>(digi,thresholdHO_) == digi.zsMarkAndPass() )
	   agree = true;

	 //## Fill directly into Histogram?
	 if ( agree == true ) 
	   if (id.subdet() == 3) 
	     h_ho_d1->Fill(id.ieta(),id.iphi());	 
	 
       }// end loop of HO digi
   }//end check if ho_digi exists



   //hcal digis (HF)
   Handle<HFDigiCollection> hf_digi_h; 
   iEvent.getByLabel("hcalDigis",hf_digi_h);
   const HFDigiCollection* hf_digi = hf_digi_h.failedToGet () ? 0 : &*hf_digi_h;

   if (hf_digi) { // object is available 
     
     //      cout << "hf_digi is available" << endl;
     //      cout << "hf_digi->size(): " << hf_digi->size() << endl;
     
     for (HFDigiCollection::const_iterator j=hf_digi->begin(); j!=hf_digi->end(); j++)
       {
	 const HFDataFrame digi = (const HFDataFrame)(*j);
	 HcalDetId id = digi.id();
	 bool agree = false;
	 
	 //-- Mark&Pass bit in the data
	 //	 cout << "digi.zsMarkAndPass(): " << digi.zsMarkAndPass() << endl;
	 
	 //-- ZS Emulation
	 h_hfdigi_zsEmulation->Fill( ZSRealistic_impl::keepMe<HFDataFrame>(digi,thresholdHF_) );
	 // 	 cout << "ZSRealistic_impl::keepMe<HFDataFrame>(digi,thresholdHF_): "
	 // 	      << ZSRealistic_impl::keepMe<HFDataFrame>(digi,thresholdHF_) << endl;
	 //if( digi.zsMarkAndPass() )
	   if ( ZSRealistic_impl::keepMe<HFDataFrame>(digi,thresholdHF_) == digi.zsMarkAndPass() )
	     agree = true;

	 //## Fill directly into Histogram?
	 if ( agree == true ){
	   if (id.subdet() == 4){
	     if (id.depth() == 1)
	       h_hbhf_d1->Fill(id.ieta(),id.iphi());
	     if (id.depth() == 2)
	       h_hbhf_d2->Fill(id.ieta(),id.iphi());
	   }
	 } //end fill eta-phi histograms

       }// end loop of HF digi
   }//end check if hf_digi exists

   //increment number of events
   NtotEvents++;
   
}


// ------------ method called once each job just before starting event loop  ------------
void HcalZS::beginJob(const edm::EventSetup&)
{
  TFileDirectory HcalDir = fs->mkdir( "Hcal" );

  //## book histograms
  h_hbdigi_zsEmulation = HcalDir.make<TH1F>("h_hbdigi_zsEmulation",
					    "h_hbdigi_zsEmulation",
					    2, -0.5, 1.5);

  h_hedigi_zsEmulation = HcalDir.make<TH1F>("h_hedigi_zsEmulation",
					    "h_hedigi_zsEmulation",
					    2, -0.5, 1.5);
  
  h_hodigi_zsEmulation = HcalDir.make<TH1F>("h_hodigi_zsEmulation",
					    "h_hodigi_zsEmulation",
					    2, -0.5, 1.5);
  
  h_hfdigi_zsEmulation = HcalDir.make<TH1F>("h_hfdigi_zsEmulation",
					    "h_hfdigi_zsEmulation",
					    2, -0.5, 1.5);

  h_hbhf_d1 = HcalDir.make<TH2F>("h_hcal_hbhfd1","HB HF Depth1",
				 83,-41.5,41.5,
				 72,0.5,72.5);
  h_hbhf_d1->GetXaxis()->SetTitle("i#eta");
  h_hbhf_d1->GetYaxis()->SetTitle("i#phi");

  h_hbhf_d2 = HcalDir.make<TH2F>("h_hcal_hbhfd2","HB HF Depth2",
				 83,-41.5,41.5,
				 72,0.5,72.5);
  h_hbhf_d2->GetXaxis()->SetTitle("i#eta");
  h_hbhf_d2->GetYaxis()->SetTitle("i#phi");

  h_he_d3   = HcalDir.make<TH2F>("h_hcal_hed3","HE Depth3",
				 83,-41.5,41.5,
				 72,0.5,72.5);
  h_he_d3->GetXaxis()->SetTitle("i#eta");
  h_he_d3->GetYaxis()->SetTitle("i#phi");

  h_he_d2   = HcalDir.make<TH2F>("h_hcal_hed2","HE Depth2",
				 83,-41.5,41.5,
				 72,0.5,72.5);
  h_he_d2->GetXaxis()->SetTitle("i#eta");
  h_he_d2->GetYaxis()->SetTitle("i#phi");

  h_he_d1   = HcalDir.make<TH2F>("h_hcal_hed1","HE Depth1",
				 83,-41.5,41.5,
				 72,0.5,72.5);
  h_he_d1->GetXaxis()->SetTitle("i#eta");
  h_he_d1->GetYaxis()->SetTitle("i#phi");

  h_ho_d1   = HcalDir.make<TH2F>("h_hcal_hod1","HO all Depth",
				 83,-41.5,41.5,
				 72,0.5,72.5);
  h_ho_d1->GetXaxis()->SetTitle("i#eta");
  h_ho_d1->GetYaxis()->SetTitle("i#phi");

}

// ------------ method called once each job just after ending the event loop  ------------
void HcalZS::endJob() {

  //   //divide each bin for NtotEvents
  //   h_hbhf_d1->Scale(scaleFactor);
  //   h_hbhf_d2->Scale(scaleFactor);
  //   h_he_d1->Scale(scaleFactor);
  //   h_he_d2->Scale(scaleFactor);
  //   h_he_d3->Scale(scaleFactor);
  //   h_ho_d1->Scale(scaleFactor);
  

  //h_hbhf_d1
  for(int ieta = 1 ; ieta <= h_hbhf_d1->GetNbinsX() ; ieta++)
    for(int iphi = 1 ; iphi <= h_hbhf_d1->GetNbinsY() ; iphi++)
      h_hbhf_d1->SetBinContent( ieta , iphi , float(h_hbhf_d1->GetBinContent(ieta,iphi) / NtotEvents)*100 );

  h_hbhf_d1->GetZaxis()->SetLimits(0,100);
  h_hbhf_d1->GetZaxis()->SetRangeUser(0,100);

  //h_hbhf_d2
  for(int ieta = 1 ; ieta <= h_hbhf_d2->GetNbinsX() ; ieta++)
    for(int iphi = 1 ; iphi <= h_hbhf_d2->GetNbinsY() ; iphi++)
      h_hbhf_d2->SetBinContent( ieta , iphi , float(h_hbhf_d2->GetBinContent(ieta,iphi) / NtotEvents)*100 );

  h_hbhf_d2->GetZaxis()->SetLimits(0,100);
  h_hbhf_d2->GetZaxis()->SetRangeUser(0,100);

  //h_he_d1
  for(int ieta = 1 ; ieta <= h_he_d1->GetNbinsX() ; ieta++)
    for(int iphi = 1 ; iphi <= h_he_d1->GetNbinsY() ; iphi++)
      h_he_d1->SetBinContent( ieta , iphi , float(h_he_d1->GetBinContent(ieta,iphi) / NtotEvents)*100 );

  h_he_d1->GetZaxis()->SetLimits(0,100);
  h_he_d1->GetZaxis()->SetRangeUser(0,100);

  //h_he_d2
  for(int ieta = 1 ; ieta <= h_he_d2->GetNbinsX() ; ieta++)
    for(int iphi = 1 ; iphi <= h_he_d2->GetNbinsY() ; iphi++)
      h_he_d2->SetBinContent( ieta , iphi , float(h_he_d2->GetBinContent(ieta,iphi) / NtotEvents)*100 );

  h_he_d2->GetZaxis()->SetLimits(0,100);
  h_he_d2->GetZaxis()->SetRangeUser(0,100);

  //h_he_d3
  for(int ieta = 1 ; ieta <= h_he_d3->GetNbinsX() ; ieta++)
    for(int iphi = 1 ; iphi <= h_he_d3->GetNbinsY() ; iphi++)
      h_he_d3->SetBinContent( ieta , iphi , float(h_he_d3->GetBinContent(ieta,iphi) / NtotEvents)*100 );

  h_he_d3->GetZaxis()->SetLimits(0,100);
  h_he_d3->GetZaxis()->SetRangeUser(0,100);

  //h_ho_d1
  for(int ieta = 1 ; ieta <= h_ho_d1->GetNbinsX() ; ieta++)
    for(int iphi = 1 ; iphi <= h_ho_d1->GetNbinsY() ; iphi++)
      h_ho_d1->SetBinContent( ieta , iphi , float(h_ho_d1->GetBinContent(ieta,iphi) / NtotEvents)*100 );

  h_ho_d1->GetZaxis()->SetLimits(0,100);
  h_ho_d1->GetZaxis()->SetRangeUser(0,100);

      
}

