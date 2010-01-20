import FWCore.ParameterSet.Config as cms

bgEstFakeRateJetWeights = cms.EDProducer("FakeRateJetWeightProducer",
    method = cms.string("CDF"), # "simple"/"CDF"
                                         
    allTauJetSource = cms.InputTag('shrinkingConePFTauProducer'),
    preselTauJetSource = cms.InputTag('shrinkingConePFTauProducer'),
                                         
    dRmatch = cms.double(0.3),
                                         
    frTypes = cms.PSet(
        qcdMuEnriched = cms.PSet(
            tauJetDiscriminators = cms.VPSet(
                cms.PSet(
                    tauJetIdEffSource = cms.InputTag("shrinkingConeZTTEffSimAssociator", "effByStandardChainZTTsim"),
                    qcdJetFakeRateSource = cms.InputTag("shrinkingConeMuEnrichedQCDAssociator", "frByStandardChainMuEnrichedQCDsim"),
                    tauJetDiscrSource = cms.InputTag("ewkTauId")
                )
            )
        ),
        qcdDiJetLeadJet = cms.PSet(
            tauJetDiscriminators = cms.VPSet(
                cms.PSet(
                    tauJetIdEffSource = cms.InputTag("shrinkingConeZTTEffSimAssociator", "effByStandardChainZTTsim"),
                    qcdJetFakeRateSource = cms.InputTag("shrinkingConeDiJetHighPt", "frByStandardChainDiJetHighPtsim"),
                    tauJetDiscrSource = cms.InputTag("ewkTauId")
                )
            )
        ),
        qcdDiJetSecondLeadJet = cms.PSet(
            tauJetDiscriminators = cms.VPSet(
                cms.PSet(
                    tauJetIdEffSource = cms.InputTag("shrinkingConeZTTEffSimAssociator", "effByStandardChainZTTsim"),
                    qcdJetFakeRateSource = cms.InputTag("shrinkingConeDiJetSecondPt", "frByStandardChainDiJetSecondPtsim"),
                    tauJetDiscrSource = cms.InputTag("ewkTauId")
                )
            )
        ),
        WplusJets = cms.PSet(
            tauJetDiscriminators = cms.VPSet(
                cms.PSet(
                    tauJetIdEffSource = cms.InputTag("shrinkingConeZTTEffSimAssociator", "effByStandardChainZTTsim"),
                    qcdJetFakeRateSource = cms.InputTag("shrinkingConeWJets", "frByStandardChainWJetssim"),
                    tauJetDiscrSource = cms.InputTag("ewkTauId")
                )
            )
        )
        #gammaPlusJets        
    )
)
