import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------
#
# Make control plots summarizing performance of fake-rate technique
# for data-driven background estimation in the Z --> mu + tau-jet channel
#
# Author: Christian Veelken, UC Davis
#
#--------------------------------------------------------------------------------

# uncomment next line to make plots for 10 TeV centre-of-mass energy
#from TauAnalysis.Configuration.plotZtoMuTau_processes_10TeV_cfi import *
# uncomment next line to make plots for 7 TeV centre-of-mass energy
from TauAnalysis.Configuration.plotZtoMuTau_processes_7TeV_cfi import *
from TauAnalysis.Configuration.plotZtoMuTau_drawJobs_cfi import *
from TauAnalysis.Configuration.plotZtoMuTau_cff import loadZtoMuTau
from TauAnalysis.Configuration.analyzeZtoMuTau_cfi import *

from TauAnalysis.DQMTools.plotterStyleDefinitions_cfi import *

from TauAnalysis.BgEstimationTools.tools.drawFakeRateHistConfigurator import drawFakeRateHistConfigurator
from TauAnalysis.BgEstimationTools.tools.fakeRateAnalysisTools import reconfigDQMFileLoader

process = cms.Process('makeBgEstFakeRateZtoMuTauPlots')

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(0)
)

process.source = cms.Source("EmptySource")

#--------------------------------------------------------------------------------
process.loadBgEstFakeRateZtoMuTau_tauIdDiscr = cms.EDAnalyzer("DQMFileLoader",
    tauIdDiscr = cms.PSet(
        inputFileNames = cms.vstring('rfio:/castor/cern.ch/user/v/veelken/CMSSW_3_3_x/plots/ZtoMuTau/7TeV/plotsZtoMuTau_all.root'),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('tauIdDiscr')
    )
)

process.loadBgEstFakeRateZtoMuTau_tauFakeRate = copy.deepcopy(loadZtoMuTau)
reconfigDQMFileLoader(
    process.loadBgEstFakeRateZtoMuTau_tauFakeRate,
    dqmDirectory = 'tauFakeRate/#PROCESSDIR#'
)
process.loadBgEstFakeRateZtoMuTau_tauFakeRate.inputFilePath = cms.string("rfio:/castor/cern.ch/user/v/veelken/CMSSW_3_3_x/bgEstPlots/ZtoMuTau_frSimple/7TeV/")
#process.loadBgEstFakeRateZtoMuTau_tauFakeRate.inputFilePath = cms.string("rfio:/castor/cern.ch/user/v/veelken/CMSSW_3_3_x/bgEstPlots/ZtoMuTau_frCDF/7TeV/")

process.addBgEstFakeRateZtoMuTau_qcdSum_tauFakeRate = cms.EDAnalyzer("DQMHistAdder",
    qcdSum = cms.PSet(
        dqmDirectories_input = cms.vstring(
            'tauFakeRate/harvested/InclusivePPmuX/',
            'tauFakeRate/harvested/PPmuXptGt20/'
        ),
        dqmDirectory_output = cms.string('tauFakeRate/harvested/qcdSum/')
    )
)

process.addBgEstFakeRateZtoMuTau_smBgSum_tauFakeRate = cms.EDAnalyzer("DQMHistAdder",
    smBgSum = cms.PSet(
        dqmDirectories_input = cms.vstring(
            'tauFakeRate/harvested/Zmumu/',
            'tauFakeRate/harvested/WplusJets/',
            'tauFakeRate/harvested/TTplusJets/',
            'tauFakeRate/harvested/qcdSum/'
        ),
        dqmDirectory_output = cms.string('tauFakeRate/harvested/smBgSum/')
    )
)

