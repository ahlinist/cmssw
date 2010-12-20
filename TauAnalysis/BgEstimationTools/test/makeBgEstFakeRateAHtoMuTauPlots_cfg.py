import FWCore.ParameterSet.Config as cms
import os

from TauAnalysis.Configuration.recoSampleDefinitionsAHtoMuTau_7TeV_grid_cfi \
        import recoSampleDefinitionsAHtoMuTau_7TeV as samples

from TauAnalysis.Configuration.recoSampleDefinitionsAHtoMuTau_7TeV_grid_cfi \
        import make_flattened_samples

import TauAnalysis.Configuration.makePlots2_grid as makePlots
import TauAnalysis.Configuration.userRegistry as reg
from TauAnalysis.Configuration.plotAHtoMuTau_drawJobs_cfi import plots_AHtoMuTau

from TauAnalysis.BgEstimationTools.tools.drawFakeRateHistConfigurator import drawFakeRateHistConfigurator
import TauAnalysis.DQMTools.tools.drawJobConfigurator as drawjob
import TauAnalysis.Configuration.analyzeAHtoMuTau_cfi as cuts

import TauAnalysis.DQMTools.plotterStyleDefinitions_cfi as style

fr_types = [
    "tauIdDiscr",
    #"frUnweighted",
    "fr_qcdDiJetLeadJet",
    "fr_qcdDiJetSecondLeadJet",
    "fr_qcdMuEnriched",
    "fr_WplusJets",
    "fr_qcdDiJetLeadJet",
    "frSysUncertainty"
]


fr_drawOptions_bkg = {
    'tauIdDiscr' : style.drawOption_black_points,
    'fr_qcdMuEnriched' : style.drawOption_lightBlue_separate,
    'fr_qcdDiJetLeadJet' : style.drawOption_red_separate,
    'fr_qcdDiJetSecondLeadJet' : style.drawOption_orange_separate,
    'fr_WplusJets' : style.drawOption_green_separate,
    'frSysUncertainty' : style.drawOption_uncertainty,
}

fr_drawOptions_signal = {
    'tauIdDiscr' : style.drawOption_black_points,
    'fr_qcdMuEnriched' : style.drawOption_lightBlue_eff,
    'fr_qcdDiJetLeadJet' : style.drawOption_red_eff,
    'fr_qcdDiJetSecondLeadJet' : style.drawOption_orange_eff,
    'fr_WplusJets' : style.drawOption_green_eff,
    'frSysUncertainty' : style.drawOption_uncertainty,
}


fake_sources = [
    #'qcdSum',
    'WplusJetsSum',
    'TTplusJets',
    #'Zmumu'
]

signal_sources = [
    'ZtautauPU156bx'
]

samples['SAMPLES_TO_PRINT'][:] = []
samples['SAMPLES_TO_PLOT'][:] = fake_sources + signal_sources
samples['FLATTENED_SAMPLES_TO_PLOT'] = make_flattened_samples()

channel = 'AHtoMuTau'
_REGULAR_JOBID = 'Run33'
_FR_JOBID = 'Run33FR'

reg.overrideJobId(channel, 'Run33FR')

process = cms.Process('makeBgEstFakeRatePlots')

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(0)
)
process.source = cms.Source("EmptySource")

# Load the regular analysis workflow file
reg.overrideJobId(channel, _REGULAR_JOBID)
makePlots.makePlots(
    process, channel, samples,
    inputFilePath = reg.getHarvestingFilePath(channel),
    jobId = reg.getJobId(channel),
    skimStatFileMapper = reg.makeSkimStatFileMapper(channel),
    skimFilterStatistic = 'evtSelDiMuPairZmumuHypothesisVetoByLooseIsolation',
    analyzer_drawJobConfigurator_indOutputFileName_sets  = [],
    drawJobTemplate = plots_AHtoMuTau,
    enableFactorizationFunction = None,
    dqmDirectoryFilterStatistics = {
        'factorizationDisabled' : 'ahMuTauAnalyzerOS_woBtag/FilterStatistics',
        'factorizationEnabled' : 'ahMuTauAnalyzerOS_woBtag_factorizedWithMuonIsolation/FilterStatistics'
    },
    dqmDirectoryFilterStatisticsForSkim = {
        'factorizationDisabled' : 'ahMuTauAnalyzerOS_woBtag/FilterStatistics',
        'factorizationEnabled' : 'ahMuTauAnalyzerOS_woBtag_factorizedWithMuonIsolation/FilterStatistics'
    },
    disableFactorizationSample = True,
    disableFactorizationSkim = False,
    dumpDQMStore = False,
    moduleLabel = "Regular",
)

