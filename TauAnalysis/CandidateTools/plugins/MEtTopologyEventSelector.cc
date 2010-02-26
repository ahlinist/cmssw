#include "TauAnalysis/CandidateTools/plugins/MEtTopologyEventSelector.h"

#include "CommonTools/UtilAlgos/interface/EventSelectorBase.h"

#include "FWCore/Framework/interface/MakerMacros.h"

using namespace reco; 

DEFINE_EDM_PLUGIN(EventSelectorPluginFactory, MEtTopologyMinEventSelector, "MEtTopologyMinEventSelector");
DEFINE_EDM_PLUGIN(EventSelectorPluginFactory, MEtTopologyMaxEventSelector, "MEtTopologyMaxEventSelector");

