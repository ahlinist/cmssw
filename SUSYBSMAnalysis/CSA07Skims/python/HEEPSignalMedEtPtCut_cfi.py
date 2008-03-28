import FWCore.ParameterSet.Config as cms

#
# HEEP skim, med Et signal
#
# pt cut to reduce sample size while retaining enough control events
hEEPSignalMedEtPtCutBarrel = cms.EDFilter("EtMinSuperClusterCountFilterHeepSkim",
    src = cms.InputTag("correctedHybridSuperClusters"),
    etMin = cms.double(40.0),
    minNumber = cms.uint32(1)
)

hEEPSignalMedEtPtCutEndcap = cms.EDFilter("EtMinSuperClusterCountFilterHeepSkim",
    src = cms.InputTag("correctedEndcapSuperClustersWithPreshower"),
    etMin = cms.double(40.0),
    minNumber = cms.uint32(1)
)


