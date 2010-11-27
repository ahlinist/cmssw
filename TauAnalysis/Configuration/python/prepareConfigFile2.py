import FWCore.ParameterSet.Config as cms

import copy
import os

from Configuration.PyReleaseValidation.autoCond import autoCond

from TauAnalysis.Configuration.cfgOptionMethods import copyCfgFileAndApplyOptions

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

def prepareConfigFile2(configFile = None, channel = None, sample_infos = None, sample = None,
                       outputFilePath = None, jobId = None,
                       disableFactorization = False, disableSysUncertainties = False,
                       cfgdir = 'crab',
                       inputFileMap = None, outputFileMap = None,
                       enableEventDumps = False, enableFakeRates = False,
                       processName = None,
                       saveFinalEvents = False):
    """
    Create cfg.py file used as input for cmsRun analysis job
    """

    # check that configFile, channel, samples, outputFilePath and jobId
    # parameters are defined and non-empty
    if configFile is None:
        raise ValueError("Undefined configFile Parameter !!")
    if channel is None:
        raise ValueError("Undefined channel Parameter !!")
    if sample_infos is None:
        raise ValueError("Undefined sample_infos Parameter !!")
    if sample is None:
        raise ValueError("Undefined sample Parameter !!")
    if outputFilePath is None:
        raise ValueError("Undefined outputFilePath Parameter !!")
    if jobId is None:
        raise ValueError("Undefined jobId Parameter !!")

    sample_info = sample_infos['RECO_SAMPLES'][sample]
    # Make job info
    jobInfo = {
        'channel' : channel,
        'sample' : sample,
        'id' : jobId
    }
    jobOptions = copy.copy(_JOB_OPTIONS_DEFAULTS)

    # Change the process name if desired.  Used for local running
    jobOptions.append(('processName', processName))

    # Check if we want to disable the duplicate check mode for events.
    # This is needed for the embedded Ztautau sample.
    jobOptions.append(('disableDuplicateCheck', sample_info['disableDuplicateCheck']))

    # Check if we want to use a special file for the produced cfg file
    # File map is a function that takes a sample name and returns a list of
    # files corresponding to that file.  If files is None, no change will be
    # made.
    if inputFileMap is not None:
        input_files = inputFileMap(sample)
        if input_files is None:
            print "Warning: No special input files specified for sample"\
                  "%s, using default" % sample
        else:
            jobOptions.append(('files', input_files))
            
    if outputFileMap is not None:
        output_file = outputFileMap(sample)
        jobOptions.append(('outputFile', output_file))

    # Get the type and genPhaseSpace cut
    jobOptions.append(('type', sample_info['type']))
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
        jobOptions.append(('enableFactorization',
                           sample_info['factorize']))

    # Enable fake rates if desired - this must be done AFTER factorization
    if 'enableFakeRates' in sample_info and enableFakeRates:
        jobOptions.append(('enableFakeRates',
                           sample_info['enableFakeRates']))

    # Apply Z-recoil correction to MEt if requested
    jobOptions.append(('applyZrecoilCorrection',
                       sample_info['applyZrecoilCorrection']))

    # Apply muon trigger efficiency correction if requested
    jobOptions.append(('applyMuonTriggerEfficiencyCorrection',
                       sample_info['applyMuonTriggerEfficiencyCorrection']))

    # Apply vertex multiplicity reweighting if requested
    jobOptions.append(('applyVertexMultiplicityReweighting',
                       sample_info['applyVertexMultiplicityReweighting']))

    # This must be done after the factorization step ?
    jobOptions.append(('enableSysUncertainties',
                       sample_info['enableSysUncertainties']
                       and not disableSysUncertainties))

    jobOptions.append(('eventDump', enableEventDumps))

    jobOptions.append(('saveFinalEvents', saveFinalEvents))

    # Always include the plot files
    output_files = ["%s_%s_%s_%s.root" % (
        _PLOT_FILES_PREFIX, jobInfo['channel'],
        jobInfo['sample'], jobInfo['id'])]

    # Add our final event skim as well
    if saveFinalEvents:
        output_files.append("final_events_%s_%s_%s.root" % (
            jobInfo['channel'], jobInfo['sample'], jobInfo['id']))

    # Create config file
    workingDirectory = os.getcwd()
    submissionDirectory = os.path.join(workingDirectory, cfgdir)
    configFilePath = os.path.join(workingDirectory, configFile)
    if not os.path.exists(configFilePath):
        raise ValueError("Can't find config file %s in current directory !!" % configFile)

    # Strip off _cfg.py and add sample info
    configFileName = configFile.replace('_cfg.py', '_%s_%s' % (jobInfo['sample'], jobInfo['id']))

    # New name of config file
    newConfigFile =  configFileName + '@Grid_cfg.py'
    newConfigFilePath = os.path.join(submissionDirectory, newConfigFile)
        
    # Copy the config file and add our specialization options
    copyCfgFileAndApplyOptions(configFilePath, newConfigFilePath, jobInfo, jobOptions)

    # Return name of config file
    return newConfigFilePath
        
