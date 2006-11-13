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
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "DataFormats/JetReco/interface/GenJetfwd.h"
#include "DataFormats/BTauReco/interface/JetTracksAssociation.h"
#include "DataFormats/BTauReco/interface/IsolatedTauTagInfo.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "AnalysisExamples/SusyAnalysis/interface/MrParticle.h"
#include "AnalysisExamples/SusyAnalysis/interface/ObjectCleaner.h"
#include "AnalysisExamples/SusyAnalysis/interface/Isolator.h"
#include "AnalysisExamples/SusyAnalysis/interface/MatchObjects.h"
#include "AnalysisExamples/SusyAnalysis/interface/Hemisphere.h"
#include "AnalysisExamples/SusyAnalysis/interface/MatchHemis.h"

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
     
   // names of parameter sets  
     edm::ParameterSet acceptance_cuts;
     edm::ParameterSet cleaner_params;
     edm::ParameterSet isolator_params;
     edm::ParameterSet objectmatch_params;
     
  // names of histograms
     TFile*      hOutputFile ;
//     TH1D*       hJetEt;
     TH1D*       hJetMultclean;
     TH1D*       hElecMultclean;
     TH1D*       hLeptonMult;
     TH1D*       hElectronMult;
     TH1D*       hMuonMult;
     TH1D*       hPhotonMult;
     TH1D*       hJetMult;
     TH1D*       hBJetMult;
     TH1D*       hPtElec;
     TH1D*       hPtMuon;
     TH1D*       hPtJet1;
     TH1D*       hPtJet2;
     TH1D*       hPtJet3;
     TH1D*       hMissingET;
     TH1D*       hEtSum;
     TH1D*       hHT;
     TH1D*       hJetGoodEt;
     TH1D*       hJetWrongEt;

  // Define the vectors holding pointers to MC and Reco data

     vector<MrParticle*> MCData; 	 
     vector<MrParticle*> RecoData;
     // vectors used for hemisphere reconstruction
     vector<float> vPx, vPy, vPz, vE; 
     vector<float> vA1, vA2;
     vector<int> vgroups;
  
  // Define all pointers to objects
     
     ObjectCleaner * myCleaner;
     Isolator * myIsolator;
     MatchObjects* myMatcher;    
     Hemisphere * myHemi;
     MatchHemis * myHemiMatcher;

  // acceptance cut variables
    double ana_elecEtaMax; 
    double ana_elecPtMin1; 
   
    double ana_muonEtaMax; 
    double ana_muonPtMin1; 
   
    double ana_tauEtaMax; 
    double ana_tauPtMin1; 
   
    double ana_photonEtaMax; 
    double ana_photonPtMin1; 
   
    double ana_jetEtaMax; 
    double ana_jetPtMin1;

    double ana_elecPtMin2;
    double ana_muonPtMin2;
    double ana_tauPtMin2;
    double ana_photonPtMin2; 
    double ana_jetPtMin2;


    // global counters
    
  int numTotEvt;
  int numTotEvtNoTrigger;
  int numTotEvtBadNoisy;
  int numTotEvtBadMET;
  int numTotEvtBadHemi;

  int numTotElectrons;  
  int numTotElectronsNonIso;  
  int numTotElectronsfinal;  
  int numTotElectronsMatched;
  int numTotMuons;  
  int numTotMuonsNonIso;  
  int numTotMuonsfinal;  
  int numTotMuonsMatched;
  int numTotTaus;
  int numTotTausNonIso;
  int numTotTausfinal;
  int numTotTausMatched;
  int numTotPhotons;  
  int numTotPhotonsNonIso;  
  int numTotPhotonsfinal;  
  int numTotPhotonsMatched;
  int numTotJets;  
  int numTotBJets;  
  int numTotJetsfinal;  
  int numTotBJetsfinal;  
  int numTotJetsMatched;
    

    // private methods
  void PrintCuts(void);
  void PrintMCInfo(int);
  void PrintMCStatusInfo(void);
  void PrintRecoInfo(void);
  void PrintStatistics(void);
  bool AcceptTrigger();  // could become a class once implemented
  void CleanMemory();
  // some methods to scan through the MCData and compute kinematical quantities
  int FindProducedSusyParticles(int*);
  int FindTopSusyMother(int);
  int FindLowSusyMother(int);
  int FirstSMParton(int);
  bool ComesFromSquark(int);
  bool ComesFromGluino(int);
  void AddToJet(int);
  int FindNearestJet(int);
  float MetFromMC(void);
  float DeltaPhi(float, float);
  float GetDeltaR(float, float, float, float);
  
  int DEBUGLVL;
  
};

#endif
