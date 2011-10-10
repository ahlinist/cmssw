import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
import FWCore.ParameterSet.Types as CfgTypes
import PhysicsTools.PythonAnalysis.LumiList as LumiList

from PhysicsTools.PatAlgos.tools.cmsswVersionTools import pickRelValInputFiles

# setup 'analysis'  options
options = VarParsing.VarParsing ('analysis')

## register customized options
options.register("jsonFile",
    "", # default value
    VarParsing.VarParsing.multiplicity.singleton, # singleton or list
    VarParsing.VarParsing.varType.string,          # string, int, or float
    "JSON file to be applied."
)

## default options
options.inputFiles = pickRelValInputFiles(
    cmsswVersion  = 'CMSSW_4_1_3',
    relVal        = 'RelValTTbar',
    globalTag     = 'START311_V2',
    numberOfFiles = 1
)


options.outputFile = "vtxTree.root"

## get and parse the command line arguments
options.parseArguments()

process = cms.Process("DEMO")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
## Enable LogDebug
## Remember to recompile with:
## scramv1 b USER_CXXFLAGS="-g\ -D=EDM_ML_DEBUG"
process.MessageLogger.debugModules = ["test"]
process.MessageLogger.cerr.threshold = "DEBUG"

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

# JSON file
if options.jsonFile != "":
    myLumis = LumiList.LumiList(filename = options.jsonFile
                               ).getCMSSWString().split(',')
    process.source.lumisToProcess = \
        CfgTypes.untracked( CfgTypes.VLuminosityBlockRange() )
    process.source.lumisToProcess.extend(myLumis)


process.load('Misc.TreeMaker.vertexTree_cfi')

process.p = cms.Path(
    process.vertexTree
)

## Enable history and tab-completion for interactive inspection
if __name__ == "__main__": import user
