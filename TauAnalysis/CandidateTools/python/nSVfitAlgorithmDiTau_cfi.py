import FWCore.ParameterSet.Config as cms

from TrackingTools.TransientTrack.TransientTrackBuilder_cfi import *
from RecoMET.METProducers.METSigParams_cfi import *

nSVfitTrackService = cms.Service("NSVfitTrackService")

nSVfitElectronLikelihoodPhaseSpace = cms.PSet(
    pluginName = cms.string("nSVfitTauToElecLikelihoodPhaseSpace"),
    pluginType = cms.string("NSVfitTauToElecLikelihoodPhaseSpace"),
    verbosity = cms.int32(0)  
)

nSVfitElectronLikelihoodPolarization = cms.PSet(
    pluginName = cms.string("nSVfitTauToElecLikelihoodPolarization"),
    pluginType = cms.string("NSVfitTauToElecLikelihoodPolarization"),
    verbosity = cms.int32(0)  
)

nSVfitElectronLikelihoodTrackInfo = cms.PSet(
    pluginName = cms.string("nSVfitTauToElecLikelihoodTrackInfo"),
    pluginType = cms.string("NSVfitTauDecayLikelihoodTrackInfo"),
    useLifetimeConstraint = cms.bool(True),
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

nSVfitMuonLikelihoodPolarization = cms.PSet(
    pluginName = cms.string("nSVfitTauToMuLikelihoodPolarization"),
    pluginType = cms.string("NSVfitTauToMuLikelihoodPolarization"),
    verbosity = cms.int32(0)  
)

nSVfitMuonLikelihoodTrackInfo = cms.PSet(
    pluginName = cms.string("nSVfitTauToMuonLikelihoodTrackInfo"),
    pluginType = cms.string("NSVfitTauDecayLikelihoodTrackInfo"),
    useLifetimeConstraint = cms.bool(True),
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

nSVfitTauLikelihoodPolarization = cms.PSet(
    pluginName = cms.string("nSVfitTauToHadLikelihoodPolarization"),
    pluginType = cms.string("NSVfitTauToHadLikelihoodPolarization"),
    ##mapRecToGenTauDecayModes = cms.PSet(
    ##    fileName = cms.string("/afs/cern.ch/user/v/veelken/public/plotsAHtoMuTau.root"),
    ##    meName = cms.string('DQMData/ahMuTauAnalyzer_woBtag/afterEvtSelNonCentralJetEt20bTag/TauQuantities/TauRecVsGenDecayMode')
    ##),
    decayModeParameters = cms.PSet(
        oneProngZeroPi0s = cms.PSet(
            pMin = cms.double(0.05)
        ),
        oneProngOnePi0 = cms.PSet(
            xSigma = cms.string("0.014"),
            xBias = cms.string("0.000"),
            pMin = cms.double(0.05)
        ),
        oneProngTwoPi0s = cms.PSet(
            xSigma = cms.string("0.013"),
            xBias = cms.string("0.000"),
            pMin = cms.double(0.05)
        ),
        threeProngZeroPi0s = cms.PSet(
            xSigma = cms.string("0.018"),
            xBias = cms.string("0.000"),
            pMin = cms.double(0.05)
        )
    ),
    verbosity = cms.int32(0)  
)

nSVfitTauLikelihoodTrackInfo = cms.PSet(
    pluginName = cms.string("nSVfitTauToHadLikelihoodTrackInfo"),
    pluginType = cms.string("NSVfitTauDecayLikelihoodTrackInfo"),
    useLifetimeConstraint = cms.bool(True),
    verbosity = cms.int32(0)  
)

nSVfitTauToHadBuilder = cms.PSet(
    pluginName = cms.string("nSVfitTauToHadBuilder"),
    pluginType = cms.string("NSVfitTauToHadBuilder"),
    verbosity = cms.int32(0)  
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
    power = cms.double(1.0),  
    verbosity = cms.int32(0)     
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
        perpBias = cms.string("0.")
    ),
    power = cms.double(1.0), 
    verbosity = cms.int32(0)
)

nSVfitEventLikelihoodMEt2 = cms.PSet(
    pluginName = cms.string("nSVfitEventLikelihoodMEt2"),
    pluginType = cms.string("NSVfitEventLikelihoodMEt2"),
    srcPFJets = cms.InputTag('ak5PFJets'),
    srcPFCandidates = cms.InputTag('particleFlow'),
    resolution = METSignificance_params,
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
        srcPrimaryVertex = cms.InputTag("offlinePrimaryVerticesWithBS"),
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
                min = cms.double(10.),                            
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
    dRmin = cms.double(0.3),
    instanceLabel = cms.string("")                           
)                                
