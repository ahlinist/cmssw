import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# Print-out cut-flow information for Z --> mu + tau-jet channel
#--------------------------------------------------------------------------------

dumpZtoMuTau = cms.EDAnalyzer("DQMDumpFilterStatisticsTables",
    dqmDirectories = cms.PSet(
        Ztautau = cms.string('Ztautau/zMuTauAnalyzer/FilterStatistics'),
        Zmumu = cms.string('Zmumu/zMuTauAnalyzer/FilterStatistics/'),
        #ZtautauPlusJets = cms.string('ZtautauPlusJets/zMuTauAnalyzer/FilterStatistics'),
        #ZmumuPlusJets = cms.string('ZmumuPlusJets/zMuTauAnalyzer/FilterStatistics/'),
        WplusJets = cms.string('WplusJets/zMuTauAnalyzer/FilterStatistics/'),
        QCD = cms.string('qcdSum/zMuTauAnalyzer/FilterStatistics/'),
        TTplusJets = cms.string('TTbar/zMuTauAnalyzer/FilterStatistics')
    ),
    columnsSummaryTable = cms.vstring("Passed", "cumul. Efficiency", "Efficiency")
)
 
