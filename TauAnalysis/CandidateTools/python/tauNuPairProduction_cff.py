import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# produce combinations of tau-jet + missing transverse momentum 
#--------------------------------------------------------------------------------

allTauNuPairs = cms.EDProducer("PATTauNuPairProducer",
    srcVisDecayProducts = cms.InputTag('selectedLayer1TausForWTauNuEcalCrackVetoCumulative'),
    srcMET = cms.InputTag('patMETs'),
    verbosity = cms.untracked.int32(0)
)

allTauNuPairsLooseIsolation = cms.EDProducer("PATTauNuPairProducer",
    srcVisDecayProducts = cms.InputTag('selectedLayer1TausForWTauNuEcalCrackVetoLooseIsolationCumulative'),
    srcMET = cms.InputTag('patMETs'),
    verbosity = cms.untracked.int32(0)
)

produceTauNuPairs = cms.Sequence( allTauNuPairs * allTauNuPairsLooseIsolation )
