import FWCore.ParameterSet.Config as cms

process = cms.Process('harvestWtoTauNuPlots')

from TauAnalysis.Configuration.plotWtoTauNu_processes_cfi import *

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system
#
process.loadWtoTauNu = cms.EDAnalyzer("DQMFileLoader",
    #process# = copy.deepcopy(#inputFileNames#)
)

process.saveWtoTauNu = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string(#outputFileName#)
)
#--------------------------------------------------------------------------------

process.harvestWtoTauNuPlots = cms.Sequence( process.loadWtoTauNu
                                            +process.saveWtoTauNu )

process.p = cms.Path(process.harvestWtoTauNuPlots)

# print-out all python configuration parameter information
#print process.dumpPython()
