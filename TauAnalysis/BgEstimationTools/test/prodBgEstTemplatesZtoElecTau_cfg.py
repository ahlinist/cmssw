import FWCore.ParameterSet.Config as cms

from TauAnalysis.Configuration.recoSampleDefinitionsZtoElecTau_grid_cfi import recoSampleDefinitionsZtoElecTau
from TauAnalysis.Configuration.recoSampleDefinitionsAHtoElecTau_grid_cfi import recoSampleDefinitionsAHtoElecTau
from TauAnalysis.Configuration.makePlots2_grid import makePlots
from TauAnalysis.Configuration.userRegistry import getHarvestingFilePath, getJobId

from TauAnalysis.Configuration.plotZtoElecTau_drawJobs_cfi import \
        drawJobConfigurator_ForElecTau, plots_ZtoElecTau



process = cms.Process('prodBgEstTemplatesZtoElecTau')

channel = 'ZtoElecTau_bgEstTemplate'

#channel = 'Run01'

inputFilePath = getHarvestingFilePath(channel)
#inputFilePath = '/afs/crc.nd.edu/user/l/lantonel/CMSSW_3_8_7_patch1/src/TauAnalysis/BgEstimationTools/test'
jobId = getJobId(channel)
#jobId = 1

makePlots(process, channel = channel, samples = recoSampleDefinitionsZtoElecTau,
          inputFilePath = inputFilePath, jobId = jobId,
          analyzer_drawJobConfigurator_indOutputFileName_sets = [
##                [ 'BgEstTemplateAnalyzer_ZeeJetMisIdEnriched' ],
##                [ 'BgEstTemplateAnalyzer_ZeeElectronMisIdEnriched' ],
##                [ 'BgEstTemplateAnalyzer_WplusJetsEnriched' ],
##                [ 'BgEstTemplateAnalyzer_PhotonPlusJetsEnriched' ],
##                [ 'BgEstTemplateAnalyzer_QCDenriched' ],
##                [ 'BgEstTemplateAnalyzer_ZtautauEnriched' ]
                  [ "BgEstTemplateAnalyzer_ZtautauEnriched", drawJobConfigurator_ForElecTau, "plotZtoElecTau_#PLOT#.png" ],
##                [ 'BgEstTemplateAnalyzer_ZtautauEnriched',drawJobConfigurator_ForElecTau, "plotZtoElecTau_#PLOT#.png"  ]
          ],
          dqmDirectoryFilterStatistics = {
              'factorizationDisabled' : 'BgEstTemplateAnalyzer_ZtautauEnriched/FilterStatistics/',
              'factorizationEnabled' : 'BgEstTemplateAnalyzer_ZtautauEnriched/FilterStatistics/'
          },
          dumpDQMStore = False,
          drawJobTemplate = plots_ZtoElecTau
          )

#--------------------------------------------------------------------------------
# Print-out cut-flow information for background enriched samples
# from which template histograms are obtained
#
def configureDQMDumpFilterStatisticsTable(dqmDirectoriesProcess, dqmDirectoryFilterStatisticsTable):
    module = cms.EDAnalyzer("DQMDumpFilterStatisticsTables",
        dqmDirectories = cms.PSet(),
        #columnsSummaryTable = cms.vstring("Passed"),
        #columnsSummaryTable = cms.vstring("Passed","indiv. Efficiency"),
        printSummaryTableOnly = cms.bool(False)                                                    
    )

    for processName, dqmDirectoryProcess in dqmDirectoriesProcess.items():
        dqmDirectory_full = '/harvested/' + dqmDirectoryProcess + '/' + dqmDirectoryFilterStatisticsTable + '/'
        setattr(module.dqmDirectories, processName, cms.string(dqmDirectory_full))

    return module

dqmDirectoriesProcess = {
    'Ztautau'        : 'ZtautauSum',
    'Zee'            : 'ZeeSum',
    'WplusJets'      : 'WplusJets_madgraph_pat',
    'QCD'            : 'qcdSum',
#    'PhotonPlusJets' : 'photonPlusJetsSum',
    'Data'           : 'data'
}    

process.dumpZtoElecTau_bgEstZtautauEnriched = configureDQMDumpFilterStatisticsTable(dqmDirectoriesProcess,
    'BgEstTemplateAnalyzer_ZtautauEnriched/FilterStatistics')
#process.dumpZtoElecTau_bgEstZeeJetMisIdEnriched = configureDQMDumpFilterStatisticsTable(dqmDirectoriesProcess,
#    'BgEstTemplateAnalyzer_ZeeJetMisIdEnriched/FilterStatistics')
process.dumpZtoElecTau_bgEstZeeElectronMisIdEnriched = configureDQMDumpFilterStatisticsTable(dqmDirectoriesProcess,
    'BgEstTemplateAnalyzer_ZeeElectronMisIdEnriched/FilterStatistics')
process.dumpZtoElecTau_bgEstWplusJetsEnriched = configureDQMDumpFilterStatisticsTable(dqmDirectoriesProcess,
    'BgEstTemplateAnalyzer_WplusJetsEnriched/FilterStatistics')
#process.dumpZtoElecTau_bgEstPhotonPlusJetsEnriched = configureDQMDumpFilterStatisticsTable(dqmDirectoriesProcess,
#    'BgEstTemplateAnalyzer_PhotonPlusJetsEnriched/FilterStatistics')
process.dumpZtoElecTau_bgEstQCDenriched = configureDQMDumpFilterStatisticsTable(dqmDirectoriesProcess,
    'BgEstTemplateAnalyzer_QCDenriched/FilterStatistics')


process.dumpDQMStore = cms.EDAnalyzer("DQMStoreDump")

process.dumpZtoElecTauBgEstTemplates = cms.Sequence(
    #process.dumpDQMStore
    process.dumpZtoElecTau_bgEstZtautauEnriched
#    + process.dumpZtoElecTau_bgEstZeeJetMisIdEnriched
    + process.dumpZtoElecTau_bgEstZeeElectronMisIdEnriched
    + process.dumpZtoElecTau_bgEstWplusJetsEnriched
#    + process.dumpZtoElecTau_bgEstPhotonPlusJetsEnriched
    + process.dumpZtoElecTau_bgEstQCDenriched
)    
#--------------------------------------------------------------------------------

process.q = cms.Path(process.dumpZtoElecTauBgEstTemplates)

process.schedule = cms.Schedule(process.p, process.q)

# print-out all python configuration parameter information
#print process.dumpPython()
