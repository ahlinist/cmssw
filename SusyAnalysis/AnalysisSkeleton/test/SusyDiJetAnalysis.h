#ifndef SUSYDIJETANALYSIS
#define SUSYDIJETANALYSIS



// System include files
#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

// ROOT includes
#include <TNtuple.h>

// Framework include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

// SUSY include files
#include "SusyAnalysis/EventSelector/interface/SusyEventSelector.h"
#include "SusyAnalysis/EventSelector/interface/SelectorSequence.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Framework/interface/TriggerNames.h"

#include "SusyAnalysis/AnalysisSkeleton/test/ALPGENParticleId.cc"

#include "PhysicsTools/Utilities/interface/deltaPhi.h"
#include "PhysicsTools/Utilities/interface/deltaR.h"
#include "DataFormats/PatCandidates/interface/Hemisphere.h"


//
// Class declaration
//
class SusyDiJetAnalysis : public edm::EDAnalyzer {
public:
  explicit SusyDiJetAnalysis(const edm::ParameterSet&);
  ~SusyDiJetAnalysis();
  
private:
  //*** CMSSW interface
  /// Called once per job, at start
  virtual void beginJob(const edm::EventSetup&) ;
  /// Called for each event
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  /// Called once per job, at end
  virtual void endJob();

  /// Print a summary of counts for all selectors
  virtual void printSummary(void);

  //*** Plotting
  /// Define all plots
  virtual void initPlots();
  /// Fill all plots for an event
  virtual void fillPlots( const edm::Event&, const SelectorDecisions& );

  virtual bool filter(const edm::Event& evt,const edm::EventSetup& iSetup );


private:

  // Selection
  SelectorSequence sequence_;              ///< Interface to selectors
  std::vector<std::string> plotSelection_; ///< Container for plotting selection
  std::vector<size_t> plotSelectionIndices_; ///< Selector indices for plotting selection

  // Event information
  double eventWeight_;  ///< Event weight from config. file (if <0, get it from event)
  edm::InputTag weightSource_; ///< Source for CSA07 event weight producer
  double weight_;       ///< Actual event weight (either config. or event)
  int    processId_;    ///< CSA07 generator process ID
  int run_, event_;


  // Counters
  unsigned int nrEventTotalRaw_;          ///< Raw number of events (+1 at each event)
  double nrEventTotalWeighted_;           ///< Weighted #(events)
  std::vector<float> nrEventSelected_;    ///< Selected #(events) for each module
  std::vector<float> nrEventAllButOne_;   ///< All-but-one selected #(events) for each module
  std::vector<float> nrEventCumulative_;  ///< Cumulative selected #(events) for each module

  // Plots
  TNtuple* ntuple_; // Will contain all the selector information we want to keep
  TTree * mAllData; // Will contain the additional di-jet specific data
  TTree * mSelectorData; // Will contain the information on the selector decisions

  int mTempTreeRun;
  int mTempTreeEvent;


  bool mTempTreeHLT1JET;
  bool mTempTreeHLT2JET;
  bool mTempTreeHLT1MET1HT;
  bool mTempTreeHLT1Muon;

 

  int    mTempTreeNjets;
 
  int   mTempTreeNJetsT[50];
  float mTempTreeJetsTPt[50];
  float mTempTreeJetsTEta[50];
  float mTempTreeJetsTPhi[50];
 
  double mTempTreeJetsEt[50];
  double mTempTreeJetsPt[50];
  double mTempTreeJetsPx[50];
  double mTempTreeJetsPy[50];
  double mTempTreeJetsPz[50];
  double mTempTreeJetsE[50];
  double mTempTreeJetsEta[50];
  double mTempTreeJetsPhi[50];
  double mTempTreeJetsFem[50];

  double mTempTreeJetsBTag_TkCountHighEff[50];
  double mTempTreeJetsBTag_SimpleSecVtx[50];
  double mTempTreeJetsBTag_CombSecVtx[50];

