// -*- C++ -*-
//
// Package:    SusyAllHadronicAnalysis
// Class:      SusyAllHadronicAnalysis
// 
/**\class SusyAllHadronicAnalysis SusyAllHadronicAnalysis.cc SusyAnalysis/AnalysisSkeleton/src/SusyAllHadronicAnalysis.cc

Description: Skeleton analysis for SUSY search with Jets + MET

Implementation:
Uses the EventSelector interface for event selection and TFileService for plotting.

*/
//
// Original Author:  Markus Stoye
//         Created:  Mon Feb 18 15:40:44 CET 2008
// $Id: SusyAllHadronicAnalysis.cpp,v 1.4 2008/07/30 13:35:42 fronga Exp $
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
#include <TTree.h>

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



//
// Class declaration
//
class SusyAllHadronicAnalysis : public edm::EDAnalyzer {
public:
  explicit SusyAllHadronicAnalysis(const edm::ParameterSet&);
  ~SusyAllHadronicAnalysis();
  
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
  std::vector<std::string> filterSelection_; ///< Container for filter selection (i.e., hard cuts)
  std::vector<size_t> filterSelectionIndices_; ///< Selector indices for filter selection

  // Event information
  double eventWeight_;  ///< Event weight from config. file (if <0, get it from event)
  edm::InputTag weightSource_; ///< Source for CSA07 event weight producer
  double weight_;       ///< Actual event weight (either config. or event)
  int    processId_;    ///< CSA07 generator process ID
  int    run_, event_;  ///< Store run and event number

  // Counters
  unsigned int nrEventTotalRaw_;          ///< Raw number of events (+1 at each event)
  double nrEventTotalWeighted_;           ///< Weighted #(events)
  std::vector<float> nrEventSelected_;    ///< Selected #(events) for each module
  std::vector<float> nrEventAllButOne_;   ///< All-but-one selected #(events) for each module
  std::vector<float> nrEventCumulative_;  ///< Cumulative selected #(events) for each module

  // Ntuple
  TTree* decisionTree_;  ///< Will contain all the decisions for ALL processed events
  TTree* selectionTree_; ///< Will contain all the information we want to keep
  float* variables_;     ///< Container for the tree variables (from selectors)
  bool*  decisions_;     ///< Container for all selector decisions
  bool   globalDec_;     ///< Global decision for event

};

//________________________________________________________________________________________
SusyAllHadronicAnalysis::SusyAllHadronicAnalysis(const edm::ParameterSet& iConfig):
  sequence_( iConfig.getParameter<edm::ParameterSet>("selections") ),
  filterSelection_( iConfig.getParameter<std::vector<std::string> >("filterSelection") ),
  eventWeight_( iConfig.getParameter<double>("eventWeight") ),
  weightSource_(iConfig.getParameter<edm::InputTag>("weightSource") ),
  weight_(0.0), nrEventTotalRaw_(0), nrEventTotalWeighted_(0.0)
{
  // Translate filterSelection strings to indices
  filterSelectionIndices_.reserve(filterSelection_.size());
  for ( size_t i=0; i<filterSelection_.size(); ++i )
    filterSelectionIndices_.push_back(sequence_.selectorIndex(filterSelection_[i]));
    
  // List all selectors and selection variables
  edm::LogVerbatim("SusyAllHadronic") << "Selectors are:" << std::endl;
  for ( std::vector<const SusyEventSelector*>::const_iterator it = sequence_.selectors().begin();
        it != sequence_.selectors().end(); ++it )
    {
      edm::LogVerbatim("SusyAllHadronic") << " * " << (*it)->name()
                                          << " selects on following " 
                                          << (*it)->numberOfVariables() << " variable(s):";
      for ( unsigned int i=0; i<(*it)->numberOfVariables(); ++i )
        edm::LogVerbatim("SusyAllHadronic") << "    - " << (*it)->variableNames()[i];
      edm::LogVerbatim("SusyAllHadronic") << std::endl;
    }

  // Say something about event weights
  if ( eventWeight_< 0. ) 
    edm::LogInfo("SusyAllHadronic") << "Will get event weights from event content";
  else
    edm::LogInfo("SusyAllHadronic") << "Global event weight set to " << eventWeight_;
  
  // Initialise counters
  nrEventSelected_.resize( sequence_.size(), 0.0 );
  nrEventAllButOne_.resize( sequence_.size(), 0.0 );
  nrEventCumulative_.resize( sequence_.size(), 0.0 );

  // Initialise plots
  initPlots();

}


//________________________________________________________________________________________
SusyAllHadronicAnalysis::~SusyAllHadronicAnalysis() {
  // Delete arrays we created
  delete [] decisions_;
  delete [] variables_;
}


