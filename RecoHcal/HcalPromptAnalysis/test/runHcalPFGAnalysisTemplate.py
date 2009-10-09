import FWCore.ParameterSet.Config as cms

####### tested with CMSSW_3_2_5 - 8 October 2009 ########

#### NOTE:
#### IF YOU RUN ON *RECO* DATA --> PATH1
#### IF YOU RUN ON *RAW* DATA --> PATH 2(only HCAL) or 3(All CMS) or 4(HCAL+L1GT)

process = cms.Process("HcalPFGAnalysisTemplate")

## Input
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

process.source = cms.Source("PoolSource",
    dropMetaData = cms.untracked.bool(True),
    fileNames = cms.untracked.vstring(
    ##RECO
    '/store/data/CRAFT09/Cosmics/RECO/v1/000/111/009/F82BA4D9-EC8B-DE11-A352-001D09F241B9.root'
    ##RAW
    #'/store/data/CRAFT09/Cosmics/RAW/v1/000/111/009/F6686755-478A-DE11-B66F-000423D8F63C.root'
    )
)

## Global tags (TO BE CHECKED)
## Check the best tag here:
## https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions#3XY_Releases_data
# process.load("Configuration.StandardSequences.FakeConditions_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = "GR09_31X_V6P::All"
process.GlobalTag.globaltag = "GR09_P_V1::All"
process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource','GlobalTag')
process.prefer("GlobalTag")

## Magnetic Field
process.load("Configuration.StandardSequences.MagneticField_0T_cff")
# process.load('Configuration/StandardSequences/MagneticField_38T_cff')

#Geometry
process.load("Configuration.StandardSequences.GeometryPilot2_cff")
#process.load("Configuration.StandardSequences.Geometry_cff")

# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1
process.MessageLogger.cerr.default.limit = 10

## Hcal PFG Analysis Template 
process.hcalPFGAnalysisTemplate = cms.EDAnalyzer ("HcalPFGAnalysisTemplate")

## Standard sequences for digitization and reconstruction of *all* CMS data
process.load("Configuration.StandardSequences.RawToDigi_Data_cff")
process.load("Configuration.StandardSequences.ReconstructionCosmics_cff")

########## EXTRA ##########

## Hcal *only* digi and rechits reconstruction
#process.load("EventFilter.HcalRawToDigi.HcalRawToDigi_cfi")
#process.load("RecoLocalCalo.HcalRecProducers.HcalHitReconstructor_hbhe_cfi")
#process.load("RecoLocalCalo.HcalRecProducers.HcalHitReconstructor_ho_cfi")
#process.load("RecoLocalCalo.HcalRecProducers.HcalHitReconstructor_hf_cfi")
#process.load("RecoLocalCalo.HcalRecProducers.HcalHitReconstructor_zdc_cfi")

## Hcal Emulator (NEEDED FOR PATH4)
#process.load('SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff')
#process.valHcalTriggerPrimitiveDigis = process.simHcalTriggerPrimitiveDigis.clone()
#process.valHcalTriggerPrimitiveDigis.inputLabel = cms.VInputTag('hcalDigis', 'hcalDigis')
#process.HcalTPGCoderULUT.LUTGenerationMode = cms.bool(False)

## L1 Trigger Unpacker Stuff (NEEDED FOR PATH4)
#process.load("CondCore.DBCommon.CondDBSetup_cfi")
#process.load("L1Trigger.Configuration.L1DummyConfig_cff")
#process.load("EventFilter.L1GlobalTriggerRawToDigi.l1GtUnpack_cfi")
#process.l1GtUnpack.DaqGtInputTag = 'source'

#########################

## Output
process.TFileService = cms.Service("TFileService",
    fileName = cms.string('histHcalPFGAnalysisTemplate.root')
)

## PATH N.1
## Path (running on RECO files)
process.p1 = cms.Path(process.hcalPFGAnalysisTemplate)

## PATH N.2
##only HCAL (a la DQM)
#process.p1 = cms.Path(process.hcalDigis
#                      *process.horeco
#                      *process.hfreco
#                      *process.hbhereco
#                      *process.zdcreco
#                      *process.hcalPFGAnalysisTemplate)

## PATH N.3
##All CMS data
#process.p1 = cms.Path(process.RawToDigi
#                      *process.reconstructionCosmics
#                      *process.hcalPFGAnalysisTemplate)

## PATH N.4
##only HCAL + HCAL stuff needed for emulator (a la DQM)
#process.p1 = cms.Path(process.hcalDigis
#                      *process.valHcalTriggerPrimitiveDigis
#                      *process.l1GtUnpack
#                      *process.horeco
#                      *process.hfreco
#                      *process.hbhereco
#                      *process.zdcreco
#                      *process.hcalPFGAnalysisTemplate)

