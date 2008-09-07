import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")
process.load("CondCore.DBCommon.CondDBSetup_cfi")

process.load("Configuration.EventContent.EventContentCosmics_cff")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff") 

# Magnetic field: force mag field to be 0 tesla
process.load("Configuration.StandardSequences.MagneticField_0T_cff")
#process.load("Configuration.StandardSequences.MagneticField_cff")

#process.load("Configuration.GlobalRuns.ForceZeroTeslaField_cff")

process.load("Configuration.StandardSequences.Geometry_cff")

process.load("Configuration.StandardSequences.ReconstructionCosmics_cff") 

process.load("Configuration.StandardSequences.RawToDigi_Data_cff")

process.load("L1Trigger.Configuration.L1Config_cff")

process.load("L1TriggerConfig.CSCTFConfigProducers.CSCTFConfigProducer_cfi") 

process.load("L1TriggerConfig.CSCTFConfigProducers.L1MuCSCTFConfigurationRcdSrc_cfi") 

process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")
#process.load("MagneticField.Engine.volumeBasedMagneticField_cfi")

process.load("Geometry.CaloEventSetup.CaloGeometry_cfi")

process.load("Geometry.CaloEventSetup.CaloTopology_cfi")

#process.load("L1TriggerConfig.L1GtConfigProducers.L1GtConfig_cff")

#process.load("L1TriggerConfig.L1GtConfigProducers.Luminosity.lumi1x1032.L1Menu_CRUZET200805_gr7_muon_cff") 

process.maxEvents = cms.untracked.PSet(
 input = cms.untracked.int32(-1)
)
process.source = cms.Source("PoolSource",
 fileNames = cms.untracked.vstring(
'/store/data/Commissioning08/Cosmics/RAW/MW36_v1/000/061/090/14404076-647A-DD11-A768-000423D9870C.root' 
)
)

process.prefer("GlobalTag")
process.MessageLogger = cms.Service("MessageLogger")

#process.FEVT = cms.OutputModule("PoolOutputModule",
#    fileName = cms.untracked.string('/tmp/efe/promptrecoCosmics.root')
#)

process.GlobalTag.connect = 'frontier://cmsfrontier.cern.ch:8000/FrontierProd/CMS_COND_21X_GLOBALTAG' 
process.GlobalTag.globaltag = 'CRUZET4_V4P::All'

process.demo = cms.EDAnalyzer('HFMon')

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('HFprompt_out.root')
)

process.p = cms.Path(process.RawToDigi*process.hfreco*process.demo)
#process.p = cms.Path(process.RawToDigi*process.hfreco)
#process.outpath = cms.EndPath(process.FEVT)
