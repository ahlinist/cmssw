import FWCore.ParameterSet.Config as cms

process = cms.Process('harvestbbAHtoElecTauPlots')

from TauAnalysis.Configuration.plotbbAHtoElecTau_processes_cfi import *

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system
#
process.loadbbAHtoElecTau = cms.EDAnalyzer("DQMFileLoader",
    #process# = copy.deepcopy(#inputFileNames#)
)

process.savebbAHtoElecTau = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string(#outputFileName#)
)
#--------------------------------------------------------------------------------

process.harvestbbAHtoElecTauPlots = cms.Sequence( process.loadbbAHtoElecTau
                                              +process.savebbAHtoElecTau )

process.p = cms.Path(process.harvestbbAHtoElecTauPlots)

# print-out all python configuration parameter information
#print process.dumpPython()
