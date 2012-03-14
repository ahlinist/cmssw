import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.Configuration.recoSampleDefinitionsAHtoElecTau_grid_cfi import ALL_SAMPLES as all_samples
from TauAnalysis.Configuration.recoSampleDefinitionsAHtoElecTau_grid_cfi import SAMPLES_TO_PRINT as samples_to_print

#--------------------------------------------------------------------------------
# Print-out cut-flow information for Z --> muon + tau-jet channel
#--------------------------------------------------------------------------------

def sample_dqm_name(sample):
    " Retrieve the containing DQM folder for a given sample "
    sample_info = all_samples[sample]
    if 'factorize' in sample_info and sample_info['factorize']:
        return "%s_factorized" % sample
    else:
        return sample

dumpAHtoElecTauOS_woBtag = cms.EDAnalyzer("DQMDumpFilterStatisticsTables",
        dqmDirectories = cms.PSet(
            **dict(
                (sample, cms.string(
                    '/harvested/%s/ahElecTauAnalyzerOS_woBtag/FilterStatistics' 
                    % sample_dqm_name(sample))
                    ) for sample in samples_to_print)
                ), 
        columnsSummaryTable = cms.vstring("Passed", "cumul. Efficiency", "margin. Efficiency")
)
dumpAHtoElecTauOS_wBtag = cms.EDAnalyzer("DQMDumpFilterStatisticsTables",
        dqmDirectories = cms.PSet(
            **dict(
                (sample, cms.string(
                    '/harvested/%s/ahElecTauAnalyzerOS_wBtag/FilterStatistics' 
                    % sample_dqm_name(sample))
                    ) for sample in samples_to_print)
                ), 
        columnsSummaryTable = cms.vstring("Passed", "cumul. Efficiency", "margin. Efficiency")
)
dumpAHtoElecTauOS_VBF = cms.EDAnalyzer("DQMDumpFilterStatisticsTables",
        dqmDirectories = cms.PSet(
            **dict(
                (sample, cms.string(
                    '/harvested/%s/ahElecTauAnalyzerOS_VBF/FilterStatistics' 
                    % sample_dqm_name(sample))
                    ) for sample in samples_to_print)
                ), 
        columnsSummaryTable = cms.vstring("Passed", "cumul. Efficiency", "margin. Efficiency")
)
dumpAHtoElecTauOS_ZeroOneJets = cms.EDAnalyzer("DQMDumpFilterStatisticsTables",
        dqmDirectories = cms.PSet(
            **dict(
                (sample, cms.string(
                    '/harvested/%s/ahElecTauAnalyzerOS_ZeroOneJets/FilterStatistics' 
                    % sample_dqm_name(sample))
                    ) for sample in samples_to_print)
                ), 
        columnsSummaryTable = cms.vstring("Passed", "cumul. Efficiency", "margin. Efficiency")
)
dumpAHtoElecTauOS_Boosted = cms.EDAnalyzer("DQMDumpFilterStatisticsTables",
        dqmDirectories = cms.PSet(
            **dict(
                (sample, cms.string(
                    '/harvested/%s/ahElecTauAnalyzerOS_Boosted/FilterStatistics' 
                    % sample_dqm_name(sample))
                    ) for sample in samples_to_print)
                ), 
        columnsSummaryTable = cms.vstring("Passed", "cumul. Efficiency", "margin. Efficiency")
)
