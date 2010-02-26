import FWCore.ParameterSet.Config as cms

process = cms.Process('harvestWtoTauNuPlots')

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system
#
from #recoSampleDefinitionsFile# import *

process.loadWtoTauNu = cms.EDAnalyzer("DQMFileLoader",
   #process# = copy.deepcopy(#inputFileNames#),
   inputFilePath = cms.string(#inputFilePath#)
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