  double mTempTreeGenJetsEt[50];
  double mTempTreeGenJetsPt[50];
  double mTempTreeGenJetsE[50];
  double mTempTreeGenJetsPx[50];
  double mTempTreeGenJetsPy[50];
  double mTempTreeGenJetsPz[50];
  double mTempTreeGenJetsEta[50];
  double mTempTreeGenJetsPhi[50];

double mTempTreeMET;
  double mTempTreeMEX;
  double mTempTreeMEY;
  double mTempTreeMETphi;
  double mTempTreeMETeta;
  double mTempTreeSumET;
  double mTempTreeSumETSignif;
  double mTempTreeMETuncor;
  double mTempTreeMETphiuncor;
 

  int    mTempTreeNphot;
  double mTempTreePhotEt[50];
  double mTempTreePhotPt[50];
  double mTempTreePhotPx[50];
  double mTempTreePhotPy[50];
  double mTempTreePhotPz[50];
  double mTempTreePhotE[50];
  double mTempTreePhotEta[50];
  double mTempTreePhotPhi[50];
  double mTempTreePhotTrkIso[50];
  double mTempTreePhotECalIso[50];
  double mTempTreePhotHCalIso[50];
  double mTempTreePhotAllIso[50];

  int    mTempTreeNelec;
  double mTempTreeElecEt[50];
  double mTempTreeElecPt[50];
  double mTempTreeElecPx[50];
  double mTempTreeElecPy[50];
  double mTempTreeElecPz[50];
  double mTempTreeElecE[50];
  double mTempTreeElecEta[50];
  double mTempTreeElecPhi[50];
  double mTempTreeElecTrkIso[50];
  double mTempTreeElecECalIso[50];
  double mTempTreeElecHCalIso[50];
  double mTempTreeElecAllIso[50];
  double mTempTreeElecTrkChiNorm[50];


  double mTempTreeElecCharge[50];

  int    mTempTreeNmuon;
  double mTempTreeMuonEt[50];
  double mTempTreeMuonPt[50];
  double mTempTreeMuonPx[50];
  double mTempTreeMuonPy[50];
  double mTempTreeMuonPz[50];
  double mTempTreeMuonE[50];
  double mTempTreeMuonEta[50];
  double mTempTreeMuonPhi[50];
  double mTempTreeMuonTrkIso[50];
  double mTempTreeMuonECalIso[50];
  double mTempTreeMuonHCalIso[50];
  double mTempTreeMuonAllIso[50];
  double mTempTreeMuonTrkChiNorm[50];

  double mTempTreeMuonCharge[50];

  // markus 
  int mTempAlpIdTest;
  double mTempAlpPtScale;

  double mTempMuonPairMass;
  int mTempMuonPairIndex[2];

  int    mTempTreeNtau;
  double mTempTreeTauEt[50];
  double mTempTreeTauPt[50];
  double mTempTreeTauPx[50];
  double mTempTreeTauPy[50];
  double mTempTreeTauPz[50];
  double mTempTreeTauE[50];
  double mTempTreeTauEta[50];
  double mTempTreeTauPhi[50];
  double mTempTreeTauTrkIso[50];
  double mTempTreeTauECalIso[50];
  double mTempTreeTauHCalIso[50];
  double mTempTreeTauAllIso[50];


  int mTempTreeNhemispheres;
  double mTempTreeHemispheresEt[2];
  double mTempTreeHemispheresPt[2];
  double mTempTreeHemispheresPx[2];
  double mTempTreeHemispheresPy[2];
  double mTempTreeHemispheresPz[2];
  double mTempTreeHemispheresE[2];
  double mTempTreeHemispheresEta[2];
  double mTempTreeHemispheresPhi[2]; 


  bool is_ok;
  int mTempSimuCheck;

  int length;
  int ids[1000];
  int refs[1000];
  float genPt[1000];
  float genPhi[1000];
  float genEta[1000];
  int genStatus[1000];


  double mTempTreeEventWeight;
  int    mTempTreeProcID;
  double mTempTreePthat;
  int mGlobalDecision;

  // Data tags
  edm::InputTag triggerResults_; 
  std::vector<std::string> pathNames_;


  edm::InputTag jetTag_;
  edm::InputTag metTag_;
  edm::InputTag photTag_;
  edm::InputTag elecTag_;
  edm::InputTag muonTag_;
  edm::InputTag tauTag_;
  edm::InputTag genTag_;
  std::string outputFileName_;

  //input from .cfg
  bool theSoup;
  double fileWeight;

  double localPi;
  unsigned int *mSelectorResults;

};























#endif















