import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# Print-out cut-flow information for Z --> mu + tau-jet channel
#--------------------------------------------------------------------------------

dumpAHtoMuTau_centralJetVeto = cms.EDAnalyzer("DQMDumpFilterStatisticsTables",
    dqmDirectories = cms.PSet(
        Ztautau = cms.string('harvested/Ztautau/ahMuTauAnalyzer_centralJetVeto/FilterStatistics/'),
        Zmumu = cms.string('harvested/Zmumu/ahMuTauAnalyzer_centralJetVeto/FilterStatistics/'),
        WplusJets = cms.string('harvested/WplusJets/ahMuTauAnalyzer_centralJetVeto/FilterStatistics/'),
        QCD = cms.string('harvested/qcdSum/ahMuTauAnalyzer_centralJetVeto/FilterStatistics/'),
        TTplusJets = cms.string('harvested/TTplusJets/ahMuTauAnalyzer_centralJetVeto/FilterStatistics/'),
        AH_tautauSum = cms.string('harvested/AH_tautauSum/ahMuTauAnalyzer_centralJetVeto/FilterStatistics/')
    ),
    columnsSummaryTable = cms.vstring("Passed", "cumul. Efficiency", "margin. Efficiency", "indiv. Efficiency")
)

dumpAHtoMuTau_centralJetBtag = cms.EDAnalyzer("DQMDumpFilterStatisticsTables",
    dqmDirectories = cms.PSet(
        Ztautau = cms.string('harvested/Ztautau/ahMuTauAnalyzer_centralJetBtag/FilterStatistics/'),
        Zmumu = cms.string('harvested/Zmumu/ahMuTauAnalyzer_centralJetBtag/FilterStatistics/'),
        WplusJets = cms.string('harvested/WplusJets/ahMuTauAnalyzer_centralJetBtag/FilterStatistics/'),
        QCD = cms.string('harvested/qcdSum/ahMuTauAnalyzer_centralJetBtag/FilterStatistics/'),
        TTplusJets = cms.string('harvested/TTplusJets/ahMuTauAnalyzer_centralJetBtag/FilterStatistics/'),
        AH_tautauSum = cms.string('harvested/AH_tautauSum/ahMuTauAnalyzer_centralJetBtag/FilterStatistics/')
    ),
    columnsSummaryTable = cms.vstring("Passed", "cumul. Efficiency", "margin. Efficiency", "indiv. Efficiency")
)

dumpAHtoMuTau = cms.Sequence(dumpAHtoMuTau_centralJetVeto * dumpAHtoMuTau_centralJetBtag)

