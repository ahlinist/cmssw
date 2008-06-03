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
// Updated        :  Taylan Yetkin (2008/05/08)
//         Created:  Wed Apr 16 10:03:18 CEST 2008
// $Id: HcalProm.h,v 1.9 2008/06/02 20:13:42 fedor Exp $
//
//


// system include files
#include <memory>
#include <fstream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

//TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

class TH1F;
class TH2F;
class TStyle;

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
      
      //adding HTML output functions (very similar to DQM) 
      void htmlOutput(void);
      void CaloTowerHTMLOutput(std::string startTime, std::string htmlDir, std::string htmlName);
      void DigiHTMLOutput(std::string startTime, std::string htmlDir, std::string htmlName);
      void RecHitHTMLOutput(std::string startTime, std::string htmlDir, std::string htmlName);
      void JetMetHTMLOutput(std::string startTime, std::string htmlDir, std::string htmlName);
      void MuonHTMLOutput(std::string startTime, std::string htmlDir, std::string htmlName);
      void HPDNoiseHTMLOutput(std::string startTime, std::string htmlDir, std::string htmlName);
      void histoHTML(TH1F * hist, const char *xlab, const char *ylab, int width, std::ofstream & htmlFile, std::string htmlDir);
      void histoHTML2(TH2F * hist, const char *xlab, const char *ylab, int width, std::ofstream & htmlFile, std::string htmlDir, bool color = false);
      std::string getIMG(TH1F* hist, int size, std::string htmlDir, const char* xlab, const char* ylab);
      std::string getIMG2(TH2F* hist, int size, std::string htmlDir, const char* xlab, const char* ylab,bool color);
      void parseString(std::string& title);
      void cleanString(std::string& title);

  //------- histogram bookers ----------
  void bookHistograms ();
  TH1F* book1DHistogram (TFileDirectory& fDir, const std::string& fName, const std::string& fTitle, 
			 int fNbins, double fXmin, double fXmax) const;
  TH2F* book2DHistogram (TFileDirectory& fDir, const std::string& fName, const std::string& fTitle, 
			 int fNbinsX, double fXmin, double fXmax,
			 int fNbinsY, double fYmin, double fYmax) const;

      // ----------member data ---------------------------
      edm::Service<TFileService> fs;

      TH1F* h_global_trigger_bit;

      TH1F* h_hbhe_rechit_energy;
      TH1F* h_maxhbherec;
      TH1F* h_maxhbMinusrec;
      TH1F* h_maxhbPlusrec;
      TH2F* h_hbhe_eta_phi;	
      TH1F* h_hf_rechit_energy;
      TH2F* h_hf_eta_phi;
      TH1F* h_ho_rechit_energy;
      TH2F* h_ho_eta_phi;

      TH1F* h_eb_rechit_energy;
      TH1F* h_maxebeerec;
      TH1F* h_maxhorec;

      TH1F* h_maxebee_plus_maxhbhe;

      TH1F* h_hbtiming;
      TH1F* h_ecal_cluster_energy;
      TH1F* h_ecal_cluster_eta;
      TH1F* h_ecal_cluster_phi;
      TH2F* h_ecal_vs_hcal_X;
      TH2F* h_ecal_vs_hcal_Y; 
      
      TH1F* h_calo_tower_energy;

      TH1F* h_Qiesum;
      TH2F* h_NoiseChan[9];
      TH1F* h_NoisePhi[9];
	      

  
      TH1F *h_jet_Pt;
      TH1F *h_jet_Eta;
      TH1F *h_jet_Phi;
      
      TH1F *h_leadJet_Pt;
      TH1F *h_leadJet_Eta;
      TH1F *h_leadJet_Phi;
      
      TH1F *h_caloMet_Met;
      TH1F *h_caloMet_Phi;
      TH1F *h_caloMet_SumEt;
      TH1F *h_HT;
      TH1F *h_MHT;

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
      int trigDT;
      std::string baseHtmlDir_;
      long runBegin,lumibegin,lumiend,evtNo;
      std::string startTime;
      bool prompt_htmlPrint;
      bool doDigiHTML;
      bool doCaloTowerHTML;
      bool doRecHitHTML;
      bool doJetMetHTML;
      bool doMuonHTML;
      bool doHPDNoiseHTML;
};
