import FWCore.ParameterSet.Config as cms

from TauAnalysis.Configuration.recoSampleDefinitionsZtoMuTau_7TeV_grid_cfi import recoSampleDefinitionsZtoMuTau_7TeV
from TauAnalysis.Configuration.plotZtoMuTau_drawJobs_cfi import *
from TauAnalysis.Configuration.makePlots2_grid import makePlots
from TauAnalysis.Configuration.userRegistry import getHarvestingFilePath, overrideJobId, getJobId

# import utility function to enable factorization
from TauAnalysis.Configuration.tools.factorizationTools import enableFactorization_makeZtoMuTauPlots_grid2

process = cms.Process('makeZtoMuTauPlots')

process.load("TauAnalysis.Configuration.dumpZtoMuTau_grid_cff")

channel = 'ZtoMuTau'
overrideJobId(channel, '2011Feb01_HPSloose')
inputFilePath = getHarvestingFilePath(channel)
jobId = getJobId(channel)

makePlots(process, channel = channel, samples = recoSampleDefinitionsZtoMuTau_7TeV,
          inputFilePath = inputFilePath, jobId = jobId,
          analyzer_drawJobConfigurator_indOutputFileName_sets = [
              [ "zMuTauAnalyzerOS", drawJobConfigurator_ZtoMuTauOS, "plotZtoMuTauOS_#PLOT#.pdf" ],
	      [ "zMuTauAnalyzerSS", drawJobConfigurator_ZtoMuTauSS, "plotZtoMuTauSS_#PLOT#.pdf" ]
          ],
          drawJobTemplate = plots_ZtoMuTau,
          enableFactorizationFunction = enableFactorization_makeZtoMuTauPlots_grid2,
          dqmDirectoryFilterStatistics = {
              'factorizationDisabled' : 'zMuTauAnalyzerOS/FilterStatistics',
              'factorizationEnabled' : 'zMuTauAnalyzerOS_factorizedWithMuonIsolation/FilterStatistics'
          },
          dumpDQMStore = False)

# print-out all python configuration parameter information
#print process.dumpPython()
