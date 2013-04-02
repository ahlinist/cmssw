import FWCore.ParameterSet.Config as cms

process = cms.Process("testAnalyzer")

process.maxEvents = cms.untracked.PSet(
#    input = cms.untracked.int32(1000)
   input = cms.untracked.int32(-1)
)

#process.source = cms.Source("PoolSource",
process.source = cms.Source("HcalTBSource",
    fileNames = cms.untracked.vstring(
#
## Laser:
#     'file:rfiles/USC_133070.root' ## only HF digis 10K
#     'file:rfiles/USC_141849.root' ## only HF digis 5K
#     'file:rfiles/USC_178165.root' ## only HF digis 10K
#     'file:rfiles/USC_178455.root' ## only HF digis 10K
#     'file:rfiles/USC_211875.root' ## only HBHE digis 10K problems
#
##LED:
     'file:rfiles/USC_195682.root',
#     'file:rfiles/USC_196870.root',
#     'file:rfiles/USC_201305.root',
#     'file:rfiles/USC_202341.root',
#     'file:rfiles/USC_203339.root',
#     'file:rfiles/USC_204172.root',
#     'file:rfiles/USC_205355.root',
#     'file:rfiles/USC_206456.root'
## short:
#     'file:rfiles/USC_188148.root',
#     'file:rfiles/USC_191392.root',
#     'file:rfiles/USC_193001.root'
## problematic:
##     'file:rfiles/USC_190161.root'
##     'file:rfiles/USC_194167.root'    
##     'file:rfiles/USC_195406.root'
##     'file:rfiles/USC_206974.root'
##     'file:rfiles/USC_207846.root'
#
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
                                  MapCreation = cms.untracked.int32(1),
                                  #
                                  recordNtuples = cms.untracked.bool(False),
                                  #recordNtuples = cms.untracked.bool(True),
                                  #
                                  #recordHistoes = cms.untracked.bool(False),
                                  recordHistoes = cms.untracked.bool(True),
                                  #
                                  ##scripts: zRunRatio34.C, zRunNbadchan.C
                                  #studyRunDependenceHist = cms.untracked.bool(True),
                                  studyRunDependenceHist = cms.untracked.bool(False),
                                  #
                                  ##scripts: zerrors.C
                                  studyCapIDErrorsHist = cms.untracked.bool(True),
                                  #studyCapIDErrorsHist = cms.untracked.bool(False),
                                  #
                                  ##scripts: zrms.C
                                  studyRMSshapeHist = cms.untracked.bool(True),
                                  #studyRMSshapeHist = cms.untracked.bool(False),
                                  #
                                  ##scripts: zratio34.C
                                  studyRatioShapeHist = cms.untracked.bool(True),
                                  #studyRatioShapeHist = cms.untracked.bool(False),
                                  #
                                  ##scripts: ztsmean.C
                                  studyTSmeanShapeHist = cms.untracked.bool(True),
                                  #studyTSmeanShapeHist = cms.untracked.bool(False),
                                  #
                                  ##scripts: ztsmaxa.C
                                  studyTSmaxShapeHist = cms.untracked.bool(True),
                                  #studyTSmaxShapeHist = cms.untracked.bool(False),
                                  #
                                  ##scripts: zdifampl.C
                                  studyDiffAmplHist = cms.untracked.bool(True),
                                  #studyDiffAmplHist = cms.untracked.bool(False),
                                  #
                                  ##scripts: zcalib.C
                                  studyCalibCellsHist = cms.untracked.bool(True),
                                  #studyCalibCellsHist = cms.untracked.bool(False),
                                  #
                                  ##DigiCollectionLabel = cms.untracked.InputTag("hcalDigis"),
                                  #Verbosity = cms.untracked.int32(-54),
                                  #Verbosity = cms.untracked.int32(-22),
                                  #Verbosity = cms.untracked.int32(-11),
                                  #Verbosity = cms.untracked.int32(-13),
                                  #Verbosity = cms.untracked.int32(-51),
                                  #Verbosity = cms.untracked.int32(-24),
                                  #Verbosity = cms.untracked.int32(-244),
                                  Verbosity = cms.untracked.int32(0),
                                  #
                                  # -53 for  BAD HBHEHF channels from study on shape Ratio
                                  #Verbosity = cms.untracked.int32(-53),
                                  ratioHBMin = cms.double(0.50),
                                  ratioHBMax = cms.double(0.88),
                                  ratioHEMin = cms.double(0.50),
                                  ratioHEMax = cms.double(0.93),
                                  ratioHFMin = cms.double(0.30),
                                  ratioHFMax = cms.double(1.02),
                                  ratioHOMin = cms.double(0.54),
                                  ratioHOMax = cms.double(1.04),
                                  # -54 for  BAD HBHEHF channels from study on RMS of shapes
                                  #Verbosity = cms.untracked.int32(-54),
                                  rmsHBMin = cms.double(1.0),
                                  rmsHBMax = cms.double(2.3),
                                  rmsHEMin = cms.double(1.0),
                                  rmsHEMax = cms.double(2.3),
                                  rmsHFMin = cms.double(0.2),
                                  rmsHFMax = cms.double(2.8),
                                  rmsHOMin = cms.double(0.2),
                                  rmsHOMax = cms.double(2.4),
                                  # -55 for  BAD HBHEHF channels from study on TSmean of shapes
                                  #Verbosity = cms.untracked.int32(-55),
                                  TSmeanHBMin = cms.double(1.0),
                                  TSmeanHBMax = cms.double(5.2),
                                  TSmeanHEMin = cms.double(1.0),
                                  TSmeanHEMax = cms.double(5.2),
                                  TSmeanHFMin = cms.double(1.5),
                                  TSmeanHFMax = cms.double(4.5),
                                  TSmeanHOMin = cms.double(2.5),
                                  TSmeanHOMax = cms.double(4.5),
                                  # -55 for  BAD HBHEHF channels from study on TSmax of shapes
                                  #Verbosity = cms.untracked.int32(-55),
                                  TSpeakHBMin = cms.double(1.5),
                                  TSpeakHBMax = cms.double(6.5),
                                  TSpeakHEMin = cms.double(1.5),
                                  TSpeakHEMax = cms.double(6.5),
                                  TSpeakHFMin = cms.double(0.5),
                                  TSpeakHFMax = cms.double(5.5),
                                  TSpeakHOMin = cms.double(0.5),
                                  TSpeakHOMax = cms.double(5.5),
                                  # for  BAD HBHEHOHF CALIBRATION channels from study on shape Ratio
                                  calibratioHBMin = cms.double(0.65),
                                  calibratioHEMin = cms.double(0.65),
                                  calibratioHOMin = cms.double(0.25),
                                  calibratioHFMin = cms.double(0.25),
                                  # to see run dependence of rate of events with Nbadchannels > :
                                  nbadchannels1 = cms.int32(10),
                                  nbadchannels2 = cms.int32(30),
                                  nbadchannels3 = cms.int32(60),
                                  #
                                  #
                                  HistOutFile = cms.untracked.string('test.root'),
#                                  HistOutFile = cms.untracked.string('testLaser178165.root'),
#                                  HistOutFile = cms.untracked.string('testLaser141849.root'),
#                                  HistOutFile = cms.untracked.string('testLaser133070.root'),
#                                  HistOutFile = cms.untracked.string('testLaser211875.root'),
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



