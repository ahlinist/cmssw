import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

from PhysicsTools.PatAlgos.tools.cmsswVersionTools import *
from PhysicsTools.PatAlgos.tools.coreTools import *

def applyOptions(process,options):
    options.parseArguments()

    #process.MessageLogger.FwkReport.reportEvery=options.reportEvery
    if options.globalTag == "":
        raise ValueError('\"\" is not a valid option for the global tag!')
    else:
        process.GlobalTag.globaltag = options.globalTag
    #remove MC matching if we run on data
    if options.isData == 'True':
        removeMCMatching(process)
        removeMCMatching(process,names=['METs'],postfix="TC")
        removeMCMatching(process,names=['METs'],postfix="PF")
    ## Spring10 MC was produced with CMSS_3_5_6 - make sure we can run on it
    if options.Use35XInput == 'True':
        run36xOn35xInput(process, genJets = "ak5GenJets")


options = VarParsing.VarParsing("analysis")
options.register("globalTag",
                 "GR_P_V7::All", # default value is latest prompt reco (August 2010)
                 VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                 VarParsing.VarParsing.varType.string,         # string, int, or float
                 "Global tag to be used."
                 )
options.register("reportEvery",
                 100, # default value
                 VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                 VarParsing.VarParsing.varType.int,          # string, int, or float
                 "Frequency of ouput."
                 )
options.register("isData",
                 'False', # default value
                 VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                 VarParsing.VarParsing.varType.string,         # string, int, or float
                 "Is this real data?"
                 )
options.register("Use35XInput",
                 False,
                 VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                 VarParsing.VarParsing.varType.string,         # string, int, or float
                 "Is this 35X data?"
                 )
