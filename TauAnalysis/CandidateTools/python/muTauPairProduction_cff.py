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

svFitLikelihoodMuTauPairMEt = copy.deepcopy(svFitLikelihoodDiTauMEt)
svFitLikelihoodMuTauPairMEt.pluginType = cms.string("SVfitLikelihoodMuTauPairMEt")

svFitLikelihoodMuTauPairTrackInfo = copy.deepcopy(svFitLikelihoodTrackInfo)
svFitLikelihoodMuTauPairTrackInfo.pluginType = "SVfitLikelihoodMuTauPairTrackInfo"
svFitLikelihoodMuTauPairTrackInfo.leg1.pluginType = "SVfitMuonLikelihoodTrackInfo"
svFitLikelihoodMuTauPairTrackInfo.leg2.pluginType = "SVfitTauLikelihoodTrackInfo"

svFitLikelihoodMuTauPairPtBalance = copy.deepcopy(svFitLikelihoodDiTauPtBalance)
svFitLikelihoodMuTauPairPtBalance.pluginType = cms.string("SVfitLikelihoodMuTauPairPtBalance")

svFitLikelihoodMuTauPairPtBalance2 = copy.deepcopy(svFitLikelihoodDiTauPtBalance2)
svFitLikelihoodMuTauPairPtBalance2.pluginType = cms.string("SVfitLikelihoodMuTauPairPtBalance2")

svFitLikelihoodMuTauPairZprod = copy.deepcopy(svFitLikelihoodDiTauProdZ0)
svFitLikelihoodMuTauPairZprod.pluginType = cms.string("SVfitLikelihoodMuTauPairProd")
svFitLikelihoodMuTauPairZprod.process = cms.string("Z0")

#--------------------------------------------------------------------------------
# produce combinations of muon + tau-jet pairs
#--------------------------------------------------------------------------------

svfit_with_track = cms.PSet(
    likelihoodFunctions = cms.VPSet(
        svFitLikelihoodMuTauPairKinematicsPhaseSpace,
        svFitLikelihoodMuTauPairMEt,
        svFitLikelihoodMuTauPairTrackInfo,
        svFitLikelihoodMuTauPairPtBalance
    ),
    parameterizeVertexAlongTrackLeg1 = cms.bool(True),
    parameterizeVertexAlongTrackLeg2 = cms.bool(True),
    estUncertainties = cms.PSet(
        #numSamplings = cms.int32(1000)
        numSamplings = cms.int32(-1)
    )
)

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
        ),
        psKine_MEt_ptBalance2 = cms.PSet(
            likelihoodFunctions = cms.VPSet(
                svFitLikelihoodMuTauPairKinematicsPhaseSpace,
                svFitLikelihoodMuTauPairMEt,
                svFitLikelihoodMuTauPairPtBalance2
            ),
            estUncertainties = cms.PSet(
                #numSamplings = cms.int32(1000)
                numSamplings = cms.int32(-1)
            )
        ),
        ##psKine_MEt_Track_ptBalance = cms.PSet(
        ##    likelihoodFunctions = cms.VPSet(
        ##        svFitLikelihoodMuTauPairKinematicsPhaseSpace,
        ##        svFitLikelihoodMuTauPairMEt,
        ##        svFitLikelihoodMuTauPairTrackInfo,
        ##        svFitLikelihoodMuTauPairPtBalance
        ##    ),
        ##    parameterizeVertexAlongTrack = cms.bool(True),
        ##    estUncertainties = cms.PSet(
        ##        #numSamplings = cms.int32(1000)
        ##        numSamplings = cms.int32(-1)
        ##    )
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

from TauAnalysis.CandidateTools.nSVfitAlgorithmDiTau_cfi import *
nSVfitMuTauPairHypothesesPS1 = nSVfitProducer.clone(    
    instanceLabel = cms.string("psKine")
)
nSVfitMuTauPairHypothesesPS1.config.event.resonances.A.likelihoodFunctions = cms.VPSet()
nSVfitMuTauPairHypothesesPS1.config.event.likelihoodFunctions = cms.VPSet()
nSVfitMuTauPairHypothesesPS2 = nSVfitMuTauPairHypothesesPS1.clone(
    instanceLabel = cms.string("psKine+MEt")
)
nSVfitMuTauPairHypothesesPS2.config.event.likelihoodFunctions = cms.VPSet(nSVfitEventLikelihoodMEt)
nSVfitMuTauPairHypothesesPS3 = nSVfitMuTauPairHypothesesPS2.clone(
    instanceLabel = cms.string("psKine+MEt+ptBalance")
)
nSVfitMuTauPairHypothesesPS3.config.event.resonances.A.likelihoodFunctions = cms.VPSet(nSVfitResonanceLikelihoodPtBalance)

