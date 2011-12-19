#include "TauAnalysis/RecoTools/plugins/TriggerResultEventSelector.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Common/interface/TriggerNames.h"

#include "DataFormats/Common/interface/TriggerResults.h"

TriggerResultEventSelector::TriggerResultEventSelector(const edm::ParameterSet& cfg)
{ 
  //std::cout << "<TriggerResultEventSelector::TriggerResultEventSelector>:" << std::endl;

  src_ = cfg.getParameter<edm::InputTag>("src");
  //std::cout << " src = " << src_ << std::endl;

  triggerPaths_ = cfg.getParameter<vstring>("triggerPaths");
  //std::cout << " triggerPaths = " << triggerPaths_ << std::endl;
  
  if ( cfg.exists("triggerRunConfig") ) {
    typedef std::vector<edm::ParameterSet> vParameterSet;
    vParameterSet conf = cfg.getParameter<vParameterSet>("triggerRunConfig");
    for ( vParameterSet::const_iterator it = conf.begin(); 
	  it != conf.end(); it++ ){
      TriggerConfig trg;
      trg.first = it->getParameter<int>("firstRun");
      trg.last = it->getParameter<int>("lastRun");
      if ( it->exists("src") )
	trg.src = it->getParameter<edm::InputTag>("src");
      else
	trg.src = src_;
      if ( it->exists("triggerPaths") )
	trg.paths = it->getParameter<vstring>("triggerPaths");
      else
	trg.paths = triggerPaths_;
      triggerRunConfig_.push_back(trg);
    }
    std::cout << "Trigger configurations" << std::endl;
    for ( std::vector<TriggerConfig>::const_iterator it = triggerRunConfig_.begin();
	  it != triggerRunConfig_.end(); it++ ) {
      std::cout << "Runs " << it->first << "-" << it->last << std::endl;
      std::cout << "\tcollection = " << it->src << std::endl;
      std::cout << "\ttrigger paths: " << std::endl;
      for ( vstring::const_iterator strIt = it->paths.begin(); strIt != it->paths.end(); strIt++ )
	std::cout << "\t\t" << *strIt << std::endl;
    }
  }
}

bool TriggerResultEventSelector::operator()(edm::Event& evt, const edm::EventSetup&)
{
  if ( triggerRunConfig_.size() > 0 ) {
    for ( std::vector<TriggerConfig>::const_iterator it = triggerRunConfig_.begin();
	  it != triggerRunConfig_.end(); it++) {
      if( ((int)evt.run() <= it->last) && ((int)evt.run() >= it->first) ) {
	src_ = it->src;
	triggerPaths_ = it->paths;
      }
    }
  }

  edm::Handle<edm::TriggerResults> triggerResults;
  evt.getByLabel(src_, triggerResults);
  
  const edm::TriggerNames& triggerNames = evt.triggerNames(*triggerResults);
  
  bool foundATrigger = 0;
  for ( vstring::const_iterator triggerPath = triggerPaths_.begin();
	triggerPath != triggerPaths_.end(); ++triggerPath ) {
    unsigned int index = triggerNames.triggerIndex(*triggerPath);
    if ( index < triggerNames.size() ) {
      foundATrigger = 1;
//--- event triggered by triggerPath
      if ( triggerResults->accept(index) ) return true;
    } 
  }
  
  if( !foundATrigger ) { // none of the specified trigger paths was found in the event!
    edm::LogError ("TriggerResultEventSelector::operator()") << " Found none of requested trigger paths!!";
    std::cout << "Available trigger Paths:" << std::endl;
    for ( edm::TriggerNames::Strings::const_iterator triggerName = triggerNames.triggerNames().begin();
	  triggerName != triggerNames.triggerNames().end(); ++triggerName ) {
      unsigned int index = triggerNames.triggerIndex(*triggerName);
      if ( index < triggerNames.size() ) {
	std::string triggerDecision = ( triggerResults->accept(index) ) ? "passed" : "failed";
	std::cout << " triggerName = " << (*triggerName) << " " << triggerDecision << std::endl;
      }
    }
  }

//--- none of the triggers specified in the triggerPaths list triggered the event
  return false;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(EventSelectorPluginFactory, TriggerResultEventSelector, "TriggerResultEventSelector");      

#include "CommonTools/UtilAlgos/interface/EventSelectorAdapter.h"

typedef EventSelectorAdapter<TriggerResultEventSelector> TriggerResultFilter;

DEFINE_FWK_MODULE(TriggerResultFilter);
