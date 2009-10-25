#include "TauAnalysis/RecoTools/plugins/TriggerResultEventSelector.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/TriggerNames.h"

#include "DataFormats/Common/interface/TriggerResults.h"

TriggerResultEventSelector::TriggerResultEventSelector(const edm::ParameterSet& cfg)
{ 
  //std::cout << "<TriggerResultEventSelector::TriggerResultEventSelector>:" << std::endl;

  src_ = cfg.getParameter<edm::InputTag>("src");
  //std::cout << " src = " << src_ << std::endl;

  triggerPaths_ = cfg.getParameter<vstring>("triggerPaths");
  //std::cout << " triggerPaths = " << triggerPaths_ << std::endl;
}

bool TriggerResultEventSelector::operator()(edm::Event& evt, const edm::EventSetup&)
{
  edm::Handle<edm::TriggerResults> triggerResults;
  evt.getByLabel(src_, triggerResults);

  edm::TriggerNames triggerNames;
  triggerNames.init(*triggerResults);

  /*
    for ( edm::TriggerNames::Strings::const_iterator triggerName = triggerNames.triggerNames().begin();
          triggerName != triggerNames.triggerNames().end(); ++triggerName ) {
      unsigned int index = triggerNames.triggerIndex(*triggerName);
      if ( index < triggerNames.size() ) {
        std::string triggerDecision = ( triggerResults->accept(index) ) ? "passed" : "failed";
    
	std::cout << " triggerName = " << (*triggerName) << " " << triggerDecision << std::endl;
      }
    }
  */

  for ( vstring::const_iterator triggerPath = triggerPaths_.begin();
	triggerPath != triggerPaths_.end(); ++triggerPath ) {
    unsigned int index = triggerNames.triggerIndex(*triggerPath);
    if ( index < triggerNames.size() ) {
//--- event triggered by triggerPath
      if ( triggerResults->accept(index) ) return true;
    } else {
      edm::LogError ("TriggerResultEventSelector::operator()") << " Undefined trigger Path = " << (*triggerPath) << " --> skipping !!";
      continue;
    }
  }

//--- none of the triggers specified in the triggerPaths list triggered the event
  return false;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(EventSelectorPluginFactory, TriggerResultEventSelector, "TriggerResultEventSelector");      

#include "PhysicsTools/UtilAlgos/interface/EventSelectorAdapter.h"

typedef EventSelectorAdapter<TriggerResultEventSelector> TriggerResultFilter;

DEFINE_ANOTHER_FWK_MODULE(TriggerResultFilter);
