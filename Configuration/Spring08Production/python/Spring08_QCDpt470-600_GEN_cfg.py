import FWCore.ParameterSet.Config as cms

process = cms.Process("Gen")
process.load("FWCore.MessageService.MessageLogger_cfi")

# control point for all seeds
process.load("Configuration.StandardSequences.SimulationRandomNumberGeneratorSeeds_cff")

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.load("Configuration.Spring08Production.Spring08_QCDpt470_600_cfi")

process.load("Configuration.EventContent.EventContent_cff")

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1 $'),
    name = cms.untracked.string('$Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/Spring08Production/data/Spring08_QCDpt470-600_GEN.cfg,v $'),
    annotation = cms.untracked.string('FastSim QCD Pthat 470-600 for Spring08')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)
process.GEN = cms.OutputModule("PoolOutputModule",
    process.FEVTSIMEventContent,
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN')
    ),
    fileName = cms.untracked.string('QCDpt470-600.root')
)

process.e = cms.EndPath(process.GEN)
process.schedule = cms.Schedule(process.e)


