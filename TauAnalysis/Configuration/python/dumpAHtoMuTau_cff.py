import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# Print-out cut-flow information for Z --> mu + tau-jet channel
#--------------------------------------------------------------------------------

dumpAHtoMuTau_woBtag = cms.EDAnalyzer("DQMDumpFilterStatisticsTables",
    dqmDirectories = cms.PSet(
        Ztautau = cms.string('harvested/Ztautau/ahMuTauAnalyzerOS_woBtag/FilterStatistics/'),
        Zmumu = cms.string('harvested/Zmumu/ahMuTauAnalyzerOS_woBtag/FilterStatistics/'),
        ZplusJets = cms.string('harvested/ZplusJets/ahMuTauAnalyzerOS_woBtag/FilterStatistics/'),
        Vqq = cms.string('harvested/Vqq/ahMuTauAnalyzerOS_woBtag/FilterStatistics/'),
        WplusJets = cms.string('harvested/WplusJetsSum/ahMuTauAnalyzerOS_woBtag/FilterStatistics/'),
        QCD = cms.string('harvested/qcdSum/ahMuTauAnalyzerOS_woBtag/FilterStatistics/'),
        TTplusJets = cms.string('harvested/TTplusJets/ahMuTauAnalyzerOS_woBtag/FilterStatistics/'),
        AH120_tautau = cms.string('harvested/AH120_tautau/ahMuTauAnalyzerOS_woBtag/FilterStatistics/'),
        AHbb120_tautau = cms.string('harvested/AHbb120_tautau/ahMuTauAnalyzerOS_woBtag/FilterStatistics/'),
        AHsum120_tautau = cms.string('harvested/AHsum120_tautau/ahMuTauAnalyzerOS_woBtag/FilterStatistics/')
    ),
    columnsSummaryTable = cms.vstring("Passed", "cumul. Efficiency", "margin. Efficiency", "indiv. Efficiency")
)

dumpAHtoMuTau_wBtag = cms.EDAnalyzer("DQMDumpFilterStatisticsTables",
    dqmDirectories = cms.PSet(
        Ztautau = cms.string('harvested/Ztautau/ahMuTauAnalyzerOS_wBtag/FilterStatistics/'),
        Zmumu = cms.string('harvested/Zmumu/ahMuTauAnalyzerOS_wBtag/FilterStatistics/'),
        ZplusJets = cms.string('harvested/ZplusJets/ahMuTauAnalyzerOS_wBtag/FilterStatistics/'),
        Vqq = cms.string('harvested/Vqq/ahMuTauAnalyzerOS_wBtag/FilterStatistics/'),
        WplusJets = cms.string('harvested/WplusJetsSum/ahMuTauAnalyzerOS_wBtag/FilterStatistics/'),
        QCD = cms.string('harvested/qcdSum/ahMuTauAnalyzerOS_wBtag/FilterStatistics/'),
        TTplusJets = cms.string('harvested/TTplusJets/ahMuTauAnalyzerOS_wBtag/FilterStatistics/'),
        AH120_tautau = cms.string('harvested/AH120_tautau/ahMuTauAnalyzerOS_wBtag/FilterStatistics/'),
        AHbb120_tautau = cms.string('harvested/AHbb120_tautau/ahMuTauAnalyzerOS_wBtag/FilterStatistics/'),
        AHsum120_tautau = cms.string('harvested/AHsum120_tautau/ahMuTauAnalyzerOS_wBtag/FilterStatistics/')
    ),
    columnsSummaryTable = cms.vstring("Passed", "cumul. Efficiency", "margin. Efficiency", "indiv. Efficiency")
)

dumpAHtoMuTau = cms.Sequence(dumpAHtoMuTau_woBtag * dumpAHtoMuTau_wBtag)

