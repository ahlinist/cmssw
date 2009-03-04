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
// $Id: HcalZS.cc,v 1.5 2009/03/03 19:31:53 santanas Exp $
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
const float LIMIT = 0.000000001;

//
// constructors and destructor
//
HcalZS::HcalZS(const edm::ParameterSet& iConfig)

{
  //now do what ever initialization is needed
  NtotEvents = 0;
  NtotDigis = 0;

  thresholdHB_ = iConfig.getUntrackedParameter < int > ("thresholdHB", 9);
  thresholdHE_ = iConfig.getUntrackedParameter < int > ("thresholdHE", 9);
  thresholdHO_ = iConfig.getUntrackedParameter < int > ("thresholdHO", 9);
  thresholdHF_ = iConfig.getUntrackedParameter < int > ("thresholdHF", 9);
  thresholdCalib_ = iConfig.getUntrackedParameter < int > ("thresholdCalib", 9);

  cout << "using digi thresholdHB_ = " << thresholdHB_ << endl;
  cout << "using digi thresholdHE_ = " << thresholdHE_ << endl;
  cout << "using digi thresholdHO_ = " << thresholdHO_ << endl;
  cout << "using digi thresholdHF_ = " << thresholdHF_ << endl;
  cout << "using digi thresholdCalib_ = " << thresholdCalib_ << endl;
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

	 //cout << "HBHE digi.zsUnsuppressed(): " << digi.zsUnsuppressed() << endl;
	 h_hbhe_US->Fill(digi.zsUnsuppressed());
	 if(digi.zsUnsuppressed() != 1)
	   continue;

	 //count digis
	 NtotDigis++;

	 //just counting
	 if (id.subdet() == 1){
	   if (id.depth() == 1)
	     h_hbhf_d1_all->Fill(id.ieta(),id.iphi());
	   if (id.depth() == 2)
	     h_hbhf_d2_all->Fill(id.ieta(),id.iphi());
	 }
	 
	 if (id.subdet() == 2){
	   if (id.depth() == 1)
	     h_he_d1_all->Fill(id.ieta(),id.iphi());
	   if (id.depth() == 2)
	     h_he_d2_all->Fill(id.ieta(),id.iphi());
	   if (id.depth() == 3)
	     h_he_d3_all->Fill(id.ieta(),id.iphi());
	 }
	 
	 //-- Mark&Pass bit in the data
	 //cout << "digi.zsMarkAndPass(): " << digi.zsMarkAndPass() << endl;
	 
	 //-- ZS Emulation
	 if ( id.subdet() == HcalBarrel ) 
	   {	  
	     h_hbdigi_zsEmulation->Fill( ZSRealistic_impl::keepMe<HBHEDataFrame>(digi,thresholdHB_) );
	     // 	     cout << "ZSRealistic_impl::keepMe<HBHEDataFrame>(digi,thresholdHB_): "
	     // 		  << ZSRealistic_impl::keepMe<HBHEDataFrame>(digi,thresholdHB_) << endl;
	     
	     if( ZSRealistic_impl::keepMe<HBHEDataFrame>(digi,thresholdHB_) !=  digi.zsMarkAndPass() )
	       agree = true;
	   }
	 else
	   { 
	     h_hedigi_zsEmulation->Fill( ZSRealistic_impl::keepMe<HBHEDataFrame>(digi,thresholdHE_) );
	     // 	     cout << "ZSRealistic_impl::keepMe<HBHEDataFrame>(digi,thresholdHE_): "
	     // 		  << ZSRealistic_impl::keepMe<HBHEDataFrame>(digi,thresholdHE_) << endl;
	    
	     if( ZSRealistic_impl::keepMe<HBHEDataFrame>(digi,thresholdHE_) !=  digi.zsMarkAndPass() )
	       agree = true;
	   }


	 h_ZSagree->Fill(agree);

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
	 }// end fill eta-phi histograms

	 if ( agree == false ){

	   if (id.subdet() == 1){
	     if (id.depth() == 1)
	       h_hbhf_d1->Fill(id.ieta(),id.iphi(),0);
	     if (id.depth() == 2)
	       h_hbhf_d2->Fill(id.ieta(),id.iphi(),0);
	   }
	 
	   if (id.subdet() == 2){
	     if (id.depth() == 1)
	       h_he_d1->Fill(id.ieta(),id.iphi(),0);
	     if (id.depth() == 2)
	       h_he_d2->Fill(id.ieta(),id.iphi(),0);
	     if (id.depth() == 3)
	       h_he_d3->Fill(id.ieta(),id.iphi(),0);
	   }
	 }// end fill eta-phi histograms
	
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

	 //cout << "HO digi.zsUnsuppressed(): " << digi.zsUnsuppressed() << endl;
	 h_ho_US->Fill(digi.zsUnsuppressed());
	 if(digi.zsUnsuppressed() != 1)
	   continue;

	 //count digis
	 NtotDigis++;
	 
	 //just counting
	 if (id.subdet() == 3) 
	   h_ho_d4_all->Fill(id.ieta(),id.iphi());	 
	
	 //-- Mark&Pass bit in the data
	 //	 cout << "digi.zsMarkAndPass(): " << digi.zsMarkAndPass() << endl;
	 
	 //-- ZS Emulation
	 h_hodigi_zsEmulation->Fill( ZSRealistic_impl::keepMe<HODataFrame>(digi,thresholdHO_) );
	 // 	 cout << "ZSRealistic_impl::keepMe<HODataFrame>(digi,thresholdHO_): "
	 // 	      << ZSRealistic_impl::keepMe<HODataFrame>(digi,thresholdHO_) << endl;

	 if ( ZSRealistic_impl::keepMe<HODataFrame>(digi,thresholdHO_) != digi.zsMarkAndPass() )
	   agree = true;

	 h_ZSagree->Fill(agree);

	 //## Fill directly into Histogram?
	 if ( agree == true ) 
	   if (id.subdet() == 3) 
	     h_ho_d4->Fill(id.ieta(),id.iphi());	 

	 if ( agree == false ) 
	   if (id.subdet() == 3) 
	     h_ho_d4->Fill(id.ieta(),id.iphi(),0);	 

	 
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

	 //cout << "HF digi.zsUnsuppressed(): " << digi.zsUnsuppressed() << endl;
	 h_hf_US->Fill(digi.zsUnsuppressed());
	 if(digi.zsUnsuppressed() != 1)
	   continue;

	 //count digis
	 NtotDigis++;

	 //just counting
	 if (id.subdet() == 4){
	   if (id.depth() == 1)
	     h_hbhf_d1_all->Fill(id.ieta(),id.iphi());
	   if (id.depth() == 2)
	     h_hbhf_d2_all->Fill(id.ieta(),id.iphi());
	 }
 
	 //-- Mark&Pass bit in the data
	 //	 cout << "digi.zsMarkAndPass(): " << digi.zsMarkAndPass() << endl;
	 
	 //-- ZS Emulation
	 h_hfdigi_zsEmulation->Fill( ZSRealistic_impl::keepMe<HFDataFrame>(digi,thresholdHF_) );
	 // 	 cout << "ZSRealistic_impl::keepMe<HFDataFrame>(digi,thresholdHF_): "
	 // 	      << ZSRealistic_impl::keepMe<HFDataFrame>(digi,thresholdHF_) << endl;
	 //if( digi.zsMarkAndPass() )
	 if ( ZSRealistic_impl::keepMe<HFDataFrame>(digi,thresholdHF_) != digi.zsMarkAndPass() )
	   agree = true;

	 h_ZSagree->Fill(agree);

	 //## Fill directly into Histogram?
	 if ( agree == true ){
	   if (id.subdet() == 4){
	     if (id.depth() == 1)
	       h_hbhf_d1->Fill(id.ieta(),id.iphi());
	     if (id.depth() == 2)
	       h_hbhf_d2->Fill(id.ieta(),id.iphi());
	   }
	 }//end fill eta-phi histograms

	 if ( agree == false ){
	   if (id.subdet() == 4){
	     if (id.depth() == 1)
	       h_hbhf_d1->Fill(id.ieta(),id.iphi(),0);
	     if (id.depth() == 2)
	       h_hbhf_d2->Fill(id.ieta(),id.iphi(),0);
	   }
	 }//end fill eta-phi histograms

       }// end loop of HF digi
   }//end check if hf_digi exists


   //calibration channels
   edm::Handle<HcalCalibDigiCollection> calib;
   iEvent.getByType(calib);
   const HcalCalibDigiCollection* calib_digi = calib.failedToGet () ? 0 : &*calib;
   
   if(calib_digi) { // object is available 
     
     for(HcalCalibDigiCollection::const_iterator calibdigi=calib_digi->begin();calibdigi!=calib_digi->end();calibdigi++)
       {

	 // int nTS=calibdigi->size();
	 // double e=0;
	 // for(int i=0;i<nTS;i++){ e+=calibdigi->sample(i).adc(); }
	 // 	 printf("hcalSubdet: %i ieta: %i iphi: %i Energy: %f\n",
	 // 		calibdigi->id().hcalSubdet(),calibdigi->id().ieta(),calibdigi->id().iphi(),e);

	 const HcalCalibDataFrame digi = (const HcalCalibDataFrame)(*calibdigi);
	 bool agree = false;

	 //cout << "HF digi.zsUnsuppressed(): " << digi.zsUnsuppressed() << endl;
	 h_calib_US->Fill(digi.zsUnsuppressed());
	 if(digi.zsUnsuppressed() != 1)
	   continue;

	 //count digis
	 NtotDigis++;

	 //-- ZS Emulation
	 h_calib_zsEmulation->Fill( ZSRealistic_impl::keepMe<HcalCalibDataFrame>(digi,thresholdCalib_) );
	 if ( ZSRealistic_impl::keepMe<HcalCalibDataFrame>(digi,thresholdCalib_) != digi.zsMarkAndPass() )
	   agree = true;

	 h_ZSagree_calib->Fill(agree);
	 h_ZSagree->Fill(agree);

       }
   }

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

  h_calib_zsEmulation = HcalDir.make<TH1F>("h_calib_zsEmulation",
					   "h_calib_zsEmulation",
					   2, -0.5, 1.5);

  //ZS global agree
  h_ZSagree = HcalDir.make<TH1F>("h_ZSagree",
				 "h_ZSagree",
				 2, -0.5, 1.5);

  h_ZSagree_calib = HcalDir.make<TH1F>("h_ZSagree_calib",
				       "h_ZSagree_calib",
				       2, -0.5, 1.5);

  //US
  h_hbhe_US = HcalDir.make<TH1F>("h_hbhe_US",
				 "h_hbhe_US",
				 2, -0.5, 1.5);
  h_ho_US = HcalDir.make<TH1F>("h_ho_US",
			       "h_ho_US",
			       2, -0.5, 1.5);
  h_hf_US = HcalDir.make<TH1F>("h_hf_US",
			       "h_hf_US",
			       2, -0.5, 1.5);
  h_calib_US = HcalDir.make<TH1F>("h_calib_US",
				  "h_calib_US",
				  2, -0.5, 1.5);

  //M&P agreement
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

  h_ho_d4   = HcalDir.make<TH2F>("h_hcal_hod4","HO Depth4",
				 83,-41.5,41.5,
				 72,0.5,72.5);
  h_ho_d4->GetXaxis()->SetTitle("i#eta");
  h_ho_d4->GetYaxis()->SetTitle("i#phi");


  //just counting
  h_hbhf_d1_all = HcalDir.make<TH2F>("h_hcal_hbhfd1_all","HB HF Depth1",
				 83,-41.5,41.5,
				 72,0.5,72.5);
  h_hbhf_d1_all->GetXaxis()->SetTitle("i#eta");
  h_hbhf_d1_all->GetYaxis()->SetTitle("i#phi");

  h_hbhf_d2_all = HcalDir.make<TH2F>("h_hcal_hbhfd2_all","HB HF Depth2",
				 83,-41.5,41.5,
				 72,0.5,72.5);
  h_hbhf_d2_all->GetXaxis()->SetTitle("i#eta");
  h_hbhf_d2_all->GetYaxis()->SetTitle("i#phi");

  h_he_d3_all   = HcalDir.make<TH2F>("h_hcal_hed3_all","HE Depth3",
				 83,-41.5,41.5,
				 72,0.5,72.5);
  h_he_d3_all->GetXaxis()->SetTitle("i#eta");
  h_he_d3_all->GetYaxis()->SetTitle("i#phi");

  h_he_d2_all   = HcalDir.make<TH2F>("h_hcal_hed2_all","HE Depth2",
				 83,-41.5,41.5,
				 72,0.5,72.5);
  h_he_d2_all->GetXaxis()->SetTitle("i#eta");
  h_he_d2_all->GetYaxis()->SetTitle("i#phi");

  h_he_d1_all   = HcalDir.make<TH2F>("h_hcal_hed1_all","HE Depth1",
				 83,-41.5,41.5,
				 72,0.5,72.5);
  h_he_d1_all->GetXaxis()->SetTitle("i#eta");
  h_he_d1_all->GetYaxis()->SetTitle("i#phi");

  h_ho_d4_all   = HcalDir.make<TH2F>("h_hcal_hod4_all","HO Depth4",
				 83,-41.5,41.5,
				 72,0.5,72.5);
  h_ho_d4_all->GetXaxis()->SetTitle("i#eta");
  h_ho_d4_all->GetYaxis()->SetTitle("i#phi");

}

