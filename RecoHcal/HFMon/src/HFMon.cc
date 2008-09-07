// -*- C++ -*-
//
// Package:    HFMon
// Class:      HFMon
// 
/**\class HFMon HFMon.cc RecoHcal/HFMon/src/HFMon.cc

   Description: <one line class summary>

   Implementation:
   <Notes on implementation>
*/
//
// Original Author:  Efe YAZGAN
//         Created:  Fri Sep  5 14:54:54 CEST 2008
// $Id$
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
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
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
//trigger
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuRegionalCand.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTReadoutCollection.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GtPsbWord.h"
//TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"


//@@@@@@@@@@@@@@@@@@@@ L1 Technical Trigger Bits
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetup.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


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

#include "HFMon.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <sys/time.h>

using namespace edm;
using namespace std;
using namespace reco;

HFMon::HFMon(const edm::ParameterSet& iConfig)
{
  //now do what ever initialization is needed
 runBegin = -1;
}


HFMon::~HFMon()
{
 
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
HFMon::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  time_t a = (iEvent.time().value()) >> 32;

  int lumi = iEvent.luminosityBlock();

 

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
  //cout << " Luminosity Block: " << lumibegin<<endl;

  /*
    Handle< L1GlobalTriggerReadoutRecord > gtRecord;
    iEvent.getByLabel( "gtDigis", gtRecord);
    const TechnicalTriggerWord tWord = gtRecord->technicalTriggerWord();
    bool techTrigger37 = tWord.at(37);
  */

  Handle < HFDigiCollection > hf_digi;
  iEvent.getByLabel("hcalDigis", hf_digi);
  const HFDigiCollection hf = *(hf_digi.product());



  Handle < HFRecHitCollection > hfrh;
  iEvent.getByLabel("hfreco", hfrh);
  //  const HFRecHitCollection Hithf = *(hfrh.product());
  const HFRecHitCollection hfHits = *(hfrh.product());
  HFRecHitCollection::const_iterator HFiter;



  const int HFETASIZE = 84;
  const int ETA_OFFSET_HF = 41;
  const int ETA_BOUND_HF = 29;
  float occThresh_ = 1.;

  //  double totalX=0;
  //  double totalY=0;
  //  double totalE=0;

  double HFtotalX=0;
  double HFtotalY=0;
  double HFtotalE=0;
  float etaBounds[13] = { 2.853, 2.964, 3.139, 3.314, 3.489, 3.664, 3.839, 4.013, 4.191, 4.363, 4.538, 4.716, 4.889};
  float area[13]={0.111,0.175,0.175,0.175,0.175,0.175,0.174,0.178,0.172,0.175,0.178,0.346,0.604};
  float radius[13]={1300,1162,975,818,686,576,483,406,340,286,240,201,169};

     
  float hitsp[13][36][2];
  float hitsm[13][36][2];

  for(int m=0;m<13;m++){
    for(int n=0;n<36;n++){
      hitsp[m][n][0]=0;
      hitsp[m][n][1]=0; 
      hitsm[m][n][0]=0;
      hitsm[m][n][1]=0;
    }
  }
  
  ///////////////////////////////////
  // HF loop
  if(hfHits.size()>0)
    {
      double HF_weightedX[HFETASIZE]={0.};
      double HF_weightedY[HFETASIZE]={0.};
      double HF_energy[HFETASIZE]={0.};
      double offset;

      int ieta, iphi;
      float et,eta,phi,r;
      for (HFiter=hfHits.begin(); HFiter!=hfHits.end();++HFiter){ 
	if (HFiter->energy()<0) continue;  // don't include negative-energy cells?
	eta = etaBounds[abs(HFiter->id().ieta())-29];
	et = HFiter->energy()/cosh(eta)/area[abs(HFiter->id().ieta())-29];
	r = radius[abs(HFiter->id().ieta())-29];
	if (HFiter->id().iphi()<37){ 
	  phi = HFiter->id().iphi()*0.087266;
	}
	else{ 
	  phi = (HFiter->id().iphi()-72)*0.087266;
	}

	if (HFiter->id().depth() == 1){                      
	  if(HFiter->id().ieta() > 0){
      	    Etsum_eta_L->Fill(eta,et);
            Etsum_phi_L->Fill(phi,et);
            Etsum_map_L->Fill(eta,phi,et);
            Etsum_rphi_L->Fill(r,phi,et);
            hitsp[HFiter->id().ieta()-29][(HFiter->id().iphi()-1)/2][0] = HFiter->energy();
	  }
	  if(HFiter->id().ieta() < 0){
            Etsum_eta_L->Fill(-eta,et);
            Etsum_phi_L->Fill(phi,et);
            Etsum_rphi_L->Fill(r,phi,et);
            Etsum_map_L->Fill(-eta,phi,et);
            hitsm[-HFiter->id().ieta()-29][(HFiter->id().iphi()-1)/2][0] = HFiter->energy(); 	    
	  }
	} 


	//Fill 3 histos for Short Fibers :
	if (HFiter->id().depth() == 2){
	  if(HFiter->id().ieta() > 0){
	    Etsum_eta_S->Fill(eta,et);
	    Etsum_phi_S->Fill(phi,et);
	    Etsum_rphi_S->Fill(r,phi,et); 
	    Etsum_map_S->Fill(eta,phi,et);
	    hitsp[HFiter->id().ieta()-29][(HFiter->id().iphi()-1)/2][1] = HFiter->energy();
	  }
	  if(HFiter->id().ieta() < 0){  
	    Etsum_eta_S->Fill(-eta,et);
	    Etsum_map_S->Fill(-eta,phi,et);
	    Etsum_phi_S->Fill(phi,et);
	    Etsum_rphi_S->Fill(r,phi,et); 
	    hitsm[-HFiter->id().ieta()-29][(HFiter->id().iphi()-1)/2][1] = HFiter->energy();
	  }          
	}
	Energy_Occ->Fill(HFiter->energy()); 

	//HF: no non-threshold occupancy map is filled?
	           
	if(HFiter->energy()>occThresh_){
	  if (HFiter->id().depth() == 1){
	    if(HFiter->id().ieta() > 0){ 
		Occ_eta_L->Fill(eta,1);
		Occ_phi_L->Fill(phi,1);
		Occ_map_L->Fill(eta,phi,1);
		Occ_rphi_L->Fill(r,phi,1);
	      }
	    if(HFiter->id().ieta() < 0){ 
	      Occ_eta_L->Fill(-eta,1);
	      Occ_phi_L->Fill(phi,1);
	      Occ_map_L->Fill(-eta,phi,1);
	      Occ_rphi_L->Fill(r,phi,1);
	    }
	  }
	  if (HFiter->id().depth() == 2){
	    if(HFiter->id().ieta() > 0){ 
	      Occ_eta_S->Fill(eta,1);
	      Occ_phi_S->Fill(phi,1);
	      Occ_map_S->Fill(eta,phi,1);
	      Occ_rphi_S->Fill(r,phi,1);
            }  
            if(HFiter->id().ieta() < 0){ 
	      Occ_eta_S->Fill(-eta,1);
	      Occ_map_S->Fill(-eta,phi,1);
	      Occ_phi_S->Fill(phi,1);
	      Occ_rphi_S->Fill(r,phi,1);
	    }  
	  }
	}
        else{ 
	  if (HFiter->id().depth() == 1){ 
	    if(HFiter->id().ieta() > 0){ 
		Occ_eta_L->Fill(eta,0);
		Occ_map_L->Fill(eta,phi,0);
		Occ_phi_L->Fill(phi,0); 
		Occ_rphi_L->Fill(r,phi,0);
	    }
	    if(HFiter->id().ieta() < 0){ 
	      Occ_eta_L->Fill(-eta,0);
	      Occ_map_L->Fill(-eta,phi,0);
	      Occ_phi_L->Fill(phi,0);
	      Occ_rphi_L->Fill(r,phi,0);
	    }
	  }
	  if (HFiter->id().depth() == 2){
	    if(HFiter->id().ieta() > 0){ 
	      Occ_eta_S->Fill(eta,0);
	      Occ_map_S->Fill(eta,phi,0);
	      Occ_phi_S->Fill(phi,0);
	      Occ_rphi_S->Fill(r,phi,0);
	    }  
            if(HFiter->id().ieta() < 0){ 
	      Occ_eta_S->Fill(-eta,0);
	      Occ_map_S->Fill(-eta,phi,0);
	      Occ_phi_S->Fill(phi,0);
	      Occ_rphi_S->Fill(r,phi,0);
	    }  
	  }
	}//else

	HcalDetId id(HFiter->detid().rawId());
	ieta=id.ieta();
	iphi=id.iphi();

	HFtotalX+=HFiter->energy()*cos(2.*TMath::Pi()*iphi/72);
	HFtotalY+=HFiter->energy()*sin(2.*TMath::Pi()*iphi/72);
	HFtotalE+=HFiter->energy();

	unsigned int index;
	index=ieta+ETA_OFFSET_HF;
	HF_weightedX[index]+=HFiter->energy()*cos(2.*TMath::Pi()*iphi/72);
	HF_weightedY[index]+=HFiter->energy()*sin(2.*TMath::Pi()*iphi/72);
	HF_energy[index]+=HFiter->energy();
      } // for (HFiter=hfHits.begin();...)

      int hfeta=ETA_OFFSET_HF;
      for (int i=-1*hfeta;i<=hfeta;++i){
	  if (i == 0) continue;
	  if (i > -1*ETA_BOUND_HF && i < ETA_BOUND_HF) continue;
	  int index = i + ETA_OFFSET_HF;
	  if (HF_energy[index] == 0) continue;
	  double moment = pow(HF_weightedX[index],2)+pow(HF_weightedY[index],2);
	  moment = pow(moment,0.5);
	  moment /= HF_energy[index];
	  offset = (i>0 ? 0.5: -0.5);
	  if (moment !=0 ){
	      //		  HF_CenterOfEnergyRadius[i]->Fill(moment);
	      COEradiusVSeta->Fill(i+offset,moment);
	    }
	} // for (int i=-1*hfeta;i<=hfeta;++i)
      float ratiom,ratiop;
      
      for(int i=0;i<13;i++){
        for(int j=0;j<36;j++){
          
          if(hitsp[i][j][0]==hitsp[i][j][1]) continue;
                    
          ratiop=(hitsp[i][j][0]-hitsp[i][j][1])/(hitsp[i][j][0]+hitsp[i][j][1]);
          //cout<<ratiop<<endl;
	  Etsum_ratio_p->Fill(ratiop);
          if(abs(ratiop>0.85))
	    Etsum_ratio_map->Fill(i+29,2*j+1); 
	}
      }

      for(int p=0;p<13;p++){
        for(int q=0;q<36;q++){
	 
	  if(hitsm[p][q][0]==hitsm[p][q][1]) continue;
	  ratiom=(hitsm[p][q][0]-hitsm[p][q][1])/(hitsm[p][q][0]+hitsm[p][q][1]);         
	  Etsum_ratio_m->Fill(ratiom);
	  if(abs(ratiom>0.85))
            Etsum_ratio_map->Fill(-p-29,2*q+1);
	}
      } 
    } // if (hfHits.size()>0)
      

  
    double moment;
    if (HFtotalE > 0){
      moment=pow(HFtotalX*HFtotalX+HFtotalY*HFtotalY,0.5)/HFtotalE;
      HFCenterOfEnergyRadius->Fill(moment);
      HFCenterOfEnergy->Fill(HFtotalX/HFtotalE, HFtotalY/HFtotalE);
    }

    for (HFDigiCollection::const_iterator j=hf.begin(); j!=hf.end(); j++){
      const HFDataFrame digi = (const HFDataFrame)(*j);
      //  calibs_= cond.getHcalCalibrations(digi.id());  // Old method was made private. 
      //       float en=0;
      //       float ts =0; float bs=0;
      //       int maxi=0; float maxa=0;
      //       for(int i=sigS0_; i<=sigS1_; i++){
      // 	if(digi.sample(i).adc()>maxa){maxa=digi.sample(i).adc(); maxi=i;}
      //       }
      //       for(int i=sigS0_; i<=sigS1_; i++){	  
      // 	float tmp1 =0;   
      //         int j1=digi.sample(i).adc();
      //         tmp1 = (LedMonAdc2fc[j1]+0.5);   	  
      // 	en += tmp1-calibs_.pedestal(digi.sample(i).capid());
      // 	if(i>=(maxi-1) && i<=maxi+1){
      // 	  ts += i*(tmp1-calibs_.pedestal(digi.sample(i).capid()));
      // 	  bs += tmp1-calibs_.pedestal(digi.sample(i).capid());
      // 	}
      //       }
      
      //---HFlumiplots
      int theTStobeused = 6;
      // will have masking later:
      int mask = 1; 
      if (mask != 1) continue;
      //if we want to sum the 10 TS instead of just taking one:
      for (int i = 0; i<digi.size(); i++) {
	if (i == theTStobeused) {
	  float tmpET =0;
	  int jadc = digi.sample(i).adc();
	  //NOW LUT used in HLX are only identy LUTs, so Et filled
	  //with unlinearised adc, ie tmpET = jadc
	  //	  tmpET = (adc2fc[jadc]+0.5);
	  tmpET = jadc;

	  //-find which wedge we are in
	  //  ETsum and Occupancy will be summed for both L and S
	  if(digi.id().ieta() > 28){
	    if((digi.id().iphi() == 1) || (digi.id().iphi() == 71)){
	      HFlumi_ETsum_perwedge->Fill(1,tmpET);
	      if((digi.id().ieta() == 33) || (digi.id().ieta() == 34)) {
		if(jadc > 100) HFlumi_Occupancy_above_thr_r1->Fill(1,1);
		if((jadc >= 10) && (jadc <= 100)) HFlumi_Occupancy_between_thrs_r1->Fill(1,1);
		if(jadc < 10) HFlumi_Occupancy_below_thr_r1->Fill(1,1);
	      }
	      else if((digi.id().ieta()==35)||(digi.id().ieta()==36)) {
		if(jadc>100) HFlumi_Occupancy_above_thr_r2->Fill(1,1);
		if((jadc>=10)&&(jadc<=100)) HFlumi_Occupancy_between_thrs_r2->Fill(1,1);
		if(jadc<10) HFlumi_Occupancy_below_thr_r2->Fill(1,1);
	      }
	    }
	    else {
	      for (int iwedge=2; iwedge<19; iwedge++) {
		int itmp=4*(iwedge-1);
		if( (digi.id().iphi()==(itmp+1)) || (digi.id().iphi()==(itmp-1))) {
		  HFlumi_ETsum_perwedge->Fill(iwedge,tmpET);
		  if((digi.id().ieta()==33)||(digi.id().ieta()==34)) {
		    if(jadc>100) HFlumi_Occupancy_above_thr_r1->Fill(iwedge,1);
		    if((jadc>=10)&&(jadc<=100)) HFlumi_Occupancy_between_thrs_r1->Fill(iwedge,1);
		    if(jadc<10) HFlumi_Occupancy_below_thr_r1->Fill(iwedge,1);
		  }
		  else if((digi.id().ieta()==35)||(digi.id().ieta()==36)) {
		    if(jadc>100) HFlumi_Occupancy_above_thr_r2->Fill(iwedge,1);
		    if((jadc>=10)&&(jadc<=100)) HFlumi_Occupancy_between_thrs_r2->Fill(iwedge,1);
		    if(jadc<10) HFlumi_Occupancy_below_thr_r2->Fill(iwedge,1);
		  }
		  iwedge=99;
		}
	      }
	    }
	  }  //--endif ieta in HF+
	  else if(digi.id().ieta()<-28){
	    if((digi.id().iphi()==1)||(digi.id().iphi()==71)){
	      HFlumi_ETsum_perwedge->Fill(19,tmpET);
	      if((digi.id().ieta()==-33)||(digi.id().ieta()==-34)) {
		if(jadc>100) HFlumi_Occupancy_above_thr_r1->Fill(19,1);
		if((jadc>=10)&&(jadc<=100)) HFlumi_Occupancy_between_thrs_r1->Fill(19,1);
		if(jadc<10) HFlumi_Occupancy_below_thr_r1->Fill(19,1);
	      }
	      else if((digi.id().ieta() == -35) || (digi.id().ieta() == -36)) {
		if(jadc > 100) HFlumi_Occupancy_above_thr_r2->Fill(19,1);
		if((jadc >= 10) && (jadc <= 100)) HFlumi_Occupancy_between_thrs_r2->Fill(19,1);
		if(jadc < 10) HFlumi_Occupancy_below_thr_r2->Fill(19,1);
	      }
	    }
	    else {
	      for (int iw=2; iw<19; iw++) {
		int itemp=4*(iw-1);
		if( (digi.id().iphi() == (itemp+1)) || (digi.id().iphi() == (itemp-1))) {
		  HFlumi_ETsum_perwedge->Fill(iw+18,tmpET);
		  if((digi.id().ieta() == -33) || (digi.id().ieta() == -34)) {
		    if(jadc > 100) HFlumi_Occupancy_above_thr_r1->Fill(iw+18,1);
		    if((jadc >= 10) && (jadc <= 100)) HFlumi_Occupancy_between_thrs_r1->Fill(iw+18,1);
		    if(jadc < 10) HFlumi_Occupancy_below_thr_r1->Fill(iw+18,1);
		  }
		  else if((digi.id().ieta() == -35) || (digi.id().ieta() == -36)) {
		    if(jadc > 100) HFlumi_Occupancy_above_thr_r2->Fill(iw+18,1);
		    if((jadc >= 10) && (jadc <= 100)) HFlumi_Occupancy_between_thrs_r2->Fill(iw+18,1);
		    if(jadc < 10) HFlumi_Occupancy_below_thr_r2->Fill(iw+18,1);
		  }
		  iw = 99;
		}
	      }
	    }
	  }//---endif ieta inHF-
	}//---endif TS=nr6
      } 
    }//------end loop over TS for lumi
    
}


