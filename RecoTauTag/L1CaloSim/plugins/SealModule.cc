#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/PluginManager/interface/ModuleDef.h"

#include "RecoTauTag/L1CaloSim/plugins/L1TauCommision.h"
#include "RecoTauTag/L1CaloSim/plugins/L1TauCommision_Fast.h"
#include "RecoTauTag/L1CaloSim/plugins/L1TauCommision_TrigFilt.h"
#include "RecoTauTag/L1CaloSim/plugins/L1TauCommision_MuonFilt.h"

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(L1TauCommision);
DEFINE_ANOTHER_FWK_MODULE(L1TauCommision_Fast);
DEFINE_ANOTHER_FWK_MODULE(L1TauCommision_TrigFilt);
DEFINE_ANOTHER_FWK_MODULE(L1TauCommision_MuonFilt);
