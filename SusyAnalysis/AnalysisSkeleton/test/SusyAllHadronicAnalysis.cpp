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
// $Id: SusyAllHadronicAnalysis.cpp,v 1.9 2008/06/20 12:35:36 fronga Exp $
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
  std::vector<std::string> plotSelection_; ///< Container for plotting selection
  std::vector<size_t> plotSelectionIndices_; ///< Selector indices for plotting selection

  // Event information
  double eventWeight_;  ///< Event weight from config. file (if <0, get it from event)
  edm::InputTag weightSource_; ///< Source for CSA07 event weight producer
  double weight_;       ///< Actual event weight (either config. or event)
  int    processId_;    ///< CSA07 generator process ID

  // Counters
  unsigned int nrEventTotalRaw_;          ///< Raw number of events (+1 at each event)
  double nrEventTotalWeighted_;           ///< Weighted #(events)
  std::vector<float> nrEventSelected_;    ///< Selected #(events) for each module
  std::vector<float> nrEventAllButOne_;   ///< All-but-one selected #(events) for each module
  std::vector<float> nrEventCumulative_;  ///< Cumulative selected #(events) for each module

  // Plots
  TNtuple* ntuple_; ///< Will contain all the information we want to keep

};

//________________________________________________________________________________________
SusyAllHadronicAnalysis::SusyAllHadronicAnalysis(const edm::ParameterSet& iConfig):
  sequence_( iConfig.getParameter<edm::ParameterSet>("selections") ),
  plotSelection_( iConfig.getParameter<std::vector<std::string> >("plotSelection") ),
  eventWeight_( iConfig.getParameter<double>("eventWeight") ),
  weightSource_(iConfig.getParameter<edm::InputTag>("weightSource") ),
  weight_(0.0), nrEventTotalRaw_(0), nrEventTotalWeighted_(0.0)
{
  // Translate plotSelection strings to indices
  plotSelectionIndices_.reserve(plotSelection_.size());
  for ( size_t i=0; i<plotSelection_.size(); ++i )
    plotSelectionIndices_.push_back(sequence_.selectorIndex(plotSelection_[i]));
    
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

  // Initialise plots [should improve in the future]
  initPlots();

}


//________________________________________________________________________________________
SusyAllHadronicAnalysis::~SusyAllHadronicAnalysis() {}


//________________________________________________________________________________________
// Method called to for each event
void
SusyAllHadronicAnalysis::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

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
    edm::LogVerbatim("SusyAllHadronicEvent")
      << " " << sequence_.selectorName(i)
      << " " << decisions.decision(i)
      << " " << decisions.complementaryDecision(i)
      << " " << decisions.cumulativeDecision(i)
      << " " << dec << std::endl;

    // Update counters
    if ( decisions.decision(i) ) nrEventSelected_[i] += weight_;
    if ( decisions.complementaryDecision(i) ) nrEventAllButOne_[i] += weight_;
    if ( decisions.cumulativeDecision(i) ) nrEventCumulative_[i] += weight_;
     
  }

  // Fill some plots (only if some selections passed, as configured)
  dec = true;
  for ( size_t i=0; i<plotSelectionIndices_.size(); ++i )
    dec = dec&&decisions.decision(plotSelectionIndices_[i]);
  if ( dec ) fillPlots( iEvent, decisions );

  // Print summary so far (every 10 till 100, every 100 till 1000, etc.)
  for ( unsigned int i=10; i<nrEventTotalRaw_; i*=10 )
    if ( nrEventTotalRaw_<=10*i && (nrEventTotalRaw_%i)==0 )
      printSummary();

}


//________________________________________________________________________________________
void 
SusyAllHadronicAnalysis::beginJob(const edm::EventSetup&) {}

//________________________________________________________________________________________
void 
SusyAllHadronicAnalysis::endJob() {

  printSummary();

}


//________________________________________________________________________________________
void
SusyAllHadronicAnalysis::printSummary( void ) {

  edm::LogInfo("SusyAllHadronic|SummaryCount") << "*** Summary of counters: ";
  edm::LogVerbatim("SusyAllHadronic|SummaryCount") 
    << "Total number of events = " << nrEventTotalWeighted_ 
    << " (" << nrEventTotalRaw_ << " unweighted)"
    << " ; selected = " << nrEventCumulative_.back()
    << " [" << std::setprecision(2)  << std::fixed 
    << nrEventCumulative_.back()/nrEventTotalWeighted_ >> "%]" << std::endl;

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
void
SusyAllHadronicAnalysis::initPlots() {

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
  ntuple_ = fs->make<TNtuple>( "ntuple","SusyAllHadronicAnalysis variables",
                               variables.str().c_str() );

  edm::LogInfo("SusyAllHadronic") << "Ntuple variables " << variables.str();

}


//________________________________________________________________________________________
void
SusyAllHadronicAnalysis::fillPlots( const edm::Event& iEvent, 
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
    edm::LogWarning("SusyAllHadronic") << "@SUB=fillPlots()" << "Problem filling ntuple";
  }

  delete [] x; // Important! otherwise we'll leak...

}

//________________________________________________________________________________________
// Define this as a plug-in
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(SusyAllHadronicAnalysis);
