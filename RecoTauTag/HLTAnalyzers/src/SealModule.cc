
#include "FWCore/Framework/interface/MakerMacros.h"
#include "RecoTauTag/HLTAnalyzers/interface/TauJetMCFilter.h"
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "RecoTauTag/HLTAnalyzers/interface/HLTTauMcInfo.h"
#include "RecoTauTag/HLTAnalyzers/interface/HLTTauPathComparison.h"
#include "RecoTauTag/HLTAnalyzers/interface/SingleDoubleTrkTau.h"
#include "RecoTauTag/HLTAnalyzers/interface/L1TauAnalyzer.h"
DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(TauJetMCFilter);
DEFINE_ANOTHER_FWK_MODULE(HLTTauMcInfo);
DEFINE_ANOTHER_FWK_MODULE(HLTTauPathComparison);
DEFINE_ANOTHER_FWK_MODULE(SingleDoubleTrkTau);
DEFINE_ANOTHER_FWK_MODULE(L1TauAnalyzer);



