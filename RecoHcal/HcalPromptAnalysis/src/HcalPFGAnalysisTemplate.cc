// -*- C++ -*-
//
// Package:    HcalPFGAnalysisTemplate
// Class:      HcalPFGAnalysisTemplate
// 
// \class HcalPFGAnalysisTemplate HcalPFGAnalysisTemplate.cc /RecoHcal/HcalPromptAnalysis/src/HcalPFGAnalysisTemplate.cc
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

#include "RecoHcal/HcalPromptAnalysis/interface/HcalPFGAnalysisTemplate.h"

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
HcalPFGAnalysisTemplate::HcalPFGAnalysisTemplate(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed
  TrigDT = 0;

}


HcalPFGAnalysisTemplate::~HcalPFGAnalysisTemplate()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void HcalPFGAnalysisTemplate::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
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
  //also:
  //HORecHitCollection
  //HFRecHitCollection
  //HcalCalibRecHitCollection

  //ecal rechits
  Handle<EcalRecHitCollection> ebrechit_h;
  iEvent.getByLabel("ecalRecHit","EcalRecHitsEB",ebrechit_h);
  const EcalRecHitCollection*  ebrechit = ebrechit_h.failedToGet () ? 0 : &*ebrechit_h;


  //####### towers, clusters and jets #######

  //calo towers
  Handle<CaloTowerCollection> calo_h;
  iEvent.getByLabel("towerMaker", calo_h);
  const CaloTowerCollection* towers = calo_h.failedToGet () ? 0 :&*calo_h;

  //ecal clusters
  //    Handle<reco::BasicClusterCollection> pIslandBarrelBasicClusters_h;
  //    iEvent.getByLabel("islandBasicClusters","islandBarrelBasicClusters",pIslandBarrelBasicClusters_h);
  //    const BasicClusterCollection* islandBarrelBasicClusters = pIslandBarrelBasicClusters_h.failedToGet () ? 0 : &*pIslandBarrelBasicClusters_h;

  // reco jets
  Handle<CaloJetCollection> caloJet_h;
  iEvent.getByLabel("iterativeCone5CaloJets",caloJet_h);
  const CaloJetCollection* cjet = caloJet_h.failedToGet () ? 0 : &*caloJet_h;

  //######## muons #######

  //Cosmic Muons
  Handle<TrackCollection> cosmicmuon_h;
  iEvent.getByLabel("cosmicMuons",cosmicmuon_h);
  const TrackCollection* cosmicmuon = cosmicmuon_h.failedToGet () ? 0 : &*cosmicmuon_h;


  //######## trigger info ########

  //trigger
  Handle<L1MuGMTReadoutCollection> gmtrc_h; 
  iEvent.getByLabel("l1GmtEmulDigis",gmtrc_h);
  const L1MuGMTReadoutCollection* gmtrc = gmtrc_h.failedToGet () ? 0 : &*gmtrc_h;

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

  
  //## L1 TRIGGER quantities
   
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
  
  if (hbhe_hits) { // object is available 
    for (HBHERecHitCollection::const_iterator hhit=hbhe_hits->begin(); hhit!=hbhe_hits->end(); hhit++) {
      if (hhit->energy() > 0.6){
	h_hcal_rechit_energy->Fill(hhit->energy());
	//cout << (hhit->id()).ieta() << " "<< (hhit->id()).iphi()) << endl;
      }
    }
  }


  //## ECAL quantities

  if (ebrechit) { // object is available 
    for (EcalRecHitCollection::const_iterator ehit=ebrechit->begin(); ehit!=ebrechit->end(); ehit++) {
      h_ecal_rechit_energy->Fill(ehit->energy());
    }
  }

  //check if the collection still exists???
  //    if (islandBarrelBasicClusters) { // object is available 
  //      for(reco::BasicClusterCollection::const_iterator aClus = islandBarrelBasicClusters->begin(); aClus != islandBarrelBasicClusters->end(); aClus++) {
  //        h_ecal_cluster_energy->Fill(aClus->energy());
  //        h_ecal_cluster_eta->Fill(aClus->eta());
  //        h_ecal_cluster_phi->Fill(aClus->phi());
  //      }
  //    }


  //## MUON quantities

  if (cosmicmuon) { // object is available 
    for(TrackCollection::const_iterator ncm = cosmicmuon->begin(); ncm != cosmicmuon->end();  ++ncm) {
      h_muon_vertex_x->Fill(ncm->vx());
      h_muon_px->Fill(ncm->px());
      h_muon_p->Fill(ncm->p());
    }
  }


  //## TOWERS / JETS / MET .. quantities

  if (towers) { // object is available 
    for (CaloTowerCollection::const_iterator kal=towers->begin(); kal!=towers->end(); kal++){	
      h_calo_tower_energy->Fill(kal->energy());
    }
  }

  if (cjet) { // object is available 
    for (CaloJetCollection::const_iterator jetiter=cjet->begin(); jetiter!=cjet->end(); jetiter++){
      h_jet_energy->Fill(jetiter->energy());
      h_jet_eta->Fill(jetiter->eta());
      h_jet_phi->Fill(jetiter->phi());
    }
  }

  //NOTE: use it only if the MET is reconstructed in the event
  if (met) { // object is available 
    h_caloMet_Met->Fill(met->pt());
    h_caloMet_Phi->Fill(met->phi());
    h_caloMet_SumEt->Fill(met->sumEt());
  }


  //CORRELATION quantities
  //?? check if cluster collection still exists
  //    if (cosmicmuon && islandBarrelBasicClusters) { // objects are available
  //      for(TrackCollection::const_iterator ncm = cosmicmuon->begin(); ncm != cosmicmuon->end();  ++ncm) {
  //        if (fabs(ncm->eta())>1.3) continue;
  //        for(reco::BasicClusterCollection::const_iterator aClus = islandBarrelBasicClusters->begin(); aClus != islandBarrelBasicClusters->end(); aClus++) {
  // 	 h_ecalx_vs_muonx->Fill(aClus->x(),ncm->vx());
  // 	 h_impact_diff->Fill(aClus->z()-ncm->vx());
  //        }
  //      } 
  //    }


}


