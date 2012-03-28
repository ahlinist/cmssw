import FWCore.ParameterSet.Config as cms

import copy
import os

from Configuration.PyReleaseValidation.autoCond import autoCond

from TauAnalysis.Configuration.cfgOptionMethods import copyCfgFileAndApplyOptions

PLOT_FILES_PREFIX = 'plots'

def getNewConfigFileName(configFile = None, cfgdir = None, sample = None,
                         jobId = None, index=None, label = ""):
    # check that configFile, cfgdir, sample and jobId
    # parameters are defined and non-empty
    if configFile is None:
        raise ValueError("Undefined 'configFile' Parameter !!")
    if cfgdir is None:
        raise ValueError("Undefined 'cfgdir' Parameter !!")
    if sample is None:
        raise ValueError("Undefined 'sample' Parameter !!")
    if jobId is None:
        raise ValueError("Undefined 'jobId' Parameter !!")

    workingDirectory = os.getcwd()
    submissionDirectory = os.path.join(workingDirectory, cfgdir)

    # Add an index if desired
    if index is not None:
        jobId = "_".join([jobId, str(index)])
    # Strip off _cfg.py and add sample info
    newConfigFile = configFile.replace(
        '_cfg.py', '_%s_%s%s_cfg.py' % (sample, jobId, label))

    newConfigFilePath = os.path.join(submissionDirectory, newConfigFile)

    return newConfigFilePath

