// -*- C++ -*-
//
// Package:    HcalProm
// Class:      HcalProm
// 
/**\class HcalProm HcalProm.cc RecoHcal/HcalProm/src/HcalProm.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Efe Yazgan
//         Created:  Wed Apr 16 10:03:18 CEST 2008
// $Id: HcalProm.cc,v 1.1 2008/04/17 09:36:53 efe Exp $
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


//
// class decleration
//

class HcalProm : public edm::EDAnalyzer {
   public:
      explicit HcalProm(const edm::ParameterSet&);
      ~HcalProm();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      // ----------member data ---------------------------
  edm::Service<TFileService> fs;

  TH1F* h_hcal_rechit_energy;
  TH2F* h_eta_phi_HBHE;	

  TH1F* h_ecal_rechit_energy;


  TH1F* h_hbtiming;

  TH1F* h_ecal_cluster_energy;
  TH1F* h_ecal_cluster_eta;
  TH1F* h_ecal_cluster_phi;
  TH2F* h_ecal_vs_hcal_eta;
  TH2F* h_ecal_vs_hcal_phi; 

  TH1F* h_calo_tower_energy;

  TH1F* h_jet_energy;
  TH1F* h_jet_eta;
  TH1F* h_jet_phi;

  TH1F* h_muon_vertex_x;
  TH1F* h_muon_px;
  TH1F* h_muon_p;
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
   iEvent.getByLabel("hbhereco", hbhe);
   const HBHERecHitCollection Hithbhe = *(hbhe.product());

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

   // reco jets
   Handle<CaloJetCollection> caloJet;
   iEvent.getByLabel("iterativeCone5CaloJets",caloJet);
   const CaloJetCollection cjet = *(caloJet.product());

   //Cosmic Muons
   Handle<TrackCollection> cosmicmuon;
   iEvent.getByLabel("cosmicMuons",cosmicmuon);


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

  for (EcalRecHitCollection::const_iterator ehit=Hiteb.begin(); ehit!=Hiteb.end(); ehit++) {
      h_ecal_rechit_energy->Fill(ehit->energy());
  }

  for(reco::BasicClusterCollection::const_iterator aClus = islandBarrelBasicClusters.begin(); aClus != islandBarrelBasicClusters.end(); aClus++) {
    h_ecal_cluster_energy->Fill(aClus->energy());
    h_ecal_cluster_eta->Fill(aClus->eta());
    h_ecal_cluster_phi->Fill(aClus->phi());
  }


  for(TrackCollection::const_iterator ncm = cosmicmuon->begin(); ncm != cosmicmuon->end();  ++ncm) {
    h_muon_vertex_x->Fill(ncm->vx());
    h_muon_px->Fill(ncm->px());
    h_muon_p->Fill(ncm->p());
  }

  for (CaloJetCollection::const_iterator jetiter=cjet.begin(); jetiter!=cjet.end(); jetiter++){
    h_jet_energy->Fill(jetiter->energy());
    h_jet_eta->Fill(jetiter->eta());
    h_jet_phi->Fill(jetiter->phi());
  }

  for (CaloTowerCollection::const_iterator kal=calohbhe.begin(); kal!=calohbhe.end(); kal++){	
    h_calo_tower_energy->Fill(kal->energy());
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
  h_eta_phi_HBHE = HcalDir.make<TH2F>("h_eta_phi_HBHE","#eta(HBHE)",100,-7,7,100,-7,7);
  h_hbtiming = HcalDir.make<TH1F>("h_hbtiming","HBHE Timing",10,-0.5,9.5);

  h_ecal_rechit_energy = EcalDir.make<TH1F>(" h_ecal_rechit_energy","RecHit Energy EB",130,-10,120);
 
  h_ecal_cluster_energy = EcalDir.make<TH1F>("h_ecal_cluster_energy","EB Cluster Energy",130,-10,120);
  h_ecal_cluster_eta = EcalDir.make<TH1F>("h_ecal_cluster_eta","#eta(EB Cluster)",100,-7,7);
  h_ecal_cluster_phi = EcalDir.make<TH1F>("h_ecal_cluster_phi","#phi(EB Cluster)",100,-7,7);
  
  h_ecal_vs_hcal_eta = CorrDir.make<TH2F>("h_ecal_vs_hcal_eta","#eta(EB) vs #eta(HB)",100,-7,7,100,-7,7);
  h_ecal_vs_hcal_phi = CorrDir.make<TH2F>("h_ecal_vs_hcal_phi","#phi(EB) vs #phi(HB)",100,-7,7,100,-7,7);

  h_calo_tower_energy = JetMetDir.make<TH1F>("h_calo_tower_energy","Calo Tower Energy",130,-10,120);

  h_jet_energy = JetMetDir.make<TH1F>("h_jet_energy","Jet Energy",130,-10,120);
  h_jet_eta = JetMetDir.make<TH1F>("h_jet_eta","#eta(j)",100,-7,7);
  h_jet_phi = JetMetDir.make<TH1F>("h_jet_phi","#phi(j)",100,-7,7);

  h_muon_vertex_x = MuonDir.make<TH1F>("h_muon_vertex_x","Muon Vertex X",10000,-1000,1000);
  h_muon_px = MuonDir.make<TH1F>("h_muon_px","P_{X}(#mu)",1000,-10,100);
  h_muon_p = MuonDir.make<TH1F>("h_muon_p","P(#mu)",1000,-10,100);
}

// ------------ method called once each job just after ending the event loop  ------------
void HcalProm::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(HcalProm);
