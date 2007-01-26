#ifndef RecoProcessor_h
#define RecoProcessor_h


/*  \class PrepareData
*
*  Class which does the cleaning, isolation, final cuts and matching 
*  of MrParticle objects
*
*  Authors: Luc Pape & Filip Moortgat      Date: October 2006
*                                          Updated: 
*
*/


#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include <cmath>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "AnalysisExamples/SusyAnalysis/interface/MrEvent.h" 
#include "AnalysisExamples/SusyAnalysis/interface/MrParticle.h" 
#include "AnalysisExamples/SusyAnalysis/interface/ObjectCleaner.h"
#include "AnalysisExamples/SusyAnalysis/interface/Isolator.h"
#include "AnalysisExamples/SusyAnalysis/interface/MatchObjects.h"

class RecoProcessor {

public:

// Constructor:
RecoProcessor(MrEvent *, const TrackCollection *, 
    const VertexCollection*, const CaloTowerCollection*);
RecoProcessor(MrEvent *, const TrackCollection *, 
    const VertexCollection*, const CaloTowerCollection*, 
//    const edm::ParameterSet&);
    Config_t *);

// Destructor:
~RecoProcessor(){};

// Methods:
bool RecoDriver(void);
void SetDebug(int debug)  {DEBUGLVL  = debug; } 

  // counters of events
int NumEvtNoReco(void) {return numEvtNoReco; }
int NumEvtNoTracks(void) {return numEvtNoTracks; }
int NumEvtNoCalo(void) {return numEvtNoCalo; }
int NumEvtEmpty(void) {return numEvtEmpty; }
int NumEvtNoPrimary(void) {return numEvtNoPrimary; }
int NumEvtCleanEmpty(void) {return numEvtCleanEmpty; }
int NumEvtFinalEmpty(void) {return numEvtFinalEmpty; }
int NumEvtBadNoisy(void) {return numEvtBadNoisy; }
int NumEvtBadMET(void) {return numEvtBadMET; }

  // counters of objects
int NumNotPrimaryTrk(void) {return numNotPrimaryTrk; }
int NumNotClean(void) {return numNotClean; }
int NumDuplicate(void) {return numDuplicate; }
int NumElectrons(void) {return numElectrons; }
int NumElecNotPrimaryTrk(void) {return numElecNotPrimaryTrk; }
int NumElecNotClean(void) {return numElecNotClean; }
int NumElecDupl(void) {return numElecDupl; }
int NumElectronsNonIso(void) {return numElectronsNonIso; }
int NumElectronsfinal(void) {return numElectronsfinal; }
int NumElectronsMatched(void) {return numElectronsMatched; }
int NumMuons(void) {return numMuons; }
int NumMuonNotPrimaryTrk(void) {return numMuonNotPrimaryTrk; }
int NumMuonNotClean(void) {return numMuonNotClean; }
int NumMuonDupl(void) {return numMuonDupl; }
int NumMuonsNonIso(void) {return numMuonsNonIso; }
int NumMuonsfinal(void) {return numMuonsfinal; }
int NumMuonsMatched(void) {return numMuonsMatched; }
int NumTaus(void) {return numTaus; }
int NumTauNotPrimaryTrk(void) {return numTauNotPrimaryTrk; }
int NumTauNotClean(void) {return numTauNotClean; }
int NumTauDupl(void) {return numTauDupl; }
int NumTausNonIso(void) {return numTausNonIso; }
int NumTausfinal(void) {return numTausfinal; }
int NumTausMatched(void) {return numTausMatched; }
int NumPhotons(void) {return numPhotons; }
int NumPhotNotPrimaryTrk(void) {return numPhotNotPrimaryTrk; }
int NumPhotNotClean(void) {return numPhotNotClean; }
int NumPhotDupl(void) {return numPhotDupl; }
int NumPhotonsNonIso(void) {return numPhotonsNonIso; }
int NumPhotonsfinal(void) {return numPhotonsfinal; }
int NumPhotonsMatched(void) {return numPhotonsMatched; }
int NumJets(void) {return numJets; }
int NumJetNotPrimaryTrk(void) {return numJetNotPrimaryTrk; }
int NumJetNotClean(void) {return numJetNotClean; }
int NumJetDupl(void) {return numJetDupl; }
int NumBJets(void) {return numBJets; }
int NumJetsfinal(void) {return numJetsfinal; }
int NumBJetsfinal(void) {return numBJetsfinal; }
int NumJetsMatched(void) {return numJetsMatched; }

int NumAllObjects(void) {return counter; }

math::XYZVector PmissRecoil(void) {return metRecoilvector;}

private:
     
   // names of parameter sets  
//  edm::ParameterSet& iConfig;
  Config_t * myConfig;
  edm::ParameterSet acceptance_cuts;
  edm::ParameterSet cleaner_params;
  edm::ParameterSet isolator_params;
  edm::ParameterSet objectmatch_params;
  
  MrEvent * myEventData;
  std::vector<MrParticle*> & RecoData;
  std::vector<MrParticle*> & MCData;
  const TrackCollection * TrackData;
  const VertexCollection* VertexData;
  const CaloTowerCollection* CaloTowerData;
  
  
  // Define all pointers to objects
     
  ObjectCleaner * myCleaner;
  Isolator * myIsolator;
  MatchObjects* myMatcher;    

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
  // Define the parameters for extrapolation error adjustment
  float reco_elecD0ErrorThresh;
  float reco_elecDzErrorThresh;
  float reco_muonD0ErrorThresh;
  float reco_muonDzErrorThresh;
  float reco_jetD0ErrorThresh;
  float reco_jetDzErrorThresh;
  
  // Define the counters per event
  // counters of events
  int numEvtNoReco;
  int numEvtNoTracks;
  int numEvtNoCalo;
  int numEvtEmpty;
  int numEvtNoPrimary;
  int numEvtCleanEmpty;
  int numEvtFinalEmpty;
  int numEvtBadNoisy;
  int numEvtBadMET;
  
  // Define the counters of objects
  int numNotPrimaryTrk;
  int numNotClean;
  int numDuplicate;
  int numElectrons;
  int numElecNotPrimaryTrk;
  int numElecNotClean;
  int numElecDupl;
  int numElectronsNonIso;
  int numElectronsfinal;
  int numElectronsMatched;
  int numMuons;
  int numMuonNotPrimaryTrk;
  int numMuonNotClean;
  int numMuonDupl;
  int numMuonsNonIso;
  int numMuonsfinal;
  int numMuonsMatched;
  int numTaus;
  int numTauNotPrimaryTrk;
  int numTauNotClean;
  int numTauDupl;
  int numTausNonIso;
  int numTausfinal;
  int numTausMatched;
  int numPhotons;
  int numPhotNotPrimaryTrk;
  int numPhotNotClean;
  int numPhotDupl;
  int numPhotonsNonIso;
  int numPhotonsfinal;
  int numPhotonsMatched;
  int numJets;
  int numJetNotPrimaryTrk;
  int numJetNotClean;
  int numJetDupl;
  int numBJets;
  int numJetsfinal;
  int numBJetsfinal;
  int numJetsMatched;
  int counter;
  
  // Define MET vectors 
  math::XYZVector metRecoilvector;

  // Define the private methods included
  void PrintRecoInfo(void);
  bool GetJetVx(int);
  void GetJetTrks(float, float, float, vector<int>*);
  void AddToJet(int);
  int FindNearestJet(int);
  float GetPtwrtJet(int, int);
  float GetJetTrkPtsum(float, float, float);
  float DeltaPhi(float, float);
  float GetDeltaR(float, float, float, float);

};

#endif

