import FWCore.ParameterSet.Config as cms


diTauProducer = cms.EDProducer(
    "DiCandidatePairProducer",

    # collection of Candidate based objects acting as the hadronic tau
    srcLeg1 = cms.InputTag(''),

    # collection of Candidate based objects acting as the leptonic tau
    srcLeg2 = cms.InputTag(''),

    # minimum distance in eta-phi between leptonic and hadronic tau decay products
    dRmin12 = cms.double(0.),

    # collection of Candidate based objects acting as MET
    # must contain only one object
    srcMET = cms.InputTag(''),

    # recinstruction mode for di-tau four-momentum:
    #  o collinear approximation ('collinearApprox')
    #  o "CDF method" ('cdfMethod')
    #  o or di-tau reconstructed from visible decay products only ('')
    # NOTE: leave recoMode empty in case of no srcMET defined   
    recoMode = cms.string('collinearApprox'),

    # scale function for diTaus rescued by the improved collinear approximation
    # '1' is the default
    scaleFuncImprovedCollinearApprox = cms.string('1'),
    
    # if true, use the highest pT tau on both legs.
    # otherwise, try all combinations
    useLeadingTausOnly = cms.bool(True),
    
    verbosity = cms.untracked.int32(0)
)
