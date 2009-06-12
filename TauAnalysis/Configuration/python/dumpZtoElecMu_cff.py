import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# Print-out cut-flow information for Z --> e + mu channel
#--------------------------------------------------------------------------------

dumpZtoElecMu = cms.EDAnalyzer("DQMDumpFilterStatisticsTables",
    dqmDirectories = cms.PSet(
        #Ztautau = cms.string('Ztautau/zElecMuAnalyzer/FilterStatistics'),
        #Zmumu = cms.string('Zmumu/zElecMuAnalyzer/FilterStatistics/'),
        ZtautauPlusJets = cms.string('ZtautauPlusJets/zElecMuAnalyzer/FilterStatistics'),
        ZmumuPlusJets = cms.string('ZmumuPlusJets/zElecMuAnalyzer/FilterStatistics/'),
        ZeePlusJets = cms.string('ZeePlusJets/zElecMuAnalyzer/FilterStatistics/'),
        WplusJets = cms.string('WplusJets/zElecMuAnalyzer/FilterStatistics/'),
        TTplusJets = cms.string('TTplusJets/zElecMuAnalyzer/FilterStatistics/'),
        QCD = cms.string('qcdSum/zElecMuAnalyzer/FilterStatistics/')
    ),
    columnsSummaryTable = cms.vstring("Passed", "cumul. Efficiency", "Efficiency")
)
 
