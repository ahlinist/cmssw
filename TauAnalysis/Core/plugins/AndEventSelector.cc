#include "TauAnalysis/Core/plugins/AndEventSelector.h"

#include <string>

AndEventSelector::AndEventSelector(const edm::ParameterSet& cfg)
{
  typedef std::vector<edm::ParameterSet> vParameterSet;
  vParameterSet cfgSelectors = cfg.getParameter<vParameterSet>("selectors");
  for ( vParameterSet::const_iterator cfgSelector = cfgSelectors.begin(); 
	cfgSelector != cfgSelectors.end(); ++cfgSelector ) {
    std::string selectorType = cfgSelector->getParameter<std::string>("pluginType");
    EventSelectorBase* selector = EventSelectorPluginFactory::get()->create(selectorType, *cfgSelector);
    selectors_.push_back(selector);
  }
}

AndEventSelector::~AndEventSelector()
{
  for ( std::vector<EventSelectorBase*>::iterator selector = selectors_.begin();
	selector != selectors_.end(); ++selector ) {
    delete (*selector);
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(EventSelectorPluginFactory, AndEventSelector, "AndEventSelector");
