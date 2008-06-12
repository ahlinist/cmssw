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
// $Id: HcalProm.h,v 1.11 2008/06/11 10:52:50 efe Exp $
//
//


// system include files
#include <memory>
#include <fstream>
#include <math.h>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
//TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

//geometry
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
//point
#include "DataFormats/GeometryVector/interface/GlobalPoint.h" 
//vector
#include "DataFormats/GeometryVector/interface/GlobalVector.h"

//HBHERecHits
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"

//trigger
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuRegionalCand.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTReadoutCollection.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GtPsbWord.h"

//propagation
#include "FastSimulation/BaseParticlePropagator/interface/BaseParticlePropagator.h"
#include "FastSimulation/CalorimeterProperties/interface/Calorimeter.h"
#include "FastSimulation/CaloGeometryTools/interface/CaloGeometryHelper.h"

class TH1F;
class TH2F;
class TStyle;
#include "TH1I.h"
#include "TMath.h"
#include "TString.h"
#include "TStyle.h"
#include "TCanvas.h"
//
// class decleration
//
class HcalProm : public edm::EDAnalyzer {
   public:
      explicit HcalProm(const edm::ParameterSet&);
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
      float deltaR(float eta0, float phi0, float eta, float phi);
      float getNxNClusterEnergy(int gridSize,int ieta, int iphi, const HBHERecHitCollection*  HBHERecHits);
      float get1xNClusterEnergy(int gridSize,int ieta, int iphi, const HBHERecHitCollection*  HBHERecHits);
      float getEtaCluster(int gridSize,int ieta, int iphi, const HBHERecHitCollection*  HBHERecHits);
      float getPhiCluster(int gridSize,int ieta, int iphi, const HBHERecHitCollection*  HBHERecHits);
      //muon extrapolation
      void getHBmax(
          // inputs
          int tphi, const HBHERecHitCollection*  HBHERecHits,
          // outputs
          int *ietaMax, int *iphiMax, double *Emax, double *E2max
          );

      bool Extrapolate(
          // inputs
	  double ox, double oy, double oz, double px, double py, double pz, 
	  double radius, double zwidth, double ImpPar,
	  // outputs
	  double *xp_out, double *yp_out, double *zp_out,
	  double *xm_out, double *ym_out, double *zm_out,
	  double *thetap_out, double *phip_out,
	  double *thetam_out, double *phim_out
	  );
      reco::Track bestTrack(const reco::TrackCollection&) const;
      HcalDetId getClosestCell(float dR,float eta, float phi, const HBHERecHitCollection*  HBHERecHits, const CaloGeometry* geo, bool &found);
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

      TH1F* h_AllTracks;
      TH1F* h_ct1;
      TH1F* h_ct2;
      TH1F* h_ct3;
      TH1F* h_ct4;
      TH1F* h_ct5;
      TH1F* h_ct6;
      TH1F* h_ct7;
      TH1F* h_ct8;
      TH1F* h_ct9;
      TH1F* h_NTracks1;
      TH1F* h_NTracks2;
      TH1F* h_NPTracks;
      TH1F* h_NMTracks;
      TH1F* h_NMuons;
      TH1F* h_Trigger;
      TH1F* h_Trigger2;
      TH1F* h_Pt;
      TH1F* h_Px;
      TH1F* h_Py;
      TH1F* h_d0;
      TH1F* h_Chi2;
      TH1F* h_Vtx_X;
      TH1F* h_Vtx_Y;
      TH1F* h_Vtx_Z;
      TH1F* h_Vtx_R1;
      TH1F* h_Vtx_R2;
      TH1F* h_diff;
      TH1F* h_InnerPos_X;
      TH1F* h_InnerPos_Y;
      TH1F* h_InnerPos_Z;
      TH1F* h_OuterPos_X;
      TH1F* h_OuterPos_Y;
      TH1F* h_OuterPos_Z;
      TH1F* h_Angle1;
      TH1F* h_Angle2;
      TH1F* h_Pt_Top;
      TH1F* h_Pt_Bottom;
      TH1F* h_Eta_Top;
      TH1F* h_Eta_Bottom;
      TH1F* h_Phi_Top;
      TH1F* h_Phi_Bottom;
      TH1F* h_dR_Top;
      TH1F* h_dR_Bottom;
      
