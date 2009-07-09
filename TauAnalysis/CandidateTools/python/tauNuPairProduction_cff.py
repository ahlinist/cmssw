import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# produce combinations of tau-jet + missing transverse momentum 
#--------------------------------------------------------------------------------

allTauNuPairs = cms.EDProducer("PATTauNuPairProducer",
    srcVisDecayProducts = cms.InputTag('selectedLayer1TausMuonVetoCumulative'),
    srcMET = cms.InputTag('layer1METs'),
    verbosity = cms.untracked.int32(0)
)

produceTauNuPairs = cms.Sequence( allTauNuPairs )
