import FWCore.ParameterSet.Config as cms

process = cms.Process("Gen")
process.load("FWCore.MessageService.MessageLogger_cfi")

# control point for all seeds
#
process.load("Configuration.StandardSequences.SimulationRandomNumberGeneratorSeeds_cff")

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.load("Configuration.Spring08Production.Spring08_BCToEFilter_QCD_Pt_30_80_10TeV_cfi")

process.load("GeneratorInterface.GenFilters.BCToEFilter_cfi")

#these are needed by the filter
process.load("PhysicsTools.HepMCCandAlgos.genParticles_cfi")

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
    annotation = cms.untracked.string('generation of QCD pthat 30-80, with bctoe filter applied')
)
process.GEN = cms.OutputModule("PoolOutputModule",
    process.FEVTSIMEventContent,
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('p1')
    ),
    fileName = cms.untracked.string('GEN.root')
)

process.p1 = cms.Path(process.genParticles*process.bctoefilter)
process.outpath = cms.EndPath(process.GEN)
process.schedule = cms.Schedule(process.p1,process.outpath)


