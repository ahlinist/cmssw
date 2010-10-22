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
}

bool TriggerResultEventSelector::operator()(edm::Event& evt, const edm::EventSetup&)
{
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

  if( ! foundATrigger) { // none of the specified trigger paths was found in the event!
    
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
