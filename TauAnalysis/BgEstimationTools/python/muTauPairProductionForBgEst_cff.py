import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------  
# produce collections of muon + tau-jet pairs for data-driven background estimation methods
#--------------------------------------------------------------------------------

muTauPairsForBgEstZmumuEnriched = cms.EDProducer("PATMuTauPairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('muonsTrkTightIsolationForBgEst'),
    srcLeg2 = cms.InputTag('selectedLayer1TausProngCumulative'),
    dRmin12 = cms.double(0.7),
    srcMET = cms.InputTag('layer1METs'),
    recoMode = cms.string(""),
    verbosity = cms.untracked.int32(0)
)

muTauPairsForBgEstWplusJetsEnriched = cms.EDProducer("PATMuTauPairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('muonsTrkTightIsolationForBgEst'),
    srcLeg2 = cms.InputTag('tausAntiOverlapWithMuonsVetoLooseIsolationForBgEst'),
    dRmin12 = cms.double(0.7),
    srcMET = cms.InputTag('layer1METs'),
    recoMode = cms.string(""),
    verbosity = cms.untracked.int32(0)
)

muTauPairsForBgEstTTplusJetsEnriched = cms.EDProducer("PATMuTauPairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('muonsTrkTightIsolationForBgEst'),
    srcLeg2 = cms.InputTag('selectedLayer1TausForMuTauMuonVetoCumulative'),
    dRmin12 = cms.double(0.7),
    srcMET = cms.InputTag('layer1METs'),
    recoMode = cms.string(""),
    verbosity = cms.untracked.int32(0)
)

muTauPairsForBgEstQCDenriched = cms.EDProducer("PATMuTauPairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('muonsTrkLooseIsolationForBgEst'),
    srcLeg2 = cms.InputTag('selectedLayer1TausForMuTauMuonVetoCumulative'),
    dRmin12 = cms.double(0.7),
    srcMET = cms.InputTag('layer1METs'),
    recoMode = cms.string(""),
    verbosity = cms.untracked.int32(0)
)

produceMuTauPairsForBgEst = cms.Sequence( muTauPairsForBgEstZmumuEnriched
                                         * muTauPairsForBgEstWplusJetsEnriched
                                         * muTauPairsForBgEstTTplusJetsEnriched
                                         * muTauPairsForBgEstQCDenriched )
