import FWCore.ParameterSet.Config as cms

import copy

from Configuration.PyReleaseValidation.autoCond import autoCond
from TauAnalysis.Configuration.submitToGrid2 import submitToGrid

_PLOT_FILES_PREFIX = 'plots'

_JOB_OPTIONS_DEFAULTS = [
    ('maxEvents', -1),
    ('inputFileType', 'RECO/AOD'),
    ('isBatchMode', True),
    ('plotsOutputFileName', _PLOT_FILES_PREFIX)
]

def _get_conditions(globalTag):
    """ Retrieve appropriate conditions

    Conditions can be automatically retrieved using the autoCond
    utility if the input is in [mc, startup, com10, craft09, etc]
    """
    if globalTag in autoCond:
        return autoCond[globalTag]
    else:
        return globalTag

def _number_of_jobs(sample_info, preferred=10000, max_jobs=300):
    """
    Try to run on <preferred> events per job, unless it would
    create to many jobs.
    """
    # Check if explicitly specified
    if 'number_of_jobs' in sample_info:
        return sample_info['number_of_jobs']
    # Otherwise try and run 10k events per job, with a max of 300 jobs
    to_process = sample_info['events_processed']*sample_info['skim_eff']
    desired_njobs = to_process/preferred
    if desired_njobs < 0:
        return max_jobs
    output = (desired_njobs > max_jobs) and max_jobs or desired_njobs
    return int(output)

def submitAnalysisToGrid(configFile = None, channel = None, samples = None, outputFilePath = None, jobId = None,
                         samplesToAnalyze = None, samplesToSkip = None, disableFactorization = False, disableSysUncertainties = False):
    """
    Submit analysis job (event selection, filling of histogram)
    via crab
    """

    # check that configFile, channel, samples, outputFilePath and jobId
    # parameters are defined and non-empty
    if configFile is None:
        raise ValueError("Undefined configFile Parameter !!")
    if channel is None:
        raise ValueError("Undefined channel Parameter !!")
    if samples is None:
        raise ValueError("Undefined samples Parameter !!")
    if outputFilePath is None:
        raise ValueError("Undefined outputFilePath Parameter !!")
    if jobId is None:
        raise ValueError("Undefined jobId Parameter !!")

    # Loop over the samples to be analyzed
    for sample in samples['SAMPLES_TO_ANALYZE']:
        # Skip submitting crab job in case
        #  o list of samples for which crab jobs are to be submitted has been explicitely specified
        #  o sample has explicitely been requested to be skipped
        if samplesToAnalyze:
            if sample not in samplesToAnalyze:
                print "Skipping", sample
                continue
        if samplesToSkip:
            if sample in samplesToSkip:
                print "Skipping", sample
                continue
        print "Submitting ", sample

        sample_info = samples['RECO_SAMPLES'][sample]
        # Make job info
        jobInfo = {
            'channel' : channel,
            'sample' : sample,
            'id' : jobId
        }
        jobOptions = copy.copy(_JOB_OPTIONS_DEFAULTS)
        # Get the type and genPhase space cut
        jobOptions.append(('type', sample_info['type']))
        # For the genphase space cut, we need to do it for the two different
        jobOptions.append(('genPhaseSpaceCut', sample_info['genPhaseSpaceCut']))

        # Check if we need to change the HLT tag
        if 'hlt' in sample_info:
            jobOptions.append(('hlt', sample_info['hlt']))

        # Update our HLT selection
        jobOptions.append(('hlt_paths', sample_info['hlt_paths']))

        # Get the appropriate GlobalTag
        jobOptions.append(('globalTag', _get_conditions(sample_info['conditions'])))

        # Enable factorization if necessary
        if not disableFactorization:
            jobOptions.append(('enableFactorization', sample_info['factorize']))

        # Apply Z-recoil correction to MEt if requested
        jobOptions.append(('applyZrecoilCorrection', sample_info['applyZrecoilCorrection']))

        # This must be done after the factorization step ?
        jobOptions.append(('enableSysUncertainties', sample_info['enableSysUncertainties'] and not disableSysUncertainties))

        # Build crab options
        crabOptions = {
            'number_of_jobs' : _number_of_jobs(sample_info),
            'datasetpath' : sample_info['datasetpath'],
            'dbs_url' : sample_info['dbs_url'],
            'user_remote_dir' : outputFilePath,
            'output_file' : "%s_%s_%s_%s.root" % (
                _PLOT_FILES_PREFIX, jobInfo['channel'],
                jobInfo['sample'], jobInfo['id']),
            # Default MC info
            'split_type' : (sample_info['type'] == 'Data') and 'lumis' or 'events',
            'lumi_mask' : sample_info['lumi_mask'],
            'runselection' : sample_info['runselection'],
            'SE_white_list' : sample_info['SE_white_list'],
            'SE_black_list' : sample_info['SE_black_list']
        }

        submitToGrid(configFile, jobInfo, jobOptions, crabOptions, create=True, submit=True)
        ##submitToGrid(configFile, jobInfo, jobOptions, crabOptions, create=False, submit=False) # CV: only for testing

