import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.CandidateTools.resolutions_cfi import *
from TauAnalysis.CandidateTools.svFitAlgorithm_cfi import *
from TauAnalysis.CandidateTools.nSVfitAlgorithmDiTau_cfi import *

diTauProducer = cms.EDProducer(
    "DiCandidatePairProducer",

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
    doSVreco = cms.bool(True),
    svFit = cms.PSet(
        psKine = cms.PSet(
            likelihoodFunctions = cms.VPSet(
                svFitLikelihoodDiTauKinematicsPhaseSpace
            ),
            estUncertainties = cms.PSet(
                numSamplings = cms.int32(-1)
            )
        ),
        psKine_MEt = cms.PSet(
            likelihoodFunctions = cms.VPSet(
                svFitLikelihoodDiTauKinematicsPhaseSpace,
                svFitLikelihoodDiTauMEt
            ),
            estUncertainties = cms.PSet(
                numSamplings = cms.int32(-1)
            )
        ),
        psKine_MEt_ptBalance = cms.PSet(
            likelihoodFunctions = cms.VPSet(
                svFitLikelihoodDiTauKinematicsPhaseSpace,
                svFitLikelihoodDiTauMEt,
                svFitLikelihoodDiTauPtBalance
            ),
            estUncertainties = cms.PSet(
                #numSamplings = cms.int32(1000)
                numSamplings = cms.int32(-1)
            )
        ),
        psKine_MEt_ptBalance2 = cms.PSet(
            likelihoodFunctions = cms.VPSet(
                svFitLikelihoodDiTauKinematicsPhaseSpace,
                svFitLikelihoodDiTauMEt,
                svFitLikelihoodDiTauPtBalance2
            ),
            estUncertainties = cms.PSet(
                #numSamplings = cms.int32(1000)
                numSamplings = cms.int32(-1)
            )
        ),
        ##psKine_MEt_Track_ptBalance = cms.PSet(
        ##    likelihoodFunctions = cms.VPSet(
        ##        svFitLikelihoodDiTauKinematicsPhaseSpace,
        ##        svFitLikelihoodDiTauMEt,
        ##        svFitLikelihoodDiTauTrackInfo,
        ##        svFitLikelihoodDiTauPtBalance
        ##    ),
        ##    parameterizeVertexAlongTrack = cms.bool(True),
        ##    estUncertainties = cms.PSet(
        ##        #numSamplings = cms.int32(1000)
        ##        numSamplings = cms.int32(-1)
        ##    )
    ),
    nSVfit = cms.PSet(                               
        psKine_MEt_ptBalance = cms.PSet(
            config    = nSVfitConfig,
            algorithm = cms.PSet(
                pluginName = cms.string("nSVfitAlgorithmByIntegration"),
                pluginType = cms.string("NSVfitAlgorithmByIntegration"),                                    
                parameters = cms.PSet(
                    mass_A = cms.PSet(
                        #min = cms.double(20.),
                        min = cms.double(60.),                            
                        max = cms.double(200.),
                        stepSize = cms.double(5.),                                                            
                        replace = cms.string("leg1.x"),
                        by = cms.string("(A.p4.mass/mass_A)*(A.p4.mass/mass_A)/leg2.x")
                    )
                ),
                vegasOptions = cms.PSet(
                    numCalls = cms.uint32(10000)                             
                )
            )
        )
    ),
    # scale function for diTaus rescued by the improved collinear approximation
    # '1' is the default
    scaleFuncImprovedCollinearApprox = cms.string('1'),
        
    verbosity = cms.untracked.int32(0)
)
