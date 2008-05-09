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
// Updated        :  Taylan Yetkin (2008/05/08)
//         Created:  Wed Apr 16 10:03:18 CEST 2008
// $Id: HcalProm.cc,v 1.14 2008/05/09 08:23:43 efe Exp $
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
#include "DataFormats/METReco/interface/CaloMET.h"
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
#include <TStyle.h>
#include "TMath.h"

#include "HcalProm.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <sys/time.h>



class CaloJetSort {
  public:
    bool operator() (const CaloJet & a, const CaloJet & b) {
          return a.pt() > b.pt();
    }
};

using namespace edm;
using namespace std;
using namespace reco;

HcalProm::HcalProm(const edm::ParameterSet& iConfig)

{
    prompt_htmlPrint = iConfig.getUntrackedParameter < bool > ("printPromptHTML", false);
    doDigiHTML = iConfig.getUntrackedParameter < bool > ("printDigiHTML", false);
    doCaloTowerHTML = iConfig.getUntrackedParameter < bool > ("printCaloTowerHTML", false);
    doRecHitHTML = iConfig.getUntrackedParameter < bool > ("printRecHitHTML", false);
    doJetMetHTML = iConfig.getUntrackedParameter < bool > ("printJetMetHTML", false);
    doMuonHTML = iConfig.getUntrackedParameter < bool > ("printMuonHTML", false);
    doHPDNoiseHTML = iConfig.getUntrackedParameter < bool > ("printHPDNoiseHTML", false);
    // base Html output directory
    baseHtmlDir_ = iConfig.getUntrackedParameter < string > ("baseHtmlDir", "");
    if (baseHtmlDir_.size() != 0)
        cout << "-->HTML output will go to baseHtmlDir = '" << baseHtmlDir_ << "'" << endl;
    else
        cout << "-->HTML output is disabled" << endl;
    runNo = 0;
    evtNo = 0;
    startTime = "Not Avaliable";
    // global ROOT style
    gStyle->Reset("Default");
    gStyle->SetOptFit(1);
    gStyle->SetCanvasColor(0);
    gStyle->SetPadColor(0);
    gStyle->SetFillColor(0);
    gStyle->SetTitleFillColor(10);
    // gStyle->SetOptStat(0);
    gStyle->SetOptStat("ouemr");
    gStyle->SetPalette(1);
}


HcalProm::~HcalProm()
{}



