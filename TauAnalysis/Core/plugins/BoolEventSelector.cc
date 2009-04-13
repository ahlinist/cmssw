#include "TauAnalysis/Core/plugins/BoolEventSelector.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

BoolEventSelector::BoolEventSelector(const edm::ParameterSet& cfg)
{ 
  //std::cout << "<BoolEventSelector::BoolEventSelector>:" << std::endl;

  src_ = cfg.getParameter<edm::InputTag>("src");
  //std::cout << " src = " << src_ << std::endl;
}

bool BoolEventSelector::operator()(edm::Event& evt, const edm::EventSetup&)
{
  edm::Handle<bool> booleanFlag;
  evt.getByLabel(src_, booleanFlag);

  return (*booleanFlag);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(EventSelectorPluginFactory, BoolEventSelector, "BoolEventSelector");      

#include "PhysicsTools/UtilAlgos/interface/EventSelectorAdapter.h"

typedef EventSelectorAdapter<BoolEventSelector> BoolEventFilter;

DEFINE_ANOTHER_FWK_MODULE(BoolEventFilter);
