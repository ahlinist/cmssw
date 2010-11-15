import FWCore.ParameterSet.Config as cms

import copy
import os

import TauAnalysis.DQMTools.plotterStyleDefinitions_cfi as styles
from TauAnalysis.Configuration.userRegistry import userSettings

dqmHistPlotter_template = cms.EDAnalyzer("DQMHistPlotter",
    xAxes = cms.PSet(
        Pt = copy.deepcopy(styles.xAxis_pt),
        Eta = copy.deepcopy(styles.xAxis_eta),
        Phi = copy.deepcopy(styles.xAxis_phi),
        IPxy = copy.deepcopy(styles.xAxis_ipXY),
        IPz = copy.deepcopy(styles.xAxis_ipZ),
        dR = copy.deepcopy(styles.xAxis_dR),
        dPhi = copy.deepcopy(styles.xAxis_dPhi),
        Chi2 = copy.deepcopy(styles.xAxis_Chi2),
        prob = copy.deepcopy(styles.xAxis_prob),
        posZ = copy.deepcopy(styles.xAxis_posZ),
        Mt = copy.deepcopy(styles.xAxis_transMass),
        Mass = copy.deepcopy(styles.xAxis_mass),
        N = copy.deepcopy(styles.xAxis_num),
        PdgId = copy.deepcopy(styles.xAxis_pdgId),
        GeV = copy.deepcopy(styles.xAxis_GeV),
        unlabeled = copy.deepcopy(styles.xAxis_unlabeled)
    ),

    yAxes = cms.PSet(
        numEntries_linear = copy.deepcopy(styles.yAxis_numEntries_linear),
        numEntries_log = copy.deepcopy(styles.yAxis_numEntries_log)
    ),

    legends = cms.PSet(
        regular = copy.deepcopy(styles.legend_regular)
    ),

    labels = cms.PSet(
        mcNormScale = copy.deepcopy(styles.label_mcNormScale)
    ),

    drawOptionSets = cms.PSet(),

    drawJobs = cms.PSet(),

    canvasSizeX = cms.int32(userSettings[os.environ['LOGNAME']]['global']['drawOptions']['canvasSizeX']),
    canvasSizeY = cms.int32(userSettings[os.environ['LOGNAME']]['global']['drawOptions']['canvasSizeY']),

    outputFilePath = cms.string('./plots/')
)