process.addBgEstFakeRateZtoMuTau_smSum_tauFakeRate = cms.EDAnalyzer("DQMHistAdder",
    smSum = cms.PSet(
        dqmDirectories_input = cms.vstring(
            'tauFakeRate/harvested/Ztautau/',
            'tauFakeRate/harvested/smBgSum/'
        ),
        dqmDirectory_output = cms.string('tauFakeRate/harvested/smSum/')
    )
)
process.addBgEstFakeRateZtoMuTau_tauFakeRate = cms.Sequence(
    process.addBgEstFakeRateZtoMuTau_qcdSum_tauFakeRate
   + process.addBgEstFakeRateZtoMuTau_smBgSum_tauFakeRate
   + process.addBgEstFakeRateZtoMuTau_smSum_tauFakeRate
)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
process.compErrorBandsBgEstFakeRateZtoMuTau = cms.EDAnalyzer("DQMHistErrorBandProducer",
    config = cms.VPSet(
        cms.PSet(
            dqmDirectories_inputVariance = cms.vstring(
                'tauFakeRate/harvested/Ztautau/zMuTauAnalyzer_qcdMuEnriched',
                'tauFakeRate/harvested/Ztautau/zMuTauAnalyzer_qcdDiJetLeadJet',
                'tauFakeRate/harvested/Ztautau/zMuTauAnalyzer_qcdDiJetSecondLeadJet',
                'tauFakeRate/harvested/Ztautau/zMuTauAnalyzer_WplusJets'
                #'tauFakeRate/harvested/Ztautau/zMuTauAnalyzer_gammaPlusJets'
            ),
            dqmDirectory_output = cms.string('tauFakeRate/harvested/Ztautau/frSysUncertainty'),
            method = cms.string("min_max")
        ),
        cms.PSet(
            dqmDirectories_inputVariance = cms.vstring(
                'tauFakeRate/harvested/Zmumu/zMuTauAnalyzer_qcdMuEnriched',
                'tauFakeRate/harvested/Zmumu/zMuTauAnalyzer_qcdDiJetLeadJet',
                'tauFakeRate/harvested/Zmumu/zMuTauAnalyzer_qcdDiJetSecondLeadJet',
                'tauFakeRate/harvested/Zmumu/zMuTauAnalyzer_WplusJets'
                #'tauFakeRate/harvested/Zmumu/zMuTauAnalyzer_gammaPlusJets'
            ),
            dqmDirectory_output = cms.string('tauFakeRate/harvested/Zmumu/frSysUncertainty'),
            method = cms.string("min_max")
        ),
        cms.PSet(
            dqmDirectories_inputVariance = cms.vstring(
                'tauFakeRate/harvested/WplusJets/zMuTauAnalyzer_qcdMuEnriched',
                'tauFakeRate/harvested/WplusJets/zMuTauAnalyzer_qcdDiJetLeadJet',
                'tauFakeRate/harvested/WplusJets/zMuTauAnalyzer_qcdDiJetSecondLeadJet',
                'tauFakeRate/harvested/WplusJets/zMuTauAnalyzer_WplusJets'
                #'tauFakeRate/harvested/WplusJets/zMuTauAnalyzer_gammaPlusJets'
            ),
            dqmDirectory_output = cms.string('tauFakeRate/harvested/WplusJets/frSysUncertainty'),
            method = cms.string("min_max")
        ),
        cms.PSet(
            dqmDirectories_inputVariance = cms.vstring(
                'tauFakeRate/harvested/TTplusJets/zMuTauAnalyzer_qcdMuEnriched',
                'tauFakeRate/harvested/TTplusJets/zMuTauAnalyzer_qcdDiJetLeadJet',
                'tauFakeRate/harvested/TTplusJets/zMuTauAnalyzer_qcdDiJetSecondLeadJet',
                'tauFakeRate/harvested/TTplusJets/zMuTauAnalyzer_WplusJets'
                #'tauFakeRate/harvested/TTplusJets/zMuTauAnalyzer_gammaPlusJets'
            ),
            dqmDirectory_output = cms.string('tauFakeRate/harvested/TTplusJets/frSysUncertainty'),
            method = cms.string("min_max")
        ),
        cms.PSet(
            dqmDirectories_inputVariance = cms.vstring(
                'tauFakeRate/harvested/qcdSum/zMuTauAnalyzer_qcdMuEnriched',
                'tauFakeRate/harvested/qcdSum/zMuTauAnalyzer_qcdDiJetLeadJet',
                'tauFakeRate/harvested/qcdSum/zMuTauAnalyzer_qcdDiJetSecondLeadJet',
                'tauFakeRate/harvested/qcdSum/zMuTauAnalyzer_WplusJets'
                #'tauFakeRate/harvested/qcdSum/zMuTauAnalyzer_gammaPlusJets'
            ),
            dqmDirectory_output = cms.string('tauFakeRate/harvested/qcdSum/frSysUncertainty'),
            method = cms.string("min_max")
        ),
        cms.PSet(
            dqmDirectories_inputVariance = cms.vstring(
                'tauFakeRate/harvested/smBgSum/zMuTauAnalyzer_qcdMuEnriched',
                'tauFakeRate/harvested/smBgSum/zMuTauAnalyzer_qcdDiJetLeadJet',
                'tauFakeRate/harvested/smBgSum/zMuTauAnalyzer_qcdDiJetSecondLeadJet',
                'tauFakeRate/harvested/smBgSum/zMuTauAnalyzer_WplusJets'
                #'tauFakeRate/harvested/smBgSum/zMuTauAnalyzer_gammaPlusJets'
            ),
            dqmDirectory_output = cms.string('tauFakeRate/harvested/smBgSum/frSysUncertainty'),
            method = cms.string("min_max")
        )
    )
)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
dqmDirectories = dict()
dqmDirectories["tauIdDiscr"] = 'tauIdDiscr' + '/' + '#PROCESSDIR#' + '/' + 'zMuTauAnalyzer'
dqmDirectories["frQCDmuEnriched"] = 'tauFakeRate' + '/' + '#PROCESSDIR#' + '/' + 'zMuTauAnalyzer_qcdMuEnriched'
dqmDirectories["frQCDdiJetLeadJet"] = 'tauFakeRate' + '/' + '#PROCESSDIR#' + '/' + 'zMuTauAnalyzer_qcdDiJetLeadJet'
dqmDirectories["frQCDdiJetSecondLeadJet"] = 'tauFakeRate' + '/' + '#PROCESSDIR#' + '/' + 'zMuTauAnalyzer_qcdDiJetSecondLeadJet'
dqmDirectories["frWplusJets"] = 'tauFakeRate' + '/' + '#PROCESSDIR#' + '/' + 'zMuTauAnalyzer_WplusJets'
##dqmDirectories["frGammaPlusJets"] = 'tauFakeRate' + '/' + '#PROCESSDIR#' + '/' + 'zMuTauAnalyzer_gammaPlusJets'
dqmDirectories["frSysUncertainty"] = 'tauFakeRate' + '/' + '#PROCESSDIR#' + '/' + 'frSysUncertainty'
dqmDirectories["tauIdEff"] = 'tauFakeRate' + '/' + '#PROCESSDIR#' + '/' + 'zMuTauAnalyzer_tauIdEfficiency'

