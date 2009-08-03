import FWCore.ParameterSet.Config as cms
import copy

#
# Plot histograms for Z --> e + mu channel
#
# Author: Christian Veelken, UC Davis
#

from TauAnalysis.Configuration.plotZtoElecMu_processes_cfi import *
from TauAnalysis.Configuration.plotZtoElecMu_drawJobs_cfi import *
from TauAnalysis.DQMTools.plotterStyleDefinitions_cfi import *

loadZtoElecMu = cms.EDAnalyzer("DQMFileLoader",
    ZtautauPlusJets = copy.deepcopy(processZtoElecMu_ZtautauPlusJets.config_dqmFileLoader),                            
    Ztautau = copy.deepcopy(processZtoElecMu_Ztautau.config_dqmFileLoader),
    ZmumuPlusJets = copy.deepcopy(processZtoElecMu_ZmumuPlusJets.config_dqmFileLoader),
    Zmumu = copy.deepcopy(processZtoElecMu_Zmumu.config_dqmFileLoader),
    ZeePlusJets = copy.deepcopy(processZtoElecMu_ZeePlusJets.config_dqmFileLoader),                           
    Zee = copy.deepcopy(processZtoElecMu_Zee.config_dqmFileLoader),
    WplusJets = copy.deepcopy(processZtoElecMu_WplusJetsSum.config_dqmFileLoader),
    TTplusJets = copy.deepcopy(processZtoElecMu_TTplusJetsSum.config_dqmFileLoader),
    InclusivePPmuX = copy.deepcopy(processZtoElecMu_InclusivePPmuX.config_dqmFileLoader),
    PPmuXptGt20 = copy.deepcopy(processZtoElecMu_PPmuXptGt20Sum.config_dqmFileLoader)                
)

addZtoElecMu_qcdSum = cms.EDAnalyzer("DQMHistAdder",
    qcdSum = cms.PSet(
        dqmDirectories_input = cms.vstring(
            'InclusivePPmuX',
            'PPmuXptGt20'
        ),
        dqmDirectory_output = cms.string('qcdSum')
    )
)

addZtoElecMu_smSum = cms.EDAnalyzer("DQMHistAdder",
    smSum = cms.PSet(
        dqmDirectories_input = cms.vstring(
            #'ZtautauPlusJets',
            'Ztautau',
            'ZmumuPlusJets',
            #'Zmumu',
            'ZeePlusJets',
            #'Zee',
            'WplusJets',
            'TTplusJets',
            'qcdSum'
        ),
        dqmDirectory_output = cms.string('smSum')
    )
)

addZtoElecMu = cms.Sequence(addZtoElecMu_qcdSum + addZtoElecMu_smSum)

plotZtoElecMu = cms.EDAnalyzer("DQMHistPlotter",
    processes = cms.PSet(
        ZtautauPlusJets = copy.deepcopy(processZtoElecMu_ZtautauPlusJets.config_dqmHistPlotter),
        Ztautau = copy.deepcopy(processZtoElecMu_Ztautau.config_dqmHistPlotter),
        ZmumuPlusJets = copy.deepcopy(processZtoElecMu_ZmumuPlusJets.config_dqmHistPlotter),
        Zmumu = copy.deepcopy(processZtoElecMu_Zmumu.config_dqmHistPlotter),
        ZeePlusJets = copy.deepcopy(processZtoElecMu_ZeePlusJets.config_dqmHistPlotter),
        Zee = copy.deepcopy(processZtoElecMu_Zee.config_dqmHistPlotter),
        WplusJets = copy.deepcopy(processZtoElecMu_WplusJets.config_dqmHistPlotter),
        TTplusJets = copy.deepcopy(processZtoElecMu_TTplusJets.config_dqmHistPlotter), 
        InclusivePPmuX = copy.deepcopy(processZtoElecMu_InclusivePPmuX.config_dqmHistPlotter),
        PPmuXptGt20 = copy.deepcopy(processZtoElecMu_PPmuXptGt20.config_dqmHistPlotter),
        qcdSum = cms.PSet(
            dqmDirectory = cms.string('qcdSum'),
            legendEntry = cms.string('QCD'),
            type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
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
            ZtautauPlusJets = copy.deepcopy(drawOption_ZtautauPlusJets),
            Ztautau = copy.deepcopy(drawOption_Ztautau),
            ZmumuPlusJets = copy.deepcopy(drawOption_ZmumuPlusJets),
            Zmumu = copy.deepcopy(drawOption_Zmumu),
            ZeePlusJets = copy.deepcopy(drawOption_ZeePlusJets),
            Zee = copy.deepcopy(drawOption_Zee),
            WplusJets = copy.deepcopy(drawOption_WplusJets),
            TTplusJets = copy.deepcopy(drawOption_TTplusJets),
            qcdSum = copy.deepcopy(drawOption_QCD)
       )
    ),

    drawJobs = drawJobConfigurator_ZtoElecMu.configure(),

    canvasSizeX = cms.int32(800),
    canvasSizeY = cms.int32(640),                         

    outputFilePath = cms.string('./plots/'),
    #outputFileName = cms.string('plotsZtoElecMu.ps')
    indOutputFileName = cms.string('plotZtoElecMu_#PLOT#.png')
)

saveZtoElecMu = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('plotsZtoElecMu_all.root')
)
