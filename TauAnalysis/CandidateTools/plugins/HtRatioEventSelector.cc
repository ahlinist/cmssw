#include "TauAnalysis/CandidateTools/plugins/HtRatioEventSelector.h"

#include "CommonTools/UtilAlgos/interface/EventSelectorBase.h"

#include "FWCore/Framework/interface/MakerMacros.h"

using namespace reco; 

DEFINE_EDM_PLUGIN(EventSelectorPluginFactory, HtRatioMinEventSelector, "HtRatioMinEventSelector");
DEFINE_EDM_PLUGIN(EventSelectorPluginFactory, HtRatioMaxEventSelector, "HtRatioMaxEventSelector");