// ------------ method called once each job just before starting event loop  ------------
void HcalPFGAnalysisTemplate::beginJob(const edm::EventSetup&)
{
  TFileDirectory L1GTDir = fs->mkdir( "L1" );
  TFileDirectory HcalDir = fs->mkdir( "Hcal" );
  TFileDirectory EcalDir = fs->mkdir( "Ecal" );
  TFileDirectory MuonDir = fs->mkdir( "Muon" );
  TFileDirectory JetMetDir = fs->mkdir( "JetMet" );
  TFileDirectory CorrDir = fs->mkdir( "Correlations" );

  //L1
  h_l1_gtbit = L1GTDir.make<TH1F>("h_l1_gtbit", "L1 Algo Trigger Bit", 128, -0.5, 127.5);
  h_l1_gtbit_techtrig = L1GTDir.make<TH1F>("h_l1_gtbit_techtrig", "L1 Technical Trigger Bit", 64, -0.5, 63.5);

  //HCAL
  h_hcal_rechit_energy = HcalDir.make<TH1F>(" h_hcal_rechit_energy","RecHit Energy HBHE",130,-10,120);
  //h_hbtiming = HcalDir.make<TH1F>("h_hbtiming","HBHE Timing",10,-0.5,9.5);

  //ECAL
  h_ecal_rechit_energy = EcalDir.make<TH1F>(" h_ecal_rechit_energy","RecHit Energy EB",130,-10,120); 
  
  //   h_ecal_cluster_energy = EcalDir.make<TH1F>("h_ecal_cluster_energy","EB Cluster Energy",130,-10,120);
  //   h_ecal_cluster_eta = EcalDir.make<TH1F>("h_ecal_cluster_eta","#eta(EB Cluster)",100,-7,7);
  //   h_ecal_cluster_phi = EcalDir.make<TH1F>("h_ecal_cluster_phi","#phi(EB Cluster)",100,-7,7);

  //MUONS
  h_muon_vertex_x = MuonDir.make<TH1F>("h_muon_vertex_x","Muon Vertex X",10000,-1000,1000);
  h_muon_px = MuonDir.make<TH1F>("h_muon_px","P_{X}(#mu)",1000,-10,100);
  h_muon_p = MuonDir.make<TH1F>("h_muon_p","P(#mu)",1000,-10,100);
  
  //JETMET
  h_calo_tower_energy = JetMetDir.make<TH1F>("h_calo_tower_energy","Calo Tower Energy",130,-10,120);

  h_jet_energy = JetMetDir.make<TH1F>("h_jet_energy","Jet Energy",130,-10,120);
  h_jet_eta = JetMetDir.make<TH1F>("h_jet_eta","#eta(j)",100,-7,7);
  h_jet_phi = JetMetDir.make<TH1F>("h_jet_phi","#phi(j)",100,-7,7);

  h_caloMet_Met = JetMetDir.make<TH1F>("h_caloMet_Met", "MET from CaloTowers", 100, 0, 25);
  h_caloMet_Phi = JetMetDir.make<TH1F>("h_caloMet_Phi", "MET #phi from CaloTowers", 100, -4, 4);
  h_caloMet_SumEt = JetMetDir.make<TH1F>("h_caloMet_SumEt", "SumET from CaloTowers", 100, 0, 40);

  //CORRELATION
  //h_ecal_vs_hcal_X = CorrDir.make<TH2F>("h_ecal_vs_hcal_X","X(EB) vs X(HB)",100,-7,7,100,-7,7);
  //h_ecal_vs_hcal_Y = CorrDir.make<TH2F>("h_ecal_vs_hcal_Y","Y(EB) vs Y(HB)",100,-7,7,100,-7,7);
  //h_ecalx_vs_muonx = CorrDir.make<TH2F>("h_ecalx_vs_muonx","h_ecalx_vs_muonx",100,0,50,100,0,50);
  //h_impact_diff = CorrDir.make<TH1F>("h_impact_diff","h_impact_diff",1000,-200,200);

}

// ------------ method called once each job just after ending the event loop  ------------
void HcalPFGAnalysisTemplate::endJob() {
}
