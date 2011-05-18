import FWCore.ParameterSet.Config as cms

from TauAnalysis.Configuration.recoSampleDefinitionsWtoTauNu_grid_cfi import recoSampleDefinitionsWtoTauNu_grid
from TauAnalysis.Configuration.makePlots2_grid import makePlots
from TauAnalysis.Configuration.userRegistry import getHarvestingFilePath, getJobId

process = cms.Process('prodBgEstTemplatesWtoTauNu')

channel = 'WtoTauNu_bgEstTemplate'
inputFilePath = getHarvestingFilePath(channel)
jobId = getJobId(channel)

makePlots(process, channel = channel,
          samples = recoSampleDefinitionsWtoTauNu_grid,
          inputFilePath = inputFilePath, jobId = jobId,
          analyzer_drawJobConfigurator_indOutputFileName_sets = [
    [ 'bgEstTemplateAnalyzer_QCDEnriched' ]
    ],
          dqmDirectoryFilterStatistics = {
    'factorizationDisabled' : 'BgEstTemplateAnalyzer_QCDEnriched/FilterStatistics/'
#    'factorizationEnabled' : 'BgEstTemplateAnalyzer_QCDenriched/FilterStatistics/'
    },
          dumpDQMStore = False)

#--------------------------------------------------------------------------------
# Print-out cut-flow information for background enriched samples
# from which template histograms are obtained
#
def configureDQMDumpFilterStatisticsTable(dqmDirectoriesProcess, dqmDirectoryFilterStatisticsTable):
    module = cms.EDAnalyzer("DQMDumpFilterStatisticsTables",
        dqmDirectories = cms.PSet(),
        columnsSummaryTable = cms.vstring("Passed"),
        printSummaryTableOnly = cms.bool(True)                                                    
    )

    for processName, dqmDirectoryProcess in dqmDirectoriesProcess.items():
        dqmDirectory_full = '/harvested' + '/' + dqmDirectoryProcess + '/' + dqmDirectoryFilterStatisticsTable + '/'
        setattr(module.dqmDirectories, processName, cms.string(dqmDirectory_full))

    return module

dqmDirectoriesProcess = {
    'Ztautau'  : 'Ztautau',
    'Wenu'     : 'Wenu',
    'Wmunu'    : 'Wmunu',
    'Wtaunu'   : 'Wtaunu',
    'QCD'      : 'QCD',
    'Data'     : 'data'
}    

process.dumpWtoTauNu_bgEstQCDenriched = configureDQMDumpFilterStatisticsTable(dqmDirectoriesProcess,
    'BgEstTemplateAnalyzer_QCDEnriched/FilterStatistics')

process.dumpDQMStore = cms.EDAnalyzer("DQMStoreDump")

process.dumpWtoTauNuBgEstTemplates = cms.Sequence(
#    process.dumpDQMStore
    process.dumpWtoTauNu_bgEstQCDenriched
)    
#--------------------------------------------------------------------------------

process.q = cms.Path(process.dumpWtoTauNuBgEstTemplates)

process.schedule = cms.Schedule(process.p, process.q)

# print-out all python configuration parameter information
#print process.dumpPython()
