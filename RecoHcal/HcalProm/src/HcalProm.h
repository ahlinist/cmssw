// -*- C++ -*-
//
// Package:    HcalProm
// Class:      HcalProm
// 
/**\class HcalProm HcalProm.cc RecoHcal/HcalProm/src/HcalProm.cc

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
// $Id: HcalProm.h,v 1.2 2008/05/04 23:11:28 fedor Exp $
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
class HcalProm : public edm::EDAnalyzer {
   public:
      explicit HcalProm(const edm::ParameterSet&);
      void Extrapolate(
              double ox,
              double oy,
              double oz,
              double px,
              double py,
              double pz,
              double radius,
              double *thetap_out,
              double *phip_out,
              double *thetam_out,
              double *phim_out
              );
      ~HcalProm();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      // ----------member data ---------------------------
  edm::Service<TFileService> fs;

  TH1F* h_hcal_rechit_energy;
  TH1F* h_maxhbherec;
  TH1F* h_caloMet_energy;
  TH2F* h_eta_phi_HBHE;	
  TH1F* h_hf_rechit_energy;
  TH2F* h_eta_phi_HF;
  TH1F* h_ho_rechit_energy;
  TH2F* h_eta_phi_HO;

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
  TH1F* h_jet_highestPt;

  TH1F* h_muon_vertex_x;
  TH1F* h_muon_px;
  TH1F* h_muon_p;

  TH2F* h_ecalx_vs_muonx;
  TH2F* h_ecaly_vs_muony;
  TH2F* h_jetphi_vs_muonphi;
  TH1F* h_impact_diff;

  TH2F*  DT_HCAL_eta_correlation;
  TH2F*  DT_HCAL_eta_correlation_all;
  TH2F*  DT_HCAL_phi_correlation;
  TH2F*  DT_HCAL_phi_correlation_all;

  TH1F*  HCAL_energy_correlation;
  TH1F*  HCAL_energy_correlation_all;



  int TrigDT;
};
