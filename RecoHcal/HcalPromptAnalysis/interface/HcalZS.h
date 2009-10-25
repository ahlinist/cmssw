// -*- C++ -*-
//
// Package:    HcalZS
// Class:      HcalZS
// 
/**\class HcalZS HcalZS.cc RecoHcal/HcalPromptAnalysis/src/HcalZS.cc

 SIMPLE EXAMPLE CODE demonstrating the access to officially reconstructed global run data.
 The config file is under the test directory.
 The config file should read the officially reconstructed root file in castor or 
 The output from Configuration/GlobalRuns/data/recoT0.cfg (official reco cfg). 

 Implementation:
     <Notes on implementation>
*/
//
// Original Author: Efe Yazgan
//         Created:  Wed Apr 16 10:03:18 CEST 2008
// $Id: HcalZS.h,v 1.6 2009/10/09 09:17:46 santanas Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

//TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"


//Mapping stuff
#include "DataFormats/HcalDetId/interface/HcalGenericDetId.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/HcalDetId/interface/HcalCalibDetId.h"
#include "DataFormats/HcalDetId/interface/HcalFrontEndId.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "CondFormats/HcalObjects/interface/HcalElectronicsMap.h"
#include "CondFormats/HcalObjects/interface/HcalLogicalMap.h"
#include "CalibCalorimetry/HcalAlgos/interface/HcalLogicalMapGenerator.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"

class TH1F;
class TH2F;

//
// class declaration
//

class HcalZS : public edm::EDAnalyzer {
public:
  explicit HcalZS(const edm::ParameterSet&);
  ~HcalZS();
  
private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  // ----------member data ---------------------------
  
  int thresholdHB_;
  int thresholdHE_;
  int thresholdHO_;
  int thresholdHOSiPM_;
  int thresholdHF_;
  int thresholdCalib_;

  int zs_mask[9]; //9 Sums of 2 TS

  edm::Service<TFileService> fs;

  TH1F* h_hbdigi_MarkAndPassEmu;
  TH1F* h_hedigi_MarkAndPassEmu;
  TH1F* h_hodigi_MarkAndPassEmu;
  TH1F* h_hfdigi_MarkAndPassEmu;
  TH1F* h_calib_MarkAndPassEmu;

  TH1F* h_hbdigi_MarkAndPassData;
  TH1F* h_hedigi_MarkAndPassData;
  TH1F* h_hodigi_MarkAndPassData;
  TH1F* h_hfdigi_MarkAndPassData;
  TH1F* h_calib_MarkAndPassData;

  TH1F* h_hbhe_US;
  TH1F* h_ho_US;
  TH1F* h_hf_US;
  TH1F* h_calib_US;

  TH2F* h_hbhf_d1;
  TH2F* h_hbhf_d2;
  TH2F* h_he_d3;
  TH2F* h_he_d2;
  TH2F* h_he_d1;
  TH2F* h_ho_d4;

  TH2F* h_hbhf_d1_all;
  TH2F* h_hbhf_d2_all;
  TH2F* h_he_d3_all;
  TH2F* h_he_d2_all;
  TH2F* h_he_d1_all;
  TH2F* h_ho_d4_all;

  TH1F* h_ZSagree;
  TH1F* h_ZSagree_calib;

  int NtotEvents;
  int NtotDigis;

  //------------------------------
  // HCAL logical map
  HcalLogicalMap *lMap;
  //------------------------------

};