# Load the fake rate workflow file
reg.overrideJobId(channel, _FR_JOBID)
makePlots.makePlots(
    process, channel, samples,
    inputFilePath = reg.getHarvestingFilePath(channel),
    jobId = reg.getJobId(channel),
    skimStatFileMapper = reg.makeSkimStatFileMapper(channel),
    skimFilterStatistic = 'evtSelDiMuPairZmumuHypothesisVetoByLooseIsolation',
    analyzer_drawJobConfigurator_indOutputFileName_sets  = [],
    drawJobTemplate = plots_AHtoMuTau,
    enableFactorizationFunction = None,
    dqmDirectoryFilterStatistics = {
        'factorizationDisabled' : 'ahMuTauAnalyzerOS_woBtag_frUnweighted/FilterStatistics',
        'factorizationEnabled' : 'ahMuTauAnalyzerOS_woBtag_factorizedWithMuonIsolation_frUnweighted/FilterStatistics'
    },
    dqmDirectoryFilterStatisticsForSkim = {
        'factorizationDisabled' : 'ahMuTauAnalyzerOS_woBtag/FilterStatistics',
        'factorizationEnabled' : 'ahMuTauAnalyzerOS_woBtag_factorizedWithMuonIsolation/FilterStatistics'
    },
    disableFactorizationSample = True,
    disableFactorizationSkim = False,
    dumpDQMStore = False,
    moduleLabel = "FakeRate"
)

# Now modify the stuff makePlots calls created created
# Move the regualr stuff to their own workflow

regular_loader = getattr(process, "load%sSamplesRegular" % channel)
for parameter in regular_loader.parameterNames_():
    param = regular_loader.getParameter(parameter)
    if isinstance(param, cms.PSet):
        param.dqmDirectory_store = cms.string(
            "/tauIdDiscr" + param.dqmDirectory_store.value())

# Move the FR weighted histograms to their own folder /tauFakeRate
fakerate_loader = getattr(process, "load%sSamplesFakeRate" % channel)
for parameter in fakerate_loader.parameterNames_():
    param = fakerate_loader.getParameter(parameter)
    if isinstance(param, cms.PSet):
        param.dqmDirectory_store = cms.string(
            "/tauFakeRate" + param.dqmDirectory_store.value())

# Build our fake rate sequence
process.fakeRateSequence = cms.Sequence(regular_loader * fakerate_loader)

# Modify the sample adders so that merge samples get moved correctly
for type, dir in [('Regular', '/tauIdDiscr'), ('FakeRate', '/tauFakeRate')]:
    sample_merger = getattr(process, "mergeSamples%s%s" % (channel, type))
    for parameter in sample_merger.parameterNames_():
        param = sample_merger.getParameter(parameter)
        if isinstance(param, cms.PSet):
            param.dqmDirectory_output = cms.string(
                dir+ param.dqmDirectory_output.value())
            param.dqmDirectories_input = cms.vstring(
                list(dir + value for value in param.dqmDirectories_input)
            )
    process.fakeRateSequence += sample_merger

# Build error band
process.buildErrorBand = cms.EDAnalyzer(
    "DQMHistErrorBandProducer",
    config = cms.VPSet()
)
process.fakeRateSequence += process.buildErrorBand

analyzers = [
    ("ahMuTauAnalyzerOS_woBtag", cuts.evtSelDiTauCandidateForAHtoMuTauZeroCharge),
    ("ahMuTauAnalyzerOS_wBtag", cuts.evtSelDiTauCandidateForAHtoMuTauZeroCharge),
    ("ahMuTauAnalyzerSS_woBtag", cuts.evtSelDiTauCandidateForAHtoMuTauNonZeroCharge),
    ("ahMuTauAnalyzerSS_wBtag", cuts.evtSelDiTauCandidateForAHtoMuTauNonZeroCharge)
]

