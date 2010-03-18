import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# Print-out cut-flow information for Z --> mu + tau-jet channel
#--------------------------------------------------------------------------------

dumpAHtoMuTau_woBtag = cms.EDAnalyzer("DQMDumpFilterStatisticsTables",
    dqmDirectories = cms.PSet(
        Ztautau = cms.string('harvested/Ztautau/ahMuTauAnalyzer_woBtag/FilterStatistics/'),
        Zmumu = cms.string('harvested/Zmumu/ahMuTauAnalyzer_woBtag/FilterStatistics/'),
        WplusJets = cms.string('harvested/WplusJets/ahMuTauAnalyzer_woBtag/FilterStatistics/'),
        QCD = cms.string('harvested/qcdSum/ahMuTauAnalyzer_woBtag/FilterStatistics/'),
        TTplusJets = cms.string('harvested/TTplusJets/ahMuTauAnalyzer_woBtag/FilterStatistics/'),
        AH_tautauSum = cms.string('harvested/AH_tautauSum/ahMuTauAnalyzer_woBtag/FilterStatistics/')
    ),
    columnsSummaryTable = cms.vstring("Passed", "cumul. Efficiency", "margin. Efficiency", "indiv. Efficiency")
)

dumpAHtoMuTau_wBtag = cms.EDAnalyzer("DQMDumpFilterStatisticsTables",
    dqmDirectories = cms.PSet(
        Ztautau = cms.string('harvested/Ztautau/ahMuTauAnalyzer_wBtag/FilterStatistics/'),
        Zmumu = cms.string('harvested/Zmumu/ahMuTauAnalyzer_wBtag/FilterStatistics/'),
        WplusJets = cms.string('harvested/WplusJets/ahMuTauAnalyzer_wBtag/FilterStatistics/'),
        QCD = cms.string('harvested/qcdSum/ahMuTauAnalyzer_wBtag/FilterStatistics/'),
        TTplusJets = cms.string('harvested/TTplusJets/ahMuTauAnalyzer_wBtag/FilterStatistics/'),
        AH_tautauSum = cms.string('harvested/AH_tautauSum/ahMuTauAnalyzer_wBtag/FilterStatistics/')
    ),
    columnsSummaryTable = cms.vstring("Passed", "cumul. Efficiency", "margin. Efficiency", "indiv. Efficiency")
)

dumpAHtoMuTau = cms.Sequence(dumpAHtoMuTau_woBtag * dumpAHtoMuTau_wBtag)

