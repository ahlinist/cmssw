#!/usr/bin/env cmsRun

import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

options = VarParsing.VarParsing ('analysis')

# Register options
options.register(
    'dirout', '',
    VarParsing.VarParsing.multiplicity.singleton,
    VarParsing.VarParsing.varType.string,
    "DQM output folder")

# Register options
options.register(
    'dirin', '',
    VarParsing.VarParsing.multiplicity.list,
    VarParsing.VarParsing.varType.string,
    "DQM input folder - must be in same order as input files")

options.parseArguments()


process = cms.Process('genericScaler')
process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(0))

process.source = cms.Source("EmptySource")

process.load = cms.EDAnalyzer(
    "DQMFileLoader",
    toMerge = cms.PSet(
        inputFileNames = cms.vstring(options.inputFiles),
        dqmDirectory_store = cms.string('/'),
    )
)

# Clean up the file names.  They need forward slashes?
input_dirs = []
for input in options.dirin:
    if input[0] == "/":
        input_dirs.append(input)
    else:
        input_dirs.append("/" + input)

output_dir = options.dirout
if output_dir[0] != "/":
    output_dir = "/" + output_dir

process.add = cms.EDAnalyzer(
    "DQMHistAdder",
    toAdd = cms.PSet(
        dqmDirectories_input = cms.vstring(input_dirs),
        dqmDirectory_output = cms.string(output_dir),
    )
)

print "Input dirs:", process.add.toAdd.dqmDirectories_input
print "Output dirs:", process.add.toAdd.dqmDirectory_output

process.dumpDQMStore = cms.EDAnalyzer("DQMStoreDump")

out_commands = [ "drop %s/*" % dir for dir in options.dirin ]

process.save = cms.EDAnalyzer(
    "DQMSimpleFileSaver",
    outputFileName = cms.string(options.outputFile),
    outputCommands = cms.vstring(out_commands)
)

process.harvest = cms.Sequence(
    process.load
   + process.add
   #+ process.dumpDQMStore
   + process.save
)

process.p = cms.Path(process.harvest)
