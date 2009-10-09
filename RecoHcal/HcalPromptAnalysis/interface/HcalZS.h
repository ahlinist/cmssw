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
// $Id: HcalZS.h,v 1.5 2009/08/20 07:26:17 santanas Exp $
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

  //   int hb_digiCounter[82][72];
  //   int he_digiCounter[82][72];
  //   int ho_digiCounter[82][72];
  //   int hf_digiCounter[82][72];

};
