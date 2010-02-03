import FWCore.ParameterSet.Config as cms

bgEstFakeRateJetWeights = cms.EDProducer("FakeRateJetWeightProducer",
    method = cms.string("CDF"),
    #method = cms.string("simple"),          
                                         
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
    ),

    # minimum/maximum jet weights below/above which
    # jet weights get "truncated"                                          
    minJetWeight = cms.double(-1.),
    maxJetWeight = cms.double(+1.),

    # kinematic range within which jet weights are computed
    # (jets with Pt/Eta < mininimum or > maximum get assigned a weights of zero)
    minJetPt = cms.double(20.),
    maxJetPt = cms.double(120.),
    minJetEta = cms.double(-2.1),
    maxJetEta = cms.double(+2.1)             
)
