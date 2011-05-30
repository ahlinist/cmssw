import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.CandidateTools.tools.objProdConfigurator import *
from TauAnalysis.CandidateTools.resolutions_cfi import *
from TauAnalysis.CandidateTools.svFitAlgorithm_cfi import *
from TauAnalysis.CandidateTools.nSVfitAlgorithmDiTau_cfi import *
from TauAnalysis.CandidateTools.nSVfitAlgorithmTauDecayKineMC_cfi import *
from RecoMET.METProducers.METSigParams_cfi import *

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
    nSVfit = cms.PSet(),
    scaleFuncImprovedCollinearApprox = cms.string('1'),
    doPFMEtSign = cms.bool(True),                           
    pfMEtSign = cms.PSet(
        srcPFJets = cms.InputTag('ak5PFJets'),
        srcPFCandidates = cms.InputTag('particleFlow'),
        resolution = METSignificance_params,
        dRoverlapPFJet = cms.double(0.3),
        dRoverlapPFCandidate = cms.double(0.1)
    ),                   
    verbosity = cms.untracked.int32(0)
)
                              
nSVfitMuTauPair_template = cms.PSet(
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

#--------------------------------------------------------------------------------
# Tau decay Kinematics
allMuTauPairs.nSVfit.psKine_fit = copy.deepcopy(nSVfitMuTauPair_template)
allMuTauPairs.nSVfit.psKine_fit.config.event.resonances.A.likelihoodFunctions = cms.VPSet()
allMuTauPairs.nSVfit.psKine_fit.config.event.likelihoodFunctions = cms.VPSet()
allMuTauPairs.nSVfit.psKine_fit.algorithm = cms.PSet(
    pluginName = cms.string("nSVfitAlgorithmByLikelihoodMaximization"),
    pluginType = cms.string("NSVfitAlgorithmByLikelihoodMaximization"),                                    
    minimizer  = cms.vstring("Minuit2", "Migrad"),
    maxObjFunctionCalls_ = cms.uint32(5000),  
    verbosity = cms.int32(0)
)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# Tau decay Kinematics + MET
allMuTauPairs.nSVfit.psKine_MEt_fit = copy.deepcopy(allMuTauPairs.nSVfit.psKine_fit)
allMuTauPairs.nSVfit.psKine_MEt_fit.config.event.likelihoodFunctions = cms.VPSet(nSVfitEventLikelihoodMEt)

allMuTauPairs.nSVfit.psKine_MEt2_fit = copy.deepcopy(allMuTauPairs.nSVfit.psKine_MEt_fit)
allMuTauPairs.nSVfit.psKine_MEt2_fit.config.event.likelihoodFunctions = cms.VPSet(nSVfitEventLikelihoodMEt2)

allMuTauPairs.nSVfit.mcKineDeltaRall_MEt2_fit = copy.deepcopy(allMuTauPairs.nSVfit.psKine_MEt2_fit)
allMuTauPairs.nSVfit.mcKineDeltaRall_MEt2_fit.config.event.resonances.A.daughters.leg1.likelihoodFunctions = cms.VPSet(
    nSVfitTauDecayLikelihoodMC_pt_dR_all.clone(
        pluginName = cms.string("nSVfitTauToMuLikelihoodMC"),
        pluginType = cms.string("NSVfitTauToMuLikelihoodMC")
    )
)
allMuTauPairs.nSVfit.mcKineDeltaRall_MEt2_fit.config.event.resonances.A.daughters.leg2.likelihoodFunctions = cms.VPSet(
    nSVfitTauDecayLikelihoodMC_pt_dR_all.clone(
        pluginName = cms.string("nSVfitTauToHadLikelihoodMC"),
        pluginType = cms.string("NSVfitTauToHadLikelihoodMC")
    )
)

allMuTauPairs.nSVfit.mcKineDeltaRsel_MEt2_fit = copy.deepcopy(allMuTauPairs.nSVfit.psKine_MEt2_fit)
allMuTauPairs.nSVfit.mcKineDeltaRsel_MEt2_fit.config.event.resonances.A.daughters.leg1.likelihoodFunctions = cms.VPSet(
    nSVfitTauDecayLikelihoodMC_pt_dR_selected.clone(
        pluginName = cms.string("nSVfitTauToMuLikelihoodMC"),
        pluginType = cms.string("NSVfitTauToMuLikelihoodMC")
    )
)
allMuTauPairs.nSVfit.mcKineDeltaRsel_MEt2_fit.config.event.resonances.A.daughters.leg2.likelihoodFunctions = cms.VPSet(
    nSVfitTauDecayLikelihoodMC_pt_dR_selected.clone(
        pluginName = cms.string("nSVfitTauToHadLikelihoodMC"),
        pluginType = cms.string("NSVfitTauToHadLikelihoodMC")
    )
)

allMuTauPairs.nSVfit.mcKineAngleAll_MEt2_fit = copy.deepcopy(allMuTauPairs.nSVfit.psKine_MEt2_fit)
allMuTauPairs.nSVfit.mcKineAngleAll_MEt2_fit.config.event.resonances.A.daughters.leg1.likelihoodFunctions = cms.VPSet(
    nSVfitTauDecayLikelihoodMC_energy_angle_all.clone(
        pluginName = cms.string("nSVfitTauToMuLikelihoodMC"),
        pluginType = cms.string("NSVfitTauToMuLikelihoodMC")
    )
)
allMuTauPairs.nSVfit.mcKineAngleAll_MEt2_fit.config.event.resonances.A.daughters.leg2.likelihoodFunctions = cms.VPSet(
    nSVfitTauDecayLikelihoodMC_energy_angle_all.clone(
        pluginName = cms.string("nSVfitTauToHadLikelihoodMC"),
        pluginType = cms.string("NSVfitTauToHadLikelihoodMC")
    )
)

allMuTauPairs.nSVfit.mcKineAngleSel_MEt2_fit = copy.deepcopy(allMuTauPairs.nSVfit.psKine_MEt2_fit)
allMuTauPairs.nSVfit.mcKineAngleSel_MEt2_fit.config.event.resonances.A.daughters.leg1.likelihoodFunctions = cms.VPSet(
    nSVfitTauDecayLikelihoodMC_energy_angle_selected.clone(
        pluginName = cms.string("nSVfitTauToMuLikelihoodMC"),
        pluginType = cms.string("NSVfitTauToMuLikelihoodMC")
    )
)
allMuTauPairs.nSVfit.mcKineAngleSel_MEt2_fit.config.event.resonances.A.daughters.leg2.likelihoodFunctions = cms.VPSet(
    nSVfitTauDecayLikelihoodMC_energy_angle_selected.clone(
        pluginName = cms.string("nSVfitTauToHadLikelihoodMC"),
        pluginType = cms.string("NSVfitTauToHadLikelihoodMC")
    )
)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# Tau decay Kinematics + MET + PtBalance 
allMuTauPairs.nSVfit.psKine_MEt2_pow05ptBalance_fit = copy.deepcopy(allMuTauPairs.nSVfit.psKine_MEt2_fit)
allMuTauPairs.nSVfit.psKine_MEt2_pow05ptBalance_fit.config.event.resonances.A.likelihoodFunctions = cms.VPSet(
    nSVfitResonanceLikelihoodPtBalance.clone(
        power = cms.double(0.5)                         
    )
)

allMuTauPairs.nSVfit.psKine_MEt2_pow10ptBalance_fit = copy.deepcopy(allMuTauPairs.nSVfit.psKine_MEt2_fit)
allMuTauPairs.nSVfit.psKine_MEt2_pow10ptBalance_fit.config.event.resonances.A.likelihoodFunctions = cms.VPSet(
    nSVfitResonanceLikelihoodPtBalance.clone(
        power = cms.double(1.0)                         
    )
)
#--------------------------------------------------------------------------------

muTauPairProdConfigurator = objProdConfigurator(
    allMuTauPairs,
    pyModuleName = __name__
)

produceMuTauPairs = muTauPairProdConfigurator.configure(pyNameSpace = locals())

allMuTauPairsPFtype1MET = copy.deepcopy(allMuTauPairs)
allMuTauPairsPFtype1MET.srcMET = cms.InputTag('patPFtype1METs')
produceMuTauPairs += allMuTauPairsPFtype1MET

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
