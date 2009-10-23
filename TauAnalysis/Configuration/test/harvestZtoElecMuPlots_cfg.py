import FWCore.ParameterSet.Config as cms

process = cms.Process('harvestZtoElecMuPlots')

from TauAnalysis.Configuration.plotZtoElecMu_processes_cfi import *

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system
#
process.loadZtoElecMu = cms.EDAnalyzer("DQMFileLoader",
    #process# = copy.deepcopy(#inputFileNames#),
    inputFilePath = cms.string(#inputFilePath#)
)

process.saveZtoElecMu = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string(#outputFileName#)
)
#--------------------------------------------------------------------------------

process.harvestZtoElecMuPlots = cms.Sequence( process.loadZtoElecMu
                                             +process.saveZtoElecMu )

process.p = cms.Path(process.harvestZtoElecMuPlots)

# print-out all python configuration parameter information
#print process.dumpPython()
