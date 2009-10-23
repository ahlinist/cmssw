import FWCore.ParameterSet.Config as cms

process = cms.Process("EcalTrigPrimValidation")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.EndOfProcess_cff")

process.load('Configuration.EventContent.EventContent_cff')

# ecal mapping
process.load("Geometry.EcalMapping.EcalMapping_cfi")

process.load("Geometry.EcalMapping.EcalMappingRecord_cfi")

# Calo geometry service model
process.load("Geometry.CaloEventSetup.CaloGeometry_cfi")

# Calo geometry service model
process.load("Geometry.CaloEventSetup.EcalTrigTowerConstituents_cfi")

# IdealGeometryRecord
process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")

process.load("CalibCalorimetry.Configuration.Ecal_FakeConditions_cff")

process.load("SimCalorimetry.EcalTrigPrimProducers.ecalTriggerPrimitiveDigis_with_suppressed_cff")



process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

process.source = cms.Source("PoolSource",
    
    fileNames = cms.untracked.vstring(
       '/store/relval/CMSSW_3_2_2/RelValSingleElectronPt35/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0001/7439E178-3B78-DE11-8A32-00304867915A.root',
       '/store/relval/CMSSW_3_2_2/RelValSingleElectronPt35/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0001/62E19B66-7578-DE11-AB18-001A92810ADE.root',
       '/store/relval/CMSSW_3_2_2/RelValSingleElectronPt35/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V3-v1/0001/0CCB2301-3878-DE11-B93B-001731AF6701.root'
    )
)

process.output = cms.OutputModule("PoolOutputModule",
    outputCommands = process.FEVTDEBUGEventContent.outputCommands,
    fileName = cms.untracked.string('ecalTriggerPrimitivesValidation.root')
)


process.load("Validation.L1Trigger.ecaltpgvalidation_cfi")

process.tpg_step = cms.Path(process.simEcalTriggerPrimitiveDigis+process.tpgVal)
process.end_step = cms.Path(process.endOfProcess)
process.out_step = cms.EndPath(process.output)

process.schedule = cms.Schedule(process.tpg_step, process.end_step, process.out_step)
