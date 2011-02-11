import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------
# Plot histograms for W --> tau-jet + nu channel
#--------------------------------------------------------------------------------

# uncomment next line to make plots for 7 TeV centre-of-mass energy
from TauAnalysis.Configuration.plotWtoTauNu_processes_7TeV_cfi import *

from TauAnalysis.Configuration.plotWtoTauNu_drawJobs_cfi import *
from TauAnalysis.DQMTools.plotterStyleDefinitions_cfi import *

loadWtoTauNu = cms.EDAnalyzer("DQMFileLoader",
#    Wtaunu = copy.deepcopy(processWtoTauNu_WtaunuSum.config_dqmFileLoader),
#    qcd_W = copy.deepcopy(processWtoTauNu_qcd_WSum.config_dqmFileLoader),
#    Wmunu = copy.deepcopy(processWtoTauNu_WmunuSum.config_dqmFileLoader),
    Wenu = copy.deepcopy(processWtoTauNu_WenuSum.config_dqmFileLoader),
#    ZplusJets = copy.deepcopy(processWtoTauNu_ZplusJetsSum.config_dqmFileLoader)
)

#addWtoTauNu_qcdSum = cms.EDAnalyzer("DQMHistAdder",
#                                 qcd = cms.PSet(
#        dqmDirectories_input = cms.vstring(
#            'harvested/qcd_W/wTauNuAnalyzer'
#            ),
#        dqmDirectory_output = cms.string('qcd/wTauNuAnalyzer')
#        )
#                                 )

addWtoTauNu_smSum = cms.EDAnalyzer("DQMHistAdder",
                                   smSum = cms.PSet(
        dqmDirectories_input = cms.vstring(
            'harvested/Wtaunu/wTauNuAnalyzer',
            'harvested/Wmunu/wTauNuAnalyzer',
            'harvested/Wenu/wTauNuAnalyzer',
            'harvested/ZplusJets/wTauNuAnalyzer',
            ),
        dqmDirectory_output = cms.string('smSum/wTauNuAnalyzer')
        )
                                   )



addWtoTauNu = cms.Sequence(addWtoTauNu_smSum)

plotWtoTauNu = cms.EDAnalyzer("DQMHistPlotter",
                              processes = cms.PSet(
        Wtaunu = copy.deepcopy(processWtoTauNu_Wtaunu.config_dqmHistPlotter),
       # qcd_W = copy.deepcopy(processWtoTauNu_qcd_W.config_dqmHistPlotter),        
        Wmunu = copy.deepcopy(processWtoTauNu_Wmunu.config_dqmHistPlotter),
        Wenu = copy.deepcopy(processWtoTauNu_Wenu.config_dqmHistPlotter),
        ZplusJets = copy.deepcopy(processWtoTauNu_ZplusJets.config_dqmHistPlotter),
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
            Wtaunu = copy.deepcopy(drawOption_Wtaunu),
            qcd_W = copy.deepcopy(drawOption_QCD),
            Wmunu = copy.deepcopy(drawOption_Wmunu),
            Wenu = copy.deepcopy(drawOption_Wenu),
            ZplusJets = copy.deepcopy(drawOption_ZplusJets),
            )
    ),
                              
    drawJobs = drawJobConfigurator_WtoTauNu.configure(),

    canvasSizeX = cms.int32(800),
    canvasSizeY = cms.int32(640),                         

    outputFilePath = cms.string('./plots/'),
    #outputFileName = cms.string('plotsWtoTauNu.ps')
    indOutputFileName = cms.string('plotWtoTauNu_#PLOT#.png')
)


saveWtoTauNu = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('plotsWtoTauNu_all.root')
)



  
