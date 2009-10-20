import FWCore.ParameterSet.Config as cms

process = cms.Process('TEST')
process.load("FWCore.Framework.test.cmsExceptionsFatal_cff")

process.load("Configuration.StandardSequences.Services_cff")


##########################################################################################################
#              services
##########################################################################################################


process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    moduleSeeds = cms.PSet(
        g4SimHits = cms.untracked.uint32(311422),
        mix = cms.untracked.uint32(123215),
        VtxSmeared = cms.untracked.uint32(9823432),
        caloRecHits = cms.untracked.uint32(6123321),
        MuonSimHits = cms.untracked.uint32(951331),
        muonCSCDigis = cms.untracked.uint32(2514232),
        muonDTDigis = cms.untracked.uint32(667376),
        famosSimHits = cms.untracked.uint32(357319),
        famosPileUp = cms.untracked.uint32(98273),
        l1ParamMuons = cms.untracked.uint32(870926),
        paramMuons = cms.untracked.uint32(541225),
        muonRPCDigis = cms.untracked.uint32(2146964),
        siTrackerGaussianSmearingRecHits = cms.untracked.uint32(321480),
        simMuonCSCDigis = cms.untracked.uint32(121245),
        simMuonDTDigis = cms.untracked.uint32(123115),
        simMuonRPCDigis = cms.untracked.uint32(1215235),
        generator = cms.untracked.uint32(10000),
        ecalRecHit = cms.untracked.uint32(18734),
        ecalPreshowerRecHit = cms.untracked.uint32(18734),
        hbhereco = cms.untracked.uint32(18734),
        horeco = cms.untracked.uint32(18734),
        hfreco = cms.untracked.uint32(18734)
    ), 
    sourceSeed = cms.untracked.uint32(10000)
)
from IOMC.RandomEngine.RandomServiceHelper import  RandomNumberServiceHelper
randHelper =  RandomNumberServiceHelper(process.RandomNumberGeneratorService)
randHelper.populate()
process.RandomNumberGeneratorService.saveFileName =  cms.untracked.string("RandomEngineState.log")


process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(9000)
)

#process.MessageLogger.categories.append('L1GtTrigReport')
#process.MessageLogger.categories.append('HLTrigReport')

process.MessageLogger.cerr.noTimeStamps = cms.untracked.bool(True)
process.MessageLogger.cerr.threshold=cms.untracked.string('INFO')

##########################################################################################################
#              generation step
##########################################################################################################

process.source = cms.Source("EmptySource")

from Configuration.Generator.PythiaUESettings_cfi import *

process.generator = cms.EDFilter("Pythia6GeneratorFilter",
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    maxEventsToPrint = cms.untracked.int32(1),
    pythiaPylistVerbosity = cms.untracked.int32(0),
    pythiaFrame = cms.string('CMS'),
    comEnergy = cms.double(10000.0),
    PythiaParameters = cms.PSet(
        pythiaUESettingsBlock,
        bbbarSettings = cms.vstring('MSEL=5          ! b events'),
        # This is a vector of ParameterSet names to be read, in this order
        parameterSets = cms.vstring('pythiaUESettings','bbbarSettings')
    ),
    ExternalDecays = cms.PSet(
        EvtGen = cms.untracked.PSet(
             operates_on_particles = cms.vint32(0), 
             use_default_decay = cms.untracked.bool(False),
             decay_table = cms.FileInPath('GeneratorInterface/ExternalDecays/data/DECAY.DEC'),
             # decay_table = cms.FileInPath('GeneratorInterface/ExternalDecays/data/DECAY_NOLONGLIFE.DEC'),             
             particle_property_file = cms.FileInPath('GeneratorInterface/ExternalDecays/data/evt.pdl'),
             user_decay_file = cms.FileInPath('HeavyFlavorAnalysis/Examples/data/Bd2JpsiKstar.dec'),
             list_forced_decays = cms.vstring("MyB0", "anti-MyB0")
        ),
        parameterSets = cms.vstring('EvtGen')
    )
)

