#include "TauAnalysis/Core/plugins/NotEventSelector.h"

NotEventSelector::NotEventSelector(const edm::ParameterSet& cfg)
{
  edm::ParameterSet cfgSelector = cfg.getParameter<edm::ParameterSet>("selector");
  std::string selectorType = cfg.getParameter<std::string>("pluginType");
  selector_ = EventSelectorPluginFactory::get()->create(selectorType, cfg);
}

NotEventSelector::~NotEventSelector()
{    
  delete selector_;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(EventSelectorPluginFactory, NotEventSelector, "NotEventSelector");
