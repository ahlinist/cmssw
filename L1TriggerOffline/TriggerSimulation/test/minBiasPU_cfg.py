import FWCore.ParameterSet.Config as cms

process = cms.Process("PROD")
process.load("L1TriggerOffline.TriggerSimulation.pythiaMinBias_cfi")

process.load("L1TriggerOffline.TriggerSimulation.slhcPileUp_cfi")

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.load("L1TriggerOffline.TriggerSimulation.TrackTriggerNaiveGeometry_cff")
process.load("L1TriggerOffline.TriggerSimulation.trackTriggerHitsFromMC_cfi")
process.trackTriggerHits.inputTag = 'source'
process.trackTriggerHits.doPileUp = True

#process.load("L1TriggerOffline.TriggerSimulation.dumpL1TrackHits_cfi")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)
process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    moduleSeeds = cms.PSet(
        mix = cms.untracked.uint32(54525)
    ),
    sourceSeed = cms.untracked.uint32(123456789)
)

process.output = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_l1TrackerHits_*_*', 
        'keep *_l1TrackStubs_*_*'),
    fileName = cms.untracked.string('minBiasPU.root')
)

process.p = cms.Path(process.mix*process.trackTriggerHits)
process.outpath = cms.EndPath(process.output)
#Xprocess.PythiaSource.firstEvent = 1

