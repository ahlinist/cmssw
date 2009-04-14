#include "TauAnalysis/RecoTools/plugins/BoolEventSelFlagProducer.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

BoolEventSelFlagProducer::BoolEventSelFlagProducer(const edm::ParameterSet& cfg)
{ 
  //std::cout << "<BoolEventSelFlagProducer::BoolEventSelFlagProducer>:" << std::endl;

//--- check if configuration parameters contain only one
//    or multiple definitions of plugins derrived from EventSelectorBase
  if ( cfg.exists("selectors") ) { // multiple plugin case
    typedef std::vector<edm::ParameterSet> vParameterSet;
    vParameterSet cfgSelectors = cfg.getParameter<vParameterSet>("selectors");
    for ( vParameterSet::const_iterator cfgSelector = cfgSelectors.begin(); 
	  cfgSelector != cfgSelectors.end(); ++cfgSelector ) {
      eventSelectorEntry eventSelector;
      eventSelector.instanceName_ = cfgSelector->getParameter<std::string>("instanceName");
      std::string eventSelectorType = cfgSelector->getParameter<std::string>("pluginType");
      eventSelector.plugin_ = EventSelectorPluginFactory::get()->create(eventSelectorType, *cfgSelector);
      eventSelectors_.push_back(eventSelector);

//--- declare production of single boolean flag
//    with instance label set to 'instanceName' configuration parameter
      produces<bool>(eventSelector.instanceName_);
    }
  } else { // single plugin case
    eventSelectorEntry eventSelector;
    std::string eventSelectorType = cfg.getParameter<std::string>("pluginType");
    eventSelector.plugin_ = EventSelectorPluginFactory::get()->create(eventSelectorType, cfg);
    eventSelectors_.push_back(eventSelector);

//--- declare production of single boolean flag
//    (without instance label)
    produces<bool>();
  }
}

BoolEventSelFlagProducer::~BoolEventSelFlagProducer()
{
  for ( std::vector<eventSelectorEntry>::iterator it = eventSelectors_.begin();
	it != eventSelectors_.end(); ++it ) {
    delete it->plugin_;
  }
}

void BoolEventSelFlagProducer::produce(edm::Event& evt, const edm::EventSetup& es)
{
  for ( std::vector<eventSelectorEntry>::iterator eventSelector = eventSelectors_.begin();
	eventSelector != eventSelectors_.end(); ++eventSelector ) {
//--- evaluate whether event passes or fails the event selection
    bool booleanFlag = (*eventSelector->plugin_)(evt, es);
    
    std::auto_ptr<bool> booleanFlagPtr(new bool(booleanFlag));

//--- add boolean flag to the event
    if ( eventSelector->instanceName_ != "" ) 
      evt.put(booleanFlagPtr, eventSelector->instanceName_);
    else
      evt.put(booleanFlagPtr);
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(BoolEventSelFlagProducer);
