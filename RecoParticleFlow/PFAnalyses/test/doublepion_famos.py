import FWCore.ParameterSet.Config as cms

process = cms.Process("PROD")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(2000)
)

#fastsim
process.load("FastSimulation.Configuration.RandomServiceInitialization_cff")
process.load("FastSimulation.Configuration.CommonInputs_cff")
process.load("FastSimulation.Configuration.FamosSequences_cff")
process.GlobalTag.globaltag = "IDEAL_30X::All"


process.famosSimHits.SimulateCalorimetry = True
process.famosSimHits.SimulateTracking = True
process.famosPileUp.PileUpSimulator.averageNumber = 0.0

process.famosSimHits.VertexGenerator.BetaStar = 0.00001
process.famosSimHits.VertexGenerator.SigmaZ = 0.00001

# Parametrized magnetic field (new mapping, 4.0 and 3.8T)
process.load("Configuration.StandardSequences.MagneticField_0T_cff")
#process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.VolumeBasedMagneticFieldESProducer.useParametrizedTrackerField = True

process.famosSimHits.MaterialEffects.PairProduction = False
process.famosSimHits.MaterialEffects.Bremsstrahlung = False
process.famosSimHits.MaterialEffects.EnergyLoss = False
process.famosSimHits.MaterialEffects.MultipleScattering = False
process.famosSimHits.MaterialEffects.NuclearInteraction = False
process.famosSimHits.Calorimetry.UnfoldedMode = True

process.load("RecoParticleFlow.PFBlockProducer.particleFlowSimParticle_cff")

process.source = cms.Source("FlatRandomEGunSource",
    PGunParameters = cms.untracked.PSet(
        PartID = cms.untracked.vint32(211),
        MaxEta = cms.untracked.double(0.5),
        MaxPhi = cms.untracked.double(3.14159265359),
        MinEta = cms.untracked.double(0.0),
        MinE = cms.untracked.double(2.0),
        DoubleParticle = cms.untracked.bool(True),
        MinPhi = cms.untracked.double(-3.14159265359),
        MaxE = cms.untracked.double(2.01)
    ),
    Verbosity = cms.untracked.int32(1),
    AddAntiParticle = cms.untracked.bool(True)
)


process.particleFlowRecHitHCAL.thresh_Barrel = cms.double(0.0)
process.particleFlowClusterHCAL.thresh_Seed_Barrel = cms.double(1.4)
process.particleFlowClusterHCAL.thresh_Barrel = cms.double(0.8)
process.particleFlowBlock.pf_chi2_ECAL_HCAL = cms.double(100.0)


process.dump = cms.EDAnalyzer("EventContentAnalyzer")

process.extraction = cms.EDAnalyzer("ExtractionAnalyzer",
    useSimAsTrack = cms.bool(False),
    PFClustersHcal = cms.InputTag("particleFlowClusterHCAL"),
    deltaRClustersToTrack = cms.double(0.4),
    EventDelegateType = cms.string('DipionDelegate'),
    deltaRCandToTrack = cms.double(0.4),
    PFCandidates = cms.InputTag("particleFlow"),
    deltaRRechitsToTrack = cms.double(0.4),
    deltaPhi = cms.double(0.4),
    debug = cms.int32(1),
    PFRecTracks = cms.InputTag("elecpreid"),
    PFRecHitsEcal = cms.InputTag("particleFlowRecHitECAL"),
    deltaEta = cms.double(0.4),
    PFSimParticles = cms.InputTag("particleFlowSimParticle"),
    PFRecHitsHcal = cms.InputTag("particleFlowRecHitHCAL"),
    isMC = cms.bool(True),
    PFClustersEcal = cms.InputTag("particleFlowClusterECAL"),
    clustersFromCandidates = cms.bool(True),
    particlePDG = cms.int32(211),
    SimCaloRecHitsEcalEB = cms.InputTag("famosSimHits", "EcalHitsEB"),
    SimCaloRecHitsEcalEE = cms.InputTag("famosSimHits", "EcalHitsEE"),
    SimCaloRecHitsHcal = cms.InputTag("famosSimHits", "HcalHits")
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('DipionDelegate_mono2GeV_BarrelOnly_2k_3_1.root')
    #fileName = cms.string('DipionDelegate_mono25GeV_3_X_test.root')
)

#process.finishup = cms.OutputModule("PoolOutputModule",
#   fileName=cms.untracked.string("DipionDelegate_mono50GeV_3_1_Collections.root"),
#    outputCommands=cms.untracked.vstring('keep *')
#    
#)


process.p1 = cms.Path(process.famosWithEverything+process.caloJetMetGen*process.particleFlowSimParticle)
#process.p1 = cms.Path(process.famosWithEverything+process.caloJetMetGen*process.particleFlowSimParticle*process.extraction)
#process.outpath = cms.EndPath(process.finishup)




