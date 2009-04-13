#include "TauAnalysis/RecoTools/plugins/BoolEventSelFlagProducer.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

BoolEventSelFlagProducer::BoolEventSelFlagProducer(const edm::ParameterSet& cfg)
{ 
  //std::cout << "<BoolEventSelFlagProducer::BoolEventSelFlagProducer>:" << std::endl;

  std::string eventSelectorType = cfg.getParameter<std::string>("pluginType");
  eventSelector_ = EventSelectorPluginFactory::get()->create(eventSelectorType, cfg);

//--- declare production of single boolean flag
  produces<bool>();
}

BoolEventSelFlagProducer::~BoolEventSelFlagProducer()
{
  delete eventSelector_;
}

void BoolEventSelFlagProducer::produce(edm::Event& evt, const edm::EventSetup& es)
{
  if ( !eventSelector_ ) {
    edm::LogError ("BoolEventSelFlagProducer::produce") << " EventSelectorBase not initialized !!";
    return;
  }

//--- evaluate whether event passes or fails the event selection
  bool booleanFlag = (*eventSelector_)(evt, es);

  std::auto_ptr<bool> booleanFlagPtr(new bool(booleanFlag));

//--- add boolean flag to the event
  evt.put(booleanFlagPtr);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(BoolEventSelFlagProducer);
