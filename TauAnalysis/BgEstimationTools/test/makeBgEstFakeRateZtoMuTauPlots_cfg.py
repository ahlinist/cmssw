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
from TauAnalysis.Configuration.plotZtoMuTau_processes_10TeV_cfi import *
# uncomment next line to make plots for 7 TeV centre-of-mass energy
#from TauAnalysis.Configuration.plotZtoMuTau_processes_7TeV_cfi import *
from TauAnalysis.Configuration.plotZtoMuTau_drawJobs_cfi import *
from TauAnalysis.Configuration.plotZtoMuTau_cff import loadZtoMuTau
from TauAnalysis.Configuration.analyzeZtoMuTau_cfi import *

from TauAnalysis.DQMTools.plotterStyleDefinitions_cfi import *

from TauAnalysis.BgEstimationTools.tools.drawFakeRateHistConfigurator import drawFakeRateHistConfigurator
from TauAnalysis.BgEstimationTools.tools.fakeRateTools import reconfigDQMFileLoader

process = cms.Process('makeBgEstFakeRateZtoMuTauPlots')

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

#--------------------------------------------------------------------------------
process.loadBgEstFakeRateZtoMuTau_tauIdDiscr = cms.EDAnalyzer("DQMFileLoader",
    tauIdDiscr = cms.PSet(
        inputFileNames = cms.vstring('rfio:/castor/cern.ch/user/v/veelken/CMSSW_3_1_2/plots/ZtoMuTau/10TeV/plotsZtoMuTau_all.root'),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('tauIdDiscr')
    )
)

process.loadBgEstFakeRateZtoMuTau_tauFakeRate = copy.deepcopy(loadZtoMuTau)
reconfigDQMFileLoader(
    process.loadBgEstFakeRateZtoMuTau_tauFakeRate,
    dqmDirectory = 'tauFakeRate/#PROCESSDIR#'
)
process.loadBgEstFakeRateZtoMuTau_tauFakeRate.inputFilePath = cms.string("rfio:/castor/cern.ch/user/v/veelken/CMSSW_3_1_2/bgEstPlots/ZtoMuTau_frCDF/10TeV/")

process.addBgEstFakeRateZtoMuTau_qcdSum_tauFakeRate = cms.EDAnalyzer("DQMHistAdder",
    qcdSum = cms.PSet(
        dqmDirectories_input = cms.vstring(
            'tauFakeRate/harvested/InclusivePPmuX/',
            'tauFakeRate/harvested/PPmuXptGt20/'
        ),
        dqmDirectory_output = cms.string('tauFakeRate/harvested/qcdSum/')
    )                          
)

process.addBgEstFakeRateZtoMuTau_tauFakeRate = cms.Sequence( process.addBgEstFakeRateZtoMuTau_qcdSum_tauFakeRate )
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
dqmDirectories = dict()
dqmDirectories["tauIdDiscr"] = 'tauIdDiscr' + '/' + '#PROCESSDIR#' + '/' + 'zMuTauAnalyzer'
dqmDirectories["frQCDmuEnriched"] = 'tauFakeRate' + '/' + '#PROCESSDIR#' + '/' + 'zMuTauAnalyzer_qcdMuEnriched'
dqmDirectories["frQCDdiJetLeadJet"] = 'tauFakeRate' + '/' + '#PROCESSDIR#' + '/' + 'zMuTauAnalyzer_qcdDiJetLeadJet'
dqmDirectories["frQCDdiJetSecondLeadJet"] = 'tauFakeRate' + '/' + '#PROCESSDIR#' + '/' + 'zMuTauAnalyzer_qcdDiJetSecondLeadJet'
dqmDirectories["frWplusJets"] = 'tauFakeRate' + '/' + '#PROCESSDIR#' + '/' + 'zMuTauAnalyzer_WplusJets'
##dqmDirectories["frGammaPlusJets"] = 'tauFakeRate' + '/' + '#PROCESSDIR#' + '/' + 'zMuTauAnalyzer_gammaPlusJets'

