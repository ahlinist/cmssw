import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

## setup 'analysis'  options
options = VarParsing.VarParsing ('analysis')

## register customized options
def newBoolOption(name, default, description):
    options.register(name,
                     default,
                     options.multiplicity.singleton,
                     options.varType.bool,
                     description)

newBoolOption("applyHltFilter", False, "Apply the HLT filter?")
newBoolOption("applyNoScrapingFilter", True, "Apply the no scraping filter?")
newBoolOption("applyPrimaryVertexFilter", True,
              "Apply the primary vertex filter?")
newBoolOption("applySkimFilter", True, "Apply the skim filter?")

options.register("hltProcessName",
    "REDIGI39X",                    # default value
    options.multiplicity.singleton, # singleton or list
    options.varType.string,         # bool, string, int, or float
    "Name of the Process that produced the HLT information."
    )

## setup any defaults you want
options.outputFile = "MmgFsrRecoAnalyzer.root"
options.maxEvents = -1 # -1 means all events
options.inputFiles = ["/store/mc/Winter10/DYToMuMu_M-20_CT10_TuneZ2_7TeV-powheg-pythia/GEN-SIM-RECO/E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/0033/FE75602D-4810-E011-B662-1CC1DE051038.root"]

## get and parse the command line arguments
options.parseArguments()


## define the process
process = cms.Process("Test")

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring() + options.inputFiles
)

process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(options.maxEvents)
)

## Detector Conditions (needed for a EcalChannelStatusRcd)
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.PyReleaseValidation.autoCond import autoCond
process.GlobalTag.globaltag = cms.string( autoCond[ 'startup' ] )


## Message logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )
process.MessageLogger.cerr.FwkReport.reportEvery = 100

## Load all sequences
process.load("ElectroWeakAnalysis.MultiBosons.Skimming.goodCollisionDataSequence_cff")
process.load("ElectroWeakAnalysis.MultiBosons.Skimming.ZToMMGSkim_cff")
process.load("ElectroWeakAnalysis.MultiBosons.MmgFsrRecoAnalyzer_cfi")

process.ZToMMGSkimFilterSequence.replace(
    process.ZToMMGHltFilter,
    process.ZToMMGHltFilter + process.goodCollisionDataSequence
    )

process.p = cms.Path(process.ZToMMGSkimFilterSequence + process.analyzeMmgFsr)

## Remove the hltPhysicsDeclared - it kills some good events
process.goodCollisionDataSequence.remove(process.hltPhysicsDeclared)

if not options.applyHltFilter:
    process.p.remove(process.ZToMMGHltFilter)

if not options.applyNoScrapingFilter:
    process.p.remove(process.noScraping)

if not options.applyPrimaryVertexFilter:
    process.p.remove(process.primaryVertexFilter)

if not options.applySkimFilter:
    for module in """ZToMMGMuonsCountFilter
                     ZToMMGLooseMuons
                     ZToMMGLooseMuonsCountFilter
                     ZToMMGTightMuons
                     ZToMMGDimuons
                     ZToMMGDimuonsFilter""".split():
        process.p.remove( getattr(process, module) )

process.ZToMMGHltFilter.TriggerResultsTag = cms.InputTag(
    "TriggerResults", "", options.hltProcessName
    )

process.TFileService = cms.Service("TFileService",
  fileName = cms.string('MmgFsrRecoAnalyzer.root')
)

process.options.wantSummary = True

## Enable LogDebug for analyzeMmgFsr module
# process.MessageLogger.debugModules = ["analyzeMmgFsr"]
# process.MessageLogger.cerr.threshold = "DEBUG"

if __name__ == "__main__": import user