// ------------ method called once each job just after ending the event loop  ------------
void HcalZS::endJob() {

  cout << "########## SUMMARY OF THE M&P TEST ##########" << endl;
  float LIMITPREC = 1./float(NtotEvents);
  float LIMITPRECALL = 1./float(NtotDigis);

  cout << "NtotEvents: " << NtotEvents << endl;
  cout << "NtotDigis: " << NtotDigis << endl;
  cout << "Limit precision for this test on the single channel: " << LIMITPREC << endl;
  cout << "Limit precision for this test (considering all the digi): " << LIMITPRECALL << endl;

  /*

  //divide each bin for NtotEvents and final report

  //h_hbhf_d1
  for(int ieta = 1 ; ieta <= h_hbhf_d1->GetNbinsX() ; ieta++)
    for(int iphi = 1 ; iphi <= h_hbhf_d1->GetNbinsY() ; iphi++)
      {
	int thisNtotEvents = int(h_hbhf_d1_all->GetBinContent(ieta,iphi));
	if( thisNtotEvents == 0 ) 
	  h_hbhf_d1->SetBinContent( ieta , iphi , 0. );
	else
	  {
	    float percDisagree = 1 - float(h_hbhf_d1->GetBinContent(ieta,iphi) / thisNtotEvents);
	    if(percDisagree == 0) 
	      {
		percDisagree = -LIMITPREC;
	      }
	    h_hbhf_d1->SetBinContent( ieta , iphi , percDisagree);
	    if(percDisagree > LIMITPREC)
	      {
		cout << "HB/HF depth=1" 
		     << " ieta=" << ieta - 42 << " iphi=" << iphi 
		     << " percentage of disagree=" << percDisagree 
		     << " (number of digis used for the test=" << thisNtotEvents << ")" << endl;
	      }
	  }
      }
  
  h_hbhf_d1->SetMinimum(LIMITPREC);
  h_hbhf_d1->GetZaxis()->SetLimits(LIMITPREC,1);
  h_hbhf_d1->GetZaxis()->SetRangeUser(LIMITPREC,1);

  //h_hbhf_d2
  for(int ieta = 1 ; ieta <= h_hbhf_d2->GetNbinsX() ; ieta++)
    for(int iphi = 1 ; iphi <= h_hbhf_d2->GetNbinsY() ; iphi++)
      {
	int thisNtotEvents = int(h_hbhf_d2_all->GetBinContent(ieta,iphi));
	if( thisNtotEvents == 0 ) 
	  h_hbhf_d2->SetBinContent( ieta , iphi , 0. );
	else
	  {
	    float percDisagree = 1 - float(h_hbhf_d2->GetBinContent(ieta,iphi) / thisNtotEvents);
	    if(percDisagree == 0) 
	      percDisagree = -LIMITPREC;
	    h_hbhf_d2->SetBinContent( ieta , iphi , percDisagree);
	    if(percDisagree > LIMITPREC)
	      {
		cout << "HB/HF depth=2" 
		     << " ieta=" << ieta - 42 << " iphi=" << iphi 
		     << " percentage of disagree=" << percDisagree 
		     << " (number of digis used for the test=" << thisNtotEvents << ")" << endl;
	      }
	  }
      }
  h_hbhf_d2->SetMinimum(LIMITPREC);
  h_hbhf_d2->GetZaxis()->SetLimits(LIMITPREC,1);
  h_hbhf_d2->GetZaxis()->SetRangeUser(LIMITPREC,1);

  //h_he_d1
  for(int ieta = 1 ; ieta <= h_he_d1->GetNbinsX() ; ieta++)
    for(int iphi = 1 ; iphi <= h_he_d1->GetNbinsY() ; iphi++)
      {
	int thisNtotEvents = int(h_he_d1_all->GetBinContent(ieta,iphi));
	if( thisNtotEvents == 0 ) 
	  h_he_d1->SetBinContent( ieta , iphi , 0. );
	else
	  {
	    float percDisagree = 1 - float(h_he_d1->GetBinContent(ieta,iphi) / thisNtotEvents);
	    if(percDisagree == 0) 
	      percDisagree = -LIMITPREC;
	    h_he_d1->SetBinContent( ieta , iphi , percDisagree);
	    if(percDisagree > LIMITPREC)
	      {
		cout << "HE depth=1" 
		     << " ieta=" << ieta - 42 << " iphi=" << iphi 
		     << " percentage of disagree=" << percDisagree 
		     << " (number of digis used for the test=" << thisNtotEvents << ")" << endl;
	      }
	  }
      }
  h_he_d1->SetMinimum(LIMITPREC);
  h_he_d1->GetZaxis()->SetLimits(LIMITPREC,1);
  h_he_d1->GetZaxis()->SetRangeUser(LIMITPREC,1);

  //h_he_d2
  for(int ieta = 1 ; ieta <= h_he_d2->GetNbinsX() ; ieta++)
    for(int iphi = 1 ; iphi <= h_he_d2->GetNbinsY() ; iphi++)
      {
	int thisNtotEvents = int(h_he_d2_all->GetBinContent(ieta,iphi));
	if( thisNtotEvents == 0 ) 
	  h_he_d2->SetBinContent( ieta , iphi , 0. );
	else
	  {
	    float percDisagree = 1 - float(h_he_d2->GetBinContent(ieta,iphi) / thisNtotEvents);
	    if(percDisagree == 0) 
	      percDisagree = -LIMITPREC;
	    h_he_d2->SetBinContent( ieta , iphi , percDisagree);
	    if(percDisagree > LIMITPREC)
	      {
		cout << "HE depth=2" 
		     << " ieta=" << ieta - 42 << " iphi=" << iphi 
		     << " percentage of disagree=" << percDisagree 
		     << " (number of digis used for the test=" << thisNtotEvents << ")" << endl;
	      }
	  }
      }
  h_he_d2->SetMinimum(LIMITPREC);
  h_he_d2->GetZaxis()->SetLimits(LIMITPREC,1);
  h_he_d2->GetZaxis()->SetRangeUser(LIMITPREC,1);

  //h_he_d3
  for(int ieta = 1 ; ieta <= h_he_d3->GetNbinsX() ; ieta++)
    for(int iphi = 1 ; iphi <= h_he_d3->GetNbinsY() ; iphi++)
      {
	int thisNtotEvents = int(h_he_d3_all->GetBinContent(ieta,iphi));
	if( thisNtotEvents == 0 ) 
	  h_he_d3->SetBinContent( ieta , iphi , 0. );
	else
	  {
	    float percDisagree = 1 - float(h_he_d3->GetBinContent(ieta,iphi) / thisNtotEvents);
	    if(percDisagree == 0) 
	      percDisagree = -LIMITPREC;
	    h_he_d3->SetBinContent( ieta , iphi , percDisagree);
	    if(percDisagree > LIMITPREC)
	      {
		cout << "HE depth=3" 
		     << " ieta=" << ieta - 42 << " iphi=" << iphi 
		     << " percentage of disagree=" << percDisagree 
		     << " (number of digis used for the test=" << thisNtotEvents << ")" << endl;
	      }
	  }
      }

  h_he_d3->SetMinimum(LIMITPREC);
  h_he_d3->GetZaxis()->SetLimits(LIMITPREC,1);
  h_he_d3->GetZaxis()->SetRangeUser(LIMITPREC,1);

  //h_ho_d4
  for(int ieta = 1 ; ieta <= h_ho_d4->GetNbinsX() ; ieta++)
    for(int iphi = 1 ; iphi <= h_ho_d4->GetNbinsY() ; iphi++)
      {
	int thisNtotEvents = int(h_ho_d4_all->GetBinContent(ieta,iphi));
	if( thisNtotEvents == 0 ) 
	  h_ho_d4->SetBinContent( ieta , iphi , 0. );
	else
	  {
	    float percDisagree = 1 - float(h_ho_d4->GetBinContent(ieta,iphi) / thisNtotEvents);
	    if(percDisagree == 0) 
	      percDisagree = -LIMITPREC;
	    h_ho_d4->SetBinContent( ieta , iphi , percDisagree);
	    if(percDisagree > LIMITPREC)
	      {
		cout << "HO depth=4" 
		     << " ieta=" << ieta - 42 << " iphi=" << iphi 
		     << " percentage of disagree=" << percDisagree 
		     << " (number of digis used for the test=" << thisNtotEvents << ")" << endl;
	      }
	  }
      }

  h_ho_d4->SetMinimum(LIMITPREC);
  h_ho_d4->GetZaxis()->SetLimits(LIMITPREC,1);
  h_ho_d4->GetZaxis()->SetRangeUser(LIMITPREC,1);

  */

}