// ------------ method called to for each event  ------------
void HcalProm::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    runNo = iEvent.id().run();
    // old versions time_t a = (iEvent.time().value()) & 0xffffffff;
    time_t a = (iEvent.time().value()) >> 32;

    if (evtNo < 1) {
        startTime = ctime(&a);
    }

    //hcal rechits
    Handle<HBHERecHitCollection> hbhe;
    iEvent.getByLabel("hbhereco", hbhe);
    const HBHERecHitCollection Hithbhe = *(hbhe.product());

   Handle<HFRecHitCollection> hfrh;
   iEvent.getByLabel("hfreco",hfrh);
   const HFRecHitCollection Hithf = *(hfrh.product());
 
   Handle<HORecHitCollection> horh; 
   iEvent.getByLabel( "horeco", horh );
   const HORecHitCollection Hitho = *(horh.product());

    // Missing Et
    const CaloMET *calomet;
    Handle < CaloMETCollection > recmet;
    iEvent.getByLabel("met", recmet);
    const CaloMETCollection *metCol = recmet.product();
    calomet = &(metCol->front());

   //hcal digis
   //   Handle<HBHEDigiCollection> hbhe_digi; 
   //   iEvent.getByLabel("hcalZeroSuppressedDigis",hbhe_digi);
   //iEvent.getByLabel("hcalDigis",hbhe_digi);
   
   //calo towers
   Handle<CaloTowerCollection> calo;
   iEvent.getByLabel("towerMaker", calo);
   const CaloTowerCollection calohbhe = *(calo.product());
  
   //ecal rechits
   Handle<EcalRecHitCollection> ebrechit;
   iEvent.getByLabel("ecalRecHit","EcalRecHitsEB",ebrechit);
   const EcalRecHitCollection Hiteb = *(ebrechit.product());

   //ecal clusters
   /*   Handle<reco::BasicClusterCollection> pIslandBarrelBasicClusters;
   iEvent.getByLabel("islandBasicClusters","islandBarrelBasicClusters",pIslandBarrelBasicClusters);
   const BasicClusterCollection islandBarrelBasicClusters = *(pIslandBarrelBasicClusters.product());
   */
   Handle<reco::BasicClusterCollection> bccHandle;
   iEvent.getByLabel("cosmicBasicClusters","CosmicBarrelBasicClusters", bccHandle);
   const reco::BasicClusterCollection *clusterCollection_p = bccHandle.product();
   

   

   // reco jets
   Handle<CaloJetCollection> caloJet;
   iEvent.getByLabel("iterativeCone15CaloJets",caloJet);
   const CaloJetCollection cjet = *(caloJet.product());

   //Cosmic Muons
   Handle<TrackCollection> cosmicmuon;
   iEvent.getByLabel("cosmicMuons",cosmicmuon);

   //trigger
   Handle<L1MuGMTReadoutCollection> gmtrc_handle; 
   iEvent.getByLabel("gtDigis",gmtrc_handle);
   L1MuGMTReadoutCollection const* gmtrc = gmtrc_handle.product();

   Handle<L1GlobalTriggerReadoutRecord> gtrr_handle; 
   iEvent.getByLabel("gtDigis",gtrr_handle);
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
   

   /* DIGIS ARE TAKEN OUT FROM T0 RECONSTRUCTION
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
   */

   float maxhbherec = 0;
   float next_to_maxhbherec = 0;
   float maxhbherec_ETA = 0;
   float maxhbherec_PHI = 0;
   float next_to_maxhbherec_ETA = 0;
   float next_to_maxhbherec_PHI = 0;   

   float maxhbMinusrec = 0;
   float next_to_maxhbMinusrec = 0;
   float maxhbMinusrec_ETA = 0;
   float maxhbMinusrec_PHI = 0;
   float next_to_maxhbMinusrec_ETA = 0;
   float next_to_maxhbMinusrec_PHI = 0;

   float maxhbPlusrec = 0;
   float next_to_maxhbPlusrec = 0;
   float maxhbPlusrec_ETA = 0;
   float maxhbPlusrec_PHI = 0;
   float next_to_maxhbPlusrec_ETA = 0;
   float next_to_maxhbPlusrec_PHI = 0;
   for (HBHERecHitCollection::const_iterator hhit=Hithbhe.begin(); hhit!=Hithbhe.end(); hhit++) {
     if (hhit->energy() > 0.6){
       h_hbhe_rechit_energy->Fill(hhit->energy());
       h_hbhe_eta_phi->Fill((hhit->id()).ieta(),(hhit->id()).iphi());
       if (hhit->energy() > maxhbherec){ 
	 maxhbherec = hhit->energy();
	 maxhbherec_ETA = hhit->id().ieta();
         maxhbherec_PHI = hhit->id().iphi();
       }
       if (hhit->energy() > next_to_maxhbherec && hhit->energy()< maxhbherec){ 
	 next_to_maxhbherec = hhit->energy();
	 next_to_maxhbherec_ETA = hhit->id().ieta();
         next_to_maxhbherec_PHI = hhit->id().iphi();
       }
       //HB ONLY
       if (hhit->id() > 16) continue;
       //HB-
       if (hhit->energy() > maxhbMinusrec){ 
	 maxhbMinusrec = hhit->energy();
	 maxhbMinusrec_ETA = hhit->id().ieta();
         maxhbMinusrec_PHI = hhit->id().iphi();
       }
       if (hhit->energy() > next_to_maxhbMinusrec && hhit->energy()< maxhbMinusrec){ 
	 next_to_maxhbMinusrec = hhit->energy();
	 next_to_maxhbMinusrec_ETA = hhit->id().ieta();
         next_to_maxhbMinusrec_PHI = hhit->id().iphi();
       }       
       //HB+
       if (hhit->energy() > maxhbPlusrec){ 
	 maxhbPlusrec = hhit->energy();
	 maxhbPlusrec_ETA = hhit->id().ieta();
         maxhbPlusrec_PHI = hhit->id().iphi();
       }
       if (hhit->energy() > next_to_maxhbPlusrec && hhit->energy()< maxhbPlusrec){ 
	 next_to_maxhbPlusrec = hhit->energy();
	 next_to_maxhbPlusrec_ETA = hhit->id().ieta();
         next_to_maxhbPlusrec_PHI = hhit->id().iphi();
       }       
     }
   }


 
   for (HFRecHitCollection::const_iterator hhit=Hithf.begin(); hhit!=Hithf.end(); hhit++) {
     if (hhit->energy() > 0.6){
       h_hf_rechit_energy->Fill(hhit->energy());
       h_hf_eta_phi->Fill((hhit->id()).ieta(),(hhit->id()).iphi());
     }
   }

   float maxhorec = 0;
   float next_to_maxhorec = 0;
   float maxhorec_ETA = 0;
   float maxhorec_PHI = 0;
   float next_to_maxhorec_ETA = 0;
   float next_to_maxhorec_PHI = 0;     
   for (HORecHitCollection::const_iterator hhit=Hitho.begin(); hhit!=Hitho.end(); hhit++) {
     if (hhit->energy() > 0.6){
       h_ho_rechit_energy->Fill(hhit->energy());
       h_ho_eta_phi->Fill((hhit->id()).ieta(),(hhit->id()).iphi());
       if (hhit->energy() > maxhorec){ 
	 maxhorec = hhit->energy();
	 maxhorec_ETA = hhit->id().ieta();
         maxhorec_PHI = hhit->id().iphi();
       }
       if (hhit->energy() > next_to_maxhorec && hhit->energy()< maxhorec){ 
	 next_to_maxhorec = hhit->energy();
	 next_to_maxhorec_ETA = hhit->id().ieta();
         next_to_maxhorec_PHI = hhit->id().iphi();
       }
     }
   }
   
  for (EcalRecHitCollection::const_iterator ehit=Hiteb.begin(); ehit!=Hiteb.end(); ehit++) {
      h_eb_rechit_energy->Fill(ehit->energy());
  }

  float maxebeerec = 0;
  float next_to_maxebeerec = 0;
  float maxebeerec_ETA = 0;
  float maxebeerec_PHI = 0;
  float next_to_maxebeerec_ETA = 0;
  float next_to_maxebeerec_PHI = 0; 
  for (reco::BasicClusterCollection::const_iterator clus = clusterCollection_p->begin(); clus != clusterCollection_p->end(); ++clus)
    {
      if (clus->eta()>2.9) continue;
      if (clus->energy()<0.2) continue;
      if (clus->energy() > maxebeerec){ 
	maxebeerec = clus->energy();
	maxebeerec_ETA = clus->eta();
	maxebeerec_PHI = clus->phi();
      }
      if (clus->energy() > next_to_maxebeerec && clus->energy()< maxebeerec){
	next_to_maxebeerec = clus->energy();
	next_to_maxebeerec_ETA = clus->eta();
	next_to_maxebeerec_PHI = clus->eta();
      }
   }

  float total_hbhe = maxhbherec+next_to_maxhbherec;
  float total_ebee = maxebeerec+next_to_maxebeerec;
  float total_ho = maxhorec+next_to_maxhorec;
  float total_hbMinus = maxhbMinusrec+next_to_maxhbMinusrec;
  float total_hbPlus = maxhbPlusrec+next_to_maxhbPlusrec;
  float hbhe_DR = sqrt(pow((maxhbherec_ETA-next_to_maxhbherec_ETA),2)+pow((maxhbherec_ETA-next_to_maxhbherec_ETA),2));
  float ebee_DR = sqrt(pow((maxebeerec_ETA-next_to_maxebeerec_ETA),2)+pow((maxebeerec_ETA-next_to_maxebeerec_ETA),2));
  float ho_DR = sqrt(pow((maxhorec_ETA-next_to_maxhorec_ETA),2)+pow((maxhorec_ETA-next_to_maxhorec_ETA),2));
  float hbMinus_DR = sqrt(pow((maxhbMinusrec_ETA-next_to_maxhbMinusrec_ETA),2)+pow((maxhbMinusrec_ETA-next_to_maxhbMinusrec_ETA),2));
  float hbPlus_DR = sqrt(pow((maxhbPlusrec_ETA-next_to_maxhbPlusrec_ETA),2)+pow((maxhbPlusrec_ETA-next_to_maxhbPlusrec_ETA),2));
  if (hbhe_DR < 2.5 && total_hbhe > 0. ) h_maxhbherec->Fill(total_hbhe);
  if (ebee_DR < 0.4 && total_ebee > 0. ) h_maxebeerec->Fill(total_ebee);
  if (ho_DR < 2.5 && total_ho > 0.) h_maxhorec->Fill(total_ho);
  if ((hbhe_DR < 2.5 && total_hbhe > 0.) && (ebee_DR < 0.4 && total_ebee > 0.) ) h_maxebee_plus_maxhbhe->Fill(total_ebee+total_hbhe);
  if (hbMinus_DR < 2.5 && total_hbMinus > 0.) h_maxhbMinusrec->Fill(total_hbMinus);  
  if (hbPlus_DR < 2.5 && total_hbPlus > 0.) h_maxhbPlusrec->Fill(total_hbPlus);


  for(TrackCollection::const_iterator ncm = cosmicmuon->begin(); ncm != cosmicmuon->end();  ++ncm) {
    h_muon_vertex_x->Fill(ncm->vx());
    h_muon_px->Fill(ncm->px());
    h_muon_p->Fill(ncm->p());
  }

  std::vector < CaloJet > myjet;
    for (CaloJetCollection::const_iterator jetiter = cjet.begin(); jetiter != cjet.end(); jetiter++) {
        h_jet_Pt->Fill(jetiter->pt());
        h_jet_Eta->Fill(jetiter->eta());
        h_jet_Phi->Fill(jetiter->phi());
        myjet.push_back(*jetiter);
    }
    if (myjet.size() > 0) {
        std::stable_sort(myjet.begin(), myjet.end(), CaloJetSort());
        // now go to deeper levels:
        // 1-from jet to calo-tower
        CaloJet jet;

        jet = myjet[0];
        h_leadJet_Pt->Fill(jet.pt());
        h_leadJet_Eta->Fill(jet.eta());
        h_leadJet_Phi->Fill(jet.phi());
    }
    // MET
    h_caloMet_Met->Fill(calomet->pt());
    h_caloMet_Phi->Fill(calomet->phi());
    h_caloMet_SumEt->Fill(calomet->sumEt());


  for (CaloTowerCollection::const_iterator kal=calohbhe.begin(); kal!=calohbhe.end(); kal++){	
    h_calo_tower_energy->Fill(kal->energy());
  }

  //correlations
  for(TrackCollection::const_iterator ncm = cosmicmuon->begin(); ncm != cosmicmuon->end();  ++ncm) {
    for (reco::BasicClusterCollection::const_iterator clus = clusterCollection_p->begin(); clus != clusterCollection_p->end(); ++clus){
      h_ecalx_vs_muonx->Fill(clus->x(),ncm->vx());
      h_ecaly_vs_muony->Fill(clus->y(),ncm->vy());
      h_impact_diff->Fill(clus->z()-ncm->vx());
    }
    for (CaloJetCollection::const_iterator jetiter=cjet.begin(); jetiter!=cjet.end(); jetiter++){
      h_jetphi_vs_muonphi->Fill(jetiter->phi(),ncm->phi());
    } 
  }
    // Check Muon and HCal match
    double radius_hcal = 286.4;
    double conesize = -999999.;
    bool abort_cone_increase = false;

    for (reco::TrackCollection::const_iterator cmTrack = cosmicmuon->begin(); cmTrack != cosmicmuon->end(); ++cmTrack) {
        double dRtracktowerp = 999999.;
        double dRtracktowerm = 999999.;

        double thetap = -999.;
        double etap = -999.;
        double phip = -999.;
        double thetam = -999.;
        double etam = -999.;
        double phim = -999.;

        Extrapolate(cmTrack->innerPosition().x(), cmTrack->innerPosition().y(), cmTrack->innerPosition().z(),
          cmTrack->innerMomentum().x(), cmTrack->innerMomentum().y(), cmTrack->innerMomentum().z(),
          radius_hcal, &thetap, &phip, &thetam, &phim);

        if (thetam != -999.) {
            etam = -1 * TMath::Log(TMath::Tan(thetam / 2.));
            etap = -1 * TMath::Log(TMath::Tan(thetap / 2.));
            if (TMath::IsNaN(etam))
                etam = -999;
            if (TMath::IsNaN(etap))
                etap = -999;
        }
        // loop different matching cones, 0.1, 0.2 - 1.0
        abort_cone_increase = false;
        for (int conei = 1; conei <= 10; ++conei) {
            if (abort_cone_increase)
                break;

            conesize = conei * 0.01;    // max cone of dR<0.1
            for (CaloTowerCollection::const_iterator cnd = calo->begin(); cnd != calo->end(); cnd++) {

                float ieta = cnd->eta();
                float iphi = cnd->phi();
                double ene = cnd->hadEnergy();

                DT_HCAL_eta_correlation_all->Fill(etam, ieta);
                DT_HCAL_eta_correlation_all->Fill(etap, ieta);
                DT_HCAL_phi_correlation_all->Fill(phim, iphi);
                DT_HCAL_phi_correlation_all->Fill(phip, iphi);

                HCAL_energy_correlation_all->Fill(ene);

                if (TMath::Abs(etap) < 4.)
                    dRtracktowerp = TMath::Sqrt(TMath::Power((ieta - etap), 2.) + TMath::Power((iphi - phip), 2.));
                if (TMath::Abs(etam) < 4.)
                    dRtracktowerm = TMath::Sqrt(TMath::Power((ieta - etam), 2.) + TMath::Power((iphi - phim), 2.));

                if (TMath::IsNaN(dRtracktowerm))
                    dRtracktowerm = 999.;
                if (TMath::IsNaN(dRtracktowerp))
                    dRtracktowerp = 999.;

                if (ene > 1. && (dRtracktowerp < conesize)) {
                    std::cout << "In Run/Event   : " << runNo << "/" << evtNo << std::endl;
                    std::cout << "Match within   : " << dRtracktowerp << std::endl;
                    std::cout << "cal eta()      : " << ieta << std::endl;
                    std::cout << "cal phi()      : " << iphi << std::endl;
                    std::cout << "cal energy()   : " << ene << std::endl;
                    std::cout << "Cosmic muon extrapolated to be at eta   p: " << etap << std::endl;
                    std::cout << "Cosmic muon extrapolated to be at phi   p: " << phip << std::endl;
                    abort_cone_increase = true;
                    DT_HCAL_eta_correlation->Fill(etap, ieta);
                    DT_HCAL_phi_correlation->Fill(phip, iphi);
                    HCAL_energy_correlation->Fill(ene);
                }
                if (ene > 1. && (dRtracktowerm < conesize)) {
                    std::cout << "In Run/Event   : " << runNo << "/" << evtNo << std::endl;
                    std::cout << "Match within   : " << dRtracktowerp << std::endl;
                    std::cout << "cal eta()      : " << ieta << std::endl;
                    std::cout << "cal phi()      : " << iphi << std::endl;
                    std::cout << "cal energy()   : " << ene << std::endl;
                    std::cout << "Cosmic muon extrapolated to be at eta   p: " << etap << std::endl;
                    std::cout << "Cosmic muon extrapolated to be at phi   p: " << phip << std::endl;
                    abort_cone_increase = true;
                    DT_HCAL_eta_correlation->Fill(etap, ieta);
                    DT_HCAL_phi_correlation->Fill(phip, iphi);
                    HCAL_energy_correlation->Fill(ene);
                }
            }
        }
    }
    ++evtNo;

}


