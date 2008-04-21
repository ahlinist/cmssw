import FWCore.ParameterSet.Config as cms

process = cms.Process("Gen")
process.load("FWCore.MessageService.MessageLogger_cfi")

# control point for all seeds
#
process.load("Configuration.StandardSequences.SimulationRandomNumberGeneratorSeeds_cff")

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

# physics event generation
#  
process.load("Configuration.Spring08Production.Spring08_MinBias_Pythia_cfi")

process.load("Configuration.EventContent.EventContent_cff")

process.maxEvents = cms.untracked.PSet(
    output = cms.untracked.int32(100)
)
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string(''),
    name = cms.untracked.string(''),
    annotation = cms.untracked.string('Spring08 b-enriched minbias with Pythia')
)
process.bfilter = cms.EDFilter("PythiaFilter",
    ParticleID = cms.untracked.int32(5)
)

process.GEN = cms.OutputModule("PoolOutputModule",
    process.FEVTSIMEventContent,
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('p1')
    ),
    fileName = cms.untracked.string('Spring08_MinBias_BB_Pythia_GEN.root')
)

process.p1 = cms.Path(process.bfilter)
process.e = cms.EndPath(process.GEN)
process.schedule = cms.Schedule(process.p1,process.e)


