// -*- C++ -*-
//
// Package:    HcalProm
// Class:      HcalProm


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
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "DataFormats/CaloTowers/interface/CaloTowerDetId.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"


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
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GtPsbWord.h"

// track propagation
#include "DataFormats/TrajectorySeed/interface/TrajectorySeedCollection.h"
#include "DataFormats/TrajectorySeed/interface/PropagationDirection.h"
#include "DataFormats/GeometrySurface/interface/PlaneBuilder.h"
#include "DataFormats/GeometrySurface/interface/Cylinder.h"
#include "DataFormats/GeometrySurface/interface/Plane.h"
#include "TrackingTools/GeomPropagators/interface/Propagator.h"
#include "TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h"

//tracks
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

//TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

// Response corr
#include "CondFormats/HcalObjects/interface/HcalRespCorrs.h"
#include "CondFormats/DataRecord/interface/HcalRespCorrsRcd.h"

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


namespace {

  const unsigned l1TableSize = 10;

  // grab names from https://twiki.cern.ch/twiki/bin/view/CMS/GlobalTriggerMenu_L1Menu_CRUZET200805
  const std::string l1TriggerNames[l1TableSize] = {
    "L1_SingleMu3", "L1_SingleMu5", "L1_SingleMu7", "L1_SingleMu10", "L1_SingleMu14",
    "L1_SingleMu20", "L1_SingleMu25", "L1_SingleIsoEG5", "L1_SingleIsoEG8", "L1_SingleIsoEG10"
  };
}

using namespace edm;
using namespace std;
using namespace reco;