nSVfitMuTauPairHypothesesPol1 = nSVfitProducer.clone(    
    instanceLabel = cms.string("polKine")
)
nSVfitMuTauPairHypothesesPol1.config.event.resonances.A.likelihoodFunctions = cms.VPSet()
nSVfitMuTauPairHypothesesPol1.config.event.likelihoodFunctions = cms.VPSet()
nSVfitMuTauPairHypothesesPol1.config.event.resonances.A.daughters.leg1.likelihoodFunctions = cms.VPSet(nSVfitMuonLikelihoodPolarization)
nSVfitMuTauPairHypothesesPol1.config.event.resonances.A.daughters.leg2.likelihoodFunctions = cms.VPSet(nSVfitTauLikelihoodPolarization)
nSVfitMuTauPairHypothesesPol2 = nSVfitMuTauPairHypothesesPol1.clone(
    instanceLabel = cms.string("polKine+MEt")
)
nSVfitMuTauPairHypothesesPol2.config.event.likelihoodFunctions = cms.VPSet(nSVfitEventLikelihoodMEt)
nSVfitMuTauPairHypothesesPol3 = nSVfitMuTauPairHypothesesPol2.clone(
    instanceLabel = cms.string("polKine+MEt+ptBalance")
)
nSVfitMuTauPairHypothesesPol3.config.event.resonances.A.likelihoodFunctions = cms.VPSet(nSVfitResonanceLikelihoodPtBalance)

nSVfitMuTauPairHypothesesPolColApprox1 = nSVfitProducer.clone(    
    instanceLabel = cms.string("polKine")
)
nSVfitMuTauPairHypothesesPolColApprox1.config.event.resonances.A.likelihoodFunctions = cms.VPSet()
nSVfitMuTauPairHypothesesPolColApprox1.config.event.likelihoodFunctions = cms.VPSet()
nSVfitMuTauPairHypothesesPolColApprox1.config.event.resonances.A.daughters.leg1.likelihoodFunctions = cms.VPSet(
    nSVfitMuonLikelihoodPolarization.clone(
        useCollApproxFormulas = cms.bool(True)
    )
)    
nSVfitMuTauPairHypothesesPolColApprox1.config.event.resonances.A.daughters.leg2.likelihoodFunctions = cms.VPSet(
    nSVfitTauLikelihoodPolarization.clone(
        useCollApproxFormulas = cms.bool(True)
    )
)
nSVfitMuTauPairHypothesesPolColApprox2 = nSVfitMuTauPairHypothesesPolColApprox1.clone(
    instanceLabel = cms.string("polKine+MEt")
)
nSVfitMuTauPairHypothesesPolColApprox2.config.event.likelihoodFunctions = cms.VPSet(nSVfitEventLikelihoodMEt)
nSVfitMuTauPairHypothesesPolColApprox3 = nSVfitMuTauPairHypothesesPolColApprox2.clone(
    instanceLabel = cms.string("polKine+MEt+ptBalance")
)
nSVfitMuTauPairHypothesesPolColApprox3.config.event.resonances.A.likelihoodFunctions = cms.VPSet(nSVfitResonanceLikelihoodPtBalance)

nSVfitMuTauPairHypotheses = cms.Sequence(
    nSVfitMuTauPairHypothesesPS1 + nSVfitMuTauPairHypothesesPS2 + nSVfitMuTauPairHypothesesPS3
   + nSVfitMuTauPairHypothesesPol1 + nSVfitMuTauPairHypothesesPol2 + nSVfitMuTauPairHypothesesPol3
   + nSVfitMuTauPairHypothesesPolColApprox1 + nSVfitMuTauPairHypothesesPolColApprox2 + nSVfitMuTauPairHypothesesPolColApprox3
)
produceMuTauPairsAll *= nSVfitMuTauPairHypotheses
