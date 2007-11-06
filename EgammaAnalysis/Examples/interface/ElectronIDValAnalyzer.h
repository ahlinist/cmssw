#ifndef ElectronIDValAnalyzer_h
#define ElectronIDValAnalyzer_h

#include <string>

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DQMServices/Core/interface/DaqMonitorBEInterface.h"
#include "DQMServices/Daemon/interface/MonitorDaemon.h"

#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectronFwd.h"

#include "DataFormats/EgammaCandidates/interface/Photon.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include "EgammaAnalysis/Examples/interface/MCMatchingTools.h"

#include "TFile.h"
#include "TGraphAsymmErrors.h"
#include "TTree.h"
#include "TString.h"

class ElectronIDValAnalyzer : public edm::EDAnalyzer {
 public:
  explicit ElectronIDValAnalyzer(const edm::ParameterSet&);
  ~ElectronIDValAnalyzer();
  
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

 private:

  int nEvents;
  TFile*  rootFile_;

  TString datasetName;
  TTree* efficiencyInf;

  MCMatchingTools mcMatchingTools;
  
  int tmc,treco,tgood,tmatchedreco,tmatchedgood;

  Float_t EleRecEfficiency;
  Float_t EleIdEfficiency;
  Float_t EleEfficiency;

  Float_t EleRecPurity; 
  Float_t EleIdPurity; 

  edm::InputTag MCTruthCollection_;
  edm::InputTag ElectronIDAssocMap_;
  edm::InputTag RecoCollection_;

  TH1D* hMCElecPT;
  TH1D* hRECElecPT;
  TH1D* hSelElecPT;
  TH1D* hRECElecPTEne;
  TH1D* hSelElecPTEne;
  TH1D* hRECElecPTAllCharge;
  TH1D* hSelElecPTAllCharge;
  TH1D* hRECElecPTChargeMis;
  TH1D* hSelElecPTChargeMis;

  TH1D* hRECElecPT1;
  TH1D* hRECElecPT2;
  TH1D* hSelElecPT1;
  TH1D* hSelElecPT2;
 
  TH1D* hMCElecEta;
  TH1D* hRECElecEta;
  TH1D* hSelElecEta;
  TH1D* hRECElecEtaEne;
  TH1D* hSelElecEtaEne;
  TH1D* hRECElecEtaAllCharge;
  TH1D* hSelElecEtaAllCharge;
  TH1D* hRECElecEtaChargeMis;
  TH1D* hSelElecEtaChargeMis;
 
  TH1D* hRECElecEta1;
  TH1D* hRECElecEta2;
  TH1D* hSelElecEta1;
  TH1D* hSelElecEta2;

  TH1D* hMCElecPhi;
  TH1D* hRECElecPhi;
  TH1D* hSelElecPhi;
  TH1D* hRECElecPhiEne;
  TH1D* hSelElecPhiEne;
  TH1D* hRECElecPhiAllCharge;
  TH1D* hSelElecPhiAllCharge;
  TH1D* hRECElecPhiChargeMis;
  TH1D* hSelElecPhiChargeMis;

  TH1D* hRECElecPhi1;
  TH1D* hRECElecPhi2;
  TH1D* hSelElecPhi1;
  TH1D* hSelElecPhi2;
  
  TH1D* hDPByPMatch ;

  TH1D* hMCNElec;
  TH1D* hRECNElec;
  TH1D* hSelNElec;

  TH1D* hRECEffInclusive;
  TH1D* hSelEffInclusive;
  