legendEntries = dict()
legendEntries["tauIdDiscr"] = "Final Analysis"
legendEntries["frQCDmuEnriched"] = "QCD fr., #mu enriched."
legendEntries["frQCDdiJetLeadJet"] = "QCD fr., lead. Jet"
legendEntries["frQCDdiJetSecondLeadJet"] = "QCD fr., next-to-lead. Jet"
legendEntries["frWplusJets"] = "W + Jets fr."
##legendEntries["frGammaPlusJets"] = "#gamma + Jets fr."
legendEntries["frSysUncertainty"] = "Average fr."
legendEntries["tauIdEff"] = "Tau Id. eff."

bgEstFakeRatePlots = [
    drawJobConfigEntry(
        meName = 'MuonQuantities/Muon#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "Muon (final Event sample)",
        xAxis = '#PAR#',
        name = "bgEstFakeRatePlots_#PROCESSNAME#_muon"
    ),
    drawJobConfigEntry(
        meName = 'TauQuantities/Tau#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "Tau (final Event sample)",
        xAxis = '#PAR#',
        name = "bgEstFakeRatePlots_#PROCESSNAME#_tau"
    ),
    drawJobConfigEntry(
        meName = 'TauQuantities/TauLeadTrkPt',
        title = "Tau lead. Track (final Event sample)",
        xAxis = 'Pt',
        name = "bgEstFakeRatePlots_#PROCESSNAME#_tauLeadTrkPt"
    ),
    drawJobConfigEntry(
        meName = 'DiTauCandidateQuantities/Mt1MET',
        title = "M_{T}(Muon + MET) (final Event sample)",
        xAxis = 'Mt',
        name = "bgEstFakeRatePlots_#PROCESSNAME#_mtMuonMET"
    ),
    drawJobConfigEntry(
        meName = 'DiTauCandidateQuantities/VisMass',
        title = "M_{vis}(Muon + Tau) (final Event sample)",
        xAxis = 'Mass',
        name = "bgEstFakeRatePlots_#PROCESSNAME#_mVisible"
    )
]

