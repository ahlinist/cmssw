import FWCore.ParameterSet.Config as cms
import copy

import TauAnalysis.Configuration.recoSampleDefinitionsZtoDiTau_7TeV_grid_cfi as samples
import TauAnalysis.DQMTools.plotterStyleDefinitions_cfi as styles
import TauAnalysis.Configuration.plotZtoDiTau_drawJobs_cfi as drawJobs

#--------------------------------------------------------------------------------
#
# Plot histograms for Z --> tau-jet + tau-jet analysis
#
# Authors: Christian Veelken, UC Davis
#          Evan Friis, UC Davis
#
#--------------------------------------------------------------------------------

# Define the file loader jobs for each of our analyzed jobs
_fileLoaderJobs = {}
for sample in samples.FLATTENED_SAMPLES_TO_PLOT:
    sample_info = samples.RECO_SAMPLES[sample]
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
        sample_pset.targetIntLumi = cms.double(samples.TARGET_LUMI)

        # Define the filter to take the processed events from
        sample_pset.filterToUse = cms.string("genPhaseSpaceCut")
        sample_pset.filterStatisticsLocation = cms.string('zDiTauAnalyzer/FilterStatistics/')
    else:
        # For data, don't apply any scaling
        sample_pset.scaleFactor = cms.double(1.0)
    # If the sample is factorized, we need to change the location of the filter
    # statistics information
    if sample_info['factorize']:
        sample_pset.filterStatisticsLocation = cms.string(
            'zDiTauAnalyzer_factorizedTight2ndTau/FilterStatistics/')

    # Add to our sample dictionary
    _fileLoaderJobs[sample] = sample_pset

_histAdderJobs = {}
# Loop over the different merged samples we have defined and 
# merge them using the HistAdder
for merge_name in samples.MERGE_SAMPLES.keys():
    # Don't merge if we don't care about it
    if merge_name not in samples.SAMPLES_TO_PLOT:
        if merge_name not in samples.SAMPLES_TO_PRINT:
            continue
    merge_info = samples.MERGE_SAMPLES[merge_name]
    # Build the new PSet
    new_pset = cms.PSet(
        dqmDirectories_input = cms.vstring([
            '/harvested/%s/zDiTauAnalyzer'%(sample) 
            for sample in merge_info['samples']
        ]),
        dqmDirectory_output = cms.string(
            '/harvested/%s/zDiTauAnalyzer'%(merge_name))
    )
    # Add the new PSet to our list
    _histAdderJobs["merge_%s" % (merge_name)] = new_pset

# Build the file loader
loadZtoDiTauSamples = cms.EDAnalyzer("DQMFileLoader", **_fileLoaderJobs)
# Build the histogram mergers
mergeSamplesZtoDiTau = cms.EDAnalyzer('DQMHistAdder', **_histAdderJobs)

# Add an additional sequence layer that we can hook into when factorization
# This is necessary because the sample merging step will be changed to access
# to the factorized samples
loadAndFactorizeZtoDiTauSamples = cms.Sequence(loadZtoDiTauSamples)

loadZtoDiTau = cms.Sequence(loadAndFactorizeZtoDiTauSamples *
                             mergeSamplesZtoDiTau)

# Define plot processes and styles for each sample
# These get passed as kwargs to the DQMHistPlotter
_processesForZtoDiTauPlots = dict(
    (sampleName, cms.PSet(
        dqmDirectory = cms.string('/harvested/%s' % sampleName),
        legendEntry=cms.string(samples.ALL_SAMPLES[sampleName]['legendEntry']),
        type = cms.string(samples.ALL_SAMPLES[sampleName]['type']),
    )) for sampleName in samples.SAMPLES_TO_PLOT
)

_drawOptionSets = dict(
    (sampleName, samples.ALL_SAMPLES[sampleName]['drawOption'])
    for sampleName in samples.SAMPLES_TO_PLOT)

# Define draw job configurator for our smaples
drawJobTemplate = copy.deepcopy(drawJobs.plots_ZtoDiTau)
drawJobTemplate.plots.processes = cms.vstring(samples.SAMPLES_TO_PLOT)
# Stack all non-BSM sample
drawJobTemplate.stack = cms.vstring([
    sample for sample in samples.SAMPLES_TO_PLOT 
    if samples.ALL_SAMPLES[sample]['type'].find('bsm') == -1 and
    samples.ALL_SAMPLES[sample]['type'].find('Data') == -1
])
drawJobTemplate.yAxis = cms.string('numEntries_log')
##drawJobTemplate.yAxis = cms.string('numEntries_linear')

# Reset the template for the drawJob configurators
drawJobs.drawJobConfigurator_ZtoDiTau.setTemplate(drawJobTemplate)

plotZtoDiTau = cms.EDAnalyzer("DQMHistPlotter",
    processes = cms.PSet(**_processesForZtoDiTauPlots),

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

    drawOptionSets = cms.PSet(default = cms.PSet(**_drawOptionSets)),
                              
    drawJobs = drawJobs.drawJobConfigurator_ZtoDiTau.configure(),

    #canvasSizeX = cms.int32(800),
    #canvasSizeY = cms.int32(640),                         
    canvasSizeX = cms.int32(640),
    canvasSizeY = cms.int32(800),                         

    outputFilePath = cms.string('./plots/'),
    #outputFileName = cms.string('plotsZtoDiTau.ps')
    indOutputFileName = cms.string('plotZtoDiTau_#PLOT#.pdf')
)

saveZtoDiTau = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('plotsZtoDiTau_all.root')
)
