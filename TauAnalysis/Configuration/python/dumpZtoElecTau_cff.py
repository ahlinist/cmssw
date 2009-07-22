import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# Print-out cut-flow information for Z --> e + tau-jet channel
#--------------------------------------------------------------------------------

dumpZtoElecTau = cms.EDAnalyzer("DQMDumpFilterStatisticsTables",
    dqmDirectories = cms.PSet(
        Ztautau = cms.string('Ztautau/zElecTauAnalyzer/FilterStatistics/'),
        #Zee = cms.string('Zee/zElecTauAnalyzer/FilterStatistics/'),
        #ZtautauPlusJets = cms.string('ZtautauPlusJets/zElecTauAnalyzer/FilterStatistics/'),
        ZeePlusJets = cms.string('ZeePlusJets/zElecTauAnalyzer/FilterStatistics/'),
        WplusJets = cms.string('WplusJets/zElecTauAnalyzer/FilterStatistics/'),
        TTplusJets = cms.string('TTplusJets/zElecTauAnalyzer/FilterStatistics/'),
        QCD = cms.string('qcdSum/zElecTauAnalyzer/FilterStatistics/'),
        gammaPlusJetsSum = cms.string('gammaPlusJetsSum/zElecTauAnalyzer/FilterStatistics/')
    ),
    columnsSummaryTable = cms.vstring("Passed", "cumul. Efficiency", "Efficiency")
)
 
