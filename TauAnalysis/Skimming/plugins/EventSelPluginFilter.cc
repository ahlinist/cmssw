#include "TauAnalysis/Skimming/plugins/EventSelPluginFilter.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

EventSelPluginFilter::EventSelPluginFilter(const edm::ParameterSet& cfg)
{
  //std::cout << "<EventSelPluginFilter::EventSelPluginFilter>:" << std::endl;

  edm::ParameterSet cfgEventSelector = cfg.getParameter<edm::ParameterSet>("selector");

  std::string pluginTypeEventSelector = cfgEventSelector.getParameter<std::string>("pluginType");

  eventSelector_ = EventSelectorPluginFactory::get()->create(pluginTypeEventSelector, cfgEventSelector);
}

EventSelPluginFilter::~EventSelPluginFilter()
{
  delete eventSelector_;
}

bool EventSelPluginFilter::filter(edm::Event& evt, const edm::EventSetup& es)
{
  if ( eventSelector_ ) {
    return (*eventSelector_)(evt, es); 
  } else {
    edm::LogError ("filter") << " Invalid event Selector --> skipping !!";
    return false;
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(EventSelPluginFilter);

