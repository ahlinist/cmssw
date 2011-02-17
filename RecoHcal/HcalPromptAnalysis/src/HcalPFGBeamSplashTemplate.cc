// -*- C++ -*-
//
// Package:    HcalPFGBeamSplashTemplate
// Class:      HcalPFGBeamSplashTemplate
// 
// \class HcalPFGBeamSplashTemplate HcalPFGBeamSplashTemplate.cc /RecoHcal/HcalPromptAnalysis/src/HcalPFGBeamSplashTemplate.cc
//
// SIMPLE EXAMPLE CODE for Hcal PFG analyses
// The config file is under the test directory.
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

//ecal
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"

//trigger
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuRegionalCand.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTReadoutCollection.h"

//tracks
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

//TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

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

#include "RecoHcal/HcalPromptAnalysis/interface/HcalPFGBeamSplashTemplate.h"

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
HcalPFGBeamSplashTemplate::HcalPFGBeamSplashTemplate(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed

}


HcalPFGBeamSplashTemplate::~HcalPFGBeamSplashTemplate()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void HcalPFGBeamSplashTemplate::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace std;
  using namespace reco;

  //###################
  //DECLARE DOLLECTIONS
  //###################


  //####### Digis ######

  //hcal digis
  //    Handle<HBHEDigiCollection> hbhe_digi_h; 
  //    //   iEvent.getByLabel("hcalZeroSuppressedDigis",hbhe_digi_h);
  //    iEvent.getByLabel("hcalDigis",hbhe_digi_h);
  //    const HBHEDigiCollection* hbhe_digi = hbhe_digi_h.failedToGet () ? 0 : &*hbhe_digi_h;
  //also:
  //HODigiCollection
  //HFDigiCollection
  //HcalCalibDigiCollection

  //####### RecHits #####

  //hcal rechits
  Handle<HBHERecHitCollection> hbhe_hits_h;
  iEvent.getByLabel("hbhereco", hbhe_hits_h);
  const HBHERecHitCollection* hbhe_hits = hbhe_hits_h.failedToGet () ? 0 : &*hbhe_hits_h;

  Handle<HORecHitCollection> ho_hits_h;
  iEvent.getByLabel("horeco", ho_hits_h);
  const HORecHitCollection* ho_hits = ho_hits_h.failedToGet () ? 0 : &*ho_hits_h;

  Handle<HFRecHitCollection> hf_hits_h;
  iEvent.getByLabel("hfreco", hf_hits_h);
  const HFRecHitCollection* hf_hits = hf_hits_h.failedToGet () ? 0 : &*hf_hits_h;

  //HORecHitCollection
  //HFRecHitCollection
  //HcalCalibRecHitCollection

  //ecal rechits
  Handle<EcalRecHitCollection> ebrechit_h;
  iEvent.getByLabel("ecalRecHit","EcalRecHitsEB",ebrechit_h);
  const EcalRecHitCollection*  ebrechit = ebrechit_h.failedToGet () ? 0 : &*ebrechit_h;

  Handle<EcalRecHitCollection> eerechit_h;
  iEvent.getByLabel("ecalRecHit","EcalRecHitsEE",eerechit_h);
  const EcalRecHitCollection*  eerechit = eerechit_h.failedToGet () ? 0 : &*eerechit_h;



  //####### towers, clusters and jets #######

  //   //calo towers
  //   Handle<CaloTowerCollection> calo_h;
  //   iEvent.getByLabel("towerMaker", calo_h);
  //   const CaloTowerCollection* towers = calo_h.failedToGet () ? 0 :&*calo_h;
  
  //ecal clusters
  //    Handle<reco::BasicClusterCollection> pIslandBarrelBasicClusters_h;
  //    iEvent.getByLabel("islandBasicClusters","islandBarrelBasicClusters",pIslandBarrelBasicClusters_h);
  //    const BasicClusterCollection* islandBarrelBasicClusters = pIslandBarrelBasicClusters_h.failedToGet () ? 0 : &*pIslandBarrelBasicClusters_h;
  
  //   // reco jets
  //   Handle<CaloJetCollection> caloJet_h;
  //   iEvent.getByLabel("iterativeCone5CaloJets",caloJet_h);
  //   const CaloJetCollection* cjet = caloJet_h.failedToGet () ? 0 : &*caloJet_h;
  
  //######## muons #######
  
  //   //Cosmic Muons
  //   Handle<TrackCollection> cosmicmuon_h;
  //   iEvent.getByLabel("cosmicMuons",cosmicmuon_h);
  //   const TrackCollection* cosmicmuon = cosmicmuon_h.failedToGet () ? 0 : &*cosmicmuon_h;
  

  //######## trigger info ########

  //   //trigger
  //   Handle<L1MuGMTReadoutCollection> gmtrc_h; 
  //   iEvent.getByLabel("l1GmtEmulDigis",gmtrc_h);
  //   const L1MuGMTReadoutCollection* gmtrc = gmtrc_h.failedToGet () ? 0 : &*gmtrc_h;

  // L1 global trigger
  Handle <L1GlobalTriggerReadoutRecord> gtRecord_h;
  iEvent.getByType (gtRecord_h); // assume only one L1 trigger record here
  const L1GlobalTriggerReadoutRecord* gtRecord = gtRecord_h.failedToGet () ? 0 : &*gtRecord_h;

  //######### MET, SumET, HT, .... ########

  //NOTE: use it only if the MET is reconstructed in the event
  // MET  
  Handle <CaloMETCollection> recmet_h;
  iEvent.getByLabel("met", recmet_h);
  const CaloMET* met  = recmet_h.failedToGet () ? 0 : &(recmet_h->front());

  //######################################################################################
  //######################################################################################
   
   
  //################
  //PERFORM ANALYSIS
  //################


  //## EVENT quantities

  //cout << "iEvent.bunchCrossing() : " << iEvent.bunchCrossing() << endl;
  //cout << "iEvent.orbitNumber() : " << iEvent.orbitNumber() << endl;  

  h_event_bx->Fill(iEvent.bunchCrossing());


  //## L1 TRIGGER quantities
   
  // L1 Global trigger
  if (gtRecord) { // object is available
    
    //algo trigger bits
    for (int l1bit = 0; l1bit < 128; ++l1bit) {
      if (gtRecord->decisionWord() [l1bit]) h_l1_gtbit->Fill (l1bit);
    }
    
    
    //technical trigger bits.

    for (int l1bit = 0; l1bit < 64; ++l1bit) {
      if (gtRecord->technicalTriggerWord() [l1bit]) h_l1_gtbit_techtrig->Fill (l1bit);
    }
    //All tech. triggers listed here (https://twiki.cern.ch/twiki/bin/view/CMS/L1TechnicalTriggerBits)   
    //gtRecord->technicalTriggerWord() [0]) //BPTX+ and BPTX-
    //gtRecord->technicalTriggerWord() [1]) //BPTX+ (beam 1)
    //gtRecord->technicalTriggerWord() [2]) //BPTX- (beam 2)
    //gtRecord->technicalTriggerWord() [3]) //BPTX+ or BPTX-
    //gtRecord->technicalTriggerWord() [4]) //BPTX+ and BPTX-
    //gtRecord->technicalTriggerWord() [5]) //BPTX+ and (not BPTX-)
    //gtRecord->technicalTriggerWord() [6]) //BPTX- and (not BPTX+)
    //gtRecord->technicalTriggerWord() [8]) //HF total OR (coincidence between any two HFP wedges)
    //gtRecord->technicalTriggerWord() [9]) //HF total OR (coincidence between any two wedges) (min-bias)
    //gtRecord->technicalTriggerWord() [10]) //HF total OR (coincidence between any two HFM wedges)
    //gtRecord->technicalTriggerWord() [11]) //HO total OR
    //gtRecord->technicalTriggerWord() [12]) //HB/HE total OR (2/3 of channels) (only after CRAFT09)
    
  }


  //## HCAL quantities

  //needs digis 
  //   if (hbhe_digi) { // object is available 
  //     int adcs[10];
  //     //CORRECT:  Timing plot should be done using linearized ADC's!
  //     for (HBHEDigiCollection::const_iterator j=hbhe_digi->begin(); j!=hbhe_digi->end(); j++){
  //       const HBHEDataFrame digi = (const HBHEDataFrame)(*j);
  //       HcalDetId id = digi.id();
  //       if (id.subdet() != 1) continue;
  //        int maxadc = 0;
  //        for (int TS = 0; TS < 10 && TS < digi.size(); ++TS){     
  // 	 adcs[TS] = digi[TS].adc();
  // 	 if (digi[TS].adc() > maxadc) maxadc = digi[TS].adc();
  //        }
  //        for (int TS = 0; TS < 10 && TS < digi.size(); ++TS){     
  // 	 if (maxadc > 10)	 h_hbtiming->Fill(TS,adcs[TS]);
  //        }
  //      }
  //    }
  
  float SumE_hb = 0;
  float SumE_he = 0;
  float SumE_ho = 0;
  float SumE_hf = 0;
  float SumE_htot = 0;
  
  float E_threshold = 10;
  int NhitAboveThr_hb = 0;
  int NhitAboveThr_he = 0;
  int NhitAboveThr_ho = 0; 
  int NhitAboveThr_hf = 0;
  int NhitAboveThr_htot = 0;

  if (hbhe_hits) { // object is available 
    for (HBHERecHitCollection::const_iterator hhit=hbhe_hits->begin(); hhit!=hbhe_hits->end(); hhit++) {      
      if(hhit->id().subdet() == HcalBarrel)
	{
	  SumE_hb += hhit->energy();
	  if(hhit->energy() > E_threshold)
	    NhitAboveThr_hb++;
	}	
      else
	{
	  SumE_he += hhit->energy();
	  if(hhit->energy() > E_threshold)
	    NhitAboveThr_he++;
	}	
    }
  }


  if (ho_hits) { // object is available 
    for (HORecHitCollection::const_iterator hhit=ho_hits->begin(); hhit!=ho_hits->end(); hhit++) {
      SumE_ho += hhit->energy();
      if(hhit->energy() > E_threshold)
	NhitAboveThr_ho++;	
    }
  }


  if (hf_hits) { // object is available 
    for (HFRecHitCollection::const_iterator hhit=hf_hits->begin(); hhit!=hf_hits->end(); hhit++) {
      SumE_hf += hhit->energy();	
      if(hhit->energy() > E_threshold)
	NhitAboveThr_hf++;      
    }
  }


  SumE_htot = SumE_hb + SumE_he +SumE_ho + SumE_hf;
  NhitAboveThr_htot = NhitAboveThr_hb + NhitAboveThr_he + NhitAboveThr_ho + NhitAboveThr_hf;

  h_hcal_SumE_hb->Fill(SumE_hb);
  h_hcal_SumE_he->Fill(SumE_he);
  h_hcal_SumE_ho->Fill(SumE_ho);
  h_hcal_SumE_hf->Fill(SumE_hf);
  h_hcal_SumE_tot->Fill(SumE_htot);

  h_hcal_NhitAboveThr_hb->Fill(NhitAboveThr_hb);
  h_hcal_NhitAboveThr_he->Fill(NhitAboveThr_he);
  h_hcal_NhitAboveThr_ho->Fill(NhitAboveThr_ho);
  h_hcal_NhitAboveThr_hf->Fill(NhitAboveThr_hf);
  h_hcal_NhitAboveThr_tot->Fill(NhitAboveThr_htot);


  //## ECAL quantities

  float SumE_eb = 0;
  float SumE_ee = 0;
  float SumE_etot = 0;

  int NhitAboveThr_ee = 0; 
  int NhitAboveThr_eb = 0;
  int NhitAboveThr_etot = 0;

  if (ebrechit) { // object is available 
    for (EcalRecHitCollection::const_iterator ehit=ebrechit->begin(); ehit!=ebrechit->end(); ehit++) {      
      SumE_eb += ehit->energy();
      if(ehit->energy() > E_threshold)
	NhitAboveThr_eb++;
    }
  }


  if (eerechit) { // object is available 
    for (EcalRecHitCollection::const_iterator ehit=eerechit->begin(); ehit!=eerechit->end(); ehit++) {      
      SumE_ee += ehit->energy();
      if(ehit->energy() > E_threshold)
	NhitAboveThr_ee++;
    }
  }

  SumE_etot = SumE_eb + SumE_ee;
  NhitAboveThr_etot = NhitAboveThr_eb + NhitAboveThr_ee;


  h_ecal_SumE_eb->Fill(SumE_eb);
  h_ecal_SumE_ee->Fill(SumE_ee);
  h_ecal_SumE_etot->Fill(SumE_etot);
  
  
  //Print-out of interesting events

  if( SumE_htot > 1000000
      //       NhitAboveThr_hb > 1000 
      //       && NhitAboveThr_he > 1000 
      //       && NhitAboveThr_ho > 1000 
      //       && NhitAboveThr_hf > 1000 )
      )
    {
      std::cout << "#### Splash event candidate ####" << std::endl; 
      std::cout << "iEvent.id().event() : " << iEvent.id().event() << std::endl;
      std::cout << "iEvent.bunchCrossing() : " << iEvent.bunchCrossing() << std::endl;
      std::cout << "iEvent.orbitNumber() : " << iEvent.orbitNumber() << std::endl;  
      std::cout << "--------------------------------" << std::endl;
      std::cout << "## HCAL ##" << std::endl;
      std::cout << "SumE_hb = " << SumE_hb << std::endl;
      std::cout << "SumE_he = " << SumE_he << std::endl;
      std::cout << "SumE_ho = " << SumE_ho << std::endl;
      std::cout << "SumE_hf = " << SumE_hf << std::endl;
      std::cout << "SumE_htot = " << SumE_htot << std::endl;
      std::cout << "NhitAboveThr_hb = " << NhitAboveThr_hb << std::endl;
      std::cout << "NhitAboveThr_he = " << NhitAboveThr_he << std::endl;
      std::cout << "NhitAboveThr_hf = " << NhitAboveThr_hf << std::endl;
      std::cout << "NhitAboveThr_ho = " << NhitAboveThr_ho << std::endl;
      std::cout << "NhitAboveThr_htot = " << NhitAboveThr_htot << std::endl;
      std::cout << "--------------------------------" << std::endl;
      std::cout << "## ECAL ##" << std::endl;
      std::cout << "SumE_eb = " << SumE_eb << std::endl;
      std::cout << "SumE_ee = " << SumE_ee << std::endl;
      std::cout << "SumE_etot = " << SumE_etot << std::endl;
      std::cout << "NhitAboveThr_eb = " << NhitAboveThr_eb << std::endl;
      std::cout << "NhitAboveThr_ee = " << NhitAboveThr_ee << std::endl;
      std::cout << "NhitAboveThr_etot = " << NhitAboveThr_etot << std::endl;
      std::cout << "--------------------------------" << std::endl;
      std::cout << std::endl;

      h_event_bx_splash->Fill(iEvent.bunchCrossing());


      h2_HCAL_vs_ECAL_splash->Fill( SumE_htot, SumE_etot);
      h2_HB_vs_EB_splash->Fill( SumE_hb, SumE_eb);
      h2_HE_vs_EE_splash->Fill( SumE_he, SumE_ee);


      if (hbhe_hits) { // object is available 
	for (HBHERecHitCollection::const_iterator hhit=hbhe_hits->begin(); hhit!=hbhe_hits->end(); hhit++) {	  
	  h_hcal_rechit_energy_hbhe_splash->Fill(hhit->energy());
	  h2_hcal_rechit_energy_eta_phi_noho_splash->Fill( (hhit->id()).ieta(), 
							   (hhit->id()).iphi(), 
							   hhit->energy() ) ;
	  h2_hcal_rechit_time_eta_phi_noho_splash->Fill( (hhit->id()).ieta(), 
							 (hhit->id()).iphi(), 
							 hhit->time() ) ;
	}
      }

      if (ho_hits) { // object is available 
	for (HORecHitCollection::const_iterator hhit=ho_hits->begin(); hhit!=ho_hits->end(); hhit++) {	  
	  h_hcal_rechit_energy_ho_splash->Fill(hhit->energy());
	  h2_hcal_rechit_energy_eta_phi_ho_splash->Fill( (hhit->id()).ieta(), 
							 (hhit->id()).iphi(), 
							 hhit->energy() ) ;
	  h2_hcal_rechit_time_eta_phi_ho_splash->Fill( (hhit->id()).ieta(), 
						       (hhit->id()).iphi(), 
						       hhit->time() ) ;
	}
      }

      if (hf_hits) { // object is available 
	for (HFRecHitCollection::const_iterator hhit=hf_hits->begin(); hhit!=hf_hits->end(); hhit++) {	 
	  h_hcal_rechit_energy_hf_splash->Fill(hhit->energy());
	  h2_hcal_rechit_energy_eta_phi_noho_splash->Fill( (hhit->id()).ieta(), 
							   (hhit->id()).iphi(), 
							   hhit->energy() ) ;
	  h2_hcal_rechit_time_eta_phi_noho_splash->Fill( (hhit->id()).ieta(), 
							 (hhit->id()).iphi(), 
							 hhit->time() ) ;
	}
      }


      if (ebrechit) { // object is available 
	for (EcalRecHitCollection::const_iterator ehit=ebrechit->begin(); ehit!=ebrechit->end(); ehit++) {      
	  h_ecal_rechit_energy_eb_splash->Fill(ehit->energy());
	}
      }

      if (eerechit) { // object is available 
	for (EcalRecHitCollection::const_iterator ehit=eerechit->begin(); ehit!=eerechit->end(); ehit++) {      
	  h_ecal_rechit_energy_ee_splash->Fill(ehit->energy());
	}
      }

    }//end splash event

  else

    if(SumE_htot > 10000 && SumE_htot < 1000000  
       //NhitAboveThr_hf > 100 && NhitAboveThr_hf < 1000 )
       )
      {
	std::cout << "#### After splash event ####" << std::endl; 
	std::cout << "iEvent.id().event() : " << iEvent.id().event() << std::endl;
	std::cout << "iEvent.bunchCrossing() : " << iEvent.bunchCrossing() << std::endl;
	std::cout << "iEvent.orbitNumber() : " << iEvent.orbitNumber() << std::endl;  
	std::cout << "--------------------------------" << std::endl;
	std::cout << "## HCAL ##" << std::endl;
	std::cout << "SumE_hb = " << SumE_hb << std::endl;
	std::cout << "SumE_he = " << SumE_he << std::endl;
	std::cout << "SumE_ho = " << SumE_ho << std::endl;
	std::cout << "SumE_hf = " << SumE_hf << std::endl;
	std::cout << "SumE_htot = " << SumE_htot << std::endl;
	std::cout << "NhitAboveThr_hb = " << NhitAboveThr_hb << std::endl;
	std::cout << "NhitAboveThr_he = " << NhitAboveThr_he << std::endl;
	std::cout << "NhitAboveThr_hf = " << NhitAboveThr_hf << std::endl;
	std::cout << "NhitAboveThr_ho = " << NhitAboveThr_ho << std::endl;
	std::cout << "NhitAboveThr_htot = " << NhitAboveThr_htot << std::endl;
	std::cout << "--------------------------------" << std::endl;
	std::cout << "## ECAL ##" << std::endl;
	std::cout << "SumE_eb = " << SumE_eb << std::endl;
	std::cout << "SumE_ee = " << SumE_ee << std::endl;
	std::cout << "SumE_etot = " << SumE_etot << std::endl;
	std::cout << "NhitAboveThr_eb = " << NhitAboveThr_eb << std::endl;
	std::cout << "NhitAboveThr_ee = " << NhitAboveThr_ee << std::endl;
	std::cout << "NhitAboveThr_etot = " << NhitAboveThr_etot << std::endl;
	std::cout << "--------------------------------" << std::endl;
	std::cout << std::endl;
	
	h_event_bx_aftersplash->Fill(iEvent.bunchCrossing());

	h2_HCAL_vs_ECAL_aftersplash->Fill( SumE_htot, SumE_etot);

	if (hbhe_hits) { // object is available 
	  for (HBHERecHitCollection::const_iterator hhit=hbhe_hits->begin(); hhit!=hbhe_hits->end(); hhit++) {	  
	    h_hcal_rechit_energy_hbhe_aftersplash->Fill(hhit->energy());
	    h2_hcal_rechit_energy_eta_phi_noho_aftersplash->Fill( (hhit->id()).ieta(), 
								  (hhit->id()).iphi(), 
								  hhit->energy() ) ;
	    h2_hcal_rechit_time_eta_phi_noho_aftersplash->Fill( (hhit->id()).ieta(), 
								(hhit->id()).iphi(), 
								hhit->time() ) ;
	  }
	}

	if (ho_hits) { // object is available 
	  for (HORecHitCollection::const_iterator hhit=ho_hits->begin(); hhit!=ho_hits->end(); hhit++) {	  
	    h_hcal_rechit_energy_ho_aftersplash->Fill(hhit->energy());
	    h2_hcal_rechit_energy_eta_phi_ho_aftersplash->Fill( (hhit->id()).ieta(), 
								(hhit->id()).iphi(), 
								hhit->energy() ) ;	 
	    h2_hcal_rechit_time_eta_phi_ho_aftersplash->Fill( (hhit->id()).ieta(), 
							      (hhit->id()).iphi(), 
							      hhit->time() ) ;	 
	  }
	}

	if (hf_hits) { // object is available 
	  for (HFRecHitCollection::const_iterator hhit=hf_hits->begin(); hhit!=hf_hits->end(); hhit++) {	  
	    h_hcal_rechit_energy_hf_aftersplash->Fill(hhit->energy());
	    h2_hcal_rechit_energy_eta_phi_noho_aftersplash->Fill( (hhit->id()).ieta(), 
								  (hhit->id()).iphi(), 
								  hhit->energy() ) ;	
	    h2_hcal_rechit_time_eta_phi_noho_aftersplash->Fill( (hhit->id()).ieta(), 
								(hhit->id()).iphi(), 
								hhit->time() ) ;	
	  }
	}

	if (ebrechit) { // object is available 
	  for (EcalRecHitCollection::const_iterator ehit=ebrechit->begin(); ehit!=ebrechit->end(); ehit++) {      
	    h_ecal_rechit_energy_eb_aftersplash->Fill(ehit->energy());
	  }
	}

	if (eerechit) { // object is available 
	  for (EcalRecHitCollection::const_iterator ehit=eerechit->begin(); ehit!=eerechit->end(); ehit++) {      
	    h_ecal_rechit_energy_ee_aftersplash->Fill(ehit->energy());
	  }
	}

      }

    else

      {//no splash event

	//cout << "no splash event" << std::endl;

	if (hbhe_hits) { // object is available 
	  for (HBHERecHitCollection::const_iterator hhit=hbhe_hits->begin(); hhit!=hbhe_hits->end(); hhit++) {	  
	    h_hcal_rechit_energy_hbhe->Fill(hhit->energy());
	    //cout << (hhit->id()).ieta() << " "<< (hhit->id()).iphi()) << std::endl;	  
	  }
	}
	
	if (ho_hits) { // object is available 
	  for (HORecHitCollection::const_iterator hhit=ho_hits->begin(); hhit!=ho_hits->end(); hhit++) {	  
	    h_hcal_rechit_energy_ho->Fill(hhit->energy());
	    //cout << (hhit->id()).ieta() << " "<< (hhit->id()).iphi()) << std::endl;	  
	  }
	}
	
	if (hf_hits) { // object is available 
	  for (HFRecHitCollection::const_iterator hhit=hf_hits->begin(); hhit!=hf_hits->end(); hhit++) {	 
	    h_hcal_rechit_energy_hf->Fill(hhit->energy());
	    //cout << (hhit->id()).ieta() << " "<< (hhit->id()).iphi()) << std::endl;	 
	  }
	}
	
	if (ebrechit) { // object is available 
	  for (EcalRecHitCollection::const_iterator ehit=ebrechit->begin(); ehit!=ebrechit->end(); ehit++) {      
	    h_ecal_rechit_energy_eb->Fill(ehit->energy());
	  }
	}
	
	if (eerechit) { // object is available 
	  for (EcalRecHitCollection::const_iterator ehit=eerechit->begin(); ehit!=eerechit->end(); ehit++) {      
	    h_ecal_rechit_energy_ee->Fill(ehit->energy());
	  }
	}

      }


  //## TOWERS / JETS / MET .. quantities

  //NOTE: use it only if the MET is reconstructed in the event
  if (met) { // object is available 
    h_caloMet_Met->Fill(met->pt());
    h_caloMet_Phi->Fill(met->phi());
    h_caloMet_SumEt->Fill(met->sumEt());
  }


}


