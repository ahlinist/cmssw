import FWCore.ParameterSet.Config as cms

process = cms.Process("EcalTimeCalibrationValidator")

# shaping our Message logger to suit our needs
process.MessageLogger = cms.Service("MessageLogger",
    cout = cms.untracked.PSet(threshold = cms.untracked.string('INFO')),
    categories = cms.untracked.vstring('*'),
    destinations = cms.untracked.vstring('cout')
)

process.expressValidator = cms.EDAnalyzer("EcalTimeCalibrationValidator",
  #InputTreeFileName = cms.string("file:/data2/scooper/data/OutOfTimeBuckets_August2010/cafJobs/fill1089/EcalTimePi0Tree_135149_30_1_la6.root"),
  InputTreeFileName = cms.string("file:EcalTimeTree_147114_9_1_uaG.root"),
  OutputTreeFileName = cms.string("file:test.tree.root"),
  CalibConstantXMLFileName = cms.string("EcalTimeCalibConstants-134520-134663.xml"),
  MaxTreeEntriesToProcess = cms.untracked.int32(100000000)
)

process.maxEvents = cms.untracked.PSet(  input = cms.untracked.int32(1) )
process.source = cms.Source("EmptySource")

process.p = cms.Path(process.expressValidator)