  TGraphAsymmErrors* gEffPT;
  TGraphAsymmErrors* gEffIdPT;
  TGraphAsymmErrors* gEffPTEne;
  TGraphAsymmErrors* gEffIdPTEne;
  TGraphAsymmErrors* gEffEta;
  TGraphAsymmErrors* gEffIdEta;
  TGraphAsymmErrors* gEffEtaEne;
  TGraphAsymmErrors* gEffIdEtaEne;
  TGraphAsymmErrors* gEffPhi;
  TGraphAsymmErrors* gEffIdPhi;
  TGraphAsymmErrors* gEffPhiEne;
  TGraphAsymmErrors* gEffIdPhiEne;
  TGraphAsymmErrors* gEffRecIdPT;
  TGraphAsymmErrors* gEffRecIdEta;
  TGraphAsymmErrors* gEffRecIdPhi;
  TGraphAsymmErrors* gEffRecIdPTEne;
  TGraphAsymmErrors* gEffRecIdEtaEne;
  TGraphAsymmErrors* gEffRecIdPhiEne;
  TGraphAsymmErrors* gPurRecPT;
  TGraphAsymmErrors* gPurRecEta;
  TGraphAsymmErrors* gPurRecPhi;
  TGraphAsymmErrors* gPurIdPT;
  TGraphAsymmErrors* gPurIdEta;
  TGraphAsymmErrors* gPurIdPhi;
  TGraphAsymmErrors* gMisChargeRecPT;
  TGraphAsymmErrors* gMisChargeRecEta;
  TGraphAsymmErrors* gMisChargeRecPhi;
  TGraphAsymmErrors* gMisChargeIdPT;
  TGraphAsymmErrors* gMisChargeIdEta;
  TGraphAsymmErrors* gMisChargeIdPhi;
 
  double PtCut;
  double EtaCut;
  double maxdEoverECrack;
  double maxdEoverEBarrel;
  double maxdEoverEEndcap;

  double histEffPT_min_Pt_;
  double histEffPT_max_Pt_;
  int histEffPT_bins_Pt_;

  double histEffEta_min_Eta_;
  double histEffEta_max_Eta_;
  int histEffEta_bins_Eta_;

  double histEffPhi_min_Phi_;
  double histEffPhi_max_Phi_;
  int histEffPhi_bins_Phi_;

  int histNElec_bins_;
  double histNElec_min_ ;
  double histNElec_max_ ;

  int histEffEvent_bins_ ;
  double histEffEvent_min_ ;
  double histEffEvent_max_ ;

  int histPurEvent_bins_ ;
  double histPurEvent_min_;
  double histPurEvent_max_ ;

  int histEffInclusive_bins_ ;
  double histEffInclusive_min_;
  double histEffInclusive_max_ ;

  double hist_min_Et_;
  double hist_max_Et_;
  int    hist_bins_Et_;
    
  double hist_min_E_;
  double hist_max_E_;
  int    hist_bins_E_;
    
  double hist_min_Eta_;
  double hist_max_Eta_;
  int    hist_bins_Eta_;
    
  double hist_min_Phi_;
  double hist_max_Phi_;
  int    hist_bins_Phi_;
    
  double hist_min_EtOverTruth_;
  double hist_max_EtOverTruth_;
  int    hist_bins_EtOverTruth_;
    
  double hist_min_EOverTruth_;
  double hist_max_EOverTruth_;
  int    hist_bins_EOverTruth_;

  double hist_min_EtaOverTruth_;
  double hist_max_EtaOverTruth_;
  int    hist_bins_EtaOverTruth_;
    
  double hist_min_PhiOverTruth_;
  double hist_max_PhiOverTruth_;
  int    hist_bins_PhiOverTruth_;

  double hist_min_deltaEta_;
  double hist_max_deltaEta_;
  int    hist_bins_deltaEta_;
    
  double hist_min_deltaPhi_;
  double hist_max_deltaPhi_;
  int    hist_bins_deltaPhi_;
    
  double hist_min_recoMass_;
  double hist_max_recoMass_;
  int    hist_bins_recoMass_; 

  void loadCMSSWObjects(const edm::ParameterSet& iConfig);
  void loadHistoParameters(const edm::ParameterSet& iConfig);
     
};


#endif

