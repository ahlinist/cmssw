import FWCore.ParameterSet.Config as cms

from TauAnalysis.CandidateTools.svFitAlgorithmWtauNu_cfi import *

#--------------------------------------------------------------------------------
# produce combinations of tau-jet + missing transverse momentum 
#--------------------------------------------------------------------------------

allTauNuPairs = cms.EDProducer("PATTauNuPairProducer",
    srcVisDecayProducts = cms.InputTag('selectedPatTausForWTauNuEcalCrackVetoCumulative'),
    srcMET = cms.InputTag('patMETs'),
    srcPrimaryVertex = cms.InputTag("offlinePrimaryVerticesWithBS"),
    srcBeamSpot = cms.InputTag("offlineBeamSpot"),
    doSVreco = cms.bool(True),
    svFit = cms.PSet(
        psKine_MEt_ptBalance = cms.PSet(
            likelihoodFunctions = cms.VPSet(
                svFitLikelihoodWtauNuKinematicsPhaseSpace,
                svFitLikelihoodWtauNuMEt,
                svFitLikelihoodWtauNuPtBalance
            )
        )
    ),                           
    verbosity = cms.untracked.int32(0)
)


produceTauNuPairs = cms.Sequence(allTauNuPairs)
