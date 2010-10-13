import FWCore.ParameterSet.Config as cms
from ElectroWeakAnalysis.MultiBosons.tools.VarParsingWithBool \
  import VarParsing

options = VarParsing("analysis")

options.setDefault("outputFile", "VGammaSkim.root")

options.register("globalTag",
  "GR10_P_V7::All", # default value is latest prompt reco (August 2010)
  VarParsing.multiplicity.singleton, # singleton or list
  VarParsing.varType.string,         # bool, string, int, or float
  "Global tag to be used."
  )

options.register("reportEvery",
  100, # default value
  VarParsing.multiplicity.singleton, # singleton or list
  VarParsing.varType.int,            # bool, string, int, or float
  "Frequency of ouput."
  )

options.register("isRealData",
  False, # default value
  VarParsing.multiplicity.singleton, # singleton or list
  VarParsing.varType.bool,           # bool, string, int, or float
  "Is this real data?"
  )

options.register("use35XInput",
  False,
  VarParsing.multiplicity.singleton, # singleton or list
  VarParsing.varType.bool,           # bool, string, int, or float
  "Is this 35X data?"
  )

options.register("isMaxEventsOutput",
  False,
  VarParsing.multiplicity.singleton, # singleton or list
  VarParsing.varType.bool,           # bool, string, int, or float
  "Does maxEvents refer to the output?"
  )

options.register("wantSummary",
  True,
  VarParsing.multiplicity.singleton, # singleton or list
  VarParsing.varType.bool,           # bool, string, int, or float
  "Do you want long trigger and timing reports at the end?"
  )

options.register("hltProcessName",
  "HLT",                             # default value
  VarParsing.multiplicity.singleton, # singleton or list
  VarParsing.varType.string,         # bool, string, int, or float
  "Name of the Process that produced the HLT information."
  )

options.register("hltPaths",
  "",                                # default value
  VarParsing.multiplicity.list,      # singleton or list
  VarParsing.varType.string,         # bool, string, int, or float
  "List of HLT paths whose OR is required for selected events."
  )

options.register("skimType",
  "MuonPhoton", # default value
  VarParsing.multiplicity.singleton, # singleton or list
  VarParsing.varType.string,         # bool, string, int, or float
  "What is the type of this skim, e.g. MuonPhoton?"
  )

options.register("crabOptions",
  "", # default value
  VarParsing.multiplicity.singleton, # singleton or list
  VarParsing.varType.string,         # bool, string, int, or float
  "Pass several options in one string delimited by a `,'?"
  )

options.setupTags(tag = "%s", ifCond = "jobType != ''", tagArg = "jobType")

## Use this to work around bug with passing multiple option parameters with
## crab.  Pass just this one parameter and associate it with several other
## options in applyJobOptions.
options.register("jobType",
  "", # default value
  VarParsing.multiplicity.singleton, # singleton or list
  VarParsing.varType.string,         # bool, string, int, or float
  "What is the type of this job? (Defines multiple options.)"
  )

