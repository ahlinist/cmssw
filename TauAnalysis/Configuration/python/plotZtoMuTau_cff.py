import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------
#
# Plot histograms for Z --> mu + tau-jet channel
#
# Author: Christian Veelken, UC Davis
#
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.plotZtoMuTau_processes_cfi import *
from TauAnalysis.Configuration.plotZtoMuTau_drawJobs_cfi import *
from TauAnalysis.DQMTools.plotterStyleDefinitions_cfi import *

loadZtoMuTau = cms.EDAnalyzer("DQMFileLoader",
    Ztautau = copy.deepcopy(processZtoMuTau_ZtautauSum.config_dqmFileLoader),
    #Ztautau_from_selZmumu = copy.deepcopy(processZtoMuTau_Ztautau_from_selZmumuSum.config_dqmFileLoader),                
    Zmumu = copy.deepcopy(processZtoMuTau_ZmumuSum.config_dqmFileLoader),
    ZeePlusJets = copy.deepcopy(processZtoMuTau_ZeePlusJetsSum.config_dqmFileLoader),
    ZmumuPlusJets = copy.deepcopy(processZtoMuTau_ZmumuPlusJetsSum.config_dqmFileLoader),
    ZtautauPlusJets = copy.deepcopy(processZtoMuTau_ZtautauPlusJetsSum.config_dqmFileLoader),
    WplusJets = copy.deepcopy(processZtoMuTau_WplusJetsSum.config_dqmFileLoader),
    InclusivePPmuX = copy.deepcopy(processZtoMuTau_InclusivePPmuX.config_dqmFileLoader),
    PPmuXptGt20 = copy.deepcopy(processZtoMuTau_PPmuXptGt20Sum.config_dqmFileLoader),
    TTplusJets = copy.deepcopy(processZtoMuTau_TTplusJetsSum.config_dqmFileLoader)
)

addZtoMuTau_qcdSum = cms.EDAnalyzer("DQMHistAdder",
    qcdSum = cms.PSet(
        dqmDirectories_input = cms.vstring(
            'harvested/InclusivePPmuX/zMuTauAnalyzer',
            'harvested/PPmuXptGt20/zMuTauAnalyzer'
        ),
        dqmDirectory_output = cms.string('harvested/qcdSum/zMuTauAnalyzer')
    )                          
)

addZtoMuTau_smSum = cms.EDAnalyzer("DQMHistAdder",
    smSum = cms.PSet(
        dqmDirectories_input = cms.vstring(
            'harvested/Ztautau/zMuTauAnalyzer',
            'harvested/Zmumu/zMuTauAnalyzer',
            'harvested/ZeePlusJets/zMuTauAnalyzer',
            #'harvested/ZmumuPlusJets/zMuTauAnalyzer',
            #'harvested/ZtautauPlusJets/zMuTauAnalyzer',
            'harvested/WplusJets/zMuTauAnalyzer',
            'harvested/TTplusJets/zMuTauAnalyzer',
            'harvested/qcdSum/zMuTauAnalyzer'
        ),
        dqmDirectory_output = cms.string('harvested/smSum/zMuTauAnalyzer')
    )
)

addZtoMuTau = cms.Sequence(addZtoMuTau_qcdSum + addZtoMuTau_smSum)

plotZtoMuTau = cms.EDAnalyzer("DQMHistPlotter",
    processes = cms.PSet(
        Ztautau = copy.deepcopy(process_Ztautau.config_dqmHistPlotter),
        Zmumu = copy.deepcopy(process_Zmumu.config_dqmHistPlotter),
        ZeePlusJets = copy.deepcopy(process_ZeePlusJets.config_dqmHistPlotter),
        #ZmumuPlusJets = copy.deepcopy(process_ZmumuPlusJets.config_dqmHistPlotter),
        #ZtautauPlusJets = copy.deepcopy(process_ZtautauPlusJets.config_dqmHistPlotter),
        WplusJets = copy.deepcopy(process_WplusJets.config_dqmHistPlotter),
        TTplusJets = copy.deepcopy(process_TTplusJets.config_dqmHistPlotter),
        InclusivePPmuX = copy.deepcopy(process_InclusivePPmuX.config_dqmHistPlotter),
        PPmuXptGt20 = copy.deepcopy(process_PPmuXptGt20.config_dqmHistPlotter),
        qcdSum = cms.PSet(
            dqmDirectory = cms.string('harvested/qcdSum'),
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
            Ztautau = copy.deepcopy(drawOption_Ztautau),
            Zmumu = copy.deepcopy(drawOption_Zmumu),
            ZeePlusJets = copy.deepcopy(drawOption_ZeePlusJets),
            #ZmumuPlusJets = copy.deepcopy(drawOption_ZmumuPlusJets),
            #ZtautauPlusJets = copy.deepcopy(drawOption_ZtautauPlusJets),
            WplusJets = copy.deepcopy(drawOption_WplusJets),
            TTplusJets = copy.deepcopy(drawOption_TTplusJets),
            qcdSum = copy.deepcopy(drawOption_QCD)
        )
    ),
                              
    drawJobs = drawJobConfigurator_ZtoMuTau.configure(),

    canvasSizeX = cms.int32(800),
    canvasSizeY = cms.int32(640),                         

    outputFilePath = cms.string('./plots/'),
    #outputFileName = cms.string('plotsZtoMuTau.ps')
    indOutputFileName = cms.string('plotZtoMuTau_#PLOT#.png')
)

saveZtoMuTau = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('plotsZtoMuTau_all.root')
)


  
