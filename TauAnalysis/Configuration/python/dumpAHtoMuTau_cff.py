import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# Print-out cut-flow information for Z --> mu + tau-jet channel
#--------------------------------------------------------------------------------

dumpAHtoMuTau_woBtag = cms.EDAnalyzer("DQMDumpFilterStatisticsTables",
    dqmDirectories = cms.PSet(
        Ztautau = cms.string('harvested/Ztautau/ahMuTauAnalyzer_woBtag/FilterStatistics/'),
        Zmumu = cms.string('harvested/Zmumu/ahMuTauAnalyzer_woBtag/FilterStatistics/'),
        ZplusJets = cms.string('harvested/ZplusJets/ahMuTauAnalyzer_woBtag/FilterStatistics/'),
        Vqq = cms.string('harvested/Vqq/ahMuTauAnalyzer_woBtag/FilterStatistics/'),
        WplusJets = cms.string('harvested/WplusJets/ahMuTauAnalyzer_woBtag/FilterStatistics/'),
        QCD = cms.string('harvested/qcdSum/ahMuTauAnalyzer_woBtag/FilterStatistics/'),
        TTplusJets = cms.string('harvested/TTplusJets/ahMuTauAnalyzer_woBtag/FilterStatistics/'),
        AH120_tautau = cms.string('harvested/AH120_tautau/ahMuTauAnalyzer_woBtag/FilterStatistics/'),
        AHbb120_tautau = cms.string('harvested/AHbb120_tautau/ahMuTauAnalyzer_woBtag/FilterStatistics/'),
        AH120_tautauSum = cms.string('harvested/AH120_tautauSum/ahMuTauAnalyzer_woBtag/FilterStatistics/')
    ),
    columnsSummaryTable = cms.vstring("Passed", "cumul. Efficiency", "margin. Efficiency", "indiv. Efficiency")
)

dumpAHtoMuTau_wBtag = cms.EDAnalyzer("DQMDumpFilterStatisticsTables",
    dqmDirectories = cms.PSet(
        Ztautau = cms.string('harvested/Ztautau/ahMuTauAnalyzer_wBtag/FilterStatistics/'),
        Zmumu = cms.string('harvested/Zmumu/ahMuTauAnalyzer_wBtag/FilterStatistics/'),
        ZplusJets = cms.string('harvested/ZplusJets/ahMuTauAnalyzer_wBtag/FilterStatistics/'),
        Vqq = cms.string('harvested/Vqq/ahMuTauAnalyzer_wBtag/FilterStatistics/'),
        WplusJets = cms.string('harvested/WplusJets/ahMuTauAnalyzer_wBtag/FilterStatistics/'),
        QCD = cms.string('harvested/qcdSum/ahMuTauAnalyzer_wBtag/FilterStatistics/'),
        TTplusJets = cms.string('harvested/TTplusJets/ahMuTauAnalyzer_wBtag/FilterStatistics/'),
        AH120_tautau = cms.string('harvested/AH120_tautau/ahMuTauAnalyzer_wBtag/FilterStatistics/'),
        AHbb120_tautau = cms.string('harvested/AHbb120_tautau/ahMuTauAnalyzer_wBtag/FilterStatistics/'),
        AH120_tautauSum = cms.string('harvested/AH120_tautauSum/ahMuTauAnalyzer_wBtag/FilterStatistics/')
    ),
    columnsSummaryTable = cms.vstring("Passed", "cumul. Efficiency", "margin. Efficiency", "indiv. Efficiency")
)

dumpAHtoMuTau = cms.Sequence(dumpAHtoMuTau_woBtag * dumpAHtoMuTau_wBtag)