HcalProm::HcalProm(const edm::ParameterSet & iConfig) {

  // @@
  fIPDistance = iConfig.getUntrackedParameter < double >("MuonAcceptance", 10000.);
  fAlgoType = iConfig.getUntrackedParameter < int >("MuonSelectionAlgoType", 1);
  fIP_z = iConfig.getUntrackedParameter < double >("IPZ", 20.);
  fIP_r = iConfig.getUntrackedParameter < double >("IPR", 50.);
  fHBThreshold = iConfig.getUntrackedParameter < double >("HBThreshold", 1.);
  top_ihpi_low = iConfig.getUntrackedParameter < int >("topIPHIlow", 15);
  top_ihpi_high = iConfig.getUntrackedParameter < int >("topIPHIhigh", 22);
  bottom_ihpi_low = iConfig.getUntrackedParameter < int >("topIPHIlow", 51);
  bottom_ihpi_high = iConfig.getUntrackedParameter < int >("topIPHIhigh", 58);


  NTotal = 0;
  NAccepted = 0;
  //trigger bit counters
  t1 = 0; 
  t2 = 0; 
  t3 = 0; 
  t4 = 0; 
  t5 = 0; 
  t6 = 0; 
  t7 = 0; 
  // @@
  runBegin = -1;
  evtNo = 0;
  lumibegin = 0;
  lumiend = 0;
  startTime = "Not Avaliable";
  trigDT = 0;
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


HcalProm::~HcalProm() {
}



// ------------ method called to for each event  ------------
void HcalProm::analyze(const edm::Event & iEvent, const edm::EventSetup & iSetup) {
  // old versions time_t a = (iEvent.time().value()) & 0xffffffff;
  time_t a = (iEvent.time().value()) >> 32;
  event = iEvent.id().event();
  run = iEvent.id().run();
  int lumi = iEvent.luminosityBlock();
  if(run==67647&&event>34000000)return;// Mag. went down
  if(run==67838&&event>43500000)return;// Mag. went down
  if(run==67126&&event<150000) return;// bad ped - unstable HCAL?
  if(run==67122)return;// bad ped
  if(run==67124)return;// bad ped

  if (runBegin < 0) {         // parameters for the first event
    startTime = ctime(&a);
    lumibegin = lumiend = lumi;
    runBegin = iEvent.id().run();
    bookHistograms();       // book all histograms
  }

  run = iEvent.id().run();//needed in the tree

  if (lumi < lumibegin)
    lumibegin = lumi;
  if (lumi > lumiend)
    lumiend = lumi;

  cout << " Luminosity Block: " << lumibegin << " Time : " << startTime << endl;


  //tracker
  edm::Handle<reco::TrackCollection> ctfTrackCollectionHandle;
  iEvent.getByLabel("ctfWithMaterialTracksP5", ctfTrackCollectionHandle);
//  iEvent.getByLabel("cosmictrackfinderP5", ctfTrackCollectionHandle);
  const TrackCollection ctftC = *(ctfTrackCollectionHandle.product());

  h_AllTracks->Fill(ctftC.size());
  if(ctftC.size()!=1)return;

  //Cosmic Muons
  Handle<reco::TrackCollection> cosmicmuon;
  iEvent.getByLabel("cosmicMuons",cosmicmuon);
  if (cosmicmuon->size() < 1) return;

  // hcal rechits
  Handle < HBHERecHitCollection > hbhe;
  iEvent.getByLabel("hbhereco", hbhe);
  const HBHERecHitCollection Hithbhe = *(hbhe.product());

  if(Hithbhe.size()<4800) return; // removes ZS data

  const HBHERecHitCollection *HBHERecHits = 0;
  iEvent.getByLabel("hbhereco", hbhe);
  HBHERecHits = hbhe.product();   // get a ptr to the product

  string errMsg("");

  int hasresp=0;
  const HcalRespCorrs* myRecalib;
  try{
  edm::ESHandle <HcalRespCorrs> recalibCorrs;
  iSetup.get<HcalRespCorrsRcd>().get("recalibrate",recalibCorrs);
  myRecalib = recalibCorrs.product();
  hasresp=1;
  }catch(const cms::Exception & e) {
    errMsg = errMsg + "  -- No  recalibrate\n" + e.what();
  }

  // ecal rechits
  Handle < EcalRecHitCollection > ebrechit;
  iEvent.getByLabel("ecalRecHit", "EcalRecHitsEB", ebrechit);
  const EcalRecHitCollection Hiteb = *(ebrechit.product());

  // mag. field 
  edm::ESHandle<MagneticField> theMagField;
  iSetup.get<IdealMagneticFieldRecord>().get(theMagField );


  Handle < reco::BasicClusterCollection > bccHandle;
  iEvent.getByLabel("cosmicBasicClusters", "CosmicBarrelBasicClusters", bccHandle);
  const reco::BasicClusterCollection * clusterCollection_p = 0;

  if (!bccHandle.failedToGet())
    clusterCollection_p = bccHandle.product();


  // geometry
  const CaloGeometry *geo;
  //    const CaloSubdetectorGeometry *geometry_hb;

  ESHandle < CaloGeometry > pG;
  //    iSetup.get < IdealGeometryRecord > ().get(pG);
  iSetup.get<CaloGeometryRecord>().get(pG);
  geo = pG.product();
  //    geometry_hb = geo->getSubdetectorGeometry(DetId::Hcal, HcalBarrel);


  cout << "\nEvent ID = " << iEvent.id() << std::endl;
  ++NTotal;

  // Cosmic Muons
//  Handle < TrackCollection > muons;
//  iEvent.getByLabel("cosmicMuons", muons);
//  const TrackCollection tC = *(muons.product());
//  cout << "Number of cosmic muon tracks in this event: " << tC.size() << endl;

  cout << "Number of cosmic muon tracks in this event: " << cosmicmuon->size() << endl;
  cout<<"HBHERecHits Size:   "<<Hithbhe.size()<<endl;
  //COMM
  // trigger

  Handle < L1MuGMTReadoutCollection > gmtrc_handle;
  iEvent.getByLabel("gtDigis", gmtrc_handle);
  L1MuGMTReadoutCollection const *gmtrc = gmtrc_handle.product();

  Handle < L1GlobalTriggerReadoutRecord > gtrr_handle;
  iEvent.getByLabel("gtDigis", gtrr_handle);
  L1GlobalTriggerReadoutRecord const *gtrr = gtrr_handle.product();


  try {
    iEvent.getByLabel("hbhereco", hbhe);
  } catch(const cms::Exception & e) {
    errMsg = errMsg + "  -- No  hbhereco\n" + e.what();
  }

  if (errMsg != "") {
    errMsg = errMsg + ".";
    std::cout << "% Warning" << errMsg << std::endl;
  }


  //COMM
  bool dt_l1a = false;
  bool rpc_l1a = false;
  bool csc_l1a = false;
  bool hcal_l1a = false;

  vector < L1MuGMTReadoutRecord > gmt_records = gmtrc->getRecords();
  vector < L1MuGMTReadoutRecord >::const_iterator igmtrr;

  for (igmtrr = gmt_records.begin(); igmtrr != gmt_records.end(); igmtrr++) {
  vector < L1MuRegionalCand >::const_iterator iter1;
  vector < L1MuRegionalCand > rmc;

  // DT Trigger
  int idt = 0;

  rmc = igmtrr->getDTBXCands();
  for (iter1 = rmc.begin(); iter1 != rmc.end(); iter1++) {
  if (!(*iter1).empty())
  ++idt;
  }

  if (idt > 0)
  cout << "Found " << idt << " valid DT candidates in bx wrt. L1A = " << igmtrr->getBxInEvent() << endl;
  if (igmtrr->getBxInEvent() == 0 && idt > 0)
  dt_l1a = true;

  // RPCb Trigger
  int irpcb = 0;

  rmc = igmtrr->getBrlRPCCands();
  for (iter1 = rmc.begin(); iter1 != rmc.end(); iter1++) {
  if (!(*iter1).empty())
  ++irpcb;
  }

  if (irpcb > 0)
  cout << "Found " << irpcb << " valid RPC candidates in bx wrt. L1A = " << igmtrr->getBxInEvent() << endl;
  if (igmtrr->getBxInEvent() == 0 && irpcb > 0)
  rpc_l1a = true;

  // CSC Trigger
  int icsc = 0;

  rmc = igmtrr->getCSCCands();
  for (iter1 = rmc.begin(); iter1 != rmc.end(); iter1++) {
  if (!(*iter1).empty())
  ++icsc;
  }
  if (icsc > 0){
  cout << "Found " << icsc << " valid CSC candidates in bx wrt. L1A = " << igmtrr->getBxInEvent() << endl;
  cout << "BX Number   " << igmtrr->getBxNr() << endl;
  }
  if (igmtrr->getBxInEvent() == 0 && icsc > 0)
  csc_l1a = true;

  }
    


  // trigger summary
  DecisionWord gtDecisionWord = gtrr->decisionWord();

  for (size_t iBit = 0; iBit < gtDecisionWord.size(); ++iBit) {
  if (gtDecisionWord[iBit]) {
  h_global_trigger_bit->Fill(iBit);
  // std::string triggerName = iBit < l1TableSize ? l1TriggerNames [iBit] : "Undefined";
  // std::cout << "L1 Accepted: L1 bit " << iBit <<", trigger " << triggerName << std::endl;
  }
  }

  for (int ibx = -1; ibx <= 1; ibx++) {
  bool hcal_top = false;
  bool hcal_bot = false;
  const L1GtPsbWord psb = gtrr->gtPsbWord(0xbb0d, ibx);
  std::vector < int >valid_phi;

  if ((psb.aData(4) & 0x3f) > 1) {
  valid_phi.push_back((psb.aData(4) >> 10) & 0x1f);
  }
  if ((psb.bData(4) & 0x3f) > 1) {
  valid_phi.push_back((psb.bData(4) >> 10) & 0x1f);
  }
  if ((psb.aData(5) & 0x3f) > 1) {
  valid_phi.push_back((psb.aData(5) >> 10) & 0x1f);
  }
  if ((psb.bData(5) & 0x3f) > 1) {
  valid_phi.push_back((psb.bData(5) >> 10) & 0x1f);
  }
  std::vector < int >::const_iterator iphi;

  for (iphi = valid_phi.begin(); iphi != valid_phi.end(); iphi++) {
  std::cout << "Found HCAL mip with phi=" << *iphi << " in bx wrt. L1A = " << ibx << std::endl;
  if (*iphi < 9)
  hcal_top = true;
  if (*iphi > 8)
  hcal_bot = true;
  }
  if (ibx == 0 && hcal_top && hcal_bot)
  hcal_l1a = true;
  }
//  int triggerBit[4] = { 0, 0, 0, 0 };

  TriggerBit[0] = 0;
  TriggerBit[1] = 0;
  TriggerBit[2] = 0;
  TriggerBit[3] = 0;

  // 0 : DT
  // 1 : RPC
  // 2 : CSC
  // 3 : HCAL
/*
  std::cout << "**** Trigger Source ****" << std::endl;
  const Int_t nt = 7;
  char *nameTrig[nt] = {"DT","RPC","CSC","HCAL", "DT_AND_HCAL", "RPC_AND_HCAL", "CSC_AND_HCAL" };
  for(int b=0;b<nt;++b)h_Trigger->GetXaxis()->SetBinLabel(b+1,nameTrig[b]);	
  if (dt_l1a) {
  triggerBit[0] = 1;
  TriggerBit[0] = 1;
  ++t1;
  h_Trigger->SetBinContent(1,t1);
  }
  if (rpc_l1a) {
  triggerBit[1] = 1;
  TriggerBit[1] = 1;
  ++t2;
  std::cout << "RPC" << std::endl;
  h_Trigger->SetBinContent(2,t2);
  }
  if (csc_l1a) {
  triggerBit[2] = 1;
  TriggerBit[2] = 1;
  ++t3;
  std::cout << "CSC" << std::endl;
  h_Trigger->SetBinContent(3,t3);
  }
  if (hcal_l1a) {
  triggerBit[3] = 1;
  TriggerBit[3] = 1;
  ++t4;
  std::cout << "HCAL" << std::endl;
  h_Trigger->SetBinContent(4,t4);
  }
  if (dt_l1a && hcal_l1a) {
  ++t5;
  h_Trigger->SetBinContent(5,t5);
  }
  if (rpc_l1a && hcal_l1a) {
  ++t6;
  h_Trigger->SetBinContent(6,t6);
  }
  if (csc_l1a && hcal_l1a) {
  ++t7;
  h_Trigger->SetBinContent(7,t7);
  }
  std::cout << "************************" << std::endl;

  cout<<"HBHERecHits Size:   "<<Hithbhe.size()<<endl;
*/
  hbheC = Hithbhe.size();

    int isValid= 0;
    double InRadiusHB = 177.7;
    double OutRadiusHB = 287.65;
//cout<<"before ctftrk loop"<<endl;
  for (reco::TrackCollection::const_iterator ctftrk =ctfTrackCollectionHandle->begin(); ctftrk != ctfTrackCollectionHandle->end();++ctftrk){
      tk_mom = ctftrk->p();
      tk_momt = ctftrk->pt();
      tk_ndof = int(ctftrk->ndof());
      if(tk_momt<3.) return;
      if(tk_ndof<3) return;
      tk_chi2 = ctftrk->chi2();
      tk_lost = ctftrk->lost();
      tk_charge = ctftrk->charge();
      tk_d0 = ctftrk->d0();
cout<<"NDOF "<<tk_ndof<<"\t"<<ctftrk->ndof()<<"\t"<<ctftrk->lost()<<endl;
      isValidHBtop=1;
      isValidHBbot=1;
      GlobalPoint gposB( ctftrk->outerPosition().X(), ctftrk->outerPosition().Y(), ctftrk->outerPosition().Z());
      GlobalVector gmomB( ctftrk->outerMomentum().X(), ctftrk->outerMomentum().Y(), ctftrk->outerMomentum().Z());
//cout<<"before ctftrk PropagateF"<<endl;
      isValid=PropagateF( gposB, gmomB ,tk_charge,InRadiusHB,
			 &TK_XinPosHBbot,&TK_YinPosHBbot,&TK_ZinPosHBbot);	
      isValid=PropagateF( gposB, gmomB ,tk_charge,OutRadiusHB,
			 &TK_XoutPosHBbot,&TK_YoutPosHBbot,&TK_ZoutPosHBbot);	
      TK_LengthHBbot=sqrt(pow((TK_XinPosHBbot-TK_XoutPosHBbot),2)+pow((TK_YinPosHBbot-TK_YoutPosHBbot),2)+
           pow((TK_ZinPosHBbot-TK_ZoutPosHBbot),2))/(OutRadiusHB-InRadiusHB);

      isValidHBbot=isValid;
      GlobalPoint  gposT( ctftrk->innerPosition().X(), ctftrk->innerPosition().Y(), ctftrk->innerPosition().Z());
      GlobalVector gmomT( ctftrk->innerMomentum().X(), ctftrk->innerMomentum().Y(), ctftrk->innerMomentum().Z());
//cout<<"before ctftrk PropagateB"<<endl;
      isValid=PropagateB( gposT, gmomT ,tk_charge,InRadiusHB,
			 &TK_XinPosHBtop,&TK_YinPosHBtop,&TK_ZinPosHBtop);	
      isValid=PropagateB( gposT, gmomT ,tk_charge,OutRadiusHB,
			 &TK_XoutPosHBtop,&TK_YoutPosHBtop,&TK_ZoutPosHBtop);	
      TK_LengthHBtop=sqrt(pow((TK_XinPosHBtop-TK_XoutPosHBtop),2)+pow((TK_YinPosHBtop-TK_YoutPosHBtop),2)+
           pow((TK_ZinPosHBtop-TK_ZoutPosHBtop),2))/(OutRadiusHB-InRadiusHB);

      isValidHBtop=isValid;
//cout<<"after ctftrk PropagateF"<<endl;
      GlobalPoint  gposTi(TK_XinPosHBtop,TK_YinPosHBtop,TK_ZinPosHBtop);
      GlobalPoint  gposTo(TK_XoutPosHBtop,TK_YoutPosHBtop,TK_ZoutPosHBtop);
      GlobalPoint  gposBi(TK_XinPosHBbot,TK_YinPosHBbot,TK_ZinPosHBbot);
      GlobalPoint  gposBo(TK_XoutPosHBbot,TK_YoutPosHBbot,TK_ZoutPosHBbot);

      PhiInTowHBtop=gposTi.phi();
      PhiOutTowHBtop=gposTo.phi();
      PhiInTowHBbot=gposBi.phi();
      PhiOutTowHBbot=gposBo.phi();

      EtaInTowHBtop=gposTi.eta();
      EtaOutTowHBtop=gposTo.eta();
      EtaInTowHBbot=gposBi.eta();
      EtaOutTowHBbot=gposBo.eta();
      EtaInTowHBtop+=EtaInTowHBtop/abs(EtaInTowHBtop)*0.087;
      EtaOutTowHBtop+=EtaOutTowHBtop/abs(EtaOutTowHBtop)*0.087;
      EtaInTowHBbot+=EtaInTowHBbot/abs(EtaInTowHBbot)*0.087;
      EtaOutTowHBbot+=EtaOutTowHBbot/abs(EtaOutTowHBbot)*0.087;

      iEtaInTowHBtop=int(EtaInTowHBtop/0.087);
      iEtaOutTowHBtop=int(EtaOutTowHBtop/0.087);
      iEtaInTowHBbot=int(EtaInTowHBbot/0.087);
      iEtaOutTowHBbot=int(EtaOutTowHBbot/0.087);

//cout<<"before ctftrk abs ieta"<<endl;
      double Pi = TMath::Pi();
      if(PhiInTowHBtop<0.)PhiInTowHBtop+=2.*Pi;
      if(PhiOutTowHBtop<0.)PhiOutTowHBtop+=2.*Pi;
      if(PhiInTowHBbot<0.)PhiInTowHBbot+=2.*Pi;
      if(PhiOutTowHBbot<0.)PhiOutTowHBbot+=2.*Pi;
      Pi/=36.;
      iPhiInTowHBtop=int(PhiInTowHBtop/Pi)+1;
      iPhiOutTowHBtop=int(PhiOutTowHBtop/Pi)+1;
      iPhiInTowHBbot=int(PhiInTowHBbot/Pi)+1;
      iPhiOutTowHBbot=int(PhiOutTowHBbot/Pi)+1;
 
      if(iPhiInTowHBtop!=iPhiOutTowHBtop)isValidHBtop=0;
      if(iEtaInTowHBtop*iEtaOutTowHBtop<0)isValidHBtop=0;
      if(iPhiInTowHBbot!=iPhiOutTowHBbot)isValidHBbot=0;
      if(iEtaInTowHBbot*iEtaOutTowHBbot<0)isValidHBbot=0;

  } // end TK loop

//cout<<"after ctftrk loop"<<endl;
//cout<<"before cmTrack loop"<<endl;
cout<<"NDOF1 "<<tk_ndof<<endl;

  // Make matching between Muon Track and HBCal 
  NumDTtracks =0;
  for (reco::TrackCollection::const_iterator cmTrack = cosmicmuon->begin(); cmTrack != cosmicmuon->end(); ++cmTrack) {
      DT_valid[NumDTtracks]=1;
      DT_mom[NumDTtracks] = cmTrack->p();
      DT_momt[NumDTtracks] = cmTrack->pt();
      DT_ndof[NumDTtracks] = int(cmTrack->ndof());
      DT_chi2[NumDTtracks] = cmTrack->chi2();
      DT_lost[NumDTtracks] = cmTrack->lost();
      DT_charge[NumDTtracks] = cmTrack->charge();
      DT_d0[NumDTtracks] = cmTrack->d0();
      DT_XinPos[NumDTtracks] = cmTrack->innerPosition().x();
      DT_YinPos[NumDTtracks] = cmTrack->innerPosition().y();
      DT_ZinPos[NumDTtracks] = cmTrack->innerPosition().z();
      DT_XoutPos[NumDTtracks] = cmTrack->outerPosition().x();
      DT_YoutPos[NumDTtracks] = cmTrack->outerPosition().y();
      DT_ZoutPos[NumDTtracks] = cmTrack->outerPosition().z();
      DT_isTop[NumDTtracks]=0;
      if(DT_YinPos[NumDTtracks]>0&&DT_YoutPos[NumDTtracks]>0)DT_isTop[NumDTtracks]=1;
      if(DT_YinPos[NumDTtracks]*DT_YoutPos[NumDTtracks]<0)DT_isTop[NumDTtracks]=-1;

      DT_XoutPosHB[NumDTtracks]=-99999;
      DT_YoutPosHB[NumDTtracks]=-99999;
      DT_ZoutPosHB[NumDTtracks]=-99999;
      isValid=0;
      if(DT_YinPos[NumDTtracks]>0)
      {
      GlobalPoint gposT( cmTrack->outerPosition().X(), cmTrack->outerPosition().Y(), cmTrack->outerPosition().Z());
      GlobalVector gmomT( cmTrack->outerMomentum().X(), cmTrack->outerMomentum().Y(), cmTrack->outerMomentum().Z());
      isValid=PropagateF( gposT, gmomT ,cmTrack->charge(),OutRadiusHB,
                         &DT_XoutPosHB[NumDTtracks],&DT_YoutPosHB[NumDTtracks],&DT_ZoutPosHB[NumDTtracks]);
      }

      if(DT_YinPos[NumDTtracks]<0)
      {
      GlobalPoint gposB( cmTrack->innerPosition().X(), cmTrack->innerPosition().Y(), cmTrack->innerPosition().Z());
      GlobalVector gmomB( cmTrack->innerMomentum().X(), cmTrack->innerMomentum().Y(), cmTrack->innerMomentum().Z());
      isValid=PropagateB( gposB, gmomB ,cmTrack->charge(),OutRadiusHB,
                         &DT_XoutPosHB[NumDTtracks],&DT_YoutPosHB[NumDTtracks],&DT_ZoutPosHB[NumDTtracks]);
      }
      DT_valid[NumDTtracks]=isValid;
      if(DT_isTop[NumDTtracks]<0)DT_valid[NumDTtracks]=0;

      DT_etaHB[NumDTtracks]=-9999;
      DT_phiHB[NumDTtracks]=-9999;
      DT_ietaHB[NumDTtracks]=-9999;
      DT_iphiHB[NumDTtracks]=-9999;

      if(isValid)
      {
        GlobalPoint gpos(DT_XoutPosHB[NumDTtracks],DT_YoutPosHB[NumDTtracks],DT_ZoutPosHB[NumDTtracks]);

	DT_etaHB[NumDTtracks]=gpos.eta();
	DT_phiHB[NumDTtracks]=gpos.phi();
        DT_etaHB[NumDTtracks]+=DT_etaHB[NumDTtracks]/abs(DT_etaHB[NumDTtracks]);
	DT_ietaHB[NumDTtracks]=int(DT_etaHB[NumDTtracks]/0.087);
        double Pi = TMath::Pi();
	if(DT_phiHB[NumDTtracks]<0.)DT_phiHB[NumDTtracks]+=2.*Pi;
        Pi/=36.;
        DT_iphiHB[NumDTtracks]=int(DT_phiHB[NumDTtracks]/Pi)+1;

      } // end isPropagated

    NumDTtracks++;
  } // end DT loop

//cout<<"after cmTrack loop"<<endl;
//cout<<NumDTtracks<<endl;

cout<<"NDOF2 "<<tk_ndof<<endl;

    for(int i=0;i<30;i++)
    for(int j=0;j<3;j++)
     {
       ETowHBtop[i][j]=0;
       ETowHBbot[i][j]=0;
       ETowHBtopCr[i][j]=0;
       ETowHBbotCr[i][j]=0;
       TTowHBtop[i][j]=-200;
       TTowHBbot[i][j]=-200;
     }
    for(int i=0;i<14;i++)
    for(int j=0;j<72;j++)
    {
      HBpedP[i][j]=-200;
      HBpedM[i][j]=-200;
    }
      int EtaMinTop=iEtaInTowHBtop;
      int EtaMaxTop=iEtaOutTowHBtop;
      int EtaMinBot=iEtaInTowHBbot;
      int EtaMaxBot=iEtaOutTowHBbot;
      if(iEtaInTowHBbot>iEtaOutTowHBbot)
      {
       EtaMinBot=iEtaOutTowHBbot;
       EtaMaxBot=iEtaInTowHBbot;
      } 
      if(iEtaInTowHBtop>iEtaOutTowHBtop)
      {
       EtaMinTop=iEtaOutTowHBtop;
       EtaMaxTop=iEtaInTowHBtop;
      } 

      NTowHBbot=abs(iEtaOutTowHBbot-iEtaInTowHBbot)+3;
      NTowHBtop=abs(iEtaOutTowHBtop-iEtaInTowHBtop)+3;

      EtaMaxTop+=1;
      EtaMaxBot+=1;
      EtaMinTop-=1;
      EtaMinBot-=1;
      if(EtaMinBot==0)EtaMinBot=-1;
      if(EtaMinTop==0)EtaMinTop=-1;
      if(EtaMaxBot==0)EtaMaxBot=1;
      if(EtaMaxTop==0)EtaMaxTop=1;

//      GlobalPoint pos = geo->getPosition((*hhit).detid());
//      ehb = hhit->energy();

//cout<<"before hhit loop"<<endl;
//cout<<NTowHBbot<<"  "<<NTowHBtop<<endl;

    EHBbot=0;
    EHBtop=0;
    EHBbotCr=0;
    EHBtopCr=0;
    THBbot=0;
    THBtop=0;
    
    for (HBHERecHitCollection::const_iterator hhit=Hithbhe.begin(); hhit!=Hithbhe.end(); hhit++) {
      int hieta=hhit->id().ieta();
      int hiphi=hhit->id().iphi();
      if(abs(hieta)>14)continue;

      float icalconst=1.;
      DetId mydetid = hhit->id().rawId();
      if(hasresp)
      icalconst=myRecalib->getValues(mydetid)->getValue();

//cout<<"myRecalib  : "<<hieta<<"\t"<<hiphi<<"\t"<<icalconst<<endl;

      if(hiphi<=37||hiphi==72) //top
       {
         if(isValidHBtop==0)continue;
         if(hieta>0&&abs(iPhiInTowHBtop-hiphi)>3)HBpedP[hieta-1][hiphi-1]=hhit->energy();
         if(hieta<0&&abs(iPhiInTowHBtop-hiphi)>3)HBpedM[-hieta-1][hiphi-1]=hhit->energy();
         if(hiphi==72)hiphi=0;
         if(hieta>EtaMaxTop||hieta<EtaMinTop||abs(iPhiInTowHBtop-hiphi)>1)continue;
         ETowHBtop[hieta-EtaMinTop][hiphi+1-iPhiInTowHBtop]=hhit->energy();
         ETowHBtopCr[hieta-EtaMinTop][hiphi+1-iPhiInTowHBtop]=hhit->energy()*icalconst;
         TTowHBtop[hieta-EtaMinTop][hiphi+1-iPhiInTowHBtop]=hhit->time();
         EHBtop+=hhit->energy();
         EHBtopCr+=hhit->energy()*icalconst;
         THBtop+=hhit->energy()*hhit->time();
       }
      hiphi=hhit->id().iphi();
      if(hiphi>=35||hiphi==1)  // bottom
      {
         if(isValidHBbot==0)continue;
         if(hieta>0&&abs(iPhiInTowHBbot-hiphi)>3)HBpedP[hieta-1][hiphi-1]=hhit->energy();
         if(hieta<0&&abs(iPhiInTowHBbot-hiphi)>3)HBpedM[-hieta-1][hiphi-1]=hhit->energy();
         if(hiphi==1)hiphi=73;
         if(hieta>EtaMaxBot||hieta<EtaMinBot||abs(iPhiInTowHBbot-hiphi)>1)continue;
         ETowHBbot[hieta-EtaMinBot][hiphi+1-iPhiInTowHBbot]=hhit->energy();
         ETowHBbotCr[hieta-EtaMinBot][hiphi+1-iPhiInTowHBbot]=hhit->energy()*icalconst;
         TTowHBbot[hieta-EtaMinBot][hiphi+1-iPhiInTowHBbot]=hhit->time();
         EHBbot+=hhit->energy();
         EHBbotCr+=hhit->energy()*icalconst;
         THBbot+=hhit->energy()*hhit->time();
       }
    }  //end cicle for HB Towers hhit
cout<<"NDOF3 "<<tk_ndof<<endl;

    if(fabs(EHBtop)>0.001)THBtop/=EHBtop;
    if(fabs(EHBbot)>0.001)THBbot/=EHBbot;
//cout<<"after hhit loop"<<endl;
//cout<<NumDTtracks<<endl;
  myTree->Fill();
cout<<"NDOF4 "<<tk_ndof<<endl;
//cout<<"after myTree"<<endl;
  ++evtNo;
}


// ------------ method called once each job just before starting event loop  ------------
void HcalProm::beginJob(const edm::EventSetup & iSetup) {
  edm::ESHandle<MagneticField> bField;
  iSetup.get<IdealMagneticFieldRecord>().get(bField);
//  string PropagationDirection = "alongMomentum" // or "oppositeToMomentum" or "anyDirection"
  stepPropF  = new SteppingHelixPropagator(&*bField,alongMomentum);
  stepPropF->setMaterialMode(false);
  stepPropF->applyRadX0Correction(true);
  stepPropB  = new SteppingHelixPropagator(&*bField,oppositeToMomentum);
  stepPropB->setMaterialMode(false);
  stepPropB->applyRadX0Correction(true);
}

TH1F *HcalProm::book1DHistogram(TFileDirectory & fDir, const std::string & fName, const std::string & fTitle,
				int fNbins, double fXmin, double fXmax) const {
  char title[1024];

  sprintf(title, "%s [RUN:%ld/%ld]", fTitle.c_str(), runBegin, lumibegin);
  std::cout << "booking 1d histogram " << title << std::endl;
  return fDir.make < TH1F > (fName.c_str(), title, fNbins, fXmin, fXmax);
} 
TH2F *HcalProm::book2DHistogram(TFileDirectory & fDir, const std::string & fName, const std::string & fTitle,
				int fNbinsX, double fXmin, double fXmax, int fNbinsY, double fYmin, double fYmax) const {
  char title[1024];

  sprintf(title, "%s [RUN:%ld/%ld]", fTitle.c_str(), runBegin, lumibegin);
  std::cout << "booking 2d histogram " << title << std::endl;
  return fDir.make < TH2F > (fName.c_str(), title, fNbinsX, fXmin, fXmax, fNbinsY, fYmin, fYmax);
} 
TProfile *HcalProm::bookProfileHistogram(TFileDirectory & fDir, const std::string & fName, const std::string & fTitle,
					 int fNbins, double fXmin, double fXmax, double fYmin, double fYmax) const {
  char title[1024];
  sprintf(title, "%s [RUN:%ld/%ld]", fTitle.c_str(), runBegin, lumibegin);
  std::cout << "booking profile histogram " << title << std::endl;
  return fDir.make < TProfile > (fName.c_str(), title, fNbins, fXmin, fXmax, fYmin, fYmax); 
}

void HcalProm::bookHistograms() {

  TFileDirectory HcalDir = fs->mkdir("Hcal");
  TFileDirectory MuonDir = fs->mkdir("Muon");
  TFileDirectory TriggerDir = fs->mkdir("Trigger");
  h_global_trigger_bit =
    book1DHistogram(TriggerDir, "h_global_trigger_bit", "Global Trigger Bit Fired", 128, -0.5, 127.5);

  h_AllTracks = book1DHistogram(MuonDir, "NAllTracks", "Nb. of Reconstructed Thracker Muon Tracks", 10, -0.5, 9.5);

  // ntuple inicialization
  myTree = new TTree("muonHB","muonHB Tree");
  myTree->Branch("run",  &run, "run/I");
  myTree->Branch("event",  &event, "event/I");
  myTree->Branch("hbheC",  &hbheC, "hbheC/I");
  myTree->Branch("TriggerBit",  TriggerBit, "TriggerBit[4]/I");
  myTree->Branch("HBpedP",  HBpedP, "HBpedP[14][72]/F");
  myTree->Branch("HBpedM",  HBpedM, "HBpedM[14][72]/F");

 // Treacker
  myTree->Branch("tk_mom", &tk_mom, "tk_mom/F"); 
  myTree->Branch("tk_momt", &tk_momt, "tk_momt/F"); 
  myTree->Branch("tk_ndof", &tk_ndof, "tk_ndof/I"); 
  myTree->Branch("tk_chi2", &tk_chi2, "tk_chi2/F"); 
  myTree->Branch("tk_lost", &tk_lost, "tk_lost/I"); 
  myTree->Branch("tk_charge", &tk_charge, "tk_charge/I"); 
  myTree->Branch("tk_d0", &tk_d0, "tk_d0/F"); 

 // DT tracks with > 24 Hits
  myTree->Branch("NumDTtracks", &NumDTtracks, "NumDTtracks/I");
  myTree->Branch("DT_valid",  DT_valid, "DT_valid[NumDTtracks]/I"); 
  myTree->Branch("DT_mom",  DT_mom, "DT_mom[NumDTtracks]/F"); 
  myTree->Branch("DT_momt",  DT_momt, "DT_momt[NumDTtracks]/F"); 
  myTree->Branch("DT_ndof",  DT_ndof, "DT_ndof[NumDTtracks]/I"); 
  myTree->Branch("DT_lost",  DT_lost, "DT_lost[NumDTtracks]/I"); 
  myTree->Branch("DT_charge",  DT_charge, "DT_charge[NumDTtracks]/I"); 
  myTree->Branch("DT_chi2",  DT_chi2, "DT_chi2[NumDTtracks]/F"); 
  myTree->Branch("DT_d0",  DT_d0, "DT_d0[NumDTtracks]/F"); 
  myTree->Branch("DT_XinPos", DT_XinPos, "DT_XinPos[NumDTtracks]/F"); 
  myTree->Branch("DT_YinPos", DT_YinPos, "DT_YinPos[NumDTtracks]/F"); 
  myTree->Branch("DT_ZinPos", DT_ZinPos, "DT_ZinPos[NumDTtracks]/F"); 
  myTree->Branch("DT_XoutPos", DT_XoutPos, "DT_XoutPos[NumDTtracks]/F"); 
  myTree->Branch("DT_YoutPos", DT_YoutPos, "DT_YoutPos[NumDTtracks]/F"); 
  myTree->Branch("DT_ZoutPos", DT_ZoutPos, "DT_ZoutPos[NumDTtracks]/F"); 
  myTree->Branch("DT_XoutPosHB", DT_XoutPosHB, "DT_XoutPosHB[NumDTtracks]/F"); 
  myTree->Branch("DT_YoutPosHB", DT_YoutPosHB, "DT_YoutPosHB[NumDTtracks]/F"); 
  myTree->Branch("DT_ZoutPosHB", DT_ZoutPosHB, "DT_ZoutPosHB[NumDTtracks]/F"); 
  myTree->Branch("DT_etaHB", DT_etaHB, "DT_etaHB[NumDTtracks]/F"); 
  myTree->Branch("DT_phiHB", DT_phiHB, "DT_phiHB[NumDTtracks]/F"); 
  myTree->Branch("DT_ietaHB", DT_ietaHB, "DT_ietaHB[NumDTtracks]/I"); 
  myTree->Branch("DT_iphiHB", DT_iphiHB, "DT_iphiHB[NumDTtracks]/I"); 
  myTree->Branch("DT_isTop", DT_isTop, "DT_isTop[NumDTtracks]/I"); 

 // HB info
// HB top
  myTree->Branch("NTowHBtop",&NTowHBtop,"NTowHBtop/I");
  myTree->Branch("isValidHBtop",&isValidHBtop,"isValidHBtop/I");
  myTree->Branch("ETowHBtop",  ETowHBtop, "ETowHBtop[NTowHBtop][3]/F");
  myTree->Branch("ETowHBtopCr",  ETowHBtopCr, "ETowHBtopCr[NTowHBtop][3]/F");
  myTree->Branch("TTowHBtop",  TTowHBtop, "TTowHBtop[NTowHBtop][3]/F");
  myTree->Branch("EHBtop",  &EHBtop, "EHBtop/F");
  myTree->Branch("EHBtopCr",  &EHBtopCr, "EHBtopCr/F");
  myTree->Branch("THBtop",  &THBtop, "THBtop/F");
  myTree->Branch("iPhiInTowHBtop",  &iPhiInTowHBtop, "iPhiInTowHBtop/I");
  myTree->Branch("iPhiOutTowHBtop",  &iPhiOutTowHBtop, "iPhiOutTowHBtop/I");
  myTree->Branch("iEtaInTowHBtop",  &iEtaInTowHBtop, "iEtaInTowHBtop/I");
  myTree->Branch("iEtaOutTowHBtop",  &iEtaOutTowHBtop, "iEtaOutTowHBtop/I");
  myTree->Branch("PhiInTowHBtop",  &PhiInTowHBtop, "PhiInTowHBtop/F");
  myTree->Branch("PhiOutTowHBtop",  &PhiOutTowHBtop, "PhiOutTowHBtop/F");
  myTree->Branch("EtaInTowHBtop",  &EtaInTowHBtop, "EtaInTowHBtop/F");
  myTree->Branch("EtaOutTowHBtop",  &EtaOutTowHBtop, "EtaOutTowHBtop/F");
  myTree->Branch("TK_XinPosHBtop", &TK_XinPosHBtop, "TK_XinPosHBtop/F"); 
  myTree->Branch("TK_YinPosHBtop", &TK_YinPosHBtop, "TK_YinPosHBtop/F"); 
  myTree->Branch("TK_ZinPosHBtop", &TK_ZinPosHBtop, "TK_ZinPosHBtop/F"); 
  myTree->Branch("TK_XoutPosHBtop", &TK_XoutPosHBtop, "TK_XoutPosHBtop/F"); 
  myTree->Branch("TK_YoutPosHBtop", &TK_YoutPosHBtop, "TK_YoutPosHBtop/F"); 
  myTree->Branch("TK_ZoutPosHBtop", &TK_ZoutPosHBtop, "TK_ZoutPosHBtop/F"); 
  myTree->Branch("TK_LengthHBtop", &TK_LengthHBtop, "TK_LengthHBtop/F"); 

// HB Bottom
  myTree->Branch("NTowHBbot",&NTowHBbot,"NTowHBbot/I");
  myTree->Branch("isValidHBbot",&isValidHBbot,"isValidHBbot/I");
  myTree->Branch("ETowHBbot",  ETowHBbot, "ETowHBbot[NTowHBbot][3]/F");
  myTree->Branch("ETowHBbotCr",  ETowHBbotCr, "ETowHBbotCr[NTowHBbot][3]/F");
  myTree->Branch("TTowHBbot",  TTowHBbot, "TTowHBbot[NTowHBbot][3]/F");
  myTree->Branch("EHBbot",  &EHBbot, "EHBbot/F");
  myTree->Branch("EHBbotCr",  &EHBbotCr, "EHBbotCr/F");
  myTree->Branch("THBbot",  &THBbot, "THBbot/F");
  myTree->Branch("iPhiInTowHBbot",  &iPhiInTowHBbot, "iPhiInTowHBbot/I");
  myTree->Branch("iPhiOutTowHBbot",  &iPhiOutTowHBbot, "iPhiOutTowHBbot/I");
  myTree->Branch("iEtaInTowHBbot",  &iEtaInTowHBbot, "iEtaInTowHBbot/I");
  myTree->Branch("iEtaOutTowHBbot",  &iEtaOutTowHBbot, "iEtaOutTowHBbot/I");
  myTree->Branch("PhiInTowHBbot",  &PhiInTowHBbot, "PhiInTowHBbot/F");
  myTree->Branch("PhiOutTowHBbot",  &PhiOutTowHBbot, "PhiOutTowHBbot/F");
  myTree->Branch("EtaInTowHBbot",  &EtaInTowHBbot, "EtaInTowHBbot/F");
  myTree->Branch("EtaOutTowHBbot",  &EtaOutTowHBbot, "EtaOutTowHBbot/F");
  myTree->Branch("TK_XinPosHBbot", &TK_XinPosHBbot, "TK_XinPosHBbot/F");
  myTree->Branch("TK_YinPosHBbot", &TK_YinPosHBbot, "TK_YinPosHBbot/F");
  myTree->Branch("TK_ZinPosHBbot", &TK_ZinPosHBbot, "TK_ZinPosHBbot/F");
  myTree->Branch("TK_XoutPosHBbot", &TK_XoutPosHBbot, "TK_XoutPosHBbot/F");
  myTree->Branch("TK_YoutPosHBbot", &TK_YoutPosHBbot, "TK_YoutPosHBbot/F");
  myTree->Branch("TK_ZoutPosHBbot", &TK_ZoutPosHBbot, "TK_ZoutPosHBbot/F");
  myTree->Branch("TK_LengthHBbot", &TK_LengthHBbot, "TK_LengthHBbot/F"); 



  // @@
  h_ct1 = book1DHistogram(MuonDir, "ct1", "Found good track in zHB and IPxy  ", 2, -0.5, 1.5);
  h_ct2 = book1DHistogram(MuonDir, "ct2", "1x3 Condition is satisfied (same iphi, deta < 2tow) ", 2, -0.5, 1.5);
  h_ct3 = book1DHistogram(MuonDir, "ct3", "Found closest HB cell  ", 2, -0.5, 1.5);
  h_ct4 = book1DHistogram(MuonDir, "ct4", " 2 tracks with Nrec hits>24", 2, -0.5, 1.5);
  h_ct5 = book1DHistogram(MuonDir, "ct5", " ", 2, -0.5, 1.5);
  h_ct6 = book1DHistogram(MuonDir, "ct6", " ", 2, -0.5, 1.5);
  h_ct7 = book1DHistogram(MuonDir, "ct7", " ", 2, -0.5, 1.5);
  h_ct8 = book1DHistogram(MuonDir, "ct8", " ", 2, -0.5, 1.5);
  h_ct9 = book1DHistogram(MuonDir, "ct9", " ", 2, -0.5, 1.5);


//  char name[1024];
//  char title[1024];
//  char *trig[4] = { "DT", "RPC", "CSC", "HCAL" };

}

// ------------ method called once each job just after ending the event loop  ------------
void HcalProm::endJob() {

  cout << "End of job!" << endl;
}

// Propagation

bool HcalProm::PropagateF(
			 // inputs
			 //  double ox, double oy, double oz, double px, double py, double pz, double ra,
			 GlobalPoint pos, GlobalVector mom, int charge, double ra, 
			 // outputs
			 Float_t *x_HB, Float_t *y_HB, Float_t *z_HB){
  bool isValid = 0;
  *x_HB=-10000.;
  *y_HB=-10000.;
  *z_HB=-10000.;


  const FreeTrajectoryState *freetrajectorystate_ =new FreeTrajectoryState(pos, mom ,charge , &(*theMagField));

  Cylinder *cylinder = new Cylinder(Surface::PositionType(0,0,0),
				    Surface::RotationType(), ra);

  TrajectoryStateOnSurface steppingHelixstateinfo_ = stepPropF->propagate(*freetrajectorystate_, (*cylinder));

  isValid = steppingHelixstateinfo_.isValid();
  if (steppingHelixstateinfo_.isValid()) {
    *x_HB=steppingHelixstateinfo_.freeState()->position().x();        
    *y_HB=steppingHelixstateinfo_.freeState()->position().y();        
    *z_HB=steppingHelixstateinfo_.freeState()->position().z();        
  }
  return isValid;
}


bool HcalProm::PropagateB(
			 // inputs
			 //  double ox, double oy, double oz, double px, double py, double pz, double ra,
			 GlobalPoint pos, GlobalVector mom, int charge, double ra,
			 // outputs
			 Float_t *x_HB, Float_t *y_HB, Float_t *z_HB){
  bool isValid = 0;
  *x_HB=-10000.;
  *y_HB=-10000.;
  *z_HB=-10000.;


  const FreeTrajectoryState *freetrajectorystate_ =new FreeTrajectoryState(pos, mom ,charge , &(*theMagField));

  Cylinder *cylinder = new Cylinder(Surface::PositionType(0,0,0),
				    Surface::RotationType(), ra);

  TrajectoryStateOnSurface steppingHelixstateinfo_ = stepPropB->propagate(*freetrajectorystate_, (*cylinder));

  isValid = steppingHelixstateinfo_.isValid();
  if (steppingHelixstateinfo_.isValid()) {
    *x_HB=steppingHelixstateinfo_.freeState()->position().x();        
    *y_HB=steppingHelixstateinfo_.freeState()->position().y();        
    *z_HB=steppingHelixstateinfo_.freeState()->position().z();        
  }
  return isValid;
}


