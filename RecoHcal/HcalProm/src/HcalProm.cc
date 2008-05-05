// -*- C++ -*-
//
// Package:    HcalProm
// Class:      HcalProm
// 
/**\class HcalProm HcalProm.cc RecoHcal/HcalProm/src/HcalProm.cc

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
// $Id: HcalProm.cc,v 1.6 2008/05/04 23:11:28 fedor Exp $
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
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/Jet.h"

#include "DataFormats/Candidate/interface/Candidate.h"
//Missing Et
#include "DataFormats/METReco/interface/CaloMETCollection.h"

//ecal
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"

//trigger
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuRegionalCand.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTReadoutCollection.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GtPsbWord.h"

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

#include "HcalProm.h"

#include <iostream>
#include <algorithm>
#include <vector>



//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//


HcalProm::HcalProm(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed

}


HcalProm::~HcalProm()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
HcalProm::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;
   using namespace reco;

   //hcal rechits
   Handle<HBHERecHitCollection> hbhe;
   iEvent.getByLabel("hbhereco_", hbhe);
   const HBHERecHitCollection Hithbhe = *(hbhe.product());

   Handle<HFRecHitCollection> hfrh;
   iEvent.getByLabel("hfreco",hfrh);
   const HFRecHitCollection Hithf = *(hfrh.product());
 
   Handle<HORecHitCollection> horh; 
   iEvent.getByLabel( "horeco", horh );
   const HORecHitCollection Hitho = *(horh.product());

   // Missing Et
//   Handle<CaloMETCollection> recmet;  
//   iEvent.getByLabel( "met", recmet ); 

   //hcal digis
   Handle<HBHEDigiCollection> hbhe_digi; 
   //   iEvent.getByLabel("hcalZeroSuppressedDigis",hbhe_digi);
   iEvent.getByLabel("hcalDigis",hbhe_digi);
   
   //calo towers
   Handle<CaloTowerCollection> calo;
   iEvent.getByLabel("towerMaker", calo);
   const CaloTowerCollection calohbhe = *(calo.product());
  
   //ecal rechits
   Handle<EcalRecHitCollection> ebrechit;
   iEvent.getByLabel("ecalRecHit","EcalRecHitsEB",ebrechit);
   const EcalRecHitCollection Hiteb = *(ebrechit.product());

   //ecal clusters
   Handle<reco::BasicClusterCollection> pIslandBarrelBasicClusters;
   iEvent.getByLabel("islandBasicClusters","islandBarrelBasicClusters",pIslandBarrelBasicClusters);
   const BasicClusterCollection islandBarrelBasicClusters = *(pIslandBarrelBasicClusters.product());

   Handle<reco::BasicClusterCollection> bccHandle;
   iEvent.getByLabel("barrelClusterProducer", "barrelClusterCollection", bccHandle);
   const reco::BasicClusterCollection *clusterCollection_p = bccHandle.product();

   // reco jets
   Handle<CaloJetCollection> caloJet;
   iEvent.getByLabel("iterativeCone5CaloJets",caloJet);
   const CaloJetCollection cjet = *(caloJet.product());

   //Cosmic Muons
   Handle<TrackCollection> cosmicmuon;
   iEvent.getByLabel("cosmicMuons",cosmicmuon);

   //trigger
   Handle<L1MuGMTReadoutCollection> gmtrc_handle; 
   iEvent.getByLabel("l1GmtEmulDigis",gmtrc_handle);
   L1MuGMTReadoutCollection const* gmtrc = gmtrc_handle.product();

   Handle<L1GlobalTriggerReadoutRecord> gtrr_handle; 
   iEvent.getByLabel("l1GtUnpack",gtrr_handle);
   L1GlobalTriggerReadoutRecord const* gtrr = gtrr_handle.product();

   string errMsg("");
   try {
    iEvent.getByLabel ("hbhereco", hbhe);
   } catch (const cms::Exception& e){
    errMsg=errMsg + "  -- No  hbhereco\n"+e.what();
   }

   if (errMsg != ""){
    errMsg=errMsg + ".";
    std::cout << "% Warning" << errMsg << std::endl;
   }

   bool dt_l1a = false;
   bool rpc_l1a = false;
   bool hcal_l1a = false;

   vector<L1MuGMTReadoutRecord> gmt_records = gmtrc->getRecords();
   vector<L1MuGMTReadoutRecord>::const_iterator igmtrr;
 
   for(igmtrr=gmt_records.begin(); igmtrr!=gmt_records.end(); igmtrr++) {
     vector<L1MuRegionalCand>::const_iterator iter1;
     vector<L1MuRegionalCand> rmc;
     
     //DT Trigger
     int idt = 0;
     rmc = igmtrr->getDTBXCands();
     for(iter1=rmc.begin(); iter1!=rmc.end(); iter1++) {
       if (!(*iter1).empty()) ++idt;
     }

     if(idt>0) cout << "Found " << idt << " valid DT candidates in bx wrt. L1A = " << igmtrr->getBxInEvent() << endl; 
     if(igmtrr->getBxInEvent()==0 && idt>0) dt_l1a = true;  

     // RPCb Trigger
     int irpcb = 0;
     rmc = igmtrr->getBrlRPCCands(); 
     for(iter1=rmc.begin(); iter1!=rmc.end(); iter1++) {
       if (!(*iter1).empty()) ++irpcb;
     }

     if(irpcb>0) cout << "Found " << irpcb << " valid RPC candidates in bx wrt. L1A = " << igmtrr->getBxInEvent() <<endl;
     if(igmtrr->getBxInEvent()==0 && irpcb>0) rpc_l1a = true;
   }
   
   for(int ibx=-1; ibx<=1; ibx++) {
     bool hcal_top = false;
     bool hcal_bot = false;
     const L1GtPsbWord psb = gtrr->gtPsbWord(0xbb0d,ibx);
     std::vector<int> valid_phi;
     if((psb.aData(4)&0x3f) > 1) {valid_phi.push_back( (psb.aData(4)>>10)&0x1f ); }
     if((psb.bData(4)&0x3f) > 1) {valid_phi.push_back( (psb.bData(4)>>10)&0x1f ); }
     if((psb.aData(5)&0x3f) > 1) {valid_phi.push_back( (psb.aData(5)>>10)&0x1f ); }
     if((psb.bData(5)&0x3f) > 1) {valid_phi.push_back( (psb.bData(5)>>10)&0x1f ); }
     std::vector<int>::const_iterator iphi;
     for(iphi=valid_phi.begin(); iphi!=valid_phi.end(); iphi++) {
       std::cout << "Found HCAL mip with phi=" << *iphi << " in bx wrt. L1A = " << ibx << std::endl;
       if(*iphi<9) hcal_top=true;
       if(*iphi>8) hcal_bot=true;
     }
     if(ibx==0 && hcal_top && hcal_bot) hcal_l1a=true;
   }
   
   std::cout << "**** Trigger Source ****" << std::endl;
   if(dt_l1a) std::cout << "DT" << std::endl;
   if(rpc_l1a) std::cout << "RPC" << std::endl;
   if(hcal_l1a) std::cout << "HCAL" << std::endl;
   std::cout << "************************" << std::endl;
   

   
   int adcs[10] = {};

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



  for (HBHERecHitCollection::const_iterator hhit=Hithbhe.begin(); hhit!=Hithbhe.end(); hhit++) {
    if (hhit->energy() > 0.6){
      h_hcal_rechit_energy->Fill(hhit->energy());
      h_eta_phi_HBHE->Fill((hhit->id()).ieta(),(hhit->id()).iphi());
    }
  }

  for (HFRecHitCollection::const_iterator hhit=Hithf.begin(); hhit!=Hithf.end(); hhit++) {
    if (hhit->energy() > 0.6){
      h_hf_rechit_energy->Fill(hhit->energy());
      h_eta_phi_HF->Fill((hhit->id()).ieta(),(hhit->id()).iphi());
    }
  }

  for (HORecHitCollection::const_iterator hhit=Hitho.begin(); hhit!=Hitho.end(); hhit++) {
    if (hhit->energy() > 0.6){
      h_ho_rechit_energy->Fill(hhit->energy());
      h_eta_phi_HO->Fill((hhit->id()).ieta(),(hhit->id()).iphi());
    }
  }

  for (EcalRecHitCollection::const_iterator ehit=Hiteb.begin(); ehit!=Hiteb.end(); ehit++) {
      h_ecal_rechit_energy->Fill(ehit->energy());
  }

  for(reco::BasicClusterCollection::const_iterator aClus = islandBarrelBasicClusters.begin(); aClus != islandBarrelBasicClusters.end(); aClus++) {
    h_ecal_cluster_energy->Fill(aClus->energy());
    h_ecal_cluster_eta->Fill(aClus->eta());
    h_ecal_cluster_phi->Fill(aClus->phi());
  }

  for (reco::BasicClusterCollection::const_iterator clus = clusterCollection_p->begin(); clus != clusterCollection_p->end(); ++clus)
    {
     double energy_eb_basic_cluster = clus->energy();
     double phi_eb_basic_cluster    = clus->phi();
     double eta_eb_basic_cluster    = clus->eta();
   }

  for(TrackCollection::const_iterator ncm = cosmicmuon->begin(); ncm != cosmicmuon->end();  ++ncm) {
    h_muon_vertex_x->Fill(ncm->vx());
    h_muon_px->Fill(ncm->px());
    h_muon_p->Fill(ncm->p());
  }


  std::vector<double> value;
  for (CaloJetCollection::const_iterator jetiter=cjet.begin(); jetiter!=cjet.end(); jetiter++){
    h_jet_energy->Fill(jetiter->energy());
    h_jet_eta->Fill(jetiter->eta());
    h_jet_phi->Fill(jetiter->phi());
    value.push_back(jetiter->pt());
  }

  std::sort(value.begin(), value.end());
  if(value.size()>0){
  h_jet_highestPt->Fill(value[value.size()-1]);
  }

  for (CaloTowerCollection::const_iterator kal=calohbhe.begin(); kal!=calohbhe.end(); kal++){	
    h_calo_tower_energy->Fill(kal->energy());
  }

  //correlations
  for(TrackCollection::const_iterator ncm = cosmicmuon->begin(); ncm != cosmicmuon->end();  ++ncm) {
    if (fabs(ncm->eta())>1.3) continue;
    for(reco::BasicClusterCollection::const_iterator aClus = islandBarrelBasicClusters.begin(); aClus != islandBarrelBasicClusters.end(); aClus++) {
      h_ecalx_vs_muonx->Fill(aClus->x(),ncm->vx());
      h_impact_diff->Fill(aClus->z()-ncm->vx());
    }
  } 


}


// ------------ method called once each job just before starting event loop  ------------
void HcalProm::beginJob(const edm::EventSetup&)
{
  TFileDirectory EcalDir = fs->mkdir( "Ecal" );
  TFileDirectory HcalDir = fs->mkdir( "Hcal" );
  TFileDirectory MuonDir = fs->mkdir( "Muon" );
  TFileDirectory CorrDir = fs->mkdir( "Correlations" );
  TFileDirectory NoiseDir = fs->mkdir( "Noise" );
  TFileDirectory JetMetDir = fs->mkdir( "JetMet" );

  //Add runnumbers to histograms!

  h_hcal_rechit_energy = HcalDir.make<TH1F>(" h_hcal_rechit_energy","RecHit Energy HBHE",130,-10,120);
  h_caloMet_energy = HcalDir.make<TH1F>(" h_caloMet_energy","CaloMET0 Energy",130,-10,120);
  h_eta_phi_HBHE = HcalDir.make<TH2F>("h_eta_phi_HBHE","#eta(HBHE)",100,-7,7,100,-7,7);
  h_hf_rechit_energy = HcalDir.make<TH1F>(" h_hf_rechit_energy","RecHit Energy HF",130,-10,120);
  h_eta_phi_HF = HcalDir.make<TH2F>("h_eta_phi_HF","#eta(HF)",100,-7,7,100,-7,7);
  h_ho_rechit_energy = HcalDir.make<TH1F>(" h_ho_rechit_energy","RecHit Energy HO",130,-10,120);
  h_eta_phi_HO = HcalDir.make<TH2F>("h_eta_phi_HO","#eta(HO)",100,-7,7,100,-7,7);
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
  h_jet_highestPt = JetMetDir.make<TH1F>("h_jet_highestPt","Highest Jet pT",7000,0,7000);

  h_muon_vertex_x = MuonDir.make<TH1F>("h_muon_vertex_x","Muon Vertex X",10000,-1000,1000);
  h_muon_px = MuonDir.make<TH1F>("h_muon_px","P_{X}(#mu)",1000,-10,100);
  h_muon_p = MuonDir.make<TH1F>("h_muon_p","P(#mu)",1000,-10,100);

  h_ecalx_vs_muonx = CorrDir.make<TH2F>("h_ecalx_vs_muonx","h_ecalx_vs_muonx",10000,-1000,1000,10000,-1000,1000);
  h_impact_diff = CorrDir.make<TH1F>("h_impact_diff","h_impact_diff",1000,-200,200);
  TrigDT = 0;
}

// ------------ method called once each job just after ending the event loop  ------------
void HcalProm::endJob() {
}