process.HepPDTESSource = cms.ESSource("HepPDTESSource",
    pdtFileName = cms.FileInPath('SimGeneral/HepPDTESSource/data/particle.tbl')
)

process.JPsiFilter = cms.EDFilter(
    "MCSingleParticleFilter",
    MaxEta = cms.untracked.vdouble(2.5),
    MinEta = cms.untracked.vdouble(-2.5),
    Status = cms.untracked.vint32(0),
    MinPt = cms.untracked.vdouble(-0.1),
    ParticleID = cms.untracked.vint32(443)
)


process.BdFilter = cms.EDFilter(
    "MCSingleParticleFilter",
    MaxEta = cms.untracked.vdouble(25.),
    MinEta = cms.untracked.vdouble(-25.),
    Status = cms.untracked.vint32(0),
    MinPt = cms.untracked.vdouble(-0.1),
    ParticleID = cms.untracked.vint32(511, -511)
)

process.mumugenFilter = cms.EDFilter("MCParticlePairFilter",
    moduleLabel = cms.untracked.string('generator'),
    Status = cms.untracked.vint32(1, 1),
    MinPt = cms.untracked.vdouble(2.5, 2.5),
    MaxEta = cms.untracked.vdouble(2.5, 2.5),
    MinEta = cms.untracked.vdouble(-2.5, -2.5),
    ParticleCharge = cms.untracked.int32(0),
    # Use the following to require J/psi -> mu+ mu- 
    # MaxInvMass = cms.untracked.double(3.15),
    # MinInvMass = cms.untracked.double(3.05),
    ParticleID1 = cms.untracked.vint32(13),
    ParticleID2 = cms.untracked.vint32(13)
)



##########################################################################################################
#              fast simulation step
##########################################################################################################

# Famos sequences (Frontier conditions)
process.load("FastSimulation.Configuration.CommonInputs_cff")
process.GlobalTag.globaltag = "STARTUP31X_V2::All"
#process.GlobalTag.globaltag = "MC_31X_V3::All"

# Famos sequences
process.load("FastSimulation.Configuration.FamosSequences_cff")

# L1 Emulator and HLT Setup
process.load("FastSimulation.HighLevelTrigger.HLTSetup_cff")

process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')

# Parametrized magnetic field
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
#process.VolumeBasedMagneticFieldESProducer.useParametrizedTrackerField = True

# HLT paths -- defined from ConfigDB
process.load("FastSimulation.Configuration.HLT_8E29_cff")


# You many not want to simulate everything
process.famosSimHits.SimulateCalorimetry = True
process.famosSimHits.SimulateTracking = True
process.famosSimHits.SimulateMuons = True

# Number of pileup events per crossing
process.famosPileUp.PileUpSimulator.averageNumber = 0.0
process.famosSimHits.ActivateDecays.comEnergy = 10000


process.load("FastSimulation.Configuration.EventContent_cff")



##########################################################################################################
#              output step
##########################################################################################################

process.genParticles = cms.EDProducer("GenParticleProducer",
                                      src = cms.InputTag( "generator" ),
                                      saveBarCodes = cms.untracked.bool(True),
                                      abortOnUnknownPDGCode = cms.untracked.bool(False)
)

process.genParticlesPlusGEANT = cms.EDProducer("GenPlusSimParticleProducer",
                                              #src           = cms.InputTag("g4SimHits"),    # use "famosSimHits" for FAMOS
                                              src           = cms.InputTag("famosSimHits"),  # use "famosSimHits" for FAMOS
                                              setStatus     = cms.int32(8),                 # set status = 8 for GEANT GPs
                                              genParticles   = cms.InputTag("genParticles") # original genParticle list
)


process.tree = cms.EDFilter("HFTree",
    fileName = cms.string('Bd2JpsiKstar.root')
)

