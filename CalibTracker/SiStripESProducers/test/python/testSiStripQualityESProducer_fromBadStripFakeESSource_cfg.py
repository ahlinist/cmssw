import FWCore.ParameterSet.Config as cms

process = cms.Process("CALIB")

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring(''),
    QualityReader = cms.untracked.PSet(
        threshold = cms.untracked.string('INFO')
    ),
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('INFO')
    ),
    destinations = cms.untracked.vstring('QualityReader.log')
)

process.source = cms.Source("EmptyIOVSource",
    lastValue = cms.uint64(100),
    timetype = cms.string('runnumber'),
    firstValue = cms.uint64(1),
    interval = cms.uint64(90)
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

process.load("CalibTracker.SiStripESProducers.fake.SiStripBadModuleFakeESSource_cfi")
process.load("CalibTracker.SiStripESProducers.fake.SiStripBadFiberFakeESSource_cfi")
process.load("CalibTracker.SiStripESProducers.fake.SiStripBadChannelFakeESSource_cfi")
process.load("CalibTracker.SiStripESProducers.fake.SiStripModuleHVFakeESSource_cfi")
process.load("CalibTracker.SiStripESProducers.fake.SiStripModuleLVFakeESSource_cfi")


process.load("CalibTracker.SiStripESProducers.SiStripQualityESProducer_cfi")
process.siStripQualityESProducer.ListOfRecordToMerge = cms.VPSet(
     cms.PSet( record = cms.string("SiStripModuleHVRcd"), tag    = cms.string("") ),
     cms.PSet( record = cms.string("SiStripModuleLVRcd"), tag    = cms.string("") ),
     cms.PSet( record = cms.string("SiStripBadChannelRcd"), tag    = cms.string("") ),
     cms.PSet( record = cms.string("SiStripBadFiberRcd"),   tag    = cms.string("") ),
     cms.PSet( record = cms.string("SiStripBadModuleRcd"),  tag    = cms.string("") )
     )

process.reader = cms.EDAnalyzer("SiStripQualityStatistics",
                              dataLabel = cms.untracked.string(""),
                              TkMapFileName = cms.untracked.string("")
                              )

process.p = cms.Path(process.reader)

