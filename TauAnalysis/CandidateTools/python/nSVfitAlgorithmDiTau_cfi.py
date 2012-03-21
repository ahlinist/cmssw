import FWCore.ParameterSet.Config as cms

from TrackingTools.TransientTrack.TransientTrackBuilder_cfi import TransientTrackBuilderESProducer
import RecoMET.METProducers.METSigParams_cfi as met_config
import TauAnalysis.CandidateTools.nSVfitAlgorithmTauDecayKineMC_cfi as kineMC_config
import TauAnalysis.CandidateTools.nSVfitAlgorithmVisPtCutCorrections_cfi as visPtCutCorrections

nSVfitTrackService = cms.Service("NSVfitTrackService")

nSVfitElectronLikelihoodPhaseSpace = cms.PSet(
    pluginName = cms.string("nSVfitTauToElecLikelihoodPhaseSpace"),
    pluginType = cms.string("NSVfitTauToElecLikelihoodPhaseSpace"),
    applySinThetaFactor = cms.bool(True),
    verbosity = cms.int32(0)
)

nSVfitElectronLikelihoodMC_energy_angle_all = kineMC_config.nSVfitTauDecayLikelihoodMC_energy_angle_all.clone(
    pluginName = cms.string("nSVfitTauToElecLikelihoodMC_energy_angle_all"),
    pluginType = cms.string("NSVfitTauToElecLikelihoodMC"),
    verbosity = cms.int32(0)
)

nSVfitElectronLikelihoodMatrixElement = cms.PSet(
    pluginName = cms.string("nSVfitTauToElecLikelihoodMatrixElement"),
    pluginType = cms.string("NSVfitTauToElecLikelihoodMatrixElement"),
    applySinThetaFactor = cms.bool(True),
    verbosity = cms.int32(0)
)

nSVfitTauToElecBuilder = cms.PSet(
    pluginName = cms.string("nSVfitTauToElecBuilder"),
    pluginType = cms.string("NSVfitTauToElecBuilder"),
    verbosity = cms.int32(0)
)

nSVfitMuonLikelihoodPhaseSpace = cms.PSet(
    pluginName = cms.string("nSVfitTauToMuLikelihoodPhaseSpace"),
    pluginType = cms.string("NSVfitTauToMuLikelihoodPhaseSpace"),
    applySinThetaFactor = cms.bool(True),
    verbosity = cms.int32(0)
)

nSVfitMuonLikelihoodMC_energy_angle_all = kineMC_config.nSVfitTauDecayLikelihoodMC_energy_angle_all.clone(
    pluginName = cms.string("nSVfitTauToMuLikelihoodMC_energy_angle_all"),
    pluginType = cms.string("NSVfitTauToMuLikelihoodMC"),
    verbosity = cms.int32(0)
)

nSVfitMuonLikelihoodMatrixElement = cms.PSet(
    pluginName = cms.string("nSVfitTauToMuLikelihoodMatrixElement"),
    pluginType = cms.string("NSVfitTauToMuLikelihoodMatrixElement"),
    applySinThetaFactor = cms.bool(True),
    verbosity = cms.int32(0)
)

nSVfitTauToMuBuilder = cms.PSet(
    pluginName = cms.string("nSVfitTauToMuBuilder"),
    pluginType = cms.string("NSVfitTauToMuBuilder"),
    verbosity = cms.int32(0)
)

nSVfitTauLikelihoodPhaseSpace = cms.PSet(
    pluginName = cms.string("nSVfitTauToHadLikelihoodPhaseSpace"),
    pluginType = cms.string("NSVfitTauToHadLikelihoodPhaseSpace"),
    VMshapeFileName = cms.FileInPath("TauAnalysis/CandidateTools/data/shapes.root")
    applySinThetaFactor = cms.bool(True),
    verbosity = cms.int32(0)
)

nSVfitTauToHadLikelihoodMC_energy_angle_all = kineMC_config.nSVfitTauDecayLikelihoodMC_energy_angle_all.clone(
    pluginName = cms.string("nSVfitTauToHadLikelihoodMC_energy_angle_all"),
    pluginType = cms.string("NSVfitTauToHadLikelihoodMC"),
    verbosity = cms.int32(0)
)

nSVfitTauLikelihoodMatrixElement = nSVfitTauLikelihoodPhaseSpace # CV: not implemented yet

