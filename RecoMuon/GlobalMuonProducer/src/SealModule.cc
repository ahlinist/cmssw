#include "FWCore/PluginManager/interface/ModuleDef.h"

#include "FWCore/Framework/interface/MakerMacros.h"

#include "RecoMuon/GlobalMuonProducer/src/GlobalMuonProducer.h"
#include "RecoMuon/GlobalMuonProducer/src/TevMuonProducer.h"

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(GlobalMuonProducer);
DEFINE_ANOTHER_FWK_MODULE(TevMuonProducer);

