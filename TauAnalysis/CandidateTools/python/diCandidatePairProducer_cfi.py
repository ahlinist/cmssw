import FWCore.ParameterSet.Config as cms
import copy

diTauProducer = cms.EDProducer("DiCandidatePairProducer",

    # if true, use the highest pT tau on both legs.
    # otherwise, try all combinations
    useLeadingTausOnly = cms.bool(False),

    # collection of Candidate based objects acting as the hadronic tau
    srcLeg1 = cms.InputTag(''),

    # collection of Candidate based objects acting as the leptonic tau
    srcLeg2 = cms.InputTag(''),

    # minimum distance in eta-phi between leptonic and hadronic tau decay products
    dRmin12 = cms.double(0.),

    # collection of Candidate based objects acting as MET
    # must contain only one object
    srcMET = cms.InputTag(''),

    # collection of PVs and beam spot source
    srcPrimaryVertex = cms.InputTag("offlinePrimaryVerticesWithBS"),
    srcBeamSpot = cms.InputTag("offlineBeamSpot"),

    # gen parts collection
    srcGenParticles = cms.InputTag('genParticles'),

    # recinstruction mode for di-tau four-momentum:
    #  o collinear approximation ('collinearApprox')
    #  o "CDF method" ('cdfMethod')
    #  o or di-tau reconstructed from visible decay products only ('')
    # NOTE: leave recoMode empty in case of no srcMET defined   
    recoMode = cms.string(''),

    # SVfit algorithm requires pat::Electron, pat::Muon, pat::Tau types
    # (does not work with 'generic' particle Candidates)
    doSVreco = cms.bool(False),
                               
    # scale function for diTaus rescued by the improved collinear approximation
    # '1' is the default
    scaleFuncImprovedCollinearApprox = cms.string('1'),
        
    verbosity = cms.untracked.int32(0)
)
