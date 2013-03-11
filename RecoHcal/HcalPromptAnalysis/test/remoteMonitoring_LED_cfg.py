import FWCore.ParameterSet.Config as cms

process = cms.Process("testAnalyzer")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

#process.source = cms.Source("PoolSource",
process.source = cms.Source("HcalTBSource",
    fileNames = cms.untracked.vstring(
#	'file:USC_211154.root'
#     'file:/tmp/zhokin/HcalNZSRAW-193-116.root'
     'file:rfiles/USC_185396.root',
     'file:rfiles/USC_187927.root',
     'file:rfiles/USC_191392.root',
     'file:rfiles/USC_194334.root',
     'file:rfiles/USC_196597.root',
     'file:rfiles/USC_203056.root',
     'file:rfiles/USC_210378.root',
     'file:rfiles/USC_212179.root'
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
##                                  Threshold = cms.untracked.int32(5),
##                                 DigiCollectionLabel = cms.untracked.InputTag("hcalDigis"),
#                                  Verbosity = cms.untracked.int32(-54),
#                                  Verbosity = cms.untracked.int32(-22),
#                                  Verbosity = cms.untracked.int32(-11),
#                                  Verbosity = cms.untracked.int32(-13),
                                  Verbosity = cms.untracked.int32(0),
#
#                         -53 for  BAD HBHE channels from study on shape Ratio
#                                  Verbosity = cms.untracked.int32(-53),
                                  ratioHBMin = cms.double(0.4),
                                  ratioHBMax = cms.double(0.7),
                                  ratioHEMin = cms.double(0.3),
                                  ratioHEMax = cms.double(0.8),
#                         -54 for  BAD HBHE channels from study on RMS of shapes
#                                  Verbosity = cms.untracked.int32(-54),
                                  rmsHBMin = cms.double(-1.),
                                  rmsHBMax = cms.double(2.025),
                                  rmsHEMin = cms.double(-1.),
                                  rmsHEMax = cms.double(2.225),
#                            for  BAD HBHEHOHF CALIBRATION channels from study on shape Ratio
                                  calibratioHBMin = cms.double(0.45),
                                  calibratioHEMin = cms.double(0.7),
                                  calibratioHOMin = cms.double(0.5),
                                  calibratioHFMin = cms.double(0.2),
#
#
                                  HistOutFile = cms.untracked.string('test.root')
#                                  HistOutFile = cms.untracked.string('BadRBXrun211146.root')
#                                  HistOutFile = cms.untracked.string('BadRBXrun211154.root')
#                                  HistOutFile = cms.untracked.string('PFGNZSRAW-run193116.root')
                                  #HistOutFile = cms.untracked.string('/tmp/zhokin/BadRBXrun211146.root')
                                  #HistOutFile = cms.untracked.string('/tmp/zhokin/analysisUSC211154.root')	
                                  #HistOutFile = cms.untracked.string('/tmp/zhokin/rawTest.root')
#
##                                  OutputFilePath = cms.string('/tmp/zhokin/'),        
##                                  OutputFileExt = cms.string(''),
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
#process.GlobalTag.globaltag = 'GR_P_V28::All'
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



