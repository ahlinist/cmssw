// -*- C++ -*-
//
// Package:    HcalPFGBeamSplashTemplate
// Class:      HcalPFGBeamSplashTemplate
// 
// \class HcalPFGBeamSplashTemplate HcalPFGBeamSplashTemplate.cc /RecoHcal/HcalPromptAnalysis/src/HcalPFGBeamSplashTemplate.cc
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

//TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

class TH1F;
class TH2F;

//
// class decleration
//

class HcalPFGBeamSplashTemplate : public edm::EDAnalyzer {
   public:
      explicit HcalPFGBeamSplashTemplate(const edm::ParameterSet&);
      ~HcalPFGBeamSplashTemplate();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      // ----------member data ---------------------------
  edm::Service<TFileService> fs;

  TH1F* h_hcal_rechit_energy_hbhe;
  TH1F* h_hcal_rechit_energy_ho;
  TH1F* h_hcal_rechit_energy_hf;
  
  TH1F* h_hcal_rechit_energy_hbhe_splash;
  TH1F* h_hcal_rechit_energy_ho_splash;
  TH1F* h_hcal_rechit_energy_hf_splash;

  TH1F* h_hcal_rechit_energy_hbhe_aftersplash;
  TH1F* h_hcal_rechit_energy_ho_aftersplash;
  TH1F* h_hcal_rechit_energy_hf_aftersplash;

  TH2F* h2_hcal_rechit_energy_eta_phi_noho_splash;
  TH2F* h2_hcal_rechit_energy_eta_phi_ho_splash;
  TH2F* h2_hcal_rechit_energy_eta_phi_noho_aftersplash;
  TH2F* h2_hcal_rechit_energy_eta_phi_ho_aftersplash;

  TH2F* h2_hcal_rechit_time_eta_phi_noho_splash;
  TH2F* h2_hcal_rechit_time_eta_phi_ho_splash;
  TH2F* h2_hcal_rechit_time_eta_phi_noho_aftersplash;
  TH2F* h2_hcal_rechit_time_eta_phi_ho_aftersplash;

  TH1F* h_hcal_SumE_hb;
  TH1F* h_hcal_SumE_he;
  TH1F* h_hcal_SumE_ho;
  TH1F* h_hcal_SumE_hf;
  TH1F* h_hcal_SumE_tot;

  TH1F* h_hcal_NhitAboveThr_hb;
  TH1F* h_hcal_NhitAboveThr_he;
  TH1F* h_hcal_NhitAboveThr_ho;
  TH1F* h_hcal_NhitAboveThr_hf;
  TH1F* h_hcal_NhitAboveThr_tot;

  TH1F* h_ecal_rechit_energy_eb;
  TH1F* h_ecal_rechit_energy_ee;

  TH1F* h_ecal_rechit_energy_eb_splash;
  TH1F* h_ecal_rechit_energy_ee_splash;

  TH1F* h_ecal_rechit_energy_eb_aftersplash;
  TH1F* h_ecal_rechit_energy_ee_aftersplash;

  TH1F* h_ecal_SumE_eb;
  TH1F* h_ecal_SumE_ee;
  TH1F* h_ecal_SumE_etot;

  TH1F* h_caloMet_Met;
  TH1F* h_caloMet_Phi;
  TH1F* h_caloMet_SumEt;

  TH1F* h_l1_gtbit;
  TH1F* h_l1_gtbit_techtrig;

  TH1F* h_event_bx;
  TH1F* h_event_bx_splash;
  TH1F* h_event_bx_aftersplash;

  TH2F* h2_HCAL_vs_ECAL_splash;
  TH2F* h2_HB_vs_EB_splash;
  TH2F* h2_HE_vs_EE_splash;

  TH2F* h2_HCAL_vs_ECAL_aftersplash;

};
