import FWCore.ParameterSet.Config as cms

import TauAnalysis.Configuration.recoSampleDefinitionsAHtoMuTau_7TeV_grid_cfi as samples

#--------------------------------------------------------------------------------
# Print-out cut-flow information for for MSSM Higgs analysis in
# A/H --> muon + tau-jet channel
#--------------------------------------------------------------------------------

def sample_dqm_name(sample):
    " Retrieve the containing DQM folder for a given sample "
    sample_info = samples.ALL_SAMPLES[sample]
    if 'factorize' in sample_info and sample_info['factorize']:
        return "%s_factorized" % sample
    else:
        return sample


dumpAHtoMuTau_woBtag = cms.EDAnalyzer(
    "DQMDumpFilterStatisticsTables",
    dqmDirectories = cms.PSet(
        **dict(
            (sample, cms.string(
                '/harvested/%s/ahMuTauAnalyzer_woBtag/FilterStatistics'
                % sample_dqm_name(sample))
            ) for sample in samples.SAMPLES_TO_PRINT)
    ),
    columnsSummaryTable = cms.vstring("Passed", "cumul. Efficiency", "margin. Efficiency", "indiv. Efficiency")
)

dumpAHtoMuTau_wBtag = cms.EDAnalyzer(
    "DQMDumpFilterStatisticsTables",
    dqmDirectories = cms.PSet(
        **dict(
            (sample, cms.string(
                '/harvested/%s/ahMuTauAnalyzer_wBtag/FilterStatistics'
                % sample_dqm_name(sample))
            ) for sample in samples.SAMPLES_TO_PRINT)
    ),
    columnsSummaryTable = cms.vstring("Passed", "cumul. Efficiency", "margin. Efficiency", "indiv. Efficiency")
)

#dumpAHtoMuTau = cms.Sequence(dumpAHtoMuTau_woBtag * dumpAHtoMuTau_wBtag)
dumpAHtoMuTau = cms.Sequence(dumpAHtoMuTau_woBtag)

