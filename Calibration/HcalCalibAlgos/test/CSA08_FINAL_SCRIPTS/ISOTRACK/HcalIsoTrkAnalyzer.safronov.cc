// -*- C++ -*-
//
// Package:  HcalCalibAlgos  
// Class:    HcalIsoTrkAnalyzer 

// 
/**\class HcalIsoTrkAnalyzer HcalIsoTrkAnalyzer.cc Calibration/HcalCalibAlgos/src/HcalIsoTrkAnalyzer.cc

   Description: <one line class summary>

   Implementation:
   <Notes on implementation>
*/
//
// Original Author:  Andrey Pozdnyakov
//                   ... and Sergey Petrushanko (all lines between M+ and M-)
//         Created:  Thu Jul 12 18:12:19 CEST 2007
// $Id: HcalIsoTrkAnalyzer.cc,v 1.7 2008/05/01 15:45:59 kodolova Exp $
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
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/DetId/interface/DetId.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "TrackingTools/TrackAssociator/interface/TrackDetMatchInfo.h"
#include "TrackingTools/TrackAssociator/interface/TrackAssociatorParameters.h"
#include "TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h"

#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h" 

#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h" 
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"

#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "TrackPropagation/SteppingHelixPropagator/interface/SteppingHelixPropagator.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"

// M+
#include "Calibration/Tools/interface/MinL3AlgoUniv.h"
// M-

#include "TROOT.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TTree.h"
#include <iostream>
#include <fstream>

#include "CondFormats/HcalObjects/interface/HcalRespCorrs.h"
#include "CondFormats/DataRecord/interface/HcalRespCorrsRcd.h"


using namespace edm;
using namespace std;
using namespace reco;

//
// class decleration
//

class HcalIsoTrkAnalyzer : public edm::EDAnalyzer {
public:
  explicit HcalIsoTrkAnalyzer(const edm::ParameterSet&);
  ~HcalIsoTrkAnalyzer();
 
 
private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  // ----------member data ---------------------------
  
 
  TrackDetectorAssociator trackAssociator_;
  TrackAssociatorParameters parameters_;

  const CaloGeometry* geo;
  InputTag hbheLabel_;
  InputTag hoLabel_;
  InputTag eLabel_;
  InputTag trackLabel_;

  std::string m_inputTrackLabel;
  std::string m_ecalLabel;
  std::string m_ebInstance;
  std::string m_eeInstance;
  std::string m_hcalLabel;
  int m_histoFlag;

  double associationConeSize_;
  string AxB_;
  bool allowMissingInputs_;
  string outputFileName_;

  int NisoTrk;
  double trackPt, trackEta, trackPhi; 

  double ptrack, rvert;
  double eecal, ehcal;
  
  int nHCRecHits,  nECRecHits, nHORecHits;
  double ecRHen[1500], ecRHeta[1500], ecRHphi[1500];

  double hcRHen[1500], hcRHeta[1500], hcRHphi[1500];
  int hcRHieta[1500], hcRHiphi[1500], hcRHdepth[1500];
  double hoRHen[1500], hoRHeta[1500], hoRHphi[1500];
  int hoRHieta[1500], hoRHiphi[1500], hoRHdepth[1500];

  double HcalAxBxDepthEnergy, MaxHhitEnergy;
  double HCAL3x3[9], HCAL5x5[25];

  int numbers[60][72];
  int numbers2[60][72];

  // M+
  vector<float> EnergyVector;
  vector<vector<float> > EventMatrix;
  vector<vector<HcalDetId> > EventIds;
  //  MinL3AlgoUniv<HcalDetId>* MyL3Algo;
  map<HcalDetId,float> solution;
  int nIterations;
  float eventWeight;
  double energyMinIso, energyMaxIso;
  // M-  


  char dirname[50];
  char hname[20];
  char htitle[80];


  ofstream input_to_L3;

  int NisoTrk_gt_15;

  TFile* tf2;

  TH1F* h_mip_resp_barrel_20;
  TH1F* h_mip_resp_barrel_35;
  TH1F* h_mip_resp_barrel_50;
  TH1F* h_mip_resp_eta_lt_1_20;
  TH1F* h_mip_resp_eta_lt_1_35;
  TH1F* h_mip_resp_eta_lt_1_50;
  TH1F* h_mip_resp_barrel_eta_gt_1_20;
  TH1F* h_mip_resp_barrel_eta_gt_1_35;
  TH1F* h_mip_resp_barrel_eta_gt_1_50;
  TH1F* h_ecal_cluster_20;
  TH1F* h_ecal_cluster_35;
  TH1F* h_ecal_cluster_50;
  TH2F* h_banana_20;
  TH2F* h_banana_35;
  TH2F* h_banana_50;

  TH1F* h_number_of_tracks_15;
  TH1F* h_p_trk_barrel;
  TH1F* h_p_trk_endcap;
  TH2F* h_trk_eta_p;
  TH2F* h_trk_eta_p_wo_mip_cut;
  TH2F* h_trk_eta_pt;
  TH2F* h_trk_eta_pt_wo_mip_cut;  

  TH1F* h_50_tail_resp_TRK_P;
  TH2F* h_50_tail_resp_TRK_ETA_PHI;
  TH1F* h_50_low_tail_resp_TRK_P;
  TH2F* h_50_low_tail_resp_TRK_ETA_PHI;


  //HE
  TH1F* h_HE_mipresp_25_40;
  TH1F* h_HE_mipresp_40_50;
  TH1F* h_HE_mipresp_50_60;
  TH1F* h_HE_mipresp_60_80;
  TH1F* h_HE_mipresp_80_110;
  TH1F* h_HE_mipresp_290_320;

};



