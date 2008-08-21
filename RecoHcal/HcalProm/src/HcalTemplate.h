// -*- C++ -*-
//
// Package:    HcalTemplate
// Class:      HcalTemplate
// 
/**\class HcalTemplate HcalTemplate.cc RecoHcal/HcalProm/src/HcalTemplate.cc

 SIMPLE EXAMPLE CODE demonstrating the access to officially reconstructed global run data.
 The config file is under the test directory.
 The config file should read the officially reconstructed root file in castor or 
 The output from Configuration/GlobalRuns/data/recoT0.cfg (official reco cfg). 

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Efe Yazgan
//         Created:  Wed Apr 16 10:03:18 CEST 2008
// $Id: HcalTemplate.h,v 1.1 2008/04/30 20:57:31 fedor Exp $
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

class HcalTemplate : public edm::EDAnalyzer {
   public:
      explicit HcalTemplate(const edm::ParameterSet&);
      ~HcalTemplate();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      // ----------member data ---------------------------
  edm::Service<TFileService> fs;

  TH1F* h_hcal_rechit_energy;
  TH2F* h_eta_phi_HBHE;	

  TH1F* h_ecal_rechit_energy;


  TH1F* h_hbtiming;

  TH1F* h_ecal_cluster_energy;
  TH1F* h_ecal_cluster_eta;
  TH1F* h_ecal_cluster_phi;
  TH2F* h_ecal_vs_hcal_X;
  TH2F* h_ecal_vs_hcal_Y; 

  TH1F* h_calo_tower_energy;

  TH1F* h_jet_energy;
  TH1F* h_jet_eta;
  TH1F* h_jet_phi;

  TH1F* h_muon_vertex_x;
  TH1F* h_muon_px;
  TH1F* h_muon_p;

  TH2F* h_ecalx_vs_muonx;
  TH1F* h_impact_diff;

  TH1F* h_caloMet_Met;
  TH1F* h_caloMet_Phi;
  TH1F* h_caloMet_SumEt;

  int TrigDT;
};
