import FWCore.ParameterSet.Config as cms

process = cms.Process('harvestZtoElecTauPlots')

from TauAnalysis.Configuration.plotZtoElecTau_processes_cfi import *

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system
#
process.loadZtoElecTau = cms.EDAnalyzer("DQMFileLoader",
    #process# = copy.deepcopy(#inputFileNames#),
    inputFilePath = cms.string(#inputFilePath#)
)

process.saveZtoElecTau = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string(#outputFileName#)
)
#--------------------------------------------------------------------------------

process.harvestZtoElecTauPlots = cms.Sequence( process.loadZtoElecTau
                                              +process.saveZtoElecTau )

process.p = cms.Path(process.harvestZtoElecTauPlots)

# print-out all python configuration parameter information
#print process.dumpPython()