HcalIsoTrkAnalyzer::HcalIsoTrkAnalyzer(const edm::ParameterSet& iConfig)
{
  //now do what ever initialization is needed

  m_ecalLabel = iConfig.getUntrackedParameter<std::string> ("ecalRecHitsLabel","ecalRecHit");
  m_ebInstance = iConfig.getUntrackedParameter<std::string> ("ebRecHitsInstance","EcalRecHitsEB");
  m_eeInstance = iConfig.getUntrackedParameter<std::string> ("eeRecHitsInstance","EcalRecHitsEE");
  m_hcalLabel = iConfig.getUntrackedParameter<std::string> ("hcalRecHitsLabel","hbhereco");
  m_histoFlag = iConfig.getUntrackedParameter<int>("histoFlag",0);
 
  hbheLabel_= iConfig.getParameter<edm::InputTag>("hbheInput");
  hoLabel_=iConfig.getParameter<edm::InputTag>("hoInput");
  eLabel_=iConfig.getParameter<edm::InputTag>("eInput");
  trackLabel_ = iConfig.getParameter<edm::InputTag>("trackInput");
  associationConeSize_=iConfig.getParameter<double>("associationConeSize");
  allowMissingInputs_=iConfig.getParameter<bool>("allowMissingInputs");
  outputFileName_=iConfig.getParameter<std::string>("outputFileName");

  AxB_=iConfig.getParameter<std::string>("AxB");

  // M+
  nIterations = iConfig.getUntrackedParameter<int>("noOfIterations",10);
  eventWeight = iConfig.getUntrackedParameter<double>("eventWeight",0.);
  energyMinIso = iConfig.getUntrackedParameter<double>("energyMinIso",2.);
  energyMaxIso = iConfig.getUntrackedParameter<double>("energyMaxIso",1000.);
  // M-

  edm::ParameterSet parameters = iConfig.getParameter<edm::ParameterSet>("TrackAssociatorParameters");
  parameters_.loadParameters( parameters );
  trackAssociator_.useDefaultPropagator();

}

HcalIsoTrkAnalyzer::~HcalIsoTrkAnalyzer()
{
  tf2->Close();
}


// ------------ method called to for each event  ------------
void
HcalIsoTrkAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace std;

  // M+
  vector<float> rawEnergyVec;
  vector<HcalDetId> detidvec;
  float calEnergy;
  // M-

  
  edm::Handle<reco::TrackCollection> trackCollection;
  iEvent.getByLabel(trackLabel_,trackCollection);
  const reco::TrackCollection isoTrack = *(trackCollection.product());
  //  LogInfo("IsoTracks: ")<<" IsoTracks size "<<(isoTrack).size();
  //    cout << " IsoTracks size "<<(isoTrack).size() << endl;
    
  edm::Handle<EcalRecHitCollection> ecal;
  iEvent.getByLabel(eLabel_,ecal);
  const EcalRecHitCollection Hitecal = *(ecal.product());
  //  LogInfo("ECAL: ")<<" Size of ECAl "<<(Hitecal).size();
  //    cout << " Size of ECAl "<<(Hitecal).size() << endl;
    
  edm::Handle<HBHERecHitCollection> hbhe;
  iEvent.getByLabel(hbheLabel_,hbhe);
  const HBHERecHitCollection Hithbhe = *(hbhe.product());
  //  LogInfo("HBHE: ")<<" Size of HBHE "<<(Hithbhe).size();

  ESHandle<CaloGeometry> pG;
  iSetup.get<IdealGeometryRecord>().get(pG);
  geo = pG.product();
  

