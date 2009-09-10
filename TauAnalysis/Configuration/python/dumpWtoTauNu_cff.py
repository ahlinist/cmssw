import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# Print-out cut-flow information for W --> tau + nuc channel
#--------------------------------------------------------------------------------

dumpWtoTauNu = cms.EDAnalyzer("DQMDumpFilterStatisticsTables",
    dqmDirectories = cms.PSet(
        Wtaunu = cms.string('harvested/Wtaunu/wTauNuAnalyzer/FilterStatistics'),
        qcd_W = cms.string('harvested/qcd_W/wTauNuAnalyzer/FilterStatistics/'),
        Wmunu = cms.string('harvested/Wmunu/wTauNuAnalyzer/FilterStatistics/'),
        Wenu = cms.string('harvested/Wenu/wTauNuAnalyzer/FilterStatistics/'),
	ZplusJets = cms.string('harvested/ZplusJets/wTauNuAnalyzer/FilterStatistics/'),
        TTplusJets = cms.string('harvested/TTplusJets/wTauNuAnalyzer/FilterStatistics')
    ),
    columnsSummaryTable = cms.vstring("Passed", "cumul. Efficiency", "margin. Efficiency","indiv. Efficiency")
)
 
