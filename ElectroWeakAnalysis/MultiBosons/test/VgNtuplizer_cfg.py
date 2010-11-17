import FWCore.ParameterSet.Config as cms
import os
from FWCore.PythonUtilities.LumiList import LumiList

process = cms.Process("VgKit")

process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('START38_V10::All')
process.load("Configuration.StandardSequences.MagneticField_cff")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

thelist = LumiList(filename="/scratch/lgray/vgamma_skims/data/Cert_132440-149442_7TeV_StreamExpress_Collisions10_JSON.txt")

prefix="/scratch/lgray/vgamma_skims/data/PromptRecov2/"

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(),
                            noEventSort = cms.untracked.bool(True),
                            duplicateCheckMode = cms.untracked.string('noDuplicateCheck'),
                            lumisToProcess = cms.untracked.VLuminosityBlockRange()
                            )

process.source.fileNames.extend( ['file:'+os.path.join(prefix,file) for file in os.listdir(prefix)] )

process.source.lumisToProcess = [ str(a) for a in thelist.getCMSSWString().split(',') ] 


process.load("ElectroWeakAnalysis.MultiBosons.VgNtuplizer_cfi")
#process.VgAnalyzerKit.saveHLTInfo = cms.untracked.bool(False);
process.VgNtuplizer.doStoreJets = cms.bool(True);

process.TFileService = cms.Service("TFileService", fileName = cms.string('vgtree.root'))

process.p = cms.Path(process.VgNtuplizer)