// rof 16.05.2008 start: include the possibility for recalibration (use "recalibrate" label for safety)
/*
  edm::ESHandle <HcalRespCorrs> recalibCorrs;
  iSetup.get<HcalRespCorrsRcd>().get("recalibrate",recalibCorrs);
  const HcalRespCorrs* myRecalib = recalibCorrs.product();
*/
// rof end



  parameters_.useEcal = true;
  parameters_.useHcal = true;
  parameters_.useCalo = false;
  parameters_.dREcal = 0.5;
  parameters_.dRHcal = 0.6;  

  //	    cout<<"  BEGINNING "<< calEnergy << "  " << rawEnergyVec.size() <<  "  " << detidvec.size()<< endl; 

  NisoTrk_gt_15 = 0;

  for (reco::TrackCollection::const_iterator it = isoTrack.begin(); it!=isoTrack.end(); it++)
    { 
      /////////// Safronov
      //throw away "fake" tracks in barrel
      if (it->hitPattern().numberOfValidHits()<13) continue;
      
      //throw away "fake" tracks in endcap
      if (fabs(it->eta())>1.47&&it->hitPattern().numberOfValidStripTECHits()<11) continue;
      
      /////////// Safronov
      //container for used recHits
      std::vector<int> usedHits; 
      //////////
      
      //	    cout<<"  BEGINNING 1 "<< calEnergy << "  " << rawEnergyVec.size() <<  "  " << detidvec.size()<< endl; 

      NisoTrk++;
     
      ptrack = sqrt(it->px()*it->px()+it->py()*it->py()+it->pz()*it->pz());

      // M+      
      calEnergy = sqrt(it->px()*it->px()+it->py()*it->py()+it->pz()*it->pz()+0.14*0.14);
      // M-

      if (calEnergy > 15) ++NisoTrk_gt_15; 
      //if (ptrack < 20) continue;
      
      trackPt  = it->pt();
      trackEta = it->eta();
      trackPhi = it->phi();
      
      double corrHCAL = 1.;
      /*      
      if (fabs(trackEta)<1.47) {

	if (calEnergy < 5.) corrHCAL = 1.55;
	if (calEnergy > 5. && calEnergy < 9.) corrHCAL = 1.55 - 0.18*(calEnergy-5.)/4.;
	if (calEnergy > 9. && calEnergy < 20.) corrHCAL = 1.37 - 0.18*(calEnergy-9.)/11.;
	if (calEnergy > 20. && calEnergy < 30.) corrHCAL = 1.19 - 0.06*(calEnergy-20.)/10.;
	if (calEnergy > 30. && calEnergy < 50.) corrHCAL = 1.13 - 0.02*(calEnergy-30.)/20.;
	if (calEnergy > 50. && calEnergy < 100.) corrHCAL = 1.11 - 0.02*(calEnergy-50.)/50.;
	if (calEnergy > 100. && calEnergy < 1000.) corrHCAL = 1.09 - 0.09*(calEnergy-100.)/900.;
      
      }
      
      if (fabs(trackEta)>1.47) {

	if (calEnergy < 5.) corrHCAL = 1.49;
	if (calEnergy > 5. && calEnergy < 9.) corrHCAL = 1.49 - 0.08*(calEnergy-5.)/4.;
	if (calEnergy > 9. && calEnergy < 20.) corrHCAL = 1.41- 0.15*(calEnergy-9.)/11.;
	if (calEnergy > 20. && calEnergy < 30.) corrHCAL = 1.26 - 0.07*(calEnergy-20.)/10.;
	if (calEnergy > 30. && calEnergy < 50.) corrHCAL = 1.19 - 0.04*(calEnergy-30.)/20.;
	if (calEnergy > 50. && calEnergy < 100.) corrHCAL = 1.15 - 0.03*(calEnergy-50.)/50.;
	if (calEnergy > 100. && calEnergy < 1000.) corrHCAL = 1.12 - 0.12*(calEnergy-100.)/900.;
      
      }      
      */

      //      cout << endl << " ISO TRACK E = "<< calEnergy << " ETA = " << trackEta<< " PHI = " << trackPhi <<  " Correction " <<  corrHCAL<< endl;

      rvert = sqrt(it->vx()*it->vx()+it->vy()*it->vy()+it->vz()*it->vz());      
                 
      //Associate track with a calorimeter
      TrackDetMatchInfo info = trackAssociator_.associate(iEvent, iSetup, trackAssociator_.getFreeTrajectoryState(iSetup, *it), parameters_);
      
      double etaecal=info.trkGlobPosAtEcal.eta();
      double phiecal=info.trkGlobPosAtEcal.phi();

      eecal=info.coneEnergy(parameters_.dREcal, TrackDetMatchInfo::EcalRecHits);
      ehcal=info.coneEnergy(parameters_.dRHcal, TrackDetMatchInfo::HcalRecHits);

      double rmin = 0.07;
      if( fabs(etaecal) > 1.47 ) rmin = 0.07*(fabs(etaecal)-0.47)*1.2;
      if( fabs(etaecal) > 2.2 ) rmin = 0.07*(fabs(etaecal)-0.47)*1.4;

      struct
      {
	int iphihit;
	int ietahit;
	int depthhit;
	double hitenergy;
      } MaxHit;

      // Find Ecal RecHit with maximum energy and collect other information
      MaxHit.hitenergy=-100;
      nECRecHits=0;
      
      double econus = 0.;
      float ecal_cluster = 0.;      

      ///////Safronov
      //clear usedHits
      usedHits.clear();
      ///////

      for (std::vector<EcalRecHit>::const_iterator ehit=Hitecal.begin(); ehit!=Hitecal.end(); ehit++)	
	{
	  //////////// Safronov
	  //check that this hit was not considered before and push it into usedHits
	  bool hitIsUsed=false;
	  int hitHashedIndex=-10000; 
	  if (ehit->id().subdetId()==EcalBarrel) 
	    {
	      EBDetId did(ehit->id());
	      hitHashedIndex=did.hashedIndex();
	    }
	      
	  if (ehit->id().subdetId()==EcalEndcap) 
	    {
	      EEDetId did(ehit->id());
	      hitHashedIndex=did.hashedIndex();
	    }
	  for (uint32_t i=0; i<usedHits.size(); i++)
	    {
	      if (usedHits[i]==hitHashedIndex) hitIsUsed=true;
	    }
	  if (hitIsUsed) continue;
	  usedHits.push_back(hitHashedIndex);
	  ////////////

	  if((*ehit).energy() > MaxHit.hitenergy) 
	    {
	      MaxHit.hitenergy = (*ehit).energy();
	    }

	  GlobalPoint pos = geo->getPosition((*ehit).detid());
	  double phihit = pos.phi();
	  double etahit = pos.eta();
	 
	  double dphi = fabs(phiecal - phihit); 
	  if(dphi > 4.*atan(1.)) dphi = 8.*atan(1.) - dphi;
	  double deta = fabs(etaecal - etahit); 
	  double dr = sqrt(dphi*dphi + deta*deta);

	  //         cout << " eta "<<  etahit << " phi "<< phihit << " en " << (*ehit).energy() << "  dr "<< dr << endl;

	  if (dr < rmin) {
	    econus = econus + (*ehit).energy();
	  }
          if (dr < 0.13) ecal_cluster += (*ehit).energy();
	 
	  ecRHen [nECRecHits] = (*ehit).energy();
	  ecRHeta[nECRecHits] = etahit;
	  ecRHphi[nECRecHits] = phihit;
	  nECRecHits++;
	}


      MaxHit.hitenergy=-100;
      nHCRecHits=0;
      
      ///////Safronov
      //clear usedHits
      usedHits.clear();
      ///////

      for (HBHERecHitCollection::const_iterator hhit=Hithbhe.begin(); hhit!=Hithbhe.end(); hhit++) 
	{
	  //////////// Safronov
	  //check that this hit was not considered before and push it into usedHits
	  bool hitIsUsed=false;
	  int hitHashedIndex=hhit->id().hashed_index();
	  for (uint32_t i=0; i<usedHits.size(); i++)
	    {
	      if (usedHits[i]==hitHashedIndex) hitIsUsed=true;
	    }
	  if (hitIsUsed) continue;
	  usedHits.push_back(hitHashedIndex);
	  ////////////

	  // LogInfo("ALL HBHERecHit: ")<<" ieta->"<<(hhit->id()).ieta()<<" iphi->"<<(hhit->id()).iphi()<<" depth->"<<(hhit->id()).depth();

	  // rof 16.05.2008 start: include the possibility for recalibration
	  DetId mydetid = hhit->id().rawId();
	  //	  float recal = myRecalib->getValues(mydetid)->getValue();
	  float recal = 1;
	  // rof end

	  GlobalPoint pos = geo->getPosition(hhit->detid());
	  double phihit = pos.phi();
	  double etahit = pos.eta();
	  
	  int iphihit  = (hhit->id()).iphi();
	  int ietahit  = (hhit->id()).ieta();
	  int depthhit = (hhit->id()).depth();
	  
	  
	  double dphi = fabs(info.trkGlobPosAtHcal.phi() - phihit); 
	  if(dphi > 4.*atan(1.)) dphi = 8.*atan(1.) - dphi;
	  double deta = fabs(info.trkGlobPosAtHcal.eta() - etahit); 
	  double dr = sqrt(dphi*dphi + deta*deta);
	  
	  if(dr<associationConeSize_) 
	    {
	      hcRHen[nHCRecHits]    = hhit->energy() * recal;
	      hcRHeta[nHCRecHits]   = etahit;
	      hcRHphi[nHCRecHits]   = phihit;
	      hcRHieta[nHCRecHits]  = ietahit;
	      hcRHiphi[nHCRecHits]  = iphihit;
	      hcRHdepth[nHCRecHits] = depthhit;
	      nHCRecHits++;
	 
	      if(hhit->energy() * recal > MaxHit.hitenergy) 
		{
		  MaxHit.hitenergy =  hhit->energy() * recal;
		  MaxHit.ietahit   = (hhit->id()).ieta();
		  MaxHit.iphihit   = (hhit->id()).iphi();
		  MaxHit.depthhit  = (hhit->id()).depth();
		}
	    }
	}
      MaxHhitEnergy = MaxHit.hitenergy;

      

      if(m_histoFlag==1) 
	{
	  int MinIETA= 999;
	  int MaxIETA= -999;
	  int MinIPHI= 999;   
	  int MaxIPHI= -999;
	  for (int k=0; k<nHCRecHits; k++)
	    {
	      
	      MinIETA = MinIETA > hcRHieta[k] ? hcRHieta[k] : MinIETA;
	      MaxIETA = MaxIETA < hcRHieta[k] ? hcRHieta[k] : MaxIETA;
	      MinIPHI = MinIPHI > hcRHiphi[k] ? hcRHiphi[k] : MinIPHI;
	      MaxIPHI = MaxIPHI < hcRHiphi[k] ? hcRHiphi[k] : MaxIPHI;
	    }

	}

      if(AxB_=="3x3")
	{
	  HcalAxBxDepthEnergy=0.;
	  for(int ih=0; ih<9; ih++){HCAL3x3[ih]=0.;}

	  ///////Safronov
	  //clear usedHits
	  usedHits.clear();
	  ///////

	  for (HBHERecHitCollection::const_iterator hhit=Hithbhe.begin(); hhit!=Hithbhe.end(); hhit++) 
	    {
	      //////////// Safronov
	      //check that this hit was not considered before and push it into usedHits
	      bool hitIsUsed=false;
	      int hitHashedIndex=hhit->id().hashed_index();
	      for (uint32_t i=0; i<usedHits.size(); i++)
		{
		  if (usedHits[i]==hitHashedIndex) hitIsUsed=true;
		}
	      if (hitIsUsed) continue;
	      usedHits.push_back(hitHashedIndex);
	      ////////////

	      int DIETA = 100;
	      if(MaxHit.ietahit*(hhit->id()).ieta()>0)
		{
		  DIETA = MaxHit.ietahit - (hhit->id()).ieta();
		}
	      if(MaxHit.ietahit*(hhit->id()).ieta()<0)
		{
		  DIETA = MaxHit.ietahit - (hhit->id()).ieta();
		  DIETA = DIETA>0 ? DIETA-1 : DIETA+1; 
		}
	      
	      int DIPHI = MaxHit.iphihit - (hhit->id()).iphi();
	      DIPHI = DIPHI>36 ? 72-DIPHI : DIPHI;

	      if( abs(DIETA)<=1 && abs(DIPHI)<=1) {
		
		// rof 16.05.2008 start: include the possibility for recalibration
		DetId mydetid = hhit->id().rawId();
//		float recal = myRecalib->getValues(mydetid)->getValue();
		float recal = 1;
		
	        if ((hhit->id()).ieta() > 15){
		  if (hhit->energy()*recal < 0.31){
		    HcalAxBxDepthEnergy += 0;
		  }
		  if (hhit->energy()*recal > 0.31){
		    HcalAxBxDepthEnergy += hhit->energy() * recal;
		  }
	        }
		 
                if ((hhit->id()).ieta() < 17){
		  if (hhit->energy()*recal < 0.23){
		    HcalAxBxDepthEnergy += 0;
		  }
		  if (hhit->energy()*recal > 0.23){
		    HcalAxBxDepthEnergy += hhit->energy() * recal;
		  }
	        }
	       
	      

		  if(DIPHI==-1  && DIETA== 1) {HCAL3x3[0] += hhit->energy() * recal;}
		  if(DIPHI==-1  && DIETA== 0) {HCAL3x3[1] += hhit->energy() * recal;}
		  if(DIPHI==-1  && DIETA==-1) {HCAL3x3[2] += hhit->energy() * recal;}
		  if(DIPHI== 0  && DIETA== 1) {HCAL3x3[3] += hhit->energy() * recal;}
		  if(DIPHI== 0  && DIETA== 0) {HCAL3x3[4] += hhit->energy() * recal;}
		  if(DIPHI== 0  && DIETA==-1) {HCAL3x3[5] += hhit->energy() * recal;}
		  if(DIPHI== 1  && DIETA== 1) {HCAL3x3[6] += hhit->energy() * recal;}
		  if(DIPHI== 1  && DIETA== 0) {HCAL3x3[7] += hhit->energy() * recal;}
		  if(DIPHI== 1  && DIETA==-1) {HCAL3x3[8] += hhit->energy() * recal;}

		    
		  if(calEnergy > energyMinIso && calEnergy < energyMaxIso && econus < 1. && MaxHit.hitenergy > 0.){
		      rawEnergyVec.push_back(hhit->energy() * recal * corrHCAL);
		      detidvec.push_back(hhit->id());
		      numbers2[(hhit->id()).ieta()+29][(hhit->id()).iphi()] = numbers2[(hhit->id()).ieta()+29][(hhit->id()).iphi()] + 1;
		      
		  }
	      }
	    }
	}

      if(AxB_=="5x5")
	{
	  HcalAxBxDepthEnergy=0.;
	  for(int ih=0; ih<9; ih++){HCAL5x5[ih]=0.;}

	  ///////Safronov
	  //clear usedHits
	  usedHits.clear();
	  ///////
      
	  for (HBHERecHitCollection::const_iterator hhit=Hithbhe.begin(); hhit!=Hithbhe.end(); hhit++) 
	    {
	      //////////// Safronov
	      //check that this hit was not considered before and push it into usedHits
	      bool hitIsUsed=false;
	      int hitHashedIndex=hhit->id().hashed_index();
	      for (uint32_t i=0; i<usedHits.size(); i++)
		{
		  if (usedHits[i]==hitHashedIndex) hitIsUsed=true;
		}
	      if (hitIsUsed) continue;
	      usedHits.push_back(hitHashedIndex);
	      ////////////
    
	      int DIETA = 100;
	      if(MaxHit.ietahit*(hhit->id()).ieta()>0)	{DIETA = MaxHit.ietahit - (hhit->id()).ieta();}
	      if(MaxHit.ietahit*(hhit->id()).ieta()<0){ DIETA = MaxHit.ietahit - (hhit->id()).ieta();DIETA = DIETA>0 ? DIETA-1 : DIETA+1;}
	      int DIPHI = MaxHit.iphihit - (hhit->id()).iphi();
	      DIPHI = DIPHI>36 ? 72-DIPHI : DIPHI;
	      
	      if( abs(DIETA)<=2 && abs(DIPHI)<=2) 
		{
		  // rof 16.05.2008 start: include the possibility for recalibration
		  DetId mydetid = hhit->id().rawId();
		  //  float recal = myRecalib->getValues(mydetid)->getValue();
		  float recal = 1;
		  // rof end

		
	        if ((hhit->id()).ieta() > 16){
		  if (hhit->energy()*recal < 0.31){
		    HcalAxBxDepthEnergy += 0;
		  }
		  if (hhit->energy()*recal > 0.31){
		    HcalAxBxDepthEnergy += hhit->energy() * recal;
		  }
	        }
		 
                if ((hhit->id()).ieta() < 17){
		  if (hhit->energy()*recal < 0.23){
		    HcalAxBxDepthEnergy += 0;
		  }
		  if (hhit->energy()*recal > 0.23){
		    HcalAxBxDepthEnergy += hhit->energy() * recal;
		  }
	        }		
	  
		  
		  
		  // HCAL Energy in each RecHit of 5x5 (sum of depth) 
		  // HCAL3x3[25] ->  0  1  2  3  4
		  //                 5  6  7  9 10
		  //                11 12 13 14 15
		  //                16 17 18 19 20
		  //                21 22 23 24 25
		  if(DIPHI==-2  && DIETA== 2) {HCAL3x3[0] += hhit->energy() * recal;}
		  if(DIPHI==-2  && DIETA== 1) {HCAL3x3[1] += hhit->energy() * recal;}
		  if(DIPHI==-2  && DIETA== 0) {HCAL3x3[3] += hhit->energy() * recal;}
		  if(DIPHI==-2  && DIETA==-1) {HCAL3x3[4] += hhit->energy() * recal;}
		  if(DIPHI==-2  && DIETA==-2) {HCAL3x3[5] += hhit->energy() * recal;}

		  if(DIPHI==-1  && DIETA== 2) {HCAL3x3[6] += hhit->energy() * recal;}
		  if(DIPHI==-1  && DIETA== 1) {HCAL3x3[7] += hhit->energy() * recal;}
		  if(DIPHI==-1  && DIETA== 0) {HCAL3x3[8] += hhit->energy() * recal;}
		  if(DIPHI==-1  && DIETA==-1) {HCAL3x3[9] += hhit->energy() * recal;}
		  if(DIPHI==-1  && DIETA==-2) {HCAL3x3[10] += hhit->energy() * recal;}

		  if(DIPHI== 0  && DIETA== 2) {HCAL3x3[11] += hhit->energy() * recal;}
		  if(DIPHI== 0  && DIETA== 1) {HCAL3x3[12] += hhit->energy() * recal;}
		  if(DIPHI== 0  && DIETA== 0) {HCAL3x3[13] += hhit->energy() * recal;}
		  if(DIPHI== 0  && DIETA==-1) {HCAL3x3[14] += hhit->energy() * recal;}
		  if(DIPHI== 0  && DIETA==-2) {HCAL3x3[15] += hhit->energy() * recal;}
		  
		  if(DIPHI== 1  && DIETA== 2) {HCAL3x3[16] += hhit->energy() * recal;}
		  if(DIPHI== 1  && DIETA== 1) {HCAL3x3[17] += hhit->energy() * recal;}
		  if(DIPHI== 1  && DIETA== 0) {HCAL3x3[18] += hhit->energy() * recal;}
		  if(DIPHI== 1  && DIETA==-1) {HCAL3x3[19] += hhit->energy() * recal;}
		  if(DIPHI== 1  && DIETA==-2) {HCAL3x3[20] += hhit->energy() * recal;}
		  
		  if(DIPHI== 2  && DIETA== 2) {HCAL3x3[21] += hhit->energy() * recal;}
		  if(DIPHI== 2  && DIETA== 1) {HCAL3x3[22] += hhit->energy() * recal;}
		  if(DIPHI== 2  && DIETA== 0) {HCAL3x3[23] += hhit->energy() * recal;}
		  if(DIPHI== 2  && DIETA==-1) {HCAL3x3[24] += hhit->energy() * recal;}
		  if(DIPHI== 2  && DIETA==-2) {HCAL3x3[25] += hhit->energy() * recal;}

		  rawEnergyVec.push_back(hhit->energy() * recal * corrHCAL);
		  detidvec.push_back(hhit->id());
		}	  
	    }
	}
      
      if(AxB_!="3x3" && AxB_!="5x5") LogWarning(" AxB ")<<"   Not supported: "<< AxB_;

      if(calEnergy > energyMinIso && calEnergy < energyMaxIso && econus < 1. && 
	 (MaxHit.ietahit+29) > -1 && (MaxHit.ietahit+29) < 60 && MaxHit.hitenergy > 0.)
	{
	  EventMatrix.push_back(rawEnergyVec);
	  EventIds.push_back(detidvec);
	  EnergyVector.push_back(calEnergy);

	  numbers[MaxHit.ietahit+29][MaxHit.iphihit] = numbers[MaxHit.ietahit+29][MaxHit.iphihit] + 1;
	}

      if (calEnergy > 10. && econus < 1. && 
	 (MaxHit.ietahit+29) > -1 && (MaxHit.ietahit+29) < 60 && MaxHit.hitenergy > 0.){
	  input_to_L3 << rawEnergyVec.size() << "   " << calEnergy;
	  for (unsigned int i=0; i<rawEnergyVec.size(); i++)
	    {
	      input_to_L3 << "   " << rawEnergyVec.at(i) << "   " << detidvec.at(i).rawId();
	    }
	  input_to_L3 <<endl;
      }
      rawEnergyVec.clear();
      detidvec.clear();

      h_number_of_tracks_15->Fill(NisoTrk_gt_15);
      if (calEnergy > 15.) h_trk_eta_p_wo_mip_cut->Fill(trackEta,calEnergy);
      if (calEnergy > 15.) h_trk_eta_pt_wo_mip_cut->Fill(trackEta,trackPt);

      if (calEnergy > 10. && econus < 1. && (MaxHit.ietahit+29) > -1 && (MaxHit.ietahit+29) < 60 && MaxHit.hitenergy > 0.){
	if (calEnergy > 15. && calEnergy < 25. && abs(trackEta) < 1.3) h_mip_resp_barrel_20->Fill(HcalAxBxDepthEnergy/calEnergy);
        if (calEnergy > 25. && calEnergy < 40. && abs(trackEta) < 1.3) h_mip_resp_barrel_35->Fill(HcalAxBxDepthEnergy/calEnergy);
	if (calEnergy > 40. && calEnergy < 60. && abs(trackEta) < 1.3) h_mip_resp_barrel_50->Fill(HcalAxBxDepthEnergy/calEnergy);



        if (calEnergy > 25. && calEnergy < 60. && abs(trackEta) < 1.0 && (HcalAxBxDepthEnergy/calEnergy) > 1.7) {
	  h_50_tail_resp_TRK_P->Fill(calEnergy);
          h_50_tail_resp_TRK_ETA_PHI->Fill(abs(trackEta),abs(trackPhi));
	} 
        if (calEnergy > 25. && calEnergy < 60. && abs(trackEta) < 1.0 && (HcalAxBxDepthEnergy/calEnergy) < 0.4) {
	  h_50_low_tail_resp_TRK_P->Fill(calEnergy);
          h_50_low_tail_resp_TRK_ETA_PHI->Fill(abs(trackEta),abs(trackPhi));
	} 

	if (calEnergy > 15. && calEnergy < 25. && abs(trackEta) < 1.0) h_mip_resp_eta_lt_1_20->Fill(HcalAxBxDepthEnergy/calEnergy);
	if (calEnergy > 25. && calEnergy < 40. && abs(trackEta) < 1.0) h_mip_resp_eta_lt_1_35->Fill(HcalAxBxDepthEnergy/calEnergy);
	if (calEnergy > 40. && calEnergy < 60. && abs(trackEta) < 1.0) h_mip_resp_eta_lt_1_50->Fill(HcalAxBxDepthEnergy/calEnergy);

	if (calEnergy > 15. && calEnergy < 25. && abs(trackEta) < 1.3 && abs(trackEta) > 1.0) h_mip_resp_barrel_eta_gt_1_20->Fill(HcalAxBxDepthEnergy/calEnergy);
	if (calEnergy > 25. && calEnergy < 40. && abs(trackEta) < 1.3 && abs(trackEta) > 1.0) h_mip_resp_barrel_eta_gt_1_35->Fill(HcalAxBxDepthEnergy/calEnergy);
	if (calEnergy > 40. && calEnergy < 60. && abs(trackEta) < 1.3 && abs(trackEta) > 1.0) h_mip_resp_barrel_eta_gt_1_50->Fill(HcalAxBxDepthEnergy/calEnergy);

	if (calEnergy > 15. && abs(trackEta) < 1.3) h_p_trk_barrel->Fill(calEnergy);
	if (calEnergy > 15. && abs(trackEta) > 1.3 && abs(trackEta) < 2.4) h_p_trk_endcap->Fill(calEnergy);
	if (calEnergy > 15.){ 
	  h_trk_eta_p->Fill(trackEta,calEnergy);
	  h_trk_eta_pt->Fill(trackEta,trackPt);
	}

      }
     if (calEnergy > 10. && (MaxHit.ietahit+29) > -1 && (MaxHit.ietahit+29) < 60 && MaxHit.hitenergy > 0.){
       if (calEnergy > 17. && calEnergy < 23.){
	 h_ecal_cluster_20->Fill(ecal_cluster);
	 h_banana_20->Fill(ecal_cluster,HcalAxBxDepthEnergy);
       }
       if (calEnergy > 47. && calEnergy < 53.){
	 h_ecal_cluster_50->Fill(ecal_cluster);
	 h_banana_50->Fill(ecal_cluster,HcalAxBxDepthEnergy);
       }
     }

     //HE
     if (calEnergy > 10. && econus < 1. && (MaxHit.ietahit+29) > -1 && (MaxHit.ietahit+29) < 60 && MaxHit.hitenergy > 0.){
	if (calEnergy > 25. && calEnergy < 40. && abs(trackEta) > 1.3) h_HE_mipresp_25_40->Fill(HcalAxBxDepthEnergy/calEnergy);
	if (calEnergy > 40. && calEnergy < 50. && abs(trackEta) > 1.3) h_HE_mipresp_40_50->Fill(HcalAxBxDepthEnergy/calEnergy);
	if (calEnergy > 50. && calEnergy < 60. && abs(trackEta) > 1.3) h_HE_mipresp_50_60->Fill(HcalAxBxDepthEnergy/calEnergy);
	if (calEnergy > 60. && calEnergy < 80. && abs(trackEta) > 1.3) h_HE_mipresp_60_80->Fill(HcalAxBxDepthEnergy/calEnergy);
	if (calEnergy > 80. && calEnergy < 110. && abs(trackEta) > 1.3) h_HE_mipresp_80_110->Fill(HcalAxBxDepthEnergy/calEnergy);
	if (calEnergy > 290. && calEnergy < 3200. && abs(trackEta) > 1.3) h_HE_mipresp_290_320->Fill(HcalAxBxDepthEnergy/calEnergy);
     }

      nHORecHits=0;
      try {
	Handle<HORecHitCollection> ho;
	iEvent.getByLabel(hoLabel_,ho);
	const HORecHitCollection Hitho = *(ho.product());

	///////Safronov
	//clear usedHits
	usedHits.clear();
	///////

	for(HORecHitCollection::const_iterator hoItr=Hitho.begin(); hoItr!=Hitho.end(); hoItr++)
	  {
	    //////////// Safronov
	    //check that this hit was not considered before and push it into usedHits
	    bool hitIsUsed=false;
	    int hitHashedIndex=hoItr->id().hashed_index();
	    for (uint32_t i=0; i<usedHits.size(); i++)
	      {
		if (usedHits[i]==hitHashedIndex) hitIsUsed=true;
	      }
	    if (hitIsUsed) continue;
	    usedHits.push_back(hitHashedIndex);
	    ////////////

	    // rof 16.05.2008 start: include the possibility for recalibration
	    DetId mydetid = hoItr->id().rawId();
	    //	    float recal = myRecalib->getValues(mydetid)->getValue();
	    float recal = 1;
	    // rof end


	    GlobalPoint pos = geo->getPosition(hoItr->detid());
	    double phihit = pos.phi();
	    double etahit = pos.eta();
	    
	    int iphihit = (hoItr->id()).iphi();
	    int ietahit = (hoItr->id()).ieta();
	    int depthhit = (hoItr->id()).depth();
	    
	    double dphi = fabs(trackPhi - phihit); 
	    if(dphi > 4.*atan(1.)) dphi = 8.*atan(1.) - dphi;
	    double deta = fabs(trackEta - etahit); 
	    double dr = sqrt(dphi*dphi + deta*deta);
	    
	    if(dr<associationConeSize_) {
	      hoRHen[nHORecHits] = hoItr->energy() * recal;
	      hoRHeta[nHORecHits] = etahit;
	      hoRHphi[nHORecHits] = phihit;
	      hoRHieta[nHORecHits] = ietahit;
	      hoRHiphi[nHORecHits] = iphihit;
	      hoRHdepth[nHORecHits] = depthhit;
	      nHORecHits++;
	      
	    }
	  }
      } catch (cms::Exception& e) { // can't find it!
	if (!allowMissingInputs_) throw e;
      }

      
    }
}

