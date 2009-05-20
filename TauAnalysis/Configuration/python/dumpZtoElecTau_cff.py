import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# Print-out cut-flow information for Z --> e + tau-jet channel
#--------------------------------------------------------------------------------

dumpZtoElecTau = cms.EDAnalyzer("DQMDumpFilterStatisticsTables",
    dqmDirectories = cms.PSet(
        Ztautau = cms.string('Ztautau/zElecTauAnalyzer/FilterStatistics'),
#        Zmumu = cms.string('Zmumu/zElecTauAnalyzer/FilterStatistics/'),
#        WplusJets = cms.string('WplusJets/zElecTauAnalyzer/FilterStatistics/'),
#        QCD = cms.string('qcdSum/zElecTauAnalyzer/FilterStatistics/')
    ),
    columnsSummaryTable = cms.vstring("Passed", "cumul. Efficiency", "Efficiency")
)
 
