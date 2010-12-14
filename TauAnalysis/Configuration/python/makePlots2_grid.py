import FWCore.ParameterSet.Config as cms

import copy
import os
import ROOT
import re

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
        MassRebin = copy.deepcopy(styles.xAxis_massRebin),
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
        #regular = copy.deepcopy(styles.legend_regular)
        regular = copy.deepcopy(styles.legend_big)
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

def get_filter_stat(file, filter_stats_dir, statistic,
                    type="processed_num#a1#s0"):
    " Parse the filter statistics of a harvested file to get # number of events"
    if isinstance(file, str):
        file = ROOT.TFile.Open(file, "READ")
    directory = os.path.join('DQMData', filter_stats_dir, statistic)
    matcher = re.compile(
        '<' + type + '>[if]=(?P<value>[0-9\.]*)</' + type + '>')
    filter_stats = file.Get(directory)
    for key in filter_stats.GetListOfKeys():
        matches = matcher.match(key.GetName())
        if matches:
            return float(matches.group('value'))
    return None

def get_skim_eff(skim_file_name, filter_stat_base_dir, skimFilterStatistic):
    " Get the efficiency for a harvested root file for a given filter stat "
    skim_file = ROOT.TFile(skim_file_name, "READ")
    # Get the filter statistics folder
    passed = get_filter_stat(
        skim_file, filter_stat_base_dir,
        skimFilterStatistic,
        'passed_cumulative_num#a1#s0')
    processed = get_filter_stat(
        skim_file, filter_stat_base_dir,
        skimFilterStatistic,
        'processed_num#a1#s0')
    local_skim_eff = passed/processed
    return local_skim_eff

def apply_auto_scale(sample_info, pset, target_lumi, dqmDirectoryFilterStatistics):
    " Build the autoscale information in the DQM loader [pset] given sample info"
    # Auto scale MC samples
    if sample_info['type'].lower().find('mc') != -1:
        pset.autoscale = cms.bool(True)
        pset.totalExpectedEventsBeforeSkim = cms.uint32(sample_info['events_processed'])
        pset.skimEfficiency = cms.double(sample_info['skim_eff'])
        pset.xSection = cms.double(sample_info['x_sec'])
        pset.targetIntLumi = cms.double(target_lumi)

        # Define the filter to take the processed events from
        pset.filterToUse = cms.string("genPhaseSpaceCut/processed_num#a1#s0")
        pset.filterStatisticsLocation = cms.string(dqmDirectoryFilterStatistics['factorizationDisabled'])
    else:
        # For data, don't apply any scaling
        pset.scaleFactor = cms.double(1.0)
    # If the sample is factorized, we need to change the location of the filter
    # statistics information
    if sample_info['factorize']:
        pset.filterStatisticsLocation = cms.string(dqmDirectoryFilterStatistics['factorizationEnabled'])

def _getInputSamples(mergeSampleDict, samples):
    # recursively expand name of samples to be merged
    # CV: a recursive implementation of this function is neccessary,
    #     as samples used as input for merging job might be defined
    #     in terms of other merged samples
    #    (e.g. smSum --> smBgSum --> qcdSum)

    retVal = []

    for sample in samples:
        if mergeSampleDict.get(sample) is not None:
            retVal.extend(_getInputSamples(mergeSampleDict, mergeSampleDict[sample]['samples']))
        else:
            retVal.append(sample)

    return retVal

def makePlots(process, channel = None, samples = None, inputFilePath = None, jobId = None,
              analyzer_drawJobConfigurator_indOutputFileName_sets = None,
              drawJobTemplate = None,
              enableFactorizationFunction = None,
              dqmDirectoryFilterStatistics = None, dumpDQMStore = False,
              skimStatFileMapper = None, skimFilterStatistic=None):

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
    samplesToLoad = []
    samplesToLoad.extend(samples['FLATTENED_SAMPLES_TO_PLOT'])
    for sample in samples['SAMPLES_TO_PRINT']:
        if not sample in samplesToLoad and not sample in samples['MERGE_SAMPLES']:
            samplesToLoad.append(sample)
    for sample in samplesToLoad:
        sample_info = samples['RECO_SAMPLES'][sample]

        # Get the location of filter stats for this sample
        filter_stat_base_dir = dqmDirectoryFilterStatistics[
            sample_info['factorize'] and 'factorizationEnabled' or
            'factorizationDisabled' ]

        # If this was run on a local skim, make sure we update the skim eff to
        # include the factor from the skim
        if skimStatFileMapper is not None:
            skim_file_name = skimStatFileMapper(sample)
            print "Loading level 2 skim info from file: %s" % skim_file_name
            local_skim_eff = get_skim_eff(skim_file_name, filter_stat_base_dir,
                                          skimFilterStatistic)
            # Update sample skim efficiency
            sample_info['skim_eff'] *= local_skim_eff
            print ("Got local skim eff: %0.2f%% for sample %s, "
                   "giving an overall skim eff of %0.2f%%" %
                   (100*local_skim_eff, sample, 100*sample_info['skim_eff']))

        # Build DQMFileLoader PSet for this sample
        sample_pset = cms.PSet(
            inputFileNames = cms.vstring(sample_mapper(sample)),
            dqmDirectory_store = cms.string('/harvested/%s' % sample),
        )

        # Apply autoscaling
        apply_auto_scale(sample_info, sample_pset, samples['TARGET_LUMI'],
                         dqmDirectoryFilterStatistics)

        # Add to our sample dictionary
        dqmFileLoaderJobs[sample] = sample_pset

    # configure DQMFileLoader module
    dqmFileLoaderModuleName = "load%sSamples" % channel
    dqmFileLoaderModule = cms.EDAnalyzer("DQMFileLoader",
        **dqmFileLoaderJobs)
    dqmFileLoaderModule.inputFilePath = cms.string(inputFilePath)
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
        input_samples = _getInputSamples(samples['MERGE_SAMPLES'], merge_info['samples'])
        # Build the new PSet
        new_pset = cms.PSet(
            dqmDirectories_input = cms.vstring([
                '/harvested/%s' % sample
                for sample in input_samples
            ]),
            dqmDirectory_output = cms.string('/harvested/%s'% merge_name),
        )
        # Add the new PSet to our list
        dqmHistAdderJobs["merge_%s" % merge_name] = new_pset

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

            print analyzer
            dqmHistPlotterModuleName = None
            if analyzer.find("_") != -1:
                dqmHistPlotterModuleName = "plot%s%s" % (channel, analyzer[analyzer.rfind("_"):])
            else:
                dqmHistPlotterModuleName = "plot%s" % channel
            dqmHistPlotterModuleName = "plot" + analyzer
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
    dqmDumpFilterStatisticsModuleName = "dump%s" % channel
    if hasattr(process, dqmDumpFilterStatisticsModuleName):
        dqmDumpFilterStatisticsModule = getattr(process, dqmDumpFilterStatisticsModuleName)
        makePlotSequence._seq = makePlotSequence._seq * dqmDumpFilterStatisticsModule
    if dqmHistPlotterSequence is not None:
        makePlotSequence._seq = makePlotSequence._seq * dqmHistPlotterSequence
    makePlotSequence._seq = makePlotSequence._seq * dqmSimpleFileSaverModule
    setattr(process, makePlotSequenceName, makePlotSequence)

    process.p = cms.Path(makePlotSequence)
