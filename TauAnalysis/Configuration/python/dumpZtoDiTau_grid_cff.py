import FWCore.ParameterSet.Config as cms

import TauAnalysis.Configuration.recoSampleDefinitionsZtoDiTau_7TeV_grid_cfi as samples

#--------------------------------------------------------------------------------
# Print-out cut-flow information for Z --> tau-jet + tau-jet channel
#--------------------------------------------------------------------------------

def sample_dqm_name(sample):
    " Retrieve the containing DQM folder for a given sample "
    sample_info = samples.ALL_SAMPLES[sample]
    if 'factorize' in sample_info and sample_info['factorize']:
        return "%s_factorized" % sample
    else:
        return sample

dumpZtoDiTau = cms.EDAnalyzer("DQMDumpFilterStatisticsTables",
    dqmDirectories = cms.PSet(
        **dict(
            (sample, cms.string(
                '/harvested/%s/zDiTauAnalyzer/FilterStatistics' 
                % sample_dqm_name(sample))
            ) for sample in samples.SAMPLES_TO_PRINT)
    ), 
    columnsSummaryTable = cms.vstring("Passed", "cumul. Efficiency", "margin. Efficiency", "indiv. Efficiency")
)


