#include "TauAnalysis/Core/plugins/OrEventSelector.h"

OrEventSelector::OrEventSelector(const edm::ParameterSet& cfg)
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

OrEventSelector::~OrEventSelector()
{
  for ( std::vector<EventSelectorBase*>::iterator selector = selectors_.begin();
	selector != selectors_.end(); ++selector ) {
    delete (*selector);
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(EventSelectorPluginFactory, OrEventSelector, "OrEventSelector");