process.genDump = cms.EDFilter("HFDumpGenerator",
                               # generatorCandidates = cms.untracked.string('genParticles'),
                               generatorCandidates = cms.untracked.string('genParticlesPlusGEANT'),
                               generatorEvent = cms.untracked.string('generator'),
                               verbose = cms.untracked.int32(0)
)

process.load("SimTracker.TrackAssociation.TrackAssociatorByChi2_cfi")
process.load("SimTracker.TrackAssociation.TrackAssociatorByHits_cfi")

process.trkDump = cms.EDFilter("HFDumpTracks",
    verbose = cms.untracked.int32(0),
    tracksLabel = cms.untracked.string('generalTracks'),
    simTracksLabel = cms.untracked.string('famosSimHits'),
    generatorEventLabel = cms.untracked.string('generator'),
    muonsLabel = cms.untracked.InputTag("muons"),
    trackingParticlesLabel = cms.untracked.string('trackingParticles'),
    associatorLabel = cms.untracked.string('TrackAssociatorByHits'),
    doTruthMatching = cms.untracked.int32(2)
)

process.muonDump = cms.EDFilter("HFDumpMuons",
    verbose = cms.untracked.int32(0),
    muonsLabel = cms.untracked.InputTag("muons"),
    doTruthMatching = cms.untracked.int32(2),
)


# ----------------------------------------------------------------------
process.triggerDump = cms.EDFilter("HFDumpTrigger",
   verbose                 = cms.untracked.int32(0),
   L1GTReadoutRecordLabel  = cms.untracked.string("gtDigis"), 
   hltL1GtObjectMap        = cms.untracked.InputTag("hltL1GtObjectMap"), 
   L1MuonsLabel            = cms.untracked.InputTag("hltL1extraParticles"), 
   HLTResultsLabel         = cms.untracked.InputTag("TriggerResults::HLT"), 
   TriggerEventLabel       = cms.untracked.InputTag("hltTriggerSummaryAOD::HLT"), 
   hltLabel      = cms.untracked.InputTag("TriggerResults::HLT"), 
)

process.stuffDump = cms.EDFilter(
    "HFDumpStuff",
    genEventScale = cms.untracked.string('generator'),
    CandidateLabel = cms.untracked.string('allMuons'),
    PrimaryVertexLabel = cms.untracked.string('offlinePrimaryVertices')
    )

process.signalDump = cms.EDFilter(
    "HFBd2JpsiKstar",
    deltaR = cms.untracked.double(18.),
    verbose = cms.untracked.int32(0),
    muonsLabel = cms.untracked.InputTag("muons"),
    muonPt = cms.untracked.double(3.0),
    trackPt = cms.untracked.double(0.4),
    tracksLabel = cms.untracked.string('generalTracks'),
    PrimaryVertexLabel = cms.untracked.string('offlinePrimaryVertices')
    )



##########################################################################################################
#              scheduling
##########################################################################################################
# Path and EndPath definitions
process.generation_step = cms.Sequence(cms.SequencePlaceholder("randomEngineStateProducer")*
                                       process.generator*
                                       process.BdFilter*process.JPsiFilter*process.mumugenFilter
)

# Simulation sequence
process.simulation = cms.Sequence(process.generation_step*process.simulationWithFamos)
process.HLTEndSequence = cms.Sequence(process.dummyModule)

process.schedule = cms.Schedule()
process.schedule.extend(process.HLTSchedule)

process.outpath = cms.EndPath(process.BdFilter*process.JPsiFilter*process.mumugenFilter*
                              process.reconstructionWithFamos*
                              process.genParticles*
                              process.genParticlesPlusGEANT*
                              process.genDump*
                              process.trkDump*
                              process.muonDump*
#                              process.triggerDump*
                              process.stuffDump*
                              process.signalDump*
                              process.tree
)


process.schedule.append(process.outpath)