drawFakeRateHistConfiguratorZtoMuTau_background = drawFakeRateHistConfigurator(
    template = cms.PSet(
        xAxis = cms.string('unlabeled'),
        #yAxis = cms.string('numEntries_linear'),
        yAxis = cms.string('numEntries_log'),
        legend = cms.string('regular'),
        labels = cms.vstring('mcNormScale')
    ),
    dqmDirectories = dqmDirectories,
    legendEntries = legendEntries,
    frTypes = [
        "tauIdDiscr",
        "frQCDmuEnriched",
        "frQCDdiJetLeadJet",
        "frQCDdiJetSecondLeadJet",
        "frWplusJets",
        ##"frGammaPlusJets",
        "frSysUncertainty"
    ]
)

drawFakeRateHistConfiguratorZtoMuTau_background.addProcess(
    "Zmumu", processZtoMuTau_Zmumu.config_dqmHistPlotter.dqmDirectory)
drawFakeRateHistConfiguratorZtoMuTau_background.addProcess("WplusJets", processZtoMuTau_WplusJets.config_dqmHistPlotter.dqmDirectory)
drawFakeRateHistConfiguratorZtoMuTau_background.addProcess("TTplusJets", processZtoMuTau_TTplusJets.config_dqmHistPlotter.dqmDirectory)
drawFakeRateHistConfiguratorZtoMuTau_background.addProcess("QCD", cms.string('harvested/qcdSum'))
drawFakeRateHistConfiguratorZtoMuTau_background.addProcess("smBackgroundSum", cms.string('harvested/smBgSum'))

drawFakeRateHistConfiguratorZtoMuTau_background.addPlots(
    afterCut = evtSelDiMuPairZmumuHypothesisVeto,
    plots = bgEstFakeRatePlots
)

