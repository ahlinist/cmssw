#include "TauAnalysis/RecoTools/plugins/VertexEventSelector.h"

#include "CommonTools/UtilAlgos/interface/EventSelectorBase.h"

#include "FWCore/Framework/interface/MakerMacros.h"

using namespace reco;

DEFINE_EDM_PLUGIN(EventSelectorPluginFactory, VertexMinEventSelector, "VertexMinEventSelector");

DEFINE_EDM_PLUGIN(EventSelectorPluginFactory, VertexMaxEventSelector, "VertexMaxEventSelector");