// ------------ method called once each job just before starting event loop  ------------
void HcalProm::beginJob(const edm::EventSetup&)
{
    runNo = 0;
    evtNo = 0;
    TrigDT = 0;
    startTime = "Not Avaliable";

    TFileDirectory EcalDir = fs->mkdir( "Ecal" );
    TFileDirectory HcalDir = fs->mkdir( "Hcal" );
    TFileDirectory MuonDir = fs->mkdir( "Muon" );
    TFileDirectory CorrDir = fs->mkdir( "Correlations" );
    TFileDirectory NoiseDir = fs->mkdir( "Noise" );
    TFileDirectory JetMetDir = fs->mkdir( "JetMet" );

    //Add runnumbers to histograms!
    
    h_hbhe_rechit_energy = HcalDir.make<TH1F>("h_hbhe_rechit_energy","RecHit Energy HBHE",130,-10,120);
    h_hf_rechit_energy = HcalDir.make<TH1F>("h_hf_rechit_energy","RecHit Energy HF",130,-10,120);
    
    h_maxhbherec = HcalDir.make<TH1F>("h_maxhbherec","HBHE Muon (GeV)",200,0,15);
    h_maxhorec = HcalDir.make<TH1F>("h_maxhorec","HO Muon (GeV)",200,0,15);
    h_maxhbMinusrec = HcalDir.make<TH1F>("h_maxhbMinusrec","HB- Muon (GeV)",200,0,15);
    h_maxhbPlusrec = HcalDir.make<TH1F>("h_maxhbPlusrec","HB+ Muon (GeV)",200,0,15);
    h_hbhe_eta_phi = HcalDir.make<TH2F>("h_hbhe_eta_phi","#eta(HBHE)",60,-30,30,72,0,72);
    h_hf_eta_phi = HcalDir.make<TH2F>("h_hf_eta_phi","#eta(HF)",96,-48,48,72,0,72);
    h_ho_rechit_energy = HcalDir.make<TH1F>(" h_ho_rechit_energy","RecHit Energy HO",130,-10,120);
    h_ho_eta_phi = HcalDir.make<TH2F>("h_ho_eta_phi","#eta(HO)",60,-30,30,72,0,72);
    
    h_hbtiming = HcalDir.make<TH1F>("h_hbtiming","HBHE Timing",10,-0.5,9.5);
    
    h_jet_Pt = JetMetDir.make < TH1F > ("h_jet_Pt", "Jet PT", 130, -10, 120);
    h_jet_Eta = JetMetDir.make < TH1F > ("h_jet_Eta", "Jet Eta", 100, -7, 7);
    h_jet_Phi = JetMetDir.make < TH1F > ("h_jet_Phi", "Jet Phi", 100, -7, 7);
    h_leadJet_Pt = JetMetDir.make < TH1F > ("h_leadJet_Pt", "Leading Jet PT", 120, 0, 120);
    h_leadJet_Eta = JetMetDir.make < TH1F > ("h_leadJet_Eta", "Leading Jet Eta", 100, -7, 7);
    h_leadJet_Phi = JetMetDir.make < TH1F > ("h_leadJet_Phi", "Leading Jet Phi", 100, -7, 7);
    h_caloMet_Met = JetMetDir.make < TH1F > ("h_caloMet_Met", "MET from CaloTowers", 100, 0, 50);
    h_caloMet_Phi = JetMetDir.make < TH1F > ("h_caloMet_Phi", "MET #phi from CaloTowers", 100, -7, 7);
    h_caloMet_SumEt = JetMetDir.make < TH1F > ("h_caloMet_SumEt", "SumET from CaloTowers", 100, 0, 50);

    h_eb_rechit_energy = EcalDir.make<TH1F>(" h_eb_rechit_energy","RecHit Energy EB",130,-10,120);
    h_maxebeerec = EcalDir.make<TH1F>("h_maxebeerec","EBEE Muon (GeV)",200,0,15);
    h_ecal_cluster_energy = EcalDir.make<TH1F>("h_ecal_cluster_energy","EB Cluster Energy",130,-10,120);
    h_ecal_cluster_eta = EcalDir.make<TH1F>("h_ecal_cluster_eta","#eta(EB Cluster)",100,-6,6);
    h_ecal_cluster_phi = EcalDir.make<TH1F>("h_ecal_cluster_phi","#phi(EB Cluster)",100,-6,6);
  
    h_maxebee_plus_maxhbhe = CorrDir.make<TH1F>("h_maxebee_plus_maxhbhe","EBEE+HBHE Muon (GeV)",200,0,15);

    h_ecal_vs_hcal_X = CorrDir.make<TH2F>("h_ecal_vs_hcal_X","X(EB) vs X(HB)",1000,-100,100,100,-100,100);
    h_ecal_vs_hcal_Y = CorrDir.make<TH2F>("h_ecal_vs_hcal_Y","Y(EB) vs Y(HB)",1000,-100,100,100,-100,100);

    h_calo_tower_energy = JetMetDir.make<TH1F>("h_calo_tower_energy","Calo Tower Energy",130,-10,120);

    h_muon_vertex_x = MuonDir.make < TH1F > ("h_muon_vertex_x", "Muon Vertex X", 10000, -1000, 1000);
    h_muon_px = MuonDir.make < TH1F > ("h_muon_px", "Px(#mu)", 1000, -10, 100);
    h_muon_p = MuonDir.make < TH1F > ("h_muon_p", "P(#mu)", 1000, -10, 100);

    h_ecalx_vs_muonx = CorrDir.make<TH2F>("h_ecalx_vs_muonx","h_ecalx_vs_muonx",1000,-400,400,1000,-400,400);
    h_ecaly_vs_muony = CorrDir.make<TH2F>("h_ecaly_vs_muony","h_ecaly_vs_muony",1000,-1000,1000,1000,-1000,1000);
    h_impact_diff = CorrDir.make<TH1F>("h_impact_diff","h_impact_diff",1000,-200,200);
    h_jetphi_vs_muonphi = CorrDir.make<TH2F>("h_jetphi_vs_muonphi","h_jetphi_vs_muonphi",100,-6,6,100,-6,6);
    
    DT_HCAL_eta_correlation =
      MuonDir.make < TH2F > ("DT_CalTower_eta_correlation", "DT eta vs Calo eta at r= 286.4cm", 800, -4., 4., 800, -4.,
      4.);
    DT_HCAL_eta_correlation_all =
      MuonDir.make < TH2F > ("DT_CalTower_eta_correlation_all", "DT eta vs Calo eta at r= 286.4cm", 800, -4., 4., 800,
      -4., 4.);
    DT_HCAL_phi_correlation =
      MuonDir.make < TH2F > ("DT_CalTower_phi_correlation", "DT phi vs Calo phi at r= 286.4cm", 800, -4., 4., 800, -4.,
      4.);
    DT_HCAL_phi_correlation_all =
      MuonDir.make < TH2F > ("DT_CalTower_phi_correlation_all", "DT phi vs Calo phi at r= 286.4cm", 800, -4., 4., 800,
      -4., 4.);
    HCAL_energy_correlation =
      MuonDir.make < TH1F > ("CalTower_energy_correlation", "Calo had energy at r= 286.4cm", 800, -4., 4.);
    HCAL_energy_correlation_all =
      MuonDir.make < TH1F > ("CalTower_energy_correlation_all", "Calo had energy at r= 286.4cm", 800, -4., 4.);
    
    /*
    h_Qiesum = NoiseDir.make < TH1F > ("h_Qiesum", "Qiesum All Channels", 30000, -100., 29900.);
    string Noise[9] = { "1", "5", "20", "80", "150", "300", "600", "1000", "1500" };
    for (int hi = 0; hi != 9; ++hi) {
        if (hi != 8) {
            h_NoiseChan[hi] =
              NoiseDir.make < TH2F > (string("h_Noise_phi_vs_eta_" + Noise[hi] + "-" + Noise[hi + 1]).c_str(),
              string("Noise i#phi vs i#eta between " + Noise[hi] + " and " + Noise[hi + 1] + "GeV").c_str(), 40, -19.5,
              20.5, 73, -.5, 72.5);
            string title;

            title = "Events between " + Noise[hi] + " and " + Noise[hi + 1] + "GeV";
            h_NoisePhi[hi] =
              NoiseDir.make < TH1F > (string("h_Noise_Phi_" + Noise[hi] + "-" + Noise[hi + 1]).c_str(), title.c_str(),
              73, -.5, 72.5);
        } else {
            h_NoiseChan[hi] = NoiseDir.make < TH2F > (string("h_Noise_phi_vs_eta_" + Noise[hi]).c_str(),
              string("Noise i#phi vs i#eta above " + Noise[hi] + "GeV").c_str(), 40, -19.5, 20.5, 73, -.5, 72.5);
            string title;

            title = "Events above " + Noise[hi];
            title = title + "GeV vs i#phi";
            h_NoisePhi[hi] =
              NoiseDir.make < TH1F > (string("h_Noise_Phi_" + Noise[hi]).c_str(), title.c_str(), 73, -.5, 72.5);
        }
    }
    */
}

