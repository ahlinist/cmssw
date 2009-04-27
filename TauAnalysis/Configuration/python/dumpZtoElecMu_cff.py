import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# Print-out cut-flow information for Z --> e + mu channel
#--------------------------------------------------------------------------------

dumpZtoElecMu = cms.EDAnalyzer("DQMDumpFilterStatisticsTables",
    dqmDirectories = cms.PSet(
        Ztautau = cms.string('Ztautau/zElecMuAnalyzer/FilterStatistics'),
        Zee = cms.string('Zee/zElecMuAnalyzer/FilterStatistics/'),
        Zmumu = cms.string('Zmumu/zElecMuAnalyzer/FilterStatistics/'),
        WplusJets = cms.string('WplusJets/zElecMuAnalyzer/FilterStatistics/'),
        QCD = cms.string('qcdSum/zElecMuAnalyzer/FilterStatistics/')
    ),
    columnsSummaryTable = cms.vstring("Passed", "cumul. Efficiency", "Efficiency")
)
 
