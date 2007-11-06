#ifndef ElectronIDFakeRateAnalyzer_h
#define ElectronIDFakeRateAnalyzer_h

#include <string>

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DQMServices/Core/interface/DaqMonitorBEInterface.h"
#include "DQMServices/Daemon/interface/MonitorDaemon.h"

#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectronFwd.h"


#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetfwd.h"
//#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"

#include "AnalysisDataFormats/Egamma/interface/ElectronID.h"
#include "AnalysisDataFormats/Egamma/interface/ElectronIDFwd.h"
#include "AnalysisDataFormats/Egamma/interface/ElectronIDAssociation.h"


#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include "PhysicsTools/Utilities/interface/EtComparator.h"


#include "TFile.h"
#include "TGraphAsymmErrors.h"
#include "TTree.h"
#include "TString.h"
#include "TH1D.h"
#include "TH2D.h"

class ElectronIDFakeRateAnalyzer : public edm::EDAnalyzer {

 public:
  explicit ElectronIDFakeRateAnalyzer(const edm::ParameterSet&);
  ~ElectronIDFakeRateAnalyzer();
  
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

 private:

  int nEvents;
  TFile*  rootFile_;
  TString datasetName;

  int tRecoElectrons,tSelElectrons,tRecoJets,tGenJets,tGenPartons1,tGenPartons2,tEvents;

  edm::InputTag MCTruthCollection_;
  edm::InputTag ElectronIDAssocMap_;
  edm::InputTag RecoCollection_;
  edm::InputTag RecoJetCollection_;
  edm::InputTag GenJetCollection_;

  TH1D* hRecoElecPT;
  TH1D* hSelElecPT;
  TH1D* hRecoJetsPT;
  TH1D* hGenJetsPT;
  TH1D* hGenPartons1PT;
  TH1D* hGenPartons2PT;
  TH1D* hEventsPT;

  TH1D* hRecoElecEta;
  TH1D* hSelElecEta;
  TH1D* hRecoJetsEta;
  TH1D* hGenJetsEta;
  TH1D* hGenPartons1Eta;
  TH1D* hGenPartons2Eta;
  TH1D* hEventsEta;

  TH1D* hStatistics;

  TH2D* hRecoElec;
  TH2D* hSelElec;
  TH2D* hRecoJets;
  TH2D* hGenJets;
  TH2D* hGenPartons1;
  TH2D* hGenPartons2;
  TH2D* hEvents;
  
  TH1D* hFakeNumEt;
  TH1D* hFakeNumSelEt;
  TH1D* hFakeDenEt;
  TH1D* hFakeNumEta;
  TH1D* hFakeNumSelEta;
  TH1D* hFakeDenEta;
  TH1D* hdRminElecJet;
  TH2D* hEtJetvsEtElectron;

  double PtCut;
  double EtaCut;
  double JetPtCut;
  double JetEtaCut;
  double dRElecJetCut;
  double JetEtCut;

  double histPT_min_Pt_;
  double histPT_max_Pt_;
  int histPT_bins_Pt_;

  double histEta_min_Eta_;
  double histEta_max_Eta_;
  int histEta_bins_Eta_;

  void loadCMSSWObjects(const edm::ParameterSet& iConfig);
  void loadHistoParameters(const edm::ParameterSet& iConfig);

  //GreaterByEt<reco::CaloJet> etComparator_;
  double getDeltaPhi (const reco::Candidate* candidate1,const reco::Candidate* candidate2); 
  double getDeltaR (const reco::Candidate* candidate1, const reco::Candidate* candidate2);

     
};


#endif

