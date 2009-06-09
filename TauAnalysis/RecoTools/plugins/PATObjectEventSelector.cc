#include "TauAnalysis/RecoTools/plugins/PATObjectEventSelector.h"

#include "PhysicsTools/UtilAlgos/interface/EventSelectorBase.h"

#include "FWCore/Framework/interface/MakerMacros.h"

using namespace pat;

DEFINE_EDM_PLUGIN(EventSelectorPluginFactory, PATCandViewMinEventSelector, "PATCandViewMinEventSelector");
DEFINE_EDM_PLUGIN(EventSelectorPluginFactory, PATCandViewMaxEventSelector, "PATCandViewMaxEventSelector");
DEFINE_EDM_PLUGIN(EventSelectorPluginFactory, PATCandViewCountEventSelector, "PATCandViewCountEventSelector");


