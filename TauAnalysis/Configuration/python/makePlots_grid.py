import FWCore.ParameterSet.Config as cms

import os

def makePlots(process, channel = None, samples = None, inputFilePath = None, jobId = None,
              enableFactorizationFunction = None, dumpDQMStore = False):

    # check that channel, samples, inputFilePath and jobId
    # parameters are defined and non-empty
    if channel is None:
        raise ValueError("Undefined channel Parameter !!")
    if samples is None:
        raise ValueError("Undefined samples Parameter !!")
    if inputFilePath is None:
        raise ValueError("Undefined inputFilePath Parameter !!")
    if jobId is None:
        raise ValueError("Undefined jobId Parameter !!")

    print(" inputFilePath = " + inputFilePath)

    outputFilePath = inputFilePath + '/' + jobId
    outputFilePath = outputFilePath.replace('//', '/')
    print(" outputFilePath = " + outputFilePath)

    if not os.path.exists(outputFilePath):        
        os.mkdir(outputFilePath)

    sample_mapper = lambda sample : "harvested_%s_%s_%s.root" % (channel, sample, jobId)

    process.DQMStore = cms.Service("DQMStore")

    process.maxEvents = cms.untracked.PSet(            
        input = cms.untracked.int32(0)         
    )

    process.source = cms.Source("EmptySource")

    # define directory from which .root files containing histograms 
    # for individual processes get loaded
    dqmFileLoaderModuleName = "load%sSamples" % channel
    dqmFileLoaderModule = getattr(process, dqmFileLoaderModuleName)    
    dqmFileLoaderModule.inputFilePath = cms.string(inputFilePath)

    # update names of input files
    for sample in samples['FLATTENED_SAMPLES_TO_PLOT']:            
        getattr(dqmFileLoaderModule, sample).inputFileNames = cms.vstring(
            sample_mapper(sample)
        )

    samplesToFactorize = [sample for sample in samples['FLATTENED_SAMPLES_TO_PLOT']
                          if samples['ALL_SAMPLES'][sample].get('factorize', False)]

    relevantMergedSamples = [sample for sample, sample_info in samples['MERGE_SAMPLES'].iteritems() if 
                             [subsample for subsample in sample_info['samples'] 
                              if subsample in samplesToFactorize]]

    print "Factorizing", samplesToFactorize
    print "Updating", relevantMergedSamples

    if enableFactorizationFunction is not None:
        enableFactorizationFunction(
            process,
            samplesToFactorize = samplesToFactorize,
            relevantMergedSamples = relevantMergedSamples,
            mergedToRecoSampleDict = samples['MERGE_SAMPLES']
        )

    dqmSimpleFileSaverModuleName = "save%s" % channel
    dqmSimpleFileSaverModule = getattr(process, dqmSimpleFileSaverModuleName)
    outputFileName = "plots%s_all.root" % channel
    dqmSimpleFileSaverModule.outputFileName = cms.string(outputFilePath + '/' + outputFileName)    

    dqmDumpFilterStatisticsModuleName = "dump%s" % channel
    dqmDumpFilterStatisticsModule = getattr(process, dqmDumpFilterStatisticsModuleName)

    # update label indicating integrated luminosity of analyzed dataset in plots
    dqmHistPlotterName = "plot%s" % channel
    intLuminosityLabel = cms.vstring(
        '%0.1fpb^{-1}' % samples['TARGET_LUMI'],
        '#sqrt{s}=7TeV'
    )
    if isinstance(getattr(process, dqmHistPlotterName), cms.Sequence):
        dqmHistPlotterSequence = getattr(process, dqmHistPlotterName)

        class SetIntLuminosityLabelVisitor(object):
            def __init__(self, intLuminosityLabel):
                self._intLuminosityLabel = intLuminosityLabel
            def enter(self,visitee):
                visitee.labels.mcNormScale.text = intLuminosityLabel
            def leave(self,visitee):
                pass
        
        dqmHistPlotterSequence.visit(SetIntLuminosityLabelVisitor(intLuminosityLabel))
    else:        
        dqmHistPlotterModule = getattr(process, dqmHistPlotterName)   
        dqmHistPlotterModule.labels.mcNormScale.text = intLuminosityLabel

    makePlotSequenceName = "make%sPlots" % channel
    makePlotSequence = cms.Sequence(dqmFileLoaderModule)
    if dumpDQMStore:
        process.dumpDQMStore = cms.EDAnalyzer("DQMStoreDump")
        makePlotSequence._seq = makePlotSequence._seq * process.dumpDQMStore
    makePlotSequence._seq = makePlotSequence._seq * dqmSimpleFileSaverModule
    makePlotSequence._seq = makePlotSequence._seq * dqmDumpFilterStatisticsModule
    makePlotSequence._seq = makePlotSequence._seq * getattr(process, dqmHistPlotterName)
    setattr(process, makePlotSequenceName, makePlotSequence)

    process.p = cms.Path(makePlotSequence)
