# The following comments couldn't be translated into the new config version:

# upload to database 

#string timetype = "timestamp"    

import FWCore.ParameterSet.Config as cms

process = cms.Process("Builder")

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring(''),
    LorentzAngleBuilder = cms.untracked.PSet(
        threshold = cms.untracked.string('INFO')
    ),
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('INFO')
    ),
    destinations = cms.untracked.vstring('LorentzAngleBuilder.log')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)
process.source = cms.Source("EmptySource",
    numberEventsInRun = cms.untracked.uint32(1),
    firstRun = cms.untracked.uint32(1)
)

process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")

process.load("CalibTracker.SiStripESProducers.fake.SiStripLorentzAngleFakeESSource_cfi")
process.load("CalibTracker.SiStripESProducers.DBWriter.SiStripLorentzAngleDummyDBWriter_cfi")

process.PoolDBOutputService = cms.Service("PoolDBOutputService",
    connect = cms.string('sqlite_file:dbfile.db'),
    timetype = cms.untracked.string('runnumber'),    
    BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
    DBParameters = cms.PSet(
        messageLevel = cms.untracked.int32(2),
        authenticationPath = cms.untracked.string('/afs/cern.ch/cms/DB/conddb')
    ),
    
    toPut = cms.VPSet(cms.PSet(
        record = cms.string('SiStripLorentzAngleRcd'),
        tag = cms.string('SiStripLorentzAngle_Ideal_31X')
    ))
)

process.siStripLorentzAngleDummyDBWriter.record=process.PoolDBOutputService.toPut[0].record

# Three possible generations:
# - give two values = (min,max)                     -> uniform distribution
# - give one value and PerCent_Err != 0             -> gaussian distribution
# - either give two equal values or a single value  -> fixed value
process.SiStripLorentzAngleGenerator.TIB_EstimatedValueMinMax = cms.vdouble(0.014, 0.024)
process.SiStripLorentzAngleGenerator.TOB_EstimatedValueMinMax = cms.vdouble(0.021, 0.027)
process.SiStripLorentzAngleGenerator.TIB_PerCent_Err=cms.double(0.)
process.SiStripLorentzAngleGenerator.TOB_PerCent_Err=cms.double(0.)

process.p1 = cms.Path(process.siStripLorentzAngleDummyDBWriter)