// ------------ method called once each job just before starting event loop  ------------
void 
HFMon::beginJob(const edm::EventSetup&)
{
}


TH1F *HFMon::book1DHistogram(TFileDirectory & fDir, const std::string & fName, const std::string & fTitle,
			     int fNbins, double fXmin, double fXmax) const {
  char title[1024];
  sprintf(title, "%s [RUN:%ld/%ld]", fTitle.c_str(), runBegin, lumibegin);
  std::cout << "booking 1d histogram " << title << std::endl;
  return fDir.make < TH1F > (fName.c_str(), title, fNbins, fXmin, fXmax);
} 
TH2F *HFMon::book2DHistogram(TFileDirectory & fDir, const std::string & fName, const std::string & fTitle,
			     int fNbinsX, double fXmin, double fXmax, int fNbinsY, double fYmin, double fYmax) const {
  char title[1024];

  sprintf(title, "%s [RUN:%ld/%ld]", fTitle.c_str(), runBegin, lumibegin);
  std::cout << "booking 2d histogram " << title << std::endl;
  return fDir.make < TH2F > (fName.c_str(), title, fNbinsX, fXmin, fXmax, fNbinsY, fYmin, fYmax);
} 
TProfile *HFMon::bookProfileHistogram(TFileDirectory & fDir, const std::string & fName, const std::string & fTitle,
				      int fNbins, double fXmin, double fXmax, double fYmin, double fYmax) const {
  char title[1024];
  sprintf(title, "%s [RUN:%ld/%ld]", fTitle.c_str(), runBegin, lumibegin);
  std::cout << "booking profile histogram " << title << std::endl;
  return fDir.make < TProfile > (fName.c_str(), title, fNbins, fXmin, fXmax, fYmin, fYmax); 
}