nSVfitTauToHadBuilder = cms.PSet(
    pluginName = cms.string("nSVfitTauToHadBuilder"),
    pluginType = cms.string("NSVfitTauToHadBuilder"),
    verbosity = cms.int32(0)
)

nSVfitResonanceLikelihoodPhaseSpace = cms.PSet(
    pluginName = cms.string("nSVfitResonanceLikelihoodPhaseSpace"),
    pluginType = cms.string("NSVfitResonanceLikelihoodPhaseSpace"),
    power = cms.double(1.0),
    verbosity = cms.int32(0)
)

nSVfitResonanceLikelihoodLogM = cms.PSet(
    pluginName = cms.string("nSVfitResonanceLikelihoodLogM"),
    pluginType = cms.string("NSVfitResonanceLikelihoodMassPenalty"),
    nll = cms.string("TMath::Log(mass)"),
    power = cms.double(1.0)
)

nSVfitResonanceLikelihoodLogEff = cms.PSet(
    pluginName = cms.string("nSVfitResonanceLikelihoodEff_power100"),
    pluginType = cms.string("NSVfitResonanceLikelihoodMassPenalty"),
    nll = cms.string("TMath::Log(TMath::Max(5.00e-3, 4.21e-2*(2.52e-2 + TMath::Erf((x - 4.40e+1)*6.90e-3))))"),
    power = cms.double(1.0)
)

nSVfitResonanceLikelihoodPolarization = cms.PSet(
    pluginName = cms.string("nSVfitResonanceLikelihoodPolarization"),
    pluginType = cms.string("NSVfitResonanceLikelihoodPolarization"),
    LR = cms.PSet(
        formula = cms.string("[0]"), # CV: linearly decrease weight of Z specific polarization
                                                         #     between mZ and mZ + 20 GeV
        xMin = cms.double(91.188),
        xMax = cms.double(111.188),
        parameter = cms.PSet(
            par0 = cms.double(0.576),
            par1 = cms.double(91.188), # mZ / GeV
            par2 = cms.double(20.)     # width of transition region between Z and Higgs specific polarization
        )
    ),
    RL = cms.PSet(
        formula = cms.string("[0]"), # CV: linearly decrease weight of Z specific polarization
                                                         #     between mZ and mZ + 20 GeV
        xMin = cms.double(91.188),
        xMax = cms.double(111.188),
        parameter = cms.PSet(
            par0 = cms.double(0.424),
            par1 = cms.double(91.188), # mZ / GeV
            par2 = cms.double(20.)     # width of transition region between Z and Higgs specific polarization
        )
    ),
    LL = cms.PSet(
        formula = cms.string("[0]"), # CV: linearly increase weight of Higgs specific polarization
                                                   #     between mZ and mZ + 20 GeV
        xMin = cms.double(91.188), 
        xMax = cms.double(111.188),
        parameter = cms.PSet(
            par0 = cms.double(0.50),
            par1 = cms.double(91.188), # mZ / GeV
            par2 = cms.double(20.),    # width of transition region between Z and Higgs specific polarization
        )
    ),
    RR = cms.PSet(
        formula = cms.string("[0]"), # CV: linearly increase weight of Higgs specific polarization
                                                   #     between mZ and mZ + 20 GeV
        xMin = cms.double(91.188), 
        xMax = cms.double(111.188),
        parameter = cms.PSet(
            par0 = cms.double(0.50),
            par1 = cms.double(91.188), # mZ / GeV
            par2 = cms.double(20.),    # width of transition region between Z and Higgs specific polarization
        )
    ),
    power = cms.double(1.0)
)

nSVfitResonanceLikelihoodPrior = cms.PSet(
    pluginName = cms.string("nSVfitResonanceLikelihoodPrior"),
    pluginType = cms.string("NSVfitResonanceLikelihoodPrior"),
    formula = cms.string("1. + [0]*TMath::Gaus(x, [1], [2])"),
    xMin = cms.double(91.188), # do not apply prior probability correction below mZ
    xMax = cms.double(1.e+3),
    parameter = cms.PSet(
        par0 = cms.double(3.),
        par1 = cms.double(91.188), # mZ / GeV
        par2 = cms.double(2.495)   # GammaZ / GeV
    ),
    power = cms.double(1.0)
)