process.plotBgEstFakeRateZtoMuTau_background = cms.EDAnalyzer("DQMHistPlotter",
    processes = cms.PSet(
        tauIdDiscr = cms.PSet(
            #dqmDirectory = cms.string(dqmDirectories["tauIdDiscr"]),
            dqmDirectory = cms.string(''),
            legendEntry = cms.string('tauIdDiscr'),
            type = cms.string('smMC')
        ),
        frQCDmuEnriched = cms.PSet(
            #dqmDirectory = cms.string(dqmDirectories["frQCDmuEnriched"]),
            dqmDirectory = cms.string(''),
            legendEntry = cms.string('frQCDmuEnriched'),
            type = cms.string('smMC')
        ),
        frQCDdiJetLeadJet = cms.PSet(
            #dqmDirectory = cms.string(dqmDirectories["frQCDdiJetLeadJet"]),
            dqmDirectory = cms.string(''),
            legendEntry = cms.string('frQCDdiJetLeadJet'),
            type = cms.string('smMC')
        ),
        frQCDdiJetSecondLeadJet = cms.PSet(
            #dqmDirectory = cms.string(dqmDirectories["frQCDdiJetSecondLeadJet"]),
            dqmDirectory = cms.string(''),
            legendEntry = cms.string('frQCDdiJetSecondLeadJet'),
            type = cms.string('smMC')
        ),
        frWplusJets = cms.PSet(
            #dqmDirectory = cms.string(dqmDirectories["frWplusJets"]),
            dqmDirectory = cms.string(''),
            legendEntry = cms.string('frWplusJets'),
            type = cms.string('smMC')
        ),
        ##frGammaPlusJets = cms.PSet(
        ##    #dqmDirectory = cms.string(dqmDirectories["frGammaPlusJets"]),
        ##    dqmDirectory = cms.string(''),
        ##    legendEntry = cms.string('frGammaPlusJets'),
        ##    type = cms.string('smMC')
        ##),
        frSysUncertainty = cms.PSet(
            #dqmDirectory = cms.string(dqmDirectories["frSysUncertainty"]),
            dqmDirectory = cms.string(''),
            legendEntry = cms.string('frSysUncertainty'),
            type = cms.string('smMC')
        )
    ),

    xAxes = cms.PSet(
        Pt = copy.deepcopy(xAxis_pt),
        Eta = copy.deepcopy(xAxis_eta),
        Phi = copy.deepcopy(xAxis_phi),
        IPxy = copy.deepcopy(xAxis_ipXY),
        IPz = copy.deepcopy(xAxis_ipZ),
        dR = copy.deepcopy(xAxis_dR),
        dPhi = copy.deepcopy(xAxis_dPhi),
        prob = copy.deepcopy(xAxis_prob),
        posZ = copy.deepcopy(xAxis_posZ),
        Mt = copy.deepcopy(xAxis_transMass),
        Mass = copy.deepcopy(xAxis_mass),
        N = copy.deepcopy(xAxis_num),
        PdgId = copy.deepcopy(xAxis_pdgId),
        GeV = copy.deepcopy(xAxis_GeV),
        unlabeled = copy.deepcopy(xAxis_unlabeled)
    ),

    yAxes = cms.PSet(
        numEntries_linear = copy.deepcopy(yAxis_numEntries_linear),
        numEntries_log = copy.deepcopy(yAxis_numEntries_log)
    ),

    legends = cms.PSet(
        regular = copy.deepcopy(legend_regular)
    ),

    labels = cms.PSet(
        mcNormScale = copy.deepcopy(label_mcNormScale)
    ),

    drawOptionEntries = cms.PSet(
        #tauIdDiscr = copy.deepcopy(drawOption_black_separate),
        tauIdDiscr = copy.deepcopy(drawOption_black_points),
        frQCDmuEnriched = copy.deepcopy(drawOption_lightBlue_separate),
        frQCDdiJetLeadJet = copy.deepcopy(drawOption_red_separate),
        frQCDdiJetSecondLeadJet = copy.deepcopy(drawOption_orange_separate),
        frWplusJets = copy.deepcopy(drawOption_green_separate),
        ##frGammaPlusJets = copy.deepcopy(drawOption_yellow_separate),
        frSysUncertainty = copy.deepcopy(drawOption_uncertainty)
    ),

    drawJobs = drawFakeRateHistConfiguratorZtoMuTau_background.configure(),

    canvasSizeX = cms.int32(800),
    canvasSizeY = cms.int32(800), # quadratic plots
    #canvasSizeY = cms.int32(640),  # rectangular (landscape) plots

    outputFilePath = cms.string('./plots/'),
    #outputFileName = cms.string('plotsBgEstFakeRateZtoMuTau.ps')
    indOutputFileName = cms.string('plotBgEstFakeRateZtoMuTau_#PLOT#.eps')
)

drawFakeRateHistConfiguratorZtoMuTau_signal = drawFakeRateHistConfigurator(
    template = cms.PSet(
        xAxis = cms.string('unlabeled'),
        yAxis = cms.string('numEntries_linear'),
        #yAxis = cms.string('numEntries_log'),
        legend = cms.string('regular'),
        labels = cms.vstring('mcNormScale')
    ),
    dqmDirectories = dqmDirectories,
    legendEntries = legendEntries,
    frTypes = [
        "frQCDmuEnriched",
        "frQCDdiJetLeadJet",
        "frQCDdiJetSecondLeadJet",
        "frWplusJets",
        ##"frGammaPlusJets"
    ]
)