// ------------ method called once each job just before starting event loop  ------------
void HcalPFGBeamSplashTemplate::beginJob(const edm::EventSetup&)
{
  TFileDirectory L1GTDir = fs->mkdir( "L1" );
  TFileDirectory HcalDir = fs->mkdir( "Hcal" );
  TFileDirectory EcalDir = fs->mkdir( "Ecal" );
  //TFileDirectory MuonDir = fs->mkdir( "Muon" );
  TFileDirectory JetMetDir = fs->mkdir( "JetMet" );
  TFileDirectory CorrDir = fs->mkdir( "Correlations" );

  //L1
  h_l1_gtbit = L1GTDir.make<TH1F>("h_l1_gtbit", "L1 Algo Trigger Bit", 128, -0.5, 127.5);
  h_l1_gtbit_techtrig = L1GTDir.make<TH1F>("h_l1_gtbit_techtrig", "L1 Technical Trigger Bit", 64, -0.5, 63.5);

  h_event_bx = L1GTDir.make<TH1F>("h_event_bx", "BX distribution", 3565, -0.5, 3563.5);
  h_event_bx_splash = L1GTDir.make<TH1F>("h_event_bx_splash", "BX distribution - Splash Event", 3565, -0.5, 3563.5);
  h_event_bx_aftersplash = L1GTDir.make<TH1F>("h_event_bx_aftersplash", "BX distribution - After Splash Event", 3565, -0.5, 3563.5);

  //HCAL
  h_hcal_rechit_energy_hbhe = HcalDir.make<TH1F>("h_hcal_rechit_energy_hbhe","RecHit Energy HBHE",10000,0,10000);
  h_hcal_rechit_energy_ho = HcalDir.make<TH1F>("h_hcal_rechit_energy_ho","RecHit Energy HO",10000,0,10000);
  h_hcal_rechit_energy_hf = HcalDir.make<TH1F>("h_hcal_rechit_energy_hf","RecHit Energy HF",10000,0,10000);

  h_hcal_rechit_energy_hbhe_splash = HcalDir.make<TH1F>("h_hcal_rechit_energy_hbhe_splash","RecHit Energy HBHE Splash Events",10000,0,10000);
  h_hcal_rechit_energy_ho_splash = HcalDir.make<TH1F>("h_hcal_rechit_energy_ho_splash","RecHit Energy HO Splash Events",10000,0,10000);
  h_hcal_rechit_energy_hf_splash = HcalDir.make<TH1F>("h_hcal_rechit_energy_hf_splash","RecHit Energy HF Splash Events",10000,0,10000);

  h_hcal_rechit_energy_hbhe_aftersplash = HcalDir.make<TH1F>("h_hcal_rechit_energy_hbhe_aftersplash","RecHit Energy HBHE After Splash Events",10000,0,10000);
  h_hcal_rechit_energy_ho_aftersplash = HcalDir.make<TH1F>("h_hcal_rechit_energy_ho_aftersplash","RecHit Energy HO After Splash Events",10000,0,10000);
  h_hcal_rechit_energy_hf_aftersplash = HcalDir.make<TH1F>("h_hcal_rechit_energy_hf_aftersplash","RecHit Energy HF After Splash Events",10000,0,10000);

  h2_hcal_rechit_energy_eta_phi_noho_splash = HcalDir.make<TH2F>("h2_hcal_rechit_energy_eta_phi_noho_splash",
								 "RecHit Energy Splash Events - i_eta (x) vs i_phi (y) plane - no HO",
								 83,-41.5,+41.5,
								 72,0.5,+72.5);

  h2_hcal_rechit_energy_eta_phi_ho_splash = HcalDir.make<TH2F>("h2_hcal_rechit_energy_eta_phi_ho_splash",
							       "RecHit Energy Splash Events - i_eta (x) vs i_phi (y) plane - HO only",
							       83,-41.5,+41.5,
							       72,0.5,+72.5);

  h2_hcal_rechit_energy_eta_phi_noho_aftersplash = HcalDir.make<TH2F>("h2_hcal_rechit_energy_eta_phi_noho_aftersplash",
								 "RecHit Energy After Splash Events - i_eta (x) vs i_phi (y) plane - no HO",
								 83,-41.5,+41.5,
								 72,0.5,+72.5);

  h2_hcal_rechit_energy_eta_phi_ho_aftersplash = HcalDir.make<TH2F>("h2_hcal_rechit_energy_eta_phi_ho_aftersplash",
							       "RecHit Energy After Splash Events - i_eta (x) vs i_phi (y) plane - HO only",
							       83,-41.5,+41.5,
							       72,0.5,+72.5);

  h2_hcal_rechit_time_eta_phi_noho_splash = HcalDir.make<TH2F>("h2_hcal_rechit_time_eta_phi_noho_splash",
								 "RecHit Time Splash Events - i_eta (x) vs i_phi (y) plane - no HO",
								 83,-41.5,+41.5,
								 72,0.5,+72.5);

  h2_hcal_rechit_time_eta_phi_ho_splash = HcalDir.make<TH2F>("h2_hcal_rechit_time_eta_phi_ho_splash",
							       "RecHit Time Splash Events - i_eta (x) vs i_phi (y) plane - HO only",
							       83,-41.5,+41.5,
							       72,0.5,+72.5);

  h2_hcal_rechit_time_eta_phi_noho_aftersplash = HcalDir.make<TH2F>("h2_hcal_rechit_time_eta_phi_noho_aftersplash",
								 "RecHit Time After Splash Events - i_eta (x) vs i_phi (y) plane - no HO",
								 83,-41.5,+41.5,
								 72,0.5,+72.5);

  h2_hcal_rechit_time_eta_phi_ho_aftersplash = HcalDir.make<TH2F>("h2_hcal_rechit_time_eta_phi_ho_aftersplash",
							       "RecHit Time After Splash Events - i_eta (x) vs i_phi (y) plane - HO only",
							       83,-41.5,+41.5,
							       72,0.5,+72.5);


  h_hcal_SumE_hb = HcalDir.make<TH1F>("h_hcal_SumE_hb","Sum RecHit Energy HB",5000,0,5000000);
  h_hcal_SumE_he = HcalDir.make<TH1F>("h_hcal_SumE_he","Sum RecHit Energy HE",5000,0,5000000);
  h_hcal_SumE_ho = HcalDir.make<TH1F>("h_hcal_SumE_ho","Sum RecHit Energy HO",5000,0,5000000);
  h_hcal_SumE_hf = HcalDir.make<TH1F>("h_hcal_SumE_hf","Sum RecHit Energy HF",5000,0,5000000);
  h_hcal_SumE_tot = HcalDir.make<TH1F>("h_hcal_SumE_tot","Sum RecHit Energy HCAL Tot",5000,0,5000000);

  h_hcal_NhitAboveThr_hb = HcalDir.make<TH1F>("h_hcal_NhitAboveThr_hb","Num RecHit Energy Above Threshold HB",10001,-0.5,10000.5);
  h_hcal_NhitAboveThr_he = HcalDir.make<TH1F>("h_hcal_NhitAboveThr_he","Num RecHit Energy Above Threshold HE",10001,-0.5,10000.5);
  h_hcal_NhitAboveThr_ho = HcalDir.make<TH1F>("h_hcal_NhitAboveThr_ho","Num RecHit Energy Above Threshold HO",10001,-0.5,10000.5);
  h_hcal_NhitAboveThr_hf = HcalDir.make<TH1F>("h_hcal_NhitAboveThr_hf","Num RecHit Energy Above Threshold HF",10001,-0.5,10000.5);
  h_hcal_NhitAboveThr_tot = HcalDir.make<TH1F>("h_hcal_NhitAboveThr_tot","Num RecHit Energy Above Threshold HCAL Tot",10001,-0.5,10000.5);

  //ECAL
  h_ecal_rechit_energy_eb = EcalDir.make<TH1F>(" h_ecal_rechit_energy_eb","RecHit Energy EB",1000,0,1000); 
  h_ecal_rechit_energy_ee = EcalDir.make<TH1F>(" h_ecal_rechit_energy_ee","RecHit Energy EE",1000,0,1000); 

  h_ecal_rechit_energy_eb_splash = EcalDir.make<TH1F>(" h_ecal_rechit_energy_eb_splash","RecHit Energy EB Splash Event",1000,0,1000); 
  h_ecal_rechit_energy_ee_splash = EcalDir.make<TH1F>(" h_ecal_rechit_energy_ee_splash","RecHit Energy EE Splash Event",1000,0,1000); 

  h_ecal_rechit_energy_eb_aftersplash = EcalDir.make<TH1F>(" h_ecal_rechit_energy_eb_aftersplash","RecHit Energy EB After Splash Event",1000,0,1000); 
  h_ecal_rechit_energy_ee_aftersplash = EcalDir.make<TH1F>(" h_ecal_rechit_energy_ee_aftersplash","RecHit Energy EE After Splash Event",1000,0,1000); 

  h_ecal_SumE_eb = EcalDir.make<TH1F>("h_ecal_SumE_eb","Sum RecHit Energy EB",5000,0,1000000);
  h_ecal_SumE_ee = EcalDir.make<TH1F>("h_ecal_SumE_ee","Sum RecHit Energy EE",5000,0,1000000);
  h_ecal_SumE_etot = EcalDir.make<TH1F>("h_ecal_SumE_etot","Sum RecHit Energy ECAL Tot",5000,0,1000000);

  //JETMET
  h_caloMet_Met = JetMetDir.make<TH1F>("h_caloMet_Met", "MET from CaloTowers", 5000, 0, 5000000);
  h_caloMet_Phi = JetMetDir.make<TH1F>("h_caloMet_Phi", "MET #phi from CaloTowers", 100, -6, 6);
  h_caloMet_SumEt = JetMetDir.make<TH1F>("h_caloMet_SumEt", "SumET from CaloTowers", 10000, 0, 5000000);

  //CORRELATION
  h2_HCAL_vs_ECAL_splash = CorrDir.make<TH2F>("h2_HCAL_vs_ECAL_splash"
					      ,"Sum Energy HCAL (x) vs ECAL (y) - Splash Event"
					      ,100,1000000,7000000
					      ,100,50000,500000);
  
  h2_HB_vs_EB_splash = CorrDir.make<TH2F>("h2_HB_vs_EB_splash"
					  ,"Sum Energy HB (x) vs EB (y) - Splash Event"
					  ,200,200000,2000000
					  ,100,50000,500000);

  h2_HE_vs_EE_splash = CorrDir.make<TH2F>("h2_HE_vs_EE_splash"
					  ,"Sum Energy HE (x) vs EE (y) - Splash Event"
					  ,200,200000,2000000
					  ,100,10000,100000);

  h2_HCAL_vs_ECAL_aftersplash = CorrDir.make<TH2F>("h2_HCAL_vs_ECAL_aftersplash"
						   ,"Sum Energy HCAL (x) vs ECAL (y) - After Splash Event"
						   ,100,10000,50000
						   ,100,50,500);
  
}

// ------------ method called once each job just after ending the event loop  ------------
void HcalPFGBeamSplashTemplate::endJob() {

  float N_splash_events =   h_event_bx_splash->GetEntries();
  float N_aftersplash_events =   h_event_bx_aftersplash->GetEntries();

  std::cout << std::endl;
  std::cout << "#####################################" << std::endl;
  std::cout << "--> "<< N_splash_events << " splash events found" << std::endl;
  std::cout << "--> "<< N_aftersplash_events << " after splash events found" <<std::endl;
  std::cout << "#####################################" << std::endl;
  

  float scale_splash = 1/N_splash_events;
  float scale_aftersplash = 1/N_aftersplash_events;

  h2_hcal_rechit_energy_eta_phi_noho_splash->Scale(scale_splash);
  h2_hcal_rechit_energy_eta_phi_ho_splash->Scale(scale_splash);
  h2_hcal_rechit_energy_eta_phi_noho_aftersplash->Scale(scale_aftersplash);
  h2_hcal_rechit_energy_eta_phi_ho_aftersplash->Scale(scale_aftersplash);

}
