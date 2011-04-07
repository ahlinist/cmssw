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
# options.outputFile = ".root"
options.maxEvents = -1 # -1 means all events
# options.inputFiles = ["/store/mc/Winter10/DYToMuMu_M-20_CT10_TuneZ2_7TeV-powheg-pythia/GEN-SIM-RECO/E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/0033/FE75602D-4810-E011-B662-1CC1DE051038.root"]

## get and parse the command line arguments
options.parseArguments()


## define the process
process = cms.Process("LISTER")

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

# Events to process
process.source.eventsToProcess = cms.untracked.VEventRange(
    "148864:105502694",
)


## Message logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.AOM = cms.OutputModule("AsciiOutputModule")

process.p = cms.Path(
    process.AOM
)

process.options.wantSummary = True

## Enable LogDebug for analyzeMmgFsr module
# process.MessageLogger.debugModules = ["analyzeMmgFsr"]
# process.MessageLogger.cerr.threshold = "DEBUG"

if __name__ == "__main__": import user

