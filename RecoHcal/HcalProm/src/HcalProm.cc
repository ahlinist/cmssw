// -*- C++ -*-
//
// Package:    HcalProm
// Class:      HcalProm
// 
/**\class HcalProm HcalProm.cc RecoHcal/HcalProm/src/HcalProm.cc

 See instructions in https://twiki.cern.ch/twiki/bin/view/CMS/HcalProm#Software for usage
 The config file should read the officially reconstructed root file in castor or 
 The output from Configuration/GlobalRuns/data/recoT0.cfg (official reco cfg). 

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Efe Yazgan
// Updated        :  Taylan Yetkin (2008/05/08)
//                   Fedor Ratnikov
//                   Jordan Damgov
//         Created:  Wed Apr 16 10:03:18 CEST 2008
// $Id: HcalProm.cc,v 1.25 2008/06/10 19:23:23 fedor Exp $
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


namespace {
  class CaloJetSort {
  public:
    bool operator() (const CaloJet & a, const CaloJet & b) {
      return a.pt() > b.pt();
    }
  };

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
    //@@
    fIPDistance = iConfig.getUntrackedParameter<double>("MuonAcceptance", 10000.);
    fAlgoType = iConfig.getUntrackedParameter<int>("MuonSelectionAlgoType", 1);
    fIP_z = iConfig.getUntrackedParameter<double>("IPZ", 20.);
    fIP_r = iConfig.getUntrackedParameter<double>("IPR", 50.);
   top_ihpi_low = iConfig.getUntrackedParameter<int>("topIPHIlow", 15);
    top_ihpi_high = iConfig.getUntrackedParameter<int>("topIPHIhigh", 22);
    bottom_ihpi_low = iConfig.getUntrackedParameter<int>("topIPHIlow", 51);
    bottom_ihpi_high = iConfig.getUntrackedParameter<int>("topIPHIhigh", 58);

    myCalorimeter_ = new CaloGeometryHelper(iConfig.getParameter<edm::ParameterSet>("Calorimetry"));
    NTotal = 0;
    NAccepted = 0;
    //@@
    runBegin = -1;
    evtNo = 0;
    lumibegin=0;
    lumiend=0;
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


HcalProm::~HcalProm()
{}



// ------------ method called to for each event  ------------
void HcalProm::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    // old versions time_t a = (iEvent.time().value()) & 0xffffffff;
    time_t a = (iEvent.time().value()) >> 32;

    int lumi = iEvent.luminosityBlock();

    if (runBegin < 0) { // parameters for the first event
        startTime = ctime(&a);
        lumibegin=lumiend=lumi;
        runBegin = iEvent.id().run();
	bookHistograms (); // book all histograms
    }

    if(lumi < lumibegin) lumibegin = lumi;
    if(lumi > lumiend) lumiend = lumi;

    cout<<" Luminosity Block: " << lumibegin <<" Time : "<<startTime<<endl;

    //hcal rechits
    Handle<HBHERecHitCollection> hbhe;
    iEvent.getByLabel("hbhereco", hbhe);
    const HBHERecHitCollection Hithbhe = *(hbhe.product());
    const HBHERecHitCollection*  HBHERecHits = 0;
    
    iEvent.getByLabel("hbhereco", hbhe);
    HBHERecHits = hbhe.product(); // get a ptr to the product

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
   /*   Handle<reco::BasicClusterCollection> pIslandBarrelBasicClusters;
   iEvent.getByLabel("islandBasicClusters","islandBarrelBasicClusters",pIslandBarrelBasicClusters);
   const BasicClusterCollection islandBarrelBasicClusters = *(pIslandBarrelBasicClusters.product());
   */


   Handle<reco::BasicClusterCollection> bccHandle;
   iEvent.getByLabel("cosmicBasicClusters","CosmicBarrelBasicClusters", bccHandle);
   const reco::BasicClusterCollection *clusterCollection_p = 0;
   if (!bccHandle.failedToGet()) clusterCollection_p = bccHandle.product();

   

   // reco jets
   Handle<CaloJetCollection> caloJet;
   iEvent.getByLabel("iterativeCone15CaloJets",caloJet);
   const CaloJetCollection cjet = *(caloJet.product());

    //geometry
    const CaloGeometry* geo;
    const CaloSubdetectorGeometry *geometry_hb ; 
    ESHandle<CaloGeometry> pG;
    iSetup.get<IdealGeometryRecord>().get(pG);
    geo = pG.product();
    geometry_hb = geo->getSubdetectorGeometry(DetId::Hcal,HcalBarrel);
    
    std::cout << "\nEvent ID = "<< iEvent.id() << std::endl ;
    ++NTotal;

   //Cosmic Muons
   Handle<TrackCollection> muons;
   iEvent.getByLabel("cosmicMuons",muons);

   const TrackCollection tC = *(muons.product());
   std::cout << "Number of cosmic muon tracks in this event: " << tC.size() << endl;

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
   bool csc_l1a = false;
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

     // CSC Trigger
     int icsc = 0;
     rmc = igmtrr->getCSCCands(); 
     for(iter1=rmc.begin(); iter1!=rmc.end(); iter1++) {
       if (!(*iter1).empty()) ++icsc;
     }
     if(icsc>0) cout << "Found " << icsc << " valid CSC candidates in bx wrt. L1A = " << igmtrr->getBxInEvent() <<endl;
     if(igmtrr->getBxInEvent()==0 && icsc>0) csc_l1a = true;
	
   }
   
   // trigger summary
   DecisionWord gtDecisionWord = gtrr->decisionWord();
   for (size_t iBit = 0; iBit < gtDecisionWord.size(); ++iBit) {
     if (gtDecisionWord [iBit]) {
       h_global_trigger_bit->Fill (iBit);
       //   std::string triggerName = iBit < l1TableSize ? l1TriggerNames [iBit] : "Undefined";
       //   std::cout << "L1 Accepted: L1 bit " << iBit <<", trigger " << triggerName << std::endl;
     }
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
   if(csc_l1a) std::cout << "CSC" << std::endl;
   if(hcal_l1a) std::cout << "HCAL" << std::endl;
   std::cout << "************************" << std::endl;
   



   // DIGIS ARE TAKEN OUT FROM T0 RECONSTRUCTION, but not at FNAL
   if (!hbhe_digi.failedToGet ()) {
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
   }
  

   //@@
    double inner_radius_hcal = 188.15;
    double outer_radius_hcal = 286.40;
    
    double inner_z_width = 350.;
    double outer_z_width = 460.;
    double ip_z_width = fIP_z;
    h_AllTracks->Fill(tC.size());
    if(tC.size()<2)return;
    //first count how many tracks can be extrapolated to HCAL:
    int nTrack = 0;
    int NGoodTracks = 0;
    vector<Track> myGoodTracks;
    vector<double> innerEtaP;
    vector<double> innerPhiP;
    vector<double> outerEtaP;
    vector<double> outerPhiP;
    vector<double> innerEtaM;
    vector<double> innerPhiM;
    vector<double> outerEtaM;
    vector<double> outerPhiM;
    vector<double> midXp;
    vector<double> midYp;
    vector<double> midZp;
    double f_corr = 1.;
    HcalDetId fhcalDetId;
    for (reco::TrackCollection::const_iterator track=tC.begin(); track!=tC.end(); track++){
	if(track->recHitsSize()<25) continue;
        f_corr = 1.;
         h_ct4->Fill(1);
	bool found1 = false;
	double inner_thetap = -999.;
	double inner_etap = -999.;
	double inner_phip   = -999.;
	double inner_thetam = -999.;
	double inner_etam = -999.;
	double inner_phim   = -999.;
	
	double outer_thetap = -999.;
	double outer_etap = -999.;
	double outer_phip   = -999.;
	double outer_thetam = -999.;
	double outer_etam = -999.;
	double outer_phim   = -999.;
	
	double inner_xp = -999.;
	double inner_yp = -999.;
	double inner_zp = -999.;
	double inner_xm = -999.;
	double inner_ym = -999.;
	double inner_zm = -999.;
	
	double outer_xp = -999.;
	double outer_yp = -999.;
	double outer_zp = -999.;
	double outer_xm = -999.;
	double outer_ym = -999.;
	double outer_zm = -999.;
	
	bool isCloseToIP_in = false;
	bool isCloseToIP_out = false;
	bool isCloseToIP_ip = false;
	//extrapolate to inner hcal
	isCloseToIP_in = Extrapolate(
		     // inputs
		     track->innerPosition().x(), track->innerPosition().y(), track->innerPosition().z(),
		     track->innerMomentum().x(), track->innerMomentum().y(), track->innerMomentum().z(),
		     inner_radius_hcal, inner_z_width, fIP_r,
		     // outputsa
		     &inner_xp, &inner_yp, &inner_zp, &inner_xm, &inner_ym, &inner_zm,
		     &inner_thetap, &inner_phip,
		     &inner_thetam, &inner_phim
		     );
	 //extrapolate to outer hcal	     
         isCloseToIP_out = Extrapolate(
	             // inputs
		     track->innerPosition().x(), track->innerPosition().y(), track->innerPosition().z(),
		     track->innerMomentum().x(), track->innerMomentum().y(), track->innerMomentum().z(),
		     outer_radius_hcal, outer_z_width,fIP_r,
		     // outputs
		     &outer_xp, &outer_yp, &outer_zp, &outer_xm, &outer_ym, &outer_zm,
		     &outer_thetap, &outer_phip,
		     &outer_thetam, &outer_phim
		     );
	 std::cout<<"PMy : "<<outer_yp<<"\t"<<track->innerPosition().y()<<endl;
	 std::cout<<"PMx : "<<outer_xp<<"\t"<<track->innerPosition().x()<<endl;
	 if (inner_thetam != -999.) {
	     inner_etam = -1*TMath::Log(TMath::Tan(inner_thetam/2.));
	     inner_etap = -1*TMath::Log(TMath::Tan(inner_thetap/2.));
	     if (TMath::IsNaN(inner_etam)) inner_etam = -999; // aiaiaiaiaaaaaiiii
	     if (TMath::IsNaN(inner_etap)) inner_etap = -999; // aiaiaiaiaaaaaiiii
         }
	 if (outer_thetam != -999.) {
	     outer_etam = -1*TMath::Log(TMath::Tan(outer_thetam/2.));
	     outer_etap = -1*TMath::Log(TMath::Tan(outer_thetap/2.));
	     if (TMath::IsNaN(outer_etam)) outer_etam = -999; // aiaiaiaiaaaaaiiii
	     if (TMath::IsNaN(outer_etap)) outer_etap = -999; // aiaiaiaiaaaaaiiii
	 }
	 if (isCloseToIP_in && isCloseToIP_out) {
	     ++NGoodTracks;

	 h_ct1->Fill(1);	 

//       Check 1x3 condition (dphi,deta)=1,2 
	     int intow_phi = int(inner_phip*36/TMath::Pi())-int(outer_phip*36/TMath::Pi());
	     double intow_eta = TMath::Abs(inner_etap-outer_etap);
             if(abs(intow_phi)>0||intow_eta>0.174) continue;
         h_ct2->Fill(1);
//       90 degree correction:
	     GlobalVector v1(track->outerMomentum().x(), track->outerMomentum().y(),track->outerMomentum().z());
	     GlobalVector vc1((inner_xp+outer_xp)/2,(inner_yp+outer_yp)/2.,0.);
             f_corr = TMath::Abs((v1.dot(vc1))/(v1.mag()*vc1.mag()));
	     if(f_corr<0.3)
		{
		   cout<<"f_corr : "<<f_corr<<endl;
		   cout<<"v1 : "<<v1<<endl;
		   cout<<"vc1 : "<<vc1<<endl;
		}
             h_corr->Fill(f_corr);
//       Look for hcalDetId
	     cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"<<endl;
	     fhcalDetId = getClosestCell(0.125, (inner_etap+outer_etap)/2, (inner_phip+outer_phip)/2, HBHERecHits, geo,found1);
             cout<<"#############################"<<endl;
//	 hcal id is good or ...?
	     if(!found1)continue;
         h_ct3->Fill(1);

	     int ieta = fhcalDetId.ieta();
	     int iphi = fhcalDetId.iphi();

	     h_HB_IPhi->Fill(iphi);
	     h_HB_IEta->Fill(ieta);
             h_EtaPhi->Fill(ieta,iphi);

//	 HB Emax variables
	     int HBMeta = -9999;
	     int HBMphi = -9999;
	     double HBMene = 0.;
	     double HBMene2 = 0.; // Max + next to max
	     getHBmax(iphi,HBHERecHits,&HBMeta,&HBMphi,&HBMene,&HBMene2);
	     if(HBMene>0.6)
	     {
	      h_HB_maxE->Fill(HBMene);
	      h_HB_maxE2->Fill(HBMene2);
	      h_HB_maxIEtaIPhi->Fill(HBMeta,HBMphi);
	      h_HB_maxIEta->Fill(HBMeta);
	      h_HB_maxIPhi->Fill(HBMphi);
	      h_HB_DPhi->Fill(iphi-HBMphi);
	      h_HB_DEta->Fill(ieta-HBMeta);
	      h_HBCorrEta->Fill(HBMeta,ieta);
	      h_HBCorrPhi->Fill(HBMphi,iphi);
	     }
//       Fill energy histos
	     if(iphi>=1&&iphi<=36)  // top HB	     
	     {
	        if(ieta < 0){   // top minus
		 h_HBM_e1[iphi-1]->Fill(f_corr*getNxNClusterEnergy(0,ieta,iphi,HBHERecHits));
		 if(ieta<-1)
		 {
		   h_HBM_e3[iphi-1]->Fill(f_corr*get1xNClusterEnergy(1,ieta,iphi,HBHERecHits));
		   h_HBTopMin_e1x3->Fill(get1xNClusterEnergy(1,ieta,iphi,HBHERecHits));
		   h_HBTopMin_e1x3cr->Fill(f_corr*get1xNClusterEnergy(1,ieta,iphi,HBHERecHits));
		 }
//		 h_HBM_e5[iphi-1]->Fill(get1xNClusterEnergy(2,ieta,iphi,HBHERecHits));
		 h_HBTopMin_e1x1->Fill(getNxNClusterEnergy(0,ieta,iphi,HBHERecHits));
		 h_HBTopMin_e3x3->Fill(getNxNClusterEnergy(1,ieta,iphi,HBHERecHits));
		 h_HBTopMin_e5x5->Fill(getNxNClusterEnergy(2,ieta,iphi,HBHERecHits));

                }else{ // top plus
		 h_HBP_e1[iphi-1]->Fill(f_corr*getNxNClusterEnergy(0,ieta,iphi,HBHERecHits));
		 if(ieta>1)
		 {
		   h_HBP_e3[iphi-1]->Fill(f_corr*get1xNClusterEnergy(1,ieta,iphi,HBHERecHits));
		   h_HBTopPlu_e1x3->Fill(get1xNClusterEnergy(1,ieta,iphi,HBHERecHits));
		   h_HBTopPlu_e1x3cr->Fill(f_corr*get1xNClusterEnergy(1,ieta,iphi,HBHERecHits));
		 }
//		 h_HBP_e5[iphi-1]->Fill(get1xNClusterEnergy(2,ieta,iphi,HBHERecHits));
		 h_HBTopPlu_e1x1->Fill(getNxNClusterEnergy(0,ieta,iphi,HBHERecHits));
		 h_HBTopPlu_e3x3->Fill(getNxNClusterEnergy(1,ieta,iphi,HBHERecHits));
		 h_HBTopPlu_e5x5->Fill(getNxNClusterEnergy(2,ieta,iphi,HBHERecHits));
                }
//      Bottom 
             }else{ // bottom
	        if(ieta < 0){   // bottom minus
		 h_HBM_e1[iphi-1]->Fill(f_corr*getNxNClusterEnergy(0,ieta,iphi,HBHERecHits));
		 if(ieta<-1)
		 {
		   h_HBM_e3[iphi-1]->Fill(f_corr*get1xNClusterEnergy(1,ieta,iphi,HBHERecHits));
		   h_HBBottomMin_e1x3->Fill(get1xNClusterEnergy(1,ieta,iphi,HBHERecHits));
		   h_HBBottomMin_e1x3cr->Fill(f_corr*get1xNClusterEnergy(1,ieta,iphi,HBHERecHits));
		 }
//		 h_HBM_e5[iphi-1]->Fill(get1xNClusterEnergy(2,ieta,iphi,HBHERecHits));
		 h_HBBottomMin_e1x1->Fill(getNxNClusterEnergy(0,ieta,iphi,HBHERecHits));
		 h_HBBottomMin_e3x3->Fill(getNxNClusterEnergy(1,ieta,iphi,HBHERecHits));
		 h_HBBottomMin_e5x5->Fill(getNxNClusterEnergy(2,ieta,iphi,HBHERecHits));
                }else{ // bottom plus
		 h_HBM_e1[iphi-1]->Fill(f_corr*getNxNClusterEnergy(0,ieta,iphi,HBHERecHits));
		 if(ieta>1)
		 {
		   h_HBP_e3[iphi-1]->Fill(f_corr*get1xNClusterEnergy(1,ieta,iphi,HBHERecHits));
		   h_HBBottomPlu_e1x3->Fill(get1xNClusterEnergy(1,ieta,iphi,HBHERecHits));
		   h_HBBottomPlu_e1x3cr->Fill(f_corr*get1xNClusterEnergy(1,ieta,iphi,HBHERecHits));
		 }
//		 h_HBM_e5[iphi-1]->Fill(get1xNClusterEnergy(2,ieta,iphi,HBHERecHits));
		 h_HBBottomPlu_e1x1->Fill(getNxNClusterEnergy(0,ieta,iphi,HBHERecHits));
		 h_HBBottomPlu_e3x3->Fill(getNxNClusterEnergy(1,ieta,iphi,HBHERecHits));
		 h_HBBottomPlu_e5x5->Fill(getNxNClusterEnergy(2,ieta,iphi,HBHERecHits));
                }
	     }
         }
     ++nTrack;
    }
   //@@

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
       if (abs(hhit->id().ieta()) > 16) continue;
       //HB-
       if (hhit->id().ieta()<0){
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
       }
       //HB+
       if (hhit->id().ieta()>0){
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
   }


 
   for (HFRecHitCollection::const_iterator hhit=Hithf.begin(); hhit!=Hithf.end(); hhit++) {
     h_hf_rechit_energy->Fill(hhit->energy());
     h_hf_eta_phi->Fill((hhit->id()).ieta(),(hhit->id()).iphi());
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
  if (clusterCollection_p) { // do ECAL clusters business
    for (reco::BasicClusterCollection::const_iterator clus = clusterCollection_p->begin(); 
	 clus != clusterCollection_p->end(); ++clus)
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
	  next_to_maxebeerec_PHI = clus->phi();
	}
      }
  }
  

  float total_hbhe = maxhbherec+next_to_maxhbherec;
  float total_ebee = maxebeerec+next_to_maxebeerec;
  float total_ho = maxhorec+next_to_maxhorec;
  float total_hbMinus = maxhbMinusrec+next_to_maxhbMinusrec;
  float total_hbPlus = maxhbPlusrec+next_to_maxhbPlusrec;
  float hbhe_DR = sqrt(pow((maxhbherec_ETA-next_to_maxhbherec_ETA),2)+pow((maxhbherec_PHI-next_to_maxhbherec_PHI),2));
  float ebee_DR = sqrt(pow((maxebeerec_ETA-next_to_maxebeerec_ETA),2)+pow((maxebeerec_PHI-next_to_maxebeerec_PHI),2));
  float ho_DR = sqrt(pow((maxhorec_ETA-next_to_maxhorec_ETA),2)+pow((maxhorec_PHI-next_to_maxhorec_PHI),2));
  float hbMinus_DR = sqrt(pow((maxhbMinusrec_ETA-next_to_maxhbMinusrec_ETA),2)+pow((maxhbMinusrec_PHI-next_to_maxhbMinusrec_PHI),2));
  float hbPlus_DR = sqrt(pow((maxhbPlusrec_ETA-next_to_maxhbPlusrec_ETA),2)+pow((maxhbPlusrec_PHI-next_to_maxhbPlusrec_PHI),2));
  if (hbhe_DR < 2.5 && total_hbhe > 0. ) h_maxhbherec->Fill(total_hbhe);
  if (ebee_DR < 0.4 && total_ebee > 0. ) h_maxebeerec->Fill(total_ebee);
  if (ho_DR < 2.5 && total_ho > 0.) h_maxhorec->Fill(total_ho);
  if ((hbhe_DR < 2.5 && total_hbhe > 0.) && (ebee_DR < 0.4 && total_ebee > 0.) ) h_maxebee_plus_maxhbhe->Fill(total_ebee+total_hbhe);
  if (hbMinus_DR < 2.5 && total_hbMinus > 0.) h_maxhbMinusrec->Fill(total_hbMinus);  
  if (hbPlus_DR < 2.5 && total_hbPlus > 0.) h_maxhbPlusrec->Fill(total_hbPlus);


  for(TrackCollection::const_iterator ncm = muons->begin(); ncm != muons->end();  ++ncm) {
    h_muon_vertex_x->Fill(ncm->vx());
    h_muon_px->Fill(ncm->px());
    h_muon_p->Fill(ncm->p());
  }

  float HT = 0;
  float MHTx = 0;
  float MHTy = 0;
  float MHT = 0;
  std::vector < CaloJet > myjet;
    for (CaloJetCollection::const_iterator jetiter = cjet.begin(); jetiter != cjet.end(); jetiter++) {
        h_jet_Pt->Fill(jetiter->pt());
        h_jet_Eta->Fill(jetiter->eta());
        h_jet_Phi->Fill(jetiter->phi());
        myjet.push_back(*jetiter);
	if (jetiter->pt() > 5.){
	  HT += jetiter->pt();
	  MHTx += jetiter->px();
	  MHTy += jetiter->py();	 
	} 
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

    h_HT->Fill(HT);
    MHT = sqrt(MHTx*MHTx+MHTy*MHTy);
    h_MHT->Fill(MHT);


  for (CaloTowerCollection::const_iterator kal=calohbhe.begin(); kal!=calohbhe.end(); kal++){	
    h_calo_tower_energy->Fill(kal->energy());
  }



  //correlations
  for(TrackCollection::const_iterator ncm = muons->begin(); ncm != muons->end();  ++ncm) {
    if (clusterCollection_p) {
      for (reco::BasicClusterCollection::const_iterator clus = clusterCollection_p->begin(); clus != clusterCollection_p->end(); ++clus){
	h_ecalx_vs_muonx->Fill(clus->x(),ncm->vx());
	h_ecaly_vs_muony->Fill(clus->y(),ncm->vy());
	h_impact_diff->Fill(clus->z()-ncm->vx());
      }
      for (CaloJetCollection::const_iterator jetiter=cjet.begin(); jetiter!=cjet.end(); jetiter++){
	h_jetphi_vs_muonphi->Fill(jetiter->phi(),ncm->phi());
      } 
    }
  }
  
}


