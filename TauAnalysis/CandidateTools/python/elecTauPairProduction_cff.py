import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.CandidateTools.tools.objProdConfigurator import *
from TauAnalysis.CandidateTools.resolutions_cfi import *
from TauAnalysis.CandidateTools.svFitAlgorithm_cfi import *

svFitLikelihoodElecTauPairKinematicsPhaseSpace = copy.deepcopy(svFitLikelihoodDiTauKinematicsPhaseSpace)
svFitLikelihoodElecTauPairKinematicsPhaseSpace.pluginType = "SVfitLikelihoodElecTauPairKinematics"
svFitLikelihoodElecTauPairKinematicsPhaseSpace.leg1.pluginType = "SVfitElectronLikelihoodPhaseSpace"
svFitLikelihoodElecTauPairKinematicsPhaseSpace.leg2.pluginType = "SVfitTauLikelihoodPhaseSpace"

svFitLikelihoodElecTauPairKinematicsPolarized = copy.deepcopy(svFitLikelihoodDiTauKinematicsPolarized)
svFitLikelihoodElecTauPairKinematicsPolarized.pluginType = "SVfitLikelihoodElecTauPairKinematics"
svFitLikelihoodElecTauPairKinematicsPolarized.leg1.pluginType = "SVfitMuonLikelihoodPolarization"
svFitLikelihoodElecTauPairKinematicsPolarized.leg2.pluginType = "SVfitTauLikelihoodPolarization"

svFitLikelihoodElecTauPairMEt = copy.deepcopy(svFitLikelihoodMEt)
svFitLikelihoodElecTauPairMEt.pluginType = cms.string("SVfitLikelihoodMEtElecTau")

svFitLikelihoodElecTauPairTrackInfo = copy.deepcopy(svFitLikelihoodTrackInfo)
svFitLikelihoodElecTauPairTrackInfo.pluginType = "SVfitLikelihoodElecTauPairTrackInfo"
svFitLikelihoodElecTauPairTrackInfo.leg1.pluginType = "SVfitElectronLikelihoodTrackInfo"
svFitLikelihoodElecTauPairTrackInfo.leg2.pluginType = "SVfitTauLikelihoodTrackInfo"

svFitLikelihoodElecTauPairPtBalance = copy.deepcopy(svFitLikelihoodDiTauPtBalance)
svFitLikelihoodElecTauPairPtBalance.pluginType = cms.string("SVfitLikelihoodElecTauPairPtBalance")

svFitLikelihoodElecTauPairPtBalance2 = copy.deepcopy(svFitLikelihoodDiTauPtBalance2)
svFitLikelihoodElecTauPairPtBalance2.pluginType = cms.string("SVfitLikelihoodElecTauPairPtBalance2")

svFitLikelihoodElecTauPairZprod = copy.deepcopy(svFitLikelihoodDiTauProdZ0)
svFitLikelihoodElecTauPairZprod.pluginType = cms.string("SVfitLikelihoodElecTauPairProd")
svFitLikelihoodElecTauPairZprod.process = cms.string("Z0")

#--------------------------------------------------------------------------------
# produce combinations of electron + tau-jet pairs
#--------------------------------------------------------------------------------

allElecTauPairs = cms.EDProducer("PATElecTauPairProducer",
    useLeadingTausOnly = cms.bool(False),
	srcLeg1 = cms.InputTag('selectedPatElectronsForElecTauTrkIPcumulative'),
	srcLeg2 = cms.InputTag('selectedPatTausForElecTauMuonVetoCumulative'),
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
                svFitLikelihoodElecTauPairKinematicsPhaseSpace         
            ),
            estUncertainties = cms.PSet(
                numSamplings = cms.int32(-1)
            )
        ),
        psKine_MEt = cms.PSet(
            likelihoodFunctions = cms.VPSet(
                svFitLikelihoodElecTauPairKinematicsPhaseSpace,
                svFitLikelihoodElecTauPairMEt
            ),
            estUncertainties = cms.PSet(
                numSamplings = cms.int32(-1)
            )
        ),
        psKine_MEt_ptBalance = cms.PSet(
            likelihoodFunctions = cms.VPSet(
                svFitLikelihoodElecTauPairKinematicsPhaseSpace,
                svFitLikelihoodElecTauPairMEt,
                svFitLikelihoodElecTauPairPtBalance
            ),
            estUncertainties = cms.PSet(
                #numSamplings = cms.int32(1000)
                numSamplings = cms.int32(-1)
            )
        ),
        psKine_MEt_ptBalance2 = cms.PSet(
            likelihoodFunctions = cms.VPSet(
                svFitLikelihoodElecTauPairKinematicsPhaseSpace,
                svFitLikelihoodElecTauPairMEt,
                svFitLikelihoodElecTauPairPtBalance2
            ),
            estUncertainties = cms.PSet(
                #numSamplings = cms.int32(1000)
                numSamplings = cms.int32(-1)
            )
        )

    ),
    scaleFuncImprovedCollinearApprox = cms.string('1'),                             
    verbosity = cms.untracked.int32(0),
)
elecTauPairProdConfigurator = objProdConfigurator(
	allElecTauPairs,
	pyModuleName = __name__
)               
                
produceElecTauPairs = elecTauPairProdConfigurator.configure(pyNameSpace = locals())

# define additional collections of electron + tau-jet candidates
# with loose track and ECAL isolation (and conversion rejection) applied on electron leg
# (NOTE: to be used for the purpose of factorizing efficiencies
#        of electron isolation from other event selection criteria,
#        in order to avoid problems with limited Monte Carlo statistics)


allElecTauPairsLooseElectronIsolation = allElecTauPairs.clone()
allElecTauPairsLooseElectronIsolation.srcLeg1 = cms.InputTag('selectedPatElectronsForElecTauTrkIPlooseIsolationCumulative')

elecTauPairProdConfiguratorLooseElectronIsolation = objProdConfigurator(
	allElecTauPairsLooseElectronIsolation,
	pyModuleName = __name__
)               
                
produceElecTauPairsLooseElectronIsolation = elecTauPairProdConfiguratorLooseElectronIsolation.configure(pyNameSpace = locals())

produceElecTauPairsAll = cms.Sequence( produceElecTauPairs * produceElecTauPairsLooseElectronIsolation )