// ------------ method called once each job just after ending the event loop  ------------
void HcalProm::endJob() {
  
  if (prompt_htmlPrint) htmlOutput();
}

//      HTML OUTPUT


void HcalProm::htmlOutput(void) 
{


    cout << "Preparing HcalPrompt html output ..." << endl;

    char tmp[10];

    if (runNo != -1)
        sprintf(tmp, "HcalPrompt_R%09d", runNo);
    else
        sprintf(tmp, "HcalPrompt_R%09d", 0);
    string htmlDir = baseHtmlDir_ + "/" + tmp + "/";

    system(("/bin/mkdir -p " + htmlDir).c_str());

    std::ofstream htmlFile;

    htmlFile.open((htmlDir + "index.html").c_str());

    // html page header
    htmlFile << "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">  " << endl;
    htmlFile << "<html>  " << endl;
    htmlFile << "<head>  " << endl;
    htmlFile << "  <meta content=\"text/html; charset=ISO-8859-1\"  " << endl;
    htmlFile << " http-equiv=\"content-type\">  " << endl;
    htmlFile << "  <title>Hcal Prompt Analysis Output</title> " << endl;
    htmlFile << "</head>  " << endl;
    htmlFile << "<body>  " << endl;
    htmlFile << "<br>  " << endl;
    htmlFile << "<center><h1>Hcal Prompt Analysis Outputs</h1></center>" << endl;
    htmlFile << "<h2>Run Number:&nbsp;&nbsp;&nbsp;" << endl;
    htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << runNo << "</span>" << endl;
    htmlFile << "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Start&nbsp;Time:&nbsp;<span style=\"color: rgb(0, 0, 153);\">" <<
      startTime << "</span></h2> " << endl;
    htmlFile << "<h2>Events processed:&nbsp;&nbsp;&nbsp;" << endl;
    htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << evtNo << "</span></h2> " << endl;
    htmlFile << "<hr>" << endl;
    htmlFile << "<ul>" << endl;

    string htmlName;

    if (doRecHitHTML) {
        htmlName = "HcalPromptRecHit.html";
        RecHitHTMLOutput(runNo, startTime, htmlDir, htmlName);
        htmlFile << "<table border=0 WIDTH=\"50%\"><tr>" << endl;
        htmlFile << "<td WIDTH=\"35%\"><a href=\"" << htmlName << "\">RecHit Prompt</a></td>" << endl;

    }
    if (doDigiHTML) {
        htmlName = "HcalPromptDigi.html";
        DigiHTMLOutput(runNo, startTime, htmlDir, htmlName);
        htmlFile << "<table border=0 WIDTH=\"50%\"><tr>" << endl;
        htmlFile << "<td WIDTH=\"35%\"><a href=\"" << htmlName << "\">Digi Prompt</a></td>" << endl;
    }
    if (doJetMetHTML) {
        htmlName = "HcalPromptJetMet.html";
        // htmlOutput(runNo, startTime,htmlDir, htmlName,htmlTitle);
        JetMetHTMLOutput(runNo, startTime, htmlDir, htmlName);
        htmlFile << "<table border=0 WIDTH=\"50%\"><tr>" << endl;
        htmlFile << "<td WIDTH=\"35%\"><a href=\"" << htmlName << "\">JetMet Prompt</a></td>" << endl;
    }
    if (doMuonHTML) {
        htmlName = "HcalPromptMuon.html";
        MuonHTMLOutput(runNo, startTime, htmlDir, htmlName);
        htmlFile << "<table border=0 WIDTH=\"50%\"><tr>" << endl;
        htmlFile << "<td WIDTH=\"35%\"><a href=\"" << htmlName << "\">Muon Prompt</a></td>" << endl;
    }
    if (doHPDNoiseHTML) {
        htmlName = "HcalPromptHPDNoise.html";
        HPDNoiseHTMLOutput(runNo, startTime, htmlDir, htmlName);
        htmlFile << "<table border=0 WIDTH=\"50%\"><tr>" << endl;
        htmlFile << "<td WIDTH=\"35%\"><a href=\"" << htmlName << "\">HPD Noise Prompt</a></td>" << endl;
    }
    if (doCaloTowerHTML) {
        htmlName = "HcalPromptCaloTower.html";
        CaloTowerHTMLOutput(runNo, startTime, htmlDir, htmlName);
        htmlFile << "<table border=0 WIDTH=\"50%\"><tr>" << endl;
        htmlFile << "<td WIDTH=\"35%\"><a href=\"" << htmlName << "\">CaloTower Prompt</a></td>" << endl;
    }
    htmlFile << "</tr></table>" << endl;
    htmlFile << "</ul>" << endl;

    // html page footer
    htmlFile << "</body> " << endl;
    htmlFile << "</html> " << endl;

    htmlFile.close();
    cout << "HcalProm html output done..." << endl;
    return;
}


