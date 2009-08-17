import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# Print-out cut-flow information for Z --> e + mu channel
#--------------------------------------------------------------------------------

dumpZtoElecMu = cms.EDAnalyzer("DQMDumpFilterStatisticsTables",
    dqmDirectories = cms.PSet(
        ZtautauPlusJets = cms.string('harvested/ZtautauPlusJets/zElecMuAnalyzer/FilterStatistics'),
        Ztautau = cms.string('harvested/Ztautau/zElecMuAnalyzer/FilterStatistics'),
        ZmumuPlusJets = cms.string('harvested/ZmumuPlusJets/zElecMuAnalyzer/FilterStatistics/'),
        Zmumu = cms.string('harvested/Zmumu/zElecMuAnalyzer/FilterStatistics/'),
        ZeePlusJets = cms.string('harvested/ZeePlusJets/zElecMuAnalyzer/FilterStatistics/'),
        WplusJets = cms.string('harvested/WplusJets/zElecMuAnalyzer/FilterStatistics/'),
        TTplusJets = cms.string('harvested/TTplusJets/zElecMuAnalyzer/FilterStatistics/'),
        QCD = cms.string('harvested/qcdSum/zElecMuAnalyzer/FilterStatistics/')
    ),
    columnsSummaryTable = cms.vstring("Passed", "cumul. Efficiency", "Efficiency")
)
 
