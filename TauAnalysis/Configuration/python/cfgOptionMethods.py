import FWCore.ParameterSet.Config as cms
import copy
import string
import pickle

import TauAnalysis.Configuration.tools.factorizationTools as factorizationTools
import TauAnalysis.Configuration.tools.sysUncertaintyTools as sysUncertaintyTools
import TauAnalysis.Configuration.tools.switchToData as switchToData

import PhysicsTools.PatAlgos.tools.helpers as patutils

def _requires(args=[], inputs=[]):
    def decorator(func):
        " Decorator to enforce required arguments and allowed input types "
        def func_with_reqs(process, value, **kwargs):
            for arg in args:
                if arg not in kwargs:
                    raise ValueError(
                        "Function %s has required argument %s" % 
                        (func.__name__, arg))
            if inputs and value not in inputs:
                raise ValueError(
                    "Bad input %s to %s, allowed inputs: %s" % 
                    (func.__name__, value, inputs))
            return func(process, value, **kwargs)
        return func_with_reqs
    return decorator

def _setGlobalTag(process, tag, **kwargs):
    " Set conditions global tag "
    process.GlobalTag.globaltag = tag

@_requires(args=['channel'])
def _setGenPhaseSpaceCut(process, value, **kwargs):
    " Set the generator level phase space cut "
    analyzer_name = "analyze%sEvents" % kwargs['channel']
    if 'suffix' in kwargs:
        analyzer_name += "_"
        analyzer_name += kwargs['suffix']
    module = getattr(process, analyzer_name)
    module.filters[0] = cms.PSet(
        pluginName = cms.string('genPhaseSpaceCut'),
        pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
        src = cms.InputTag('genPhaseSpaceEventInfo'),
        cut = cms.string(value)
    )

@_requires(inputs=[True, False])
def _setIsBatchMode(process, set, **kwargs):
    " Set batchMode option for running "
    if set:
        setattr(process, 'isBatchMode', cms.PSet())
    else:
        del process.isBatchMode

def _setMaxEvents(process, max_events, **kwargs):
    " Set max events to process "
    process.maxEvents.input = cms.untracked.int32(max_events)

def _setSourceFiles(process, fileNames, **kwargs):
    " Set source files to analyze "
    process.source.fileNames = fileNames

@_requires(args=['channel', 'sample', 'id'])
def _setPlotsOutputFileName(process, filename, **kwargs):
    ''' Set output file name for plots 

    Plots will be suffixed with _channel_sample_id.root
    '''
    module_name = "save%sPlots" % kwargs['channel']
    filename += "_%s_%s_%s.root" % (
        kwargs['channel'], kwargs['sample'], kwargs['id'])
    getattr(process, module_name).outputFileName = filename

@_requires(inputs=[True, False])
def _setEventDump(process, enable, **kwargs):
    " Enable/disable event dump "
    if not enable:
        setattr(process, 'disableEventDump', cms.PSet())
    else:
        del process.disableEventDump

@_requires(args=['channel'])
def _setEnableFactorization(process, enable, **kwargs):
    channel = kwargs['channel']
    if enable:
        print "Enabling factorization for %s" % channel
        enabler = getattr(factorizationTools, "enableFactorization_run%s" % channel)
        enabler(process)

@_requires(args=['channel'])
def _setEnableSystematics(process, enable, **kwargs):
    channel = kwargs['channel']
    if enable:
        print "Enabling systematics"
        enabler = getattr(sysUncertaintyTools, "enableSysUncertainties_run%s" % channel)
        enabler(process)
    else:
        print "Disabling systematics"
        disabler = getattr(sysUncertaintyTools, "disableSysUncertainties_run%s" % channel)
        disabler(process)

@_requires(args=['channel'], inputs=['RECO/AOD', 'PATTuple']) 
def _setInputFileType(process, filetype, **kwargs):
    # when running over RECO samples, produce PAT-tuple
    if filetype == 'RECO/AOD':
        patTupleProducerSpecific = getattr(
            process, "producePatTuple%sSpecific" % kwargs['channel'])
        process.p.replace(patTupleProducerSpecific, process.producePatTupleAll)

@_requires(inputs=['Data', 'smMC', 'smSumMC', 'bsmMC',]) 
def _setIsData(process, type, **kwargs):
    if type.lower().find('mc') == -1:
        switchToData.switchToData(process)

