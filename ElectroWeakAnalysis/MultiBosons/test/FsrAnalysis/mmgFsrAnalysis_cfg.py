import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
import FWCore.ParameterSet.Types as CfgTypes
import PhysicsTools.PythonAnalysis.LumiList as LumiList

## setup 'analysis'  options
options = VarParsing.VarParsing ('analysis')

## register customized options
options.register("jsonFile",
    "Cert_136033-149442_7TeV_Dec22ReReco_Collisions10_JSON_v3.txt", # default value
    VarParsing.VarParsing.multiplicity.singleton, # singleton or list
    VarParsing.VarParsing.varType.string,          # string, int, or float
    "JSON file to be applied."
)

## setup any defaults you want
options.outputFile = "MmgFsrAnalysis.root"
options.maxEvents = -1 # -1 means all events
# options.inputFiles = ["/store/mc/Winter10/DYToMuMu_M-20_CT10_TuneZ2_7TeV-powheg-pythia/GEN-SIM-RECO/E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/0033/FE75602D-4810-E011-B662-1CC1DE051038.root"]

## get and parse the command line arguments
options.parseArguments()


## define the process
process = cms.Process("ANALYSIS")

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring() + options.inputFiles
)
# JSON file
if options.jsonFile != "":
    myLumis = \
        LumiList.LumiList(filename = options.jsonFile
                          ).getCMSSWString().split(',')
    process.source.lumisToProcess = \
        CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
    process.source.lumisToProcess.extend(myLumis)

process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(options.maxEvents)
)

## Detector Conditions (needed for the EcalChannelStatusRcd)
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.PyReleaseValidation.autoCond import autoCond
process.GlobalTag.globaltag = cms.string( autoCond[ 'startup' ] )


## Message logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )
process.MessageLogger.cerr.FwkReport.reportEvery = 100

## Load all sequences
process.load("ElectroWeakAnalysis.MultiBosons.FsrAnalysis.defaultPhotons_cfi")
process.load("ElectroWeakAnalysis.MultiBosons.FsrAnalysis.mmgFsrFilter_cfi")

## Customize defaults
process.mmgFsrFilter.photonSrc = "defaultPhotons"
process.mmgFsrFilter.verbosity = 2
# process.mmgFsrFilter.muonSrc   = "cleanPatMuonsTriggerMatch"

process.eca = cms.EDAnalyzer("EventContentAnalyzer")

process.p = cms.Path(
    process.eca +
    process.defaultPhotons *
    process.mmgFsrFilter
)

process.TFileService = cms.Service("TFileService",
  fileName = cms.string(options.outputFile)
)

process.options.wantSummary = True

## Enable LogDebug for analyzeMmgFsr module
# process.MessageLogger.debugModules = ["analyzeMmgFsr"]
# process.MessageLogger.cerr.threshold = "DEBUG"

if __name__ == "__main__": import user

