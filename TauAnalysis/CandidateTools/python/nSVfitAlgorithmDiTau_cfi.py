import FWCore.ParameterSet.Config as cms

from TrackingTools.TransientTrack.TransientTrackBuilder_cfi import TransientTrackBuilderESProducer
import RecoMET.METProducers.METSigParams_cfi as met_config
import TauAnalysis.CandidateTools.nSVfitAlgorithmTauDecayKineMC_cfi as kineMC_config

nSVfitTrackService = cms.Service("NSVfitTrackService")

nSVfitElectronLikelihoodPhaseSpace = cms.PSet(
    pluginName = cms.string("nSVfitTauToElecLikelihoodPhaseSpace"),
    pluginType = cms.string("NSVfitTauToElecLikelihoodPhaseSpace"),
    verbosity = cms.int32(0)
)

nSVfitElectronLikelihoodMC_energy_angle_all = kineMC_config.nSVfitTauDecayLikelihoodMC_energy_angle_all.clone(
    pluginName = cms.string("nSVfitTauToElecLikelihoodMC_energy_angle_all"),
    pluginType = cms.string("NSVfitTauToElecLikelihoodMC"),
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
    verbosity = cms.int32(0)
)

nSVfitMuonLikelihoodMC_energy_angle_all = kineMC_config.nSVfitTauDecayLikelihoodMC_energy_angle_all.clone(
    pluginName = cms.string("nSVfitTauToMuLikelihoodMC_energy_angle_all"),
    pluginType = cms.string("NSVfitTauToMuLikelihoodMC"),
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
    verbosity = cms.int32(0)
)

nSVfitTauToHadLikelihoodMC_energy_angle_all = kineMC_config.nSVfitTauDecayLikelihoodMC_energy_angle_all.clone(
    pluginName = cms.string("nSVfitTauToHadLikelihoodMC_energy_angle_all"),
    pluginType = cms.string("NSVfitTauToHadLikelihoodMC"),
    verbosity = cms.int32(0)
)

nSVfitTauToHadBuilder = cms.PSet(
    pluginName = cms.string("nSVfitTauToHadBuilder"),
    pluginType = cms.string("NSVfitTauToHadBuilder"),
    verbosity = cms.int32(0)
)

nSVfitResonanceLikelihoodLogM = cms.PSet(
    pluginName = cms.string("nSVfitResonanceLikelihoodLogM"),
    pluginType = cms.string("NSVfitResonanceLikelihoodMassPenalty"),
    power = cms.double(1.0)
)

nSVfitResonanceBuilder = cms.PSet(
    pluginName = cms.string("nSVfitResonanceBuilder"),
    pluginType = cms.string("NSVfitResonanceBuilder")
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
    config    = nSVfitConfig_template,
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
            maxChi2         = cms.double(2.),
            maxIntEvalIter  = cms.uint32(5),                                          
            precision       = cms.double(0.00001)
        ),
        verbosity = cms.int32(0)
    ),
    dRmin = cms.double(0.3),
    instanceLabel = cms.string("")
)

nSVfitProducerByLikelihoodMaximization = cms.EDProducer("NSVfitProducer",
    config    = nSVfitConfig_template,
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
