import FWCore.ParameterSet.Config as cms

nSVfitMuonLikelihoodPhaseSpace = cms.PSet(
    pluginName = cms.string("nSVfitTauToMuLikelihoodPhaseSpace"),
    pluginType = cms.string("NSVfitTauToMuLikelihoodPhaseSpace"),
    verbosity = cms.int32(1)  
)

nSVfitTauToMuBuilder = cms.PSet(
    pluginName = cms.string("nSVfitTauToMuBuilder"),
    pluginType = cms.string("NSVfitTauToMuBuilder"),
    verbosity = cms.int32(1)  
)

nSVfitTauLikelihoodPhaseSpace = cms.PSet(
    pluginName = cms.string("nSVfitTauToHadLikelihoodPhaseSpace"),
    pluginType = cms.string("NSVfitTauToHadLikelihoodPhaseSpace"),
    verbosity = cms.int32(1)  
)

nSVfitTauToHadBuilder = cms.PSet(
    pluginName = cms.string("nSVfitTauToHadBuilder"),
    pluginType = cms.string("NSVfitTauToHadBuilder"),
    verbosity = cms.int32(1)  
)

nSVfitResonanceLikelihoodPtBalance = cms.PSet(
    pluginName = cms.string("nSVFitResonanceLikelihoodPtBalance"),
    pluginType = cms.string("NSVfitResonanceLikelihoodPtBalance"),
    # define parameters for muon leg
    leg1 = cms.PSet(
        smear = cms.string("4.4 + 0.036*x"),
        gaussFrac = cms.string("0.93"),
        turnOnWidth = cms.string("0.19 + (-0.0016*x) + (5.27e-6*x*x) + (-6.0e-9*x*x*x)"),
        turnOnThreshold = cms.string("1.355 + 0.379*x"),
        gammaShape = cms.string("2"),
        gammaScale = cms.string("x/4"),
        overallNorm = cms.string("2")
    ),
    # define parameters for tau leg
    leg2 = cms.PSet(
        smear = cms.string("6.3 + 0.019*x"),
        gaussFrac = cms.string("0.93"),
        turnOnWidth = cms.string("0.23 + (-0.0022*x) + (7.91e-6*x*x) + (-9.4e-9*x*x*x)"),
        turnOnThreshold = cms.string("2.2 + 0.365*x"),
        gammaShape = cms.string("2"),
        gammaScale = cms.string("x/4"),
        overallNorm = cms.string("2")
    ),
    parameter = cms.PSet(
        x = cms.string('mass')
    ),
    verbosity = cms.int32(1)     
)

nSVfitResonanceBuilder = cms.PSet(
    pluginName = cms.string("nSVfitResonanceBuilder"),
    pluginType = cms.string("NSVfitResonanceBuilder")
)

nSVfitEventLikelihoodMEt = cms.PSet(
    pluginName = cms.string("nSVfitEventLikelihoodMEt"),
    pluginType = cms.string("NSVfitEventLikelihoodMEt"),
    resolution = cms.PSet(
        parSigma = cms.string("7.54*(1 - 0.00542*x)"),
        parBias = cms.string("-0.96"),
        perpSigma = cms.string("6.85*(1 - 0.00547*x)"),
        perpBias = cms.string("0."),
    ),
    verbosity = cms.int32(1)
)

nSVfitEventBuilder = cms.PSet(
    pluginName = cms.string("nSVfitEventBuilder"),
    pluginType = cms.string("NSVfitEventBuilder")
)

nSVfitConfig = cms.PSet(
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
                likelihoodFunctions = cms.VPSet(nSVfitResonanceLikelihoodPtBalance),
                builder = nSVfitResonanceBuilder
            )
        ),
        srcMEt = cms.InputTag('patPFMETs'),
        likelihoodFunctions = cms.VPSet(nSVfitEventLikelihoodMEt),
        builder = nSVfitEventBuilder
    )
)    

nSVfitProducer = cms.EDProducer("NSVfitProducer",
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
    ),
    dRmin = cms.double(0.3)
)                                
