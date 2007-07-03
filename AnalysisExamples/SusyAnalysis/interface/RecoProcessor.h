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
virtual int NumElecNotCleanHOE(void) {return numElecNotCleanHOE; }
virtual int NumElecNotCleanShsh(void) {return numElecNotCleanShsh; }
virtual int NumElecNotCleanTmat(void) {return numElecNotCleanTmat; }
virtual int NumElecDupl(void) {return numElecDupl; }
virtual int NumElecDuplBadHOE(void) {return numElecDuplBadHOE; }
virtual int NumElecDuplBadShsh(void) {return numElecDuplBadShsh; }
virtual int NumElecDuplBadTmat(void) {return numElecDuplBadTmat; }
virtual int NumElectronsNonIso(void) {return numElectronsNonIso; }
virtual int NumElectronsNonIsoBadHOE(void) {return numElectronsNonIsoBadHOE; }
virtual int NumElectronsNonIsoBadShsh(void) {return numElectronsNonIsoBadShsh; }
virtual int NumElectronsNonIsoBadTmat(void) {return numElectronsNonIsoBadTmat; }
virtual int NumElectronsfinal(void) {return numElectronsfinal; }
virtual int NumElectronsfinalBadHOE(void) {return numElectronsfinalBadHOE; }
virtual int NumElectronsfinalBadShsh(void) {return numElectronsfinalBadShsh; }
virtual int NumElectronsfinalBadTmat(void) {return numElectronsfinalBadTmat; }
virtual int NumElectronsMatched(void) {return numElectronsMatched; }
virtual int NumElectronsMatchedBadHOE(void) {return numElectronsMatchedBadHOE; }
virtual int NumElectronsMatchedBadShsh(void) {return numElectronsMatchedBadShsh; }
virtual int NumElectronsMatchedBadTmat(void) {return numElectronsMatchedBadTmat; }
virtual int NumMuons(void) {return numMuons; }
virtual int NumMuonNotPrimaryTrk(void) {return numMuonNotPrimaryTrk; }
virtual int NumMuonNotClean(void) {return numMuonNotClean; }
virtual int NumMuonDupl(void) {return numMuonDupl; }
virtual int NumMuonDuplBad(void) {return numMuonDuplBad; }
virtual int NumMuonsNonIso(void) {return numMuonsNonIso; }
virtual int NumMuonsNonIsoBad(void) {return numMuonsNonIsoBad; }
virtual int NumMuonsfinal(void) {return numMuonsfinal; }
virtual int NumMuonsfinalBad(void) {return numMuonsfinalBad; }
virtual int NumMuonsMatched(void) {return numMuonsMatched; }
virtual int NumMuonsMatchedBad(void) {return numMuonsMatchedBad; }
virtual int NumTaus(void) {return numTaus; }
virtual int NumTauNotPrimaryTrk(void) {return numTauNotPrimaryTrk; }
virtual int NumTauNotClean(void) {return numTauNotClean; }
virtual int NumTauDupl(void) {return numTauDupl; }
virtual int NumTauDuplBad(void) {return numTauDuplBad; }
virtual int NumTausNonIso(void) {return numTausNonIso; }
virtual int NumTausNonIsoBad(void) {return numTausNonIsoBad; }
virtual int NumTausfinal(void) {return numTausfinal; }
virtual int NumTausfinalBad(void) {return numTausfinalBad; }
virtual int NumTausMatched(void) {return numTausMatched; }
virtual int NumTausMatchedBad(void) {return numTausMatchedBad; }
virtual int NumPhotons(void) {return numPhotons; }
virtual int NumPhotNotPrimaryTrk(void) {return numPhotNotPrimaryTrk; }
virtual int NumPhotNotCleanHOE(void) {return numPhotNotCleanHOE; }
virtual int NumPhotNotCleanShsh(void) {return numPhotNotCleanShsh; }
virtual int NumPhotDupl(void) {return numPhotDupl; }
virtual int NumPhotDuplBadHOE(void) {return numPhotDuplBadHOE; }
virtual int NumPhotDuplBadShsh(void) {return numPhotDuplBadShsh; }
virtual int NumPhotonsNonIso(void) {return numPhotonsNonIso; }
virtual int NumPhotonsNonIsoBadHOE(void) {return numPhotonsNonIsoBadHOE; }
virtual int NumPhotonsNonIsoBadShsh(void) {return numPhotonsNonIsoBadShsh; }
virtual int NumPhotonsfinal(void) {return numPhotonsfinal; }
virtual int NumPhotonsfinalBadHOE(void) {return numPhotonsfinalBadHOE; }
virtual int NumPhotonsfinalBadShsh(void) {return numPhotonsfinalBadShsh; }
virtual int NumPhotonsMatched(void) {return numPhotonsMatched; }
virtual int NumPhotonsMatchedBadHOE(void) {return numPhotonsMatchedBadHOE; }
virtual int NumPhotonsMatchedBadShsh(void) {return numPhotonsMatchedBadShsh; }
virtual int NumJets(void) {return numJets; }
virtual int NumJetNotPrimaryTrk(void) {return numJetNotPrimaryTrk; }
virtual int NumJetNotCleanFem(void) {return numJetNotCleanFem; }
virtual int NumJetNotCleanFtk(void) {return numJetNotCleanFtk; }
virtual int NumJetDupl(void) {return numJetDupl; }
virtual int NumJetDuplBadFem(void) {return numJetDuplBadFem; }
virtual int NumJetDuplBadFtk(void) {return numJetDuplBadFtk; }
virtual int NumBJets(void) {return numBJets; }
virtual int NumJetsfinal(void) {return numJetsfinal; }
virtual int NumJetsfinalBadFem(void) {return numJetsfinalBadFem; }
virtual int NumJetsfinalBadFtk(void) {return numJetsfinalBadFtk; }
virtual int NumBJetsfinal(void) {return numBJetsfinal; }
virtual int NumJetsMatched(void) {return numJetsMatched; }
virtual int NumJetsMatchedBadFem(void) {return numJetsMatchedBadFem; }
virtual int NumJetsMatchedBadFtk(void) {return numJetsMatchedBadFtk; }
virtual int NumUfos(void) {return numUfos; }
virtual int NumUfosNotPrimaryTrk(void) {return numUfosNotPrimaryTrk; }
virtual int NumUfosNotClean(void) {return numUfosNotClean; }
virtual int NumUfosDupl(void) {return numUfosDupl; }
virtual int NumUfosDuplBad(void) {return numUfosDuplBad; }
virtual int NumUfosNonIso(void) {return numUfosNonIso; }
virtual int NumUfosNonIsoBad(void) {return numUfosNonIsoBad; }
virtual int NumUfosfinal(void) {return numUfosfinal; }
virtual int NumUfosfinalBad(void) {return numUfosfinalBad; }
virtual int NumUfosMatched(void) {return numUfosMatched; }
virtual int NumUfosMatchedBad(void) {return numUfosMatchedBad; }

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
  float ana_minTauTagDiscriminator;  
  int  ana_ufoSelMethod;
  int  ana_ufoTkHitsmin;
  float ana_ufoCaloTowEFracmin;
  float ana_ufodRTrkFromJet;
  float ana_ufoEtaMax;
  float ana_ufoPtMin1;
  float ana_ufoDRmin;
  float ana_elecPtMin2;
  float ana_muonPtMin2;
  float ana_tauPtMin2;
  float ana_photonPtMin2;
  float ana_jetPtMin2;
  float ana_ufoPtMin2;
  // Define the parameters for extrapolation error adjustment
  float reco_elecD0ErrorThresh;
  float reco_elecDzErrorThresh;
  float reco_muonD0ErrorThresh;
  float reco_muonDzErrorThresh;
  float reco_tauD0ErrorThresh;
  float reco_tauDzErrorThresh;
  float reco_jetD0ErrorThresh;
  float reco_jetDzErrorThresh;
  float reco_bjetD0ErrorThresh;
  float reco_bjetDzErrorThresh;
  float reco_ufoD0ErrorThresh;
  float reco_ufoDzErrorThresh;
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
  int numElecNotCleanHOE;
  int numElecNotCleanShsh;
  int numElecNotCleanTmat;
  int numElecDupl;
  int numElecDuplBadHOE;
  int numElecDuplBadShsh;
  int numElecDuplBadTmat;
  int numElectronsNonIso;
  int numElectronsNonIsoBadHOE;
  int numElectronsNonIsoBadShsh;
  int numElectronsNonIsoBadTmat;
  int numElectronsfinal;
  int numElectronsfinalBadHOE;
  int numElectronsfinalBadShsh;
  int numElectronsfinalBadTmat;
  int numElectronsMatched;
  int numElectronsMatchedBadHOE;
  int numElectronsMatchedBadShsh;
  int numElectronsMatchedBadTmat;
  int numMuons;
  int numMuonNotPrimaryTrk;
  int numMuonNotClean;
  int numMuonDupl;
  int numMuonDuplBad;
  int numMuonsNonIso;
  int numMuonsNonIsoBad;
  int numMuonsfinal;
  int numMuonsfinalBad;
  int numMuonsMatched;
  int numMuonsMatchedBad;
  int numTaus;
  int numTauNotPrimaryTrk;
  int numTauNotClean;
  int numTauDupl;
  int numTauDuplBad;
  int numTausNonIso;
  int numTausNonIsoBad;
  int numTausfinal;
  int numTausfinalBad;
  int numTausMatched;
  int numTausMatchedBad;
  int numPhotons;
  int numPhotNotPrimaryTrk;
  int numPhotNotCleanHOE;
  int numPhotNotCleanShsh;
  int numPhotDupl;
  int numPhotDuplBadHOE;
  int numPhotDuplBadShsh;
  int numPhotonsNonIso;
  int numPhotonsNonIsoBadHOE;
  int numPhotonsNonIsoBadShsh;
  int numPhotonsfinal;
  int numPhotonsfinalBadHOE;
  int numPhotonsfinalBadShsh;
  int numPhotonsMatched;
  int numPhotonsMatchedBadHOE;
  int numPhotonsMatchedBadShsh;
  int numJets;
  int numJetNotPrimaryTrk;
  int numJetNotCleanFem;
  int numJetNotCleanFtk;
  int numJetDupl;
  int numJetDuplBadFem;
  int numJetDuplBadFtk;
  int numBJets;
  int numJetsfinal;
  int numJetsfinalBadFem;
  int numJetsfinalBadFtk;
  int numBJetsfinal;
  int numJetsMatched;
  int numJetsMatchedBadFem;
  int numJetsMatchedBadFtk;
  int numUfos;
  int numUfosNotPrimaryTrk;
  int numUfosNotClean;
  int numUfosDupl;
  int numUfosDuplBad;
  int numUfosNonIso;
  int numUfosNonIsoBad;
  int numUfosfinal;
  int numUfosfinalBad;
  int numUfosMatched;
  int numUfosMatchedBad;
  int counter;
  
  // Define MET vectors 
  math::XYZVector metRecoilvector;

  // Define the private methods included

};

#endif

