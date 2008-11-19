// -*- C++ -*-
//
// Package:    SusyAnalysisSkeleton
// Class:      SusyAnalysisSkeleton
// 
/**\class SusyAnalysisSkeleton SusyAnalysisSkeleton.cc SusyAnalysis/SusyAnalysisSkeleton/src/SusyAnalysisSkeleton.cc

 Description: Skeleton analysis for SUSY searches

 Implementation:

 Based on the EventSelector interface for event selection.
 Stores data in the event for edm::ntuple output.

*/
//
// Original Author:  Frederic Ronga
//         Created:  Thu Aug 28 14:37:09 CEST 2008
// $Id$
//
//


// System include files
#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

// Framework include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

// SUSY include files
#include "SusyAnalysis/EventSelector/interface/SusyEventSelector.h"
#include "SusyAnalysis/EventSelector/interface/SelectorSequence.h"


//
// Class declaration
//

class SusyAnalysisSkeleton : public edm::EDFilter {
public:
  explicit SusyAnalysisSkeleton(const edm::ParameterSet&);
  ~SusyAnalysisSkeleton();

private:
  // *** CMSSW interface
  /// Called once per job, at start
  virtual void beginJob(const edm::EventSetup&);
  /// Called for each event: returns the global decision
  virtual bool filter(edm::Event&, const edm::EventSetup&);
  /// Called once per job, at end
  virtual void endJob();
      
  /// Print a summary of counts for all selectors
  virtual void printSummary(void);

  // *** Output
  /// Declare all variables to store
  virtual void declareOutput(void);
  /// Put all variables in the event
  virtual void fillEvent(edm::Event&, const SelectorDecisions& );
  

private:
  // *** Data memebers
  
    // Selection
  SelectorSequence sequence_;                  ///< Interface to selectors
  std::vector<std::string> filterSelection_;   ///< Container for filter selection (i.e., hard cuts)
  std::vector<size_t> filterSelectionIndices_; ///< Selector indices for filter selection

  // Event information
  double eventWeight_;         ///< Event weight from config. file (if <0, get it from event)
  int    processId_;           ///< CSA07 generator process ID
  int    run_, event_;         ///< Store run and event number

  // Counters
  unsigned int nrEventTotalRaw_;          ///< Raw number of events (+1 at each event)
  double nrEventTotalWeighted_;           ///< Weighted #(events)
  std::vector<float> nrEventSelected_;    ///< Selected #(events) for each module
  std::vector<float> nrEventAllButOne_;   ///< All-but-one selected #(events) for each module
  std::vector<float> nrEventCumulative_;  ///< Cumulative selected #(events) for each module

  // Output
  float* variables_;      ///< Container for the tree variables (from selectors)
  bool   globalDecision_; ///< Global decision for event

};


//________________________________________________________________________________________
SusyAnalysisSkeleton::SusyAnalysisSkeleton(const edm::ParameterSet& iConfig) :
  sequence_( iConfig.getParameter<edm::ParameterSet>("selections") ),
  filterSelection_( iConfig.getParameter<std::vector<std::string> >("filterSelection") ),
  eventWeight_( iConfig.getParameter<double>("eventWeight") ),
  nrEventTotalRaw_(0), nrEventTotalWeighted_(0.0)
{
 
  // Translate filterSelection strings into selector indices
  filterSelectionIndices_.reserve(filterSelection_.size());
  for ( size_t i=0; i<filterSelection_.size(); ++i )
    filterSelectionIndices_.push_back(sequence_.selectorIndex(filterSelection_[i]));

  // List all selectors and selection variables
  edm::LogVerbatim("SusyAnalysisSkeleton") << "Selectors are:" << std::endl;
  for ( std::vector<const SusyEventSelector*>::const_iterator it = sequence_.selectors().begin();
        it != sequence_.selectors().end(); ++it )
    {
      edm::LogVerbatim("SusyAnalysisSkeleton") << " * " << (*it)->name()
                                          << " selects on following " 
                                          << (*it)->numberOfVariables() << " variable(s):";
      for ( unsigned int i=0; i<(*it)->numberOfVariables(); ++i )
        edm::LogVerbatim("SusyAnalysisSkeleton") << "    - " << (*it)->variableNames()[i];
      edm::LogVerbatim("SusyAnalysisSkeleton") << std::endl;
    }

  // Say something about event weights
  edm::LogInfo("SusyAnalysisSkeleton") << "Global event weight set to " << eventWeight_;
  
  // Initialise counters
  nrEventSelected_.resize( sequence_.size(), 0.0 );
  nrEventAllButOne_.resize( sequence_.size(), 0.0 );
  nrEventCumulative_.resize( sequence_.size(), 0.0 );

  // Define output variables
  declareOutput();

}


//________________________________________________________________________________________
SusyAnalysisSkeleton::~SusyAnalysisSkeleton() { }


