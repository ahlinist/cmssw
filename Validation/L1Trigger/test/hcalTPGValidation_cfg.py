import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
       '/store/relval/CMSSW_3_2_5/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V5-v2/0011/FA82B15B-D290-DE11-A70B-000423D9853C.root'
   )
)

process.load('SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff')
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = "MC_31X_V5::All"
process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource','GlobalTag')


process.load('EventFilter.HcalRawToDigi.HcalRawToDigi_cfi')

process.load('SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff')
process.valHcalTriggerPrimitiveDigis = process.simHcalTriggerPrimitiveDigis.clone()
process.valHcalTriggerPrimitiveDigis.inputLabel = cms.VInputTag('simHcalDigis', 'simHcalDigis')

process.load('Validation/L1Trigger/hcaltpgvalidation_cfi')
process.TFileService = cms.Service("TFileService",
      fileName = cms.string('histoHcalTPGValidation.root')
)

process.p = cms.Path(process.hcalTPGValidation)
