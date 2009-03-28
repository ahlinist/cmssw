import FWCore.ParameterSet.Config as cms

process = cms.Process('HcalTPGValidationTest')

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
       '/store/data/Commissioning09/Cosmics/RAW/v1/000/079/010/02D7C1ED-3219-DE11-A64D-000423D99160.root'
    )
)

#process.source = cms.Source("HcalTBSource",
#      fileNames = cms.untracked.vstring(
#         'file:/data1/kvtsang/HCAL/USC_076594.root'
#      ),
#      stream = cms.untracked.vstring(
#         #HBHE a, b, c: 
#         'HCAL_DCC700','HCAL_DCC701','HCAL_DCC702','HCAL_DCC703','HCAL_DCC704','HCAL_DCC705', 
#         'HCAL_DCC706','HCAL_DCC707','HCAL_DCC708','HCAL_DCC709','HCAL_DCC710','HCAL_DCC711', 
#         'HCAL_DCC712','HCAL_DCC713','HCAL_DCC714','HCAL_DCC715','HCAL_DCC716','HCAL_DCC717', 
#          #HF: 
#         'HCAL_DCC718','HCAL_DCC719','HCAL_DCC720','HCAL_DCC721','HCAL_DCC722','HCAL_DCC723', 
#          #HO: 
#         'HCAL_DCC724','HCAL_DCC725','HCAL_DCC726','HCAL_DCC727','HCAL_DCC728','HCAL_DCC729', 
#         'HCAL_DCC730','HCAL_DCC731', 
#         'HCAL_Trigger','HCAL_SlowData' 
#         'HCAL_Trigger'
#    )
#)

process.maxEvents = cms.untracked.PSet(
	input = cms.untracked.int32(100)
)

# process.load("Configuration.StandardSequences.FakeConditions_cff")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource','GlobalTag')
process.GlobalTag.globaltag = 'CRAFT_30X::All'

process.load('EventFilter.HcalRawToDigi.HcalRawToDigi_cfi')

process.load('SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff')
process.valHcalTriggerPrimitiveDigis = process.simHcalTriggerPrimitiveDigis.clone()
process.valHcalTriggerPrimitiveDigis.inputLabel = cms.InputTag('hcalDigis')

#For XML LUT
process.HcalTPGCoderULUT.read_XML_LUTs = cms.bool(True)
process.HcalTPGCoderULUT.inputLUTs = 'RecoHcal/HcalTPGValidation/data/CRAFTPhysicsV2.xml'

#For ASCII LUT
#process.HcalTPGCoderULUT.read_Ascii_LUTs = cms.bool(True)
#process.HcalTPGCoderULUT.inputLUTs = 'RecoHcal/HcalTPGValidation/data/CRAFTPhysicsV2.dat'
#process.CaloTPGTranscoder.read_Ascii_Compression_LUTs = cms.bool(True)
#process.CaloTPGTranscoder.hcalLUT1 = 'RecoHcal/HcalTPGValidation/data/OutputLUT.dat'

#Additional Setting
#process.valHcalTriggerPrimitiveDigis.firstTPSample = cms.int32(2)  ##First TP sample from 10 Digis. (default=2)
#process.valHcalTriggerPrimitiveDigis.TPSize = cms.int32(4)  ##TP sample size, default=4
#process.valHcalTriggerPrimitiveDigis.FG_threshold = cms.uint32(12)  ## threshold for HF fine grain bit.
#process.valHcalTriggerPrimitiveDigis.ZS_threshold = cms.uint32(0)  ## threshold for TP zero suppression.

process.TFileService = cms.Service("TFileService",
	fileName = cms.string('histo.root')
)

process.runValidation = cms.EDAnalyzer('HcalTPGValidation')

process.p = cms.Path(process.hcalDigis * process.valHcalTriggerPrimitiveDigis * process.runValidation)