// ------------ method called once each job just before starting event loop  ------------
void 
HcalIsoTrkAnalyzer::beginJob(const edm::EventSetup&)
{
  NisoTrk=0;

  //  MyL3Algo = new MinL3AlgoUniv<HcalDetId>(eventWeight);
  input_to_L3.open("input_to_l3.txt");

  tf2 = new TFile(outputFileName_.c_str(),"RECREATE");

  h_mip_resp_barrel_20 = new TH1F("h_mip_resp_barrel_20","MIP RESP Barrel 15<P_{trk}<25 GeV/c", 100,0.,4.);
  h_mip_resp_barrel_35 = new TH1F("h_mip_resp_barrel_35","MIP RESP Barrel 25<P_{trk}<40 GeV/c", 100,0.,4.);
  h_mip_resp_barrel_50 = new TH1F("h_mip_resp_barrel_50","MIP RESP Barrel 40<P_{trk}<60 GeV/c", 100,0.,4.);

  h_HE_mipresp_25_40 = new TH1F("h_HE_mipresp_25_40","MIP RESP Endcap 25<P_{trk}<40 GeV/c", 100,0.,4.);
  h_HE_mipresp_40_50 = new TH1F("h_HE_mipresp_40_50","MIP RESP Endcap 40<P_{trk}<50 GeV/c", 100,0.,4.);
  h_HE_mipresp_50_60 = new TH1F("h_HE_mipresp_50_60","MIP RESP Endcap 50<P_{trk}<60 GeV/c", 100,0.,4.);
  h_HE_mipresp_60_80 = new TH1F("h_HE_mipresp_60_80","MIP RESP Endcap 60<P_{trk}<80 GeV/c", 100,0.,4.);
  h_HE_mipresp_80_110 = new TH1F("h_HE_mipresp_80_110","MIP RESP Endcap 80<P_{trk}<110 GeV/c", 100,0.,4.);
  h_HE_mipresp_290_320 = new TH1F("h_HE_mipresp_290_320","MIP RESP Endcap 290<P_{trk}<320 GeV/c", 100,0.,4.);


  h_mip_resp_eta_lt_1_20 = new TH1F("h_mip_resp_eta_lt_1_20","MIP RESP |#eta|<1 & 15<P_{trk}<25 GeV/c ", 100,0.,4.);
  h_mip_resp_eta_lt_1_35 = new TH1F("h_mip_resp_eta_lt_1_35","MIP RESP |#eta|<1 & 25<P_{trk}<40 GeV/c ", 100,0.,4.);
  h_mip_resp_eta_lt_1_50 = new TH1F("h_mip_resp_eta_lt_1_50","MIP RESP |#eta|<1 & 40<P_{trk}<60 GeV/c", 100,0.,4.);

  h_mip_resp_barrel_eta_gt_1_20 = new TH1F("h_mip_resp_barrel_eta_gt_1_20","MIP RESP 1<|#eta|<1.3 & 15<P_{trk}<25 GeV/c", 100,0.,4.);
  h_mip_resp_barrel_eta_gt_1_35 = new TH1F("h_mip_resp_barrel_eta_gt_1_35","MIP RESP 1<|#eta|<1.3 & 25<P_{trk}<40 GeV/c", 100,0.,4.);
  h_mip_resp_barrel_eta_gt_1_50 = new TH1F("h_mip_resp_barrel_eta_gt_1_50","MIP RESP 1<|#eta|<1.3 & 40<P_{trk}<60 GeV/c", 100,0.,4.);

  h_banana_20 = new TH2F("h_banana_20","15-25",1000,-10,100,1000,-10,100);
  h_ecal_cluster_20 = new TH1F("h_ecal_cluster_20","ecal_cluster_15-25",1000,-10,100);
  h_banana_35 = new TH2F("h_banana_35","25-40",1000,-10,100,1000,-10,100);
  h_ecal_cluster_35 = new TH1F("h_ecal_cluster_35","ecal_cluster_25-40",1000,-10,100);
  h_banana_50 = new TH2F("h_banana_50","45-60",1000,-10,100,1000,-10,100);
  h_ecal_cluster_50 = new TH1F("h_ecal_cluster_50","ecal_cluster_45-60",1000,-10,100);

  h_number_of_tracks_15 = new TH1F("h_number_of_tracks_15","Number of tracks > 15 GeV/c",10,0,10);
  h_p_trk_barrel = new TH1F("h_p_trk_barrel","h_p_trk_barrel",600,0,100);
  h_p_trk_endcap = new TH1F("h_p_trk_endcap","h_p_trk_endcap",600,0,100);
  h_trk_eta_p = new TH2F("h_trk_eta_p","Track #eta vs p for mip in ecal events",100,-3,4,600,0,100);
  h_trk_eta_p->GetXaxis()->SetTitle("#eta");
  h_trk_eta_p->GetYaxis()->SetTitle("p (GeV/c)");
  h_trk_eta_p_wo_mip_cut = new TH2F("h_trk_eta_p_wo_mip_cut","Track #eta vs p wo mip cut",100,-3,4,600,0,100);
  h_trk_eta_p_wo_mip_cut->GetXaxis()->SetTitle("#eta");
  h_trk_eta_p_wo_mip_cut->GetYaxis()->SetTitle("p (GeV/c)");

  h_trk_eta_pt = new TH2F("h_trk_eta_pt","Track #eta vs pt for mip in ecal events",100,-3,4,600,0,100);
  h_trk_eta_pt->GetXaxis()->SetTitle("#eta");
  h_trk_eta_pt->GetYaxis()->SetTitle("pt (GeV/c)");
  h_trk_eta_pt_wo_mip_cut = new TH2F("h_trk_eta_pt_wo_mip_cut","Track #eta vs pt wo mip cut",100,-3,4,600,0,100);
  h_trk_eta_pt_wo_mip_cut->GetXaxis()->SetTitle("#eta");
  h_trk_eta_pt_wo_mip_cut->GetYaxis()->SetTitle("pt (GeV/c)");

  h_50_tail_resp_TRK_P = new TH1F("h_50_tail_resp_TRK_P","TRK P for rep(45-60)>1.7",600,0,100);
  h_50_low_tail_resp_TRK_P = new TH1F("h_50_low_tail_resp_TRK_P","TRK P for rep(45-60)<0.5",600,0,100);
  h_50_tail_resp_TRK_ETA_PHI = new TH2F("h_50_tail_resp_TRK_ETA_PHI","TRK #eta vs #phi for rep(45-60)>1.7",100,0,3,100,0,4);
  h_50_low_tail_resp_TRK_ETA_PHI = new TH2F("h_50_low_tail_resp_TRK_ETA_PHI","TRK #eta vs #phi for rep(45-60)<0.5",100,0,3,100,0,4);

}

