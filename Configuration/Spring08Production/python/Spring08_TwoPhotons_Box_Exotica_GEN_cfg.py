import FWCore.ParameterSet.Config as cms

process = cms.Process("Gen")
process.load("FWCore.MessageService.MessageLogger_cfi")

# control point for all seeds
#
process.load("Configuration.StandardSequences.SimulationRandomNumberGeneratorSeeds_cff")

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

# physics event generation
#  
process.load("Configuration.Spring08Production.Spring08_TwoPhotons_Box_Exotica_Pythia_cfi")

process.load("Configuration.EventContent.EventContent_cff")

process.maxEvents = cms.untracked.PSet(
    output = cms.untracked.int32(1)
)
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1 $'),
    name = cms.untracked.string('$Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/Spring08Production/data/Spring08_TwoPhotons_Box_Exotica_GEN.cfg,v $'),
    annotation = cms.untracked.string('generation of gamma+jets, CTEQ 6L1 used')
)
process.GEN = cms.OutputModule("PoolOutputModule",
    process.FEVTSIMEventContent,
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN')
    ),
    fileName = cms.untracked.string('PythiaBoxExotica.root')
)

process.outpath = cms.EndPath(process.GEN)
process.schedule = cms.Schedule(process.outpath)