_JOB_OPTIONS_DEFAULTS = [
    ('maxEvents', -1),
    ('skipEvents', 0),
    ('isBatchMode', True),
    ('plotsOutputFileName', PLOT_FILES_PREFIX)
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

def prepareConfigFile(configFile = None, jobInfo = None, newConfigFile = None,
                      sample_infos = None,
                      disableFactorization = False, disableSysUncertainties = False, disableZrecoilCorrections = False,
                      input_files = None, output_file = None,
                      enableEventDumps = False, enableFakeRates = False,
                      processName = None,
                      eventList = None,
                      saveFinalEvents = False,
                      # Add optional suffix to end of skim file name
                      saveFinalEventsFileName = None,
                      changeTauId = None,
                      customizations = [], doApplyOptions = True):
    """
    Create cfg.py file used as input for cmsRun analysis job
    """

    #print("<prepareConfigFile>:")

    # check that configFile, channel, sample and sample_infos
    # parameters are defined and non-empty
    if configFile is None:
        raise ValueError("Undefined 'configFile' Parameter !!")
    if jobInfo is None:
        raise ValueError("Undefined 'jobInfo' Parameter !!")
    if sample_infos is None:
        raise ValueError("Undefined 'sample_infos' Parameter !!")

    sample_info = sample_infos['RECO_SAMPLES'][jobInfo['sample']]
    #print("sample_info:")
    #print(sample_info)

    jobOptions = copy.copy(_JOB_OPTIONS_DEFAULTS)

    # Change the process name if desired.  Used for local running
    jobOptions.append(('processName', processName))

    # Check if we want to disable the duplicate check mode for events.
    # This is needed for the embedded Ztautau sample.
    jobOptions.append(('disableDuplicateCheck', sample_info['disableDuplicateCheck']))

    # Check if we want to set specific input_files and output_files in the produced cfg file
    # or let crab decide which samples to run on, based on the DBS names defined
    # in the recoSampleDefinitions file
    if input_files is not None:
        jobOptions.append(('files', input_files))
    if output_file is not None:
        jobOptions.append(('outputFile', output_file))

    # Set maxEvents and skipEvents parameters
    if 'maxEvents' in sample_info:
        print("--> setting 'maxEvents' to %i" % sample_info['maxEvents'])
	jobOptions.append(('maxEvents', sample_info['maxEvents']))
    if 'skipEvents' in sample_info:
        print("--> setting 'skipEvents' to %i" % sample_info['skipEvents'])
	jobOptions.append(('skipEvents', sample_info['skipEvents']))

    # Get the type and genPhaseSpace cut
    jobOptions.append(('type', sample_info['type']))

    # Check if we want to change the tauId.
    if changeTauId is not None:
        jobOptions.append(('changeTauId', changeTauId))

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

    # Apply Z-recoil correction to MEt if requested
    if not disableZrecoilCorrections:
        jobOptions.append(('applyZrecoilCorrection',
                          sample_info['applyZrecoilCorrection']))

    # Apply muon trigger efficiency correction if requested
    jobOptions.append(('applyMuonTriggerEfficiencyCorrection',
                       sample_info['applyMuonTriggerEfficiencyCorrection']))

    # Apply electron trigger efficiency correction if requested
    jobOptions.append(('applyElectronTriggerEfficiencyCorrection',
                       sample_info['applyElectronTriggerEfficiencyCorrection']))

    # Apply tau + met trigger efficiency correction if requested
    jobOptions.append(('applyTauMetTriggerEfficiencyCorrection',
                       sample_info['applyTauMetTriggerEfficiencyCorrection']))
        
    # Apply electron isolation efficiency correction if requested
    jobOptions.append(('applyElectronIsolationEfficiencyCorrection',
                       sample_info['applyElectronIsolationEfficiencyCorrection']))

    # Apply vertex multiplicity reweighting if requested
    jobOptions.append(('applyVertexMultiplicityReweighting',
                       sample_info['applyVertexMultiplicityReweighting']))

    # Apply vertex multiplicity reweighting if requested
    jobOptions.append(('applyRhoNeutralReweighting',
                       sample_info['applyRhoNeutralReweighting']))

    # CV: enabaling of systematic uncertainties must be done
    #     **after** the factorization step (**not** before)
    jobOptions.append(('enableSysUncertainties',
                       sample_info['enableSysUncertainties']
                       and not disableSysUncertainties))
    
    # disable PFTau sequence if it's already in the event content
    if sample_info.has_key('disablePFTauProduction'):
        jobOptions.append(('disablePFTauProduction',
                       sample_info['disablePFTauProduction']))

    # As the event selectors get reconfigured, do this AFTER enabling the sys
    # uncertainties
    jobOptions.append(('genPhaseSpaceCut', sample_info['genPhaseSpaceCut']))

    jobOptions.append(('genFinalStateFilter', sample_info['genFinalStateFilter']))

    # Enable fake rates if desired - this must be done AFTER factorization and
    # any other modification of the event weights of the analyzers
    if 'enableFakeRates' in sample_info and enableFakeRates:
        jobOptions.append(('enableFakeRates',
                           sample_info['enableFakeRates']))

    jobOptions.append(('eventDump', enableEventDumps))

    # Check if we want to turn off saving of final run/lumi/event numbers
    jobOptions.append(('noRunLumiEventSave', sample_info['noRunLumiEventSave']))

    saveFinalEventsOption = ('saveFinalEvents', saveFinalEvents)
    # Optionally specify the final events name.
    if saveFinalEventsFileName is not None:
        saveFinalEventsOption += ({'filename':saveFinalEventsFileName},)

    jobOptions.append(saveFinalEventsOption)

    # Check if we want to save an ntuple
    if 'saveNtuple' in sample_info:
        jobOptions.append(('saveNtuple', sample_info['saveNtuple']))

    # Check if we want to manually specify the events to run on.
    if eventList is not None:
        if not os.path.exists(eventList):
            raise ValueError("Event list file %s does not exist!" % eventList)
        full_path = os.path.abspath(eventList)
        jobOptions.append(('eventList', eventList))

    # Check if we need to restrict input to AOD event content
    if 'inputFileType' in sample_info:
        jobOptions.append(('inputFileType', sample_info['inputFileType']))

    # Create new config file with specialization options added
    workingDirectory = os.getcwd()
    configFilePath = os.path.join(workingDirectory, configFile)
    if not os.path.exists(configFilePath):
        raise ValueError("Failed to find config file %s in current directory !!" % configFile)
    copyCfgFileAndApplyOptions(configFilePath, newConfigFile, jobInfo, jobOptions, customizations, doApplyOptions)

