#include "FWCore/PluginManager/interface/ModuleDef.h"

#include "FWCore/Framework/interface/MakerMacros.h"

#include "RecoMuon/SeedGenerator/src/SETMuonProducer.h"

#include "RecoMuon/SeedGenerator/src/RPCSeedGenerator.h"

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(SETMuonProducer);
DEFINE_ANOTHER_FWK_MODULE(RPCSeedGenerator);

