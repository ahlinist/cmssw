#include "TauAnalysis/CandidateTools/plugins/DiCandidatePairEventSelector.h"

#include "CommonTools/UtilAlgos/interface/EventSelectorBase.h"

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(EventSelectorPluginFactory, DiCandidatePairMinEventSelector, "DiCandidatePairMinEventSelector");
DEFINE_EDM_PLUGIN(EventSelectorPluginFactory, DiCandidatePairMaxEventSelector, "DiCandidatePairMaxEventSelector");

DEFINE_EDM_PLUGIN(EventSelectorPluginFactory, DiCandidatePairCountEventSelector, "DiCandidatePairCountEventSelector");
