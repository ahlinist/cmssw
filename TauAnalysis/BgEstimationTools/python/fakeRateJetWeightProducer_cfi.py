import FWCore.ParameterSet.Config as cms
import TauAnalysis.BgEstimationTools.fakeRateConfiguration_cfi as config

# Define the dictionary to use to define the fake rates
fake_rate_dict = config.fake_rates[config.PRODUCER]
fake_rate_info_dict = fake_rate_dict['fake_rates']

bgEstFakeRateJetWeights = cms.EDProducer(
    "FakeRateJetWeightProducer",
    #method = cms.string("CDF"),
    method = cms.string("simple"),

    allTauJetSource = cms.InputTag(fake_rate_dict['producer_name']),
    preselTauJetSource = cms.InputTag(fake_rate_dict['producer_name']),

    dRmatch = cms.double(0.1),

    frTypes = cms.PSet(
        qcdMuEnriched = cms.PSet(
            tauJetDiscriminators = cms.VPSet(
                cms.PSet(
                    tauJetIdEffSource = cms.InputTag(config.frproducer_name(
                        config.PRODUCER, 'ZTTsim')),
                    qcdJetFakeRateSource = cms.InputTag(config.frproducer_name(
                        config.PRODUCER, 'ppMuXData')),
                    tauJetDiscrSource = cms.InputTag("ewkTauId")
                )
            )
        ),
        qcdDiJetLeadJet = cms.PSet(
            tauJetDiscriminators = cms.VPSet(
                cms.PSet(
                    tauJetIdEffSource = cms.InputTag(config.frproducer_name(
                        config.PRODUCER, 'ZTTsim')),
                    qcdJetFakeRateSource = cms.InputTag(config.frproducer_name(
                        config.PRODUCER, 'DiJetHighPtdata')),
                    tauJetDiscrSource = cms.InputTag("ewkTauId")
                )
            )
        ),
        qcdDiJetSecondLeadJet = cms.PSet(
            tauJetDiscriminators = cms.VPSet(
                cms.PSet(
                    tauJetIdEffSource = cms.InputTag(config.frproducer_name(
                        config.PRODUCER, 'ZTTsim')),
                    qcdJetFakeRateSource = cms.InputTag(config.frproducer_name(
                        config.PRODUCER, 'DiJetSecondPtdata')),
                    tauJetDiscrSource = cms.InputTag("ewkTauId")
                )
            )
        ),
        WplusJets = cms.PSet(
            tauJetDiscriminators = cms.VPSet(
                cms.PSet(
                    tauJetIdEffSource = cms.InputTag(config.frproducer_name(
                        config.PRODUCER, 'ZTTsim')),
                    qcdJetFakeRateSource = cms.InputTag(config.frproducer_name(
                        config.PRODUCER, 'WplusJetsdata')),
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
    #minJetWeight = cms.double(-1.e+6),
    #maxJetWeight = cms.double(+1.e+6),

    # kinematic range within which jet weights are computed
    # (jets with Pt/Eta < mininimum or > maximum get assigned a weights of zero)
    #minJetPt = cms.double(20.),
    #maxJetPt = cms.double(120.),
    #minJetEta = cms.double(-2.1),
    #maxJetEta = cms.double(+2.1)
    minJetPt = cms.double(-1.e+6),
    maxJetPt = cms.double(+1.e+6),
    minJetEta = cms.double(-1.e+6),
    maxJetEta = cms.double(+1.e+6)
)