void HFMon::bookHistograms() {

  TFileDirectory HFDir = fs->mkdir("HF");
  TFileDirectory HFHistory = fs->mkdir("HFHistory");
  TFileDirectory JetMetDir = fs->mkdir("JetMet");
  TFileDirectory TriggerDir = fs->mkdir("Trigger");

  HFlumi_ETsum_perwedge =  book1DHistogram(HFDir, "HFlumi_ETsum_perwedge","HF lumi ET-sum per wedge",36,1,37);
  HFlumi_Occupancy_above_thr_r1 =  book1DHistogram(HFDir, "HFlumi_Occupancy_above_thr_r1","HF lumi Occupancy above threshold ring1",36,1,37);
  HFlumi_Occupancy_between_thrs_r1 = book1DHistogram(HFDir, "HFlumi_Occupancy_between_thrs_r1","HF lumi Occupancy between thresholds ring1",36,1,37);
  HFlumi_Occupancy_below_thr_r1 = book1DHistogram(HFDir, "HFlumi_Occupancy_below_thr_r1","HF lumi Occupancy below threshold ring1",36,1,37);
  HFlumi_Occupancy_above_thr_r2 = book1DHistogram(HFDir, "HFlumi_Occupancy_above_thr_r2","HF lumi Occupancy above threshold ring2",36,1,37);
  HFlumi_Occupancy_between_thrs_r2 = book1DHistogram(HFDir, "HFlumi_Occupancy_between_thrs_r2","HF lumi Occupancy between thresholds ring2",36,1,37);
  HFlumi_Occupancy_below_thr_r2 = book1DHistogram(HFDir, "HFlumi_Occupancy_below_thr_r2","HF lumi Occupancy below threshold ring2",36,1,37);
  CenterOfEnergyRadius = book1DHistogram(HFDir, "CenterOfEnergyRadius", "Center Of Energy radius", 200,0,1);
  CenterOfEnergy = book2DHistogram(HFDir, "CenterOfEnergy", "Center of Energy", 200,-1,1,200,-1,1);

  HFCenterOfEnergyRadius = book1DHistogram(HFDir, "HFCenterOfEnergyRadius", "HF Center Of Energy radius",200,0,1);
  HFCenterOfEnergy = book2DHistogram(HFDir, "HFCenterOfEnergy", "HF Center of Energy", 200,-1,1, 200,-1,1);
  
  // Wenhan's 
  Etsum_eta_L = bookProfileHistogram(HFDir, "Etsum_eta_L","Et Sum per Area vs Eta Long Fiber",120,-6,6,0,2000);
  Etsum_eta_S = bookProfileHistogram(HFDir, "Etsum_eta_S","Et Sum per Area vs Eta Short Fiber",120,-4,4,0,2000);
  Etsum_phi_L = bookProfileHistogram(HFDir, "Etsum_phi_L","Et Sum per Area vs Phi Long Fiber",100,-4,4,0,2000);
  Etsum_phi_S = bookProfileHistogram(HFDir, "Etsum_phi_S","Et Sum per Area crossing vs Phi Short Fiber",100,-4,4,0,2000);

  Occ_eta_S = bookProfileHistogram(HFDir, "Occ_eta_S","Occ per Bunch crossing vs Eta Short Fiber",120,-6,6,0,2000);
  Occ_eta_L = bookProfileHistogram(HFDir, "Occ_eta_L","Occ per Bunch crossing vs Eta long Fiber",120,-6,6,0,2000);
  Occ_phi_L = bookProfileHistogram(HFDir, "Occ_phi_L","Occ per Bunch crossing vs Phi Long Fiber",100,-4,4,0,2000);
  Occ_phi_S = bookProfileHistogram(HFDir, "Occ_phi_S","Occ per Bunch crossing vs Phi Short Fiber",100,-4,4,0,2000);
  COEradiusVSeta = bookProfileHistogram(HFDir, "COEradiusVSeta","Center of Energy radius vs i#eta",172,-43,43,0,1);

  Etsum_ratio_p = book1DHistogram(HFDir, "Etsum_ratio_p","Energy difference of Long and Short Fiber HF+",105,-1.05,1.05);
  Energy_Occ = book1DHistogram(HFDir, "Energy_Occ","Occupancy  vs Energy ",200,0,2000);
  Etsum_ratio_m = book1DHistogram(HFDir, "Etsum_ratio_m","Energy difference of Long and Short Fiber HF-",105,-1.05,1.05);
  Etsum_map_L = book2DHistogram(HFDir, "Etsum_map_L","Et Sum 2D phi and eta Long Fiber",120,-6,6,100,-4,4);
  Etsum_map_S = book2DHistogram(HFDir, "Etsum_map_S","Et Sum 2D phi and eta Short Fiber",120,-6,6,100,-4,4);
  Etsum_rphi_S = book2DHistogram(HFDir, "Etsum_rphi_S","Et Sum 2D phi and radius Short Fiber",100,0,1500,100,-4,4);
  Etsum_rphi_L = book2DHistogram(HFDir, "Etsum_rphi_L","Et Sum 2D phi and radius Long Fiber",100,0,1500,100,-4,4);
  Etsum_ratio_map = book2DHistogram(HFDir, "Etsum_ratio_map","Abnormal fm",84,-42,42,72,0,72);
  Occ_rphi_S = book2DHistogram(HFDir, "Occ_rphi_S","Occupancy 2D phi and radius Short Fiber",100,0,1500,100,-4,4);
  Occ_rphi_L = book2DHistogram(HFDir, "Occ_rphi_L","Occupancy 2D phi and radius Long Fiber",100,0,1500,100,-4,4);
  Occ_map_L = book2DHistogram(HFDir, "Occ_map_L","Occ Map long Fiber",120,-6,6,100,-4,4);
  Occ_map_S = book2DHistogram(HFDir, "Occ_map_S","Occ Map Short Fiber",120,-6,6,100,-4,4);

}

// ------------ method called once each job just after ending the event loop  ------------
void 
HFMon::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(HFMon);