// ------------ method called once each job just before starting event loop  ------------
void HcalProm::beginJob(const edm::EventSetup&)
{
}

TH1F* HcalProm::book1DHistogram (TFileDirectory& fDir, const std::string& fName, const std::string& fTitle, 
				 int fNbins, double fXmin, double fXmax) const {
  char title [1024];
  sprintf (title, "%s [RUN:%ld/%ld]", fTitle.c_str(), runBegin, lumibegin);
  std::cout << "booking 1d histogram " << title << std::endl;
  return fDir.make<TH1F>(fName.c_str(), title, fNbins, fXmin, fXmax);
}

TH2F* HcalProm::book2DHistogram (TFileDirectory& fDir, const std::string& fName, const std::string& fTitle, 
				 int fNbinsX, double fXmin, double fXmax,
				 int fNbinsY, double fYmin, double fYmax
				 ) const {
  char title [1024];
  sprintf (title, "%s [RUN:%ld/%ld]", fTitle.c_str(), runBegin, lumibegin);
  std::cout << "booking 2d histogram " << title << std::endl;
  return fDir.make<TH2F>(fName.c_str(), title, fNbinsX, fXmin, fXmax, fNbinsY, fYmin, fYmax);
}





void HcalProm::bookHistograms () {

    TFileDirectory EcalDir = fs->mkdir( "Ecal" );
    TFileDirectory HcalDir = fs->mkdir( "Hcal" );
    TFileDirectory MuonDir = fs->mkdir( "Muon" );
    TFileDirectory CorrDir = fs->mkdir( "Correlations" );
    TFileDirectory NoiseDir = fs->mkdir( "Noise" );
    TFileDirectory JetMetDir = fs->mkdir( "JetMet" );
    TFileDirectory TriggerDir = fs->mkdir( "Trigger" );

    //Add runnumbers to histograms!
    
    h_global_trigger_bit =  book1DHistogram (TriggerDir, "h_global_trigger_bit","Global Trigger Bit Fired",128,-0.5,127.5);
    h_hbhe_rechit_energy =  book1DHistogram (HcalDir, "h_hbhe_rechit_energy","RecHit Energy HBHE",160,-10,30);
    h_hf_rechit_energy =  book1DHistogram (HcalDir, "h_hf_rechit_energy","RecHit Energy HF",160,-10,30);
    
    h_maxhbherec =  book1DHistogram (HcalDir, "h_maxhbherec","HBHE Muon (GeV)",200,0,15);
    h_maxhorec =  book1DHistogram (HcalDir, "h_maxhorec","HO Muon (GeV)",200,0,15);
    h_maxhbMinusrec =  book1DHistogram (HcalDir, "h_maxhbMinusrec","HB- Muon (GeV)",200,0,15);
    h_maxhbPlusrec =  book1DHistogram (HcalDir, "h_maxhbPlusrec","HB+ Muon (GeV)",200,0,15);
    h_hbhe_eta_phi =  book2DHistogram (HcalDir, "h_hbhe_eta_phi","#eta(HBHE)",60,-30,30,72,0,72);
    h_hf_eta_phi =  book2DHistogram (HcalDir, "h_hf_eta_phi","#eta(HF)",96,-48,48,72,0,72);
    h_ho_rechit_energy =  book1DHistogram (HcalDir, " h_ho_rechit_energy","RecHit Energy HO",160,-10,30);
    h_ho_eta_phi =  book2DHistogram (HcalDir, "h_ho_eta_phi","#eta(HO)",60,-30,30,72,0,72);
    
    h_hbtiming =  book1DHistogram (HcalDir, "h_hbtiming","HBHE Timing",10,-0.5,9.5);
    
    h_jet_Pt =  book1DHistogram (JetMetDir, "h_jet_Pt", "Jet PT", 130, -10, 120);
    h_jet_Eta =  book1DHistogram (JetMetDir, "h_jet_Eta", "Jet Eta", 100, -7, 7);
    h_jet_Phi =  book1DHistogram (JetMetDir, "h_jet_Phi", "Jet Phi", 100, -7, 7);
    h_leadJet_Pt =  book1DHistogram (JetMetDir, "h_leadJet_Pt", "Leading Jet PT", 120, 0, 120);
    h_leadJet_Eta =  book1DHistogram (JetMetDir, "h_leadJet_Eta", "Leading Jet Eta", 100, -7, 7);
    h_leadJet_Phi =  book1DHistogram (JetMetDir, "h_leadJet_Phi", "Leading Jet Phi", 100, -7, 7);
    h_caloMet_Met =  book1DHistogram (JetMetDir, "h_caloMet_Met", "MET from CaloTowers", 100, 0, 50);
    h_caloMet_Phi =  book1DHistogram (JetMetDir, "h_caloMet_Phi", "MET #phi from CaloTowers", 100, -7, 7);
    h_caloMet_SumEt =  book1DHistogram (JetMetDir, "h_caloMet_SumEt", "SumET from CaloTowers", 100, 0, 50);
    h_MHT =  book1DHistogram (JetMetDir, "h_MHT","MHT", 600,-10,200);
    h_HT =  book1DHistogram (JetMetDir, "h_HT","HT", 600,-10,200);

    h_eb_rechit_energy =  book1DHistogram (EcalDir, " h_eb_rechit_energy","RecHit Energy EB",160,-10,30);
    h_maxebeerec =  book1DHistogram (EcalDir, "h_maxebeerec","EBEE Muon (GeV)",200,0,15);
    h_ecal_cluster_energy =  book1DHistogram (EcalDir, "h_ecal_cluster_energy","EB Cluster Energy",130,-10,120);
    h_ecal_cluster_eta =  book1DHistogram (EcalDir, "h_ecal_cluster_eta","#eta(EB Cluster)",100,-6,6);
    h_ecal_cluster_phi =  book1DHistogram (EcalDir, "h_ecal_cluster_phi","#phi(EB Cluster)",100,-6,6);
  
    h_maxebee_plus_maxhbhe =  book1DHistogram (CorrDir, "h_maxebee_plus_maxhbhe","EBEE+HBHE Muon (GeV)",200,0,15);

    h_ecal_vs_hcal_X = book2DHistogram (CorrDir, "h_ecal_vs_hcal_X","X(EB) vs X(HB)",1000,-100,100,100,-100,100);
    h_ecal_vs_hcal_Y = book2DHistogram (CorrDir, "h_ecal_vs_hcal_Y","Y(EB) vs Y(HB)",1000,-100,100,100,-100,100);

    h_calo_tower_energy =  book1DHistogram (JetMetDir, "h_calo_tower_energy","Calo Tower Energy",130,-10,120);

    h_muon_vertex_x =  book1DHistogram (MuonDir, "h_muon_vertex_x", "Muon Vertex X", 10000, -1000, 1000);
    h_muon_px = book1DHistogram (MuonDir, "h_muon_px", "Px(#mu)", 1000, -10, 100);
    h_muon_p = book1DHistogram (MuonDir, "h_muon_p", "P(#mu)", 1000, -10, 100);

    h_ecalx_vs_muonx = book2DHistogram (CorrDir, "h_ecalx_vs_muonx","h_ecalx_vs_muonx",1000,-400,400,1000,-400,400);
    h_ecaly_vs_muony = book2DHistogram (CorrDir, "h_ecaly_vs_muony","h_ecaly_vs_muony",1000,-1000,1000,1000,-1000,1000);
    h_impact_diff = book1DHistogram (CorrDir, "h_impact_diff","h_impact_diff",1000,-200,200);
    h_jetphi_vs_muonphi = book2DHistogram (CorrDir, "h_jetphi_vs_muonphi","h_jetphi_vs_muonphi",100,-6,6,100,-6,6);
    
    DT_HCAL_eta_correlation =
      book2DHistogram (MuonDir, "DT_CalTower_eta_correlation", "DT eta vs Calo eta at r= 286.4cm", 800, -4., 4., 800, -4.,
      4.);
    DT_HCAL_eta_correlation_all =
      book2DHistogram (MuonDir, "DT_CalTower_eta_correlation_all", "DT eta vs Calo eta at r= 286.4cm", 800, -4., 4., 800,
      -4., 4.);
    DT_HCAL_phi_correlation =
      book2DHistogram (MuonDir, "DT_CalTower_phi_correlation", "DT phi vs Calo phi at r= 286.4cm", 800, -4., 4., 800, -4.,
      4.);
    DT_HCAL_phi_correlation_all =
      book2DHistogram (MuonDir, "DT_CalTower_phi_correlation_all", "DT phi vs Calo phi at r= 286.4cm", 800, -4., 4., 800,
      -4., 4.);
    HCAL_energy_correlation =
      book1DHistogram (MuonDir, "CalTower_energy_correlation", "Calo had energy at r= 286.4cm", 800, -4., 4.);
    HCAL_energy_correlation_all =
      book1DHistogram (MuonDir, "CalTower_energy_correlation_all", "Calo had energy at r= 286.4cm", 800, -4., 4.);

    //@@
    h_AllTracks = book1DHistogram(MuonDir,"NAllTracks", "Nb. of Reconstructed Cosmic Muon Tracks", 10, -0.5, 9.5);
    h_ct1 = book1DHistogram(MuonDir,"ct1", "Found good track in zHB and IPxy  ", 2, -0.5, 1.5);
    h_ct2 = book1DHistogram(MuonDir,"ct2", "1x3 Condition is satisfied (same iphi, deta < 2tow) ", 2, -0.5, 1.5);
    h_ct3 = book1DHistogram(MuonDir,"ct3", "Found closest HB cell  ", 2, -0.5, 1.5);
    h_ct4 = book1DHistogram(MuonDir,"ct4", " 2 tracks with Nrec hits>24", 2, -0.5, 1.5);
    h_ct5 = book1DHistogram(MuonDir,"ct5", " ", 2, -0.5, 1.5);
    h_ct6 = book1DHistogram(MuonDir,"ct6", " ", 2, -0.5, 1.5);
    h_ct7 = book1DHistogram(MuonDir,"ct7", " ", 2, -0.5, 1.5);
    h_ct8 = book1DHistogram(MuonDir,"ct8", " ", 2, -0.5, 1.5);
    h_ct9 = book1DHistogram(MuonDir,"ct9", " ", 2, -0.5, 1.5);
    h_Trigger = book1DHistogram (MuonDir, "Trigger", "Trigger Types", 4, 0, 4);
    h_Trigger2 = book1DHistogram (MuonDir, "Trigger2", "Trigger Types", 4, 0, 4);
    h_NTracks1 = book1DHistogram(MuonDir,"NTracks1", "Nb. of Cosmic Muon Tracks", 10, -0.5, 9.5);
    h_NTracks2 = book1DHistogram(MuonDir,"NTracks2", "Nb. of Cosmic Muon Tracks", 10, -0.5, 9.5);
    h_NPTracks = book1DHistogram(MuonDir,"NPTracks", "Nb. of Cosmic Muon Tracks in +", 10, -0.5, 9.5);
    h_NMTracks = book1DHistogram(MuonDir,"NMTracks", "Nb. of Cosmic Muon Tracks in -", 10, -0.5, 9.5);
    h_NMuons = book1DHistogram(MuonDir,"NMuons", "Nb. of Cosmic Muon", 10, -0.5, 9.5);
    h_Pt = book1DHistogram (MuonDir,"Pt", "Muon PT", 100, 0., 100.);
    h_Px = book1DHistogram (MuonDir,"Px", "Muon Px", 100, 0., 100.);
    h_Py = book1DHistogram (MuonDir,"Py", "Muon Py", 200, -1000., 1000.);
    h_d0 = book1DHistogram (MuonDir,"d0", "Transverse Impact Parameter", 500, 0., 500.);
    h_Chi2 = book1DHistogram (MuonDir,"Chi2", "Track Chi2", 100, 0., 100.);
    h_Vtx_X = book1DHistogram (MuonDir,"Vtx_X", "Track Vertex X", 2000, -1000., 1000.);
    h_Vtx_Y = book1DHistogram (MuonDir,"Vtx_Y", "Track Vertex Y", 2000, -1000., 1000.);
    h_Vtx_Z = book1DHistogram (MuonDir,"Vtx_Z", "Track Vertex Z", 1000, -5000., 5000.);
    h_Vtx_R1 = book1DHistogram (MuonDir,"Vtx_R1", "Track Vertex R", 1000, 0., 5000.);
    h_Vtx_R2 = book1DHistogram(MuonDir,"Vtx_R2", "Track Vertex R2", 1000, 0., 5000.);
    h_diff = book1DHistogram(MuonDir,"h_diff", "Track Diff", 5000, -5000., 5000.);
    h_InnerPos_X = book1DHistogram(MuonDir,"InnerPos_X", "Track Inner Position X", 2000, -1000., 1000.);
    h_InnerPos_Y = book1DHistogram(MuonDir,"InnerPos_Y", "Track Inner Position Y", 2000, -1000., 1000.);
    h_InnerPos_Z = book1DHistogram(MuonDir,"InnerPos_Z", "Track Inner Position Z", 2000, -1000., 1000.);
    h_OuterPos_X = book1DHistogram(MuonDir,"OuterPos_X", "Track Outer Position X", 2000, -1000., 1000.);
    h_OuterPos_Y = book1DHistogram(MuonDir,"OuterPos_Y", "Track Outer Position Y", 2000, -1000., 1000.);
    h_OuterPos_Z = book1DHistogram(MuonDir,"OuterPos_Z", "Track Outer Position Z", 2000, -1000., 1000.);
    h_Angle1 = book1DHistogram(MuonDir,"Angle1", "CosTheta between Muon Tracks", 220, -1.1, 1.1);
    h_Angle2 = book1DHistogram(MuonDir,"Angle2", "CosTheta between Muon Tracks", 220, -1.1, 1.1);
    h_Pt_Top = book1DHistogram(MuonDir,"Pt_Top", "Top Muon PT", 100, 0., 100.);
    h_Pt_Bottom = book1DHistogram(MuonDir,"Pt_Bottom", "Bottom Muon PT", 100, 0., 100.);
    h_Eta_Top = book1DHistogram(MuonDir,"Eta_Top", "Top Muon Eta", 100, -5., 5.);
    h_Eta_Bottom = book1DHistogram(MuonDir,"Eta_Bottom", "Bottom Muon Eta", 100, -5, 5.);
    h_Phi_Top = book1DHistogram(MuonDir,"Phi_Top", "Top Muon Phi", 100, -4., 4.);
    h_Phi_Bottom = book1DHistogram(MuonDir,"Phi_Bottom", "Bottom Muon Phi", 100, -4, 4.);
    h_dR_Top = book1DHistogram(MuonDir,"dR_Top", "Top dR between Muon Track and HB Cells", 100, 0., 5.);
    h_dR_Bottom = book1DHistogram(MuonDir,"dR_Bottom", "Bottom dR between Muon Track and HB Cells", 100, 0., 5.);
    h_HBP_e1x1 = book1DHistogram(MuonDir,"hbPlus1x1", "HB+ 1x1 cluster energy", 100, -10., 10.);
    h_HBP_e3x3 = book1DHistogram(MuonDir,"hbPlus3x3", "HB+ 3x3 cluster energy", 100, -10., 10.);
    h_HBP_e5x5 = book1DHistogram(MuonDir,"hbPlus5x5", "HB+ 5x5 cluster energy", 100, -10., 10.);
    h_HBM_e1x1 = book1DHistogram(MuonDir,"hbMinus1x1", "HB- 1x1 cluster energy", 100, -10., 10.);
    h_HBM_e3x3 = book1DHistogram(MuonDir,"hbMinus3x3", "HB- 3x3 cluster energy", 100, -10., 10.);
    h_HBM_e5x5 = book1DHistogram(MuonDir,"hbMinus5x5", "HB- 5x5 cluster energy", 100, -10., 10.);
    PROJ_eta = book1DHistogram(MuonDir,"PROJ_eta", "PROJ_eta", 80, -4., 4.);
    PROJ_phi = book1DHistogram(MuonDir,"PROJ_phi", "PROJ_phi", 63, 0., 6.3);
    FIN_zdist = book1DHistogram(MuonDir,"FIN_zdist", "FIN_zdist", 140, -140., 140.);
    h_EtaPhi = book2DHistogram(MuonDir,"h_EtaPhi", "Propagated tower i#eta-i#phi", 60, -30, 30,72, 0,72);

	 char name[1024];
	 char title[1024];
    for(int i=0;i<72;i++)
	{
          sprintf(name,"h_HBP_e1_phi%i",i);
          sprintf(title,"HB+ energy in 1x1 phi%i",i);
	  h_HBP_e1[i] = book1DHistogram(MuonDir,name, title , 500, -10., 90.);

          sprintf(name,"h_HBP_e3_phi%i",i);
          sprintf(title,"HB+ energy in 3x3 phi%i",i);
	  h_HBP_e3[i] = book1DHistogram(MuonDir,name, title , 500, -10., 90.);

          sprintf(name,"h_HBP_e5_phi%i",i);
          sprintf(title,"HB+ energy in 5x5 phi%i",i);
	  h_HBP_e5[i] = book1DHistogram(MuonDir,name, title , 500, -10., 90.);


          sprintf(name,"h_HBM_e1_phi%i",i);
          sprintf(title,"HB- energy in 1x1 phi%i",i);
	  h_HBM_e1[i] = book1DHistogram(MuonDir,name, title , 500, -10., 90.);

          sprintf(name,"h_HBM_e3_phi%i",i);
          sprintf(title,"HB- energy in 3x3 phi%i",i);
	  h_HBM_e3[i] = book1DHistogram(MuonDir,name, title , 500, -10., 90.);

          sprintf(name,"h_HBM_e5_phi%i",i);
          sprintf(title,"HB- energy in 5x5 phi%i",i);
	  h_HBM_e5[i] = book1DHistogram(MuonDir,name, title , 500, -10., 90.);

	}
    
    h_HB_maxE = book1DHistogram(MuonDir,"h_HB_maxE", "Maximum HB Energy (>0.6GeV)", 440, -10., 100.);
    h_HB_maxE2 = book1DHistogram(MuonDir,"h_HB_maxE2", "Maximum HB + next neighbor Energy", 440, -10., 100.);
    h_HB_maxEta = book1DHistogram(MuonDir,"h_HB_maxEta", "Maximum HB Eta", 1000, -5., 5.);
    h_HB_maxPhi = book1DHistogram(MuonDir,"h_HB_maxPhi", "Maximum HB Phi", 1000, -5., 5.);
    h_HB_maxIPhi = book1DHistogram(MuonDir,"h_HB_maxIPhi", "Maximum HB iphi", 73, 0, 73);
    h_HB_maxIEta = book1DHistogram(MuonDir,"h_HB_maxIEta", "Maximum HB ieta", 60, -30, 30);
    h_HB_maxIEtaIPhi = book2DHistogram(MuonDir,"h_HB_maxIEtaIPhi", "Maximum HB ieta:iphi", 60, -30, 30,73, 0, 73);
    h_HB_IPhi = book1DHistogram(MuonDir,"h_HB_IPhi", "HB iphi", 73, 0, 73);
    h_HB_IEta = book1DHistogram(MuonDir,"h_HB_IEta", "HB ieta", 60, -30, 30);
    h_HB_DPhi = book1DHistogram(MuonDir,"h_HB_DPhi", "HB dphi", 730, -73, 73);
    h_HB_DEta = book1DHistogram(MuonDir,"h_HB_DEta", "HB deta", 600, -30, 30);
    h_HBEnt_IPhi = book1DHistogram(MuonDir,"h_HBEnt_IPhi", "HB entry iphi", 73, 0, 73);
    h_HBEnt_IEta = book1DHistogram(MuonDir,"h_HBEnt_IEta", "HB entry ieta", 120, -60, 60);
    h_HBExt_IPhi = book1DHistogram(MuonDir,"h_HBExt_IPhi", "HB exit iphi", 73, 0, 73);
    h_HBExt_IEta = book1DHistogram(MuonDir,"h_HBExt_IEta", "HB exit ieta", 240, -120, 120);
    
    h_HBCorrEta = book2DHistogram(MuonDir,"h_HBCorrEta", "Track Eta vs HB Eta, X:max, Y:propagated", 60, -30., 30., 60, -30., 30.);
    h_HBCorrPhi = book2DHistogram(MuonDir,"h_HBCorrPhi", "Track Phi vs HB Phi, X:max, Y:propagated", 72, 0., 72., 72, 0., 72.);
    h_HBCorrEta1 = book2DHistogram(MuonDir,"h_HBCorrEta1", "Track Eta vs HB Eta", 100, -5., 5., 100, -5., 5.);
    h_HBCorrPhi1 = book2DHistogram(MuonDir,"h_HBCorrPhi1", "Track Phi vs HB Phi", 120, -6., 6., 120, -6., 6.);
    h_HBCorrEta2 = book2DHistogram(MuonDir,"h_HBCorrEta2", "Track Eta vs HB Eta", 100, -5., 5., 100, -5., 5.);
    h_HBCorrPhi2 = book2DHistogram(MuonDir,"h_HBCorrPhi2", "Track Phi vs HB Phi", 120, -6., 6., 120, -6., 6.);
    
    h_Trk1_x = book1DHistogram(MuonDir,"h_Trk1_x", "Track X", 2000, -1000., 1000.);
    h_Trk1_y = book1DHistogram(MuonDir,"h_Trk1_y", "Track Y", 2000, -1000., 1000.);
    h_Trk1_z = book1DHistogram(MuonDir,"h_Trk1_z", "Track Z", 1000, -5000., 5000.);
    h_Trk2_x = book1DHistogram(MuonDir,"h_Trk2_x", "Track X", 2000, -1000., 1000.);
    h_Trk2_y = book1DHistogram(MuonDir,"h_Trk2_y", "Track Y", 2000, -1000., 1000.);
    h_Trk2_z = book1DHistogram(MuonDir,"h_Trk2_z", "Track Z", 1000, -5000., 5000.);
    
    h_InnerPos_x = book1DHistogram(MuonDir,"h_InnerPos_x", "Track Inner x", 1000, -5000., 5000.);
    h_InnerPos_y = book1DHistogram(MuonDir,"h_InnerPos_y", "Track Inner y", 1000, -5000., 5000.);
    h_InnerPos_z = book1DHistogram(MuonDir,"h_InnerPos_z", "Track Inner z", 1000, -5000., 5000.);
    h_InnerPos_eta = book1DHistogram(MuonDir,"h_InnerPos_eta", "Track Inner eta ", 100, -5., 5.);
    h_InnerPos_phi = book1DHistogram(MuonDir,"h_InnerPos_phi", "Track Inner phi ", 120, -6., 6.);
    h_OuterPos_x = book1DHistogram(MuonDir,"h_OuterPos_x", "Track Outer x", 1000, -5000., 5000.);
    h_OuterPos_y = book1DHistogram(MuonDir,"h_OuterPos_y", "Track Outer y", 1000, -5000., 5000.);
    h_OuterPos_z = book1DHistogram(MuonDir,"h_OuterPos_z", "Track Outer z", 1000, -5000., 5000.);
    h_OuterPos_eta = book1DHistogram(MuonDir,"h_OuterPos_eta", "Track Outer eta ", 100, -5., 5.);
    h_OuterPos_phi = book1DHistogram(MuonDir,"h_OuterPos_phi", "Track Outer phi ", 120, -6., 6.);
    h_distance3D = book1DHistogram(MuonDir,"h_distance3D", "distance 3D", 1000, 0., 1000.);
    h_distance2D = book1DHistogram(MuonDir,"h_distance2D", "distance 2D", 1000, 0., 2000.);
    h_muonNHits = book1DHistogram(MuonDir,"h_muonNHits", "Muon track rec hits", 100, 0, 100);
    
    h_distance2D_vs_HB = book2DHistogram(MuonDir,"h_distance2D_vs_HB", "distance 2D vs HB ene", 1000, 0., 1000., 150,-10.,140.);
    h_distance3D_vs_HB = book2DHistogram(MuonDir,"h_distance3D_vs_HB", "distance 3D vs HB ene", 1000, 0., 1000., 150,-10.,140.);
    h_distance2D_vs_nHits = book2DHistogram(MuonDir,"h_distance2D_vs_nHits", "distance 2D vs nHits", 1000, 0., 1000., 100,0.,100.);
    h_distance2D_vs_chi2 = book2DHistogram(MuonDir,"h_distance2D_vs_chi2", "distance 2D vs chi2", 1000, 0., 1000., 100,0.,1000.);
      
    h_HBTopMin_e1x1 = book1DHistogram(MuonDir,"h_HBTopMin_e1x1", "HB- Top Sector 1x1 cluster energy", 120, -10., 50.);
    h_HBTopMin_e3x3 = book1DHistogram(MuonDir,"h_HBTopMin_e3x3", "HB- Top Sector 3x3 cluster energy", 120, -10., 50.);
    h_HBTopMin_e1x3 = book1DHistogram(MuonDir,"h_HBTopMin_e1x3", "HB- Top Sector 1x3 cluster energy", 600, -10., 50.);
    h_HBTopMin_e1x3cr = book1DHistogram(MuonDir,"h_HBTopMin_e1x3cr", "HB- Top Sector 1x3 cluster energy corr", 600, -10., 50.);
    h_HBTopMin_e5x5 = book1DHistogram(MuonDir,"h_HBTopMin_e5x5", "HB- Top Sector 5x5 cluster energy", 120, -10., 50.);
    h_HBTopPlu_e1x1 = book1DHistogram(MuonDir,"h_HBTopPlu_e1x1", "HB+ Top Sector 1x1 cluster energy", 120, -10., 50.);
    h_HBTopPlu_e3x3 = book1DHistogram(MuonDir,"h_HBTopPlu_e3x3", "HB+ Top Sector 3x3 cluster energy", 120, -10., 50.);
    h_HBTopPlu_e1x3 = book1DHistogram(MuonDir,"h_HBTopPlu_e1x3", "HB+ Top Sector 1x3 cluster energy", 600, -10., 50.);
    h_HBTopPlu_e1x3cr = book1DHistogram(MuonDir,"h_HBTopPlu_e1x3cr", "HB+ Top Sector 1x3 cluster energy corr", 600, -10., 50.);
    h_HBTopPlu_e5x5 = book1DHistogram(MuonDir,"h_HBTopPlu_e5x5", "HB+ Top Sector 5x5 cluster energy", 120, -10., 50.);
    
    h_HBBottomMin_e1x1 = book1DHistogram(MuonDir,"h_HBBottomMin_e1x1", "HB- Bottom Sector 1x1 cluster energy", 120, -10., 50.);
    h_HBBottomMin_e3x3 = book1DHistogram(MuonDir,"h_HBBottomMin_e3x3", "HB- Bottom Sector 3x3 cluster energy", 120, -10., 50.);
    h_HBBottomMin_e1x3 = book1DHistogram(MuonDir,"h_HBBottomMin_e1x3", "HB- Bottom Sector 1x3 cluster energy", 600, -10., 50.);
    h_HBBottomMin_e1x3cr = book1DHistogram(MuonDir,"h_HBBottomMin_e1x3cr", "HB- Bottom Sector 1x3 cluster energy corr", 600, -10., 50.);
    h_HBBottomMin_e5x5 = book1DHistogram(MuonDir,"h_HBBottomMin_e5x5", "HB- Bottom Sector 5x5 cluster energy", 120, -10., 50.);
    h_HBBottomPlu_e1x1 = book1DHistogram(MuonDir,"h_HBBottomPlu_e1x1", "HB+ Bottom Sector 1x1 cluster energy", 120, -10., 50.);
    h_HBBottomPlu_e3x3 = book1DHistogram(MuonDir,"h_HBBottomPlu_e3x3", "HB+ Bottom Sector 3x3 cluster energy", 120, -10., 50.);
    h_HBBottomPlu_e1x3 = book1DHistogram(MuonDir,"h_HBBottomPlu_e1x3", "HB+ Bottom Sector 1x3 cluster energy", 600, -10., 50.);
    h_HBBottomPlu_e1x3cr = book1DHistogram(MuonDir,"h_HBBottomPlu_e1x3cr", "HB+ Bottom Sector 1x3 cluster energy corr", 600, -10., 50.);
    h_HBBottomPlu_e5x5 = book1DHistogram(MuonDir,"h_HBBottomPlu_e5x5", "HB+ Bottom Sector 5x5 cluster energy", 120, -10., 50.);
    h_corr = book1DHistogram(MuonDir,"h_corr", "90deg correction it self", 210, -1., 2.);
    //@@
    
    /*
    h_Qiesum = book1DHistogram (NoiseDir, "h_Qiesum", "Qiesum All Channels", 30000, -100., 29900.);
    string Noise[9] = { "1", "5", "20", "80", "150", "300", "600", "1000", "1500" };
    for (int hi = 0; hi != 9; ++hi) {
        if (hi != 8) {
            h_NoiseChan[hi] =
              book2DHistogram (NoiseDir, string("h_Noise_phi_vs_eta_" + Noise[hi] + "-" + Noise[hi + 1]).c_str(),
              string("Noise i#phi vs i#eta between " + Noise[hi] + " and " + Noise[hi + 1] + "GeV").c_str(), 40, -19.5,
              20.5, 73, -.5, 72.5);
            string title;

            title = "Events between " + Noise[hi] + " and " + Noise[hi + 1] + "GeV";
            h_NoisePhi[hi] =
              NoiseDir.make < TH1F > (string("h_Noise_Phi_" + Noise[hi] + "-" + Noise[hi + 1]).c_str(), title.c_str(),
              73, -.5, 72.5);
        } else {
            h_NoiseChan[hi] = book2DHistogram (NoiseDir, string("h_Noise_phi_vs_eta_" + Noise[hi]).c_str(),
              string("Noise i#phi vs i#eta above " + Noise[hi] + "GeV").c_str(), 40, -19.5, 20.5, 73, -.5, 72.5);
            string title;

            title = "Events above " + Noise[hi];
            title = title + "GeV vs i#phi";
            h_NoisePhi[hi] =
              book1DHistogram (NoiseDir, string("h_Noise_Phi_" + Noise[hi]).c_str(), title.c_str(), 73, -.5, 72.5);
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

    if (runBegin != -1)
      sprintf(tmp, "HcalPrompt_R%09ld_L%ld_%ld", runBegin,lumibegin,lumiend);
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
    htmlFile << "<h2>Run Number:&nbsp;&nbsp;" << endl;
    htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << runBegin << "</span>" << endl;

    htmlFile << "&nbsp;&nbsp;LS:&nbsp;" << endl; 
    htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << lumibegin << "</span>" << endl;

    htmlFile << "-" << endl; 
    htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << lumiend << "</span>" << endl;

    htmlFile << "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Start&nbsp;Time:&nbsp;<span style=\"color: rgb(0, 0, 153);\">" <<
      startTime << "</span></h2> " << endl;
    htmlFile << "<h2>Events processed:&nbsp;&nbsp;&nbsp;" << endl;
    htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << evtNo << "</span></h2> " << endl;
    htmlFile << "<hr>" << endl;
    htmlFile << "<ul>" << endl;

    string htmlName;

    if (doRecHitHTML) {
        htmlName = "HcalPromptRecHit.html";
        RecHitHTMLOutput(startTime, htmlDir, htmlName);
        htmlFile << "<table border=0 WIDTH=\"50%\"><tr>" << endl;
        htmlFile << "<td WIDTH=\"35%\"><a href=\"" << htmlName << "\">RecHit Prompt</a></td>" << endl;

    }
    if (doDigiHTML) {
        htmlName = "HcalPromptDigi.html";
        DigiHTMLOutput(startTime, htmlDir, htmlName);
        htmlFile << "<table border=0 WIDTH=\"50%\"><tr>" << endl;
        htmlFile << "<td WIDTH=\"35%\"><a href=\"" << htmlName << "\">Digi Prompt</a></td>" << endl;
    }
    if (doJetMetHTML) {
        htmlName = "HcalPromptJetMet.html";
        // htmlOutput(runBegin,lumi, startTime,htmlDir, htmlName,htmlTitle);
        JetMetHTMLOutput(startTime, htmlDir, htmlName);
        htmlFile << "<table border=0 WIDTH=\"50%\"><tr>" << endl;
        htmlFile << "<td WIDTH=\"35%\"><a href=\"" << htmlName << "\">JetMet Prompt</a></td>" << endl;
    }
    if (doMuonHTML) {
        htmlName = "HcalPromptMuon.html";
        MuonHTMLOutput(startTime, htmlDir, htmlName);
        htmlFile << "<table border=0 WIDTH=\"50%\"><tr>" << endl;
        htmlFile << "<td WIDTH=\"35%\"><a href=\"" << htmlName << "\">Muon Prompt</a></td>" << endl;
    }
    if (doHPDNoiseHTML) {
        htmlName = "HcalPromptHPDNoise.html";
        HPDNoiseHTMLOutput(startTime, htmlDir, htmlName);
        htmlFile << "<table border=0 WIDTH=\"50%\"><tr>" << endl;
        htmlFile << "<td WIDTH=\"35%\"><a href=\"" << htmlName << "\">HPD Noise Prompt</a></td>" << endl;
    }
    if (doCaloTowerHTML) {
        htmlName = "HcalPromptCaloTower.html";
        CaloTowerHTMLOutput(startTime, htmlDir, htmlName);
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


void HcalProm::JetMetHTMLOutput(string startTime, string htmlDir, string htmlName) {

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
    htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << runBegin << "</span>" << endl;

    htmlFile << "&nbsp;&nbsp;LS:&nbsp;" << endl; 
    htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << lumibegin << "</span>" << endl;

    htmlFile << "-" << endl; 
    htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << lumiend << "</span>" << endl;

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
            histoHTML(h_jet_Pt, "All Jets Pt", "Events", 92, htmlFile, htmlDir);
            histoHTML(h_jet_Eta, "All Jets Eta", "Events", 92, htmlFile, htmlDir);
            histoHTML(h_jet_Phi, "all Jets Phi", "Events", 92, htmlFile, htmlDir);
            htmlFile << "</tr>" << endl;
        }
        if (i == 1) {
            htmlFile << "<tr align=\"left\">" << endl;
            histoHTML(h_leadJet_Pt, "Leading Jet Pt", "Events", 92, htmlFile, htmlDir);
            histoHTML(h_leadJet_Eta, "Leading Jet Eta", "Events", 92, htmlFile, htmlDir);
            histoHTML(h_leadJet_Phi, "Leading Jet Phi", "Events", 92, htmlFile, htmlDir);
            htmlFile << "</tr>" << endl;
        }
        if (i == 2) {
            htmlFile << "<tr align=\"left\">" << endl;
            histoHTML(h_caloMet_Met, "CaloMET", "Events", 92, htmlFile, htmlDir);
            histoHTML(h_caloMet_Phi, "CaloMet Phi", "Events", 92, htmlFile, htmlDir);
            histoHTML(h_caloMet_SumEt, "Scalar Sum ET", "Events", 92, htmlFile, htmlDir);
	    histoHTML(h_MHT, "MHT", "Events", 92, htmlFile, htmlDir);
	    histoHTML(h_HT, "HT", "Events", 92, htmlFile, htmlDir);
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


void HcalProm::RecHitHTMLOutput(string startTime, string htmlDir, string htmlName) {

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
    htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << runBegin << "</span>" << endl;

    htmlFile << "&nbsp;&nbsp;LS:&nbsp;" << endl; 
    htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << lumibegin << "</span>" << endl;

    htmlFile << "-" << endl; 
    htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << lumiend << "</span>" << endl;

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
	    histoHTML(h_hbhe_rechit_energy, "HBHE RecHit Energy", "Events", 92, htmlFile, htmlDir);
	    histoHTML2(h_hbhe_eta_phi, "iEta", "iPhi", 100, htmlFile, htmlDir);
	    htmlFile << "</tr>" << endl;
	}
        if (i == 1){
	    htmlFile << "<tr align=\"left\">" << endl;
	    histoHTML(h_hf_rechit_energy, "HF RecHit Energy", "Events", 92, htmlFile, htmlDir);
	    histoHTML2(h_hf_eta_phi, "iEta", "iPhi", 100, htmlFile, htmlDir);
	    htmlFile << "</tr>" << endl;
	}
        if (i == 2){
	    htmlFile << "<tr align=\"left\">" << endl;
	    histoHTML(h_ho_rechit_energy, "HO RecHit Energy", "Events", 92, htmlFile, htmlDir);
	    histoHTML2(h_hf_eta_phi, "iEta", "iPhi", 100, htmlFile, htmlDir);
	    htmlFile << "</tr>" << endl;
	}
        if (i == 3){
	    htmlFile << "<tr align=\"left\">" << endl;
	    histoHTML(h_eb_rechit_energy, "EB RecHit Energy", "Events", 92, htmlFile, htmlDir);
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

void HcalProm::DigiHTMLOutput(string startTime, string htmlDir, string htmlName) {

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
    htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << runBegin << "</span>" << endl;

    htmlFile << "&nbsp;&nbsp;LS:&nbsp;" << endl; 
    htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << lumibegin << "</span>" << endl;

    htmlFile << "-" << endl; 
    htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << lumiend << "</span>" << endl;

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

void HcalProm::HPDNoiseHTMLOutput(string startTime, string htmlDir, string htmlName) {

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
    htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << runBegin << "</span>" << endl;

    htmlFile << "&nbsp;&nbsp;LS:&nbsp;" << endl; 
    htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << lumibegin << "</span>" << endl;

    htmlFile << "-" << endl; 
    htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << lumiend << "</span>" << endl;

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
            histoHTML(h_Qiesum, "Qiesum (fC)", "Channels", 92, htmlFile, htmlDir);
            htmlFile << "</tr>" << endl;
        }
        if (i == 1) {
            // string Noise[9] = {"1", "5", "20", "80", "150", "300", "600", "1000", "1500"};
            htmlFile << "<tr align=\"left\">" << endl;
            for (int ni = 0; ni != 9; ++ni) {
                histoHTML2(h_NoiseChan[ni], "i#eta", "i#phi", 92, htmlFile, htmlDir);
            }
            htmlFile << "</tr>" << endl;
        }
        if (i == 2) {
            htmlFile << "<tr align=\"left\">" << endl;
            for (int ni = 0; ni != 9; ++ni) {
                histoHTML(h_NoisePhi[ni], "i#phi", "# Noisy Pixels in Range", 92, htmlFile, htmlDir);
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

void HcalProm::MuonHTMLOutput(string startTime, string htmlDir, string htmlName) {

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
    htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << runBegin << "</span>" << endl;

    htmlFile << "&nbsp;&nbsp;LS:&nbsp;" << endl; 
    htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << lumibegin << "</span>" << endl;

    htmlFile << "-" << endl; 
    htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << lumiend << "</span>" << endl;

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
    //    histoHTML(h_maxhbherec, "HBHE Muon (GeV)", "Events", 92, htmlFile, htmlDir);
    //    histoHTML(h_maxhorec, "HO Muon (GeV)", "Events", 92, htmlFile, htmlDir);
    //    histoHTML(h_maxhbMinusrec, "HB- Muon (GeV)", "Events", 92, htmlFile, htmlDir);
    //    histoHTML(h_maxhbPlusrec, "HB+ Muon (GeV)", "Events", 92, htmlFile, htmlDir);
    histoHTML(h_maxebeerec, "EBEE Muon (GeV)", "Events", 92, htmlFile, htmlDir);
    //    histoHTML(h_maxebee_plus_maxhbhe, "EBEE+HBHE Muon (GeV)", "Events", 92, htmlFile, htmlDir);
    //    histoHTML(h_muon_vertex_x, "Muon Vertex X", "Events", 92, htmlFile, htmlDir);
    //    histoHTML(h_muon_px, "Muon Px", "Events", 92, htmlFile, htmlDir);
    //    histoHTML(h_muon_p, "Muon P", "Events", 92, htmlFile, htmlDir);
    //    histoHTML2(h_ecalx_vs_muonx, "ECAL BasicClu x", "Muon x", 100, htmlFile, htmlDir);
    //    histoHTML2(h_ecaly_vs_muony, "ECAL BasicClu y", "Muon y", 100, htmlFile, htmlDir);
    //    histoHTML2(h_jetphi_vs_muonphi, "Jet Phi", "Muon Phi", 100, htmlFile, htmlDir);
    histoHTML(h_impact_diff, "ECAL BasicClu X - Muon X", "Events", 92, htmlFile, htmlDir);
    histoHTML2(DT_HCAL_eta_correlation, "DT eta", "Calo eta", 100, htmlFile, htmlDir);
    histoHTML2(DT_HCAL_eta_correlation_all, "DT eta all", "Calo eta all", 100, htmlFile, htmlDir);
    histoHTML2(DT_HCAL_phi_correlation, "DT phi", "Calo phi", 100, htmlFile, htmlDir);
    histoHTML2(DT_HCAL_phi_correlation_all, "DT phi all", "Calo phi all", 100, htmlFile, htmlDir);
    histoHTML(HCAL_energy_correlation, "Calo had energy", "Events", 92, htmlFile, htmlDir);
    histoHTML(HCAL_energy_correlation_all, "Calo had energy all", "Events", 92, htmlFile, htmlDir);

    histoHTML(h_AllTracks, "Nb. of Reconstructed Cosmic Muon Tracks", "Events", 92, htmlFile, htmlDir);
    histoHTML(h_d0, "Transverse Impact Parameter", "Events", 92, htmlFile, htmlDir);
    histoHTML2(h_EtaPhi, "Propagated tower i#eta", "Propagated tower i#phi", 100, htmlFile, htmlDir);
    histoHTML(h_HB_maxE, "Maximum HB Energy (>0.6GeV)", "Events", 92, htmlFile, htmlDir);
    histoHTML(h_HB_maxE2, "Maximum HB + next neighbor Energy", "Events", 92, htmlFile, htmlDir);
    histoHTML2(h_HB_maxIEtaIPhi, "Maximum HB ieta", "Maximum HB iphi", 100, htmlFile, htmlDir);
    histoHTML(h_HBTopMin_e1x3cr, "HB- Top Sector 1x3 cluster energy corr", "Events", 92, htmlFile, htmlDir);
    histoHTML(h_HBTopPlu_e1x3cr, "HB+ Top Sector 1x3 cluster energy corr", "Events", 92, htmlFile, htmlDir);
    histoHTML(h_HBBottomMin_e1x3cr, "HB- Bottom Sector 1x3 cluster energy corr", "Events", 92, htmlFile, htmlDir);
    histoHTML(h_HBBottomPlu_e1x3cr, "HB+ Bottom Sector 1x3 cluster energy corr", "Events", 92, htmlFile, htmlDir);
    histoHTML2(h_HBCorrEta, "Track Eta, X:max, Y:propagated", "HB Eta, X:max, Y:propagated", 100, htmlFile, htmlDir);
    histoHTML2(h_HBCorrPhi, "Track Phi, X:max, Y:propagated", "HB Phi, X:max, Y:propagated",100, htmlFile, htmlDir);




    htmlFile << "</tr>" << endl;
    htmlFile << "</table>" << endl;
    // end table
    htmlFile << "<br>" << endl;

    // html page footer
    htmlFile << "</body> " << endl;
    htmlFile << "</html> " << endl;
    htmlFile.close();

}

void HcalProm::CaloTowerHTMLOutput(string startTime, string htmlDir, string htmlName) {

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
    htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << runBegin << "</span>" << endl;

    htmlFile << "&nbsp;&nbsp;LS:&nbsp;" << endl; 
    htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << lumibegin << "</span>" << endl;

    htmlFile << "-" << endl; 
    htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << lumiend << "</span>" << endl;

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
    histoHTML(h_calo_tower_energy, "CaloTower energy", "Events", 92, htmlFile, htmlDir);
    htmlFile << "</tr>" << endl;
    htmlFile << "</table>" << endl;
    // end table
    htmlFile << "<br>" << endl;

    // html page footer
    htmlFile << "</body> " << endl;
    htmlFile << "</html> " << endl;
    htmlFile.close();

}

void HcalProm::histoHTML(TH1F * hist, const char *xlab, const char *ylab, int width, ofstream & htmlFile,
  string htmlDir) {

    if (hist != NULL) {
        string imgNameTMB = "";

        imgNameTMB = getIMG(hist, 1, htmlDir, xlab, ylab);
        string imgName = "";

        imgName = getIMG(hist, 2, htmlDir, xlab, ylab);

        if (imgName.size() != 0)
            htmlFile << "<td><a href=\"" << imgName << "\"><img src=\"" << imgNameTMB << "\"></a></td>" << endl;
        else
            htmlFile << "<td><img src=\"" << " " << "\"></td>" << endl;
    } else
        htmlFile << "<td><img src=\"" << " " << "\"></td>" << endl;
    return;
}





void HcalProm::histoHTML2(TH2F * hist, const char *xlab, const char *ylab, int width,
  ofstream & htmlFile, string htmlDir, bool color) {
    if (hist != NULL) {
        string imgNameTMB = "";

        imgNameTMB = getIMG2(hist, 1, htmlDir, xlab, ylab, color);
        string imgName = "";

        imgName = getIMG2(hist, 2, htmlDir, xlab, ylab, color);
        if (imgName.size() != 0)
            htmlFile << "<td><a href=\"" << imgName << "\"><img src=\"" << imgNameTMB << "\"></a></td>" << endl;
        else
            htmlFile << "<td><img src=\"" << " " << "\"></td>" << endl;
    } else
        htmlFile << "<td><img src=\"" << " " << "\"></td>" << endl;
    return;
}



string HcalProm::getIMG(TH1F * hist, int size, string htmlDir, const char *xlab, const char *ylab) {

    if (hist == NULL) {
        printf("getIMG:  This histo is NULL, %s, %s\n", xlab, ylab);
        return "";
    }

    string name = hist->GetName();

    cleanString(name);
    char dest[512];

    if (runBegin > -1)
      sprintf(dest, "%s - Run %ld LS %ld-%ld", name.c_str(), runBegin, lumibegin, lumiend);
    else
        sprintf(dest, "%s", name.c_str());
    // set run generically hist->SetTitle(dest); 
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

string HcalProm::getIMG2(TH2F * hist, int size, string htmlDir, const char *xlab, const char *ylab,
  bool color) {

    if (hist == NULL) {
        printf("getIMG2:  This histo is NULL, %s, %s\n", xlab, ylab);
        return "";
    }

    string name = hist->GetName();

    cleanString(name);
    char dest[512];

    if (runBegin > -1)
      sprintf(dest, "%s - Run %ld LS %ld-%ld", name.c_str(), runBegin, lumibegin, lumiend);
    else
        sprintf(dest, "%s", name.c_str());
    // set run generically    hist->SetTitle(dest);
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

/*
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
*/

float HcalProm::deltaR(float eta0, float phi0, float eta, float phi){
    float dphi=phi-phi0;
    if(dphi>M_PI) dphi-=2*M_PI;
    else if(dphi<=-M_PI) dphi+=2*M_PI;
    return sqrt(dphi*dphi+(eta-eta0)*(eta-eta0));
}

HcalDetId HcalProm::getClosestCell(float dR,float eta, float phi, const HBHERecHitCollection*  HBHERecHits, const CaloGeometry* geo, bool &found)
{
     
    HcalDetId hcalDetId;
    //loop over HB rechits:
    float dRMin = dR;//starting point
    HBHERecHitCollection::const_iterator it_hc;
    for (it_hc=HBHERecHits->begin(); it_hc!=HBHERecHits->end(); it_hc++){
	if(it_hc->id().subdet()==HcalBarrel){
		GlobalPoint pos = geo->getPosition(it_hc->detid());
		//float dR_ = deltaR(eta,phi,pos.eta(),pos.phi());
		float dR_ = deltaR(eta,Geom::Phi<float>(phi),pos.eta(),Geom::Phi<float>(pos.phi()));
		if(dR_ > dR)continue;
		if(dR_<dRMin){
		    dRMin=dR_;
		    hcalDetId = it_hc->id();
		}    
	}
    }
    if(hcalDetId.ieta()==0 || hcalDetId.iphi()==0){
        cout << "Couldn't find a close HB cell" << endl;
        found = false;
    }else{
        found = true;
    }
    return hcalDetId;
}
float HcalProm::getNxNClusterEnergy(int gridSize,int ieta, int iphi, const HBHERecHitCollection*  HBHERecHits)
{
    float energy = 0.;
    HBHERecHitCollection::const_iterator it_hcal;
    if(gridSize==0){
	for (it_hcal=HBHERecHits->begin(); it_hcal!=HBHERecHits->end(); it_hcal++){
	    if(it_hcal->id().subdet()==HcalBarrel){
		if( it_hcal->id().ieta() == ieta && it_hcal->id().iphi() == iphi){
		    energy = it_hcal->energy();
		}
	    }
	}
    }else{
	for (it_hcal=HBHERecHits->begin(); it_hcal!=HBHERecHits->end(); it_hcal++){
	    if(it_hcal->id().subdet()==HcalBarrel){
		int jeta = it_hcal->id().ieta();
		int jphi = it_hcal->id().iphi();
		int dEta = abs((ieta<0?ieta+1:ieta) -(jeta<0?jeta+1:jeta));
		int dPhi = abs(iphi-jphi);
		if ( abs(72-dPhi) < dPhi ) dPhi = 72-dPhi;
		if(  dEta <= gridSize && dPhi <= gridSize ) {
		    energy += it_hcal->energy();
		}
	    }
	}
    }
    return energy; 
}
float HcalProm::get1xNClusterEnergy(int gridSize,int ieta, int iphi, const HBHERecHitCollection*  HBHERecHits)
{
    float energy = 0.;
    HBHERecHitCollection::const_iterator it_hcal;
    if(gridSize==0){
	for (it_hcal=HBHERecHits->begin(); it_hcal!=HBHERecHits->end(); it_hcal++){
	    if(it_hcal->id().subdet()==HcalBarrel){
		if( it_hcal->id().ieta() == ieta && it_hcal->id().iphi() == iphi){
		    energy = it_hcal->energy();
		}
	    }
	}
    }else{
	for (it_hcal=HBHERecHits->begin(); it_hcal!=HBHERecHits->end(); it_hcal++){
	    if(it_hcal->id().subdet()==HcalBarrel){
		int jeta = it_hcal->id().ieta();
		int jphi = it_hcal->id().iphi();
		int dEta = abs((ieta<0?ieta+1:ieta) -(jeta<0?jeta+1:jeta));
		int dPhi = abs(iphi-jphi);
		if ( abs(72-dPhi) < dPhi ) dPhi = 72-dPhi;
		if(  dEta <= gridSize && it_hcal->id().iphi() == iphi ) {
		    energy += it_hcal->energy();
		}
	    }
	}
    }
    return energy; 
}
void HcalProm::getHBmax(
// find HB Emax tower: ieta,iphi,Emax , max and next to max sum E2max
				   // inputs
				   int tphi, const HBHERecHitCollection*  HBHERecHits,
				   // outputs
				   int *ietaMax, int *iphiMax, double *Emax, double *E2max
				   ){
    int ietaM = -99999;
    int iphiM = -99999;
    float tEmax = 0.;
    float tEnmax = 0.;
    int dlimit_phi = 1;
    int ulimit_phi = 36;
    if(tphi>36) 
    {
	dlimit_phi=37;
	ulimit_phi=72;
    }
    HBHERecHitCollection::const_iterator it_hcal;
	for (it_hcal=HBHERecHits->begin(); it_hcal!=HBHERecHits->end(); it_hcal++){
	    if(it_hcal->id().subdet()==HcalBarrel){
//		if(it_hcal->energy()>tEmax)
		if(it_hcal->energy()>tEmax&&it_hcal->id().iphi()>=dlimit_phi&&it_hcal->id().iphi()<=ulimit_phi)
		{
		ietaM = it_hcal->id().ieta();
		iphiM = it_hcal->id().iphi();
		tEmax= it_hcal->energy();
		}
	    }
	}
	for (it_hcal=HBHERecHits->begin(); it_hcal!=HBHERecHits->end(); it_hcal++){
	    if(it_hcal->id().subdet()==HcalBarrel){
		if(it_hcal->energy()>tEnmax&&it_hcal->energy()<tEmax&&
		  abs(it_hcal->id().ieta()-ietaM)<1&&abs(it_hcal->id().iphi()-iphiM)<=1)
		{
		  tEnmax=it_hcal->energy();
		}
	    }
	}
	*Emax     = tEmax;    
	*E2max     = tEmax+tEnmax;
	*ietaMax=ietaM;
	*iphiMax=iphiM;    

} 

bool HcalProm::Extrapolate(
				   // inputs
				   double ox, double oy, double oz, double px, double py, double pz, 
				   double ra, double zwidth, double ImpPar ,
				   // outputs
				   double *xp_out, double *yp_out, double *zp_out,
				   double *xm_out, double *ym_out, double *zm_out,
				   double *thetap_out, double *phip_out,
				   double *thetam_out, double *phim_out
				   ) {

  bool isCloseToIP = false;

  if(px == 0.) {
    std::cout<<"px is exactly 0 - the extrapolation can not handle this case currently, sorry. Not extrapolating."<<std::endl;
    return isCloseToIP;
  }


  double xp = -99999.;
  double yp = -99999.;
  double zp = -99999.;

  double xm = -99999.;
  double ym = -99999.;
  double zm = -99999.;

  double a = py/px;
  double b = oy-a*ox;

  double intX= - a*b/(a*a+1);
  double intY=a*intX+b;

  double notnegative = (ra*ra-b*b)/(1+a*a) + TMath::Power(a*b/(1+a*a),2.);

  if( notnegative > 0.) {
    xp = TMath::Sqrt( notnegative ) - a*b/(1+a*a);
    yp = (py/px)*(xp-ox)+oy;
    zp = (pz*(xp-ox)/px)+oz;
    double phip = TMath::ATan2(yp,xp);
    if (phip < 0.) phip = phip + (2. * TMath::Pi());
    double thetap = TMath::Pi()/2. - TMath::ATan2(zp,TMath::Sqrt(xp*xp+yp*yp));
    xm = -TMath::Sqrt( notnegative ) - a*b/(1+a*a);
    ym = py/px*(xm-ox)+oy;
    zm = pz*(xm-ox)/px+oz;
    
    double phim = TMath::ATan2(ym,xm);
    if (phim < 0.) phim = phim + (2. * TMath::Pi());
    double thetam = TMath::Pi()/2. - TMath::ATan2(zm,TMath::Sqrt(xm*xm+ym*ym));
    
    // set the output values if we are in a reasonable z range
    //    if(TMath::Abs(zp) < 433. && TMath::Abs(zm) < 433.) {
    // restrict to +-20 cms around z=0
     if(yp>0)
     {
      *thetap_out   = thetap;
      *phip_out     = phip;
      *thetam_out   = thetam;
      *phim_out     = phim;
      *xp_out = xp;
      *yp_out = yp;
      *zp_out = zp;
      *xm_out = xm;
      *ym_out = ym;
      *zm_out = zm;
     }else{
      *thetap_out   = thetam;
      *phip_out     = phim;
      *thetam_out   = thetap;
      *phim_out     = phip;
      *xp_out = xm;
      *yp_out = ym;
      *zp_out = zm;
      *xm_out = xp;
      *ym_out = yp;
      *zm_out = zp;
     }
    if(oy>0)
    {
     *thetam_out=*thetap_out;
     *phim_out=*phip_out;
     *xm_out=*xp_out;
     *ym_out=*yp_out;
     *zm_out=*zp_out;
    }else{
     *thetap_out=*thetam_out;
     *phip_out=*phim_out;
     *xp_out=*xm_out;
     *yp_out=*ym_out;
     *zp_out=*zm_out;
    }
    h_d0->Fill(TMath::Sqrt(intX*intX+intY*intY));
    if(TMath::Abs(*zp_out) < zwidth ) {
    if(TMath::Sqrt(intX*intX+intY*intY)<ImpPar) isCloseToIP = true;
    }

    std::cout<<"We have a cosmic muon close to the IP "<< zp << " , " << zm << std::endl;
    
  }
  else {
    //std::cout<<"No intersection of StandAlone Muon with selected cylinder."<<std::endl;
    isCloseToIP = false;
  }

  return isCloseToIP;

}

reco::Track HcalProm::bestTrack(const reco::TrackCollection& muons) const {
 
    reco::Track bestOne = muons.front();
 
    for(reco::TrackCollection::const_iterator muon = muons.begin()+1; muon != muons.end(); ++ muon ) {
 
     if (( (*muon).found() > bestOne.found() ) ||
        (((*muon).found() == bestOne.found()) && ((*muon).chi2() < bestOne.chi2())) )
        bestOne = (*muon);
    }
    return bestOne;
 
}
float HcalProm::getEtaCluster(int gridSize,int ieta, int iphi, const HBHERecHitCollection*  HBHERecHits)
{
    float energy = 0.;
    float Eta = 0.;
    HBHERecHitCollection::const_iterator it_hcal;
	for (it_hcal=HBHERecHits->begin(); it_hcal!=HBHERecHits->end(); it_hcal++){
	    if(it_hcal->id().subdet()==HcalBarrel){
		int jeta = it_hcal->id().ieta();
		int jphi = it_hcal->id().iphi();
		int dEta = abs((ieta<0?ieta+1:ieta) -(jeta<0?jeta+1:jeta));
		int dPhi = abs(iphi-jphi);
		if ( abs(72-dPhi) < dPhi ) dPhi = 72-dPhi;
		if(  dEta <= gridSize && dPhi <= gridSize&&it_hcal->energy()>0 ) {
		    energy += it_hcal->energy();
		    Eta += it_hcal->energy()*float(it_hcal->id().ieta());
		}
	    }
	}
    Eta/=energy;
    return Eta; 
}
float HcalProm::getPhiCluster(int gridSize,int ieta, int iphi, const HBHERecHitCollection*  HBHERecHits)
{
    float energy = 0.;
    float Phi = 0.;
    HBHERecHitCollection::const_iterator it_hcal;
	for (it_hcal=HBHERecHits->begin(); it_hcal!=HBHERecHits->end(); it_hcal++){
	    if(it_hcal->id().subdet()==HcalBarrel){
		int jeta = it_hcal->id().ieta();
		int jphi = it_hcal->id().iphi();
		int dEta = abs((ieta<0?ieta+1:ieta) -(jeta<0?jeta+1:jeta));
		int dPhi = abs(iphi-jphi);
		if ( abs(72-dPhi) < dPhi ) dPhi = 72-dPhi;
		if(  dEta <= gridSize && dPhi <= gridSize&&it_hcal->energy()>0 ) {
		    energy += it_hcal->energy();
		    Phi += it_hcal->energy()*float(it_hcal->id().iphi());
		}
	    }
	}
    Phi/=energy;
    return Phi; 
}
