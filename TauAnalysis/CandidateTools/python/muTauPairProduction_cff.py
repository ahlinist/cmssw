import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.CandidateTools.tools.objProdConfigurator import *
from TauAnalysis.CandidateTools.resolutions_cfi import *
from TauAnalysis.CandidateTools.svFitAlgorithm_cfi import *

svFitLikelihoodMuTauPairKinematicsPhaseSpace = copy.deepcopy(svFitLikelihoodDiTauKinematicsPhaseSpace)
svFitLikelihoodMuTauPairKinematicsPhaseSpace.pluginType = "SVfitLikelihoodMuTauPairKinematics"
svFitLikelihoodMuTauPairKinematicsPhaseSpace.leg1.pluginType = "SVfitMuonLikelihoodPhaseSpace"
svFitLikelihoodMuTauPairKinematicsPhaseSpace.leg2.pluginType = "SVfitTauLikelihoodPhaseSpace"

svFitLikelihoodMuTauPairKinematicsPolarized = copy.deepcopy(svFitLikelihoodDiTauKinematicsPolarized)
svFitLikelihoodMuTauPairKinematicsPolarized.pluginType = "SVfitLikelihoodMuTauPairKinematics"
svFitLikelihoodMuTauPairKinematicsPolarized.leg1.pluginType = "SVfitMuonLikelihoodPolarization"
svFitLikelihoodMuTauPairKinematicsPolarized.leg2.pluginType = "SVfitTauLikelihoodPolarization"

svFitLikelihoodMuTauPairMEt = copy.deepcopy(svFitLikelihoodMEt)
svFitLikelihoodMuTauPairMEt.pluginType = cms.string("SVfitLikelihoodMEtMuTau")

svFitLikelihoodMuTauPairTrackInfo = copy.deepcopy(svFitLikelihoodTrackInfo)
svFitLikelihoodMuTauPairTrackInfo.pluginType = "SVfitLikelihoodMuTauPairTrackInfo"
svFitLikelihoodMuTauPairTrackInfo.leg1.pluginType = "SVfitMuonLikelihoodTrackInfo"
svFitLikelihoodMuTauPairTrackInfo.leg2.pluginType = "SVfitTauLikelihoodTrackInfo"

svFitLikelihoodMuTauPairPtBalance = copy.deepcopy(svFitLikelihoodDiTauPtBalance)
svFitLikelihoodMuTauPairPtBalance.pluginType = cms.string("SVfitLikelihoodMuTauPairPtBalance")

svFitLikelihoodMuTauPairZprod = copy.deepcopy(svFitLikelihoodDiTauProdZ0)
svFitLikelihoodMuTauPairZprod.pluginType = cms.string("SVfitLikelihoodMuTauPairProd")
svFitLikelihoodMuTauPairZprod.process = cms.string("Z0")

#--------------------------------------------------------------------------------
# produce combinations of muon + tau-jet pairs
#--------------------------------------------------------------------------------

allMuTauPairs = cms.EDProducer("PATMuTauPairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('selectedPatMuonsTrkIPcumulative'),
    srcLeg2 = cms.InputTag('selectedPatTausForMuTauElectronVetoCumulative'),
    dRmin12 = cms.double(0.3),
    srcMET = cms.InputTag('patMETs'),
    srcPrimaryVertex = cms.InputTag("offlinePrimaryVerticesWithBS"),
    srcBeamSpot = cms.InputTag("offlineBeamSpot"),
    srcGenParticles = cms.InputTag('genParticles'),
    recoMode = cms.string(""),
    doSVreco = cms.bool(True),
    svFit = cms.PSet(
        psKine = cms.PSet(
            likelihoodFunctions = cms.VPSet(
                svFitLikelihoodMuTauPairKinematicsPhaseSpace
            ),
            estUncertainties = cms.PSet(
                numSamplings = cms.int32(-1)
            )
        ),
        psKine_MEt = cms.PSet(
            likelihoodFunctions = cms.VPSet(
                svFitLikelihoodMuTauPairKinematicsPhaseSpace,
                svFitLikelihoodMuTauPairMEt
            ),
            estUncertainties = cms.PSet(
                numSamplings = cms.int32(-1)
            )
        ),
        psKine_MEt_ptBalance = cms.PSet(
            likelihoodFunctions = cms.VPSet(
                svFitLikelihoodMuTauPairKinematicsPhaseSpace,
                svFitLikelihoodMuTauPairMEt,
                svFitLikelihoodMuTauPairPtBalance
            ),
            estUncertainties = cms.PSet(
                #numSamplings = cms.int32(1000)
                numSamplings = cms.int32(-1)
            )
        #),
        #psKine_MEt_Track_ptBalance = cms.PSet(
            #likelihoodFunctions = cms.VPSet(
                #svFitLikelihoodMuTauPairKinematicsPhaseSpace,
                #svFitLikelihoodMuTauPairMEt,
                #svFitLikelihoodMuTauPairTrackInfo,
                #svFitLikelihoodMuTauPairPtBalance
            #),
            #estUncertainties = cms.PSet(
                ##numSamplings = cms.int32(1000)
                #numSamplings = cms.int32(-1)
            #)
        )
    ),
    scaleFuncImprovedCollinearApprox = cms.string('1'),
    verbosity = cms.untracked.int32(0)
)

muTauPairProdConfigurator = objProdConfigurator(
    allMuTauPairs,
    pyModuleName = __name__
)

produceMuTauPairs = muTauPairProdConfigurator.configure(pyNameSpace = locals())

# define additional collections of muon + tau-jet candidates
# with loose track and ECAL isolation applied on muon leg
# (NOTE: to be used for the purpose of factorizing efficiencies
#        of muon isolation from other event selection criteria,
#        in order to avoid problems with limited Monte Carlo statistics)

allMuTauPairsLooseMuonIsolation = allMuTauPairs.clone(
    srcLeg1 = cms.InputTag('selectedPatMuonsTrkIPlooseIsolationCumulative'),
)

muTauPairProdConfiguratorLooseMuonIsolation = objProdConfigurator(
    allMuTauPairsLooseMuonIsolation,
    pyModuleName = __name__
)

produceMuTauPairsLooseMuonIsolation = muTauPairProdConfiguratorLooseMuonIsolation.configure(pyNameSpace = locals())

produceMuTauPairsAll = cms.Sequence(produceMuTauPairs * produceMuTauPairsLooseMuonIsolation)