drawFakeRateHistConfiguratorZtoMuTau_signal.addProcess("Ztautau", processZtoMuTau_Ztautau.config_dqmHistPlotter.dqmDirectory)

drawFakeRateHistConfiguratorZtoMuTau_signal.addPlots(
    afterCut = evtSelDiMuPairZmumuHypothesisVeto,
    plots = bgEstFakeRatePlots
)

process.plotBgEstFakeRateZtoMuTau_signal = process.plotBgEstFakeRateZtoMuTau_background.clone(
    processes = cms.PSet(
        frQCDmuEnriched = cms.PSet(
            #dqmDirectory = cms.string(dqmDirectories["frQCDmuEnriched"]),
            dqmDirectory = cms.string(''),
            legendEntry = cms.string('frQCDmuEnriched'),
            type = cms.string('smMC')
        ),
        frQCDdiJetLeadJet = cms.PSet(
            #dqmDirectory = cms.string(dqmDirectories["frQCDdiJetLeadJet"]),
            dqmDirectory = cms.string(''),
            legendEntry = cms.string('frQCDdiJetLeadJet'),
            type = cms.string('smMC')
        ),
        frQCDdiJetSecondLeadJet = cms.PSet(
            #dqmDirectory = cms.string(dqmDirectories["frQCDdiJetSecondLeadJet"]),
            dqmDirectory = cms.string(''),
            legendEntry = cms.string('frQCDdiJetSecondLeadJet'),
            type = cms.string('smMC')
        ),
        frWplusJets = cms.PSet(
            #dqmDirectory = cms.string(dqmDirectories["frWplusJets"]),
            dqmDirectory = cms.string(''),
            legendEntry = cms.string('frWplusJets'),
            type = cms.string('smMC')
        ),
        ##frGammaPlusJets = cms.PSet(
        ##    #dqmDirectory = cms.string(dqmDirectories["frGammaPlusJets"]),
        ##    dqmDirectory = cms.string(''),
        ##    legendEntry = cms.string('frGammaPlusJets'),
        ##    type = cms.string('smMC')
        ##)
    ),

    legends = cms.PSet(
        regular = cms.PSet(
            posX = cms.double(0.60),
            posY = cms.double(0.11),
            sizeX = cms.double(0.29),
            sizeY = cms.double(0.25),
            header = cms.string(''),
            option = cms.string('brNDC'),
            borderSize = cms.int32(0),
            fillColor = cms.int32(0)
        )
    ),

    drawOptionEntries = cms.PSet(
        frQCDmuEnriched = copy.deepcopy(drawOption_lightBlue_eff),
        frQCDdiJetLeadJet = copy.deepcopy(drawOption_red_eff),
        frQCDdiJetSecondLeadJet = copy.deepcopy(drawOption_orange_eff),
        frWplusJets = copy.deepcopy(drawOption_green_eff),
        ##frGammaPlusJets = copy.deepcopy(drawOption_yellow_eff)
    ),

    drawJobs = drawFakeRateHistConfiguratorZtoMuTau_signal.configure()
)

drawTauIdEffHistConfiguratorZtoMuTau = drawFakeRateHistConfigurator(
    template = cms.PSet(
        xAxis = cms.string('unlabeled'),
        yAxis = cms.string('numEntries_linear'),
        #yAxis = cms.string('numEntries_log'),
        legend = cms.string('regular'),
        labels = cms.vstring('mcNormScale')
    ),
    dqmDirectories = dqmDirectories,
    legendEntries = legendEntries,
    frTypes = [
        "tauIdDiscr",
        "tauIdEff"
    ]
)

drawTauIdEffHistConfiguratorZtoMuTau.addProcess("Ztautau", processZtoMuTau_Ztautau.config_dqmHistPlotter.dqmDirectory)

drawTauIdEffHistConfiguratorZtoMuTau.addPlots(
    afterCut = evtSelDiMuPairZmumuHypothesisVeto,
    plots = bgEstFakeRatePlots
)

