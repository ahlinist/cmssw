
#include "FWCore/Framework/interface/MakerMacros.h"
#include "RecoTauTag/HLTAnalyzers/interface/TauJetMCFilter.h"
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "RecoTauTag/HLTAnalyzers/interface/HLTmcInfo.h"
#include "RecoTauTag/HLTAnalyzers/interface/HLTPathComparison.h"

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(TauJetMCFilter);
DEFINE_ANOTHER_FWK_MODULE(HLTmcInfo);
DEFINE_ANOTHER_FWK_MODULE(HLTPathComparison);