def makePlots(process, channel = None, samples = None, inputFilePath = None, jobId = None,
              analyzer_drawJobConfigurator_indOutputFileName_sets = None,
              drawJobTemplate = None,
              enableFactorizationFunction = None,
              dqmDirectoryFilterStatistics = None, dumpDQMStore = False):

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
    if analyzer_drawJobConfigurator_indOutputFileName_sets is None:
        raise ValueError("Undefined analyzer_drawJobConfigurator_indOutputFileName_sets Parameter !!")
    if drawJobTemplate is None:
        for analyzer_drawJobConfigurator_indOutputFileName_set in analyzer_drawJobConfigurator_indOutputFileName_sets:
            if len(analyzer_drawJobConfigurator_indOutputFileName_set) > 1:
                raise ValueError("Undefined drawJobTemplate Parameter !!")
    if dqmDirectoryFilterStatistics is None:
        raise ValueError("Undefined dqmDirectoryFilterStatistics Parameter !!")

    print(" inputFilePath = " + inputFilePath)

    outputFilePath = inputFilePath
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

    # Define the file loader jobs for each of our analyzed jobs
    dqmFileLoaderJobs = {}
    for sample in samples['FLATTENED_SAMPLES_TO_PLOT']:
        sample_info = samples['RECO_SAMPLES'][sample]
        # Build DQMFileLoader PSet for this sample
        sample_pset = cms.PSet(
            inputFileNames = cms.vstring(''),
            dqmDirectory_store = cms.string('/harvested/%s' % sample),
        )
        # Auto scale MC samples
        if sample_info['type'].lower().find('mc') != -1:
            sample_pset.autoscale = cms.bool(True)
            sample_pset.totalExpectedEventsBeforeSkim = cms.uint32(sample_info['events_processed'])
            sample_pset.skimEfficiency = cms.double(sample_info['skim_eff'])
            sample_pset.xSection = cms.double(sample_info['x_sec'])
            sample_pset.targetIntLumi = cms.double(samples['TARGET_LUMI'])

            # Define the filter to take the processed events from
            sample_pset.filterToUse = cms.string("genPhaseSpaceCut")
            sample_pset.filterStatisticsLocation = cms.string(dqmDirectoryFilterStatistics['factorizationDisabled'])
        else:
            # For data, don't apply any scaling
            sample_pset.scaleFactor = cms.double(1.0)
        # If the sample is factorized, we need to change the location of the filter
        # statistics information
        if sample_info['factorize']:
            sample_pset.filterStatisticsLocation = cms.string(dqmDirectoryFilterStatistics['factorizationEnabled'])

        # Add to our sample dictionary
        dqmFileLoaderJobs[sample] = sample_pset

    # configure DQMFileLoader module
    dqmFileLoaderModuleName = "load%sSamples" % channel
    dqmFileLoaderModule = cms.EDAnalyzer("DQMFileLoader",
        **dqmFileLoaderJobs)
    dqmFileLoaderModule.inputFilePath = cms.string(inputFilePath)
    for sample in samples['FLATTENED_SAMPLES_TO_PLOT']:
        getattr(dqmFileLoaderModule, sample).inputFileNames = cms.vstring(
            sample_mapper(sample)
        )
    setattr(process, dqmFileLoaderModuleName, dqmFileLoaderModule)

    # Loop over the different merged samples we have defined and
    # merge them using the HistAdder
    dqmHistAdderJobs = {}
    for merge_name in samples['MERGE_SAMPLES'].keys():
        # Don't merge if we don't care about it
        if merge_name not in samples['SAMPLES_TO_PLOT']:
            if merge_name not in samples['SAMPLES_TO_PRINT']:
                continue
        merge_info = samples['MERGE_SAMPLES'][merge_name]
        for analyzer_drawJobConfigurator_indOutputFileName_set in analyzer_drawJobConfigurator_indOutputFileName_sets:
            analyzer = analyzer_drawJobConfigurator_indOutputFileName_set[0]
            # Build the new PSet
            new_pset = cms.PSet(
                dqmDirectories_input = cms.vstring([
                    '/harvested/%s/%s' % (sample, analyzer)
                    for sample in merge_info['samples']
                ]),
                dqmDirectory_output = cms.string(
                    '/harvested/%s/%s'%(merge_name, analyzer)),
            )
            # Add the new PSet to our list
            dqmHistAdderJobs["merge_%s_%s" % (merge_name, analyzer)] = new_pset

    # configure DQMHistAdder module
    dqmHistAdderModuleName = "mergeSamples%s" % channel
    dqmHistAdderModule = cms.EDAnalyzer("DQMHistAdder",
        **dqmHistAdderJobs
    )
    setattr(process, dqmHistAdderModuleName, dqmHistAdderModule)

    dqmLoadAndFactorizeSequenceName = "loadAndFactorize%sSamples" % channel
    dqmLoadAndFactorizeSequence = cms.Sequence(dqmFileLoaderModule)
    setattr(process, dqmLoadAndFactorizeSequenceName, dqmLoadAndFactorizeSequence)

    dqmLoadFactorizeAndMergeSequenceName = "load%s" % channel
    dqmLoadFactorizeAndMergeSequence = cms.Sequence(
        dqmLoadAndFactorizeSequence * dqmHistAdderModule)
    setattr(process, dqmLoadFactorizeAndMergeSequenceName, dqmLoadFactorizeAndMergeSequence)

    # Define plot processes and styles for each sample
    # These get passed as kwargs to the DQMHistPlotter
    processesForPlots = dict(
        (sampleName, cms.PSet(
            dqmDirectory = cms.string('/harvested/%s' % sampleName),
            legendEntry=cms.string(samples['ALL_SAMPLES'][sampleName]['legendEntry']),
            type = cms.string(samples['ALL_SAMPLES'][sampleName]['type']),
        )) for sampleName in samples['SAMPLES_TO_PLOT']
    )

    drawOptionSets = dict(
        (sampleName, samples['ALL_SAMPLES'][sampleName]['drawOption'])
        for sampleName in samples['SAMPLES_TO_PLOT'])

    # Define draw job configurator for our smaples
    if drawJobTemplate is not None:
        drawJobTemplate = copy.deepcopy(drawJobTemplate)
        drawJobTemplate.plots.processes = cms.vstring(samples['SAMPLES_TO_PLOT'])
        # Stack all non-BSM sample
        drawJobTemplate.stack = cms.vstring([
            sample for sample in samples['SAMPLES_TO_PLOT']
            if samples['ALL_SAMPLES'][sample]['type'].find('bsm') == -1 and
            samples['ALL_SAMPLES'][sample]['type'].find('Data') == -1
        ])

    dqmHistPlotterSequenceName = "plot%sSequence" % channel
    dqmHistPlotterSequence = None

    # configure DQMHistPlotter modules
    for analyzer_drawJobConfigurator_indOutputFileName_set in analyzer_drawJobConfigurator_indOutputFileName_sets:
        if len(analyzer_drawJobConfigurator_indOutputFileName_set) == 3:
            print("configuring DQMHistPlotter...")
            analyzer = analyzer_drawJobConfigurator_indOutputFileName_set[0]

            drawJobTemplate_log = copy.deepcopy(drawJobTemplate)
            drawJobTemplate_log.yAxis = cms.string('numEntries_log')
            drawJobConfigurator_log = copy.deepcopy(analyzer_drawJobConfigurator_indOutputFileName_set[1])
            drawJobConfigurator_log.setTemplate(drawJobTemplate_log)

            drawJobTemplate_linear = copy.deepcopy(drawJobTemplate)
            drawJobTemplate_linear.yAxis = cms.string('numEntries_linear')
            drawJobConfigurator_linear = copy.deepcopy(analyzer_drawJobConfigurator_indOutputFileName_set[1])
            drawJobConfigurator_linear.setTemplate(drawJobTemplate_linear)

            dqmHistPlotterModuleName = None
            if analyzer.find("_") != -1:
                dqmHistPlotterModuleName = "plot%s%s" % (channel, analyzer[analyzer.rfind("_"):])
            else:
                dqmHistPlotterModuleName = "plot%s" % channel
            print("--> configuring DQMHistPlotter: " + dqmHistPlotterModuleName)
            dqmHistPlotterModule = dqmHistPlotter_template.clone(
                processes = cms.PSet(**processesForPlots),
                drawOptionSets = cms.PSet(
                    default = cms.PSet(**dict((sampleName, samples['ALL_SAMPLES'][sampleName]['drawOption'])
                                              for sampleName in samples['SAMPLES_TO_PLOT']))
                )
            )

            dqmHistPlotterModule.labels.mcNormScale.text = cms.vstring(
                '%0.1fpb^{-1}' % samples['TARGET_LUMI'],
                '#sqrt{s}=7TeV'
            )

            indOutputFileName_log = analyzer_drawJobConfigurator_indOutputFileName_set[2]
            posSeparator = indOutputFileName_log.rfind(".")
            if posSeparator != -1:
                indOutputFileName_log  = indOutputFileName_log[:posSeparator:] + "_log" + indOutputFileName_log[posSeparator::]
            else:
                # if no graphics format is specified explicitely,
                # save all plots in PDF format
                indOutputFileName_log += "_log.pdf"
            dqmHistPlotterModule_log = dqmHistPlotterModule.clone(
                drawJobs = drawJobConfigurator_log.configure(),
                indOutputFileName = cms.string(indOutputFileName_log)
            )
            setattr(process, dqmHistPlotterModuleName + "_log", dqmHistPlotterModule_log)

            indOutputFileName_linear = indOutputFileName_log.replace("_log.", "_linear.")
            dqmHistPlotterModule_linear = dqmHistPlotterModule.clone(
                drawJobs = drawJobConfigurator_linear.configure(),
                indOutputFileName = cms.string(indOutputFileName_linear)
            )
            setattr(process, dqmHistPlotterModuleName + "_linear", dqmHistPlotterModule_linear)

            if dqmHistPlotterSequence is None:
                dqmHistPlotterSequence = cms.Sequence(dqmHistPlotterModule_log)
            else:
                dqmHistPlotterSequence._seq = dqmHistPlotterSequence._seq * dqmHistPlotterModule_log
            dqmHistPlotterSequence._seq = dqmHistPlotterSequence._seq * dqmHistPlotterModule_linear

    if dqmHistPlotterSequence is not None:
        setattr(process, dqmHistPlotterSequenceName, dqmHistPlotterSequence)

    # apply factorization
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

    # configure DQMSimpleFileSaver module
    dqmSimpleFileSaverModuleName = "save%s" % channel
    dqmSimpleFileSaverModule = cms.EDAnalyzer("DQMSimpleFileSaver",
        outputFileName = cms.string(outputFilePath + '/' + "plots%s_all.root" % channel)
    )
    setattr(process, dqmSimpleFileSaverModuleName, dqmSimpleFileSaverModule)

    makePlotSequenceName = "make%sPlots" % channel
    makePlotSequence = cms.Sequence(dqmLoadFactorizeAndMergeSequence)
    if dumpDQMStore:
        process.dumpDQMStore = cms.EDAnalyzer("DQMStoreDump")
        makePlotSequence._seq = makePlotSequence._seq * process.dumpDQMStore
    makePlotSequence._seq = makePlotSequence._seq * dqmSimpleFileSaverModule
    dqmDumpFilterStatisticsModuleName = "dump%s" % channel
    if hasattr(process, dqmDumpFilterStatisticsModuleName):
        dqmDumpFilterStatisticsModule = getattr(process, dqmDumpFilterStatisticsModuleName)
        makePlotSequence._seq = makePlotSequence._seq * dqmDumpFilterStatisticsModule
    if dqmHistPlotterSequence is not None:
        makePlotSequence._seq = makePlotSequence._seq * dqmHistPlotterSequence
    setattr(process, makePlotSequenceName, makePlotSequence)

    process.p = cms.Path(makePlotSequence)
