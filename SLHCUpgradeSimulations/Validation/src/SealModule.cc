#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "SLHCUpgradeSimulations/Validation/interface/SLHCPrimaryVertexAnalyzer.h"
#include "SLHCUpgradeSimulations/Validation/interface/SLHCPixelHitAnalyzer.h"
#include "SLHCUpgradeSimulations/Validation/interface/SLHCAllPixelHitAnalyzer.h"
#include "CommonTools/RecoAlgos/interface/TrackSelector.h"
#include "SLHCUpgradeSimulations/Validation/interface/SLHCRecoTrackSelector.h"

using namespace edm;
using namespace reco;

DEFINE_FWK_MODULE(SLHCPrimaryVertexAnalyzer);
DEFINE_FWK_MODULE(SLHCPixelHitAnalyzer);
DEFINE_FWK_MODULE(SLHCAllPixelHitAnalyzer);
namespace reco {
  typedef ObjectSelector<SLHCRecoTrackSelector> SLHCRecoTrackSelector;
  DEFINE_FWK_MODULE(SLHCRecoTrackSelector);
}


