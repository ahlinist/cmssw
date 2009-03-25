import FWCore.ParameterSet.Config as cms

process = cms.Process("PROD")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string("$Revision: 1.3 $"),
    name = cms.untracked.string("$Source: /cvs_server/repositories/CMSSW/CMSSW/L1Trigger/CSCTriggerPrimitives/test/EvtGen+DetSim+Digi+CscLCTs_cfg.py,v $"),
    annotation = cms.untracked.string("SV: single particle gun mu- 50 GeV")
)

process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring("cout"),
    categories = cms.untracked.vstring("FwkJob"),
    cout = cms.untracked.PSet(
        # untracked PSet default = { untracked int32 limit = 0 }  # kill all messages in the log
        # untracked PSet FwkJob  = { untracked int32 limit = -1 } # except *all* of FwkJob's
        # threshold = cms.untracked.string("DEBUG")
        threshold = cms.untracked.string("INFO")
        )#,
    #debugModules = cms.untracked.vstring("cscTriggerPrimitiveDigis","cscpacker")
)

# Random number seeds
#
process.load("Configuration/StandardSequences/SimulationRandomNumberGeneratorSeeds_cff")
process.RandomNumberGeneratorService.simMuonCSCDigis.initialSeed = 468
#- Randomize all the seeds
#from IOMC.RandomEngine.RandomServiceHelper import RandomNumberServiceHelper
#randSvc = RandomNumberServiceHelper(process.RandomNumberGeneratorService)
#randSvc.populate()

# Event Generation.  Single muons
#
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.source = cms.Source("EmptySource")
process.generator = cms.EDProducer("FlatRandomPtGunProducer",
    firstRun = cms.untracked.uint32(1),
    PGunParameters = cms.PSet(
        # you can request more than 1 particle
        # PartID = cms.vint32(211,11,-13),
        PartID = cms.vint32(13),
        MinEta = cms.double(-2.5),
        MaxEta = cms.double(2.5),
        MinPhi = cms.double(-3.14159265359), ## must be in rads
        MaxPhi = cms.double(3.14159265359),
        MinPt  = cms.double(49.99),
        MaxPt  = cms.double(50.01)
    ),
    AddAntiParticle = cms.bool(False),
    Verbosity = cms.untracked.int32(0) ## set to 1 (or greater) for printouts
)

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'IDEAL_30X::All'
#process.GlobalTag.globaltag = 'STARTUP_V5::All'
#process.prefer("GlobalTag")

# Event vertex smearing - applies only once (internal check)
# Note : all internal generators will always do (0,0,0) vertex
#
process.load("Configuration.StandardSequences.VtxSmearedGauss_cff")

process.load("Configuration.StandardSequences.Services_cff")

process.load("Configuration.StandardSequences.Simulation_cff")

process.load('Configuration/StandardSequences/GeometryPilot2_cff')

process.load("Configuration.StandardSequences.MagneticField_cff")

process.load("Configuration.StandardSequences.MixingNoPileUp_cff")

process.load("Configuration.StandardSequences.Reconstruction_cff")

#- CSC TP Emulator
process.load("L1TriggerConfig.L1CSCTPConfigProducers.L1CSCTriggerPrimitivesConfig_cff")
process.load("L1Trigger.CSCTriggerPrimitives.cscTriggerPrimitiveDigis_cfi")
process.cscTriggerPrimitiveDigis.alctParamMTCC2.verbosity = 2
process.cscTriggerPrimitiveDigis.clctParamMTCC2.verbosity = 2
process.cscTriggerPrimitiveDigis.tmbParam.verbosity = 2

#- CSC digi->raw
process.load("EventFilter.CSCRawToDigi.cscPacker_cfi")
process.cscpacker.alctDigiTag = cms.InputTag("cscTriggerPrimitiveDigis")
process.cscpacker.clctDigiTag = cms.InputTag("cscTriggerPrimitiveDigis")
process.cscpacker.correlatedLCTDigiTag = cms.InputTag("cscTriggerPrimitiveDigis","MPCSORTED")

# Output module
#
process.GENSIMDIGI = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string("/data0/slava/test/muminus_pt50_CMSSW_3_1_0_pre4.root"),
    outputCommands = cms.untracked.vstring("keep *", 
         # "drop *_simSiPixelDigis_*_*",
         # "drop *_simSiStripDigis_*_*",
         # "drop *_simEcalUnsuppressedDigis_*_*",
         # "drop *_simHcalDigis_*_*",
        "drop *_simMuonDTDigis_*_*", 
        "drop *_simMuonRPCDigis_*_*", 
        "drop PSimHits_*_*_*", 
        "keep PSimHits_*_MuonCSCHits_*", 
        "drop PCaloHits_*_*_*")
)

# Now order modules for execution
#
#sequence trDigi = {  simSiPixelDigis & simSiStripDigis }
#sequence calDigi = { ecalUnsuppressedDigis & hcalDigis }
#sequence muonDigi = { simMuonCSCDigis &  simMuonDTDigis & simMuonRPCDigis}
#sequence doDigi = { trDigi & calDigi & muonDigi }
#- Gen->Sim->Digi
process.p1 = cms.Path(process.generator*process.VtxSmeared*process.g4SimHits*process.mix*process.muonDigi)
#- Gen->Sim->Digi->L1->Raw
#process.p1 = cms.Path(process.VtxSmeared*process.g4SimHits*process.mix*process.muonDigi*process.cscTriggerPrimitiveDigis*process.cscpacker)
process.outpath = cms.EndPath(process.GENSIMDIGI)
