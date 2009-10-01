import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# Print-out cut-flow information for Z --> e + tau-jet channel
#--------------------------------------------------------------------------------

dumpZtoElecTau = cms.EDAnalyzer("DQMDumpFilterStatisticsTables",
    dqmDirectories = cms.PSet(
        Ztautau = cms.string('harvested/Ztautau/zElecTauAnalyzer/FilterStatistics/'),
        Zee = cms.string('harvested/Zee/zElecTauAnalyzer/FilterStatistics/'),
        ZtautauPlusJets = cms.string('harvested/ZtautauPlusJets/zElecTauAnalyzer/FilterStatistics/'),
        ZeePlusJets = cms.string('harvested/ZeePlusJets/zElecTauAnalyzer/FilterStatistics/'),
        WplusJets = cms.string('harvested/WplusJets/zElecTauAnalyzer/FilterStatistics/'),
        TTplusJets = cms.string('harvested/TTplusJets/zElecTauAnalyzer/FilterStatistics/'),
        QCD = cms.string('harvested/qcdSum/zElecTauAnalyzer/FilterStatistics/'),
        gammaPlusJetsSum = cms.string('harvested/gammaPlusJetsSum/zElecTauAnalyzer/FilterStatistics/')
    ),
    columnsSummaryTable = cms.vstring("Passed", "cumul. Efficiency", "margin. Efficiency", "indiv. Efficiency", "excl. Rejection")
)
 
