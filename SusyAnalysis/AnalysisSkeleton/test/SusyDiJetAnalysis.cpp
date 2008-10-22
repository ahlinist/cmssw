// -*- C++ -*-
//
// Package:    SusyDiJetAnalysis
// Class:      SusyDiJetAnalysis
// 
/**\class SusyDiJetAnalysis SusyDiJetAnalysis.cc SusyAnalysis/AnalysisSkeleton/src/SusyDiJetAnalysis.cc

Description: Skeleton analysis for SUSY search with Jets + MET

Implementation:Uses the EventSelector interface for event selection and TFileService for plotting.

*/
//
// Original Author:  Markus Stoye
//         Created:  Mon Feb 18 15:40:44 CET 2008
// $Id: SusyDiJetAnalysis.cpp,v 1.8 2008/06/09 12:19:02 fronga Exp $
//
//

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


  double mTempTreeHT2J;
  double mTempTreeHT3J;
  double mTempTreeHT4J;
  double mTempTreeMHT2J;
  double mTempTreeMHT2Jphi;
  double mTempTreeMHT3J;
  double mTempTreeMHT3Jphi;
  double mTempTreeMHT4J;
  double mTempTreeMHT4Jphi;

  double mTempTreeMET;
  double mTempTreeMEX;
  double mTempTreeMEY;
  double mTempTreeMETphi;
  double mTempTreeMETeta;
  double mTempTreeSumET;
  double mTempTreeSumETSignif;
  double mTempTreeMETuncor;
  double mTempTreeMETphiuncor;

  double mTempTreeR1;
  double mTempTreeR2;

  double mTempTreeAlpha;
  double mTempTreeAlphaT;
  double mTempTreeAlpha3j;
  double mTempTreeAlphaT3j;
  double mTempTreeAlpha4j;
  double mTempTreeAlphaT4j;

  double mTempTreeAlphaV3j;
  double mTempTreeAlphaVT3j;
  double mTempTreeAlphaV3j12;
  double mTempTreeAlphaVT3j12;
  double mTempTreeAlphaV3j13;
  double mTempTreeAlphaVT3j13;
  double mTempTreeAlphaV4j;
  double mTempTreeAlphaVT4j;

  double mTempTreeJJPhi;
  double mTempTreeJJPhi3j;
  double mTempTreeJJPhi4j;

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

  double mDeltaPhiHemi;
  double mAlphaTHemi;

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

//________________________________________________________________________________________
SusyDiJetAnalysis::SusyDiJetAnalysis(const edm::ParameterSet& iConfig):
  sequence_( iConfig.getParameter<edm::ParameterSet>("selections") ),
  plotSelection_( iConfig.getParameter<std::vector<std::string> >("plotSelection") ),
  eventWeight_( iConfig.getParameter<double>("eventWeight") ),
  //  weightSource_(iConfig.getParameter<edm::InputTag>("weightSource") ),genTag_(iConfig.getParameter<edm::InputTag>("genTag")), weight_(0.0), nrEventTotalRaw_(0), nrEventTotalWeighted_(0.0)
  weightSource_(iConfig.getParameter<edm::InputTag>("weightSource") ), weight_(0.0), nrEventTotalRaw_(0), nrEventTotalWeighted_(0.0),genTag_(iConfig.getParameter<edm::InputTag>("genTag"))

{
  // Translate plotSelection strings to indices
  plotSelectionIndices_.reserve(plotSelection_.size());
  for ( size_t i=0; i<plotSelection_.size(); ++i )
    plotSelectionIndices_.push_back(sequence_.selectorIndex(plotSelection_[i]));
    
  // List all selectors and selection variables
  edm::LogVerbatim("SusyDiJet") << "Selectors are:" << std::endl;
  for ( std::vector<const SusyEventSelector*>::const_iterator it = sequence_.selectors().begin();
        it != sequence_.selectors().end(); ++it )
    {
      edm::LogVerbatim("SusyDiJet") << " * " << (*it)->name()
                                          << " selects on following " 
                                          << (*it)->numberOfVariables() << " variable(s):";
      for ( unsigned int i=0; i<(*it)->numberOfVariables(); ++i )
        edm::LogVerbatim("SusyDiJet") << "    - " << (*it)->variableNames()[i];
      edm::LogVerbatim("SusyDiJet") << std::endl;
    }

  mSelectorResults = new unsigned int[sequence_.size()];

  // Say something about event weights
  if ( eventWeight_< 0. ) 
    edm::LogInfo("SusyDiJet") << "Will get event weights from event content";
  else
    edm::LogInfo("SusyDiJet") << "Global event weight set to " << eventWeight_;

  // get the data tags
  jetTag_ = iConfig.getParameter<edm::InputTag>("jetTag");
  metTag_ = iConfig.getParameter<edm::InputTag>("metTag");
  photTag_ = iConfig.getParameter<edm::InputTag>("photTag");
  elecTag_ = iConfig.getParameter<edm::InputTag>("elecTag");
  muonTag_ = iConfig.getParameter<edm::InputTag>("muonTag");
  tauTag_ = iConfig.getParameter<edm::InputTag>("tauTag");
  theSoup = iConfig.getUntrackedParameter<bool>("soup");
  fileWeight = iConfig.getUntrackedParameter<double>("weight");

  // trigger stuff
  triggerResults_ = iConfig.getParameter<edm::InputTag>("triggerResults");
  // trigger path names
  pathNames_ = iConfig.getParameter< std::vector<std::string> >("pathNames");


  // Initialise counters
  nrEventSelected_.resize( sequence_.size(), 0.0 );
  nrEventAllButOne_.resize( sequence_.size(), 0.0 );
  nrEventCumulative_.resize( sequence_.size(), 0.0 );

  localPi = acos(-1.0);

  // Initialise plots [should improve in the future]
  initPlots();

}


//________________________________________________________________________________________
SusyDiJetAnalysis::~SusyDiJetAnalysis() {}


