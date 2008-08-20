import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")

process.load("MagneticField.Engine.uniformMagneticField_cfi")

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
    fileNames = cms.untracked.vstring('/store/data/Commissioning08/Cosmics/RECO/CRUZET4_v1/000/057/620/72E5FFD3-D76D-DD11-8049-000423D6B358.root')
)
###>>>>>>>>>>>  Don't remove this line!

process.MessageLogger.cerr.INFO.limit = 1000000
process.MessageLogger.cerr.noTimeStamps = True
                                     

#process.MessageLogger = cms.Service("MessageLogger",
#    cout = cms.untracked.PSet(
#        default = cms.untracked.PSet( ## kill all messages in the log
#
#            limit = cms.untracked.int32(0)
#        ),
#        FwkJob = cms.untracked.PSet( ## but FwkJob category - those unlimitted
#
#            limit = cms.untracked.int32(-1)
#        )
#    ),
#    categories = cms.untracked.vstring('FwkJob'),
#    destinations = cms.untracked.vstring('cout')
#)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('prompt_out.root')
)

process.prom = cms.EDFilter("HcalProm",
    baseHtmlDir = cms.untracked.string('.'),
    printPromptHTML = cms.untracked.bool(True),
    printRecHitHTML = cms.untracked.bool(True),
    printJetMetHTML = cms.untracked.bool(True),
    printCaloTowerHTML = cms.untracked.bool(False),
    printDigiHTML = cms.untracked.bool(False),
    printHPDNoiseHTML = cms.untracked.bool(False),
    printMuonHTML = cms.untracked.bool(True)
)

process.p = cms.Path(process.prom)
process.UniformMagneticFieldESProducer.ZFieldInTesla = 0.001
