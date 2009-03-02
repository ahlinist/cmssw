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
  Ztautau = copy.deepcopy(processZtoElecMu_Ztautau.config_dqmFileLoader),
  Zee = copy.deepcopy(processZtoElecMu_Zee.config_dqmFileLoader),
  Zmumu = copy.deepcopy(processZtoElecMu_Zmumu.config_dqmFileLoader),                         
  WplusJets = copy.deepcopy(processZtoElecMu_WplusJets.config_dqmFileLoader),
  QCD_BCtoE_Pt20to30 = copy.deepcopy(processZtoElecMu_QCD_BCtoE_Pt20to30.config_dqmFileLoader),
  QCD_BCtoE_Pt30to80 = copy.deepcopy(processZtoElecMu_QCD_BCtoE_Pt30to80.config_dqmFileLoader),
  InclusivePPmuX = copy.deepcopy(processZtoElecMu_InclusivePPmuX.config_dqmFileLoader),
  PPmuXptGt20 = copy.deepcopy(processZtoElecMu_PPmuXptGt20.config_dqmFileLoader)                
)

addZtoElecMu_qcdSum = cms.EDAnalyzer("DQMHistAdder",
  qcdSum = cms.PSet(
    dqmDirectories_input = cms.vstring( 'QCD_BCtoE_Pt20to30',
                                        'QCD_BCtoE_Pt30to80' ),
    dqmDirectory_output = cms.string('qcdSum')
  )
  #qcdSum = cms.PSet(
  #  dqmDirectories_input = cms.vstring('InclusivePPmuX',
  #                                     'PPmuXptGt20'),
  #  dqmDirectory_output = cms.string('qcdSum')
  #)                          
)

addZtoElecMu_smSum = cms.EDAnalyzer("DQMHistAdder",
  smSum = cms.PSet(
    dqmDirectories_input = cms.vstring( 'Ztautau',
                                        'Zee',
                                        'Zmumu',
                                        'WplusJets',
                                        'qcdSum' ),
    dqmDirectory_output = cms.string('smSum')
  )
)

addZtoElecMu = cms.Sequence(addZtoElecMu_qcdSum + addZtoElecMu_smSum)

plotZtoElecMu = cms.EDAnalyzer("DQMHistPlotter",
  processes = cms.PSet(
    Ztautau = copy.deepcopy(processZtoElecMu_Ztautau.config_dqmHistPlotter),
    Zee = copy.deepcopy(processZtoElecMu_Zee.config_dqmHistPlotter),
    Zmumu = copy.deepcopy(processZtoElecMu_Zmumu.config_dqmHistPlotter),                         
    WplusJets = copy.deepcopy(processZtoElecMu_WplusJets.config_dqmHistPlotter),
    QCD_BCtoE_Pt20to30 = copy.deepcopy(processZtoElecMu_QCD_BCtoE_Pt20to30.config_dqmHistPlotter),
    QCD_BCtoE_Pt30to80 = copy.deepcopy(processZtoElecMu_QCD_BCtoE_Pt30to80.config_dqmHistPlotter),
    InclusivePPmuX = copy.deepcopy(processZtoElecMu_InclusivePPmuX.config_dqmHistPlotter),
    PPmuXptGt20 = copy.deepcopy(processZtoElecMu_PPmuXptGt20.config_dqmHistPlotter),
    qcdSum = cms.PSet(
      dqmDirectory = cms.string('qcdSum'),
      legendEntry = cms.string('QCD'),
      type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
    )
  ),

  xAxes = cms.PSet(
    pt = copy.deepcopy(xAxis_pt),
    eta = copy.deepcopy(xAxis_eta),
    phi = copy.deepcopy(xAxis_phi),
    ipXY = copy.deepcopy(xAxis_ipXY),
    dPhi = copy.deepcopy(xAxis_dPhi),
    unlabeled = copy.deepcopy(xAxis_unlabeled),
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
      Zee = copy.deepcopy(drawOption_Zee),
      Zmumu = copy.deepcopy(drawOption_Zmumu),
      WplusJets = copy.deepcopy(drawOption_WplusJets),
      qcdSum = copy.deepcopy(drawOption_QCD)
    )
  ),

  drawJobs = cms.PSet(
    cutFlowControlPlots_electronPt_afterElectronEtaCut = copy.deepcopy(plots_ZtoElecMu_electronPt_afterElectronEtaCut),
    cutFlowControlPlots_muonEta_afterGlobalMuonCut = copy.deepcopy(plots_ZtoElecMu_muonEta_afterGlobalMuonCut),
    cutFlowControlPlots_muonTrackIPxy_afterMuonAntiPionCut = copy.deepcopy(plots_ZtoElecMu_muonTrackIPxy_afterMuonAntiPionCut),
    cutFlowControlPlots_dPhi1MET_afterMuonTrkIPcut = copy.deepcopy(plots_ZtoElecMu_dPhi1MET_afterMuonTrkIPcut),
    cutFlowControlPlots_dPhi2MET_afterMuonTrkIPcut = copy.deepcopy(plots_ZtoElecMu_dPhi2MET_afterMuonTrkIPcut)
  ),

  canvasSizeX = cms.int32(800),
  canvasSizeY = cms.int32(640),                         

  outputFilePath = cms.string('./plots/'),
  outputFileName = cms.string('plotsZtoElecMu.ps')
  #indOutputFileName = cms.string('plotZtoElecMu_#PLOT#.png')
)

saveZtoElecMu = cms.EDAnalyzer("DQMSimpleFileSaver",
  outputFileName = cms.string('plotsZtoElecMu_all.root')
)

makeZtoElecMuPlots = cms.Sequence( loadZtoElecMu
                                  +addZtoElecMu
                                  +saveZtoElecMu 
                                  +plotZtoElecMu )
  