//________________________________________________________________________________________
// Method called to for each event
void
SusyDiJetAnalysis::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  std::ostringstream dbg;

  ALPGENParticleId myALPGENParticleId;
  //  edm::LogInfo("SusyDiJetEvent") << "id: "<< myALPGENParticleId.AplGenParID(iEvent);
 

  run_   = iEvent.id().run();
  event_ = iEvent.id().event();
  // Get some event information (process ID, weight)
  processId_   = 0;
  if ( eventWeight_<0. ) {     // <0 => get weight from event
    edm::Handle<double> weightHandle;
    edm::Handle<int>    processHandle;
    iEvent.getByLabel(weightSource_.label(),"weight", weightHandle);
    weight_ = (*weightHandle);
    iEvent.getByLabel(weightSource_.label(),"AlpgenProcessID", processHandle);
    processId_ = (*processHandle);
  } else {
    weight_ = eventWeight_; // default: from config
  }
  
  // Retrieve the decision of each selector module
  SelectorDecisions decisions = sequence_.decisions(iEvent);
  
  // Count all events
  nrEventTotalRaw_++;
  nrEventTotalWeighted_ += weight_;
  
  // Fill plots with all variables
  bool dec(true);
  for ( size_t i=0; i<sequence_.size(); ++i ) {
    dec = dec && decisions.decision(i);
    //   edm::LogVerbatim("SusyDiJetEvent")
    //      << " " << sequence_.selectorName(i)
    //     << " " << decisions.decision(i)
    //     << " " << decisions.complementaryDecision(i)
    //     << " " << decisions.cumulativeDecision(i)
    //    << " " << dec << std::endl;
    
    // Add the decision to the tree
    mSelectorResults[i] = (decisions.decision(i)?1:0);
    
    // Update counters
    if ( decisions.decision(i) ) nrEventSelected_[i] += weight_;
    if ( decisions.complementaryDecision(i) ) nrEventAllButOne_[i] += weight_;
    if ( decisions.cumulativeDecision(i) ) nrEventCumulative_[i] += weight_;
    
  }

  // Fill some plots (only if some selections passed, as configured)
  dec = true;
  for ( size_t i=0; i<plotSelectionIndices_.size(); ++i )
    dec = dec&&decisions.decision(plotSelectionIndices_[i]);
  //  fillPlots( iEvent, decisions );
  if ( dec ) fillPlots( iEvent, decisions );       //write only evts surviving preselection to ntuple
  //  if ( true ) fillPlots( iEvent, decisions );  //write all events to ntuple

  // Print summary so far (every 10 till 100, every 100 till 1000, etc.)
  for ( unsigned int i=10; i<nrEventTotalRaw_; i*=10 )
    if ( nrEventTotalRaw_<=10*i && (nrEventTotalRaw_%i)==0 )
      printSummary();

  // markus
  ///
  if ( !sequence_.decisions(iEvent).globalDecision() ) {
    // Just fill the failure
    mGlobalDecision = 0;
    mSelectorData->Fill();
    return;
  }
  ///

  // Just fill the success
  mGlobalDecision = 1;
  mSelectorData->Fill();
  

  //get the trigger decision
  mTempTreeHLT1JET=false;
  mTempTreeHLT2JET=false;
  mTempTreeHLT1MET1HT=false;
  // Get the trigger results and check validity
  edm::Handle<edm::TriggerResults> hltHandle;
  iEvent.getByLabel(triggerResults_, hltHandle);
  if ( !hltHandle.isValid() ) {
    edm::LogWarning("HLTEventSelector") << "No trigger results for InputTag " << triggerResults_;
    return;
  }
  // Get results
  edm::TriggerNames trgNames;
  trgNames.init(*hltHandle);
  unsigned int trgSize = trgNames.size();
  // Example for OR of all specified triggers

  //looping over list of trig path names
  for ( std::vector<std::string>::const_iterator i=pathNames_.begin();
	i!=pathNames_.end(); ++i ) {
    // Get index
    unsigned int index = trgNames.triggerIndex(*i);
    if ( index==trgSize ) {
      edm::LogWarning("HLTEventSelector") << "Unknown trigger name " << *i;
      continue;
    }
    if ( hltHandle->accept(index) ) {
      LogDebug("HLTEventSelector") << "Event selected by " << *i;
      std::string trigName = *i;
      if (trigName == "HLT1jet") mTempTreeHLT1JET=true;
      if (trigName == "HLT2jet") mTempTreeHLT2JET=true;
      if (trigName == "HLT1MET1HT") mTempTreeHLT1MET1HT=true;
      //      edm::LogWarning("HLTEventSelector") << "Henning Trigger Name: "<< trigName << 
      //	           " Decision: " << mTempTreeHLT2JET << " " << mTempTreeHLT1MET1HT ;
    } 
  }

  //get the event weight
  mTempTreeEventWeight =1.;
  if (theSoup==true) {
    Handle<double> weightHandle;
    iEvent.getByLabel ("csaweightproducer","weight", weightHandle);
    mTempTreeEventWeight = * weightHandle;
  } else {
    mTempTreeEventWeight = fileWeight;
  }

  mTempTreeRun = run_;
  mTempTreeEvent = event_;

  
  //get truth info
  mTempTreeProcID = -999;
  if (theSoup==true) {
    Handle< int >  myProcess;
    iEvent.getByLabel("genEventProcID",myProcess);
    mTempTreeProcID = (*myProcess);
    if (mTempTreeProcID==4) {
      // ALPGEN has genEventProcID==4, but includes (W=0,Z=1,tt=2) + jets
      //      ALPGENParticleId myALPGENParticleId;
      //     mTempTreeProcID = myALPGENParticleId.AplGenParID(iEvent);
      Handle < int > myAlpProcess;
      iEvent.getByLabel("csaweightproducer","AlpgenProcessID",myAlpProcess);
      mTempTreeProcID = (*myAlpProcess);
    }
  }
  
  //get pthat of process
  mTempTreePthat = -999.;
  if (theSoup==true) {
    Handle<double> genEventScale;
    iEvent.getByLabel( "genEventScale", genEventScale );
    mTempTreePthat = *genEventScale;
  }
  

// Get the hemispheres
  Handle< edm::View<pat::Hemisphere> > hemisphereHandle;
  iEvent.getByLabel("selectedLayer2Hemispheres", hemisphereHandle);
  if ( !hemisphereHandle.isValid() ) {
    edm::LogWarning("SusySelectorExample") << "No Hemisphere results for InputTag ";
    return;
  }
  const edm::View<pat::Hemisphere>& hemispheres = (*hemisphereHandle); // For simplicity...
  

  mTempTreeNhemispheres = 2;
  for (int i=0;i < 2 ;i++){
    mTempTreeHemispheresPt[i] = hemispheres[i].pt();
    mTempTreeHemispheresE[i] = hemispheres[i].energy();
    mTempTreeHemispheresEt[i] = hemispheres[i].et();
    mTempTreeHemispheresPx[i] = hemispheres[i].momentum().X();
    mTempTreeHemispheresPy[i] = hemispheres[i].momentum().Y();
    mTempTreeHemispheresPz[i] = hemispheres[i].momentum().Z();
    mTempTreeHemispheresEta[i] = hemispheres[i].eta();
    mTempTreeHemispheresPhi[i] = hemispheres[i].phi();
  }   

  mDeltaPhiHemi = fabs(deltaPhi(hemispheres[0].phi(),hemispheres[1].phi()));

  double alphaHemi;
  // Get the one with lower et()
  if ( hemispheres[0].et() > hemispheres[1].et() ) {
    alphaHemi =  hemispheres[1].et();
  } else {
    alphaHemi = hemispheres[0].et();
  }
    
  // Now divide by the invariant mass of the dijet
  double energy_sumHemi = hemispheres[0].energy() + hemispheres[1].energy();
  double et_sumHemi = hemispheres[0].et() + hemispheres[1].et();
  // TODO - there must be a better function call for this:
  double momentum_sum_xHemi =
    (hemispheres[0].momentum().X()+hemispheres[1].momentum().X());
  double momentum_sum_yHemi =
    (hemispheres[0].momentum().Y()+hemispheres[1].momentum().Y());
  double momentum_sum_zHemi =
    (hemispheres[0].momentum().Z()+hemispheres[1].momentum().Z());
  
  // Note, sqrt is inefficient, could be adapted to use squared threshold
 
  double minvtHemi = sqrt(et_sumHemi*et_sumHemi - momentum_sum_xHemi*momentum_sum_xHemi - momentum_sum_yHemi*momentum_sum_yHemi);
  
