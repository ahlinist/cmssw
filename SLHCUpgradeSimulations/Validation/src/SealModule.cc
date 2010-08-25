#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "SLHCUpgradeSimulations/Validation/interface/SLHCPrimaryVertexAnalyzer.h"
#include "SLHCUpgradeSimulations/Validation/interface/SLHCPixelHitAnalyzer.h"
#include "SLHCUpgradeSimulations/Validation/interface/SLHCAllPixelHitAnalyzer.h"
#include "SLHCUpgradeSimulations/Validation/interface/SLHCrecoBSVTagInfoValidationAnalyzer.h"

using namespace edm;
using namespace reco;

DEFINE_FWK_MODULE(SLHCPrimaryVertexAnalyzer);
DEFINE_FWK_MODULE(SLHCPixelHitAnalyzer);
DEFINE_FWK_MODULE(SLHCAllPixelHitAnalyzer);
DEFINE_FWK_MODULE(SLHCrecoBSVTagInfoValidationAnalyzer);
