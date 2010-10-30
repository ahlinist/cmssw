import FWCore.ParameterSet.Config as cms

from TauAnalysis.Configuration.recoSampleDefinitionsAHtoMuTau_7TeV_grid_cfi import recoSampleDefinitionsAHtoMuTau_7TeV
from TauAnalysis.Configuration.makePlots_grid import makePlots
from TauAnalysis.Configuration.userRegistry import getHarvestingFilePath, getJobId

# import utility function to enable factorization
from TauAnalysis.Configuration.tools.factorizationTools import enableFactorization_makeAHtoMuTauPlots_grid

process = cms.Process('makeAHtoMuTauPlots')

process.load("TauAnalysis.Configuration.dumpAHtoMuTau_grid_cff")
process.load("TauAnalysis.Configuration.plotAHtoMuTau_grid_cff")

channel = 'AHtoMuTau'
inputFilePath = getHarvestingFilePath(channel)
jobId = getJobId(channel)

makePlots(process, channel = channel, samples = recoSampleDefinitionsAHtoMuTau_7TeV,
          inputFilePath = inputFilePath, jobId = jobId,
          enableFactorizationFunction = enableFactorization_makeAHtoMuTauPlots_grid, dumpDQMStore = False)

# print-out all python configuration parameter information
#print process.dumpPython()