process.plotTauIdEffZtoMuTau = process.plotBgEstFakeRateZtoMuTau_signal.clone(
    processes = cms.PSet(
        tauIdDiscr = cms.PSet(
            #dqmDirectory = cms.string(dqmDirectories["tauIdDiscr"]),
            dqmDirectory = cms.string(''),
            legendEntry = cms.string('tauIdDiscr'),
            type = cms.string('smMC')
        ),
        tauIdEff = cms.PSet(
            #dqmDirectory = cms.string(dqmDirectories["tauIdEff"]),
            dqmDirectory = cms.string(''),
            legendEntry = cms.string('frQCDmuEnriched'),
            type = cms.string('smMC')
        )
    ),

    drawOptionEntries = cms.PSet(
        tauIdDiscr = copy.deepcopy(drawOption_black_separate),
        tauIdEff = copy.deepcopy(drawOption_red_separate)
    ),

    drawJobs = drawTauIdEffHistConfiguratorZtoMuTau.configure(),

    #outputFileName = cms.string('plotsBgEstFakeRateZtoMuTau_tauIdEff.ps')
    indOutputFileName = cms.string('plotBgEstFakeRateZtoMuTau_tauIdEff_#PLOT#.eps')
)
#--------------------------------------------------------------------------------

# import utility function for fake-rate technique
from TauAnalysis.BgEstimationTools.tools.fakeRateAnalysisTools import enableFakeRates_makeZtoMuTauPlots
enableFakeRates_makeZtoMuTauPlots(process)

#--------------------------------------------------------------------------------
process.dumpZtoMuTau_frUnweighted = cms.EDAnalyzer("DQMDumpFilterStatisticsTables",
    dqmDirectories = cms.PSet(
        Ztautau = cms.string('tauFakeRate/harvested/Ztautau/zMuTauAnalyzer_frUnweighted/FilterStatistics'),
        Zmumu = cms.string('tauFakeRate/harvested/Zmumu/zMuTauAnalyzer_frUnweighted/FilterStatistics/'),
        WplusJets = cms.string('tauFakeRate/harvested/WplusJets/zMuTauAnalyzer_frUnweighted/FilterStatistics/'),
        QCD = cms.string('tauFakeRate/harvested/qcdSum/zMuTauAnalyzer_frUnweighted/FilterStatistics/'),
        TTplusJets = cms.string('tauFakeRate/harvested/TTplusJets/zMuTauAnalyzer_frUnweighted/FilterStatistics')
    ),
    columnsSummaryTable = cms.vstring("Passed", "cumul. Efficiency", "margin. Efficiency", "indiv. Efficiency")
)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
from TauAnalysis.BgEstimationTools.tools.fakeRateAnalysisTools import makeDataBinningDumpSequence
from TauAnalysis.BgEstimationTools.tools.fakeRateAnalysisTools import makeFilterStatTableDumpSequence

processSubDirectories = dict()
processSubDirectories["Ztautau"] = processZtoMuTau_Ztautau.config_dqmHistPlotter.dqmDirectory
processSubDirectories["Zmumu"] = processZtoMuTau_Zmumu.config_dqmHistPlotter.dqmDirectory
processSubDirectories["WplusJets"] = processZtoMuTau_WplusJets.config_dqmHistPlotter.dqmDirectory
processSubDirectories["TTplusJets"] = processZtoMuTau_TTplusJets.config_dqmHistPlotter.dqmDirectory
processSubDirectories["QCD"] = cms.string('harvested/qcdSum')
processSubDirectories["smBackgroundSum"] = cms.string('harvested/smBgSum')

dqmDirectory_frDataBinning = 'tauFakeRate/#PROCESSDIR#/#FAKERATEDIR#/afterEvtSelDiMuPairZmumuHypothesisVeto/dataBinningResults/'
dqmDirectory_frFilterStatTable = 'tauFakeRate/#PROCESSDIR#/#FAKERATEDIR#/FilterStatistics/'

