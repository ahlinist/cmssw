#ifndef SusyAnalyzer_h
#define SusyAnalyzer_h


// system include files
#include <memory>
#include <string>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
//#include "FWCore/Framework/interface/Handle.h"

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HLTReco/interface/TriggerEventWithRefs.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Framework/interface/TriggerNames.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMap.h"

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/JetReco/interface/JetTracksAssociation.h"
#include "DataFormats/BTauReco/interface/IsolatedTauTagInfo.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
//#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

// needed for cluster shape in 2_2_X
//#include "FWCore/Framework/interface/ESHandle.h"
//#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
//#include "Geometry/Records/interface/IdealGeometryRecord.h"
//#include "Geometry/Records/interface/CaloGeometryRecord.h"
//#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"
//

#include "AnalysisExamples/SusyAnalysis/interface/MrParticle.h"
#include "AnalysisExamples/SusyAnalysis/interface/MrEvent.h"
#include "AnalysisExamples/SusyAnalysis/interface/InputMcData.h"
#include "AnalysisExamples/SusyAnalysis/interface/MCProcessor.h"
#include "AnalysisExamples/SusyAnalysis/interface/RecoProcessor.h"
#include "AnalysisExamples/SusyAnalysis/interface/ShapeAnalyzer.h"
#include "AnalysisExamples/SusyAnalysis/interface/UserAnalysis.h"
#include "AnalysisExamples/SusyAnalysis/interface/EffProcessor.h"

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
  
class SusyAnalyzer : public edm::EDAnalyzer {
   public:
      explicit SusyAnalyzer(const edm::ParameterSet&);
      ~SusyAnalyzer();

   virtual void analyze(const edm::Event&, const edm::EventSetup&);
   virtual void beginJob(const edm::EventSetup& ) ;
   virtual void endJob() ;

   private:
  // ----------member data ---------------------------
     
  // names of modules, producing object collections
     string m_calotowers;
     string m_electronSrc;
     edm::InputTag m_muonSrc;
     string m_tracksSrc;
     string m_vertexSrc;
     string m_jetsSrc;
     //string m_jetsCorrectionL2RelativeService;
     //string m_jetsCorrectionL3AbsoluteService;
     string m_jetsCorrectionChainService;
     string m_jetsgenSrc;
     string m_tautaginfo;
     string m_photonSrc;
     string m_calometSrc;
     string m_bjettag;  // for b-tagging
     string m_tautag;  // for tau-tagging
     edm::InputTag m_hlTriggerResults;
     edm::InputTag m_clusterShapeBarrel;
     edm::InputTag m_clusterShapeEndcap;
     edm::InputTag m_reducedEndcapRecHitCollection;
     edm::InputTag m_reducedBarrelRecHitCollection;

  


  // Define the vectors holding pointers to MC and Reco data

     vector<MrParticle*> MCData; 
     vector<MrParticle*> RecoData;
     vector<MrParticle*> GenData; 
      
  // The pointer to the MrEvent data
     
     MrEvent * EventData;
  
  // names of parameter sets  
  
     edm::ParameterSet InputMC_params;
     edm::ParameterSet rejectEvent_params;
     edm::ParameterSet acceptance_cuts;
     edm::ParameterSet cleaner_params;
     edm::ParameterSet isolator_params;
     edm::ParameterSet objectmatch_params;
     edm::ParameterSet mcproc_params;
     edm::ParameterSet useranalysis_params;

     Config_t myConfig;

     bool useAODOnly;
     bool useFastSimulation;
     
  // Define all pointers to objects

     const TrackCollection * TrackData;
     const VertexCollection * VertexData;
     const CaloTowerCollection * CaloTowerData;     
     const BasicClusterShapeAssociationCollection * clusterShapeBarrelData; 
     const BasicClusterShapeAssociationCollection * clusterShapeEndcapData; 

     InputMcData * myInputMcData;
     MCProcessor * myMCProcessor;
     RecoProcessor * myRecoProcessor;
     EffProcessor * myEffProcessor;
     ShapeAnalyzer * myHemiAna;
     UserAnalysis * myUserAnalysis;
  
  int DEBUGLVL;
  
  // Define the event rejection parameters
  bool rej_NoTriggerData;
  bool rej_NoL1fired;
  bool rej_NoHLTfired;
  bool rej_BadHemis;

    // global counters

  //counters for L1
  std::vector<int> numTotL1BitsBeforeCuts;
  std::vector<std::string> l1Names;
  //counters for HLT
  std::vector<int> numTotHltBitsBeforeCuts;
  std::vector<std::string> hlNames;
    
  int numTotEvt;
  int numTotMCReject;
  int numTotEvtReject;
  int numTotEvtExceptCaught;
  int numTotEvtNoTrigger;
  int numTotEvtNoReco;
  int numTotEvtNoTracks;
  int numTotEvtNoCalo;
  int numTotEvtEmpty;
  int numTotEvtNoPrimary;
  int numTotEvtBadHardJet;
  int numTotEvtCleanEmpty;
  int numTotEvtFinalEmpty;
  int numTotEvtBadNoisy;
  int numTotEvtBadMET;
  int numTotEvtBadHemi;

