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

#patch
process.load("L1TriggerConfig.L1GtConfigProducers.L1GtConfig_cff")
process.load("L1TriggerConfig.L1GtConfigProducers.Luminosity.startup.L1Menu_startup_v3_Unprescaled_cff");
#endpatch

process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")
#process.load("MagneticField.Engine.volumeBasedMagneticField_cfi")

process.load("Geometry.CaloEventSetup.CaloGeometry_cfi")

process.load("Geometry.CaloEventSetup.CaloTopology_cfi")

#process.load("L1TriggerConfig.L1GtConfigProducers.L1GtConfig_cff")

#process.load("L1TriggerConfig.L1GtConfigProducers.Luminosity.lumi1x1032.L1Menu_CRUZET200805_gr7_muon_cff") 

process.maxEvents = cms.untracked.PSet(
 input = cms.untracked.int32(20)
)

#process.source = cms.Source("NewEventStreamFileReader",
#                            fileNames = cms.untracked.vstring(
process.source = cms.Source("PoolSource",
 fileNames = cms.untracked.vstring(
#'file:beam2_beamhalo_tes.root'
#'file:beam2_beamhalo_tesNODQM.root'
 #   '/store/data/Commissioning08/BeamHalo/RAW/GRtoBeam_v1/000/062/096/56077B20-7B7F-DD11-B9D5-00161757BF42.root',
 #   '/store/data/Commissioning08/BeamHalo/RAW/GRtoBeam_v1/000/062/096/863014FF-7C7F-DD11-8E83-0019DB29C614.root',
 #   '/store/data/Commissioning08/BeamHalo/RAW/GRtoBeam_v1/000/062/096/96DAA256-787F-DD11-A060-000423D6C8E6.root',
#    '/store/data/Commissioning08/BeamHalo/RAW/GRtoBeam_v1/000/062/096/DA5006AF-757F-DD11-9127-000423D94700.root'

#62232
    '/store/data/BeamCommissioning08/BeamHalo/RAW/v1/000/062/232/04AF2DC6-6280-DD11-992D-000423D98800.root'
                                  )
)

#process.source = cms.Source("NewEventStreamFileReader",
#                            fileNames = cms.untracked.vstring(

process.prefer("GlobalTag")
process.MessageLogger = cms.Service("MessageLogger")

#process.FEVT = cms.OutputModule("PoolOutputModule",
#    fileName = cms.untracked.string('/tmp/efe/promptrecoCosmics2.root')
#)

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'CRUZET4_V4P::All'

process.demo = cms.EDAnalyzer('HFMon')

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('HFprompt_out.root')
)

#start from raw
process.p = cms.Path(process.RawToDigi*process.hfreco*process.hbhereco*process.demo)

#start from reco
#process.p = cms.Path(process.demo)


#process.outpath = cms.EndPath(process.FEVT)
