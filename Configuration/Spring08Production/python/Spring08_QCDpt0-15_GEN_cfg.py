import FWCore.ParameterSet.Config as cms

process = cms.Process("Gen")
process.load("FWCore.MessageService.MessageLogger_cfi")

# control point for all seeds
process.load("Configuration.StandardSequences.SimulationRandomNumberGeneratorSeeds_cff")

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.load("Configuration.Spring08Production.Spring08_QCDpt0_15_cfi")

# Needed for the filter
process.load("FastSimulation.Configuration.CommonInputsFake_cff")

process.load("FastSimulation.Configuration.FamosSequences_cff")

process.load("FastSimulation.HighLevelTrigger.common.NoHLTSetup_cff")

process.load("Configuration.EventContent.EventContent_cff")

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1 $'),
    name = cms.untracked.string('$Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/Spring08Production/data/Spring08_QCDpt0-15_GEN.cfg,v $'),
    annotation = cms.untracked.string('FastSim QCD Pthat 0-15 for Spring08')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)
process.pthatFilter = cms.EDFilter("MCProcessFilter",
    MaxPthat = cms.untracked.vdouble(15.0, 15.0, 15.0, 15.0, 15.0, 
        15.0),
    ProcessID = cms.untracked.vint32(11, 12, 13, 68, 28, 
        53),
    MinPthat = cms.untracked.vdouble(0.0, 0.0, 0.0, 0.0, 0.0, 
        0.0)
)

process.GEN = cms.OutputModule("PoolOutputModule",
    process.FEVTSIMEventContent,
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('p1')
    ),
    fileName = cms.untracked.string('QCDpt0-15.root')
)

process.p1 = cms.Path(process.pthatFilter)
process.e = cms.EndPath(process.GEN)
process.schedule = cms.Schedule(process.p1,process.e)


