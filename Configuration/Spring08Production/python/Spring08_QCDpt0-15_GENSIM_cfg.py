# The following comments couldn't be translated into the new config version:

# replace famosSimHits.SimulateMuons = false ?
# No miscalibration

import FWCore.ParameterSet.Config as cms

process = cms.Process("PROD")
process.load("Configuration.Spring08Production.Spring08_QCDpt0_15_cfi")

# Famos sequences ? 
process.load("FastSimulation.Configuration.CommonInputsFake_cff")

process.load("FastSimulation.Configuration.FamosSequences_cff")

process.load("FastSimulation.HighLevelTrigger.common.NoHLTSetup_cff")

# Don't know if we need that
process.load("FastSimulation.Configuration.EventContent_cff")

# Keep the logging output to a nice level #
process.load("FWCore.MessageService.MessageLogger_cfi")

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1 $'),
    name = cms.untracked.string('$Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/Spring08Production/data/Spring08_QCDpt0-15_GENSIM.cfg,v $'),
    annotation = cms.untracked.string('FastSim QCD Pthat 0-15 for Spring08')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)
process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    # This is to initialize the random engines of Famos
    moduleSeeds = cms.PSet(
        caloRecHits = cms.untracked.uint32(654321),
        MuonSimHits = cms.untracked.uint32(97531),
        muonCSCDigis = cms.untracked.uint32(525432),
        muonDTDigis = cms.untracked.uint32(67673876),
        famosSimHits = cms.untracked.uint32(13579),
        paramMuons = cms.untracked.uint32(54525),
        famosPileUp = cms.untracked.uint32(918273),
        VtxSmeared = cms.untracked.uint32(123456789),
        muonRPCDigis = cms.untracked.uint32(524964),
        siTrackerGaussianSmearingRecHits = cms.untracked.uint32(24680)
    ),
    # This is to initialize the random engine of the source
    sourceSeed = cms.untracked.uint32(123456789)
)

process.pthatFilter = cms.EDFilter("MCProcessFilter",
    MaxPthat = cms.untracked.vdouble(15.0, 15.0, 15.0, 15.0, 15.0, 
        15.0),
    ProcessID = cms.untracked.vint32(11, 12, 13, 68, 28, 
        53),
    MinPthat = cms.untracked.vdouble(0.0, 0.0, 0.0, 0.0, 0.0, 
        0.0)
)

process.Timing = cms.Service("Timing")

process.o1 = cms.OutputModule("PoolOutputModule",
    process.AODSIMEventContent,
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('AODSIM')
    ),
    fileName = cms.untracked.string('QCDpt0-15.root')
)

process.p1 = cms.Path(process.pthatFilter*process.famosWithEverything)
process.outpath = cms.EndPath(process.o1)
process.famosPileUp.UseTRandomEngine = True
process.famosSimHits.UseTRandomEngine = True
process.siTrackerGaussianSmearingRecHits.UseTRandomEngine = True
process.caloRecHits.UseTRandomEngine = True
process.paramMuons.UseTRandomEngine = True
process.famosPileUp.PileUpSimulator.averageNumber = 0.0
process.VolumeBasedMagneticFieldESProducer.useParametrizedTrackerField = True
process.famosSimHits.SimulateCalorimetry = True
process.famosSimHits.SimulateTracking = True
process.MessageLogger.destinations = ['detailedInfo.txt']

