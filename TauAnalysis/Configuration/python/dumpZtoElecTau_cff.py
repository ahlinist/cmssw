import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# Print-out cut-flow information for Z --> e + tau-jet channel
#--------------------------------------------------------------------------------

dumpZtoElecTau = cms.EDAnalyzer("DQMDumpFilterStatisticsTables",
    dqmDirectories = cms.PSet(
        Data = cms.string('summed/harvested/Data/zElecTauAnalyzer/FilterStatistics/'),
        Ztautau = cms.string('summed/harvested/Ztautau/zElecTauAnalyzer/FilterStatistics/'),
        Zee = cms.string('summed/harvested/Zee/zElecTauAnalyzer/FilterStatistics/'),
        WplusJets = cms.string('summed/harvested/wPlusJetsSum/zElecTauAnalyzer/FilterStatistics/'),
		#TTplusJets = cms.string('summed/harvested/TTplusJets/zElecTauAnalyzer/FilterStatistics/'),
        QCD_EMenriched = cms.string('summed/harvested/qcdEMenrichedSum/zElecTauAnalyzer/FilterStatistics/'),
        QCD_BCtoE = cms.string('summed/harvested/qcdBCtoESum/zElecTauAnalyzer/FilterStatistics/'),
		PhotonPlusJets = cms.string('summed/harvested/gammaPlusJetsSum/zElecTauAnalyzer/FilterStatistics/')
    ),
    columnsSummaryTable = cms.vstring("Passed", "cumul. Efficiency", "margin. Efficiency")
)
 