# Build each error band producer
for sample in samples['SAMPLES_TO_PLOT']:
    for analyzer, cut in analyzers:
        new_pset = cms.PSet(
            dqmDirectories_inputVariance = cms.vstring(),
            dqmDirectory_output = cms.string(
                '/tauFakeRate/harvested/%s/%s_frSysUncertainty' %
                (sample, analyzer)),
            method = cms.string("min_max")
        )
        for fr_type in fr_types:
            new_pset.dqmDirectories_inputVariance.append(
                '/tauFakeRate/harvested/%s/%s_%s' % (sample, analyzer, fr_type))
        process.buildErrorBand.config.append(new_pset)

# Map fake rate types to DQM directories
# We introduce another layer in this dict to account for multiple analyzer types
dqmDirectories = {}
for analyzer, cut in analyzers:
    ana_dict = {}
    dqmDirectories[analyzer] = ana_dict
    for fr_type in fr_types:
        ana_dict[fr_type] = os.path.join(
            '/tauFakeRate',
            '#PROCESSDIR#',
            analyzer + "_" + fr_type
        )
        # Add the non-weighted and error band
        ana_dict['tauIdDiscr'] = '/tauIdDiscr/#PROCESSDIR#/%s' % analyzer
        ana_dict['frSysUncertainty'] = '/tauFakeRate/#PROCESSDIR#/%s_frSysUncertainty' % analyzer


legendEntries = dict()
legendEntries["tauIdDiscr"] = "Final Analysis"
legendEntries["fr_qcdMuEnriched"] = "QCD fr., #mu enriched."
legendEntries["fr_qcdDiJetLeadJet"] = "QCD fr., lead. Jet"
legendEntries["fr_qcdDiJetSecondLeadJet"] = "QCD fr., next-to-lead. Jet"
legendEntries["fr_WplusJets"] = "W + Jets fr."
legendEntries["frSysUncertainty"] = "Average fr."
legendEntries["tauIdEff"] = "Tau Id. eff."

# Define the quantities we want to plot
bgEstFakeRatePlots = [
    drawjob.drawJobConfigEntry(
        meName = 'MuonQuantities/Muon#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "Muon (final Event sample)",
        xAxis = '#PAR#',
        name = "bgEstFakeRatePlots_#PROCESSNAME#_muon"
    ),
    drawjob.drawJobConfigEntry(
        meName = 'TauQuantities/Tau#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "Tau (final Event sample)",
        xAxis = '#PAR#',
        name = "bgEstFakeRatePlots_#PROCESSNAME#_tau"
    ),
    drawjob.drawJobConfigEntry(
        meName = 'TauQuantities/TauLeadTrkPt',
        title = "Tau lead. Track (final Event sample)",
        xAxis = 'Pt',
        name = "bgEstFakeRatePlots_#PROCESSNAME#_tauLeadTrkPt"
    ),
    drawjob.drawJobConfigEntry(
        meName = 'DiTauCandidateQuantities/Mt1MET',
        title = "M_{T}(Muon + MET) (final Event sample)",
        xAxis = 'Mt',
        name = "bgEstFakeRatePlots_#PROCESSNAME#_mtMuonMET"
    ),
    drawjob.drawJobConfigEntry(
        meName = 'DiTauCandidateQuantities/VisMass',
        title = "M_{vis}(Muon + Tau) (final Event sample)",
        xAxis = 'Mass',
        name = "bgEstFakeRatePlots_#PROCESSNAME#_mVisible"
    ),
    drawjob.drawJobConfigEntry(
        meName = 'DiTauCandidateSVfitQuantities/psKine_MEt_ptBalance/Mass',
        title = "M(Muon + Tau), SVfit method (final Event sample)",
        xAxis = 'MassRebin',
        name = "bgEstFakeRatePlots_#PROCESSNAME#_mSVmethod"
    ),
    drawjob.drawJobConfigEntry(
        meName = 'DiTauCandidateQuantities/PzetaDiff',
        title = "P_{#zeta} - 1.5*P_{#zeta}^{vis} (final Event sample)",
        xAxis = 'GeV',
        name = "finalSamplePlots_#PROCESSNAME#_PzetaDiff"
    ),
]

