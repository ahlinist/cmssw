import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# Print-out cut-flow information for Z --> mu + tau-jet channel
#--------------------------------------------------------------------------------

dumpZtoMuTau = cms.EDAnalyzer("DQMDumpFilterStatisticsTables",
    dqmDirectories = cms.PSet(
        Ztautau = cms.string('harvested/Ztautau/zMuTauAnalyzer/FilterStatistics'),
        Zmumu = cms.string('harvested/Zmumu/zMuTauAnalyzer/FilterStatistics/'),
        WplusJets = cms.string('harvested/WplusJets/zMuTauAnalyzer/FilterStatistics/'),
        QCD = cms.string('harvested/qcdSum/zMuTauAnalyzer/FilterStatistics/'),
        TTplusJets = cms.string('harvested/TTplusJets/zMuTauAnalyzer/FilterStatistics')
    ),
    columnsSummaryTable = cms.vstring("Passed", "cumul. Efficiency", "margin. Efficiency", "indiv. Efficiency")
)

dumpZtoMuTauSysUncertainties = cms.EDAnalyzer("DQMDumpBinningResults",
    binningService = cms.PSet(
        pluginType = cms.string("SysUncertaintyBinningService"),
        dqmDirectories = cms.PSet(
            Ztautau = cms.string('harvested/Ztautau/zMuTauAnalyzer/afterDiMuPairZmumuHypothesisVeto/sysUncertaintyBinningResults/'),
            Zmumu = cms.string('harvested/Zmumu/zMuTauAnalyzer/afterDiMuPairZmumuHypothesisVeto/sysUncertaintyBinningResults/'),
            WplusJets = cms.string('harvested/WplusJets/zMuTauAnalyzer/afterDiMuPairZmumuHypothesisVeto/sysUncertaintyBinningResults/'),
            QCD = cms.string('harvested/qcdSum/zMuTauAnalyzer/afterDiMuPairZmumuHypothesisVeto/sysUncertaintyBinningResults/'),
            TTplusJets = cms.string('harvested/TTplusJets/zMuTauAnalyzer/afterDiMuPairZmumuHypothesisVeto/sysUncertaintyBinningResults/')
        )
    )
) 