      TH1F* h_HBP_e1x1;
      TH1F* h_HBP_e3x3;
      TH1F* h_HBP_e5x5;
      TH1F* h_HBM_e1x1;
      TH1F* h_HBM_e3x3;
      TH1F* h_HBM_e5x5;

      TH1F* h_HBP_e1[72];
      TH1F* h_HBP_e3[72];
      TH1F* h_HBP_e5[72];
      TH1F* h_HBM_e1[72];
      TH1F* h_HBM_e3[72];
      TH1F* h_HBM_e5[72];
      
      TH1F* h_HBTopMin_e1x1;
      TH1F* h_HBTopMin_e3x3cr;
      TH1F* h_HBTopMin_e1x3;
      TH1F* h_HBTopMin_e1x3cr;
      TH1F* h_HBTopMin_e5x5;

      TH1F* h_HBTopPlu_e1x1;
      TH1F* h_HBTopPlu_e3x3cr;
      TH1F* h_HBTopPlu_e1x3;
      TH1F* h_HBTopPlu_e1x3cr;
      TH1F* h_HBTopPlu_e5x5;
      
      TH1F* h_HBBottomMin_e1x1;
      TH1F* h_HBBottomMin_e3x3cr;
      TH1F* h_HBBottomMin_e1x3;
      TH1F* h_HBBottomMin_e1x3cr;
      TH1F* h_HBBottomMin_e5x5;
      
      TH1F* h_HBBottomPlu_e1x1;
      TH1F* h_HBBottomPlu_e3x3cr;
      TH1F* h_HBBottomPlu_e1x3;
      TH1F* h_HBBottomPlu_e1x3cr;
      TH1F* h_HBBottomPlu_e5x5;
      TH1F* h_corr;


      TH1F* PROJ_eta;
      TH1F* PROJ_phi;
      TH1F*  FIN_zdist;
      TH2F*  h_EtaPhi;
      
      TH1F* h_HB_maxE;
      TH1F* h_HB_maxE2;
      TH1F* h_HB_maxEta;
      TH1F* h_HB_maxPhi;
      TH1F* h_HB_maxIEta;
      TH1F* h_HB_maxIPhi;
      TH2F* h_HB_maxIEtaIPhi;
      TH1F* h_HB_IEta;
      TH1F* h_HB_IPhi;
      TH1F* h_HB_DEta;
      TH1F* h_HB_DPhi;
      TH1F* h_HBEnt_IPhi;
      TH1F* h_HBEnt_IEta;
      TH1F* h_HBExt_IPhi;
      TH1F* h_HBExt_IEta;
      TH2F* h_HBCorrEta;
      TH2F* h_HBCorrPhi;
      TH2F* h_HBCorrEta1;
      TH2F* h_HBCorrPhi1;
      TH2F* h_HBCorrEta2;
      TH2F* h_HBCorrPhi2;
      TH1F* h_Trk1_x;
      TH1F* h_Trk1_y;
      TH1F* h_Trk1_z;
      TH1F* h_Trk2_x;
      TH1F* h_Trk2_y;
      TH1F* h_Trk2_z;

      TH1F* h_InnerPos_x;
      TH1F* h_InnerPos_y;
      TH1F* h_InnerPos_z;
      TH1F* h_InnerPos_eta;
      TH1F* h_InnerPos_phi;
      
      TH1F* h_OuterPos_x;
      TH1F* h_OuterPos_y;
      TH1F* h_OuterPos_z;
      TH1F* h_OuterPos_eta;
      TH1F* h_OuterPos_phi;

      TH1F* h_distance2D;
      TH1F* h_distance3D;
      TH2F* h_distance2D_vs_HB;
      TH2F* h_distance3D_vs_HB;
      TH2F* h_distance2D_vs_nHits;
      TH2F* h_distance2D_vs_chi2;
      
      TH1F* h_muonNHits;

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

      CaloGeometryHelper *myCalorimeter_;
      int NDTT;  
      int NRPCT;  
      int NCSCT;  
      int NHcalT;  
      float fIPDistance;
      int fAlgoType; 
      double fIP_z;
      double fIP_r;
      int NTotal;
      int NAccepted;
      int NAccepted2;
      int top_ihpi_low;
      int top_ihpi_high;
      int bottom_ihpi_low;
      int bottom_ihpi_high;
};
