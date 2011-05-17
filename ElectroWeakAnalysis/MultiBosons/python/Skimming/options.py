import FWCore.ParameterSet.Config as cms
from ElectroWeakAnalysis.MultiBosons.tools.VarParsingWithBool \
  import VarParsing

options = VarParsing("analysis")

options.setDefault("outputFile", "VGammaPAT.root")

options.register("globalTag",
    "GR10_P_V7::All", # default value is latest prompt reco (August 2010)
    VarParsing.multiplicity.singleton, # singleton or list
    VarParsing.varType.string,         # bool, string, int, or float
    "Global tag to be used.")
options.register("reportEvery",
    100, # default value
    VarParsing.multiplicity.singleton, # singleton or list
    VarParsing.varType.int,            # bool, string, int, or float
    "Frequency of ouput.")
options.register("isRealData",
    False, # default value
    VarParsing.multiplicity.singleton, # singleton or list
    VarParsing.varType.bool,           # bool, string, int, or float
    "Is this real data?")
options.register("use35XInput",
    False,
    VarParsing.multiplicity.singleton, # singleton or list
    VarParsing.varType.bool,           # bool, string, int, or float
    "Is this 35X data?")
options.register("isAOD",
    False,
    VarParsing.multiplicity.singleton, # singleton or list
    VarParsing.varType.bool,           # bool, string, int, or float
    "Is this AOD data or RECO?")
options.register("outEvents",
    -1,
    VarParsing.multiplicity.singleton, # singleton or list
    VarParsing.varType.int,            # bool, string, int, or float
    "What is the maximum number of output events?")
options.register("wantSummary",
    True,
    VarParsing.multiplicity.singleton, # singleton or list
    VarParsing.varType.bool,           # bool, string, int, or float
    "Do you want long trigger and timing reports at the end?")
options.register("hltProcessName",
    "HLT",                             # default value
    VarParsing.multiplicity.singleton, # singleton or list
    VarParsing.varType.string,         # bool, string, int, or float
    "Name of the Process that produced the HLT information.")
options.register("skimType",
    "MuonPhoton", # default value
    VarParsing.multiplicity.singleton, # singleton or list
    VarParsing.varType.string,         # bool, string, int, or float
    "One of MuonPhoton, ElectronPhoton, Dimuon, Jet or Inclusive.")
options.register("crabOptions",
    "", # default value
    VarParsing.multiplicity.singleton, # singleton or list
    VarParsing.varType.string,         # bool, string, int, or float
    "Pass several options in one string delimited by a `,'?")

options.register("ignoreSkimFilter",
    False, # default value
    VarParsing.multiplicity.singleton, # singleton or list
    VarParsing.varType.bool,         # bool, string, int, or float
    "Ignore the skimming filter?")

options.register("applyCollisionDataCleaningToMC",
    False, # default value
    VarParsing.multiplicity.singleton, # singleton or list
    VarParsing.varType.bool,         # bool, string, int, or float
    "Apply the collision data cleaning to MC?")

options.register("isPOWHEG",
                 False, # default value
                 VarParsing.multiplicity.singleton, # singleton or list
                 VarParsing.varType.bool,         # bool, string, int, or float
                 "Is this MC sample POWHEG?")

options.register("vetoISR",
                 False, # default value
                 VarParsing.multiplicity.singleton, # singleton or list
                 VarParsing.varType.bool,         # bool, string, int, or float
                 "Do you want to veto pythia ISR?")
options.register("vetoFSR",
                 False, # default value
                 VarParsing.multiplicity.singleton, # singleton or list
                 VarParsing.varType.bool,         # bool, string, int, or float
                 "Do you want to veto pythia FSR?")
options.register("skimISR",
                 False, # default value
                 VarParsing.multiplicity.singleton, # singleton or list
                 VarParsing.varType.bool,         # bool, string, int, or float
                 "Do you want to skim for pythia ISR?")
options.register("skimFSR",
                 False, # default value
                 VarParsing.multiplicity.singleton, # singleton or list
                 VarParsing.varType.bool,         # bool, string, int, or float
                 "Do you want to skim for pythia FSR?")
options.register("addRho",
                 True, # default value
                 VarParsing.multiplicity.singleton, # singleton or list
                 VarParsing.varType.bool,         # bool, string, int, or float
                 "Add rho from FastJet for pile-up corrected calo isolations?")

## setup tags
options.setupTags (tag = 'of_%d',
    ifCond = 'totalSections > 0',
    tagArg = 'totalSections'
)
options.setupTags (tag = '%d',
    ifCond = 'totalSections in range(1,10)',
    tagArg = 'section'
)
options.setupTags (tag = '%2d',
    ifCond = 'totalSections in range(10,100)',
    tagArg = 'section'
)
options.setupTags (tag = '%3d',
    ifCond = 'totalSections >= 100',
    tagArg = 'section'
)
options.setupTags(tag = "%doutEvents",
                  ifCond = "outEvents > 0",
                  tagArg = "outEvents")
options.setupTags(tag = "%s",
                  ifCond = "jobType != ''",
                  tagArg = "jobType")
options.setupTags(tag = "%sSkim",
                  ifCond = "skimType != ''",
                  tagArg = "skimType")

## Use this to work around bug with passing multiple option parameters with
## crab.  Pass just this one parameter and associate it with several other
## options in applyJobOptions.
options.register("jobType",
  "", # default value
  VarParsing.multiplicity.singleton, # singleton or list
  VarParsing.varType.string,         # bool, string, int, or float
  "What is the type of this job? (Defines multiple options.)"
  )

