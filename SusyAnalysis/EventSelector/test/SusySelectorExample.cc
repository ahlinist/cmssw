#include "SusyAnalysis/EventSelector/test/SusySelectorExample.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "SusyAnalysis/EventSelector/interface/SusyEventSelector.h"

#include <iostream>
#include <sstream>

SusySelectorExample::SusySelectorExample (const edm::ParameterSet& iConfig) :
  nrEventTotal_(0), nrEventSelected_(0), 
  selectors_( iConfig.getParameter<edm::ParameterSet>("selections") ) {
  // should change to LogInfo ...
  std::ostringstream msg;
  msg << "Selectors are" << std::endl;
  std::vector<std::string> names = selectors_.selectorNames();
  for ( size_t i=0; i<nrOfSelectors(); ++i ) msg << "  " << names[i];
  msg << std::endl;
  msg << "Variable names are " << selectors_.numberOfVariables() << std::endl;
  selVarNames_ = selectors_.variableNames();
  for ( std::vector<std::string>::const_iterator iv=selVarNames_.begin();
	iv!=selVarNames_.end(); ++iv )  msg << " " << *iv;
  edm::LogInfo("SusySelectorExample") << msg.str();


}

SusySelectorExample::~SusySelectorExample() {}

void
SusySelectorExample::analyze (const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  //
  // retrieve the decision of each selector module
  //
  SelectorDecisions decisions = selectors_.decisions(iEvent);
  //
  // count all events / events passing all selections
  //
  ++nrEventTotal_;
  std::ostringstream dbg;
  dbg << "selector decisions " << std::endl
      << "  name, 2xdecision, 2xcompl. decision, cumul. decision" << std::endl;
  bool dec(true);
  for ( size_t i=0; i<nrOfSelectors(); ++i ) {
    std::string name = selectors_.selectorName(i);
    size_t idxFromName = selectors_.selectorIndex(name);
    dec = dec && decisions.decision(i);
    dbg << " " << name
	<< " " << decisions.decision(i)
	<< " " << decisions.decision(idxFromName)
	<< " " << decisions.complementaryDecision(i)
	<< " " << decisions.complementaryDecision(idxFromName)
	<< " " << decisions.cumulativeDecision(i)
	<< " " << decisions.cumulativeDecision(idxFromName)
	<< " " << dec << std::endl;
  }
  dbg << " global decision = " << decisions.globalDecision();
  edm::LogVerbatim("SusySelectorExample") << "SusySelectorExample: " << dbg.str();
  if ( !decisions.globalDecision() )  return;
  ++nrEventSelected_;
  //
  // Access to cached variables (all and a specific one)
  //
  std::ostringstream dbg1;
  std::vector<double> vars = selectors_.values();
  dbg1 << "All variable values" << std::endl;
  for ( size_t i=0; i<selVarNames_.size(); ++i ) {
    dbg1 << selVarNames_[i] << " " << vars[i] << "\n";
  }
  dbg1 << "Specific variable: " << "JetEt:Jet0Et"
       << selectors_.value("JetEt","Jet0Et");
  edm::LogVerbatim("SusySelectorExample") << dbg1.str();
}


void 
SusySelectorExample::beginJob(const edm::EventSetup&) {}

void 
SusySelectorExample::endJob() 
{
  std::ostringstream msg;
  msg << "Total number of events = " << nrEventTotal_
      << " ; selected = " << nrEventSelected_;
  edm::LogInfo("SusySelectorExample") << msg.str();
}

//define this as a plug-in
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(SusySelectorExample);
