// -*- C++ -*-
//
// Package:    HcalTemplate
// Class:      HcalTemplate
// 
/**\class HcalTemplate HcalTemplate.cc RecoHcal/HcalTemplate/src/HcalTemplate.cc

 SIMPLE EXAMPLE CODE demonstrating the access to officially reconstructed global run data.
 The config file is under the test directory.
 The config file should read the officially reconstructed root file in castor or 
 The output from Configuration/GlobalRuns/data/recoT0.cfg (official reco cfg). 

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Efe Yazgan
//         Created:  Wed Apr 16 10:03:18 CEST 2008
// $Id: HcalTemplate.cc,v 1.1 2008/04/30 20:57:31 fedor Exp $
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

#include "HcalTemplate.h"

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
HcalTemplate::HcalTemplate(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
  TrigDT = 0;

}


HcalTemplate::~HcalTemplate()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
HcalTemplate::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;
   using namespace reco;

   //hcal rechits
   Handle<HBHERecHitCollection> hbhe_hits_h;
   iEvent.getByLabel("hbhereco", hbhe_hits_h);
   const HBHERecHitCollection* hbhe_hits = hbhe_hits_h.failedToGet () ? 0 : &*hbhe_hits_h;

   //hcal digis
   Handle<HBHEDigiCollection> hbhe_digi_h; 
   //   iEvent.getByLabel("hcalZeroSuppressedDigis",hbhe_digi_h);
   iEvent.getByLabel("hcalDigis",hbhe_digi_h);
   const HBHEDigiCollection* hbhe_digi = hbhe_digi_h.failedToGet () ? 0 : &*hbhe_digi_h;
  
   //calo towers
   Handle<CaloTowerCollection> calo_h;
   iEvent.getByLabel("towerMaker", calo_h);
   const CaloTowerCollection* towers = calo_h.failedToGet () ? 0 :&*calo_h;
  
   //ecal rechits
   Handle<EcalRecHitCollection> ebrechit_h;
   iEvent.getByLabel("ecalRecHit","EcalRecHitsEB",ebrechit_h);
   const EcalRecHitCollection*  ebrechit = ebrechit_h.failedToGet () ? 0 : &*ebrechit_h;

   //ecal clusters
   Handle<reco::BasicClusterCollection> pIslandBarrelBasicClusters_h;
   iEvent.getByLabel("islandBasicClusters","islandBarrelBasicClusters",pIslandBarrelBasicClusters_h);
   const BasicClusterCollection* islandBarrelBasicClusters = pIslandBarrelBasicClusters_h.failedToGet () ? 0 : &*pIslandBarrelBasicClusters_h;

   // reco jets
   Handle<CaloJetCollection> caloJet_h;
   iEvent.getByLabel("iterativeCone5CaloJets",caloJet_h);
   const CaloJetCollection* cjet = caloJet_h.failedToGet () ? 0 : &*caloJet_h;

   //Cosmic Muons
   Handle<TrackCollection> cosmicmuon_h;
   iEvent.getByLabel("cosmicMuons",cosmicmuon_h);
   const TrackCollection* cosmicmuon = cosmicmuon_h.failedToGet () ? 0 : &*cosmicmuon_h;

   //trigger
   Handle<L1MuGMTReadoutCollection> gmtrc_h; 
   iEvent.getByLabel("l1GmtEmulDigis",gmtrc_h);
   const L1MuGMTReadoutCollection* gmtrc = gmtrc_h.failedToGet () ? 0 : &*gmtrc_h;

   // L1 global trigger
  Handle <L1GlobalTriggerReadoutRecord> gtRecord_h;
  iEvent.getByType (gtRecord_h); // assume only one L1 trigger record here
  const L1GlobalTriggerReadoutRecord* gtRecord = gtRecord_h.failedToGet () ? 0 : &*gtRecord_h;

  // MET
  Handle <CaloMETCollection> recmet_h;
  iEvent.getByLabel("met", recmet_h);
  const CaloMET* met  = recmet_h.failedToGet () ? 0 : &(recmet_h->front());
  
   if (gmtrc) { // object is available   
     int idt   = 0;
     int ndt[3] = {0,0,0};
     int irpcb = 0;
     int nrpcb[3] = {0,0,0};
     int N;
     vector<L1MuGMTReadoutRecord> gmt_records = gmtrc->getRecords();
     vector<L1MuGMTReadoutRecord>::const_iterator igmtrr;
     
     int MAXDTBX = 20;
     int MAXRPCBX = 20;
     
     for(igmtrr=gmt_records.begin(); igmtrr!=gmt_records.end(); igmtrr++) {
       vector<L1MuRegionalCand>::const_iterator iter1;
       vector<L1MuRegionalCand> rmc;;
       // DTBX Trigger
       rmc = igmtrr->getDTBXCands(); N=0;
       for(iter1=rmc.begin(); iter1!=rmc.end(); iter1++) {
	 if ( idt < MAXDTBX && !(*iter1).empty() ) {
	   idt++; 
	   if(N<4) ndt[N]++; 
	 } 
	 N++;
     }
       // RPCb Trigger
       rmc = igmtrr->getBrlRPCCands(); N=0;
       for(iter1=rmc.begin(); iter1!=rmc.end(); iter1++) {
	 if ( irpcb < MAXRPCBX && !(*iter1).empty() ) {
	   irpcb++;
	   if(N<4) nrpcb[N]++;
	 }  
	 N++;
       }
     }
     
     if(ndt[0]>0 && nrpcb[0] == 0) TrigDT++;
   }

   if (hbhe_digi) { // object is available 
     int adcs[10];
     //CORRECT:  Timing plot should be done using linearized ADC's!
     for (HBHEDigiCollection::const_iterator j=hbhe_digi->begin(); j!=hbhe_digi->end(); j++){
       const HBHEDataFrame digi = (const HBHEDataFrame)(*j);
       HcalDetId id = digi.id();
       if (id.subdet() != 1) continue;
       int maxadc = 0;
       for (int TS = 0; TS < 10 && TS < digi.size(); ++TS){     
	 adcs[TS] = digi[TS].adc();
	 if (digi[TS].adc() > maxadc) maxadc = digi[TS].adc();
       }
       for (int TS = 0; TS < 10 && TS < digi.size(); ++TS){     
	 if (maxadc > 10)	 h_hbtiming->Fill(TS,adcs[TS]);
       }
     }
   }

   if (hbhe_hits) { // object is available 
     for (HBHERecHitCollection::const_iterator hhit=hbhe_hits->begin(); hhit!=hbhe_hits->end(); hhit++) {
       if (hhit->energy() > 0.6){
	 h_hcal_rechit_energy->Fill(hhit->energy());
	 h_eta_phi_HBHE->Fill((hhit->id()).ieta(),(hhit->id()).iphi());
       }
     }
   }

   if (ebrechit) { // object is available 
     for (EcalRecHitCollection::const_iterator ehit=ebrechit->begin(); ehit!=ebrechit->end(); ehit++) {
       h_ecal_rechit_energy->Fill(ehit->energy());
     }
   }

   if (islandBarrelBasicClusters) { // object is available 
     for(reco::BasicClusterCollection::const_iterator aClus = islandBarrelBasicClusters->begin(); aClus != islandBarrelBasicClusters->end(); aClus++) {
       h_ecal_cluster_energy->Fill(aClus->energy());
       h_ecal_cluster_eta->Fill(aClus->eta());
       h_ecal_cluster_phi->Fill(aClus->phi());
     }
   }


   if (cosmicmuon) { // object is available 
     for(TrackCollection::const_iterator ncm = cosmicmuon->begin(); ncm != cosmicmuon->end();  ++ncm) {
       h_muon_vertex_x->Fill(ncm->vx());
       h_muon_px->Fill(ncm->px());
       h_muon_p->Fill(ncm->p());
     }
   }

   if (cjet) { // object is available 
     for (CaloJetCollection::const_iterator jetiter=cjet->begin(); jetiter!=cjet->end(); jetiter++){
       h_jet_energy->Fill(jetiter->energy());
       h_jet_eta->Fill(jetiter->eta());
       h_jet_phi->Fill(jetiter->phi());
     }
   }

   if (towers) { // object is available 
     for (CaloTowerCollection::const_iterator kal=towers->begin(); kal!=towers->end(); kal++){	
       h_calo_tower_energy->Fill(kal->energy());
     }
   }

  //correlations
   if (cosmicmuon && islandBarrelBasicClusters) { // objects are available
     for(TrackCollection::const_iterator ncm = cosmicmuon->begin(); ncm != cosmicmuon->end();  ++ncm) {
       if (fabs(ncm->eta())>1.3) continue;
       for(reco::BasicClusterCollection::const_iterator aClus = islandBarrelBasicClusters->begin(); aClus != islandBarrelBasicClusters->end(); aClus++) {
	 h_ecalx_vs_muonx->Fill(aClus->x(),ncm->vx());
	 h_impact_diff->Fill(aClus->z()-ncm->vx());
       }
     } 
   }

   if (met) { // object is available 
     h_caloMet_Met->Fill(met->pt());
     h_caloMet_Phi->Fill(met->phi());
     h_caloMet_SumEt->Fill(met->sumEt());
   }

}


// ------------ method called once each job just before starting event loop  ------------
void HcalTemplate::beginJob(const edm::EventSetup&)
{
  TFileDirectory EcalDir = fs->mkdir( "Ecal" );
  TFileDirectory HcalDir = fs->mkdir( "Hcal" );
  TFileDirectory MuonDir = fs->mkdir( "Muon" );
  TFileDirectory CorrDir = fs->mkdir( "Correlations" );
  TFileDirectory NoiseDir = fs->mkdir( "Noise" );
  TFileDirectory JetMetDir = fs->mkdir( "JetMet" );

  //Add runnumbers to histograms!

  h_hcal_rechit_energy = HcalDir.make<TH1F>(" h_hcal_rechit_energy","RecHit Energy HBHE",130,-10,120);
  h_eta_phi_HBHE = HcalDir.make<TH2F>("h_eta_phi_HBHE","#eta(HBHE)",100,-7,7,100,-7,7);
  h_hbtiming = HcalDir.make<TH1F>("h_hbtiming","HBHE Timing",10,-0.5,9.5);

  h_ecal_rechit_energy = EcalDir.make<TH1F>(" h_ecal_rechit_energy","RecHit Energy EB",130,-10,120);
 
  h_ecal_cluster_energy = EcalDir.make<TH1F>("h_ecal_cluster_energy","EB Cluster Energy",130,-10,120);
  h_ecal_cluster_eta = EcalDir.make<TH1F>("h_ecal_cluster_eta","#eta(EB Cluster)",100,-7,7);
  h_ecal_cluster_phi = EcalDir.make<TH1F>("h_ecal_cluster_phi","#phi(EB Cluster)",100,-7,7);
  
  h_ecal_vs_hcal_X = CorrDir.make<TH2F>("h_ecal_vs_hcal_X","X(EB) vs X(HB)",100,-7,7,100,-7,7);
  h_ecal_vs_hcal_Y = CorrDir.make<TH2F>("h_ecal_vs_hcal_Y","Y(EB) vs Y(HB)",100,-7,7,100,-7,7);

  h_calo_tower_energy = JetMetDir.make<TH1F>("h_calo_tower_energy","Calo Tower Energy",130,-10,120);

  h_jet_energy = JetMetDir.make<TH1F>("h_jet_energy","Jet Energy",130,-10,120);
  h_jet_eta = JetMetDir.make<TH1F>("h_jet_eta","#eta(j)",100,-7,7);
  h_jet_phi = JetMetDir.make<TH1F>("h_jet_phi","#phi(j)",100,-7,7);

  h_muon_vertex_x = MuonDir.make<TH1F>("h_muon_vertex_x","Muon Vertex X",10000,-1000,1000);
  h_muon_px = MuonDir.make<TH1F>("h_muon_px","P_{X}(#mu)",1000,-10,100);
  h_muon_p = MuonDir.make<TH1F>("h_muon_p","P(#mu)",1000,-10,100);

  h_ecalx_vs_muonx = CorrDir.make<TH2F>("h_ecalx_vs_muonx","h_ecalx_vs_muonx",10000,-1000,1000,10000,-1000,1000);
  h_impact_diff = CorrDir.make<TH1F>("h_impact_diff","h_impact_diff",1000,-200,200);

  h_caloMet_Met = JetMetDir.make<TH1F>("h_caloMet_Met", "MET from CaloTowers", 100, 0, 25);
  h_caloMet_Phi = JetMetDir.make<TH1F>("h_caloMet_Phi", "MET #phi from CaloTowers", 100, -4, 4);
  h_caloMet_SumEt = JetMetDir.make<TH1F>("h_caloMet_SumEt", "SumET from CaloTowers", 100, 0, 40);


}

// ------------ method called once each job just after ending the event loop  ------------
void HcalTemplate::endJob() {
}

