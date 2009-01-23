#include "TauAnalysis/RecoTools/plugins/PATObjectEventSelector.h"

#include "PhysicsTools/UtilAlgos/interface/EventSelectorBase.h"

#include "FWCore/Framework/interface/MakerMacros.h"

using namespace pat;

DEFINE_EDM_PLUGIN(EventSelectorPluginFactory, PATElectronMinEventSelector, "PATElectronMinEventSelector");
DEFINE_EDM_PLUGIN(EventSelectorPluginFactory, PATMuonMinEventSelector, "PATMuonMinEventSelector");
DEFINE_EDM_PLUGIN(EventSelectorPluginFactory, PATTauMinEventSelector, "PATTauMinEventSelector");
DEFINE_EDM_PLUGIN(EventSelectorPluginFactory, PATPhotonMinEventSelector, "PATPhotonMinEventSelector");
DEFINE_EDM_PLUGIN(EventSelectorPluginFactory, PATJetMinEventSelector, "PATJetMinEventSelector");
DEFINE_EDM_PLUGIN(EventSelectorPluginFactory, PATMETMinEventSelector, "PATMETMinEventSelector");
DEFINE_EDM_PLUGIN(EventSelectorPluginFactory, PATParticleMinEventSelector, "PATParticleMinEventSelector");

DEFINE_EDM_PLUGIN(EventSelectorPluginFactory, PATElectronMaxEventSelector, "PATElectronMaxEventSelector");
DEFINE_EDM_PLUGIN(EventSelectorPluginFactory, PATMuonMaxEventSelector, "PATMuonMaxEventSelector");
DEFINE_EDM_PLUGIN(EventSelectorPluginFactory, PATTauMaxEventSelector, "PATTauMaxEventSelector");
DEFINE_EDM_PLUGIN(EventSelectorPluginFactory, PATPhotonMaxEventSelector, "PATPhotonMaxEventSelector");
DEFINE_EDM_PLUGIN(EventSelectorPluginFactory, PATJetMaxEventSelector, "PATJetMaxEventSelector");
DEFINE_EDM_PLUGIN(EventSelectorPluginFactory, PATMETMaxEventSelector, "PATMETMaxEventSelector");
DEFINE_EDM_PLUGIN(EventSelectorPluginFactory, PATParticleMaxEventSelector, "PATParticleMaxEventSelector");


