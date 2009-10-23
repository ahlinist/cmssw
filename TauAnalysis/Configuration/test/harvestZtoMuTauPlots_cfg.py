import FWCore.ParameterSet.Config as cms

process = cms.Process('harvestZtoMuTauPlots')

from TauAnalysis.Configuration.plotZtoMuTau_processes_cfi import *

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system
#
process.loadZtoMuTau = cms.EDAnalyzer("DQMFileLoader",
    #process# = copy.deepcopy(#inputFileNames#),
    inputFilePath = cms.string(#inputFilePath#)                                      
)

process.saveZtoMuTau = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string(#outputFileName#)
)
#--------------------------------------------------------------------------------

process.harvestZtoMuTauPlots = cms.Sequence( process.loadZtoMuTau
                                            +process.saveZtoMuTau )

process.p = cms.Path(process.harvestZtoMuTauPlots)

# print-out all python configuration parameter information
#print process.dumpPython()