// Calculate alpha
  alphaHemi /= minvtHemi;
  mAlphaTHemi = alphaHemi;
  //  if(alphaHemi > 0.55) cout << " alphaHemi " << alphaHemi << endl;

  
  // get the photons
  edm::Handle< std::vector<pat::Photon> > photHandle;
  iEvent.getByLabel(photTag_, photHandle);
  if ( !photHandle.isValid() ) {
    edm::LogWarning("SusySelectorExample") << "No Photon results for InputTag " << photTag_;
    return;
  }
  
  // Add the photons
  mTempTreeNphot = photHandle->size();
  if ( mTempTreeNphot > 50 ) mTempTreeNphot = 50;
  for (int i=0;i<mTempTreeNphot;i++){
    mTempTreePhotPt[i]  = (*photHandle)[i].pt();
    mTempTreePhotE[i]   = (*photHandle)[i].energy();
    mTempTreePhotEt[i]  = (*photHandle)[i].et();
    mTempTreePhotPx[i]  = (*photHandle)[i].momentum().X();
    mTempTreePhotPy[i]  = (*photHandle)[i].momentum().Y();
    mTempTreePhotPz[i]  = (*photHandle)[i].momentum().Z();
    mTempTreePhotEta[i] = (*photHandle)[i].eta();
    mTempTreePhotPhi[i] = (*photHandle)[i].phi();
    mTempTreePhotTrkIso[i] = (*photHandle)[i].trackIso();
    mTempTreePhotECalIso[i] = (*photHandle)[i].ecalIso();
    mTempTreePhotHCalIso[i] = (*photHandle)[i].hcalIso();
    mTempTreePhotAllIso[i] = (*photHandle)[i].caloIso();
  }
  
  
  // get the electrons
  edm::Handle< std::vector<pat::Electron> > elecHandle;
  iEvent.getByLabel(elecTag_, elecHandle);
  if ( !elecHandle.isValid() ) {
    edm::LogWarning("SusySelectorExample") << "No Electron results for InputTag " << elecTag_;
    return;
  }
  
  
  // Add the electrons
  mTempTreeNelec= elecHandle->size();
  if ( mTempTreeNelec > 50 ) mTempTreeNelec = 50;
  for (int i=0;i<mTempTreeNelec;i++){
    mTempTreeElecPt[i] = (*elecHandle)[i].pt();
    mTempTreeElecE[i] = (*elecHandle)[i].energy();
    mTempTreeElecEt[i] = (*elecHandle)[i].et();
    mTempTreeElecPx[i] = (*elecHandle)[i].momentum().X();
    mTempTreeElecPy[i] = (*elecHandle)[i].momentum().Y();
    mTempTreeElecPz[i] = (*elecHandle)[i].momentum().Z();
    mTempTreeElecEta[i] = (*elecHandle)[i].eta();
    mTempTreeElecPhi[i] = (*elecHandle)[i].phi();
    mTempTreeElecTrkIso[i] = ( (*elecHandle)[i].trackIso()+(*elecHandle)[i].et() )/(*elecHandle)[i].et();

    mTempTreeElecECalIso[i] = (*elecHandle)[i].ecalIso();
    mTempTreeElecHCalIso[i] = (*elecHandle)[i].hcalIso() ;
    mTempTreeElecAllIso[i] = (*elecHandle)[i].caloIso() ;
    //   if( (*elecHandle)[i].gsfTrack().isNonnull())
    //      mTempTreeElecTrkChiNorm[i] = (*elecHandle)[i].gsfTrack().get () ->chi2()/ (*elecHandle)[i].gsfTrack().get()->ndof();
    //    else  mTempTreeElecTrkChiNorm[i] = 999;
    mTempTreeElecCharge[i] = (*elecHandle)[i].charge();
  }
  
  
  // get the muons
  edm::Handle< std::vector<pat::Muon> > muonHandle;
  iEvent.getByLabel(muonTag_, muonHandle);
  if ( !muonHandle.isValid() ) {
    edm::LogWarning("SusySelectorExample") << "No Muon results for InputTag " << muonTag_;
    return;
  }
  

  // Add the muons
  mTempTreeNmuon= muonHandle->size();
  if ( mTempTreeNmuon > 50 ) mTempTreeNmuon = 50;
  for (int i=0;i<mTempTreeNmuon;i++){
    mTempTreeMuonPt[i] = (*muonHandle)[i].pt();
    mTempTreeMuonE[i] = (*muonHandle)[i].energy();
    mTempTreeMuonEt[i] = (*muonHandle)[i].et();
    mTempTreeMuonPx[i] = (*muonHandle)[i].momentum().X();
    mTempTreeMuonPy[i] = (*muonHandle)[i].momentum().Y();
    mTempTreeMuonPz[i] = (*muonHandle)[i].momentum().Z();
    mTempTreeMuonEta[i] = (*muonHandle)[i].eta();
    mTempTreeMuonPhi[i] = (*muonHandle)[i].phi();
    mTempTreeMuonTrkIso[i] = ( (*muonHandle)[i].trackIso()+(*muonHandle)[i].et() )/(*muonHandle)[i].et();
    mTempTreeMuonCharge[i] = (*muonHandle)[i].charge();
    mTempTreeMuonECalIso[i] = (*muonHandle)[i].ecalIso();
    mTempTreeMuonHCalIso[i] = (*muonHandle)[i].hcalIso() ;
    mTempTreeMuonAllIso[i] = (*muonHandle)[i].caloIso() ;
    if( (*muonHandle)[i].track().isNonnull())
      mTempTreeMuonTrkChiNorm[i] = (*muonHandle)[i].track().get () ->chi2()/ (*muonHandle)[i].track().get()->ndof();
    else  mTempTreeMuonTrkChiNorm[i] = 999;
  }
  
  double aMass = -1.;
  double diffZMass = 100.;
  mTempMuonPairMass = -1.;
  mTempMuonPairIndex[0]=-1;
  mTempMuonPairIndex[1]=-1;
  //  edm::LogInfo("SusyDiJetEvent") <<mTempTreeNmuon;
  if( mTempTreeNmuon > 1 )
    {
      //  edm::LogInfo("SusyDiJetEvent") << "2 or more muons";
      for(int i=0;i<mTempTreeNmuon;i++)
	{
	  //  edm::LogInfo("SusyDiJetEvent") << "loopi : "<< i;

	  for(int j=0;j<mTempTreeNmuon;j++)
	    {
	      //  edm::LogInfo("SusyDiJetEvent") << "loop j : "<< j;
	      if((i==j)||(i>j)||((*muonHandle)[i].charge()==(*muonHandle)[j].charge())) 
		{  continue;}
	      // edm::LogInfo("SusyDiJetEvent") << "Calculate Z mass";
	      aMass = ((*muonHandle)[i].p4()+(*muonHandle)[j].p4() ).mass();
	      if(diffZMass>fabs(aMass-91.0))
		{
		  diffZMass = fabs(aMass-91.0);
		  mTempMuonPairMass = aMass;
		  mTempMuonPairIndex[0]=i;
		  mTempMuonPairIndex[1]=j;
		}     
	    }
	}
    }
    
  // get the taus
  edm::Handle< std::vector<pat::Tau> > tauHandle;
  iEvent.getByLabel(tauTag_, tauHandle);
  if ( !tauHandle.isValid() ) {
    edm::LogWarning("SusySelectorExample") << "No Electron results for InputTag " << tauTag_;
    return;
  }
  
  // Add the taus
  mTempTreeNtau= tauHandle->size();
  if ( mTempTreeNtau > 50 ) mTempTreeNtau = 50;
  for (int i=0;i<mTempTreeNtau;i++){
    mTempTreeTauPt[i] = (*tauHandle)[i].pt();
    mTempTreeTauE[i] = (*tauHandle)[i].energy();
    mTempTreeTauEt[i] = (*tauHandle)[i].et();
    mTempTreeTauPx[i] = (*tauHandle)[i].momentum().X();
    mTempTreeTauPy[i] = (*tauHandle)[i].momentum().Y();
    mTempTreeTauPz[i] = (*tauHandle)[i].momentum().Z();
    mTempTreeTauEta[i] = (*tauHandle)[i].eta();
    mTempTreeTauPhi[i] = (*tauHandle)[i].phi();
    mTempTreeTauTrkIso[i] = ( (*tauHandle)[i].trackIso()+(*tauHandle)[i].et() )/(*tauHandle)[i].et();
    mTempTreeTauECalIso[i] = (*elecHandle)[i].ecalIso();
    mTempTreeTauHCalIso[i] = (*elecHandle)[i].hcalIso() ;
    mTempTreeTauAllIso[i] = (*elecHandle)[i].caloIso() ;
  }
  
  
  // get the jets
  edm::Handle< std::vector<pat::Jet> > jetHandle;
  iEvent.getByLabel(jetTag_, jetHandle);
  if ( !jetHandle.isValid() ) {
    edm::LogWarning("SusySelectorExample") << "No Jet results for InputTag " << jetTag_;
    return;
  }

  // check the number of jets
  if ( jetHandle->size() < 2 ) {
    edm::LogInfo("SusySelectorExample") << "Insufficient number of jets in event";
    return;
  }
  
  //get number of jets
  mTempTreeNjets = jetHandle->size();
  
  //calculate HT from the 2 jets:
  mTempTreeHT2J = (*jetHandle)[0].pt() + (*jetHandle)[1].pt();
  
  // do the azimuthal angle cut
  double delta_phi = (*jetHandle)[0].phi() - (*jetHandle)[1].phi();
  // Correct for negatives
  if ( delta_phi < 0.0 ) {
    delta_phi = -delta_phi;
  }  
  // Correct for smaller angle
  if ( delta_phi > localPi ) {
    delta_phi = 2.0*localPi - delta_phi;
  }  
  // 1D jjphi histogram
  //mJJPhi1DHistogram->Fill(delta_phi);
  mTempTreeJJPhi = delta_phi;
  

  //chf
  if (mTempTreeNjets > 2){
    double tmp_phi2j = atan2((*jetHandle)[1].momentum().Y()+(*jetHandle)[2].momentum().Y()
			     ,(*jetHandle)[1].momentum().X()+(*jetHandle)[2].momentum().X());
    delta_phi = (*jetHandle)[0].phi() -tmp_phi2j;
    if ( delta_phi < 0.0 ) {
      delta_phi = -delta_phi;
    }  
    if ( delta_phi > localPi ) {
      delta_phi = 2.0*localPi - delta_phi;
    }  
    mTempTreeJJPhi3j = delta_phi;
  } else {
    mTempTreeJJPhi3j = -999;
  }

  if (mTempTreeNjets > 3){
    double tmp_phi2j_1 = atan2((*jetHandle)[0].momentum().Y()+(*jetHandle)[3].momentum().Y()
			     ,(*jetHandle)[0].momentum().X()+(*jetHandle)[3].momentum().X());
    double tmp_phi2j_2 = atan2((*jetHandle)[1].momentum().Y()+(*jetHandle)[2].momentum().Y()
			     ,(*jetHandle)[1].momentum().X()+(*jetHandle)[2].momentum().X());
    delta_phi = tmp_phi2j_1-tmp_phi2j_2;
    if ( delta_phi < 0.0 ) {
      delta_phi = -delta_phi;
    }  
    if ( delta_phi > localPi ) {
      delta_phi = 2.0*localPi - delta_phi;
    }  
    mTempTreeJJPhi4j = delta_phi;
  } else {
    mTempTreeJJPhi4j = -999;
  }


  // Add the jets
  int i=0;
  if ( mTempTreeNjets >50 ) mTempTreeNjets = 50;
  for (int k=0;k<mTempTreeNjets;k++){
    if ( (*jetHandle)[k].et() > 20. ){
      mTempTreeJetsPt[i] = (*jetHandle)[i].pt();
      mTempTreeJetsE[i] = (*jetHandle)[i].energy();
      mTempTreeJetsEt[i] = (*jetHandle)[i].et();
      mTempTreeJetsPx[i] = (*jetHandle)[i].momentum().X();
      mTempTreeJetsPy[i] = (*jetHandle)[i].momentum().Y();
      mTempTreeJetsPz[i] = (*jetHandle)[i].momentum().Z();
      mTempTreeJetsEta[i] = (*jetHandle)[i].eta();
      mTempTreeJetsPhi[i] = (*jetHandle)[i].phi();
      mTempTreeJetsFem[i] = (*jetHandle)[i].emEnergyFraction();

      if((*jetHandle)[i].genJet()!= 0) {
	mTempTreeGenJetsPt[i]=(*jetHandle)[i].genJet()->pt();
	mTempTreeGenJetsE[i]=(*jetHandle)[i].genJet()->energy();
	mTempTreeGenJetsEt[i]=(*jetHandle)[i].genJet()->et();
	mTempTreeGenJetsPx[i]=(*jetHandle)[i].genJet()->momentum().X();
	mTempTreeGenJetsPy[i]=(*jetHandle)[i].genJet()->momentum().Y();
	mTempTreeGenJetsPz[i]=(*jetHandle)[i].genJet()->momentum().z();
	mTempTreeGenJetsEta[i]=(*jetHandle)[i].genJet()->eta();
	mTempTreeGenJetsPhi[i]=(*jetHandle)[i].genJet()->phi();
      }
      else {
	mTempTreeGenJetsPt[i]  =-999;
	mTempTreeGenJetsE[i]   =-999;
	mTempTreeGenJetsEt[i]  =-999;
	mTempTreeGenJetsPx[i]  =-999;
	mTempTreeGenJetsPy[i]  =-999;
	mTempTreeGenJetsPz[i]  =-999;
	mTempTreeGenJetsEta[i] =-999;
	mTempTreeGenJetsPhi[i] =-999;
      }

      //  const reco::TrackRefVector & associatedTracks();      
      mTempTreeNJetsT[i] = ((*jetHandle)[i].associatedTracks()).size();
      if(((*jetHandle)[i].associatedTracks()).isAvailable()) { // edm::LogWarning("SusySelectorExample") << "Jet Tracks"<< " i "<< i<<" "<< mTempTreeNJetsT[i]<< " "<< ((*jetHandle)[i].associatedTracks())[0]->pt();
	
	float maxPt = 0;
	int maxPtIndex = 0;
	for(int pttest=0; pttest<mTempTreeNJetsT[i];pttest++) { 
	  if(maxPt<float(((*jetHandle)[i].associatedTracks())[pttest]->pt())) {
	    maxPt = float(((*jetHandle)[i].associatedTracks())[pttest]->pt());
	    maxPtIndex = pttest;
	  }
	  
	  //	edm::LogWarning("SusySelectorExample") << "Jet Tracks pt"<<  float(((*jetHandle)[i].associatedTracks())[pttest]->pt());
	}
	//  edm::LogWarning("SusySelectorExample") << "Jet Tracks tst: "<<float(((*jetHandle)[i].associatedTracks())[maxPtIndex]->pt());
	mTempTreeJetsTPt[i] = float(((*jetHandle)[i].associatedTracks())[maxPtIndex]->pt());
	mTempTreeJetsTEta[i] = float(((*jetHandle)[i].associatedTracks())[maxPtIndex]->eta());
	mTempTreeJetsTPhi[i] = float(((*jetHandle)[i].associatedTracks())[maxPtIndex]->phi());
      }
      else {
	//edm::LogWarning("SusySelectorExample") << "No Jet Tracks"<< " i "<< i<<" "<< mTempTreeNJetsT[i]   ;
	mTempTreeJetsTPt[i] = -1 ;
	mTempTreeJetsTEta[i] = -10 ;
	mTempTreeJetsTPhi[i] = -10 ;
      }   
      i++;
    }
  }
  mTempTreeNjets = i;
  // Calculate alpha
  // = pt of 2nd hardest jet / dijet invariant mass
  double alpha;
  // Get the one with lower et()
  if ( (*jetHandle)[0].et() > (*jetHandle)[1].et() ) {
    alpha = (*jetHandle)[1].et();
  } else {
    alpha = (*jetHandle)[0].et();
  }
    
  // Now divide by the invariant mass of the dijet
  double energy_sum = (*jetHandle)[0].energy() + (*jetHandle)[1].energy();
  double et_sum = (*jetHandle)[0].et() + (*jetHandle)[1].et();
  // TODO - there must be a better function call for this:
  double momentum_sum_x =
    ((*jetHandle)[0].momentum().X()+(*jetHandle)[1].momentum().X());
  double momentum_sum_y =
    ((*jetHandle)[0].momentum().Y()+(*jetHandle)[1].momentum().Y());
  double momentum_sum_z =
    ((*jetHandle)[0].momentum().Z()+(*jetHandle)[1].momentum().Z());
  
  // Note, sqrt is inefficient, could be adapted to use squared threshold
  double invariant_mass = sqrt(energy_sum*energy_sum - momentum_sum_x*momentum_sum_x 
			       - momentum_sum_y*momentum_sum_y - momentum_sum_z*momentum_sum_z);
  double minvt = sqrt(et_sum*et_sum - momentum_sum_x*momentum_sum_x - momentum_sum_y*momentum_sum_y);
  
