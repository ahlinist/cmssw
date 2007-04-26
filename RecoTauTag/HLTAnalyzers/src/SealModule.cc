
#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "RecoTauTag/HLTAnalyzers/interface/SingleDoubleTrkTau.h"
#include "RecoTauTag/HLTAnalyzers/interface/HLTAnalysis.h"
#include "RecoTauTag/HLTAnalyzers/interface/TauJetMCFilter.h"

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(SingleDoubleTrkTau);
DEFINE_ANOTHER_FWK_MODULE(HLTAnalysis);
DEFINE_ANOTHER_FWK_MODULE(TauJetMCFilter);





