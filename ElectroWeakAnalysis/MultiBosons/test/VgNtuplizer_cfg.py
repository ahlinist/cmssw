import os
import FWCore.ParameterSet.Config as cms
from FWCore.PythonUtilities.LumiList import LumiList
import FWCore.ParameterSet.VarParsing as VarParsing

## setup 'analysis'  options
options = VarParsing.VarParsing ('analysis')

## setup tags
options.setupTags (tag = 'of_%03d', ifCond = 'totalSections > 0', tagArg = 'totalSections')
options.setupTags (tag = '%d', ifCond = 'totalSections > 0', tagArg = 'section')

## setup any defaults you want
#options.outputFile = 'vgtree.root'
options.outputFile = '/raid1/veverka/VGammaSkims_v3/ElectronPhotonSkim_NoPhotonTriggers/vgtree.root'
inputDir = "/mnt/tier2/store/user/veverka/EG/ElectronPhotonVGammaSkim-Sept17ReReco-Unmerged/7c145e2bfc574b9c8ba5f2e4bc572835/"
options.inputFiles=",".join(["file:" + os.path.join(inputDir, f) for f in os.listdir(inputDir)])
options.maxEvents = -1 # -1 means all events

# get and parse the command line arguments
options.parseArguments()

process = cms.Process("VgKit")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
if options.maxEvents < 0:
    process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('START38_V10::All')
process.load("Configuration.StandardSequences.MagneticField_cff")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.maxEvents)
    )

thelist = LumiList(filename="merged_JsonReRecoSep17_JsonStreamExpressV2.txt")

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(options.inputFiles),
                            noEventSort = cms.untracked.bool(True),
                            lumisToProcess = cms.untracked.VLuminosityBlockRange()
                            )

process.source.lumisToProcess = [ str(a) for a in thelist.getCMSSWString().split(',') ] 


process.load("ElectroWeakAnalysis.MultiBosons.VgNtuplizer_cfi")
#process.VgAnalyzerKit.saveHLTInfo = cms.untracked.bool(False);
process.VgNtuplizer.doStoreJets = cms.bool(True);

process.TFileService = cms.Service("TFileService", fileName = cms.string(options.outputFile))

process.p = cms.Path(process.VgNtuplizer)

if __name__ == "__main__": import user
