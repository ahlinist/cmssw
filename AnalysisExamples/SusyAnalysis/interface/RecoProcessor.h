#ifndef RecoProcessor_h
#define RecoProcessor_h


/*  \class RecoProcessor
*
*  Class which does the cleaning, isolation, final cuts and matching 
*  of MrParticle objects
*
*  Authors: Luc Pape & Filip Moortgat      Date: October 2006
*                                          Updated: 
*
*/


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

#include "AnalysisExamples/SusyAnalysis/interface/SusyRecoTools.h"

#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include <cmath>

class RecoProcessor : public SusyRecoTools {

public:

// Constructor:
RecoProcessor(MrEvent *);
RecoProcessor(MrEvent *, 
//    const edm::ParameterSet&);
    Config_t *);

// Destructor:
virtual ~RecoProcessor(){};

// Methods:
virtual bool RecoDriver(void);
void SetDebug(int debug)  {DEBUGLVL  = debug; } 

  // counters of events
virtual int NumEvtNoReco(void) {return numEvtNoReco; }
virtual int NumEvtNoTracks(void) {return numEvtNoTracks; }
virtual int NumEvtNoCalo(void) {return numEvtNoCalo; }
virtual int NumEvtEmpty(void) {return numEvtEmpty; }
virtual int NumEvtNoPrimary(void) {return numEvtNoPrimary; }
virtual int NumEvtBadHardJet(void) {return numEvtBadHardJet; }
virtual int NumEvtCleanEmpty(void) {return numEvtCleanEmpty; }
virtual int NumEvtFinalEmpty(void) {return numEvtFinalEmpty; }
virtual int NumEvtBadNoisy(void) {return numEvtBadNoisy; }
virtual int NumEvtBadMET(void) {return numEvtBadMET; }

  // counters of objects
virtual int NumNotPrimaryTrk(void) {return numNotPrimaryTrk; }
virtual int NumNotClean(void) {return numNotClean; }
virtual int NumDuplicate(void) {return numDuplicate; }
virtual int NumElectrons(void) {return numElectrons; }
virtual int NumElecNotPrimaryTrk(void) {return numElecNotPrimaryTrk; }
virtual int NumElecNotClean(void) {return numElecNotClean; }
virtual int NumElecDupl(void) {return numElecDupl; }
virtual int NumElectronsNonIso(void) {return numElectronsNonIso; }
virtual int NumElectronsfinal(void) {return numElectronsfinal; }
virtual int NumElectronsMatched(void) {return numElectronsMatched; }
virtual int NumMuons(void) {return numMuons; }
virtual int NumMuonNotPrimaryTrk(void) {return numMuonNotPrimaryTrk; }
virtual int NumMuonNotClean(void) {return numMuonNotClean; }
virtual int NumMuonDupl(void) {return numMuonDupl; }
virtual int NumMuonsNonIso(void) {return numMuonsNonIso; }
virtual int NumMuonsfinal(void) {return numMuonsfinal; }
virtual int NumMuonsMatched(void) {return numMuonsMatched; }
virtual int NumTaus(void) {return numTaus; }
virtual int NumTauNotPrimaryTrk(void) {return numTauNotPrimaryTrk; }
virtual int NumTauNotClean(void) {return numTauNotClean; }
virtual int NumTauDupl(void) {return numTauDupl; }
virtual int NumTausNonIso(void) {return numTausNonIso; }
virtual int NumTausfinal(void) {return numTausfinal; }
virtual int NumTausMatched(void) {return numTausMatched; }
virtual int NumPhotons(void) {return numPhotons; }
virtual int NumPhotNotPrimaryTrk(void) {return numPhotNotPrimaryTrk; }
virtual int NumPhotNotClean(void) {return numPhotNotClean; }
virtual int NumPhotDupl(void) {return numPhotDupl; }
virtual int NumPhotonsNonIso(void) {return numPhotonsNonIso; }
virtual int NumPhotonsfinal(void) {return numPhotonsfinal; }
virtual int NumPhotonsMatched(void) {return numPhotonsMatched; }
virtual int NumJets(void) {return numJets; }
virtual int NumJetNotPrimaryTrk(void) {return numJetNotPrimaryTrk; }
virtual int NumJetNotClean(void) {return numJetNotClean; }
virtual int NumJetDupl(void) {return numJetDupl; }
virtual int NumBJets(void) {return numBJets; }
virtual int NumJetsfinal(void) {return numJetsfinal; }
virtual int NumBJetsfinal(void) {return numBJetsfinal; }
virtual int NumJetsMatched(void) {return numJetsMatched; }

virtual int NumAllObjects(void) {return counter; }

virtual math::XYZVector PmissRecoil(void) {return metRecoilvector;}

private:
     
   // names of parameter sets  
//  edm::ParameterSet& iConfig;
  Config_t * myConfig;
  edm::ParameterSet rejectEvent_params;
  edm::ParameterSet acceptance_cuts;
  edm::ParameterSet cleaner_params;
  edm::ParameterSet isolator_params;
  edm::ParameterSet objectmatch_params;
  
//  MrEvent * EventData;
//  std::vector<MrParticle*> & RecoData;
  std::vector<MrParticle*> & MCData;
//  const TrackCollection * TrackData;
//  const VertexCollection* VertexData;
//  const CaloTowerCollection* CaloTowerData;
  
  
  // Define all pointers to objects
     
  ObjectCleaner * myCleaner;
  Isolator * myIsolator;
  MatchObjects* myMatcher;    

//  int DEBUGLVL;

  
  // Define the event rejection parameters
  bool rej_MissingRecoData;
  bool rej_MissingTrackData;
  bool rej_MissingCaloTowers;
  bool rej_Empty;
  bool rej_NoPrimary;
  bool rej_BadHardJet;
  bool rej_CleanEmpty;
  bool rej_FinalEmpty;
  bool rej_BadNoisy;
  bool rej_BadMET;
  
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
  float ana_minBtagDiscriminator;
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
  // Define the parameters for ObjectCleaner
  float clean_distVxmax;
  int clean_methodTksInJetVx;
  int clean_nJetVxTkHitsmin;
  float clean_JetVxTkPtmin;
  float clean_jetVxCaloTowEFracmin;
  float clean_dRTrkFromJetVx;
  float clean_rejEvtBadJetPtmin;
  
  // Define the counters per event
  // counters of events
  int numEvtNoReco;
  int numEvtNoTracks;
  int numEvtNoCalo;
  int numEvtEmpty;
  int numEvtNoPrimary;
  int numEvtBadHardJet;
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

};

#endif

