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

from TauAnalysis.Configuration.plotZtoMuTau_processes_cfi import *
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
        inputFileNames = cms.vstring('rfio:/castor/cern.ch/user/v/veelken/bgEstPlots/ZtoMuTau/plotsZtoMuTau_all_shrinkingCone.root'),
        scaleFactor = cms.double(1.),
        dqmDirectory_store = cms.string('tauIdDiscr')
    )
)

process.loadBgEstFakeRateZtoMuTau_tauFakeRate = copy.deepcopy(loadZtoMuTau)
reconfigDQMFileLoader(
    process.loadBgEstFakeRateZtoMuTau_tauFakeRate,
    dqmDirectory = 'tauFakeRate/#PROCESSDIR#'
)
process.loadBgEstFakeRateZtoMuTau_tauFakeRate.inputFilePath = cms.string("rfio:/castor/cern.ch/user/v/veelken/bgEstPlots/ZtoMuTau/")

process.addBgEstFakeRateZtoMuTau_qcdSum_tauFakeRate = cms.EDAnalyzer("DQMHistAdder",
    qcdSum = cms.PSet(
        dqmDirectories_input = cms.vstring(
            'tauFakeRate/harvested/InclusivePPmuX/zMuTauAnalyzer',
            'tauFakeRate/harvested/PPmuXptGt20/zMuTauAnalyzer'
        ),
        dqmDirectory_output = cms.string('tauFakeRate/harvested/qcdSum/zMuTauAnalyzer')
    )                          
)

process.addBgEstFakeRateZtoMuTau_tauFakeRate = cms.Sequence( process.addBgEstFakeRateZtoMuTau_qcdSum_tauFakeRate )
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
drawFakeRateHistConfiguratorZtoMuTau = drawFakeRateHistConfigurator(
    template = cms.PSet(
        plots = cms.PSet(  
            dqmMonitorElements = cms.vstring(''),
            processes = cms.vstring(
                'tauIdDiscr',
                'tauFakeRate'
            )
        ),
        xAxis = cms.string('unlabeled'),
        yAxis = cms.string('numEntries_linear'),
        legend = cms.string('regular'),
        labels = cms.vstring('mcNormScale'),                   
        drawOptionSet = cms.string('default')
    ),
    dqmDirectory_prefix = cms.string('#PROCESSDIR#'),
    dqmDirectory_suffix = cms.string('zMuTauAnalyzer')
)

drawFakeRateHistConfiguratorZtoMuTau.addProcess("Ztautau", processZtoMuTau_Ztautau.config_dqmHistPlotter.dqmDirectory)
#drawFakeRateHistConfiguratorZtoMuTau.addProcess("ZtautauPlusJets", processZtoMuTau_ZtautauPlusJets.config_dqmHistPlotter.dqmDirectory)
drawFakeRateHistConfiguratorZtoMuTau.addProcess("Zmumu", processZtoMuTau_Zmumu.config_dqmHistPlotter.dqmDirectory)
#drawFakeRateHistConfiguratorZtoMuTau.addProcess("ZmumuPlusJets", processZtoMuTau_ZmumuPlusJets.config_dqmHistPlotter.dqmDirectory)
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
            dqmDirectory = cms.string('tauIdDiscr'),
            legendEntry = cms.string('tauIdDiscr'),
            type = cms.string('smMC')
        ),
        tauFakeRate = cms.PSet(
            dqmDirectory = cms.string('tauFakeRate'),
            legendEntry = cms.string('tauFakeRate'),
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

    drawOptionSets = cms.PSet(
        default = cms.PSet(
            tauIdDiscr = copy.deepcopy(drawOption_darkBlue_eff),
            tauFakeRate = copy.deepcopy(drawOption_red_eff)
        )
    ),
                              
    drawJobs = drawFakeRateHistConfiguratorZtoMuTau.configure(),

    canvasSizeX = cms.int32(800),
    canvasSizeY = cms.int32(640),                         

    outputFilePath = cms.string('./plots/'),
    #outputFileName = cms.string('plotsBgEstFakeRateZtoMuTau.ps')
    indOutputFileName = cms.string('plotBgEstFakeRateZtoMuTau_#PLOT#.png')
)
#--------------------------------------------------------------------------------

# import utility function to enable factorization
from TauAnalysis.Configuration.factorizationTools import enableFactorization_makeZtoMuTauPlots
enableFactorization_makeZtoMuTauPlots(process,
    dqmDirectoryIn_InclusivePPmuX = 'tauFakeRate/harvested/InclusivePPmuX/zMuTauAnalyzer',
    dqmDirectoryOut_InclusivePPmuX = 'tauFakeRate/harvested/InclusivePPmuX_factorized/zMuTauAnalyzer',
    dqmDirectoryIn_PPmuXptGt20 = 'tauFakeRate/harvested/PPmuXptGt20/zMuTauAnalyzer',
    dqmDirectoryOut_PPmuXptGt20 = 'tauFakeRate/harvested/PPmuXptGt20_factorized/zMuTauAnalyzer',
    seqName_addZtoMuTau_qcdSum = "addBgEstFakeRateZtoMuTau_qcdSum_tauFakeRate",
    seqName_addZtoMuTau = "addBgEstFakeRateZtoMuTau_tauFakeRate")

#process.dumpZtoMuTau = cms.EDAnalyzer("DQMStoreDump")

process.dumpZtoMuTau = cms.EDAnalyzer("DQMDumpFilterStatisticsTables",
    dqmDirectories = cms.PSet(
        Ztautau = cms.string('tauFakeRate/harvested/Ztautau/zMuTauAnalyzer/FilterStatistics'),
        Zmumu = cms.string('tauFakeRate/harvested/Zmumu/zMuTauAnalyzer/FilterStatistics/'),
        #ZtautauPlusJets = cms.string('tauFakeRate/harvested/ZtautauPlusJets/zMuTauAnalyzer/FilterStatistics'),
        #ZmumuPlusJets = cms.string('tauFakeRate/harvested/ZmumuPlusJets/zMuTauAnalyzer/FilterStatistics/'),
        WplusJets = cms.string('tauFakeRate/harvested/WplusJets/zMuTauAnalyzer/FilterStatistics/'),
        QCD = cms.string('tauFakeRate/harvested/qcdSum/zMuTauAnalyzer/FilterStatistics/'),
        TTplusJets = cms.string('tauFakeRate/harvested/TTplusJets/zMuTauAnalyzer/FilterStatistics')
    ),
    columnsSummaryTable = cms.vstring("Passed", "cumul. Efficiency", "margin. Efficiency", "indiv. Efficiency")
)
 
process.saveBgEstFakeRateZtoMuTau = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('plotsZtoMuTau_bgEstFakeRate.root')
)

process.makeBgEstFakeRateZtoMuTauPlots = cms.Sequence(
    process.loadBgEstFakeRateZtoMuTau_tauIdDiscr
   + process.loadBgEstFakeRateZtoMuTau_tauFakeRate
   + process.addBgEstFakeRateZtoMuTau_tauFakeRate
   + process.dumpZtoMuTau
   + process.saveBgEstFakeRateZtoMuTau
   + process.plotBgEstFakeRateZtoMuTau
)

process.p = cms.Path(process.makeBgEstFakeRateZtoMuTauPlots)

# print-out all python configuration parameter information
#print process.dumpPython()
