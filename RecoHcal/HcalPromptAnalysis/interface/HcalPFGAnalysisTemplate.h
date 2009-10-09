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

//TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

class TH1F;
class TH2F;

//
// class decleration
//

class HcalPFGAnalysisTemplate : public edm::EDAnalyzer {
   public:
      explicit HcalPFGAnalysisTemplate(const edm::ParameterSet&);
      ~HcalPFGAnalysisTemplate();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      // ----------member data ---------------------------
  edm::Service<TFileService> fs;

  TH1F* h_hcal_rechit_energy;
  //TH1F* h_hbtiming;

  TH1F* h_ecal_rechit_energy;

  //   TH1F* h_ecal_cluster_energy;
  //   TH1F* h_ecal_cluster_eta;
  //   TH1F* h_ecal_cluster_phi;
  //   TH2F* h_ecal_vs_hcal_X;
  //   TH2F* h_ecal_vs_hcal_Y; 

  TH1F* h_calo_tower_energy;

  TH1F* h_jet_energy;
  TH1F* h_jet_eta;
  TH1F* h_jet_phi;

  TH1F* h_muon_vertex_x;
  TH1F* h_muon_px;
  TH1F* h_muon_p;

  //   TH2F* h_ecalx_vs_muonx;
  //   TH1F* h_impact_diff;

  TH1F* h_caloMet_Met;
  TH1F* h_caloMet_Phi;
  TH1F* h_caloMet_SumEt;

  TH1F* h_l1_gtbit;
  TH1F* h_l1_gtbit_techtrig;

  int TrigDT;
};
