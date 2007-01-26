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
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetfwd.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/BTauReco/interface/JetTracksAssociation.h"
#include "DataFormats/BTauReco/interface/IsolatedTauTagInfo.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"

#include "AnalysisExamples/SusyAnalysis/interface/MrParticle.h"
#include "AnalysisExamples/SusyAnalysis/interface/MrEvent.h"
#include "AnalysisExamples/SusyAnalysis/interface/MCProcessor.h"
#include "AnalysisExamples/SusyAnalysis/interface/RecoProcessor.h"
#include "AnalysisExamples/SusyAnalysis/interface/ShapeAnalyzer.h"
#include "AnalysisExamples/SusyAnalysis/interface/UserAnalysis.h"

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
     string fOutputFileName ;
  // names of modules, producing object collections
     string m_calotowers;
     string m_electronSrc;
     string m_muonSrc;
     string m_tracksSrc;
     string m_vertexSrc;
     string m_jetsSrc;
     string m_jetsgenSrc;
     string m_tautaginfo;
     string m_photonSrc;
     string m_calometSrc;

  // names of histogram output file
     TFile*      hOutputFile ;


  // Define the vectors holding pointers to MC and Reco data

     vector<MrParticle*> MCData; 
     vector<MrParticle*> RecoData;
  
  // The pointer to the MrEvent data
     
     MrEvent * myEventData;
  
  // names of parameter sets  
     
     edm::ParameterSet acceptance_cuts;
     edm::ParameterSet cleaner_params;
     edm::ParameterSet isolator_params;
     edm::ParameterSet objectmatch_params;
     edm::ParameterSet mcproc_params;

     Config_t myConfig;
     
  // Define all pointers to objects

     const TrackCollection * TrackData;
     const VertexCollection * VertexData;
     const CaloTowerCollection * CaloTowerData;
     MCProcessor * myMCProcessor;
     RecoProcessor * myRecoProcessor;
     ShapeAnalyzer * myHemiAna;
     UserAnalysis * myUserAnalysis;
  
  int DEBUGLVL;

  // Define the acceptance cuts
  float ana_elecEtaMax;
  float ana_elecPtMin1;
  float ana_muonEtaMax;
  float ana_muonPtMin1;
  float ana_tauEtaMax;
  float ana_tauPtMin1;
  float ana_photonEtaMax;
  float ana_photonPtMin1;
  float ana_jetEtaMax;
  float ana_jetPtMin1;
  float ana_elecPtMin2;
  float ana_muonPtMin2;
  float ana_tauPtMin2;
  float ana_photonPtMin2;
  float ana_jetPtMin2;

    // global counters
    
  int numTotEvt;
  int numTotEvtExceptCaught;
  int numTotEvtNoTrigger;
  int numTotEvtNoReco;
  int numTotEvtNoTracks;
  int numTotEvtNoCalo;
  int numTotEvtEmpty;
  int numTotEvtNoPrimary;
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
  
  int numTotNotPrimaryTrk;
  int numTotNotClean;
  int numTotDuplicate;
  int numTotElectrons;
  int numTotElecNotPrimaryTrk;
  int numTotElecNotClean;
  int numTotElecDupl;
  int numTotElectronsNonIso;  
  int numTotElectronsfinal;  
  int numTotElectronsMatched;
  int numTotMuons;  
  int numTotMuonNotPrimaryTrk;
  int numTotMuonNotClean;
  int numTotMuonDupl;
  int numTotMuonsNonIso;  
  int numTotMuonsfinal;  
  int numTotMuonsMatched;
  int numTotTaus;
  int numTotTauNotPrimaryTrk;
  int numTotTauNotClean;
  int numTotTauDupl;
  int numTotTausNonIso;
  int numTotTausfinal;
  int numTotTausMatched;
  int numTotPhotons;  
  int numTotPhotNotPrimaryTrk;
  int numTotPhotNotClean;
  int numTotPhotDupl;
  int numTotPhotonsNonIso;  
  int numTotPhotonsfinal;  
  int numTotPhotonsMatched;
  int numTotJets;  
  int numTotJetNotPrimaryTrk;
  int numTotJetNotClean;
  int numTotJetDupl;
  int numTotBJets;  
  int numTotJetsfinal;  
  int numTotBJetsfinal;  
  int numTotJetsMatched;

  int firstsusymother[2];
    

  // private methods
  void PrintCuts(void);
  bool AcceptTrigger();  // could become a class once implemented
  void PrintStatistics(void);
  void CleanMemory();
  // some methods to scan through the MCData and compute kinematical quantities
  int FindProducedSusyParticles(int*);
  int FindTopSusyMother(int);
  int FindLowSusyMother(int);
  int FirstSMParton(int);
  bool ComesFromSquark(int);
  bool ComesFromGluino(int);
  float DeltaPhi(float, float);
  float GetDeltaR(float, float, float, float);

};

#endif