//________________________________________________________________________________________
// Method called to for each event
void
SusyAllHadronicAnalysis::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  // Get some event information (process ID, weight)
  run_   = iEvent.id().run();
  event_ = iEvent.id().event();
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
  globalDec_ = true;
  for ( size_t i=0; i<sequence_.size(); ++i ) {
    globalDec_ = (globalDec_ && decisions.decision(i));

    // Update counters 
    if ( decisions.decision(i) )              nrEventSelected_[i]   += weight_;
    if ( decisions.complementaryDecision(i) ) nrEventAllButOne_[i]  += weight_;
    if ( decisions.cumulativeDecision(i) )    nrEventCumulative_[i] += weight_;

    // Store decision
    decisions_[i] = decisions.decision(i);
     
  }
  

  // Fill some plots (only if some selections passed, as configured)
  bool dec(true);
  for ( size_t i=0; i<filterSelectionIndices_.size(); ++i ) // only "filter selection"
    dec = dec && decisions.decision(filterSelectionIndices_[i]);
  if ( dec ) fillPlots( iEvent, decisions );

  // But also keep ALL decisions in a separate tree
  decisionTree_->Fill();

  // Print summary so far (every 10 till 100, every 100 till 1000, etc.)
  for ( unsigned int i=10; i<nrEventTotalRaw_; i*=10 )
    if ( nrEventTotalRaw_<=10*i && (nrEventTotalRaw_%i)==0 )
      printSummary();

}


//________________________________________________________________________________________
// Called once per job, at start
void 
SusyAllHadronicAnalysis::beginJob(const edm::EventSetup&) {}

//________________________________________________________________________________________
// Called once per job, at end
// Prints summary of counters
void 
SusyAllHadronicAnalysis::endJob() {

  printSummary();

}


//________________________________________________________________________________________
// Print out summary of counters
void
SusyAllHadronicAnalysis::printSummary( void ) {

  edm::LogInfo("SusyAllHadronic|SummaryCount") << "*** Summary of counters: ";
  edm::LogVerbatim("SusyAllHadronic|SummaryCount") 
    << "Total number of events = " << nrEventTotalWeighted_ 
    << " (" << nrEventTotalRaw_ << " unweighted)"
    << " ; selected = " << nrEventCumulative_.back()
    << " [" << std::setw(6) << std::setprecision(2)  << std::fixed 
    << (nrEventCumulative_.back()/nrEventTotalWeighted_)*100. << "%]" << std::endl;

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
  edm::LogVerbatim("SusyAllHadronic|SummaryCount") << summary.str();

}


//________________________________________________________________________________________
// Defines all tree branches
void
SusyAllHadronicAnalysis::initPlots() {

  // Register the tree
  edm::Service<TFileService> fs;
  selectionTree_ = fs->make<TTree>( "ntuple",       "Variables for pre-selected events" );
  decisionTree_  = fs->make<TTree>( "allDecisions", "ALL selector decisions" );

  // 1. Event variables
  // 1.a. for the tree with all variables
  selectionTree_->Branch("run",       &run_,       "run/I");
  selectionTree_->Branch("event",     &event_,     "event/I");
  selectionTree_->Branch("weight",    &weight_,    "weight/I");
  selectionTree_->Branch("processId", &processId_, "processId/I");

  // 1.b. for the tree with all decisions (of ALL events)
  decisionTree_->Branch("run",       &run_,       "run/I");
  decisionTree_->Branch("event",     &event_,     "event/I");
  decisionTree_->Branch("weight",    &weight_,    "weight/I");
  decisionTree_->Branch("processId", &processId_, "processId/I");
 
  // 2. Decision from all selectors
  // Also store global decision
  selectionTree_->Branch("all_result", &globalDec_, "all_result/O");
  decisionTree_->Branch("all_result", &globalDec_, "all_result/O");
  std::ostringstream variableNames; // Container for definition
  for ( std::vector<const SusyEventSelector*>::const_iterator it = sequence_.selectors().begin();
        it != sequence_.selectors().end(); ++it ) {
    std::string var( (*it)->name() );
    var += "_result";
    // Push to list of variables (add  type identifier or list separator)
    if ( it == sequence_.selectors().begin() ) variableNames << var << "/O";
    else variableNames << ":" << var;
  }
  decisions_ = new bool[sequence_.size()];  // Also store global decision
  selectionTree_->Branch("decisions", &decisions_[0], variableNames.str().c_str() );
  decisionTree_->Branch("decisions", &decisions_[0], variableNames.str().c_str() );

  // 3. All variables from sequence
  variableNames.str("");
  for ( std::vector<const SusyEventSelector*>::const_iterator it = sequence_.selectors().begin();
        it != sequence_.selectors().end(); ++it ) {
    for ( unsigned int i=0; i<(*it)->numberOfVariables(); ++i ) {
      std::string var( (*it)->name() );       // prefix variable with selector name: 
      var += "." + (*it)->variableNames()[i]; // format is "selectorName.varName"
      // Push to list of variables (add  type identifier or list separator)
      if ( it == sequence_.selectors().begin() && i==0 ) variableNames << var << "/F";
      else variableNames << ":" << var;
    }
  }
  variables_ = new float[sequence_.numberOfVariables()];
  selectionTree_->Branch("variables", &variables_[0], variableNames.str().c_str());

  // 4. Now we could add other user variables here...

  edm::LogInfo("SusyAllHadronic") << "Ntuple variables " << variableNames.str();

}


//________________________________________________________________________________________
void
SusyAllHadronicAnalysis::fillPlots( const edm::Event& iEvent, 
				    const SelectorDecisions& decisions ) {
  
  // Get all variables from sequence
  std::vector<double> values = sequence_.values();
  for ( size_t i=0; i<values.size(); ++i ) variables_[i] = values[i];

  if ( selectionTree_->Fill() < 0 ) { // Fill returns number of bytes committed, -1 on error
    edm::LogWarning("SusyAllHadronic") << "@SUB=fillPlots()" << "Problem filling ntuple";
  }

}

//________________________________________________________________________________________
// Define this as a plug-in
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(SusyAllHadronicAnalysis);
