import FWCore.ParameterSet.Config as cms

process = cms.Process("testAnalyzer")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
     'file:/tmp/zhokin/HcalNZSRAW-199-751.root'
   )
)

process.Analyzer = cms.EDAnalyzer("VeRawAnalyzer",
                                  #
                                  recordNtuples = cms.untracked.bool(False),
                                  #recordNtuples = cms.untracked.bool(True),
                                  #
                                  #recordHistoes = cms.untracked.bool(False),
                                  recordHistoes = cms.untracked.bool(True),
                                  #
                                  studyRunDependenceHist = cms.untracked.bool(True),
                                  #studyRunDependenceHist = cms.untracked.bool(False),
                                  #
                                  ##DigiCollectionLabel = cms.untracked.InputTag("hcalDigis"),
                                  #Verbosity = cms.untracked.int32(-54),
                                  #Verbosity = cms.untracked.int32(-22),
                                  #Verbosity = cms.untracked.int32(-11),
                                  #Verbosity = cms.untracked.int32(-13),
                                  #Verbosity = cms.untracked.int32(-51),
                                  Verbosity = cms.untracked.int32(0),
                                  MapCreation = cms.untracked.int32(1),
                                  #
                                  # -53 for  BAD HBHE channels from study on shape Ratio34
                                  #Verbosity = cms.untracked.int32(-53),
                                  ratioHBMin = cms.double(0.50),
                                  ratioHBMax = cms.double(0.92),
                                  ratioHEMin = cms.double(0.50),
                                  ratioHEMax = cms.double(0.92),
                                  # to see run dependence of rate of events with Nbadchannels > :
                                  nbadchannels1 = cms.int32(10),
                                  nbadchannels2 = cms.int32(30),
                                  nbadchannels3 = cms.int32(60),
                                  # -54 for  BAD HBHE channels from study on RMS of shapes
                                  #Verbosity = cms.untracked.int32(-54),
                                  rmsHBMin = cms.double(1.0),
                                  rmsHBMax = cms.double(2.3),
                                  rmsHEMin = cms.double(1.0),
                                  rmsHEMax = cms.double(2.3),
                                  # for  BAD HBHEHOHF CALIBRATION channels from study on shape Ratio
                                  calibratioHBMin = cms.double(0.65),
                                  calibratioHEMin = cms.double(0.65),
                                  calibratioHOMin = cms.double(0.25),
                                  calibratioHFMin = cms.double(0.25),
                                  #
                                  #
                                  HistOutFile = cms.untracked.string('testNZS.root'),
                                  MAPOutFile = cms.untracked.string('LogEleMapdb.h')
                                  #
                                  ##OutputFilePath = cms.string('/tmp/zhokin/'),        
                                  ##OutputFileExt = cms.string(''),
                                  #
)		

process.hcal_db_producer = cms.ESProducer("HcalDbProducer",
    dump = cms.untracked.vstring(''),
    file = cms.untracked.string('')
)
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.hcalDigis.FilterDataQuality = cms.bool(False)
				
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'GR_P_V41::All'
process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource','GlobalTag')

process.p = cms.Path(process.hcalDigis*process.Analyzer)

process.MessageLogger = cms.Service("MessageLogger",
     categories   = cms.untracked.vstring(''),
     destinations = cms.untracked.vstring('cout'),
     debugModules = cms.untracked.vstring('*'),
     cout = cms.untracked.PSet(
         threshold = cms.untracked.string('WARNING'),
	 WARNING = cms.untracked.PSet(limit = cms.untracked.int32(0))
     )
 )



