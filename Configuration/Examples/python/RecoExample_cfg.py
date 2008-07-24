import FWCore.ParameterSet.Config as cms

process = cms.Process("RP")
process.load("Configuration.StandardSequences.Services_cff")

process.load("Configuration.StandardSequences.GeometryPilot2_cff")

process.load("Configuration.StandardSequences.MagneticField_38T_cff")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.RawToDigi_cff")

process.load("Configuration.StandardSequences.Reconstruction_cff")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

process.load("Configuration.EventContent.EventContent_cff")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)
process.source = cms.Source("PoolSource",
    catalog = cms.untracked.string('PoolFileCatalog.xml'),
    fileNames = cms.untracked.vstring('/store/relval/2008/6/20/RelVal-RelValTTbar-1213920853/0000/028CEBF9-A53E-DD11-BF35-00161757BF42.root')
)

#process.Timing = cms.Service("Timing")


process.RECO = cms.OutputModule("PoolOutputModule",
    process.RECOEventContent,
    fileName = cms.untracked.string('reco.root')
)

process.p1 = cms.Path(process.RawToDigi*process.reconstruction)
process.outpath = cms.EndPath(process.RECO)
process.GlobalTag.globaltag = "IDEAL_V5::All"