void HcalProm::JetMetHTMLOutput(int runNo, string startTime, string htmlDir, string htmlName) {

    cout << "Preparing html output for JetMet" << endl;

    ofstream htmlFile;

    htmlFile.open((htmlDir + htmlName).c_str());

    // html page header
    htmlFile << "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">  " << endl;
    htmlFile << "<html>  " << endl;
    htmlFile << "<head>  " << endl;
    htmlFile << "  <meta content=\"text/html; charset=ISO-8859-1\"  " << endl;
    htmlFile << " http-equiv=\"content-type\">  " << endl;
    htmlFile << "  <title>JetMet Plots</title> " << endl;
    htmlFile << "</head>  " << endl;
    htmlFile << "<style type=\"text/css\"> td { font-weight: bold } </style>" << endl;
    htmlFile << "<body>  " << endl;
    htmlFile << "<br>  " << endl;
    htmlFile << "<h2>Run Number:&nbsp;&nbsp;&nbsp;" << endl;
    htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << runNo << "</span>" << endl;
    htmlFile << "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Start&nbsp;Time:&nbsp;<span style=\"color: rgb(0, 0, 153);\">" <<
      startTime << "</span></h2> " << endl;
    htmlFile << "<h2>Plots from :&nbsp;&nbsp;&nbsp;&nbsp; <span " << endl;
    htmlFile << " style=\"color: rgb(0, 0, 153);\">JetMet</span></h2> " << endl;
    htmlFile << "<h2>Events processed:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" << endl;
    htmlFile << "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span " << endl;
    htmlFile << " style=\"color: rgb(0, 0, 153);\">" << evtNo << "</span></h2>" << endl;
    // begin table for histograms
    htmlFile << "<table width=100%  border=1><tr>" << endl;
    htmlFile << "</tr></table>" << endl;
    htmlFile << "<hr>" << endl;

    htmlFile << "<h2><strong>(HcalPrompt)&nbsp;JetMet&nbsp;Histograms</strong></h2>" << endl;

    htmlFile << "<h3>" << endl;
    htmlFile << "<a href=\"#AllJet_Plots\">AllJet Plots </a></br>" << endl;
    htmlFile << "<a href=\"#LeadingJet_Plots\">LeadingJet Plots </a></br>" << endl;
    htmlFile << "<a href=\"#Met_Plots\">Met Plots </a></br>" << endl;
    htmlFile << "</h3>" << endl;
    htmlFile << "<hr>" << endl;

    for (int i = 0; i < 3; i++) {
        string type = "AllJets";

        if (i == 1)
            type = "LeadingJet";
        if (i == 2)
            type = "Met";

        htmlFile << "<tr align=\"left\">" << endl;
        htmlFile << "<td>&nbsp;&nbsp;&nbsp;<a name=\"" << type << "_Plots\"><h3>" << type <<
          " Histograms</h3></td></tr>" << endl;
        if (i == 0) {
            htmlFile << "<tr align=\"left\">" << endl;
            histoHTML(runNo, h_jet_Pt, "All Jets Pt", "Events", 92, htmlFile, htmlDir);
            histoHTML(runNo, h_jet_Eta, "All Jets Eta", "Events", 92, htmlFile, htmlDir);
            histoHTML(runNo, h_jet_Phi, "all Jets Phi", "Events", 92, htmlFile, htmlDir);
            htmlFile << "</tr>" << endl;
        }
        if (i == 1) {
            htmlFile << "<tr align=\"left\">" << endl;
            histoHTML(runNo, h_leadJet_Pt, "Leading Jet Pt", "Events", 92, htmlFile, htmlDir);
            histoHTML(runNo, h_leadJet_Eta, "Leading Jet Eta", "Events", 92, htmlFile, htmlDir);
            histoHTML(runNo, h_leadJet_Phi, "Leading Jet Phi", "Events", 92, htmlFile, htmlDir);
            htmlFile << "</tr>" << endl;
        }
        if (i == 2) {
            htmlFile << "<tr align=\"left\">" << endl;
            histoHTML(runNo, h_caloMet_Met, "CaloMET", "Events", 92, htmlFile, htmlDir);
            histoHTML(runNo, h_caloMet_Phi, "CaloMet Phi", "Events", 92, htmlFile, htmlDir);
            histoHTML(runNo, h_caloMet_SumEt, "Scalar Sum ET", "Events", 92, htmlFile, htmlDir);
            htmlFile << "</tr>" << endl;
        }

    }
    htmlFile << "</table>" << endl;
    // end table
    htmlFile << "<br>" << endl;

    // html page footer
    htmlFile << "</body> " << endl;
    htmlFile << "</html> " << endl;
    htmlFile.close();

}


void HcalProm::RecHitHTMLOutput(int runNo, string startTime, string htmlDir, string htmlName) {

    cout << "Preparing html output for " << htmlName << endl;

    ofstream htmlFile;

    htmlFile.open((htmlDir + htmlName).c_str());

    // html page header
    htmlFile << "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">  " << endl;
    htmlFile << "<html>  " << endl;
    htmlFile << "<head>  " << endl;
    htmlFile << "  <meta content=\"text/html; charset=ISO-8859-1\"  " << endl;
    htmlFile << " http-equiv=\"content-type\">  " << endl;
    htmlFile << "  <title>RecHit Plots</title> " << endl;
    htmlFile << "</head>  " << endl;
    htmlFile << "<style type=\"text/css\"> td { font-weight: bold } </style>" << endl;
    htmlFile << "<body>  " << endl;
    htmlFile << "<br>  " << endl;
    htmlFile << "<h2>Run Number:&nbsp;&nbsp;&nbsp;" << endl;
    htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << runNo << "</span>" << endl;
    htmlFile << "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Start&nbsp;Time:&nbsp;<span style=\"color: rgb(0, 0, 153);\">" <<
      startTime << "</span></h2> " << endl;
    htmlFile << "<h2>Plots from :&nbsp;&nbsp;&nbsp;&nbsp; <span " << endl;
    htmlFile << " style=\"color: rgb(0, 0, 153);\">RecHits</span></h2> " << endl;
    htmlFile << "<h2>Events processed:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" << endl;
    htmlFile << "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span " << endl;
    htmlFile << " style=\"color: rgb(0, 0, 153);\">" << evtNo << "</span></h2>" << endl;
    // begin table for histograms
    htmlFile << "<table width=100%  border=1><tr>" << endl;
    htmlFile << "</tr></table>" << endl;
    htmlFile << "<hr>" << endl;

    htmlFile << "<h2><strong>(HcalPrompt)&nbsp;RecHit&nbsp;Histograms</strong></h2>" << endl;

    htmlFile << "<h3>" << endl;
    htmlFile << "<a href=\"#HBHE_Plots\">HBHE Plots </a></br>" << endl;
    htmlFile << "<a href=\"#HF_Plots\">HF Plots </a></br>" << endl;
    htmlFile << "<a href=\"#HO_Plots\">HO Plots </a></br>" << endl;
    htmlFile << "<a href=\"#EB_Plots\">EB Plots </a></br>" << endl;
    htmlFile << "</h3>" << endl;
    htmlFile << "<hr>" << endl;

    for (int i = 0; i < 4; i++) {
        string type = "HBHE";
        if (i == 0){
	}
        if (i == 1){
            type = "HF";
        }
        if (i == 2){
            type = "HO";
        }
        if (i == 3){
            type = "EB";
        }
        htmlFile << "<tr align=\"left\">" << endl;
        htmlFile << "<td>&nbsp;&nbsp;&nbsp;<a name=\"" << type << "_Plots\"><h3>" << type <<
          " Histograms</h3></td></tr>" << endl;
        if (i == 0){
	    htmlFile << "<tr align=\"left\">" << endl;
	    histoHTML(runNo, h_hbhe_rechit_energy, "HBHE RecHit Energy", "Events", 92, htmlFile, htmlDir);
	    histoHTML2(runNo, h_hbhe_eta_phi, "iEta", "iPhi", 100, htmlFile, htmlDir);
	    htmlFile << "</tr>" << endl;
	}
        if (i == 1){
	    htmlFile << "<tr align=\"left\">" << endl;
	    histoHTML(runNo, h_hf_rechit_energy, "HF RecHit Energy", "Events", 92, htmlFile, htmlDir);
	    histoHTML2(runNo, h_hf_eta_phi, "iEta", "iPhi", 100, htmlFile, htmlDir);
	    htmlFile << "</tr>" << endl;
	}
        if (i == 2){
	    htmlFile << "<tr align=\"left\">" << endl;
	    histoHTML(runNo, h_ho_rechit_energy, "HO RecHit Energy", "Events", 92, htmlFile, htmlDir);
	    histoHTML2(runNo, h_hf_eta_phi, "iEta", "iPhi", 100, htmlFile, htmlDir);
	    htmlFile << "</tr>" << endl;
	}
        if (i == 3){
	    htmlFile << "<tr align=\"left\">" << endl;
	    histoHTML(runNo, h_eb_rechit_energy, "EB RecHit Energy", "Events", 92, htmlFile, htmlDir);
	    htmlFile << "</tr>" << endl;
	}


    }
    htmlFile << "</table>" << endl;
    // end table
    htmlFile << "<br>" << endl;

    // html page footer
    htmlFile << "</body> " << endl;
    htmlFile << "</html> " << endl;
    htmlFile.close();

}

