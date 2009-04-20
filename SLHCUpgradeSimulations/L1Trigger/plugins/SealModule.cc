#include "SLHCUpgradeSimulations/L1Trigger/interface/L1CaloTowerProducer.h"
#include "SLHCUpgradeSimulations/L1Trigger/interface/L1CaloClusterProducer.h"
#include "SLHCUpgradeSimulations/L1Trigger/interface/L1CaloClusterFilter.h"
#include "SLHCUpgradeSimulations/L1Trigger/interface/L1JetMaker.h"
#include "SLHCUpgradeSimulations/L1Trigger/interface/L1ExtraMaker.h"

DEFINE_SEAL_MODULE();
DEFINE_FWK_MODULE(L1CaloTowerProducer);
DEFINE_FWK_MODULE(L1CaloClusterProducer);
DEFINE_FWK_MODULE(L1CaloClusterFilter);
DEFINE_FWK_MODULE(L1JetMaker);
DEFINE_FWK_MODULE(L1ExtraMaker);

