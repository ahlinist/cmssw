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

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

//#include "DataFormats/L1Trigger/interface/L1ParticleMap.h"
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
  //  virtual void printSummary(void);
  // Print an HLT trigger report
  virtual void printHLTreport(void); // georgia
 
  //*** Plotting
  /// Define all plots
  virtual void initPlots();
  /// Fill all plots for an event
  // virtual void fillPlots( const edm::Event&, const SelectorDecisions& );

  //  virtual bool filter(const edm::Event& evt,const edm::EventSetup& iSetup );


private:

  bool matchJetsByCaloTowers( const pat::Jet&, const pat::Jet& );
  
  // Selection
  // SelectorSequence sequence_;              ///< Interface to selectors
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

  // GEORGIA
  int nHLT; int HLTArray[200];

  int mTempTreenVtx;
  double mTempTreeVtxChi2[5];
  double mTempTreeVtxNdof[5];
  double mTempTreeVtxNormalizedChi2[5];
  double mTempTreeVtxX[5]; double mTempTreeVtxY[5]; double mTempTreeVtxZ[5];
  double mTempTreeVtxdX[5]; double mTempTreeVtxdY[5]; double mTempTreeVtxdZ[5];
  // end GEORGIA



  int    mTempTreeNjets;
  double mTempTreeJetsEt[50];
  double mTempTreeJetsPt[50];
  double mTempTreeJetsPx[50];
  double mTempTreeJetsPy[50];
  double mTempTreeJetsPz[50];
  double mTempTreeJetsE[50];
  double mTempTreeJetsEta[50];
  double mTempTreeJetsPhi[50];
  double mTempTreeJetsFem[50];
  int mTempTreeJetPartonFlavour[50];
  int mTempTreeJetsHemi[50];

  // track info:
  int mTempTreeJetTrackNo[50];
  double mTempTreeJetTrackPhi[50];
  double mTempTreeJetTrackPhiWeighted[50];
  double mTempTreeJetTrackPt[50];


  double mTempTreeJetMCCorrFactor[50];
  double mTempTreeJetJPTCorrFactor[50];
  
  double mTempTreeccJetMCCorrFactor[50];
  double mTempTreeccJetJPTCorrFactor[50];

  bool mTempTreeccJetAssoc[50];
  double  mTempTreeccJetAssoc_E[50];
  double  mTempTreeccJetAssoc_px[50];
  double  mTempTreeccJetAssoc_py[50];
  double  mTempTreeccJetAssoc_pz[50];


  int mTempTreeJetPartonId[50];
  int mTempTreeJetPartonMother[50];
  double mTempTreeJetPartonPx[50];
  double mTempTreeJetPartonPy[50];
  double mTempTreeJetPartonPz[50];
  double mTempTreeJetPartonEt[50];
  double mTempTreeJetPartonEnergy[50];
  double mTempTreeJetPartonPhi[50];
  double mTempTreeJetPartonEta[50];

 

  float mTempTreeJetsBTag_TkCountHighEff[50];
  float mTempTreeJetsBTag_SimpleSecVtx[50];
  float mTempTreeJetsBTag_CombSecVtx[50];

  double mTempTreeGenJetsEt[50];
  double mTempTreeGenJetsPt[50];
  double mTempTreeGenJetsE[50];
  double mTempTreeGenJetsPx[50];
  double mTempTreeGenJetsPy[50];
  double mTempTreeGenJetsPz[50];
  double mTempTreeGenJetsEta[50];
  double mTempTreeGenJetsPhi[50];

  

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

  bool mTempTreeccPhotAssoc[50];
  bool mTempTreePhotLooseEM[50];
  bool mTempTreePhotLoosePhoton[50];
  bool mTempTreePhotTightPhoton[50];

  double mTempTreeGenPhotPdgId[50];
  double mTempTreeGenPhotMother[50];
  double mTempTreeGenPhotPx[50];
  double mTempTreeGenPhotPy[50];
  double mTempTreeGenPhotPz[50];

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
  //MICHELE
  double mTempTreeElecIdLoose[50];
  double mTempTreeElecIdTight[50];
  double mTempTreeElecIdRobLoose[50];
  double mTempTreeElecIdRobTight[50];
  double mTempTreeElecChargeMode[50];
  double mTempTreeElecPtTrkMode[50];
  double mTempTreeElecQOverPErrTrkMode[50];
  double mTempTreeGenElecPdgId[50];
  double mTempTreeGenElecMother[50];
  double mTempTreeGenElecPx[50];
  double mTempTreeGenElecPy[50];
  double mTempTreeGenElecPz[50];
  double mTempTreeElecCaloEnergy[50];
  double mTempTreeElecHOverE[50];
  double mTempTreeElecVx[50];
  double mTempTreeElecVy[50];
  double mTempTreeElecVz[50];
  double mTempTreeElecD0[50];
  double mTempTreeElecDz[50];
  double mTempTreeElecPtTrk[50];
  double mTempTreeElecQOverPErrTrk[50];
  double mTempTreeElecLostHits[50];
  double mTempTreeElecValidHits[50];
  double mTempTreeElecNCluster[50];
  double mTempTreeElecEtaTrk[50];
  double mTempTreeElecPhiTrk[50];
  double mTempTreeElecWidthClusterEta[50];
  double mTempTreeElecWidthClusterPhi[50];
  double mTempTreeElecPinTrk[50];
  double mTempTreeElecPoutTrk[50];
  double mTempTreeElecNormChi2[50];
  bool mTempTreeccElecAssoc[50];

  double mTempTreeElecECalIsoDeposit[50];
  double mTempTreeElecHCalIsoDeposit[50];

 

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
  bool mTempTreeMuonIsGlobal[50];
  bool mTempTreeMuonIsStandAlone[50];
  bool mTempTreeMuonIsTracker[50]; 
  bool mTempTreeMuonIsGlobalTight[50];
  bool mTempTreeMuonIsTMLastStationLoose[50];
  bool mTempTreeMuonTMLastStationTight[50];
  bool mTempTreeMuonTM2DCompatibilityLoose[50];
  bool mTempTreeMuonTM2DCompatibilityTight[50];
  bool mTempTreeccMuonAssoc[50];

 double mTempTreeMuonECalIsoDeposit[50];
 double mTempTreeMuonHCalIsoDeposit[50];
 
 double mTempTreeMuonCombChi2[50];
 double mTempTreeMuonCombNdof[50];
 double mTempTreeMuonTrkD0[50];

  //MICHELE
  double  mTempTreeMuonId[50];
  double mTempTreeMuonCombVx[50];
  double mTempTreeMuonCombVy[50];
  double mTempTreeMuonCombVz[50];
  double mTempTreeMuonCombD0[50];
  double mTempTreeMuonCombDz[50];

  double mTempTreeMuonStandValidHits[50];
  double mTempTreeMuonStandLostHits[50];
  double mTempTreeMuonStandPt[50];
  double mTempTreeMuonStandPz[50];
  double mTempTreeMuonStandP[50];
  double mTempTreeMuonStandEta[50];
  double mTempTreeMuonStandPhi[50];
  double mTempTreeMuonStandChi[50];
  double mTempTreeMuonStandCharge[50];
  double mTempTreeMuonStandQOverPError[50];

  double mTempTreeMuonTrkValidHits[50];
  double mTempTreeMuonTrkLostHits[50];
  double mTempTreeMuonTrkPt[50];
  double mTempTreeMuonTrkPz[50];
  double mTempTreeMuonTrkP[50];
  double mTempTreeMuonTrkEta[50];
  double mTempTreeMuonTrkPhi[50];
  double mTempTreeMuonTrkChi[50];
  double mTempTreeMuonTrkCharge[50];
  double mTempTreeMuonTrkQOverPError[50];
  double mTempTreeMuonTrkOuterZ[50];
  double mTempTreeMuonTrkOuterR[50];


  double mTempTreeGenMuonPdgId[50];
  double mTempTreeGenMuonMother[50];
  double mTempTreeGenMuonPx[50];
  double mTempTreeGenMuonPy[50];
  double mTempTreeGenMuonPz[50];
  //cc  
  //PIOPPI



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
  double mTempTreeTauChrg[50];
  double mTempTreeTauTrkIso[50];
  double mTempTreeTauECalIso[50];
  double mTempTreeTauHCalIso[50];
  double mTempTreeTauAllIso[50];
  //MICHELE
  double mTempTreeTauVx[50];
  double mTempTreeTauVy[50];
  double mTempTreeTauVz[50];
  double mTempTreeTauNTks[50];
  double mTempTreeTauNIsoTks[50];
  double mTempTreeTauIsoTkPt[50];
  double mTempTreeTauNNeutrals[50];
  double mTempTreeTauTk1Vx[50];
  double mTempTreeTauTk1Vy[50];
  double mTempTreeTauTk1Vz[50];
  double mTempTreeTauTk1D0[50];
  double mTempTreeTauTk1Dz[50];
  double mTempTreeTauTk1Pt[50]; 
  double mTempTreeTauTk1Pz[50];
  double mTempTreeTauTk1Eta[50];
  double mTempTreeTauTk1Phi[50];
  double mTempTreeTauTk1Chi[50];
  double mTempTreeTauTk1Charge[50];
  double mTempTreeTauTk1QOverPError[50];
  double mTempTreeTauTk1ValidHits[50];
  double mTempTreeTauTk1LostHits[50];
  double mTempTreeTauTk1CaloE[50];
  //TK2
  double mTempTreeTauTk2Vx[50];
  double mTempTreeTauTk2Vy[50];
  double mTempTreeTauTk2Vz[50];
  double mTempTreeTauTk2D0[50];
  double mTempTreeTauTk2Dz[50];
  double mTempTreeTauTk2Pt[50]; 
  double mTempTreeTauTk2Pz[50];
  double mTempTreeTauTk2Eta[50];
  double mTempTreeTauTk2Phi[50];
  double mTempTreeTauTk2Chi[50];
  double mTempTreeTauTk2Charge[50];
  double mTempTreeTauTk2QOverPError[50];
  double mTempTreeTauTk2ValidHits[50];
  double mTempTreeTauTk2LostHits[50];
  double mTempTreeTauTk2CaloE[50];
  //TK3
  double mTempTreeTauTk3Vx[50];
  double mTempTreeTauTk3Vy[50];
  double mTempTreeTauTk3Vz[50];
  double mTempTreeTauTk3D0[50];
  double mTempTreeTauTk3Dz[50];
  double mTempTreeTauTk3Pt[50]; 
  double mTempTreeTauTk3Pz[50];
  double mTempTreeTauTk3Eta[50];
  double mTempTreeTauTk3Phi[50];
  double mTempTreeTauTk3Chi[50];
  double mTempTreeTauTk3Charge[50];
  double mTempTreeTauTk3QOverPError[50];
  double mTempTreeTauTk3ValidHits[50];
  double mTempTreeTauTk3LostHits[50];
  double mTempTreeTauTk3CaloE[50];
  //NEUTRAL
  double mTempTreeTauNeutralE[50];
  double mTempTreeTauNeutralHOverHPlusE[50];
  double mTempTreeGenTauPdgId[50];
  double mTempTreeGenTauMother[50];
  double mTempTreeGenTauPx[50];
  double mTempTreeGenTauPy[50];
  double mTempTreeGenTauPz[50];
  //PIOPPI


  //benedetta
  //PF jets
  int    mTempTreeNPFjet;
  double mTempTreePFjetEta[50];
  double mTempTreePFjetPhi[50];
  double mTempTreePFjetE[50];
  double mTempTreePFjetPx[50];
  double mTempTreePFjetPy[50];
  double mTempTreePFjetPz[50];
  double mTempTreePFjetPt[50];
  double mTempTreePFjetCharge[50];

  // Stoye  
  // Generated MET
  double mTempTreeMET_Gen[3];
  // Stoye
 
  // Do the MET save for non cc MET
  int nFullMET;
  double mTempTreeMET_Fullcorr_nocc[3];
  double mTempTreeMET_Fullcorr_nocc_significance;
  double mTempTreeMET_Nocorr_nocc[2];
  double mTempTreeMET_Muoncorr_nocc[2];
  double mTempTreeMET_JECcorr_nocc[2];

  // Do the MET save for cc MET
  int nUncorrMET;
  double mTempTreeMET_Fullcorr_cc[3];
  double mTempTreeMET_Nocorr_cc[2];
  double mTempTreeMET_Muoncorr_cc[2];
  double mTempTreeMET_JECcorr_cc[2];



  int mTempTreeNhemispheres;
  double mTempTreeHemispheresEt[2];
  double mTempTreeHemispheresPt[2];
  double mTempTreeHemispheresPx[2];
  double mTempTreeHemispheresPy[2];
  double mTempTreeHemispheresPz[2];
  double mTempTreeHemispheresE[2];
  double mTempTreeHemispheresEta[2];
  double mTempTreeHemispheresPhi[2]; 



  double mTempTreeMPTPhi;
  double mTempTreeMPTPx;
  double mTempTreeMPTPy;
  double mTempTreeMPTPz;


  //  bool is_ok;
  // int mTempSimuCheck;

  int length;
  int ids[1000];
  int refs[1000];
  float genE[1000];
  float genPx[1000];
  float genPy[1000];
  float genPz[1000];
  int genStatus[1000];

  int genLepLength;
  int genLepIds[100];
  int genLepRefs[100];
  float genLepE[100];
  float genLepPx[100];
  float genLepPy[100];
  float genLepPz[100];
  int genLepStatus[100];

  //benedetta
  int genTauLength;
  int genTauIds[50];
  int genTauStatus[50];
  int genTauRefs[50];
  float genTauE[50];
  float genTauPx[50];
  float genTauPy[50];
  float genTauPz[50];

  int genTauDauCharged[50];
  int genTauDauLeptonic[50];
  int genTauDauNeutral[50];
  int genTauDauNeutrinos[50];
  int genTauDauLeptonId[50];
  float genTauDauEnergyLeptonic[50];
  float genTauDauEnergyCharged[50];
  float genTauDauEnergyNeutral[50];
  float genTauDauEnergyNeutrinos[50];
  float genTauDauPxLeptonic[50];
  float genTauDauPxCharged[50];
  float genTauDauPxNeutral[50];
  float genTauDauPxNeutrinos[50];
  float genTauDauPyLeptonic[50];
  float genTauDauPyCharged[50];
  float genTauDauPyNeutral[50];
  float genTauDauPyNeutrinos[50];
  float genTauDauPzLeptonic[50];
  float genTauDauPzCharged[50];
  float genTauDauPzNeutral[50];
  float genTauDauPzNeutrinos[50];
  int genTauDauCh1Id[50];
  float genTauDauCh1Px[50];
  float genTauDauCh1Py[50];
  float genTauDauCh1Pz[50];
  int genTauDauCh2Id[50];
  float genTauDauCh2Px[50];
  float genTauDauCh2Py[50];
  float genTauDauCh2Pz[50];
  int genTauDauCh3Id[50];
  float genTauDauCh3Px[50];
  float genTauDauCh3Py[50];
  float genTauDauCh3Pz[50];

  //end benedetta

  double mTempTreeEventWeight;
  int    mTempTreeProcID;
  double mTempTreePthat;
  int mGlobalDecision;

  // Data tags
  edm::InputTag triggerResults_; 
  std::vector<std::string> pathNames_;

  edm::TriggerNames triggerNames_;  // TriggerNames class

  // GEORGIA (15.01)
  unsigned int  nEvents_;           // number of events processed

  unsigned int  nWasRun_;           // # where at least one HLT was run
  unsigned int  nAccept_;           // # of accepted events
  unsigned int  nErrors_;           // # where at least one HLT had error
  std::vector<unsigned int> hlWasRun_; // # where HLT[i] was run
  std::vector<unsigned int> hlAccept_; // # of events accepted by HLT[i]
  std::vector<unsigned int> hlErrors_; // # of events with error in HLT[i]
  bool init_;                          // vectors initialised or not

  edm::InputTag vtxTag_;
  // end GEORGIA

  edm::InputTag jetTag_;
  edm::InputTag metTag_;
  edm::InputTag photTag_;
  edm::InputTag elecTag_;
  edm::InputTag muonTag_;
  edm::InputTag tauTag_;
  edm::InputTag genTag_;
  //benedetta: PFjets
  edm::InputTag pfjetTag_;
  bool usePfjets_;
  edm::InputTag jptTag_;
  edm::InputTag ccJptTag_;

  edm::InputTag ccjetTag_;
  edm::InputTag ccmetTag_;
  edm::InputTag ccelecTag_;
  edm::InputTag ccmuonTag_;
  edm::InputTag ccphotTag_;


  std::string outputFileName_;

  //input from .cfg
  bool theSoup;
  double fileWeight;

  double localPi;
  unsigned int *mSelectorResults;

};























#endif