frSubDirectories = dict()
frSubDirectories["frQCDmuEnriched"] = "zMuTauAnalyzer_qcdMuEnriched"
frSubDirectories["frQCDdiJetLeadJet"] = "zMuTauAnalyzer_qcdDiJetLeadJet"
frSubDirectories["frQCDdiJetSecondLeadJet"] = "zMuTauAnalyzer_qcdDiJetSecondLeadJet"
frSubDirectories["frWplusJets"] = "zMuTauAnalyzer_WplusJets"
##frSubDirectories["frGammaPlusJets"] = "zMuTauAnalyzer_GammaPlusJets"
frSubDirectories["frSysUncertainty"] = "frSysUncertainty"

process.frDataBinningDumpSequence = cms.Sequence(makeDataBinningDumpSequence(process, dqmDirectory_frDataBinning, processSubDirectories, frSubDirectories, "fr"))
process.frFilterStatTableDumpSequence = cms.Sequence(makeFilterStatTableDumpSequence(process, dqmDirectory_frFilterStatTable, processSubDirectories, frSubDirectories, "fr"))

dqmDirectory_discrDataBinning = 'tauIdDiscr/#PROCESSDIR#/zMuTauAnalyzer/afterEvtSelDiMuPairZmumuHypothesisVeto/dataBinningResults/'
dqmDirectory_discrFilterStatTable = 'tauIdDiscr/#PROCESSDIR#/zMuTauAnalyzer/FilterStatistics/'

discrSubDirectories = dict()
discrSubDirectories["discr"] = "zMuTauAnalyzer"

process.discrDataBinningDumpSequence = cms.Sequence(makeDataBinningDumpSequence(process, dqmDirectory_discrDataBinning, processSubDirectories, discrSubDirectories, "discr"))
process.discrFilterStatTableDumpSequence = cms.Sequence(makeFilterStatTableDumpSequence(process, dqmDirectory_discrFilterStatTable, processSubDirectories, discrSubDirectories, "discr"))

process.dumpBgEstFakeRateZtoMuTau = cms.Sequence(
    process.frDataBinningDumpSequence + process.discrDataBinningDumpSequence
   + process.frFilterStatTableDumpSequence + process.discrFilterStatTableDumpSequence
)

#--------------------------------------------------------------------------------

##process.loadBgEstFakeRateZtoMuTau = cms.EDAnalyzer("DQMFileLoader",
##    all = cms.PSet(
##        inputFileNames = cms.vstring('rfio:/castor/cern.ch/user/v/veelken/CMSSW_3_3_x/bgEstPlots/ZtoMuTau_frSimple/7TeV/plotsZtoMuTau_bgEstFakeRate.root'),
##        scaleFactor = cms.double(1.),
##        dqmDirectory_store = cms.string('')
##    )
##)

process.saveBgEstFakeRateZtoMuTau = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('plotsZtoMuTau_bgEstFakeRate.root'),
    outputCommands = cms.vstring('drop tauIdDiscr/*')
)

process.dumpDQMStore = cms.EDAnalyzer("DQMStoreDump")

process.makeBgEstFakeRateZtoMuTauPlots = cms.Sequence(
    process.loadBgEstFakeRateZtoMuTau_tauIdDiscr
   + process.loadBgEstFakeRateZtoMuTau_tauFakeRate
   + process.addBgEstFakeRateZtoMuTau_tauFakeRate
   + process.compErrorBandsBgEstFakeRateZtoMuTau
   ## process.loadBgEstFakeRateZtoMuTau
   + process.dumpDQMStore
   + process.dumpZtoMuTau_frUnweighted
   + process.dumpBgEstFakeRateZtoMuTau
   + process.plotBgEstFakeRateZtoMuTau_background + process.plotBgEstFakeRateZtoMuTau_signal
   + process.plotTauIdEffZtoMuTau
   + process.saveBgEstFakeRateZtoMuTau
)

process.p = cms.Path(process.makeBgEstFakeRateZtoMuTauPlots)

# print-out all python configuration parameter information
#print process.dumpPython()