# Make plots for each analysis chain
for analyzer, cut in analyzers:
    # The basic draw job configurator
    drawFakeRateHist_background = drawFakeRateHistConfigurator(
        template = cms.PSet(
            xAxis = cms.string('unlabeled'),
            #yAxis = cms.string('numEntries_linear'),
            yAxis = cms.string('numEntries_log'),
            legend = cms.string('regular'),
            labels = cms.vstring('mcNormScale')
        ),
        dqmDirectories = dqmDirectories[analyzer],
        legendEntries = legendEntries,
        frTypes = fr_types
    )
    # Add all our of our fake background sources
    for source in fake_sources:
        drawFakeRateHist_background.addProcess(
            source, cms.string('/harvested/%s' % source))

    drawFakeRateHist_background.addPlots(
        afterCut = cut,
        plots = bgEstFakeRatePlots
    )

    # Now build the actual DQM hist plotter
    plotter = makePlots.dqmHistPlotter_template.clone()
    # build the styles
    plotter.drawOptionEntries = cms.PSet(
        **fr_drawOptions_bkg)
    # Build the different processes
    processes = {}
    for fr_type in fr_types:
        processes[fr_type] = cms.PSet(
            dqmDirectory = cms.string(''),
            legendEntry = cms.string(fr_type),
            type = cms.string('smMC')
        )
    plotter.processes = cms.PSet(**processes)
    plotter.outputFilePath = cms.string("./plots/")
    plotter.indOutputFileName = cms.string("plotBgEstFakeRate_%s_#PLOT#.png" %
                                           analyzer)

    plotter.labels.mcNormScale.text = cms.vstring(
        '%0.1fpb^{-1}' % samples['TARGET_LUMI'],
        '#sqrt{s}=7TeV'
    )
    # Configure draw jobs
    plotter.drawJobs = drawFakeRateHist_background.configure()
    # Add to the process
    setattr(process, "makePlots" + analyzer, plotter)
    process.fakeRateSequence += plotter

    #################################################################
    ############## Make signal plotter
    #################################################################
    drawFakeRateHist_signal = drawFakeRateHistConfigurator(
        template = cms.PSet(
            xAxis = cms.string('unlabeled'),
            yAxis = cms.string('numEntries_linear'),
            #yAxis = cms.string('numEntries_log'),
            legend = cms.string('regular'),
            labels = cms.vstring('mcNormScale')
        ),
        dqmDirectories = dqmDirectories[analyzer],
        legendEntries = legendEntries,
        frTypes = fr_types,
    )

    drawFakeRateHist_signal.addProcess(
        signal_sources[0], cms.string("/harvested/%s" % signal_sources[0]))

    drawFakeRateHist_signal.addPlots(
        afterCut = cut,
        plots = bgEstFakeRatePlots
    )
    sig_plotter = plotter.clone()
    sig_plotter.drawOptionEntries = cms.PSet(
        **fr_drawOptions_signal)

    sig_plotter.drawJobs = drawFakeRateHist_signal.configure()
    sig_plotter.indOutputFileName = cms.string(
        "plotSignalBgEstFakeRate_%s_#PLOT#.png" % analyzer)

    setattr(process, "makePlotsSignal" + analyzer, sig_plotter)
    # Only enable the ZTT plots if we are loading it.
    if signal_sources:
        process.fakeRateSequence += sig_plotter

process.save = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('fakerates.root'),
    #outputCommands = cms.vstring('drop tauIdDiscr/*')
)
process.dumpDQMStore = cms.EDAnalyzer("DQMStoreDump")
# Todo add signal
process.p = cms.Path(
    process.fakeRateSequence
   + process.dumpDQMStore
    *process.save)

dump = open('dump.py', 'w')
dump.write(process.dumpPython())