// Calculate alpha
  alpha /= invariant_mass;
  
  // Fill the histogram
  //mAlpha1DHistogram->Fill(alpha);
  mTempTreeAlpha = alpha;
  mTempTreeAlphaT = (*jetHandle)[1].et()/minvt;

  // Do the MHT save
  // Just vector sum of di-jet momenta scaled
  mTempTreeMHT2J = sqrt(momentum_sum_x*momentum_sum_x + momentum_sum_y*momentum_sum_y);
  mTempTreeMHT2Jphi = atan2(-momentum_sum_y,-momentum_sum_x);

  if ( mTempTreeNjets > 2 ) {
    energy_sum += (*jetHandle)[2].energy();
    et_sum += (*jetHandle)[2].et();
    momentum_sum_x +=(*jetHandle)[2].momentum().X();
    momentum_sum_y +=(*jetHandle)[2].momentum().Y();
    momentum_sum_z +=(*jetHandle)[2].momentum().Z();
    invariant_mass = sqrt(energy_sum*energy_sum - momentum_sum_x*momentum_sum_x 
			  - momentum_sum_y*momentum_sum_y - momentum_sum_z*momentum_sum_z);
    minvt = sqrt(et_sum*et_sum - momentum_sum_x*momentum_sum_x - momentum_sum_y*momentum_sum_y);

    double pt23 = sqrt(pow(mTempTreeJetsPx[1]+mTempTreeJetsPx[2],2) 
                     + pow(mTempTreeJetsPy[1]+mTempTreeJetsPy[2],2));
    double p23  = sqrt(pow(mTempTreeJetsPx[1]+mTempTreeJetsPx[2],2) 
                     + pow(mTempTreeJetsPy[1]+mTempTreeJetsPy[2],2)
                     + pow(mTempTreeJetsPz[1]+mTempTreeJetsPz[2],2) );
    double etvec23 = (mTempTreeJetsE[1]+mTempTreeJetsE[2])* pt23/p23;

    double pt13 = sqrt(pow(mTempTreeJetsPx[0]+mTempTreeJetsPx[2],2) 
                     + pow(mTempTreeJetsPy[0]+mTempTreeJetsPy[2],2));
    double p13  = sqrt(pow(mTempTreeJetsPx[0]+mTempTreeJetsPx[2],2) 
                     + pow(mTempTreeJetsPy[0]+mTempTreeJetsPy[2],2)
                     + pow(mTempTreeJetsPz[0]+mTempTreeJetsPz[2],2) );
    double etvec13 = (mTempTreeJetsE[0]+mTempTreeJetsE[2])* pt13/p13;

    double pt12 = sqrt(pow(mTempTreeJetsPx[1]+mTempTreeJetsPx[0],2) 
                     + pow(mTempTreeJetsPy[1]+mTempTreeJetsPy[0],2));
    double p12  = sqrt(pow(mTempTreeJetsPx[1]+mTempTreeJetsPx[0],2) 
                     + pow(mTempTreeJetsPy[1]+mTempTreeJetsPy[0],2)
                     + pow(mTempTreeJetsPz[1]+mTempTreeJetsPz[0],2) );
    double etvec12 = (mTempTreeJetsE[1]+mTempTreeJetsE[0])* pt12/p12;

    if ( (*jetHandle)[0].et() > etvec23){
      mTempTreeAlphaV3j  = etvec23/invariant_mass;
      mTempTreeAlphaVT3j = etvec23/minvt;
    } else {
      mTempTreeAlphaV3j  = (*jetHandle)[0].et()/invariant_mass;
      mTempTreeAlphaVT3j = (*jetHandle)[0].et()/minvt;
    }
    if ( (*jetHandle)[1].et() > etvec13){
      mTempTreeAlphaV3j13  = etvec13/invariant_mass;
      mTempTreeAlphaVT3j13 = etvec13/minvt;
    } else {
      mTempTreeAlphaV3j13  = (*jetHandle)[1].et()/invariant_mass;
      mTempTreeAlphaVT3j13 = (*jetHandle)[1].et()/minvt;
    }
    if ( (*jetHandle)[2].et() > etvec12){
      mTempTreeAlphaV3j12  = etvec12/invariant_mass;
      mTempTreeAlphaVT3j12 = etvec12/minvt;
    } else {
      mTempTreeAlphaV3j12  = (*jetHandle)[2].et()/invariant_mass;
      mTempTreeAlphaVT3j12 = (*jetHandle)[2].et()/minvt;
    }

    if ( (*jetHandle)[0].et() > (*jetHandle)[1].et()+(*jetHandle)[2].et()){
      mTempTreeAlpha3j = ((*jetHandle)[1].et()+(*jetHandle)[2].et())/invariant_mass;
      mTempTreeAlphaT3j = ((*jetHandle)[1].et()+(*jetHandle)[2].et())/minvt;
    } else {
      mTempTreeAlpha3j = (*jetHandle)[0].et()/invariant_mass;
      mTempTreeAlphaT3j = (*jetHandle)[0].et()/minvt;
    }

    mTempTreeHT3J = mTempTreeHT2J + (*jetHandle)[2].pt();
    mTempTreeMHT3J = sqrt(momentum_sum_x*momentum_sum_x + momentum_sum_y*momentum_sum_y);
    mTempTreeMHT3Jphi = atan2(-momentum_sum_y,-momentum_sum_x);

  } else {
    mTempTreeAlpha3j  = -999;
    mTempTreeAlphaT3j = -999;

    mTempTreeAlphaV3j    = -999;
    mTempTreeAlphaVT3j   = -999;
    mTempTreeAlphaV3j13  = -999;
    mTempTreeAlphaVT3j13 = -999;
    mTempTreeAlphaV3j12  = -999;
    mTempTreeAlphaVT3j12 = -999;

    mTempTreeHT3J     = -999;
    mTempTreeMHT3J    = -999;
    mTempTreeMHT3Jphi = -999;
  }

  if ( mTempTreeNjets > 3 ) {
    energy_sum += (*jetHandle)[3].energy();
    et_sum += (*jetHandle)[3].et();
    momentum_sum_x +=(*jetHandle)[3].momentum().X();
    momentum_sum_y +=(*jetHandle)[3].momentum().Y();
    momentum_sum_z +=(*jetHandle)[3].momentum().Z();
    invariant_mass = sqrt(energy_sum*energy_sum - momentum_sum_x*momentum_sum_x 
			  - momentum_sum_y*momentum_sum_y - momentum_sum_z*momentum_sum_z);
    minvt = sqrt(et_sum*et_sum - momentum_sum_x*momentum_sum_x - momentum_sum_y*momentum_sum_y);



    double pt23 = sqrt(pow(mTempTreeJetsPx[1]+mTempTreeJetsPx[2],2) + pow(mTempTreeJetsPy[1]+mTempTreeJetsPy[2],2));
    double p23  = sqrt(pow(mTempTreeJetsPx[1]+mTempTreeJetsPx[2],2) + pow(mTempTreeJetsPy[1]+mTempTreeJetsPy[2],2)
                     + pow(mTempTreeJetsPz[1]+mTempTreeJetsPz[2],2) );

    double pt14 = sqrt(pow(mTempTreeJetsPx[0]+mTempTreeJetsPx[3],2) + pow(mTempTreeJetsPy[0]+mTempTreeJetsPy[3],2));
    double p14  = sqrt(pow(mTempTreeJetsPx[0]+mTempTreeJetsPx[3],2) + pow(mTempTreeJetsPy[0]+mTempTreeJetsPy[3],2)
                     + pow(mTempTreeJetsPz[0]+mTempTreeJetsPz[3],2) );

    double etvec23 = (mTempTreeJetsE[1]+mTempTreeJetsE[2])* pt23/p23;
    double etvec14 = (mTempTreeJetsE[0]+mTempTreeJetsE[3])* pt14/p14;

    if ( etvec14 > etvec23 ){
      mTempTreeAlphaV4j  = etvec23/invariant_mass;
      mTempTreeAlphaVT4j = etvec23/minvt;
    } else {
      mTempTreeAlphaV4j  = etvec14/invariant_mass;
      mTempTreeAlphaVT4j = etvec14/minvt;
    }

    if ( (*jetHandle)[0].et()+ (*jetHandle)[3].et() > (*jetHandle)[1].et()+(*jetHandle)[2].et()){
      mTempTreeAlpha4j  = ((*jetHandle)[1].et()+(*jetHandle)[2].et())/invariant_mass;
      mTempTreeAlphaT4j = ((*jetHandle)[1].et()+(*jetHandle)[2].et())/minvt;
    } else {
      mTempTreeAlpha4j  = ((*jetHandle)[0].et()+(*jetHandle)[3].et())/invariant_mass;
      mTempTreeAlphaT4j = ((*jetHandle)[0].et()+(*jetHandle)[3].et())/minvt;
    }

    mTempTreeHT4J = mTempTreeHT3J+(*jetHandle)[3].pt();
    mTempTreeMHT4J = sqrt(momentum_sum_x*momentum_sum_x + momentum_sum_y*momentum_sum_y);
    mTempTreeMHT4Jphi = atan2(-momentum_sum_y,-momentum_sum_x);

  } else {
    mTempTreeAlpha4j   = -999;
    mTempTreeAlphaT4j  = -999;
    mTempTreeAlphaV4j  = -999;
    mTempTreeAlphaVT4j = -999;
    mTempTreeHT4J      = -999;
    mTempTreeMHT4J     = -999;
    mTempTreeMHT4Jphi  = -999;
  }

  //
  // get the MET result
  //
  edm::Handle< std::vector<pat::MET> > metHandle;
  iEvent.getByLabel(metTag_, metHandle);
  if ( !metHandle.isValid() ) {
    edm::LogWarning("METEventSelector") << "No Met results for InputTag " << metTag_;
    return;
  }
  
  //
  // sanity check on collection
  //
  if ( metHandle->size()!=1 ) {
    edm::LogWarning("METEventSelector") << "MET collection size is "
					<< metHandle->size() << " instead of 1";
    return;
  }
  
  // Do the MET save
  mTempTreeMET = metHandle->front().et();
  mTempTreeMEX = metHandle->front().momentum().X();
  mTempTreeMEY = metHandle->front().momentum().Y();
  mTempTreeSumET = metHandle->front().sumEt();
  mTempTreeMETeta = metHandle->front().eta();
  mTempTreeMETphi = metHandle->front().phi();
  mTempTreeSumETSignif = metHandle->front().mEtSig();
  
  mTempTreeMETuncor = metHandle->front().uncorrectedPt(pat::MET::UncorectionType(2));
  mTempTreeMETphiuncor = metHandle->front().uncorrectedPhi(pat::MET::UncorectionType(2));
  
  
  
  //calculate R1 and R2
  //  double dphi1 = mTempTreeJetsPhi[0] - mTempTreeMETphi;
  //  double dphi2 = mTempTreeJetsPhi[1] - mTempTreeMETphi;
  //calculate R1 and R2 with MHT instead of MET
  double dphi1 = mTempTreeJetsPhi[0] - mTempTreeMHT2Jphi;
  double dphi2 = mTempTreeJetsPhi[1] - mTempTreeMHT2Jphi;
  
  if (dphi1 < 0) dphi1 *=-1;
  if (dphi1 > localPi) dphi1 = 2.*localPi - dphi1;
  if (dphi2 < 0) dphi2 *=-1;
  if (dphi2 > localPi) dphi2 = 2.*localPi - dphi2;
  
  mTempTreeR1 = sqrt(dphi2*dphi2 + (localPi - dphi1)*(localPi - dphi1));
  mTempTreeR2 = sqrt(dphi1*dphi1 + (localPi - dphi2)*(localPi - dphi2));
  

  mTempAlpIdTest = myALPGENParticleId.AplGenParID(iEvent,genTag_);
  mTempAlpPtScale = myALPGENParticleId.getPt();
  // std::vector<int>* ids;
  // std::vector<int>* refs;
  // std::vector<int>* ids_test;
  // std::vector<int>* refs_test;
 
  //set information of event is affected by b-bug
  is_ok = true;
  length = 1000;
  length =  myALPGENParticleId.AplGenParID(iEvent,genTag_,  ids , refs ,genPt ,genPhi ,genEta ,genStatus, length);

  mTempSimuCheck = myALPGENParticleId.SimBug(iEvent,genTag_);

  float min_dR;
  int matched_jet = 0;
  int matched_b = 0;
  for(int i=0 ; i < length;i++) {
    min_dR = 100;
    matched_jet = -1;
    matched_b = -1;
    if(fabs(ids[i]) == 5 && genPt[i] > 100){
      for (int jeti=0;jeti < mTempTreeNjets;jeti++){
	
	if(sqrt(reco::deltaPhi((*jetHandle)[jeti].phi(),genPhi[i])
		*reco::deltaPhi((*jetHandle)[jeti].phi(),genPhi[i])
		+((*jetHandle)[jeti].eta()-genEta[i])*((*jetHandle)[jeti].eta()-genEta[i])) < min_dR) {
	  min_dR = sqrt(reco::deltaPhi((*jetHandle)[jeti].phi(),genPhi[i]) 
			* reco::deltaPhi((*jetHandle)[jeti].phi(),genPhi[i])
			+((*jetHandle)[jeti].eta()-genEta[i]) * ((*jetHandle)[jeti].eta()-genEta[i]));
	  matched_jet = jeti;
	  matched_b = i; 
	}
      }
      if(fabs((*jetHandle)[matched_jet].pt()- genPt[matched_b])/(*jetHandle)[matched_jet].pt() > 5 && min_dR < 0.5)  is_ok = false;
    }
  }

  
  //   for(int i=0 ; i<length;i++)
  //     {
  //      edm::LogInfo("SusyDiJetEvent")  <<  "id: "<< ids[i] << " Pt "<<genPt[i] <<" Eta "<<genEta[i] <<  "Phi" <<genPhi[i]  <<" mother "<< refs[i];
  
  //         if    (ids[i]==2000002) edm::LogInfo("SusyDiJetEvent")  <<"jaja = 2000002";
  //   }
   
  
  // Fill the tree
  mAllData->Fill();
  
}


