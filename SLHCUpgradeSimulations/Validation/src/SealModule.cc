#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "SLHCUpgradeSimulations/Validation/interface/SLHCPrimaryVertexAnalyzer.h"
#include "DQM/SiStripCommon/interface/SiStripFolderOrganizer.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "SLHCUpgradeSimulations/Validation/interface/SLHCPixelHitAnalyzer.h"

using namespace edm;
using namespace reco;

DEFINE_FWK_MODULE(SLHCPrimaryVertexAnalyzer);
DEFINE_FWK_MODULE(SLHCPixelHitAnalyzer);
