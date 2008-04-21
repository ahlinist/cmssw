import FWCore.ParameterSet.Config as cms

process = cms.Process("Gen")
process.load("FWCore.MessageService.MessageLogger_cfi")

# control point for all seeds
#
process.load("Configuration.StandardSequences.SimulationRandomNumberGeneratorSeeds_cff")

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.load("Configuration.Spring08Production.Spring08_MinBias_Pythia_cfi")

process.load("Configuration.EventContent.EventContent_cff")

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1 $'),
    name = cms.untracked.string('$Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/Spring08Production/data/Spring08_MinBias_Pythia_GEN.cfg,v $'),
    annotation = cms.untracked.string('generation of minbias with pythia6, CTEQ 6L1 used')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10000)
)
process.GEN = cms.OutputModule("PoolOutputModule",
    process.FEVTSIMEventContent,
    fileName = cms.untracked.string('PythiaMinBias.root')
)

process.e = cms.EndPath(process.GEN)
process.schedule = cms.Schedule(process.e)


