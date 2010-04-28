#include "TauAnalysis/RecoTools/plugins/MultiBoolEventSelFlagSelector.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <TPRegexp.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TString.h>

#include <iostream>
#include <fstream>
#include <iomanip>

const int noMatchRequired = -1;

MultiBoolEventSelFlagSelector::MultiBoolEventSelFlagSelector(const edm::ParameterSet& cfg)
{
  //std::cout << "<MultiBoolEventSelFlagSelector::MultiBoolEventSelFlagSelector>:" << std::endl;

  cfgError_ = 0;

  evtSelFlags_ = cfg.getParameter<vInputTag>("flags");
  if ( !evtSelFlags_.size() >= 1 ) {
    edm::LogError ("MultiBoolEventSelFlagSelector") << " List of BoolEventSelFlags must not be empty !!";
    cfgError_ = 1;
  }
}

MultiBoolEventSelFlagSelector::~MultiBoolEventSelFlagSelector()
{
  std::cout << "<MultiBoolEventSelFlagSelector::~MultiBoolEventSelFlagSelector>:" << std::endl;
  std::cout << "Filter Statistics:" << std::endl;
  for ( vInputTag::const_iterator evtSelFlag = evtSelFlags_.begin();
	evtSelFlag != evtSelFlags_.end(); ++evtSelFlag ) {
    std::cout << " " << std::setw(20) << evtSelFlag->label() << ": " << evtSelStatistics_[evtSelFlag->label()] << std::endl;
  }
}

bool MultiBoolEventSelFlagSelector::operator()(edm::Event& evt, const edm::EventSetup& es)
{
//--- check that configuration parameters contain no errors
  if ( cfgError_ ) {
    edm::LogError ("operator()") << " Error in Configuration ParameterSet --> skipping !!";
    return false;
  }

//--- check values of boolean flags;
//    return true only if **all** flags evaluate to true,
//    otherwise return false
  for ( vInputTag::const_iterator evtSelFlag = evtSelFlags_.begin();
	evtSelFlag != evtSelFlags_.end(); ++evtSelFlag ) {
    edm::Handle<bool> value;
    evt.getByLabel(*evtSelFlag, value);

//--- return immediately once one flag evaluates to false
    if ( (*value) == false ) return false;

    ++evtSelStatistics_[evtSelFlag->label()];
  }

//--- all flags evaluate to true
  return true;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(EventSelectorPluginFactory, MultiBoolEventSelFlagSelector, "MultiBoolEventSelFlagSelector");

#include "CommonTools/UtilAlgos/interface/EventSelectorAdapter.h"

typedef EventSelectorAdapter<MultiBoolEventSelFlagSelector> MultiBoolEventSelFlagFilter;

DEFINE_ANOTHER_FWK_MODULE(MultiBoolEventSelFlagFilter);