void HcalProm::DigiHTMLOutput(int runNo, string startTime, string htmlDir, string htmlName) {

    cout << "Preparing html output for " << htmlName << endl;

    ofstream htmlFile;

    htmlFile.open((htmlDir + htmlName).c_str());

    // html page header
    htmlFile << "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">  " << endl;
    htmlFile << "<html>  " << endl;
    htmlFile << "<head>  " << endl;
    htmlFile << "  <meta content=\"text/html; charset=ISO-8859-1\"  " << endl;
    htmlFile << " http-equiv=\"content-type\">  " << endl;
    htmlFile << "  <title>Digi Plots</title> " << endl;
    htmlFile << "</head>  " << endl;
    htmlFile << "<style type=\"text/css\"> td { font-weight: bold } </style>" << endl;
    htmlFile << "<body>  " << endl;
    htmlFile << "<br>  " << endl;
    htmlFile << "<h2>Run Number:&nbsp;&nbsp;&nbsp;" << endl;
    htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << runNo << "</span>" << endl;
    htmlFile << "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Start&nbsp;Time:&nbsp;<span style=\"color: rgb(0, 0, 153);\">" <<
      startTime << "</span></h2> " << endl;
    htmlFile << "<h2>Plots from :&nbsp;&nbsp;&nbsp;&nbsp; <span " << endl;
    htmlFile << " style=\"color: rgb(0, 0, 153);\">Digis</span></h2> " << endl;
    htmlFile << "<h2>Events processed:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" << endl;
    htmlFile << "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span " << endl;
    htmlFile << " style=\"color: rgb(0, 0, 153);\">" << evtNo << "</span></h2>" << endl;
    // begin table for histograms
    htmlFile << "<table width=100%  border=1><tr>" << endl;
    htmlFile << "</tr></table>" << endl;
    htmlFile << "<hr>" << endl;

    htmlFile << "<h2><strong>(HcalPrompt)&nbsp;Digi&nbsp;Histograms</strong></h2>" << endl;

    htmlFile << "<h3>" << endl;
    htmlFile << "<a href=\"#HB_Plots\">HB Plots </a></br>" << endl;
    htmlFile << "<a href=\"#HE_Plots\">HE Plots </a></br>" << endl;
    htmlFile << "<a href=\"#HF_Plots\">HF Plots </a></br>" << endl;
    htmlFile << "<a href=\"#HO_Plots\">HO Plots </a></br>" << endl;
    htmlFile << "</h3>" << endl;
    htmlFile << "<hr>" << endl;
    htmlFile << "</table>" << endl;
    // end table
    htmlFile << "<br>" << endl;

    // html page footer
    htmlFile << "</body> " << endl;
    htmlFile << "</html> " << endl;
    htmlFile.close();

}

void HcalProm::HPDNoiseHTMLOutput(int runNo, string startTime, string htmlDir, string htmlName) {

    cout << "Preparing html output for HPDNoise" << endl;

    ofstream htmlFile;

    htmlFile.open((htmlDir + htmlName).c_str());

    // html page header
    htmlFile << "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">  " << endl;
    htmlFile << "<html>  " << endl;
    htmlFile << "<head>  " << endl;
    htmlFile << "  <meta content=\"text/html; charset=ISO-8859-1\"  " << endl;
    htmlFile << " http-equiv=\"content-type\">  " << endl;
    htmlFile << "  <title>HPDNoise Plots</title> " << endl;
    htmlFile << "</head>  " << endl;
    htmlFile << "<style type=\"text/css\"> td { font-weight: bold } </style>" << endl;
    htmlFile << "<body>  " << endl;
    htmlFile << "<br>  " << endl;
    htmlFile << "<h2>Run Number:&nbsp;&nbsp;&nbsp;" << endl;
    htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << runNo << "</span>" << endl;
    htmlFile <<
      "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Start&nbsp;Time:&nbsp;<span style=\"color: rgb(0, 0, 153);\">" <<
      startTime << "</span></h2> " << endl;
    htmlFile << "<h2>Plots from :&nbsp;&nbsp;&nbsp;&nbsp; <span " << endl;
    htmlFile << " style=\"color: rgb(0, 0, 153);\">HPD Noise</span></h2> " << endl;
    htmlFile << "<h2>Events processed:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" << endl;
    htmlFile << "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span " << endl;
    htmlFile << " style=\"color: rgb(0, 0, 153);\">" << evtNo << "</span></h2>" << endl;
    // begin table for histograms
    htmlFile << "<table width=100%  border=1><tr>" << endl;
    htmlFile << "</tr></table>" << endl;
    htmlFile << "<hr>" << endl;

    htmlFile << "<h2><strong>(HcalPrompt)&nbsp;HPDNoise&nbsp;Histograms</strong></h2>" << endl;

    htmlFile << "<h3>" << endl;
    htmlFile << "<a href=\"#Qiesum_Plots\">Qiesum Plot </a></br>" << endl;
    htmlFile << "<a href=\"#NoiseChan_Plots\">NoiseChan Plots </a></br>" << endl;
    htmlFile << "<a href=\"#NoisePhi_Plots\">NoisePhi Plots </a></br>" << endl;
    htmlFile << "</h3>" << endl;
    htmlFile << "<hr>" << endl;
    for (int i = 0; i < 3; i++) {
        string type = "Qiesum";

        if (i == 1)
            type = "NoiseChan";
        if (i == 2)
            type = "NoisePhi";

        htmlFile << "<tr align=\"left\">" << endl;
        htmlFile << "<td>&nbsp;&nbsp;&nbsp;<a name=\"" << type << "_Plots\"><h3>" << type <<
          " Histograms</h3></td></tr>" << endl;
        if (i == 0) {
            htmlFile << "<tr align=\"left\">" << endl;
            histoHTML(runNo, h_Qiesum, "Qiesum (fC)", "Channels", 92, htmlFile, htmlDir);
            htmlFile << "</tr>" << endl;
        }
        if (i == 1) {
            // string Noise[9] = {"1", "5", "20", "80", "150", "300", "600", "1000", "1500"};
            htmlFile << "<tr align=\"left\">" << endl;
            for (int ni = 0; ni != 9; ++ni) {
                histoHTML2(runNo, h_NoiseChan[ni], "i#eta", "i#phi", 92, htmlFile, htmlDir);
            }
            htmlFile << "</tr>" << endl;
        }
        if (i == 2) {
            htmlFile << "<tr align=\"left\">" << endl;
            for (int ni = 0; ni != 9; ++ni) {
                histoHTML(runNo, h_NoisePhi[ni], "i#phi", "# Noisy Pixels in Range", 92, htmlFile, htmlDir);
            }
            htmlFile << "</tr>" << endl;
        }

    }
    htmlFile << "</table>" << endl;
    // end table
    htmlFile << "<br>" << endl;

    // html page footer
    htmlFile << "</body> " << endl;
    htmlFile << "</html> " << endl;
    htmlFile.close();

}

