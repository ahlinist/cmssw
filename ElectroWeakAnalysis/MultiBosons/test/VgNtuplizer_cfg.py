import FWCore.ParameterSet.Config as cms

process = cms.Process("VgKit")

process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('START38_V10::All')
process.load("Configuration.StandardSequences.MagneticField_cff")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
    )

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring('file:Skimming/VGammaSkim_testSummer10_numEvent10.root'),
                            noEventSort = cms.untracked.bool(True),
                            duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
                            )

process.load("ElectroWeakAnalysis.MultiBosons.VgNtuplizer_cfi")
#process.VgAnalyzerKit.saveHLTInfo = cms.untracked.bool(False);
process.VgNtuplizer.doStoreJets = cms.bool(True);

process.TFileService = cms.Service("TFileService", fileName = cms.string('vgtree.root'))

process.p = cms.Path(process.VgNtuplizer)
