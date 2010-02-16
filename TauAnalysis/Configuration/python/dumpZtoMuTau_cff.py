import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# Print-out cut-flow information for Z --> mu + tau-jet channel
#--------------------------------------------------------------------------------

dumpZtoMuTau = cms.EDAnalyzer("DQMDumpFilterStatisticsTables",
    dqmDirectories = cms.PSet(
        Ztautau = cms.string('harvested/Ztautau/zMuTauAnalyzer/FilterStatistics/'),
        Zmumu = cms.string('harvested/Zmumu/zMuTauAnalyzer/FilterStatistics/'),
        WplusJets = cms.string('harvested/WplusJets/zMuTauAnalyzer/FilterStatistics/'),
        QCD = cms.string('harvested/qcdSum/zMuTauAnalyzer/FilterStatistics/'),
        TTplusJets = cms.string('harvested/TTplusJets/zMuTauAnalyzer/FilterStatistics/')
    ),
    columnsSummaryTable = cms.vstring("Passed", "cumul. Efficiency", "margin. Efficiency", "indiv. Efficiency")
)

