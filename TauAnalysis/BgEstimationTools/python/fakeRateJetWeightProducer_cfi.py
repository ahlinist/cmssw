import FWCore.ParameterSet.Config as cms

bgEstFakeRateJetWeights = cms.EDProducer("FakeRateJetWeightProducer",
    #method = cms.string("CDF"),
    method = cms.string("simple"),

    # collection of all reco::PFTau objects
    allTauJetSource = cms.InputTag(),

    # collection of reco::PFTau/pat::Tau objects
    # for which fake-rates are to be computed.
    # Entries in the reco::PFTau 'allTauJetSource' collection 
    # are matched in eta-phi to entries in the collection specified by 'preselTauJetSource'.
    # For unmatched 'allTauJetSource' entries the fake-rate is set to 0. 
    #
    # NOTE: 'allTauJetSource' and 'preselTauJetSource' are set in 
    #
    preselTauJetSource = cms.InputTag(),

    dRmatch = cms.double(0.1),

    # define fake-rates and tau id. efficiencies
    # obtained by Monte Carlo simulation/measured in Data
    # and for different types of signal/background processes
    #
    # NOTE: set by configureFakeRateWeightProduction function
    #       in TauAnalysis/BgEstimationTools/python/tools/fakeRateAnalysisTools.py
    #
    frTypes = cms.PSet(),

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
