
#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "RecoTauTag/HLTAnalyzers/interface/SingleDoubleTrkTau.h"
#include "RecoTauTag/HLTAnalyzers/interface/HLTAnalysis.h"


DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(SingleDoubleTrkTau);
DEFINE_ANOTHER_FWK_MODULE(HLTAnalysis);





