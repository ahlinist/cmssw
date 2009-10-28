import FWCore.ParameterSet.Config as cms

bgEstFakeRateJetWeights = cms.EDProducer("FakeRateJetWeightProducer",
    method = cms.string("CDF"), # "simple"/"CDF"                                     
    allTauJetSource = cms.InputTag('shrinkingConePFTauProducer'),
    preselTauJetSource = cms.InputTag('shrinkingConePFTauProducer'),
    dRmatch = cms.double(0.3),                                         
    tauJetDiscriminators = cms.VPSet(
        cms.PSet(
            tauJetIdEffSource = cms.InputTag("shrinkingConeEfficienciesProducerFromFile", "effByECALIsolationZtautausim"),
            qcdJetFakeRateSource = cms.InputTag("shrinkingConeEfficienciesProducerFromFile", "frByECALIsolationMuEnrichedQCDsim"),
            tauJetDiscrSource = cms.InputTag("shrinkingConePFTauDiscriminationByIsolation")
        )
    )
)