legendEntries = dict()
legendEntries["tauIdDiscr"] = "Final Analysis"
legendEntries["frQCDmuEnriched"] = "QCD fr., #mu enriched."
legendEntries["frQCDdiJetLeadJet"] = "QCD fr., lead. Jet"
legendEntries["frQCDdiJetSecondLeadJet"] = "QCD fr., next-to-lead. Jet"
legendEntries["frWplusJets"] = "W + Jets fr."
##legendEntries["frGammaPlusJets"] = "#gamma + Jets fr."

drawFakeRateHistConfiguratorZtoMuTau = drawFakeRateHistConfigurator(
    template = cms.PSet(
        xAxis = cms.string('unlabeled'),
        yAxis = cms.string('numEntries_linear'),
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
        "frWplusJets"
        ##"frGammaPlusJets"
    ]
)

drawFakeRateHistConfiguratorZtoMuTau.addProcess("Ztautau", processZtoMuTau_Ztautau.config_dqmHistPlotter.dqmDirectory)
drawFakeRateHistConfiguratorZtoMuTau.addProcess("Zmumu", processZtoMuTau_Zmumu.config_dqmHistPlotter.dqmDirectory)
drawFakeRateHistConfiguratorZtoMuTau.addProcess("WplusJets", processZtoMuTau_WplusJets.config_dqmHistPlotter.dqmDirectory)
drawFakeRateHistConfiguratorZtoMuTau.addProcess("TTplusJets", processZtoMuTau_TTplusJets.config_dqmHistPlotter.dqmDirectory)
drawFakeRateHistConfiguratorZtoMuTau.addProcess("QCD", cms.string('harvested/qcdSum'))

drawFakeRateHistConfiguratorZtoMuTau.addPlots(
    afterCut = evtSelDiTauCandidateForMuTauPzetaDiff,
    plots = [
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
)

process.plotBgEstFakeRateZtoMuTau = cms.EDAnalyzer("DQMHistPlotter",
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
        ##),
        ##frGammaPlusJets = cms.PSet(
        ##    #dqmDirectory = cms.string(dqmDirectories["frGammaPlusJets"]),
        ##    dqmDirectory = cms.string(''),
        ##    legendEntry = cms.string('frGammaPlusJets'),
        ##    type = cms.string('smMC')
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
        tauIdDiscr = copy.deepcopy(drawOption_black_separate),
        frQCDmuEnriched = copy.deepcopy(drawOption_lightBlue_separate),
        frQCDdiJetLeadJet = copy.deepcopy(drawOption_red_separate),
        frQCDdiJetSecondLeadJet = copy.deepcopy(drawOption_orange_separate),
        frWplusJets = copy.deepcopy(drawOption_green_separate),
        ##frGammaPlusJets = copy.deepcopy(drawOption_yellow_separate)
    ),
                              
    drawJobs = drawFakeRateHistConfiguratorZtoMuTau.configure(),

    canvasSizeX = cms.int32(800),
    canvasSizeY = cms.int32(640),                         

    outputFilePath = cms.string('./plots/'),
    #outputFileName = cms.string('plotsBgEstFakeRateZtoMuTau.ps')
    indOutputFileName = cms.string('plotBgEstFakeRateZtoMuTau_#PLOT#.png')
)
#--------------------------------------------------------------------------------

# import utility function for fake-rate technique
from TauAnalysis.BgEstimationTools.tools.fakeRateTools import enableFakeRates_makeZtoMuTauPlots
enableFakeRates_makeZtoMuTauPlots(process)
 
process.saveBgEstFakeRateZtoMuTau = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('plotsZtoMuTau_bgEstFakeRate.root')
)

process.dumpDQMStore = cms.EDAnalyzer("DQMStoreDump")

process.makeBgEstFakeRateZtoMuTauPlots = cms.Sequence(
    process.loadBgEstFakeRateZtoMuTau_tauIdDiscr
   + process.loadBgEstFakeRateZtoMuTau_tauFakeRate
   + process.dumpDQMStore
   + process.addBgEstFakeRateZtoMuTau_tauFakeRate
   #+ process.saveBgEstFakeRateZtoMuTau
   + process.plotBgEstFakeRateZtoMuTau
)

process.p = cms.Path(process.makeBgEstFakeRateZtoMuTauPlots)

# print-out all python configuration parameter information
print process.dumpPython()