nSVfitResonanceBuilder = cms.PSet(
    pluginName = cms.string("nSVfitResonanceBuilder"),
    pluginType = cms.string("NSVfitResonanceBuilder"),
    polStates = cms.vstring( # polarization states to be considered when evaluating likelihoods
    "undefined"
    #"LR", "RL", # Z case
    #"LL", "RR"  # Higgs case
    )
)

nSVfitEventLikelihoodMEt2 = cms.PSet(
    pluginName = cms.string("nSVfitEventLikelihoodMEt2"),
    pluginType = cms.string("NSVfitEventLikelihoodMEt2"),
    srcPFJets = cms.InputTag('ak5PFJets'),
    srcPFCandidates = cms.InputTag('particleFlow'),
    resolution = met_config.METSignificance_params,
    dRoverlapPFJet = cms.double(0.3),
    dRoverlapPFCandidate = cms.double(0.1),
    power = cms.double(1.0),
    verbosity = cms.int32(0)
)

nSVfitEventBuilder = cms.PSet(
    pluginName = cms.string("nSVfitEventBuilder"),
    pluginType = cms.string("NSVfitEventBuilder"),
    srcBeamSpot = cms.InputTag("offlineBeamSpot")
)

nSVfitConfig_template = cms.PSet(
    event = cms.PSet(
        resonances = cms.PSet(
            A = cms.PSet(
                daughters = cms.PSet(
                    leg1 = cms.PSet(
                        src = cms.InputTag('selectedPatMuonsTrkIPcumulative'),
                        likelihoodFunctions = cms.VPSet(nSVfitMuonLikelihoodPhaseSpace),
                        builder = nSVfitTauToMuBuilder
                    ),
                    leg2 = cms.PSet(
                        src = cms.InputTag('selectedPatTausForMuTauElectronVetoCumulative'),
                        likelihoodFunctions = cms.VPSet(nSVfitTauLikelihoodPhaseSpace),
                        builder = nSVfitTauToHadBuilder
                    )
                ),
                likelihoodFunctions = cms.VPSet(nSVfitResonanceLikelihoodLogM),
                #likelihoodFunctions = cms.VPSet(nSVfitResonanceLikelihoodLogEff),
                builder = nSVfitResonanceBuilder
            )
        ),
        srcMEt = cms.InputTag('patPFMETs'),
        srcPrimaryVertex = cms.InputTag("offlinePrimaryVerticesWithBS"),
        likelihoodFunctions = cms.VPSet(nSVfitEventLikelihoodMEt2),
        builder = nSVfitEventBuilder
    )
)

nSVfitProducerByIntegration = cms.EDProducer("NSVfitProducerByIntegration",
    config = nSVfitConfig_template,
    algorithm = cms.PSet(
        pluginName = cms.string("nSVfitAlgorithmByIntegration"),
        pluginType = cms.string("NSVfitAlgorithmByIntegration"),
        parameters = cms.PSet(
            mass_A = cms.PSet(
                min = cms.double(5.),
                max = cms.double(2000.),                                         
                stepSizeFactor = cms.double(1.025), # nextM = max(stepSizeFactor*currentM, minStepSize)
                minStepSize = cms.double(2.5),      
                replace = cms.string("leg1.x"),
                by = cms.string("(A.p4.mass/mass_A)*(A.p4.mass/mass_A)/leg2.x")
            )
        ),
        vegasOptions = cms.PSet(
            numCallsGridOpt = cms.uint32(1000),
            numCallsIntEval = cms.uint32(10000),
            maxChi2 = cms.double(2.),
            maxIntEvalIter = cms.uint32(5),                                          
            precision = cms.double(0.00001)
        ),
        verbosity = cms.int32(0)
    ),
    dRmin = cms.double(0.3),
    instanceLabel = cms.string("")
)

nSVfitProducerByLikelihoodMaximization = cms.EDProducer("NSVfitProducer",
    config = nSVfitConfig_template,
    algorithm = cms.PSet(
        pluginName = cms.string("nSVfitAlgorithmByLikelihoodMaximization"),
        pluginType = cms.string("NSVfitAlgorithmByLikelihoodMaximization"),
        minimizer  = cms.vstring("Minuit2", "Migrad"),
        maxObjFunctionCalls = cms.uint32(5000),
        verbosity = cms.int32(0)
    ),
    dRmin = cms.double(0.3),
    instanceLabel = cms.string("")
)