void HcalProm::MuonHTMLOutput(int runNo, string startTime, string htmlDir, string htmlName) {

    cout << "Preparing html output for Muon" << endl;

    ofstream htmlFile;

    htmlFile.open((htmlDir + htmlName).c_str());

    // html page header
    htmlFile << "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">  " << endl;
    htmlFile << "<html>  " << endl;
    htmlFile << "<head>  " << endl;
    htmlFile << "  <meta content=\"text/html; charset=ISO-8859-1\"  " << endl;
    htmlFile << " http-equiv=\"content-type\">  " << endl;
    htmlFile << "  <title>Muon Plots</title> " << endl;
    htmlFile << "</head>  " << endl;
    htmlFile << "<style type=\"text/css\"> td { font-weight: bold } </style>" << endl;
    htmlFile << "<body>  " << endl;
    htmlFile << "<br>  " << endl;
    htmlFile << "<h2>Run Number:&nbsp;&nbsp;&nbsp;" << endl;
    htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << runNo << "</span>" << endl;
    htmlFile <<
      "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Start&nbsp;Time:&nbsp;<span style=\"color: rgb(0, 0, 153);\">" <<
      startTime << "</span></h2> " << endl;
    htmlFile << "<h2>Plots from :&nbsp;&nbsp;&nbsp;&nbsp; <span " << endl;
    htmlFile << " style=\"color: rgb(0, 0, 153);\">Muons</span></h2> " << endl;
    htmlFile << "<h2>Events processed:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" << endl;
    htmlFile << "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span " << endl;
    htmlFile << " style=\"color: rgb(0, 0, 153);\">" << evtNo << "</span></h2>" << endl;
    // begin table for histograms
    htmlFile << "<table width=100%  border=1><tr>" << endl;
    htmlFile << "</tr></table>" << endl;
    htmlFile << "<hr>" << endl;

    htmlFile << "<h2><strong>(HcalPrompt)&nbsp;Muon&nbsp;Histograms</strong></h2>" << endl;

    htmlFile << "<h3>" << endl;
    htmlFile << "<a href=\"#Muon_Plots\">Muon Plots </a></br>" << endl;
    htmlFile << "</h3>" << endl;
    htmlFile << "<hr>" << endl;

    string type = "Muon";

    htmlFile << "<tr align=\"left\">" << endl;
    htmlFile << "<td>&nbsp;&nbsp;&nbsp;<a name=\"" << type << "_Plots\"><h3>" << type << " Histograms</h3></td></tr>" << endl;
    htmlFile << "<tr align=\"left\">" << endl;
    histoHTML(runNo, h_maxhbherec, "HBHE Muon (GeV)", "Events", 92, htmlFile, htmlDir);
    histoHTML(runNo, h_maxhorec, "HO Muon (GeV)", "Events", 92, htmlFile, htmlDir);
    histoHTML(runNo, h_maxhbMinusrec, "HB- Muon (GeV)", "Events", 92, htmlFile, htmlDir);
    histoHTML(runNo, h_maxhbPlusrec, "HB+ Muon (GeV)", "Events", 92, htmlFile, htmlDir);
    histoHTML(runNo, h_maxebeerec, "EBEE Muon (GeV)", "Events", 92, htmlFile, htmlDir);
    histoHTML(runNo, h_maxebee_plus_maxhbhe, "EBEE+HBHE Muon (GeV)", "Events", 92, htmlFile, htmlDir);
    histoHTML(runNo, h_muon_vertex_x, "Muon Vertex X", "Events", 92, htmlFile, htmlDir);
    histoHTML(runNo, h_muon_px, "Muon Px", "Events", 92, htmlFile, htmlDir);
    histoHTML(runNo, h_muon_p, "Muon P", "Events", 92, htmlFile, htmlDir);
    histoHTML2(runNo, h_ecalx_vs_muonx, "ECAL BasicClu x", "Muon x", 100, htmlFile, htmlDir);
    histoHTML2(runNo, h_ecaly_vs_muony, "ECAL BasicClu y", "Muon y", 100, htmlFile, htmlDir);
    histoHTML2(runNo, h_jetphi_vs_muonphi, "Jet Phi", "Muon Phi", 100, htmlFile, htmlDir);
    histoHTML(runNo, h_impact_diff, "ECAL BasicClu X - Muon X", "Events", 92, htmlFile, htmlDir);
    histoHTML2(runNo, DT_HCAL_eta_correlation, "DT eta", "Calo eta", 100, htmlFile, htmlDir);
    histoHTML2(runNo, DT_HCAL_eta_correlation_all, "DT eta all", "Calo eta all", 100, htmlFile, htmlDir);
    histoHTML2(runNo, DT_HCAL_phi_correlation, "DT phi", "Calo phi", 100, htmlFile, htmlDir);
    histoHTML2(runNo, DT_HCAL_phi_correlation_all, "DT phi all", "Calo phi all", 100, htmlFile, htmlDir);
    histoHTML(runNo, HCAL_energy_correlation, "Calo had energy", "Events", 92, htmlFile, htmlDir);
    histoHTML(runNo, HCAL_energy_correlation_all, "Calo had energy all", "Events", 92, htmlFile, htmlDir);
    htmlFile << "</tr>" << endl;
    htmlFile << "</table>" << endl;
    // end table
    htmlFile << "<br>" << endl;

    // html page footer
    htmlFile << "</body> " << endl;
    htmlFile << "</html> " << endl;
    htmlFile.close();

}

void HcalProm::CaloTowerHTMLOutput(int runNo, string startTime, string htmlDir, string htmlName) {

    cout << "Preparing html output for CaloTower" << endl;

    ofstream htmlFile;

    htmlFile.open((htmlDir + htmlName).c_str());

    // html page header
    htmlFile << "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">  " << endl;
    htmlFile << "<html>  " << endl;
    htmlFile << "<head>  " << endl;
    htmlFile << "  <meta content=\"text/html; charset=ISO-8859-1\"  " << endl;
    htmlFile << " http-equiv=\"content-type\">  " << endl;
    htmlFile << "  <title>Calo Tower Plots</title> " << endl;
    htmlFile << "</head>  " << endl;
    htmlFile << "<style type=\"text/css\"> td { font-weight: bold } </style>" << endl;
    htmlFile << "<body>  " << endl;
    htmlFile << "<br>  " << endl;
    htmlFile << "<h2>Run Number:&nbsp;&nbsp;&nbsp;" << endl;
    htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << runNo << "</span>" << endl;
    htmlFile <<
      "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Start&nbsp;Time:&nbsp;<span style=\"color: rgb(0, 0, 153);\">" <<
      startTime << "</span></h2> " << endl;
    htmlFile << "<h2>Plots from :&nbsp;&nbsp;&nbsp;&nbsp; <span " << endl;
    htmlFile << " style=\"color: rgb(0, 0, 153);\">CaloTowers</span></h2> " << endl;
    htmlFile << "<h2>Events processed:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" << endl;
    htmlFile << "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span " << endl;
    htmlFile << " style=\"color: rgb(0, 0, 153);\">" << evtNo << "</span></h2>" << endl;
    // begin table for histograms
    htmlFile << "<table width=100%  border=1><tr>" << endl;
    htmlFile << "</tr></table>" << endl;
    htmlFile << "<hr>" << endl;

    htmlFile << "<h2><strong>(HcalPrompt)&nbsp;CaloTower&nbsp;Histograms</strong></h2>" << endl;

    htmlFile << "<h3>" << endl;
    htmlFile << "<a href=\"#CaloTower_Plots\">CaloTower Plots </a></br>" << endl;
    htmlFile << "</h3>" << endl;
    htmlFile << "<hr>" << endl;

    string type = "CaloTower";

    htmlFile << "<tr align=\"left\">" << endl;
    htmlFile << "<td>&nbsp;&nbsp;&nbsp;<a name=\"" << type << "_Plots\"><h3>" << type << " Histograms</h3></td></tr>" << endl;
    htmlFile << "<tr align=\"left\">" << endl;
    histoHTML(runNo, h_calo_tower_energy, "CaloTower energy", "Events", 92, htmlFile, htmlDir);
    htmlFile << "</tr>" << endl;
    htmlFile << "</table>" << endl;
    // end table
    htmlFile << "<br>" << endl;

    // html page footer
    htmlFile << "</body> " << endl;
    htmlFile << "</html> " << endl;
    htmlFile.close();

}

