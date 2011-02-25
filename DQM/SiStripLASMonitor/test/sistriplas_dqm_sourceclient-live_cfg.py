import FWCore.ParameterSet.Config as cms

process = cms.Process( "sistriplaserDQMLive" )
process.MessageLogger = cms.Service( "MessageLogger",
  cout = cms.untracked.PSet(threshold = cms.untracked.string( 'ERROR' )),
  destinations = cms.untracked.vstring( 'cout')
)
#----------------------------
# Event Source
#-----------------------------
process.load("DQM.Integration.test.inputsource_cfi")
process.EventStreamHttpReader.consumerName = 'SiStripLAS DQM Consumer'
process.EventStreamHttpReader.SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('HLT_L1*','HLT_TrackerCalibration*'))  

process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32( -1 )
)

process.load( "EventFilter.SiStripRawToDigi.SiStripDigis_cfi" )
process.siStripDigis.ProductLabel = "source"#"hltCalibrationRaw"
#--------------------------
# Calibration
#--------------------------
process.load("DQM.Integration.test.FrontierCondition_GT_cfi")

process.load("DQMServices.Core.DQM_cfg" )
process.load("DQMServices.Components.DQMEnvironment_cfi")
#----------------------------
# DQM Live Environment
#-----------------------------
process.load("DQM.Integration.test.environment_cfi")
process.dqmEnv.subSystemFolder    = "SiStripLAS"
process.dqmSaver.producer = "Playback"
process.dqmSaver.saveByTime = 1 # 1. if running online
process.dqmSaver.saveByMinute = 16
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
process.DQMCommon   = cms.Sequence(process.dqmEnv*process.dqmSaver)

process.seqAnalysis = cms.Path( process.LaserAlignmentProducerDQM*process.DQMCommon)