// ------------ method called on each new Event  ------------
bool
SusyAnalysisSkeleton::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  // Get some event information
  run_   = iEvent.id().run();
  event_ = iEvent.id().event();
  processId_   = 0;

  // Retrieve the decision of each selector module
  SelectorDecisions decisions = sequence_.decisions(iEvent);

  // Count all events
  nrEventTotalRaw_++;
  nrEventTotalWeighted_ += eventWeight_;

  // Update counters 
  for ( size_t i=0; i<sequence_.size(); ++i ) {
    if ( decisions.decision(i) )              nrEventSelected_[i]   += eventWeight_;
    if ( decisions.complementaryDecision(i) ) nrEventAllButOne_[i]  += eventWeight_;
    if ( decisions.cumulativeDecision(i) )    nrEventCumulative_[i] += eventWeight_;
  }

  // Fill event with variables we computed
  fillEvent( iEvent, decisions );

  // Print summary so far (every 10 till 100, every 100 till 1000, etc.)
  for ( unsigned int i=10; i<nrEventTotalRaw_; i*=10 )
    if ( nrEventTotalRaw_<=10*i && (nrEventTotalRaw_%i)==0 )
      printSummary();
  
  return globalDecision_;

}

//________________________________________________________________________________________
// Called once per job, at start
void 
SusyAnalysisSkeleton::beginJob(const edm::EventSetup&) {}

//________________________________________________________________________________________
// Called once per job, at end
// Prints summary of counters
void 
SusyAnalysisSkeleton::endJob() 
{
  printSummary();
}

//________________________________________________________________________________________
// Print out summary of counters
void
SusyAnalysisSkeleton::printSummary( void ) {

  edm::LogInfo("SusyAnalysisSkeleton|SummaryCount") << "*** Summary of counters: ";
  edm::LogVerbatim("SusyAnalysisSkeleton|SummaryCount") 
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
  edm::LogVerbatim("SusyAnalysisSkeleton|SummaryCount") << summary.str();

}

//________________________________________________________________________________________
// Declares the output
void
SusyAnalysisSkeleton::declareOutput() {
  
  // 1. Decision from all selectors
  // Also store global decision
  produces<bool>("all.result").setBranchAlias("all.result");
  for ( size_t i=0; i<sequence_.size(); ++i ) {
    std::string var( sequence_.selectorNames()[i] );
    var += ".result";
    // Push to list of variables (add  type identifier or list separator)
    produces<bool>(var).setBranchAlias(var);
  }
  
  // 2. All variables from sequence
  std::ostringstream variableNames; // Container for definition
  for ( std::vector<const SusyEventSelector*>::const_iterator it = sequence_.selectors().begin();
        it != sequence_.selectors().end(); ++it ) {
    for ( unsigned int i=0; i<(*it)->numberOfVariables(); ++i ) {
      std::string var( (*it)->name() );       // prefix variable with selector name: 
      var += "." + (*it)->variableNames()[i]; // format is "selectorName.varName"
      produces<double>(var).setBranchAlias(var);
      variableNames << " " << var << std::endl;
    }
  }

  // Print some information about what we store
  edm::LogInfo("SusyAnalysisSkeleton") << "Ntuple variables " << variableNames.str();

}


//________________________________________________________________________________________
void SusyAnalysisSkeleton::fillEvent(edm::Event& evt, const SelectorDecisions& decisions ) {

  // 1. Decision from all selectors (and global decision)
  std::auto_ptr<bool> global_dec( new bool(false) );
  *global_dec = decisions.globalDecision();
  evt.put( global_dec, "all.result" );

  for ( size_t i=0; i<sequence_.size(); ++i ) 
    {
      std::string var( sequence_.selectorNames()[i] );
      var += ".result";
      std::auto_ptr<bool> idec( new bool(false) );
      *idec = decisions.decision(i);
      evt.put( idec, var );
    }

  // 2. All variables from sequence: only if some selections passed, as configured
  bool dec(true);
  for ( size_t i=0; i<filterSelectionIndices_.size(); ++i ) // only "filter selection"
    dec = dec && decisions.decision(filterSelectionIndices_[i]);

  for ( std::vector<const SusyEventSelector*>::const_iterator it = sequence_.selectors().begin();
        it != sequence_.selectors().end(); ++it )
    for ( unsigned int i=0; i<(*it)->numberOfVariables(); ++i ) 
      {
        std::string var( (*it)->name() );       // prefix variable with selector name: 
        var += "." + (*it)->variableNames()[i]; // format is "selectorName.varName"
        std::auto_ptr<double> value( new double() );
        *value = (*it)->values()[i];
        evt.put( value, var );
      }
}


//define this as a plug-in
DEFINE_FWK_MODULE(SusyAnalysisSkeleton);
