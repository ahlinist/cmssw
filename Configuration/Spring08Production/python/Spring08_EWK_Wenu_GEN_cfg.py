import FWCore.ParameterSet.Config as cms

process = cms.Process("Gen")
process.load("FWCore.MessageService.MessageLogger_cfi")

# control point for all seeds
process.load("Configuration.StandardSequences.SimulationRandomNumberGeneratorSeeds_cff")

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

# PYTHIA input file
process.load("Configuration.Spring08Production.Spring08_EWK_Wenu_10TeV_cfi")

process.load("Configuration.EventContent.EventContent_cff")

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1 $'),
    name = cms.untracked.string('$Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/Spring08Production/data/Spring08_EWK_Wenu_GEN.cfg,v $'),
    annotation = cms.untracked.string('Wenu GEN evts for iCSA08')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)
process.GEN = cms.OutputModule("PoolOutputModule",
    process.FEVTSIMEventContent,
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('egenfilterPath')
    ),
    fileName = cms.untracked.string('EWK_Wenu_GEN.root')
)

process.egenfilterPath = cms.Path(process.egenfilter)
process.e = cms.EndPath(process.GEN)
process.schedule = cms.Schedule(process.egenfilterPath,process.e)


