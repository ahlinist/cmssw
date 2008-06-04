#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "JetMETCorrections/DijetBalance/interface/DijetBalance.h"
#include "JetMETCorrections/DijetBalance/interface/DijetBalanceMCTruth.h"
#include "JetMETCorrections/DijetBalance/interface/DijetBalanceTreeProducer.h"

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(DijetBalance);
DEFINE_ANOTHER_FWK_MODULE(DijetBalanceMCTruth);
DEFINE_ANOTHER_FWK_MODULE(DijetBalanceTreeProducer);
