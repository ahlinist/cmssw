import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

from PhysicsTools.PatAlgos.tools.cmsswVersionTools import pickRelValInputFiles

# setup 'analysis'  options
options = VarParsing.VarParsing ('analysis')

## default options
options.inputFiles = pickRelValInputFiles(
    cmsswVersion  = 'CMSSW_4_1_3',
    relVal        = 'RelValTTbar',
    globalTag     = 'START311_V2',
    numberOfFiles = 1
)


options.outputFile = "r9Tree.root"

## get and parse the command line arguments
options.parseArguments()

process = cms.Process("TTREE")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(options.maxEvents)
)

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring() + options.inputFiles
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string(options.outputFile)
)

process.load("ElectroWeakAnalysis.MultiBosons.FsrAnalysis.r9Tree_cfi")

process.p = cms.Path(process.r9Tree)

## Enable history and tab-completion for interactive inspection
if __name__ == "__main__": import user
