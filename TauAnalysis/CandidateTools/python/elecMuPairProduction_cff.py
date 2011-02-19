import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.CandidateTools.svFitAlgorithm_cfi import *

svFitLikelihoodElecMuPairKinematicsPhaseSpace = copy.deepcopy(svFitLikelihoodDiTauKinematicsPhaseSpace)
svFitLikelihoodElecMuPairKinematicsPhaseSpace.pluginType = "SVfitLikelihoodElecMuPairKinematics"
svFitLikelihoodElecMuPairKinematicsPhaseSpace.leg1.pluginType = "SVfitElectronLikelihoodPhaseSpace"
svFitLikelihoodElecMuPairKinematicsPhaseSpace.leg2.pluginType = "SVfitMuonLikelihoodPhaseSpace"

svFitLikelihoodElecMuPairMEt = copy.deepcopy(svFitLikelihoodDiTauMEt)
svFitLikelihoodElecMuPairMEt.pluginType = cms.string("SVfitLikelihoodElecMuPairMEt")

svFitLikelihoodElecMuPairTrackInfo = copy.deepcopy(svFitLikelihoodTrackInfo)
svFitLikelihoodElecMuPairTrackInfo.pluginType = "SVfitLikelihoodElecMuPairTrackInfo"
svFitLikelihoodElecMuPairTrackInfo.leg1.pluginType = "SVfitElectronLikelihoodTrackInfo"
svFitLikelihoodElecMuPairTrackInfo.leg2.pluginType = "SVfitMuonLikelihoodTrackInfo"

svFitLikelihoodElecMuPairPtBalance = copy.deepcopy(svFitLikelihoodDiTauPtBalance)
svFitLikelihoodElecMuPairPtBalance.pluginType = cms.string("SVfitLikelihoodElecMuPairPtBalance")


#--------------------------------------------------------------------------------
# produce combinations of electron + muons pairs
#--------------------------------------------------------------------------------

allElecMuPairs = cms.EDProducer("PATElecMuPairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('selectedPatElectronsForElecMuTrkIPcumulative'),
    srcLeg2 = cms.InputTag('selectedPatMuonsTrkIPcumulative'),
    dRmin12 = cms.double(-1.),
    srcMET = cms.InputTag('patMETs'),
    srcPrimaryVertex = cms.InputTag("offlinePrimaryVerticesWithBS"),
    srcBeamSpot = cms.InputTag("offlineBeamSpot"),                               
    srcGenParticles = cms.InputTag('genParticles'),                                  
    recoMode = cms.string(""),
    doSVreco = cms.bool(True),                          
    svFit = cms.PSet(
        psKine = cms.PSet(
            likelihoodFunctions = cms.VPSet(
                svFitLikelihoodElecMuPairKinematicsPhaseSpace         
            ),
            estUncertainties = cms.PSet(
                numSamplings = cms.int32(-1)
            )
        ),
        psKine_MEt = cms.PSet(
            likelihoodFunctions = cms.VPSet(
                svFitLikelihoodElecMuPairKinematicsPhaseSpace,
                svFitLikelihoodElecMuPairMEt
            ),
            estUncertainties = cms.PSet(
                numSamplings = cms.int32(-1)
            )
        ),
        psKine_MEt_ptBalance = cms.PSet(
            likelihoodFunctions = cms.VPSet(
                svFitLikelihoodElecMuPairKinematicsPhaseSpace,
                svFitLikelihoodElecMuPairMEt,
                svFitLikelihoodElecMuPairPtBalance
            ),
            estUncertainties = cms.PSet(
                #numSamplings = cms.int32(1000)
                numSamplings = cms.int32(-1)
            )
        )
    ),                                      
    scaleFuncImprovedCollinearApprox = cms.string('1'),                            
    verbosity = cms.untracked.int32(0)
)

produceElecMuPairs = cms.Sequence( allElecMuPairs )

# define additional collections of electron + muon candidates
# with loose track and ECAL isolation applied on electron leg
# (NOTE: to be used for the purpose of factorizing efficiencies
#        of electron isolation from other event selection criteria,
#        in order to avoid problems with limited Monte Carlo statistics)

allElecMuPairsLooseElectronIsolation = cms.EDProducer("PATElecMuPairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('selectedPatElectronsForElecMuTrkIPlooseIsolationCumulative'),
    srcLeg2 = cms.InputTag('selectedPatMuonsTrkIPcumulative'),
    dRmin12 = cms.double(-1.),
    srcMET = cms.InputTag('patMETs'),
    srcGenParticles = cms.InputTag('genParticles'),                                                        
    recoMode = cms.string(""),
    scaleFuncImprovedCollinearApprox = cms.string('1'),                                                  
    verbosity = cms.untracked.int32(0)
)

produceElecMuPairsLooseElectronIsolation = cms.Sequence( allElecMuPairsLooseElectronIsolation )

produceElecMuPairsAll = cms.Sequence( produceElecMuPairs * produceElecMuPairsLooseElectronIsolation )


