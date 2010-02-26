import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# Print-out cut-flow information for W --> tau + nu channel
#--------------------------------------------------------------------------------

dumpWtoTauNu = cms.EDAnalyzer("DQMDumpFilterStatisticsTables",
    dqmDirectories = cms.PSet(
        Wtaunu = cms.string('harvested/Wtaunu/wTauNuAnalyzer/FilterStatistics'),
        Wmunu = cms.string('harvested/Wmunu/wTauNuAnalyzer/FilterStatistics/'),
        Wenu = cms.string('harvested/Wenu/wTauNuAnalyzer/FilterStatistics/'),
	#ZplusJets = cms.string('harvested/ZplusJets/wTauNuAnalyzer/FilterStatistics/'),
        qcd_W = cms.string('qcd/wTauNuAnalyzer/FilterStatistics/')
        ),
    columnsSummaryTable = cms.vstring("Passed", "cumul. Efficiency", "margin. Efficiency","indiv. Efficiency")
)

dumpWtoTauNu_boosted = cms.EDAnalyzer("DQMDumpFilterStatisticsTables",
    dqmDirectories = cms.PSet(
        Wtaunu = cms.string('harvested/Wtaunu/wTauNuBoostedAnalyzer/FilterStatistics'),
        Wmunu = cms.string('harvested/Wmunu/wTauNuBoostedAnalyzer/FilterStatistics/'),
        Wenu = cms.string('harvested/Wenu/wTauNuBoostedAnalyzer/FilterStatistics/'),
	#ZplusJets = cms.string('harvested/ZplusJets/wTauNuBoostedAnalyzer/FilterStatistics/'),
        qcd_W = cms.string('qcd/wTauNuBoostedAnalyzer/FilterStatistics/')
        ),
    columnsSummaryTable = cms.vstring("Passed", "cumul. Efficiency", "margin. Efficiency","indiv. Efficiency")
) 
