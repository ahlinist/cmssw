import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.CandidateTools.tools.objProdConfigurator import *
from TauAnalysis.CandidateTools.resolutions_cfi import *
from TauAnalysis.CandidateTools.nSVfitAlgorithmDiTau_cfi import *
from TauAnalysis.CandidateTools.nSVfitAlgorithmTauDecayKineMC_cfi import *
from RecoMET.METProducers.METSigParams_cfi import *

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
    nSVfit = cms.PSet(),                            
    scaleFuncImprovedCollinearApprox = cms.string('1'),                            
    verbosity = cms.untracked.int32(0)
)

#--------------------------------------------------------------------------------
# configure (new) SVfit algorithm
# (using combination of PS + MET likelihoods + logM regularization term
#  to reconstruct mass of tau lepton pair, as described in CMS AN-11-165)
allElecMuPairs.nSVfit.psKine_MEt_logM_fit = cms.PSet()
allElecMuPairs.nSVfit.psKine_MEt_logM_fit.config = copy.deepcopy(nSVfitConfig_template)
allElecMuPairs.nSVfit.psKine_MEt_logM_fit.config.event.resonances.A.daughters.leg1 = cms.PSet(
    src = allElecMuPairs.srcLeg1,
    likelihoodFunctions = cms.VPSet(nSVfitElectronLikelihoodPhaseSpace),
    builder = nSVfitTauToElecBuilder
)
allElecMuPairs.nSVfit.psKine_MEt_logM_fit.config.event.resonances.A.daughters.leg2 = cms.PSet(
    src = allElecMuPairs.srcLeg2,
    likelihoodFunctions = cms.VPSet(nSVfitMuonLikelihoodPhaseSpace),
    builder = nSVfitTauToMuBuilder
)
allElecMuPairs.nSVfit.psKine_MEt_logM_fit.algorithm = cms.PSet(
    pluginName = cms.string("nSVfitAlgorithmByLikelihoodMaximization"),
    pluginType = cms.string("NSVfitAlgorithmByLikelihoodMaximization"),                                    
    minimizer  = cms.vstring("Minuit2", "Migrad"),
    maxObjFunctionCalls = cms.uint32(5000),  
    verbosity = cms.int32(0)
)

allElecMuPairs.nSVfit.psKine_MEt_logM_int = cms.PSet()
allElecMuPairs.nSVfit.psKine_MEt_logM_int.config = allElecMuPairs.nSVfit.psKine_MEt_logM_fit.config
allElecMuPairs.nSVfit.psKine_MEt_logM_int.algorithm = cms.PSet(
    pluginName = cms.string("nSVfitAlgorithmByIntegration"),
    pluginType = cms.string("NSVfitAlgorithmByIntegration"),                                    
    parameters = cms.PSet(
        mass_A = cms.PSet(
            min = cms.double(5.),
            max = cms.double(2000.),
            stepSizeFactor = cms.double(1.025),
            minStepSize = cms.double(2.5),
            replace = cms.string("leg1.x"),
            by = cms.string("(A.p4.mass/mass_A)*(A.p4.mass/mass_A)/leg2.x")
        )
    ),
    vegasOptions = cms.PSet(
        numCalls = cms.uint32(10000)                             
    )
)
#--------------------------------------------------------------------------------

produceElecMuPairs = cms.Sequence(allElecMuPairs)

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

produceElecMuPairsLooseElectronIsolation = cms.Sequence(allElecMuPairsLooseElectronIsolation)

produceElecMuPairsAll = cms.Sequence(produceElecMuPairs * produceElecMuPairsLooseElectronIsolation)


