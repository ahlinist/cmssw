import FWCore.ParameterSet.Config as cms

process = cms.Process( "sistriplaserDQMLive" )
process.MessageLogger = cms.Service( "MessageLogger",
  cerr = cms.untracked.PSet(threshold = cms.untracked.string( 'ERROR' )),
  cout = cms.untracked.PSet(threshold = cms.untracked.string( 'INFO' )),
  destinations = cms.untracked.vstring( 'cout', 'cerr' )
)
#----------------------------
# Event Source
#-----------------------------
#process.load("DQM.Integration.test.inputsource_cfi")
#process.EventStreamHttpReader.consumerName = 'SiStrip DQM Consumer'
#process.EventStreamHttpReader.SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('*'))   
#process.EventStreamHttpReader.sourceURL = cms.string('http://dqm-c2d07-30.cms:22100/urn:xdaq-application:lid=30')
process.source = cms.Source( "PoolSource",
  skipEvents = cms.untracked.uint32( 15000 ), 
  fileNames = cms.untracked.vstring(
    '/store/data/Run2010A/TestEnables/RAW/v1/000/140/124/56E00D1B-308F-DF11-BE54-001D09F24691.root'
  )
)
process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32( 5000 )
  #input = cms.untracked.int32( -1 )
)
process.load( "EventFilter.SiStripRawToDigi.SiStripDigis_cfi" )
process.siStripDigis.ProductLabel = "source"#"hltCalibrationRaw"
process.load( "Configuration.StandardSequences.FrontierConditions_GlobalTag_cff" )
process.GlobalTag.globaltag = cms.string('GR_R_311_V1::All')
#----------------------------
# DQM Environment
#-----------------------------
process.load("DQMServices.Core.DQM_cfg" )
process.load("DQMServices.Components.DQMEnvironment_cfi")
#----------------------------
# DQM Live Environment
#-----------------------------
process.load("DQM.Integration.test.environment_cfi")
process.dqmEnv.subSystemFolder    = "SiStripLAS"
#process.dqmSaver.producer = "Playback"
process.dqmSaver.saveByTime = 1 # 1. if running online
process.dqmSaver.saveByMinute = 30
process.dqmSaver.saveByRun = 1 # 1 if running offline/ -1 online
process.dqmSaver.dirName = '.'
process.DQM.collectorPort = 9190
process.DQM.collectorHost = "lx1b10" # machine name with the GUI installed
#----------------------------
# DQM Alignment Software
#----------------------------
process.load( "DQM.SiStripLASMonitor.LaserAlignmentProducerDQM_cfi" )
process.LaserAlignmentProducerDQM.DigiProducerList = cms.VPSet(
  cms.PSet(
    DigiLabel = cms.string( 'ZeroSuppressed' ),
    DigiType = cms.string( 'Processed' ),
    DigiProducer = cms.string( 'siStripDigis' )
  )
)
process.LaserAlignmentProducerDQM.FolderName = "SiStripLAS"
process.LaserAlignmentProducerDQM.UpperAdcThreshold = cms.uint32( 280 )

process.seqDigitization = cms.Path( process.siStripDigis )
process.DQMCommon       = cms.Sequence(process.dqmEnv*process.dqmSaver)
process.seqAnalysis     = cms.Path( process.LaserAlignmentProducerDQM*process.DQMCommon)
