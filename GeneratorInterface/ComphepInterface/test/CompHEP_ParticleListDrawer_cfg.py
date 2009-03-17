import FWCore.ParameterSet.Config as cms

process = cms.Process("Print")

process.source = cms.Source("PoolSource",
 	  fileNames = cms.untracked.vstring('file:comphep_source.root'),
##        fileNames = cms.untracked.vstring('file:comphep_producer.root'),                   
	skipEvents = cms.untracked.uint32(0)
)

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.particleListDrawer = cms.EDAnalyzer(
    "ParticleListDrawer",
    printOnlyHardInteraction = cms.untracked.bool(False),
    maxEventsToPrint = cms.untracked.int32(1),
    src = cms.InputTag('genParticles')
  )




process.p = cms.Path(process.particleListDrawer)