//________________________________________________________________________________________
void 
SusyDiJetAnalysis::beginJob(const edm::EventSetup&) {}

//________________________________________________________________________________________
void 
SusyDiJetAnalysis::endJob() {

  printSummary();

}


//________________________________________________________________________________________
void
SusyDiJetAnalysis::printSummary( void ) {

  edm::LogWarning("SusyDiJet|SummaryCount") << "*** Summary of counters: ";
  edm::LogWarning("SusyDiJet|SummaryCount") 
    << "Total number of events = " << nrEventTotalWeighted_ 
    << " (" << nrEventTotalRaw_ << " unweighted)"
    << " ; selected = " << nrEventCumulative_.back();

  std::ostringstream summary;
  summary << std::setw(21) << std::left << "Name"
          << std::setw(21) << "Selected"
          << std::setw(21) << "AllButOne"
          << std::setw(21) << "Cumulative" << std::endl;
         
  for ( size_t i=0; i<sequence_.size(); ++i ) {
    summary << std::setw(20) << std::left << sequence_.selectorName(i) << std::right;
    summary << std::setw(10) << std::setprecision(2)  << std::fixed
            << nrEventSelected_[i] 
            << "[" << std::setw(6) 
            << (nrEventSelected_[i]/nrEventTotalWeighted_)*100. << "%]  ";
    summary << std::setw(10) << nrEventAllButOne_[i] 
            << "[" << std::setw(6) 
            << (nrEventAllButOne_[i]/nrEventTotalWeighted_)*100. << "%]  ";
    summary << std::setw(10) << nrEventCumulative_[i] 
            << "[" << std::setw(6) 
            << (nrEventCumulative_[i]/nrEventTotalWeighted_)*100. << "%]  ";
    summary << std::endl; 
  }
  edm::LogWarning("SusyDiJet|SummaryCount") << summary.str();

}


