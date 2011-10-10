import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
## Default PAT process skeleton
from PhysicsTools.PatAlgos.patTemplate_cfg import *
from PhysicsTools.PatAlgos.tools.coreTools import *

# setup 'analysis'  options
options = VarParsing.VarParsing ('analysis')

## default options
options.inputFiles = [("/store/user/apresyan/"
    "SingleGammaFlatPt10To100_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_RAW2DIGI_L1Reco_"
    "noPU_noOOTPU_GEANT4_93p01/"
    "SingleGammaFlatPt10To100_RAW2DIGI_L1Reco_RECO_GEANT_g93p01/"
    "861ce875399c8b0396efbce750d4767e/"
    "SingleGammaFlatPt10To100_RAW2DIGI_L1Reco_RECO_g93p01_9_1_Uoy.root")]
options.outputFile = "r9Tree.root"

## get and parse the command line arguments
options.parseArguments()

removeAllPATObjectsBut(process, ['Photons'])

process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(options.maxEvents)
)

process.source.fileNames = options.inputFiles

## Make the ttree on the fly, don't store edm
del process.outpath

process.TFileService = cms.Service("TFileService",
    fileName = cms.string(options.outputFile)
)

process.load("ElectroWeakAnalysis.MultiBosons.FsrAnalysis.r9Tree_cfi")
process.r9Tree.src = 'patPhotons'

process.p = cms.Path(process.patDefaultSequence + process.r9Tree)

## Enable history and tab-completion for interactive inspection
if __name__ == "__main__": import user