void HcalProm::histoHTML(int runNo, TH1F * hist, const char *xlab, const char *ylab, int width, ofstream & htmlFile,
  string htmlDir) {

    if (hist != NULL) {
        string imgNameTMB = "";

        imgNameTMB = getIMG(runNo, hist, 1, htmlDir, xlab, ylab);
        string imgName = "";

        imgName = getIMG(runNo, hist, 2, htmlDir, xlab, ylab);

        if (imgName.size() != 0)
            htmlFile << "<td><a href=\"" << imgName << "\"><img src=\"" << imgNameTMB << "\"></a></td>" << endl;
        else
            htmlFile << "<td><img src=\"" << " " << "\"></td>" << endl;
    } else
        htmlFile << "<td><img src=\"" << " " << "\"></td>" << endl;
    return;
}

void HcalProm::histoHTML2(int runNo, TH2F * hist, const char *xlab, const char *ylab, int width,
  ofstream & htmlFile, string htmlDir, bool color) {
    if (hist != NULL) {
        string imgNameTMB = "";

        imgNameTMB = getIMG2(runNo, hist, 1, htmlDir, xlab, ylab, color);
        string imgName = "";

        imgName = getIMG2(runNo, hist, 2, htmlDir, xlab, ylab, color);
        if (imgName.size() != 0)
            htmlFile << "<td><a href=\"" << imgName << "\"><img src=\"" << imgNameTMB << "\"></a></td>" << endl;
        else
            htmlFile << "<td><img src=\"" << " " << "\"></td>" << endl;
    } else
        htmlFile << "<td><img src=\"" << " " << "\"></td>" << endl;
    return;
}

string HcalProm::getIMG(int runNo, TH1F * hist, int size, string htmlDir, const char *xlab, const char *ylab) {

    if (hist == NULL) {
        printf("getIMG:  This histo is NULL, %s, %s\n", xlab, ylab);
        return "";
    }

    string name = hist->GetTitle();

    cleanString(name);
    char dest[512];

    if (runNo > -1)
        sprintf(dest, "%s - Run %d", name.c_str(), runNo);
    else
        sprintf(dest, "%s", name.c_str());
    hist->SetTitle(dest);
    string title = dest;

    int xwid = 900;
    int ywid = 540;

    if (size == 1) {
        title = title + "_tmb";
        xwid = 600;
        ywid = 360;
    }
    TCanvas *can = new TCanvas(dest, dest, xwid, ywid);
    if(name.find("Qiesum", 0)!= string::npos){
        can->SetLogy();  
    }

    parseString(title);
    string outName = title + ".gif";
    string saveName = htmlDir + outName;

    hist->SetXTitle(xlab);
    hist->SetYTitle(ylab);
    if(name.find("h_max",0)!=string::npos){
        hist->Fit("landau");
    }
    hist->Draw();

    can->SaveAs(saveName.c_str());
    delete can;

    return outName;
}

string HcalProm::getIMG2(int runNo, TH2F * hist, int size, string htmlDir, const char *xlab, const char *ylab,
  bool color) {

    if (hist == NULL) {
        printf("getIMG2:  This histo is NULL, %s, %s\n", xlab, ylab);
        return "";
    }

    string name = hist->GetTitle();

    cleanString(name);
    char dest[512];

    if (runNo > -1)
        sprintf(dest, "%s - Run %d", name.c_str(), runNo);
    else
        sprintf(dest, "%s", name.c_str());
    hist->SetTitle(dest);
    string title = dest;

    int xwid = 900;
    int ywid = 540;

    if (size == 1) {
        title = title + "_tmb";
        xwid = 600;
        ywid = 360;
    }
    TCanvas *can = new TCanvas(dest, dest, xwid, ywid);

    parseString(title);
    string outName = title + ".gif";
    string saveName = htmlDir + outName;

    hist->SetXTitle(xlab);
    hist->SetYTitle(ylab);
    if (color)
        hist->Draw();
    else {
        hist->SetStats(false);
        hist->Draw("COLZ");
    }
    can->SaveAs(saveName.c_str());
    delete can;

    return outName;
}
void HcalProm::cleanString(string & title) {

    for (unsigned int i = 0; i < title.size(); i++) {
        if (title.substr(i, 6) == " - Run") {
            title.replace(i, title.size() - i, "");
        }
        if (title.substr(i, 4) == "_Run") {
            title.replace(i, title.size() - i, "");
        }
        if (title.substr(i, 5) == "__Run") {
            title.replace(i, title.size() - i, "");
        }
    }
}

void HcalProm::parseString(string & title) {

    for (unsigned int i = 0; i < title.size(); i++) {
        if (title.substr(i, 1) == " ") {
            title.replace(i, 1, "_");
        }
        if (title.substr(i, 1) == "#") {
            title.replace(i, 1, "N");
        }
        if (title.substr(i, 1) == "-") {
            title.replace(i, 1, "_");
        }
        if (title.substr(i, 1) == "&") {
            title.replace(i, 1, "_and_");
        }
        if (title.substr(i, 1) == "(" || title.substr(i, 1) == ")") {
            title.replace(i, 1, "_");
        }
    }

    return;
}

// END OF HTML OUTPUT 


void HcalProm::Extrapolate(
  // inputs
  double ox, double oy, double oz, double px, double py, double pz, double ra,
  // outputs
  double *thetap_out, double *phip_out, double *thetam_out, double *phim_out) {

    if (px == 0.) {
        std::
          cout <<
          "px is exactly 0 - the extrapolation can not handle this case currently, sorry. Not extrapolating." <<
          std::endl;
        return;
    }

    double xp = -99999.;
    double yp = -99999.;
    double zp = -99999.;

    double xm = -99999.;
    double ym = -99999.;
    double zm = -99999.;

    double a = py / px;
    double b = oy - a * ox;

    double notnegative = (ra * ra - b * b) / (1 + a * a) + TMath::Power(a * b / (1 + a * a), 2.);

    if (notnegative > 0.) {
        xp = TMath::Sqrt(notnegative) - a * b / (1 + a * a);
        yp = (py / px) * (xp - ox) + oy;
        zp = (pz * (xp - ox) / px) + oz;
        double phip = TMath::ATan2(yp, xp);
        double thetap = TMath::Pi() / 2. - TMath::ASin(zp / TMath::Sqrt(xp * xp + yp * yp));

        xm = -TMath::Sqrt(notnegative) - a * b / (1 + a * a);
        ym = py / px * (xm - ox) + oy;
        zm = pz * (xm - ox) / px + oz;

        double phim = TMath::ATan2(ym, xm);
        double thetam = TMath::ACos(zp / TMath::Sqrt(xm * xm + ym * ym));

        // set the output values if we are in a reasonable z range
        if (TMath::Abs(zp) < 433. && TMath::Abs(zm) < 433.) {
            *thetap_out = thetap;
            *phip_out = phip;
            *thetam_out = thetam;
            *phim_out = phim;
        }
    } else {
        std::cout << "No intersection of Cosmic Muon with Calotower." << std::endl;
    }
}
