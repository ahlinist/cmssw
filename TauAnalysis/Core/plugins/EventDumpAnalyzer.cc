#include "TauAnalysis/Core/plugins/EventDumpAnalyzer.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

EventDumpAnalyzer::EventDumpAnalyzer(const edm::ParameterSet& cfg)
{
  //std::cout << "<EventDumpAnalyzer::EventDumpAnalyzer>:" << std::endl;

//--- configure eventDumps
  edm::ParameterSet cfgPlugin = cfg.getParameter<edm::ParameterSet>("plugin");

  if ( cfg.exists("evtSelFlags") ) {
    evtSelFlags_ = cfg.getParameter<vInputTag>("evtSelFlags");
  } else {
    cfgPlugin.addParameter<bool>("triggerAlways", true);
  }

  std::string pluginType = cfgPlugin.getParameter<std::string>("pluginType");
  plugin_ = EventDumpPluginFactory::get()->create(pluginType, cfgPlugin);
}

EventDumpAnalyzer::~EventDumpAnalyzer()
{
//--- nothing to be done yet...
}

void EventDumpAnalyzer::analyze(const edm::Event& evt, const edm::EventSetup& es)
{
//--- check values of boolean event selection flags
  EventDumpBase::filterResults_type evtSelResults;
  for ( vInputTag::const_iterator evtSelFlag = evtSelFlags_.begin();
	evtSelFlag != evtSelFlags_.end(); ++evtSelFlag ) {
    edm::Handle<bool> evtSelResult;
    evt.getByLabel(*evtSelFlag, evtSelResult);

    evtSelResults.insert(std::pair<std::string, bool>(evtSelFlag->label(), *evtSelResult));
  }

  double eventWeight = 1.;

  plugin_->analyze(evt, es, evtSelResults, evtSelResults, eventWeight);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(EventDumpAnalyzer);
