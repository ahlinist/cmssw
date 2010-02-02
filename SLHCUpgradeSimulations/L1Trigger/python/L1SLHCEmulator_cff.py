import FWCore.ParameterSet.Config as cms

from SLHCUpgradeSimulations.L1Trigger.L1CaloTriggerSetup_cfi import *
from SLHCUpgradeSimulations.L1Trigger.SLHCEmulatorModules_cfi import *

#L1SLHCEmulator  = cms.Sequence(L1CaloTowerProducer+L1CaloClusterProducer+L1CaloClusterFilter+L1JetProducer+L1ExtraMaker)
L1SLHCEmulator  = cms.Sequence(L1CaloTowerProducer+L1CaloClusterProducer+L1CaloClusterFilter+L1JetProducer+L1ExtraMaker)




