// -*- C++ -*-
//
// Package:    HcalPromBremss
// Class:      HcalPromBremss
// 
/**\class HcalPromBremss HcalPromBremss.cc mytest/HcalPromBremss/src/HcalPromBremss.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Francesco SANTANASTASIO
//         Created:  Thu Jan 29 14:38:17 CET 2009
// $Id: HcalPromBremss.cc,v 1.3 2009/02/26 13:52:13 santanas Exp $
//
//

// const int MAXnTRIGGER=300; 
// const int MAXnMUONdt=50; 
// const int MAXnRecHitDTMu=50; 
// const int MAXnTRACK=50; 


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
#include "Geometry/HcalTowerAlgo/interface/HcalGeometry.h"


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

// track propagation
#include "DataFormats/TrajectorySeed/interface/TrajectorySeedCollection.h"
#include "DataFormats/TrajectorySeed/interface/PropagationDirection.h"
#include "DataFormats/GeometrySurface/interface/PlaneBuilder.h"
#include "DataFormats/GeometrySurface/interface/Cylinder.h"
#include "DataFormats/GeometrySurface/interface/Plane.h"
#include "TrackingTools/GeomPropagators/interface/Propagator.h"
#include "TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h"
#include "TrackPropagation/SteppingHelixPropagator/interface/SteppingHelixPropagator.h"

//propagation
#include "FastSimulation/BaseParticlePropagator/interface/BaseParticlePropagator.h"
#include "FastSimulation/CalorimeterProperties/interface/Calorimeter.h"
#include "FastSimulation/CaloGeometryTools/interface/CaloGeometryHelper.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "TrackPropagation/SteppingHelixPropagator/interface/SteppingHelixPropagator.h"

//tracks
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

//cell navigation
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"

//TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

//ROOT include
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
#include "TTree.h"
#include "TVector3.h"

//C++ include
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <sys/time.h>

//This code
#include "HcalPromBremss.h"

//
// constants, enums and typedefs
//

using namespace edm;
using namespace std;
using namespace reco;

double DeltaEtaTower = 0.087;
double DeltaPhiTower = 0.087; 

//HCAL barrel radii (cm)
double InRadiusHB = 177.7;
double OutRadiusHB = 287.65;

//
// static data member definitions
//


//
// constructors and destructor
//
HcalPromBremss::HcalPromBremss(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed

  startTime = "Not Avaliable";
  lumi = -1;
  event = -1;
  run = -1;
  NTotal = 0;

  L1TrgCandDecision[0]=false;
  L1TrgCandDecision[1]=false;
  L1TrgCandDecision[2]=false;
  L1TrgCandDecision[3]=false;

  nTriggers=-1;

  nL1DTCandL1ABx=-99;
  nL1DTCandL1ApBx=-99;
  nL1DTCandL1AnBx=-99;
  
  nL1RPCbCandL1ABx=-99;
  nL1RPCbCandL1ApBx=-99;
  nL1RPCbCandL1AnBx=-99;
  
  nL1CSCCandL1ABx=-99;
  nL1CSCCandL1ApBx=-99;
  nL1CSCCandL1AnBx=-99;


  // parameters from config file
  printOut = iConfig.getUntrackedParameter < bool > ("printOut", true);
  rootfile_ = iConfig.getUntrackedParameter<std::string>("rootfileName","muonTree.root");

}


HcalPromBremss::~HcalPromBremss()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
HcalPromBremss::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  
#ifdef THIS_IS_AN_EVENT_EXAMPLE
  Handle<ExampleData> pIn;
  iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
  ESHandle<SetupData> pSetup;
  iSetup.get<SetupRecord>().get(pSetup);
#endif

  //cout << "New event" << endl;

  //Start here your code
   
  //#### Start time, LuminosityBlock, Run, EventN(?)
  time_t a = (iEvent.time().value()) >> 32;
  startTime = ctime(&a);
  event = iEvent.id().event();
  run = iEvent.id().run();
  lumi = iEvent.luminosityBlock();

  if (printOut)
    cout << "event: " << event 
	 << " run: "  << run 
	 << " lumi: " << lumi
	 << endl;

  //#### Define collections

  //-- Rechits, digis and calotowers

  //HBHE Rechits
  Handle < HBHERecHitCollection > hbhe;
  iEvent.getByLabel("hbhereco", hbhe);
  const HBHERecHitCollection Hithbhe = *(hbhe.product());
  const HBHERecHitCollection *HBHERecHits = 0;
  iEvent.getByLabel("hbhereco", hbhe);
  HBHERecHits = hbhe.product();   

  //HF Rechits
  Handle < HFRecHitCollection > hfrh;
  iEvent.getByLabel("hfreco", hfrh);
  const HFRecHitCollection Hithf = *(hfrh.product());
  const HFRecHitCollection *HFHFhits = 0;
  HFHFhits = hfrh.product();

  //HO Rechits
  Handle < HORecHitCollection > horh;
  iEvent.getByLabel("horeco", horh);
  const HORecHitCollection Hitho = *(horh.product());

  //HBHE digis
  Handle < HBHEDigiCollection > hbhe_digi;
  // iEvent.getByLabel("hcalZeroSuppressedDigis",hbhe_digi);
  iEvent.getByLabel("hcalDigis", hbhe_digi);

  //HF digis
  Handle < HFDigiCollection > hf_digi;
  iEvent.getByLabel("hcalDigis", hf_digi);

  //Ecal rechits
  Handle < EcalRecHitCollection > ebrechit;
  iEvent.getByLabel("ecalRecHit", "EcalRecHitsEB", ebrechit);
  const EcalRecHitCollection Hiteb = *(ebrechit.product());

  //Ecal clusters
  //   Handle<reco::BasicClusterCollection> pIslandBarrelBasicClusters;
  //   iEvent.getByLabel("islandBasicClusters","islandBarrelBasicClusters",pIslandBarrelBasicClusters); 
  //   const BasicClusterCollection islandBarrelBasicClusters = *(pIslandBarrelBasicClusters.product()); 

  //Calo towers
  Handle < CaloTowerCollection > calo;
  iEvent.getByLabel("towerMaker", calo);
  const CaloTowerCollection calohbhe = *(calo.product());

  // CosmicBasicClusters
  Handle < reco::BasicClusterCollection > bccHandle;
  iEvent.getByLabel("cosmicBasicClusters", "CosmicBarrelBasicClusters", bccHandle);
  const reco::BasicClusterCollection * clusterCollection_p = 0;
  if (!bccHandle.failedToGet())
    clusterCollection_p = bccHandle.product();
  
  //-- High level physics objects

//   //Missing Et
//   const CaloMET *calomet;
//   Handle < CaloMETCollection > recmet;
//   iEvent.getByLabel("met", recmet);
//   const CaloMETCollection *metCol = recmet.product();
//   calomet = &(metCol->front());

  // MET
  Handle<CaloMETCollection> recoMETColl;
  iEvent.getByLabel ( "met" , recoMETColl);
  const reco::CaloMET & recomet = (*recoMETColl)[0];

  //Reco jets
//   Handle < CaloJetCollection > caloJet;
//   iEvent.getByLabel("iterativeCone15CaloJets", caloJet);
//   const CaloJetCollection cjet = *(caloJet.product());

  //Reco jets
  edm::Handle<reco::CaloJetCollection> caloJetsIC5_raw;
  iEvent.getByLabel ("iterativeCone5CaloJets", caloJetsIC5_raw); 

  //Tracker
  edm::Handle<reco::TrackCollection> ctfTrackCollectionHandle;
  iEvent.getByLabel("ctfWithMaterialTracksP5", ctfTrackCollectionHandle);
  const TrackCollection ctftC = *(ctfTrackCollectionHandle.product());

  //Cosmic Muons
  Handle<TrackCollection> cosmicmuon;
  iEvent.getByLabel("cosmicMuons",cosmicmuon);
  const TrackCollection tC = *(cosmicmuon.product());

  //-- Others

  //Trigger
  Handle < L1MuGMTReadoutCollection > gmtrc_handle;
  iEvent.getByLabel("gtDigis", gmtrc_handle);
  L1MuGMTReadoutCollection const *gmtrc = gmtrc_handle.product();

  Handle < L1GlobalTriggerReadoutRecord > gtrr_handle;
  iEvent.getByLabel("gtDigis", gtrr_handle);
  L1GlobalTriggerReadoutRecord const *gtrr = gtrr_handle.product();

  //Mag. field 
  edm::ESHandle<MagneticField> theMagField;
  iSetup.get<IdealMagneticFieldRecord>().get(theMagField );

  //Geometry
  const CaloGeometry *geo;
  //    const CaloSubdetectorGeometry *geometry_hb;
  ESHandle < CaloGeometry > pG;
  //    iSetup.get < IdealGeometryRecord > ().get(pG);
  iSetup.get<CaloGeometryRecord>().get(pG);
  geo = pG.product();
  //    geometry_hb = geo->getSubdetectorGeometry(DetId::Hcal, HcalBarrel);


  //## Incrementing number of events
  NTotal++;

  //## Some print out
  if(printOut)
    {
      cout << "Ntotal is now: " << NTotal << endl;
      cout << "Number of cosmic muon tracks in this event: " << tC.size() << endl;
      cout << "Number of tracker tracks in this event: " << ctfTrackCollectionHandle->size() << endl;
    }

  //## Trigger bit definition

  //-- global muon triggers
  //L1A BX
  bool dt_l1a = false;
  bool rpc_l1a = false;
  bool csc_l1a = false;

  int Ndt_l1a = 0;
  int Nrpc_l1a = 0;
  int Ncsc_l1a = 0;

  //previous BX
  bool dt_l1a_pBx = false;
  bool rpc_l1a_pBx = false;
  bool csc_l1a_pBx = false;

  int Ndt_l1a_pBx = 0;
  int Nrpc_l1a_pBx = 0;
  int Ncsc_l1a_pBx = 0;

  //next BX
  bool dt_l1a_nBx = false;
  bool rpc_l1a_nBx = false;
  bool csc_l1a_nBx = false;

  int Ndt_l1a_nBx = 0;
  int Nrpc_l1a_nBx = 0;
  int Ncsc_l1a_nBx = 0;

  //-- hcal triggers
  bool hcal_l1a = false;

  //-- loop over the L1 info
  vector < L1MuGMTReadoutRecord > gmt_records = gmtrc->getRecords();
  vector < L1MuGMTReadoutRecord >::const_iterator igmtrr;

  //cout << "L1MuGMTReadoutRecord size(): " << gmt_records.size() << endl;

  for (igmtrr = gmt_records.begin(); igmtrr != gmt_records.end(); igmtrr++) {
    
    //     cout << endl;
    //     cout << "NEXT L1MuGMTReadoutRecord RECORD" << endl;
    //     cout << "L1MuGMTReadoutRecord getBxInEvent(): "  << igmtrr->getBxInEvent() << endl;
    
    vector < L1MuRegionalCand >::const_iterator iter1;
    vector < L1MuRegionalCand > rmc;
    
    //DT Trigger
    int idt = 0;
    rmc = igmtrr->getDTBXCands();
    //cout << "L1MuGMTReadoutRecord getDTBXCands().size(): " << rmc.size() << endl;
    for (iter1 = rmc.begin(); iter1 != rmc.end(); iter1++) {
      if (!(*iter1).empty())
	++idt;
    }
    if (idt > 0 && printOut)
      {
	cout << "Found " << idt << " valid DT candidates in bx wrt. L1A = " << igmtrr->getBxInEvent() << endl;
	cout << "BX Number   " << igmtrr->getBxNr() << endl;
      }
    if (igmtrr->getBxInEvent() == -1 && idt > 0) 
      {
	dt_l1a_pBx = true;
	Ndt_l1a_pBx = idt;
      }
    if (igmtrr->getBxInEvent() == 0 && idt > 0) 
      {
	dt_l1a = true;
	Ndt_l1a = idt;
      }
    if (igmtrr->getBxInEvent() == 1 && idt > 0) 
      {
	dt_l1a_nBx = true;
	Ndt_l1a_nBx = idt;
      }
    
    //RPCb Trigger
    int irpcb = 0;
    rmc = igmtrr->getBrlRPCCands();
    //cout << "L1MuGMTReadoutRecord getBrlRPCCands().size(): " << rmc.size() << endl;
    for (iter1 = rmc.begin(); iter1 != rmc.end(); iter1++) {
      if (!(*iter1).empty())
	++irpcb;
    }
    if (irpcb > 0 && printOut)
      {
	cout << "Found " << irpcb << " valid RPC candidates in bx wrt. L1A = " << igmtrr->getBxInEvent() << endl;
	cout << "BX Number   " << igmtrr->getBxNr() << endl;
      }
    if (igmtrr->getBxInEvent() == -1 && irpcb > 0) 
      {
	rpc_l1a_pBx = true;
	Nrpc_l1a_pBx = irpcb;
      }
    if (igmtrr->getBxInEvent() == 0 && irpcb > 0) 
      {
	rpc_l1a = true;
	Nrpc_l1a = irpcb;
      }
    if (igmtrr->getBxInEvent() == 1 && irpcb > 0) 
      {
	rpc_l1a_nBx = true;
	Nrpc_l1a_nBx = irpcb;
      }

    //CSC Trigger
    int icsc = 0;
    rmc = igmtrr->getCSCCands();
    //cout << "L1MuGMTReadoutRecord getCSCCands().size(): " << rmc.size() << endl;
    for (iter1 = rmc.begin(); iter1 != rmc.end(); iter1++) {
      if (!(*iter1).empty())
	++icsc;
    }
    if (icsc > 0 && printOut){
      cout << "Found " << icsc << " valid CSC candidates in bx wrt. L1A = " << igmtrr->getBxInEvent() << endl;
      cout << "BX Number   " << igmtrr->getBxNr() << endl;
    }
    if (igmtrr->getBxInEvent() == -1 && icsc > 0) 
      {
	csc_l1a_pBx = true;
	Ncsc_l1a_pBx = icsc;
      }
    if (igmtrr->getBxInEvent() == 0 && icsc > 0) 
      {
	csc_l1a = true;
	Ncsc_l1a = icsc;
      }
    if (igmtrr->getBxInEvent() == 1 && icsc > 0) 
      {
	csc_l1a_nBx = true;
	Ncsc_l1a_nBx = icsc;
      }
 
  }//end loop over L1MuGMTReadoutRecord

  //HCAL
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
      if(printOut)
	std::cout << "Found HCAL mip with phi=" << *iphi << " in bx wrt. L1A = " << ibx << std::endl;
      if (*iphi < 9)
	hcal_top = true;
      if (*iphi > 8)
	hcal_bot = true;
    }
    if (ibx == 0 && hcal_top && hcal_bot)
      hcal_l1a = true;
  }

  //GT
  DecisionWord gtDecisionWord = gtrr->decisionWord();
  nTriggers = gtDecisionWord.size();
  //cout << "nTriggers: " << nTriggers << endl;
  
  for (size_t iBit = 0; iBit < gtDecisionWord.size(); ++iBit) {
    //cout << iBit << " " << gtDecisionWord[iBit] << endl;
    GTDecision[iBit] = gtDecisionWord[iBit];
    //     if (gtDecisionWord[iBit]){
    //       std::cout << "L1 Accepted: L1 bit " << iBit << std::endl;
    //     }
  }
  
  //-- Now fill rootTuple variables with trigger info defined above
  L1TrgCandDecision[0]=dt_l1a;
  L1TrgCandDecision[1]=rpc_l1a;
  L1TrgCandDecision[2]=csc_l1a;
  L1TrgCandDecision[3]=hcal_l1a;

  nL1DTCandL1ABx=Ndt_l1a;
  nL1DTCandL1ApBx=Ndt_l1a_pBx;
  nL1DTCandL1AnBx=Ndt_l1a_nBx;
  
  nL1RPCbCandL1ABx=Nrpc_l1a;
  nL1RPCbCandL1ApBx=Nrpc_l1a_pBx;
  nL1RPCbCandL1AnBx=Nrpc_l1a_nBx;
  
  nL1CSCCandL1ABx=Ncsc_l1a;
  nL1CSCCandL1ApBx=Ncsc_l1a_pBx;
  nL1CSCCandL1AnBx=Ncsc_l1a_nBx;


  
  //## muon info for DT and tracker, matching DT-Tracker, find intersections with HCAL barrel

  //-- loop over cosmic muons
  muonDTCount = 0;
  //???? see how this collection is defined
  for (reco::TrackCollection::const_iterator cmTrack = cosmicmuon->begin(); 
       cmTrack != cosmicmuon->end(); 
       ++cmTrack) 
    {

      if(muonDTCount>=MAXnMUONdt)
	break;

      double XinPosMuon;
      double YinPosMuon;
      double ZinPosMuon;
      
      double XoutPosMuon;
      double YoutPosMuon;
      double ZoutPosMuon;

      // this is inner and outer position of muon in Muon Detector in cm
      XinPosMuon = cmTrack->innerPosition().x();
      YinPosMuon = cmTrack->innerPosition().y();
      ZinPosMuon = cmTrack->innerPosition().z();
      XoutPosMuon = cmTrack->outerPosition().x();
      YoutPosMuon = cmTrack->outerPosition().y();
      ZoutPosMuon = cmTrack->outerPosition().z();

      muonDTXin[muonDTCount] = XinPosMuon;
      muonDTYin[muonDTCount] = YinPosMuon;
      muonDTZin[muonDTCount] = ZinPosMuon;
      muonDTXout[muonDTCount] = XoutPosMuon;
      muonDTYout[muonDTCount] = YoutPosMuon;
      muonDTZout[muonDTCount] = ZoutPosMuon;

      muonDTNumRecHits[muonDTCount] =  cmTrack->recHitsSize(); 
      muonDTNumValidRecHits[muonDTCount] =  cmTrack->numberOfValidHits(); 

      muonDTP[muonDTCount] = cmTrack->p();
      muonDTPt[muonDTCount] = cmTrack->pt();
      muonDTCharge[muonDTCount] = cmTrack->charge();


      
      
      //## matching DT-TRACKER
      float MindistAtHBout = 999999.;
      float nearestDXYAtHBout = -999.;
      float nearestDZAtHBout = -999.;
      int nearestTrkIdxAtHBout = -1;

      bool isValidMatchCand=false;

      int trackCounter=0;
      for (reco::TrackCollection::const_iterator ctftrk =ctfTrackCollectionHandle->begin(); 
	   ctftrk != ctfTrackCollectionHandle->end();
	   ++ctftrk)
	{

	  float XOutMuonHBdt=-999;
	  float YOutMuonHBdt=-999;
	  float ZOutMuonHBdt=-999;
	  float XInMuonHBdt=-999;
	  float YInMuonHBdt=-999;
	  float ZInMuonHBdt=-999;
	  float XOutMuonHBtrk=-999;
	  float YOutMuonHBtrk=-999;
	  float ZOutMuonHBtrk=-999;
	  float XInMuonHBtrk=-999;
	  float YInMuonHBtrk=-999;
	  float ZInMuonHBtrk=-999;
	  
	  bool isValidF=false; //propagation is valid??
	  bool isValidFin=false; //propagation is valid??
	  bool isValidFout=false; //propagation is valid??

	  bool isValidB=false; //propagation is valid??
	  bool isValidBin=false; //propagation is valid??
	  bool isValidBout=false; //propagation is valid??

	  if(cmTrack->innerPosition().Y()<0) { //DT track at BOTTOM

	    //-- propagate forward track to HB barrel in & out
	    int charge = ctftrk->charge();
	    GlobalPoint gpos( ctftrk->outerPosition().X(), ctftrk->outerPosition().Y(), ctftrk->outerPosition().Z());
	    GlobalVector gmom( ctftrk->outerMomentum().X(), ctftrk->outerMomentum().Y(), ctftrk->outerMomentum().Z());
	    isValidFout=PropagateF( gpos, gmom ,charge,OutRadiusHB,
				    &XOutMuonHBtrk,&YOutMuonHBtrk,&ZOutMuonHBtrk);
	    isValidFin=PropagateF( gpos, gmom ,charge,InRadiusHB,
				   &XInMuonHBtrk,&YInMuonHBtrk,&ZInMuonHBtrk);
	    if(isValidFin==true && isValidFout==true)
	      isValidF=true;

	    if (printOut){
	      cout << "--> DT track at BOTTOM" << endl;
	      cout << "isValid propF of ctftrk: " << isValidF << endl;
	      float currentDistanceXYfin = sqrt( pow(XInMuonHBtrk,2) + pow(YInMuonHBtrk,2));
	      float currentDistanceXYfout = sqrt( pow(XOutMuonHBtrk,2) + pow(YOutMuonHBtrk,2));
	      cout << "currentDistanceXYfin propF: " << currentDistanceXYfin << endl;
	      cout << "currentDistanceXYfout propF: " << currentDistanceXYfout << endl;
	    }
	    
	    //-- propagate backward DT to HB barrel in & out
	    charge =cmTrack->charge();
	    GlobalPoint  gmpos( cmTrack->innerPosition().X(), cmTrack->innerPosition().Y(), cmTrack->innerPosition().Z());
	    GlobalVector gmmom( cmTrack->innerMomentum().X(), cmTrack->innerMomentum().Y(), cmTrack->innerMomentum().Z());
	    isValidBout=PropagateB( gmpos, gmmom ,charge,OutRadiusHB,
				    &XOutMuonHBdt,&YOutMuonHBdt,&ZOutMuonHBdt);	
	    isValidBin=PropagateB( gmpos, gmmom ,charge,InRadiusHB,
				   &XInMuonHBdt,&YInMuonHBdt,&ZInMuonHBdt);	

	    if(isValidBin==true && isValidBout==true)
	      isValidB=true;

	    if (printOut){
	      cout << "isValid propB of cmTrack: " << isValidB << endl;
	      float currentDistanceXYbin = sqrt( pow(XInMuonHBdt,2) + pow(YInMuonHBdt,2));
	      float currentDistanceXYbout = sqrt( pow(XOutMuonHBdt,2) + pow(YOutMuonHBdt,2));
	      cout << "currentDistanceXYbin propB: " << currentDistanceXYbin << endl;
	      cout << "currentDistanceXYbout propB: " << currentDistanceXYbout << endl;
	      cout << endl;
	    }

	  }else{ //DT track at TOP
	    
	    //-- propagate backward track to HB barrel in & out
	    int charge = ctftrk->charge();
	    GlobalPoint  gpos( ctftrk->innerPosition().X(), ctftrk->innerPosition().Y(), ctftrk->innerPosition().Z());
	    GlobalVector gmom( ctftrk->innerMomentum().X(), ctftrk->innerMomentum().Y(), ctftrk->innerMomentum().Z());
	    isValidBout=PropagateB( gpos, gmom ,charge,OutRadiusHB,
				    &XOutMuonHBtrk,&YOutMuonHBtrk,&ZOutMuonHBtrk);
	    isValidBin=PropagateB( gpos, gmom ,charge,InRadiusHB,
				   &XInMuonHBtrk,&YInMuonHBtrk,&ZInMuonHBtrk);
	    if(isValidBin==true && isValidBout==true)
	      isValidB=true;

	    if (printOut){
	      cout << "--> DT track at TOP" << endl;
	      cout << "isValid propB of ctftrk: " << isValidB << endl;
	      float currentDistanceXYbin = sqrt( pow(XInMuonHBtrk,2) + pow(YInMuonHBtrk,2));
	      float currentDistanceXYbout = sqrt( pow(XOutMuonHBtrk,2) + pow(YOutMuonHBtrk,2));
	      cout << "currentDistanceXYbin propB: " << currentDistanceXYbin << endl;
	      cout << "currentDistanceXYbout propB: " << currentDistanceXYbout << endl;
	    }
	    
	    //-- propagate forward dt to HB barrel in & out
	    charge =cmTrack->charge();
	    GlobalPoint gmpos( cmTrack->outerPosition().X(), cmTrack->outerPosition().Y(), cmTrack->outerPosition().Z());
	    GlobalVector gmmom( cmTrack->outerMomentum().X(), cmTrack->outerMomentum().Y(), cmTrack->outerMomentum().Z());
	    isValidFout=PropagateF( gmpos, gmmom ,charge,OutRadiusHB,
				    &XOutMuonHBdt,&YOutMuonHBdt,&ZOutMuonHBdt);	
	    isValidFin=PropagateF( gmpos, gmmom ,charge,InRadiusHB,
				   &XInMuonHBdt,&YInMuonHBdt,&ZInMuonHBdt);	
	    if(isValidFin==true && isValidFout==true)
	      isValidF=true;

	    if (printOut){
	      cout << "isValid propF of cmTrack: " << isValidF << endl;
	      float currentDistanceXYfin = sqrt( pow(XInMuonHBdt,2) + pow(YInMuonHBdt,2));
	      float currentDistanceXYfout = sqrt( pow(XOutMuonHBdt,2) + pow(YOutMuonHBdt,2));
	      cout << "currentDistanceXYfin propF: " << currentDistanceXYfin << endl;
	      cout << "currentDistanceXYfout propF: " << currentDistanceXYfout << endl;
	      cout << endl;
	    }

	  }//end if/else to select top or bottom dt muons

	  //skip track if any of the two propagations is failed
	  if(isValidF==false || isValidB==false)
	    {
	      //next track
	      trackCounter++;
	      continue;
	    }
	  
	  //-- calculate "nearest" track from this dt muon
	  float distAtHBout =  
	    pow(XOutMuonHBtrk - XOutMuonHBdt,2) +
	    pow(YOutMuonHBtrk - YOutMuonHBdt,2) +
	    pow(ZOutMuonHBtrk - ZOutMuonHBdt,2) ;

	  distAtHBout = sqrt(distAtHBout);
	  
	  if (printOut)
	    cout  << "distAtHBout: " << distAtHBout << endl;

	  if( distAtHBout < MindistAtHBout)
	    {
	      //found a track candidate for matching with dt muon
	      if(isValidF==true && isValidB==true)
		isValidMatchCand=true;

	      MindistAtHBout = distAtHBout; 
	      nearestDZAtHBout = fabs(ZOutMuonHBtrk - ZOutMuonHBdt);
	      nearestDXYAtHBout = sqrt( 
				       pow(XOutMuonHBtrk - XOutMuonHBdt,2) + 
				       pow(YOutMuonHBtrk - YOutMuonHBdt,2) 
				       );
	      nearestTrkIdxAtHBout = trackCounter;

	      if (printOut){
		cout << "MindistAtHBout: " << MindistAtHBout << endl;
		cout << "nearestDZAtHBout: " << nearestDZAtHBout << endl;
		cout << "nearestDXYAtHBout: " << nearestDXYAtHBout << endl;
		cout << "nearestTrkIdxAtHBout: " << nearestTrkIdxAtHBout << endl;
	      }
	    }

	  //next track
	  trackCounter++;

	}// end loop over tracks

      //-- fill matching info (or the default -999 if propagation is failed)
      muonDTnrstTrkDXY[muonDTCount] = nearestDXYAtHBout;
      muonDTnrstTrkDZ[muonDTCount] = nearestDZAtHBout;
      muonDTnrstTrkIdx[muonDTCount] = nearestTrkIdxAtHBout;

      if (printOut){
	cout << "muonDTnrstTrkDXY["<< muonDTCount << "]: " << nearestDXYAtHBout << endl;
	cout << "muonDTnrstTrkDZ["<< muonDTCount << "]: " << nearestDZAtHBout << endl;
	cout << "muonDTnrstTrkIdx["<< muonDTCount << "]: " << nearestTrkIdxAtHBout << endl;
	cout << endl;
      }


      //## verify if DT muon track can be propagated to both inner and outer HCAL radius
      float XOutMuonHBdt=-999;
      float YOutMuonHBdt=-999;
      float ZOutMuonHBdt=-999;

      float XInMuonHBdt=-999;
      float YInMuonHBdt=-999;
      float ZInMuonHBdt=-999;

      bool isValidDTmuonProp=false;
      bool isValidB=false;
      bool isValidBin=false;
      bool isValidBout=false;
      bool isValidF=false;
      bool isValidFin=false;
      bool isValidFout=false;

      if(cmTrack->innerPosition().Y()<0) { //DT track at BOTTOM
	//-- propagate backward DT to HB barrel in & out
	int charge =cmTrack->charge();
	GlobalPoint  gmpos( cmTrack->innerPosition().X(), cmTrack->innerPosition().Y(), cmTrack->innerPosition().Z());
	GlobalVector gmmom( cmTrack->innerMomentum().X(), cmTrack->innerMomentum().Y(), cmTrack->innerMomentum().Z());
	isValidBout=PropagateB( gmpos, gmmom ,charge,OutRadiusHB,
				&XOutMuonHBdt,&YOutMuonHBdt,&ZOutMuonHBdt);	
	isValidBin=PropagateB( gmpos, gmmom ,charge,InRadiusHB,
			       &XInMuonHBdt,&YInMuonHBdt,&ZInMuonHBdt);	
	if(isValidBin==true && isValidBout==true)
	  isValidB=true;

	//## Calculate Eta and Phi of the intersection points (in & out HB)
	TVector3 CrossAtInHB(XInMuonHBdt,YInMuonHBdt,ZInMuonHBdt);	
	TVector3 CrossAtOutHB(XOutMuonHBdt,YOutMuonHBdt,ZOutMuonHBdt);	

	if(isValidBin==true){
	  muonDTRadiusAtInHB[muonDTCount]=CrossAtInHB.Perp();	  
	  muonDTEtaAtInHB[muonDTCount]=CrossAtInHB.Eta();	  
	  muonDTPhiAtInHB[muonDTCount]=CrossAtInHB.Phi();
	}else{
	  muonDTRadiusAtInHB[muonDTCount]=-999.;	  
	  muonDTEtaAtInHB[muonDTCount]=-999.;	  
	  muonDTPhiAtInHB[muonDTCount]=-999.;
	}

	if(isValidBout==true){
	  muonDTRadiusAtOutHB[muonDTCount]=CrossAtOutHB.Perp();	  
	  muonDTEtaAtOutHB[muonDTCount]=CrossAtOutHB.Eta();	  
	  muonDTPhiAtOutHB[muonDTCount]=CrossAtOutHB.Phi();
	}else{
	  muonDTRadiusAtOutHB[muonDTCount]=-999.;	  
	  muonDTEtaAtOutHB[muonDTCount]=-999.;	  
	  muonDTPhiAtOutHB[muonDTCount]=-999.;
	}

      }else{ //DT track at TOP
	//-- propagate forward dt to HB barrel in & out
	int charge =cmTrack->charge();
	GlobalPoint gmpos( cmTrack->outerPosition().X(), cmTrack->outerPosition().Y(), cmTrack->outerPosition().Z());
	GlobalVector gmmom( cmTrack->outerMomentum().X(), cmTrack->outerMomentum().Y(), cmTrack->outerMomentum().Z());
	isValidFout=PropagateF( gmpos, gmmom ,charge,OutRadiusHB,
				&XOutMuonHBdt,&YOutMuonHBdt,&ZOutMuonHBdt);	
	isValidFin=PropagateF( gmpos, gmmom ,charge,InRadiusHB,
			       &XInMuonHBdt,&YInMuonHBdt,&ZInMuonHBdt);	
	if(isValidFin==true && isValidFout==true)
	  isValidF=true;

	//## Calculate Eta and Phi of the intersection points (in & out HB)
	TVector3 CrossAtInHB(XInMuonHBdt,YInMuonHBdt,ZInMuonHBdt);	
	TVector3 CrossAtOutHB(XOutMuonHBdt,YOutMuonHBdt,ZOutMuonHBdt);	

	if(isValidFin==true){
	  muonDTRadiusAtInHB[muonDTCount]=CrossAtInHB.Perp();	  
	  muonDTEtaAtInHB[muonDTCount]=CrossAtInHB.Eta();	  
	  muonDTPhiAtInHB[muonDTCount]=CrossAtInHB.Phi();
	}else{
	  muonDTRadiusAtInHB[muonDTCount]=-999.;	  
	  muonDTEtaAtInHB[muonDTCount]=-999.;	  
	  muonDTPhiAtInHB[muonDTCount]=-999.;
	}

	if(isValidFout==true){
	  muonDTRadiusAtOutHB[muonDTCount]=CrossAtOutHB.Perp();	  
	  muonDTEtaAtOutHB[muonDTCount]=CrossAtOutHB.Eta();	  
	  muonDTPhiAtOutHB[muonDTCount]=CrossAtOutHB.Phi();
	}else{
	  muonDTRadiusAtOutHB[muonDTCount]=-999.;	  
	  muonDTEtaAtOutHB[muonDTCount]=-999.;	  
	  muonDTPhiAtOutHB[muonDTCount]=-999.;
	}

      }//end if/else to select top or bottom dt muons

      if(isValidB==true || isValidF==true)
	{
	  isValidDTmuonProp=true;
	  muonDTisValidProp[muonDTCount]=isValidDTmuonProp;
	}



      //## calculating Length of muon in HB (simply a line approximation)
      if(isValidDTmuonProp==true)
	{
	  muonDTLengthInHB[muonDTCount] = sqrt(  pow((XInMuonHBdt-XOutMuonHBdt),2)
					       + pow((YInMuonHBdt-YOutMuonHBdt),2)
					       + pow((ZInMuonHBdt-ZOutMuonHBdt),2) );  
	}
      else
	muonDTLengthInHB[muonDTCount] = -999.;



      //## Get HCAL energy deposits for all rechits touched by the DT muon track
      const CaloGeometry& geometry = *geo ;
      const CaloSubdetectorGeometry *geometry_p ; 
      geometry_p = geometry.getSubdetectorGeometry (DetId::Hcal,4) ;

      if(isValidDTmuonProp==true)
	{
	  //muon crossing point
	  GlobalPoint posMuonAtInHB( XInMuonHBdt, YInMuonHBdt, ZInMuonHBdt);
	  GlobalPoint posMuonAtOutHB( XOutMuonHBdt, YOutMuonHBdt, ZOutMuonHBdt);
	  //closest HCAL cell
	  HcalDetId CellAtInHB( geometry_p->getClosestCell(posMuonAtInHB) );
	  HcalDetId CellAtOutHB( geometry_p->getClosestCell(posMuonAtOutHB) );
	  //position of nearest HCAL cell
	  GlobalPoint CellAtInHBpos = geo->getPosition( CellAtInHB );
	  GlobalPoint CellAtOutHBpos = geo->getPosition( CellAtOutHB );

 	  int IetaInHB = CellAtInHB.ieta(); 
	  int IphiInHB = CellAtInHB.iphi();
 	  int IetaOutHB = CellAtOutHB.ieta();
 	  int IphiOutHB = CellAtOutHB.iphi();

	  //-- DeltaR between muon crossing point and center of closest HCAL cell
	  TVector3 v3_posMuonAtInHB( posMuonAtInHB.x(), posMuonAtInHB.y(), posMuonAtInHB.z());
	  TVector3 v3_posMuonAtOutHB( posMuonAtOutHB.x(), posMuonAtOutHB.y(), posMuonAtOutHB.z());
	  TVector3 v3_CellAtInHBpos( CellAtInHBpos.x(), CellAtInHBpos.y(), CellAtInHBpos.z());
	  TVector3 v3_CellAtOutHBpos( CellAtOutHBpos.x(), CellAtOutHBpos.y(), CellAtOutHBpos.z());

	  float deltaRposMuonVSposCellAtInHB = v3_posMuonAtInHB.DeltaR(v3_CellAtInHBpos);
	  float deltaRposMuonVSposCellAtOutHB = v3_posMuonAtOutHB.DeltaR(v3_CellAtOutHBpos);

	  muonDTdeltaRatInHB[muonDTCount] = deltaRposMuonVSposCellAtInHB;
	  muonDTdeltaRatOutHB[muonDTCount] = deltaRposMuonVSposCellAtOutHB;

	  //checks
	  if (printOut){
	    cout << "-------------" << endl;
	    cout << "--> isValidDTmuonProp: " << isValidDTmuonProp << endl;
	    cout << "IetaInHB: " << IetaInHB 
		 << " VS IetaOutHB: " << IetaOutHB << endl; 
	    cout << "IphiInHB: " << IphiInHB 
		 << " VS IphiOutHB: " << IphiOutHB << endl; 
	    cout << "deltaRposMuonVSposCellAtInHB: " << deltaRposMuonVSposCellAtInHB << endl;
	    cout << "deltaRposMuonVSposCellAtOutHB: " << deltaRposMuonVSposCellAtOutHB << endl;
	    
	    //checks
	    // 	  cout << endl;
	    // 	  cout << "CellAtInHBpos.x(): " << CellAtInHBpos.x() 
	    // 	       << " XInMuonHBdt: " << XInMuonHBdt << endl;
	    // 	  cout << "CellAtInHBpos.y(): " << CellAtInHBpos.y() 
	    // 	       << " YInMuonHBdt: " << YInMuonHBdt << endl;
	    // 	  cout << "CellAtInHBpos.z(): " << CellAtInHBpos.z() 
	    // 	       << " ZInMuonHBdt: " << ZInMuonHBdt << endl;
	    
	    // 	  cout << "CellAtOutHBpos.x(): " << CellAtOutHBpos.x() 
	    // 	       << " XOutMuonHBdt: " << XOutMuonHBdt << endl;
	    // 	  cout << "CellAtOutHBpos.y(): " << CellAtOutHBpos.y() 
	    // 	       << " YOutMuonHBdt: " << YOutMuonHBdt << endl;
	    // 	  cout << "CellAtOutHBpos.z(): " << CellAtOutHBpos.z() 
	    // 	       << " ZOutMuonHBdt: " << ZOutMuonHBdt << endl;
	  }

	  //-- Navigate cells 
	  vector<int> v_eta;
	  vector<int> v_phi;
	  int startEta=0;
	  int endEta=0;
	  int startPhi=0;
	  int endPhi=0;

	  if(IetaInHB < IetaOutHB)
	    {
	      startEta=IetaInHB; 
	      endEta=IetaOutHB;
	    } 
	  else
	    {
	      startEta=IetaOutHB; 
	      endEta=IetaInHB;
	    }
	  if(IphiInHB < IphiOutHB)
	    {
	      startPhi=IphiInHB; 
	      endPhi=IphiOutHB;
	    } 
	  else
	    {
	      startPhi=IphiOutHB; 
	      endPhi=IphiInHB;
	    }
	  for( int idx_eta = startEta ; idx_eta<=endEta ; idx_eta++)
	    {
	      if(idx_eta==0)
		continue;
	      for( int idx_phi = startPhi ; idx_phi<=endPhi ; idx_phi++)
		{
		  if(idx_phi==0)
		    continue;
		  if (printOut){
		    cout << "idx_phi: " << idx_phi 
			 << " idx_eta: " << idx_eta << endl;
		  }
		  v_eta.push_back(idx_eta);
		  v_phi.push_back(idx_phi);
		}
	    }

	  //--Energy deposited in HCAL cells
	  float totalEnergy = 0;
	  int Ncells=0;

	  int size = v_eta.size(); //same as v_phi
	  for(HBHERecHitCollection::const_iterator hhit=Hithbhe.begin(); 
	      hhit!=Hithbhe.end(); hhit++) {
	    
	    for(int ii=0; ii<size; ii++)
	      {	    
		HcalDetId currentDetId (hhit->detid());
		
 		if(v_eta[ii] == currentDetId.ieta() 
 		   && v_phi[ii] == currentDetId.iphi() )
 		  {
		    // 		    cout << "hhit->energy(): " << hhit->energy() 
		    // 			 << " hhit->time(): "  << hhit->time() 
		    // 			 << " currentDetId.depth(): "  << currentDetId.depth() 
		    // 			 << " currentDetId.ieta(): "  << currentDetId.ieta() 
		    // 			 << " currentDetId.iphi(): "  << currentDetId.iphi() 
		    // 			 << endl;
		    Ncells++;
		    totalEnergy += hhit->energy();
		  }
	      }//end loop over cells touched by the muon
	    
	  }//end loop over HBHERecHits

	  //Have Cells Same Phi or Eta?
	  bool HaveCellsSamePhi=true;
	  bool HaveCellsSameEta=true;
	  
	  for(int ii=1; ii<int(v_eta.size()); ii++){
	    if(v_eta[ii]!=v_eta[ii-1])
	      {
		HaveCellsSameEta=false;
		break;
	      }
	  }
	  
	  for(int ii=1; ii<int(v_phi.size()); ii++){
	    if(v_phi[ii]!=v_phi[ii-1])
	      {
		HaveCellsSamePhi=false;
		break;
	      }
	  }
	  
	  muonDTHaveCellsSamePhi[muonDTCount] = HaveCellsSamePhi; 
	  muonDTHaveCellsSameEta[muonDTCount] = HaveCellsSameEta; 
	  
	  muonDTTotEnergy[muonDTCount] = totalEnergy;
	  muonDTNumCells[muonDTCount] = Ncells;

	  if (printOut){
	    cout << "Ncells: " << Ncells << endl;
	    cout << "HaveCellsSamePhi: " << HaveCellsSamePhi << endl;
	    cout << "HaveCellsSameEta: " << HaveCellsSameEta << endl;
	    cout << "totalEnergy: " << totalEnergy << endl;
	  }

	}//end isValidMuonProp for this DT muon track


      //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      //next cosmic muon
      muonDTCount++; // don't move this
      //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    }//end loop over cosmic muons

  //-- loop over inner tracker tracks
  trackCount = 0;
  for (reco::TrackCollection::const_iterator ctftrk =ctfTrackCollectionHandle->begin(); 
       ctftrk != ctfTrackCollectionHandle->end();
       ++ctftrk)
    {
      
      if(trackCount>=MAXnTRACK)
	break;
      
      trackP[trackCount] = ctftrk->p();
      trackPt[trackCount] = ctftrk->pt();
      trackNdof[trackCount] = ctftrk->ndof();
      trackChi2[trackCount] = ctftrk->chi2();
      trackLost[trackCount] = ctftrk->lost();
      trackCharge[trackCount] = ctftrk->charge();
      
      //## Take energy deposits near the intersection between the DT muon track and HCAL barrel

      //-- verify if tracker track can be propagated to both inner and outer HCAL radius


      //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      //next cosmic muon
      trackCount++; // don't move this
      //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    }

  //-- loop over jets
  caloJetIC5Count=0;
  for( CaloJetCollection::const_iterator calojet = caloJetsIC5_raw->begin(); calojet != caloJetsIC5_raw->end(); calojet++ ) 
    {
      //exit from loop when you reach the required number of electrons
      if(caloJetIC5Count > MAXnJET)
	break;

      float EMF = calojet->emEnergyFraction();
      float HADF = calojet->energyFractionHadronic();

      caloJetIC5Pt[caloJetIC5Count]=calojet->pt();
      caloJetIC5Energy[caloJetIC5Count]=calojet->energy();
      caloJetIC5Phi[caloJetIC5Count]=calojet->phi();
      caloJetIC5Eta[caloJetIC5Count]=calojet->eta();
      caloJetIC5EMF[caloJetIC5Count]=EMF;
      caloJetIC5HADF[caloJetIC5Count]=HADF;
      caloJetIC5towersArea[caloJetIC5Count]=calojet->towersArea();
      caloJetIC5n90[caloJetIC5Count]=calojet->n90();
      caloJetIC5n60[caloJetIC5Count]=calojet->n60();

      //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      //next calo jet
      caloJetIC5Count++; // don't move this
      //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    }

  //-- MET
  caloMET = recomet.et();
  caloMETPhi = recomet.phi();

  //########################################
  //## Fill the Tree ofr each event
  m_tree->Fill();
  //########################################

}


// ------------ method called once each job just before starting event loop  ------------
void 
HcalPromBremss::beginJob(const edm::EventSetup& iSetup)
{
  m_file = new TFile(rootfile_.c_str(),"RECREATE");
  m_tree = NULL;
  m_tree = new TTree ("muonTree","muonTree") ;

  m_tree->Branch("event",&event,"event/I");
  m_tree->Branch("lumi",&lumi,"lumi/I");
  m_tree->Branch("run",&run,"run/I");

  m_tree->Branch("L1TrgCandDecision",&L1TrgCandDecision,"L1TrgCandDecision[4]/O");
  m_tree->Branch("nTriggers",&nTriggers,"nTriggers/I");
  m_tree->Branch("GTDecision",&GTDecision,"GTDecision[nTriggers]/O");

  m_tree->Branch("nL1DTCandL1ABx",&nL1DTCandL1ABx,"nL1DTCandL1ABx/I");
  m_tree->Branch("nL1DTCandL1ApBx",&nL1DTCandL1ApBx,"nL1DTCandL1ApBx/I");
  m_tree->Branch("nL1DTCandL1AnBx",&nL1DTCandL1AnBx,"nL1DTCandL1AnBx/I");

  m_tree->Branch("nL1RPCbCandL1ABx",&nL1RPCbCandL1ABx,"nL1RPCbCandL1ABx/I");
  m_tree->Branch("nL1RPCbCandL1ApBx",&nL1RPCbCandL1ApBx,"nL1RPCbCandL1ApBx/I");
  m_tree->Branch("nL1RPCbCandL1AnBx",&nL1RPCbCandL1AnBx,"nL1RPCbCandL1AnBx/I");

  m_tree->Branch("nL1CSCCandL1ABx",&nL1CSCCandL1ABx,"nL1CSCCandL1ABx/I");
  m_tree->Branch("nL1CSCCandL1ApBx",&nL1CSCCandL1ApBx,"nL1CSCCandL1ApBx/I");
  m_tree->Branch("nL1CSCCandL1AnBx",&nL1CSCCandL1AnBx,"nL1CSCCandL1AnBx/I");

  m_tree->Branch("muonDTCount",&muonDTCount,"muonDTCount/I");
  m_tree->Branch("muonDTXin",&muonDTXin,"muonDTXin[muonDTCount]/F");
  m_tree->Branch("muonDTYin",&muonDTYin,"muonDTYin[muonDTCount]/F");
  m_tree->Branch("muonDTZin",&muonDTZin,"muonDTZin[muonDTCount]/F");
  m_tree->Branch("muonDTXout",&muonDTXout,"muonDTXout[muonDTCount]/F");
  m_tree->Branch("muonDTYout",&muonDTYout,"muonDTYout[muonDTCount]/F");
  m_tree->Branch("muonDTZout",&muonDTZout,"muonDTZout[muonDTCount]/F");
  m_tree->Branch("muonDTNumRecHits",&muonDTNumRecHits,"muonDTNumRecHits[muonDTCount]/I");
  m_tree->Branch("muonDTNumValidRecHits",&muonDTNumValidRecHits,"muonDTNumValidRecHits[muonDTCount]/I");
  m_tree->Branch("muonDTP",&muonDTP,"muonDTP[muonDTCount]/F");
  m_tree->Branch("muonDTPt",&muonDTPt,"muonDTPt[muonDTCount]/F");
  m_tree->Branch("muonDTCharge",&muonDTCharge,"muonDTCharge[muonDTCount]/I");

  m_tree->Branch("trackCount",&trackCount,"trackCount/I");
  m_tree->Branch("trackP",&trackP,"trackP[trackCount]/F");
  m_tree->Branch("trackPt",&trackPt,"trackPt[trackCount]/F");
  m_tree->Branch("trackNdof",&trackNdof,"trackNdof[trackCount]/F");
  m_tree->Branch("trackChi2",&trackChi2,"trackChi2[trackCount]/F");
  m_tree->Branch("trackLost",&trackLost,"trackLost[trackCount]/F");
  m_tree->Branch("trackCharge",&trackCharge,"trackCharge[trackCount]/I");

  m_tree->Branch("caloJetIC5Count",&caloJetIC5Count,"caloJetIC5Count/I");
  m_tree->Branch("caloJetIC5Pt",&caloJetIC5Pt,"caloJetIC5Pt[caloJetIC5Count]/F");
  m_tree->Branch("caloJetIC5Energy",&caloJetIC5Energy,"caloJetIC5Energy[caloJetIC5Count]/F");
  m_tree->Branch("caloJetIC5Phi",&caloJetIC5Phi,"caloJetIC5Phi[caloJetIC5Count]/F");
  m_tree->Branch("caloJetIC5Eta",&caloJetIC5Eta,"caloJetIC5Eta[caloJetIC5Count]/F");
  m_tree->Branch("caloJetIC5EMF",&caloJetIC5EMF,"caloJetIC5EMF[caloJetIC5Count]/F");
  m_tree->Branch("caloJetIC5HADF",&caloJetIC5HADF,"caloJetIC5HADF[caloJetIC5Count]/F");
  m_tree->Branch("caloJetIC5towersArea",&caloJetIC5towersArea,"caloJetIC5towersArea[caloJetIC5Count]/F");
  m_tree->Branch("caloJetIC5n90",&caloJetIC5n90,"caloJetIC5n90[caloJetIC5Count]/I");
  m_tree->Branch("caloJetIC5n60",&caloJetIC5n60,"caloJetIC5n60[caloJetIC5Count]/I");

  m_tree->Branch("caloMET",&caloMET,"caloMET/F");
  m_tree->Branch("caloMETPhi",&caloMETPhi,"caloMETPhi/F");

  m_tree->Branch("muonDTnrstTrkDXY",&muonDTnrstTrkDXY,"muonDTnrstTrkDXY[muonDTCount]/F");
  m_tree->Branch("muonDTnrstTrkDZ",&muonDTnrstTrkDZ,"muonDTnrstTrkDZ[muonDTCount]/F");
  m_tree->Branch("muonDTnrstTrkIdx",&muonDTnrstTrkIdx,"muonDTnrstTrkIdx[muonDTCount]/I");

  m_tree->Branch("muonDTRadiusAtInHB",&muonDTRadiusAtInHB,"muonDTRadiusAtInHB[muonDTCount]/F");
  m_tree->Branch("muonDTEtaAtInHB",&muonDTEtaAtInHB,"muonDTEtaAtInHB[muonDTCount]/F");
  m_tree->Branch("muonDTPhiAtInHB",&muonDTPhiAtInHB,"muonDTPhiAtInHB[muonDTCount]/F");
  m_tree->Branch("muonDTRadiusAtOutHB",&muonDTRadiusAtOutHB,"muonDTRadiusAtOutHB[muonDTCount]/F");
  m_tree->Branch("muonDTEtaAtOutHB",&muonDTEtaAtOutHB,"muonDTEtaAtOutHB[muonDTCount]/F");
  m_tree->Branch("muonDTPhiAtOutHB",&muonDTPhiAtOutHB,"muonDTPhiAtOutHB[muonDTCount]/F");

  m_tree->Branch("muonDTLengthInHB",&muonDTLengthInHB,"muonDTLengthInHB[muonDTCount]/F");
  m_tree->Branch("muonDTisValidProp",&muonDTisValidProp,"muonDTisValidProp[muonDTCount]/O");
  m_tree->Branch("muonDTdeltaRatInHB",&muonDTdeltaRatInHB,"muonDTdeltaRatInHB[muonDTCount]/F");
  m_tree->Branch("muonDTdeltaRatOutHB",&muonDTdeltaRatOutHB,"muonDTdeltaRatOutHB[muonDTCount]/F");
  m_tree->Branch("muonDTTotEnergy",&muonDTTotEnergy,"muonDTTotEnergy[muonDTCount]/F");
  m_tree->Branch("muonDTNumCells",&muonDTNumCells,"muonDTNumCells[muonDTCount]/F");
  m_tree->Branch("muonDTHaveCellsSameEta",&muonDTHaveCellsSameEta,"muonDTHaveCellsSameEta[muonDTCount]/O");
  m_tree->Branch("muonDTHaveCellsSamePhi",&muonDTHaveCellsSamePhi,"muonDTHaveCellsSamePhi[muonDTCount]/O");

  //Setup SteppingHelixPropagator
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

// ------------ method called once each job just after ending the event loop  ------------
void 
HcalPromBremss::endJob() {

  m_file->Write();
  m_file->Close();

}



bool HcalPromBremss::PropagateF(
			 // inputs
			 //  double ox, double oy, double oz, double px, double py, double pz, double ra,
			 GlobalPoint pos, GlobalVector mom, int charge, float ra, 
			 // outputs
			 float *x_HB, float *y_HB, float *z_HB){
  bool isValid = 0;
  *x_HB=-9999.;
  *y_HB=-9999.;
  *z_HB=-9999.;

  const FreeTrajectoryState *freetrajectorystate_ =new FreeTrajectoryState(pos, mom ,charge , &(*theMagField));
  //  PlaneBuilder::ReturnType aPlane = PlaneBuilder().plane(Surface::PositionType(0,0,0),Surface::RotationType());
  Cylinder *cylinder = new Cylinder(Surface::PositionType(0,0,0),
				    Surface::RotationType(), ra);
  //  FreeTrajectoryState * steppingHelixstateinfo_ = stepProp->propagate(*freetrajectorystate_, (*cylinder)).freeState();
  TrajectoryStateOnSurface steppingHelixstateinfo_ = stepPropF->propagate(*freetrajectorystate_, (*cylinder));
  isValid = steppingHelixstateinfo_.isValid();
  if (steppingHelixstateinfo_.isValid()) {
    //             GlobalVector hotrkpos2(steppingHelixstateinfo_.position().x(), steppingHelixstateinfo_.position().y(),steppingHelixstateinfo_.position().z());
    //             Hep3Vector hotrkdir2(steppingHelixstateinfo_.momentum().x(),steppingHelixstateinfo_.momentum().y(),steppingHelixstateinfo_.momentum().z());
    *x_HB=steppingHelixstateinfo_.freeState()->position().x();        
    *y_HB=steppingHelixstateinfo_.freeState()->position().y();        
    *z_HB=steppingHelixstateinfo_.freeState()->position().z();        
  }
  return isValid;
}

bool HcalPromBremss::PropagateB(
			 // inputs
			 //  double ox, double oy, double oz, double px, double py, double pz, double ra,
			 GlobalPoint pos, GlobalVector mom, int charge, float ra,
			 // outputs
			 float *x_HB, float *y_HB, float *z_HB){
  bool isValid = 0;
  *x_HB=-9999.;
  *y_HB=-9999.;
  *z_HB=-9999.;

  const FreeTrajectoryState *freetrajectorystate_ =new FreeTrajectoryState(pos, mom ,charge , &(*theMagField));
  //  PlaneBuilder::ReturnType aPlane = PlaneBuilder().plane(Surface::PositionType(0,0,0),Surface::RotationType());
  Cylinder *cylinder = new Cylinder(Surface::PositionType(0,0,0),
				    Surface::RotationType(), ra);
  //  FreeTrajectoryState * steppingHelixstateinfo_ = stepProp->propagate(*freetrajectorystate_, (*cylinder)).freeState();
  TrajectoryStateOnSurface steppingHelixstateinfo_ = stepPropB->propagate(*freetrajectorystate_, (*cylinder));
  isValid = steppingHelixstateinfo_.isValid();
  if (steppingHelixstateinfo_.isValid()) {
    //             GlobalVector hotrkpos2(steppingHelixstateinfo_.position().x(), steppingHelixstateinfo_.position().y(),steppingHelixstateinfo_.position().z());
    //             Hep3Vector hotrkdir2(steppingHelixstateinfo_.momentum().x(),steppingHelixstateinfo_.momentum().y(),steppingHelixstateinfo_.momentum().z());
    *x_HB=steppingHelixstateinfo_.freeState()->position().x();        
    *y_HB=steppingHelixstateinfo_.freeState()->position().y();        
    *z_HB=steppingHelixstateinfo_.freeState()->position().z();        
  }
  return isValid;
}

//define this as a plug-in
//DEFINE_FWK_MODULE(HcalPromBremss);