// ------------ method called once each job just after ending the event loop  ------------
void 
HcalIsoTrkAnalyzer::endJob() {
  /*
  // M+
  // perform the calibration with given number of iterations  
  cout<<" Begin of solution "<< EnergyVector.size() << "  "<< EventMatrix.size()<< "  "<< EventIds.size()<<endl; 
  solution = MyL3Algo->iterate(EventMatrix, EventIds, EnergyVector, nIterations);
  cout<<" End of solution "<<endl; 

  // print the solution and make a few plots
  map<HcalDetId,float>::iterator ii;
  for (ii = solution.begin(); ii != solution.end(); ii++)
    {
      int curr_eta = ii->first.ieta();
      int curr_phi = ii->first.iphi();
      int curr_depth = ii->first.depth();
      cout << "solution[eta=" << curr_eta << ",phi=" << curr_phi << ",subdet=" << ii->first.subdet()
	   << ",depth=" << curr_depth << "] = " << ii->second 
	   << " Stat " << numbers[curr_eta+29][curr_phi] << "  " << numbers2[curr_eta+29][curr_phi]
	   << endl; 
    }

  for (ii = solution.begin(); ii != solution.end(); ii++)
    {
      int curr_eta = ii->first.ieta();
      int curr_phi = ii->first.iphi();
      int curr_depth = ii->first.depth();
      cout << "  "<< ii->first.subdet() << "  " << curr_depth  << "  "<< curr_eta << "  " << curr_phi <<  "  "<< ii->second
	   << endl; 
    }
  */
  input_to_L3.close();

  tf2->cd();


  h_mip_resp_barrel_20->Write();
  h_mip_resp_barrel_35->Write();
  h_mip_resp_barrel_50->Write();
  h_mip_resp_eta_lt_1_20->Write();
  h_mip_resp_eta_lt_1_35->Write();
  h_mip_resp_eta_lt_1_50->Write();
  h_mip_resp_barrel_eta_gt_1_20->Write();
  h_mip_resp_barrel_eta_gt_1_35->Write();
  h_mip_resp_barrel_eta_gt_1_50->Write();

  h_banana_20->Write();
  h_ecal_cluster_20->Write();
  h_banana_35->Write();
  h_ecal_cluster_35->Write();
  h_banana_50->Write();
  h_ecal_cluster_50->Write();

  h_number_of_tracks_15->Write();
  h_p_trk_barrel->Write();
  h_p_trk_endcap->Write();

  h_trk_eta_p->Write();
  h_trk_eta_p_wo_mip_cut->Write();
  h_trk_eta_pt->Write();
  h_trk_eta_pt_wo_mip_cut->Write();

  h_50_tail_resp_TRK_P->Write();
  h_50_tail_resp_TRK_ETA_PHI->Write();
  h_50_low_tail_resp_TRK_P->Write();
  h_50_low_tail_resp_TRK_ETA_PHI->Write();

  h_HE_mipresp_25_40->Write();
  h_HE_mipresp_40_50->Write();
  h_HE_mipresp_50_60->Write();
  h_HE_mipresp_60_80->Write();
  h_HE_mipresp_80_110->Write();
  h_HE_mipresp_290_320->Write();	

}

//define this as a plug-in
DEFINE_FWK_MODULE(HcalIsoTrkAnalyzer);

