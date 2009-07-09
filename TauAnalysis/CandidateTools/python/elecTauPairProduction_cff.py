import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# produce combinations of electron + tau-jet pairs
#--------------------------------------------------------------------------------

allElecTauPairs = cms.EDProducer("PATElecTauPairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('selectedLayer1ElectronsTrkIPcumulative'),
    srcLeg2 = cms.InputTag('selectedLayer1TausForElecTauElectronVetoCumulative'),
    dRmin12 = cms.double(0.3),
    srcMET = cms.InputTag('layer1METs'),
    recoMode = cms.string(""),
    verbosity = cms.untracked.int32(0)
)

produceElecTauPairs = cms.Sequence( allElecTauPairs )


