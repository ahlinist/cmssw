import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")

#process.load("MagneticField.Engine.uniformMagneticField_cfi")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")

process.load("Configuration.StandardSequences.Geometry_cff")

process.load("Geometry.CaloEventSetup.CaloGeometry_cff")

process.load("Geometry.CaloEventSetup.CaloTopology_cfi")

#process.load("L1TriggerConfig.L1GtConfigProducers.L1GtConfig_cff")
#process.load("L1TriggerConfig.L1GtConfigProducers.Luminosity.lumi1x1032.L1Menu_CRUZET200805_gr7_muon_cff")

###<<<<<<<<<< Don't remove this line -- it's used by the gui when updating/replacing file names!
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)
process.source = cms.Source("PoolSource",
#    fileNames = cms.untracked.vstring('/store/data/Commissioning08/Cosmics/RECO/CRAFT_ALL_V4_ReReco-v1/0000/0C453DD0-55C1-DD11-AA47-001D0967D99A.root')
     fileNames = cms.untracked.vstring(
       '/store/data/Commissioning08/Cosmics/RECO/v1/000/068/500/0081CE08-FCA8-DD11-916A-001D09F252DA.root',
       '/store/data/Commissioning08/Cosmics/RECO/v1/000/068/500/00B675FE-FCA8-DD11-8C89-001D09F2910A.root',
       '/store/data/Commissioning08/Cosmics/RECO/v1/000/068/500/020E0A16-FCA8-DD11-9230-0030487C608C.root',
       '/store/data/Commissioning08/Cosmics/RECO/v1/000/068/500/023B3046-FBA8-DD11-9DDE-00304879FBB2.root'
    )
)
###>>>>>>>>>>>  Don't remove this line!

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.connect = 'frontier://Frontier/CMS_COND_21X_GLOBALTAG'
##process.GlobalTag.globaltag = 'CRAFT_V3P::All'  # update GlobalTag as neceesary
process.GlobalTag.globaltag = 'CRAFT_ALL_V9::All'  # update GlobalTag as neceesary
process.prefer("GlobalTag")

process.MessageLogger.cerr.INFO.limit = 1000000
process.MessageLogger.cerr.noTimeStamps = True
                                     
process.load("FWCore.MessageLogger.MessageLogger_cfi") 
process.MessageLogger.cerr.default.limit = 10
process.MessageLogger.cerr.FwkReport.reportEvery = 100

#process.TFileService = cms.Service("TFileService",
#    fileName = cms.string('prompt_out.root')
#)

process.prom = cms.EDFilter("HcalPromBremss",
                            printOut = cms.untracked.bool(False),
                            rootfileName = cms.untracked.string("muonData.root")
                            #printMuonHTML = cms.untracked.bool(True)
                            #HBThreshold = cms.untracked.double(1.)
                            )

process.p = cms.Path(process.prom)