@_requires(args=['channel']) 
def _setTriggerProcess(process, triggerTag, **kwargs):
    # Set the input tag for the HLT
    channel = kwargs['channel']
    sequences_to_modify = [ seq % channel for seq in [ 
        'producePatTuple%sSpecific', 'select%sEvents', 'analyze%sEvents'] ]
    for sequence_name in sequences_to_modify:
        print "o Resetting HLT input tag for sequence:", sequence_name
        sequence = getattr(process, sequence_name)
        patutils.massSearchReplaceAnyInputTag(
            sequence, cms.InputTag("TriggerResults", "", "HLT"), triggerTag)
    process.patTrigger.processName = triggerTag.getProcessName()
    process.patTriggerEvent.processName = triggerTag.getProcessName()


# Map the above methods to user-friendly names
_METHOD_MAP = {
    'globalTag' : _setGlobalTag,
    'genPhaseSpaceCut' : _setGenPhaseSpaceCut,
    'isBatchMode' : _setIsBatchMode,
    'maxEvents' : _setMaxEvents,
    'plotsOutputFileName' : _setPlotsOutputFileName,
    'eventDump' : _setEventDump,
    'enableFactorization' : _setEnableFactorization,
    'enableSysUncertainties' : _setEnableSystematics,
    'inputFileType' : _setInputFileType,
    'type' : _setIsData,
    'hlt' : _setTriggerProcess,
}

def applyProcessOptions(process, jobInfo, options):
    ''' Apply options to a cms.Process

    Options should be specified as a list of tuples mapping
    options to their desired values.  The list of options
    is enumerated in the _METHOD_MAP dictionary.  The jobInfo parameter
    specifies information about the current channel and sample in a dict format. 

    Example of specifiying 1000 events, and a special name for the output plots:

        applyProcessOptions(process, 
            {'channel' : 'ZtoMuTau', 'sample' : 'WplusJets'},
            [ ('maxEvents', 1000), ('plotsOutputFileName', 'myPlots') ])

    You can specify optional arguments to an option setter by passing a dictionary
    as the third element of the tuple.
    '''

    for option_tuple in options:
        # Unpack tuple
        option = option_tuple[0]
        value = option_tuple[1]
        extra_options = {}
        if len(option_tuple) > 2:
            extra_options = option_tuple[2]

        if option not in _METHOD_MAP:
            raise ValueError(
                "Option: %s unrecognized! Available options: %s" % 
                (option, _METHOD_MAP.keys()))
        # Call method
        optionsForMethod = copy.copy(jobInfo)
        optionsForMethod.update(extra_options)
        print "Setting option %s to %s - extras: %s" % (
            option, value, extra_options)
        _METHOD_MAP[option](process, value, **optionsForMethod)

def copyCfgFileAndApplyOptions(inputFile, outputFile, jobInfo, jobOptions):
    # Convert the option objects to pickle strings
    substitutions = {
        'jobInfoPickle' : pickle.dumps(jobInfo),
        'jobOptionsPickle' : pickle.dumps(jobOptions)
    }
    modifier = string.Template('''
import pickle
from TauAnalysis.Configuration.cfgOptionMethods import applyProcessOptions
_JOB_INFO = pickle.loads("""$jobInfoPickle""")
_JOB_OPTIONS = pickle.loads("""$jobOptionsPickle""")
applyProcessOptions(process, _JOB_INFO, _JOB_OPTIONS)
    ''')
    # Compute a bit of modifer we can tack on to the end of the CFG.py file
    appendage = modifier.substitute(substitutions)
    input = open(inputFile, 'r')
    output = open(outputFile, 'w')

    # Copy the input file to the output file
    output.write(input.read())
    # Add our modifiers at the end
    output.write(appendage)
    input.close()
    output.close()

if __name__=="__main__":
    copyCfgFileAndApplyOptions(
        "/afs/cern.ch/user/f/friis/scratch0/HiggsAnalysis/src/TauAnalysis/Configuration/test/runAHtoMuTau_cfg.py",
        "/tmp/friis/mycopy.cfg",
        {'channel' : 'AHtoMuTau',
         'sample' : 'WplusJets' },
        [ ('maxEvents', 1000) ]
    )



    



    


