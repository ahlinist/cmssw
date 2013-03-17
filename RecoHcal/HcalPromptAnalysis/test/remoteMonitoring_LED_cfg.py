import FWCore.ParameterSet.Config as cms

process = cms.Process("testAnalyzer")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

#process.source = cms.Source("PoolSource",
process.source = cms.Source("HcalTBSource",
    fileNames = cms.untracked.vstring(
#      'file:/afs/cern.ch/work/d/dtlisov/private/Monitoring/data/USC_212179.root',
      'file:/afs/cern.ch/work/d/dtlisov/private/Monitoring/data/USC_210378.root',
#     'file:/afs/cern.ch/work/d/dtlisov/private/Monitoring/data/USC_203056.root',
#      'file:/afs/cern.ch/work/d/dtlisov/private/Monitoring/data/USC_196597.root',
      'file:/afs/cern.ch/work/d/dtlisov/private/Monitoring/data/USC_194334.root',
      'file:/afs/cern.ch/work/d/dtlisov/private/Monitoring/data/USC_191392.root',
      'file:/afs/cern.ch/work/d/dtlisov/private/Monitoring/data/USC_187927.root',
#      'file:/afs/cern.ch/work/d/dtlisov/private/Monitoring/data/USC_185396.root'
##	'file:USC_211154.root'
##     'file:/tmp/zhokin/HcalNZSRAW-193-116.root'
#
##     'file:rfiles/USC_185396.root',
#     'file:rfiles/USC_187927.root',
#     'file:rfiles/USC_191392.root',
#     'file:rfiles/USC_194334.root',
##     'file:rfiles/USC_196597.root',
#     'file:rfiles/USC_203056.root',
#     'file:rfiles/USC_210378.root'
##     'file:rfiles/USC_212179.root'
   ), 
    streams = cms.untracked.vstring(
		  "HCAL_Trigger",
		  "HCAL_DCC700","HCAL_DCC701","HCAL_DCC702","HCAL_DCC703","HCAL_DCC704","HCAL_DCC705",
		  "HCAL_DCC706","HCAL_DCC707","HCAL_DCC708","HCAL_DCC709","HCAL_DCC710","HCAL_DCC711",
		  "HCAL_DCC712","HCAL_DCC713","HCAL_DCC714","HCAL_DCC715","HCAL_DCC716","HCAL_DCC717",
		  "HCAL_DCC718","HCAL_DCC719","HCAL_DCC720","HCAL_DCC721","HCAL_DCC722","HCAL_DCC723",
		  "HCAL_DCC724","HCAL_DCC725","HCAL_DCC726","HCAL_DCC727","HCAL_DCC728","HCAL_DCC729",
		  "HCAL_DCC730","HCAL_DCC731"
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
                                  #studyRunDependenceHist = cms.untracked.bool(True),
                                  studyRunDependenceHist = cms.untracked.bool(False),
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
                                  ratioHBMin = cms.double(0.30),
                                  ratioHBMax = cms.double(1.00),
                                  ratioHEMin = cms.double(0.30),
                                  ratioHEMax = cms.double(1.00),
                                  # to see run dependence of rate of events with Nbadchannels > :
                                  nbadchannels1 = cms.int32(5),
                                  nbadchannels2 = cms.int32(10),
                                  nbadchannels3 = cms.int32(20),
                                  # -54 for  BAD HBHE channels from study on RMS of shapes
                                  #Verbosity = cms.untracked.int32(-54),
                                  rmsHBMin = cms.double(-1.),
                                  rmsHBMax = cms.double(2.025),
                                  rmsHEMin = cms.double(-1.),
                                  rmsHEMax = cms.double(2.225),
                                  # for  BAD HBHEHOHF CALIBRATION channels from study on shape Ratio
                                  calibratioHBMin = cms.double(0.45),
                                  calibratioHEMin = cms.double(0.7),
                                  calibratioHOMin = cms.double(0.5),
                                  calibratioHFMin = cms.double(0.2),
                                  #
                                  #
                                  HistOutFile = cms.untracked.string('test.root'),
				  MAPOutFile = cms.untracked.string('LogEleMapdb.h')
                                  #HistOutFile = cms.untracked.string('BadRBXrun211146.root')
                                  #HistOutFile = cms.untracked.string('BadRBXrun211154.root')
                                  #HistOutFile = cms.untracked.string('PFGNZSRAW-run193116.root')
                                  #HistOutFile = cms.untracked.string('/tmp/zhokin/BadRBXrun211146.root')
                                  #HistOutFile = cms.untracked.string('/tmp/zhokin/analysisUSC211154.root')	
                                  #HistOutFile = cms.untracked.string('/tmp/zhokin/rawTest.root')
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
process.hcalDigis.InputLabel = cms.InputTag("source")
				
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



