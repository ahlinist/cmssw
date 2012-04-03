import FWCore.ParameterSet.Config as cms

from TauAnalysis.CandidateTools.nSVfitAlgorithmDiTau_cfi import *

#--------------------------------------------------------------------------------
# CV: define W mass and width

pdg_W_mass  = 80.399
pdg_W_width =  2.085
#--------------------------------------------------------------------------------

nSVfitResonanceBuilderWtoElecNu = cms.PSet(
    pluginName = cms.string("nSVfitResonanceBuilderW"),
    pluginType = cms.string("NSVfitResonanceBuilderW"),
    W_mass = cms.double(pdg_W_mass),
    W_width = cms.double(pdg_W_width),
    daughters = cms.PSet(
         chargedLepton = cms.PSet(
            src = cms.InputTag('patElectronsFromWdecays')
         )
    ),
    verbosity = cms.int32(0)
)

nSVfitResonanceBuilderWtoMuNu = nSVfitResonanceBuilderWtoElecNu.clone(
    daughters = cms.PSet(
         chargedLepton = cms.PSet(
            src = cms.InputTag('patMuonsFromWdecays')
         )
    )
)    

nSVfitResonanceLikelihoodBreitWignerW = cms.PSet(
    pluginName = cms.string("nSVfitResonanceLikelihoodBreitWignerW"),
    pluginType = cms.string("NSVfitResonanceLikelihoodBreitWigner"),
    mass = cms.double(pdg_W_mass),
    width = cms.double(pdg_W_width),
    power = cms.double(1.0),
    verbosity = cms.int32(0)
)

nSVfitResonanceLikelihoodPhaseSpaceW = cms.PSet(
    pluginName = cms.string("nSVfitResonanceLikelihoodPhaseSpaceW"),
    pluginType = cms.string("NSVfitResonanceLikelihoodPhaseSpace"),
    power = cms.double(1.0),
    verbosity = cms.int32(0)
)

nSVfitResonanceBuilderHiggs = cms.PSet(
    pluginName = cms.string("nSVfitResonanceBuilderHiggs"),
    pluginType = cms.string("NSVfitResonanceBuilder"),
    polStates = cms.vstring( # polarization states to be considered when evaluating likelihoods
        "LL", "RR" # Higgs case
    )
)

nSVfitConfigWH_template = cms.PSet(
    event = cms.PSet(
        resonances = cms.PSet(
            Higgs = cms.PSet(
                daughters = cms.PSet(
                    leg1 = cms.PSet(
                        src = cms.InputTag('patMuonsFromTauDecays'),
                        likelihoodFunctions = cms.VPSet(
                            nSVfitMuonLikelihoodMatrixElement.clone(
                                applySinThetaFactor = cms.bool(False),
                                applyVisPtCutCorrection = cms.bool(True)
                            )
                        ),
                        builder = nSVfitTauToMuBuilder
                    ),
                    leg2 = cms.PSet(
                        src = cms.InputTag('patTaus'),
                        likelihoodFunctions = cms.VPSet(
                            nSVfitTauLikelihoodPhaseSpace.clone(
                                applySinThetaFactor = cms.bool(False),
                                applyVisPtCutCorrection = cms.bool(True)
                            )
                        ),
                        builder = nSVfitTauToHadBuilder
                    )
                ),
                likelihoodFunctions = cms.VPSet(),
                builder = nSVfitResonanceBuilderHiggs
            ),
            W = cms.PSet(
                daughters = cms.PSet(
                    chargedLepton = cms.PSet(
                        src = cms.InputTag('patElectronFromWdecays')
                    ),
                    neutrino = cms.PSet()
                ),
                likelihoodFunctions = cms.VPSet(
                    nSVfitResonanceLikelihoodBreitWignerW,
                    nSVfitResonanceLikelihoodPhaseSpaceW
                ),
                builder = nSVfitResonanceBuilderWtoElecNu
            )
        ),
        srcMEt = cms.InputTag('patPFMETs'),
        srcPrimaryVertex = cms.InputTag("offlinePrimaryVerticesWithBS"),
        likelihoodFunctions = cms.VPSet(nSVfitEventLikelihoodMEt2),
        builder = nSVfitEventBuilder
    )
)

nSVfitProducerByIntegrationWH = cms.EDProducer("NSVfitProducerByIntegration",
    config = nSVfitConfigWH_template,
    algorithm = cms.PSet(
        pluginName = cms.string("nSVfitAlgorithmByIntegrationWH"),
        pluginType = cms.string("NSVfitAlgorithmByIntegration"),
        parameters = cms.PSet(
            mass_Higgs = cms.PSet(
                min = cms.double(5.),
                max = cms.double(2000.),                                         
                stepSizeFactor = cms.double(1.025), # nextM = max(stepSizeFactor*currentM, minStepSize)
                minStepSize = cms.double(2.5),      
                replace = cms.string("leg1.x"),
                by = cms.string("(Higgs.p4.mass/mass_Higgs)*(Higgs.p4.mass/mass_Higgs)/leg2.x")
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

nSVfitProducerByLikelihoodMaximizationWH = cms.EDProducer("NSVfitProducer",
    config = nSVfitConfigWH_template,
    algorithm = cms.PSet(
        pluginName = cms.string("nSVfitAlgorithmByLikelihoodMaximizationWH"),
        pluginType = cms.string("NSVfitAlgorithmByLikelihoodMaximization"),
        minimizer  = cms.vstring("Minuit2", "Migrad"),
        maxObjFunctionCalls = cms.uint32(5000),
        verbosity = cms.int32(0)
    ),
    dRmin = cms.double(0.3),
    instanceLabel = cms.string("")
)