  int numTotMCElec;
  int numTotMCMuon;
  int numTotMCTau;
  int numTotMCPhot;
  int numTotMCJet;
  
  int numTotMCElecBigEta;
  int numTotMCMuonBigEta;
  int numTotMCTauBigEta;
  int numTotMCPhotBigEta;
  int numTotMCJetBigEta;

  
  int numTotNotPrimaryTrk;
  int numTotNotClean;
  int numTotDuplicate;
  int numTotElectrons;
  int numTotElecNotPrimaryTrk;
  int numTotElecNotCleanHOE;
  int numTotElecNotCleanShsh;
  int numTotElecNotCleanTmat;
  int numTotElecDupl;
  int numTotElecDuplBadHOE;
  int numTotElecDuplBadShsh;
  int numTotElecDuplBadTmat;
  int numTotElectronsNonIso;  
  int numTotElectronsNonIsoBadHOE;  
  int numTotElectronsNonIsoBadShsh;  
  int numTotElectronsNonIsoBadTmat;  
  int numTotElectronsfinal;  
  int numTotElectronsfinalBadHOE;  
  int numTotElectronsfinalBadShsh;  
  int numTotElectronsfinalBadTmat;  
  int numTotElectronsMatched;  
  int numTotElectronsIsoMatched;
  int numTotElectronsMatchedBadHOE;
  int numTotElectronsMatchedBadShsh;
  int numTotElectronsMatchedBadTmat;
  int numTotMuons;  
  int numTotMuonNotPrimaryTrk;
  int numTotMuonNotClean;
  int numTotMuonDupl;
  int numTotMuonDuplBad;
  int numTotMuonsNonIso;  
  int numTotMuonsNonIsoBad;  
  int numTotMuonsfinal;  
  int numTotMuonsfinalBad;  
  int numTotMuonsMatched;  
  int numTotMuonsIsoMatched;
  int numTotMuonsMatchedBad;
  int numTotTaus;
  int numTotTauNotPrimaryTrk;
  int numTotTauNotClean;
  int numTotTauDupl;
  int numTotTauDuplBad;
  int numTotTausNonIso;
  int numTotTausNonIsoBad;
  int numTotTausfinal;
  int numTotTausfinalBad;
  int numTotTausMatched;  
  int numTotTausIsoMatched;
  int numTotTausMatchedBad;
  int numTotPhotons;  
  int numTotPhotNotPrimaryTrk;
  int numTotPhotNotCleanHOE;
  int numTotPhotNotCleanShsh;
  int numTotPhotDupl;
  int numTotPhotDuplBadHOE;
  int numTotPhotDuplBadShsh;
  int numTotPhotonsNonIso;
  int numTotPhotonsNonIsoBadHOE;
  int numTotPhotonsNonIsoBadShsh;
  int numTotPhotonsfinal;  
  int numTotPhotonsfinalBadHOE;  
  int numTotPhotonsfinalBadShsh;  
  int numTotPhotonsMatched;
  int numTotPhotonsMatchedBadHOE;
  int numTotPhotonsMatchedBadShsh;
  int numTotJets;  
  int numTotJetNotPrimaryTrk;
  int numTotJetNotCleanFem;
  int numTotJetNotCleanFtk;
  int numTotJetDupl;
  int numTotJetDuplBadFem;
  int numTotJetDuplBadFtk;
  int numTotBJets;  
  int numTotJetsfinal;  
  int numTotJetsfinalBadFem;  
  int numTotJetsfinalBadFtk;  
  int numTotBJetsfinal;  
  int numTotJetsMatched;
  int numTotJetsMatchedBadFem;
  int numTotJetsMatchedBadFtk;
  int numTotUfos;
  int numTotUfosNotPrimaryTrk;
  int numTotUfosNotClean;
  int numTotUfosDupl;
  int numTotUfosDuplBad;
  int numTotUfosNonIso;
  int numTotUfosNonIsoBad;
  int numTotUfosfinal;
  int numTotUfosfinalBad;
  int numTotUfosMatched;
  int numTotUfosMatchedBad;
  
  int numTotEventsAfterCuts;

  int firstsusymother[2];
    
  TH1I *hEventStats; //17 bins
  TH1I *hElecMuonStats;//10 bins
  TH1I *hPhotonJetStats;//10 bins
  TH1I *hMiscObjStats; // 78 bins
 
  // private methods
  virtual void PrintTitle(void);
  virtual void PrintEvtRej(void);
  virtual void PrintAccCuts(void);
  virtual void PrintExtrapCuts(void);
  virtual void PrintMCCuts(void);
  virtual void PrintCleanerCuts(void);
  virtual void PrintIsolatorCuts(void);
  virtual void PrintObjectMatchingCuts(void);
  virtual bool AcceptTrigger();  // could become a class once implemented
  virtual void PrintStatistics(void);  
  virtual void SetHistoWithStats(void);
  virtual void CleanMemory();

};


#endif