//________________________________________________________________________________________
void
SusyDiJetAnalysis::initPlots() {

  std::ostringstream variables; // Container for all variables

  // 1. Event variables
  variables << "weight:process";

  // 2. Decision from all selectors
  for ( std::vector<const SusyEventSelector*>::const_iterator it = sequence_.selectors().begin();
        it != sequence_.selectors().end(); ++it ) {
    std::string var( (*it)->name() );
    var += "_result";
    // Push to list of variables
    variables << ":" << var;
  }
  variables << ":all_result"; // Also store global decision
    
  // 3. All variables from sequence
  for ( std::vector<const SusyEventSelector*>::const_iterator it = sequence_.selectors().begin();
        it != sequence_.selectors().end(); ++it ) {
    for ( unsigned int i=0; i<(*it)->numberOfVariables(); ++i ) {
      std::string var( (*it)->name() ); // prefix variable with selector name
      var += "." + (*it)->variableNames()[i];
      // Push to list of variables
      variables << ":" << var;
    }
  }

  // Register this ntuple
  edm::Service<TFileService> fs;
  ntuple_ = fs->make<TNtuple>( "ntuple","SusyDiJetAnalysis variables",
                               variables.str().c_str() );

  // Now we add some additional ones for the dijet analysis
  mAllData = fs->make<TTree>( "allData", "data after cuts" );
  mSelectorData = fs->make<TTree>( "selectorData" , "Bit results for selectors");

  mAllData->SetAutoSave(10000);
  mSelectorData->SetAutoSave(10000);

  std::vector<std::string> names = sequence_.selectorNames();
  for ( size_t i = 0 ; i < sequence_.size() ; ++i ) {
    std::string tempName = names[i] + "/i";
    mSelectorData->Branch(names[i].c_str(),&mSelectorResults[i],tempName.c_str());
  }
  mSelectorData->Branch("globalDecision",&mGlobalDecision,"globalDecision/i");


  // Add the branches
  mAllData->Branch("run",&mTempTreeRun,"run/int");
  mAllData->Branch("event",&mTempTreeEvent,"event/int");

  mAllData->Branch("HLT1JET",&mTempTreeHLT1JET,"HLT1JET/bool");
  mAllData->Branch("HLT2JET",&mTempTreeHLT2JET,"HLT2JET/bool");
  mAllData->Branch("HLT1MET1HT",&mTempTreeHLT1MET1HT,"HLT1MET1HT/bool");

  mAllData->Branch("met",&mTempTreeMET,"met/double");
  mAllData->Branch("mex",&mTempTreeMEY,"mex/double");
  mAllData->Branch("mey",&mTempTreeMEX,"mey/double");
  mAllData->Branch("meteta",&mTempTreeMETeta,"meteta/double");
  mAllData->Branch("metphi",&mTempTreeMETphi,"metphi/double");
  mAllData->Branch("metuncor",&mTempTreeMETuncor,"metuncor/double");
  mAllData->Branch("metphiuncor",&mTempTreeMETphiuncor,"metphiuncor/double");


  mAllData->Branch("sumEt",&mTempTreeSumET,"sumEt/double");
  mAllData->Branch("sumEtsignif",&mTempTreeSumETSignif,"sumEtsignif/double");

  mAllData->Branch("ht2j",&mTempTreeHT2J,"ht2j/double");
  mAllData->Branch("ht3j",&mTempTreeHT3J,"ht3j/double");
  mAllData->Branch("ht4j",&mTempTreeHT4J,"ht4j/double");
  mAllData->Branch("mht2j",&mTempTreeMHT2J,"mht2j/double");
  mAllData->Branch("mht2jphi",&mTempTreeMHT2Jphi,"mht2jphi/double");
  mAllData->Branch("mht3j",&mTempTreeMHT3J,"mht3j/double");
  mAllData->Branch("mht3jphi",&mTempTreeMHT3Jphi,"mht3jphi/double");
  mAllData->Branch("mht4j",&mTempTreeMHT4J,"mht4j/double");
  mAllData->Branch("mht4jphi",&mTempTreeMHT4Jphi,"mht4jphi/double");

  mAllData->Branch("evtWeight",&mTempTreeEventWeight,"evtWeight/double");
  mAllData->Branch("procID",&mTempTreeProcID,"procID/int");
  mAllData->Branch("pthat",&mTempTreePthat,"pthat/double");

  mAllData->Branch("alpha",&mTempTreeAlpha,"alpha/double");
  mAllData->Branch("alpha3j",&mTempTreeAlpha3j,"alpha3j/double");
  mAllData->Branch("alpha4j",&mTempTreeAlpha4j,"alpha4j/double");
  mAllData->Branch("alphav3j",&mTempTreeAlphaV3j,"alphav3j/double");
  mAllData->Branch("alphav3j12",&mTempTreeAlphaV3j12,"alphav3j12/double");
  mAllData->Branch("alphav3j13",&mTempTreeAlphaV3j13,"alphav3j13/double");
  mAllData->Branch("alphav4j",&mTempTreeAlphaV4j,"alphav4j/double");
  mAllData->Branch("alphat",&mTempTreeAlphaT,"alphat/double");
  mAllData->Branch("alphat3j",&mTempTreeAlphaT3j,"alphat3j/double");
  mAllData->Branch("alphat4j",&mTempTreeAlphaT4j,"alphat4j/double");
  mAllData->Branch("alphavt3j",&mTempTreeAlphaVT3j,"alphavt3j/double");
  mAllData->Branch("alphavt3j12",&mTempTreeAlphaVT3j12,"alphavt3j12/double");
  mAllData->Branch("alphavt3j13",&mTempTreeAlphaVT3j13,"alphavt3j13/double");
  mAllData->Branch("alphavt4j",&mTempTreeAlphaVT4j,"alphavt4j/double");
  mAllData->Branch("jjphi",&mTempTreeJJPhi,"jjphi/double");		   
  mAllData->Branch("jjphi3j",&mTempTreeJJPhi3j,"jjphi3j/double");		   
  mAllData->Branch("jjphi4j",&mTempTreeJJPhi4j,"jjphi4j/double");		   

  mAllData->Branch("R1",&mTempTreeR1,"R1/double");
  mAllData->Branch("R2",&mTempTreeR2,"R2/double");		   

  mAllData->Branch("DeltaPhiHemi",&mDeltaPhiHemi,"DeltaPhiHemi/double");
  mAllData->Branch("AlphaTHemi",&mAlphaTHemi,"AlphaTHemi/double");

 //add hemispheres
  mAllData->Branch("Nhemispheres" ,&mTempTreeNhemispheres ,"Nhemispheres/int");  
  mAllData->Branch("HemisphereE" ,mTempTreeHemispheresE ,"HemisphereE[Nhemispheres]/double");
  mAllData->Branch("HemisphereEt",mTempTreeHemispheresEt,"HemisphereEt[Nhemispheres]/double");
  mAllData->Branch("Hemispherept",mTempTreeHemispheresPt,"Hemispherept[Nhemispheres]/double");
  mAllData->Branch("Hemispherepx",mTempTreeHemispheresPx,"Hemispherepx[Nhemispheres]/double");
  mAllData->Branch("Hemispherepy",mTempTreeHemispheresPy,"Hemispherepy[Nhemispheres]/double");
  mAllData->Branch("Hemispherepz",mTempTreeHemispheresPz,"Hemispherepz[Nhemispheres]/double");
  mAllData->Branch("Hemisphereeta",mTempTreeHemispheresEta,"Hemisphereeta[Nhemispheres]/double");
  mAllData->Branch("Hemispherephi",mTempTreeHemispheresPhi,"Hemispherephi[Nhemispheres]/double");

  mAllData->Branch("is_ok",&is_ok,"is_ok/bool");

  mAllData->Branch("SimuCheck",&mTempSimuCheck,"mTempSimuCheck/int");

  //add jets
  mAllData->Branch("Njets" ,&mTempTreeNjets ,"Njets/int");  
  mAllData->Branch("JetE" ,mTempTreeJetsE ,"JetE[Njets]/double");
  mAllData->Branch("JetEt",mTempTreeJetsEt,"JetEt[Njets]/double");
  mAllData->Branch("Jetpt",mTempTreeJetsPt,"Jetpt[Njets]/double");
  mAllData->Branch("Jetpx",mTempTreeJetsPx,"Jetpx[Njets]/double");
  mAllData->Branch("Jetpy",mTempTreeJetsPy,"Jetpy[Njets]/double");
  mAllData->Branch("Jetpz",mTempTreeJetsPz,"Jetpz[Njets]/double");
  mAllData->Branch("Jeteta",mTempTreeJetsEta,"Jeteta[Njets]/double");
  mAllData->Branch("Jetphi",mTempTreeJetsPhi,"Jetphi[Njets]/double");
  mAllData->Branch("JetFem",mTempTreeJetsFem,"JetFem[Njets]/double");
  mAllData->Branch("NJetsT",mTempTreeNJetsT,"NJetsT[Njets]/int");
  mAllData->Branch("JetsTPt",mTempTreeJetsTPt,"JetsTPt[Njets]/float");
  mAllData->Branch("JetsTEta",mTempTreeJetsTEta,"JetsTEta[Njets]/float");
  mAllData->Branch("JetsTPhi",mTempTreeJetsTPhi,"JetsTPhi[Njets]/float");

  mAllData->Branch("GenJetE" ,mTempTreeGenJetsE ,"GenJetE[Njets]/double");
  mAllData->Branch("GenJetEt",mTempTreeGenJetsEt,"GenJetEt[Njets]/double");
  mAllData->Branch("GenJetpt",mTempTreeGenJetsPt,"GenJetpt[Njets]/double");
  mAllData->Branch("GenJetpx",mTempTreeGenJetsPx,"GenJetpx[Njets]/double");
  mAllData->Branch("GenJetpy",mTempTreeGenJetsPy,"GenJetpy[Njets]/double");
  mAllData->Branch("GenJetpz",mTempTreeGenJetsPz,"GenJetpz[Njets]/double");
  mAllData->Branch("GenJeteta",mTempTreeGenJetsEta,"GenJeteta[Njets]/double");
  mAllData->Branch("GenJetphi",mTempTreeGenJetsPhi,"GenJetphi[Njets]/double");
 
  //add photons
  mAllData->Branch("Nphot" ,&mTempTreeNphot ,"Nphot/int");  
  mAllData->Branch("PhotE" ,mTempTreePhotE ,"PhotE[Nphot]/double");
  mAllData->Branch("PhotEt",mTempTreePhotEt,"PhotEt[Nphot]/double");
  mAllData->Branch("Photpt",mTempTreePhotPt,"Photpt[Nphot]/double");
  mAllData->Branch("Photpx",mTempTreePhotPx,"Photpx[Nphot]/double");
  mAllData->Branch("Photpy",mTempTreePhotPy,"Photpy[Nphot]/double");
  mAllData->Branch("Photpz",mTempTreePhotPz,"Photpz[Nphot]/double");
  mAllData->Branch("Photeta",mTempTreePhotEta,"Photeta[Nphot]/double");
  mAllData->Branch("Photphi",mTempTreePhotPhi,"Photphi[Nphot]/double");
  mAllData->Branch("PhotTrkIso",mTempTreePhotTrkIso,"mTempTreePhotTrkIso[Nphot]/double");
  mAllData->Branch("PhotECalIso",mTempTreePhotECalIso,"mTempTreePhotECalIso[Nphot]/double");
  mAllData->Branch("PhotHCalIso",mTempTreePhotHCalIso,"mTempTreePhotHCalIso[Nphot]/double");
  mAllData->Branch("PhotAllIso",mTempTreePhotAllIso,"mTempTreePhotAllIso[Nphot]/double");

  //add electrons
  mAllData->Branch("Nelec" ,&mTempTreeNelec ,"Nelec/int");  
  mAllData->Branch("ElecE" ,mTempTreeElecE ,"ElecE[Nelec]/double");
  mAllData->Branch("ElecEt",mTempTreeElecEt,"ElecEt[Nelec]/double");
  mAllData->Branch("Elecpt",mTempTreeElecPt,"Elecpt[Nelec]/double");
  mAllData->Branch("Elecpx",mTempTreeElecPx,"Elecpx[Nelec]/double");
  mAllData->Branch("Elecpy",mTempTreeElecPy,"Elecpy[Nelec]/double");
  mAllData->Branch("Elecpz",mTempTreeElecPz,"Elecpz[Nelec]/double");
  mAllData->Branch("Eleceta",mTempTreeElecEta,"Eleceta[Nelec]/double");
  mAllData->Branch("Elecphi",mTempTreeElecPhi,"Elecphi[Nelec]/double");
  mAllData->Branch("ElecCharge",mTempTreeElecCharge,"ElecCharge[Nelec]/double");
  mAllData->Branch("ElecTrkIso",mTempTreeElecTrkIso,"ElecTrkIso[Nelec]/double");
  mAllData->Branch("ElecECalIso", mTempTreeElecECalIso,"ElecECalIso[Nelec]/double");
  mAllData->Branch("ElecHCalIso", mTempTreeElecHCalIso ,"ElecHCalIso[Nelec]/double");
  mAllData->Branch("ElecAllIso",  mTempTreeElecAllIso ,"ElecAllIso[Nelec]/double");
  mAllData->Branch("ElecTrkChiNorm",mTempTreeElecTrkChiNorm  ,"ElecTrkChiNorm[Nelec]/double");



  //add muons
  mAllData->Branch("Nmuon" ,&mTempTreeNmuon ,"Nmuon/int");  
  mAllData->Branch("MuonE" ,mTempTreeMuonE ,"MuonE[Nmuon]/double");
  mAllData->Branch("MuonEt",mTempTreeMuonEt,"MuonEt[Nmuon]/double");
  mAllData->Branch("Muonpt",mTempTreeMuonPt,"Muonpt[Nmuon]/double");
  mAllData->Branch("Muonpx",mTempTreeMuonPx,"Muonpx[Nmuon]/double");
  mAllData->Branch("Muonpy",mTempTreeMuonPy,"Muonpy[Nmuon]/double");
  mAllData->Branch("Muonpz",mTempTreeMuonPz,"Muonpz[Nmuon]/double");
  mAllData->Branch("Muoneta",mTempTreeMuonEta,"Muoneta[Nmuon]/double");
  mAllData->Branch("Muonphi",mTempTreeMuonPhi,"Muonphi[Nmuon]/double");
  mAllData->Branch("MuonCharge",mTempTreeMuonCharge,"MuonCharge[Nmuon]/double");
  mAllData->Branch("MuonTrkIso",mTempTreeMuonTrkIso,"MuonTrkIso[Nmuon]/double");
  mAllData->Branch("MuonECalIso", mTempTreeMuonECalIso,"MuonECalIso[Nmuon]/double");
  mAllData->Branch("MuonHCalIso", mTempTreeMuonHCalIso ,"MuonHCalIso[Nmuon]/double");
  mAllData->Branch("MuonAllIso",  mTempTreeMuonAllIso ,"MuonAllIso[Nmuon]/double");
  mAllData->Branch("MuonTrkChiNorm",mTempTreeMuonTrkChiNorm  ,"MuonTrkChiNorm[Nmuon]/double");


  //add taus
  mAllData->Branch("Ntau" ,&mTempTreeNtau ,"Ntau/int");  
  mAllData->Branch("TauE" ,mTempTreeTauE ,"TauE[Ntau]/double");
  mAllData->Branch("TauEt",mTempTreeTauEt,"TauEt[Ntau]/double");
  mAllData->Branch("Taupt",mTempTreeTauPt,"Taupt[Ntau]/double");
  mAllData->Branch("Taupx",mTempTreeTauPx,"Taupx[Ntau]/double");
  mAllData->Branch("Taupy",mTempTreeTauPy,"Taupy[Ntau]/double");
  mAllData->Branch("Taupz",mTempTreeTauPz,"Taupz[Ntau]/double");
  mAllData->Branch("Taueta",mTempTreeTauEta,"Taueta[Ntau]/double");
  mAllData->Branch("Tauphi",mTempTreeTauPhi,"Tauphi[Ntau]/double");
  mAllData->Branch("TauTrkIso",mTempTreeTauTrkIso,"TauTrkIso[Ntau]/double");
  mAllData->Branch("TauECalIso", mTempTreeTauECalIso,"TauECalIso[Ntau]/double");
  mAllData->Branch("TauHCalIso", mTempTreeTauHCalIso ,"TauHCalIso[Ntau]/double");
  mAllData->Branch("TauAllIso",  mTempTreeTauAllIso ,"TauAllIso[Ntau]/double");


    mAllData->Branch("genN",&length,"genN/int");
    mAllData->Branch("genid",ids,"ids[genN]/int");
    mAllData->Branch("genMother",refs,"refs[genN]/int");
    mAllData->Branch("genPhi",genPhi,"genPhi[genN]/float");
    mAllData->Branch("genPt",genPt,"genPt[genN]/float");
    mAllData->Branch("genEta",genEta,"genEta[genN]/float");
    mAllData->Branch("genStatus",genStatus,"genStatus[genN]/int");


  // add test stuff
 
  mAllData->Branch("AlpPtScale" ,&mTempAlpPtScale,"mTempAlpPtScale/double");
  mAllData->Branch("AlpIdTest" ,&mTempAlpIdTest ,"AlpIdTest/int");  
  mAllData->Branch("MuonPairMass" ,&mTempMuonPairMass,"MuonPairMass/double");
  mAllData->Branch("MuonPairIndex" ,&mTempMuonPairIndex,"MuonPairIndex[2]/int");
 
  edm::LogInfo("SusyDiJet") << "Ntuple variables " << variables.str();

}


//________________________________________________________________________________________
void
SusyDiJetAnalysis::fillPlots( const edm::Event& iEvent, 
    const SelectorDecisions& decisions ) {
  
  // Container array
  float* x = new float[ntuple_->GetNbranches()];
  int ivar = 0; 

  // 1. Event variables
  x[ivar++] = weight_;
  x[ivar++] = processId_;

  // 2. Decision from all selectors
  for ( size_t i=0; i<sequence_.size(); ++i ) x[ivar++] = decisions.decision(i);
  x[ivar++] = decisions.globalDecision();

  // 3. All variables from sequence
  std::vector<double> values = sequence_.values();
  for ( size_t i=0; i<values.size(); ++i ) x[ivar++] = values[i];

  if ( ntuple_->Fill( x ) < 0 ) { // Fill returns number of bytes committed, -1 on error
    edm::LogWarning("SusyDiJet") << "@SUB=fillPlots()" << "Problem filling ntuple";
  }

  delete [] x; // Important! otherwise we'll leak...

}

//________________________________________________________________________________________
// Define this as a plug-in
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(SusyDiJetAnalysis);
