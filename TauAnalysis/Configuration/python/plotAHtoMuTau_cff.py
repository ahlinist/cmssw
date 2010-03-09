import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------
#
# Plot histograms for MSSM Higgs analysis in
# A/H --> mu + tau-jet channel
#
# Author: Christian Veelken, UC Davis
#
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.plotAHtoMuTau_processes_7TeV_cfi import *
from TauAnalysis.Configuration.plotAHtoMuTau_drawJobs_cfi import *
from TauAnalysis.DQMTools.plotterStyleDefinitions_cfi import *

loadAHtoMuTau = cms.EDAnalyzer("DQMFileLoader",
    Ztautau = copy.deepcopy(processAHtoMuTau_ZtautauSum.config_dqmFileLoader),
    Zmumu = copy.deepcopy(processAHtoMuTau_ZmumuSum.config_dqmFileLoader),
    WplusJets = copy.deepcopy(processAHtoMuTau_WplusJetsSum.config_dqmFileLoader),
    InclusivePPmuX = copy.deepcopy(processAHtoMuTau_InclusivePPmuXsum.config_dqmFileLoader),
    PPmuXptGt20 = copy.deepcopy(processAHtoMuTau_PPmuXptGt20Sum.config_dqmFileLoader),
    TTplusJets = copy.deepcopy(processAHtoMuTau_TTplusJetsSum.config_dqmFileLoader),
    AH_tautau = copy.deepcopy(processAHtoMuTau_AH_tautau.config_dqmFileLoader),
    AHbb_tautau = copy.deepcopy(processAHtoMuTau_AHbb_tautau.config_dqmFileLoader)
)

addAHtoMuTau_centralJetVeto_AH_tautauSum = cms.EDAnalyzer("DQMHistAdder",
    AH_tautauSum = cms.PSet(
        dqmDirectories_input = cms.vstring(
            'harvested/AH_tautau/ahMuTauAnalyzer_centralJetVeto',
            'harvested/AHbb_tautau/ahMuTauAnalyzer_centralJetVeto'
        ),
        dqmDirectory_output = cms.string('harvested/AH_tautauSum/ahMuTauAnalyzer_centralJetVeto')
    )                          
)

addAHtoMuTau_centralJetBtag_AH_tautauSum = cms.EDAnalyzer("DQMHistAdder",
    AH_tautauSum = cms.PSet(
        dqmDirectories_input = cms.vstring(
            'harvested/AH_tautau/ahMuTauAnalyzer_centralJetBtag',
            'harvested/AHbb_tautau/ahMuTauAnalyzer_centralJetBtag'
        ),
        dqmDirectory_output = cms.string('harvested/AH_tautauSum/ahMuTauAnalyzer_centralJetBtag')
    )                          
)

addAHtoMuTau_AH_tautauSum = cms.Sequence(addAHtoMuTau_centralJetVeto_AH_tautauSum * addAHtoMuTau_centralJetVeto_AH_tautauSum)

addAHtoMuTau_centralJetVeto_qcdSum = cms.EDAnalyzer("DQMHistAdder",
    qcdSum = cms.PSet(
        dqmDirectories_input = cms.vstring(
            'harvested/InclusivePPmuX/ahMuTauAnalyzer_centralJetVeto',
            'harvested/PPmuXptGt20/ahMuTauAnalyzer_centralJetVeto'
        ),
        dqmDirectory_output = cms.string('harvested/qcdSum/ahMuTauAnalyzer_centralJetVeto')
    )                          
)

addAHtoMuTau_centralJetBtag_qcdSum = cms.EDAnalyzer("DQMHistAdder",
    qcdSum = cms.PSet(
        dqmDirectories_input = cms.vstring(
            'harvested/InclusivePPmuX/ahMuTauAnalyzer_centralJetBtag',
            'harvested/PPmuXptGt20/ahMuTauAnalyzer_centralJetBtag'
        ),
        dqmDirectory_output = cms.string('harvested/qcdSum/ahMuTauAnalyzer_centralJetBtag')
    )                          
)

addAHtoMuTau_qcdSum = cms.Sequence(addAHtoMuTau_centralJetVeto_qcdSum * addAHtoMuTau_centralJetBtag_qcdSum)

addAHtoMuTau_centralJetVeto_smBgSum = cms.EDAnalyzer("DQMHistAdder",
    smBgSum = cms.PSet(
        dqmDirectories_input = cms.vstring(
            'harvested/Zmumu/ahMuTauAnalyzer_centralJetVeto',
            'harvested/WplusJets/ahMuTauAnalyzer_centralJetVeto',
            'harvested/TTplusJets/ahMuTauAnalyzer_centralJetVeto',
            'harvested/qcdSum/ahMuTauAnalyzer_centralJetVeto'
        ),
        dqmDirectory_output = cms.string('harvested/smBgSum/ahMuTauAnalyzer_centralJetVeto')
    )
)

addAHtoMuTau_centralJetBtag_smBgSum = cms.EDAnalyzer("DQMHistAdder",
    smBgSum = cms.PSet(
        dqmDirectories_input = cms.vstring(
            'harvested/Zmumu/ahMuTauAnalyzer_centralJetBtag',
            'harvested/WplusJets/ahMuTauAnalyzer_centralJetBtag',
            'harvested/TTplusJets/ahMuTauAnalyzer_centralJetBtag',
            'harvested/qcdSum/ahMuTauAnalyzer_centralJetBtag'
        ),
        dqmDirectory_output = cms.string('harvested/smBgSum/ahMuTauAnalyzer_centralJetBtag')
    )
)

addAHtoMuTau_smBgSum = cms.Sequence(addAHtoMuTau_centralJetVeto_smBgSum * addAHtoMuTau_centralJetBtag_smBgSum)

addAHtoMuTau_centralJetVeto_smSum = cms.EDAnalyzer("DQMHistAdder",
    smSum = cms.PSet(
        dqmDirectories_input = cms.vstring(
            'harvested/Ztautau/ahMuTauAnalyzer_centralJetVeto',
            'harvested/smBgSum/ahMuTauAnalyzer_centralJetVeto'
        ),
        dqmDirectory_output = cms.string('harvested/smSum/ahMuTauAnalyzer_centralJetVeto')
    )
)

addAHtoMuTau_centralJetBtag_smSum = cms.EDAnalyzer("DQMHistAdder",
    smSum = cms.PSet(
        dqmDirectories_input = cms.vstring(
            'harvested/Ztautau/ahMuTauAnalyzer_centralJetBtag',
            'harvested/smBgSum/ahMuTauAnalyzer_centralJetBtag'
        ),
        dqmDirectory_output = cms.string('harvested/smSum/ahMuTauAnalyzer_centralJetBtag')
    )
)

addAHtoMuTau_smSum = cms.Sequence(addAHtoMuTau_centralJetVeto_smSum * addAHtoMuTau_centralJetBtag_smSum)

addAHtoMuTau = cms.Sequence(addAHtoMuTau_AH_tautauSum * addAHtoMuTau_qcdSum * addAHtoMuTau_smBgSum * addAHtoMuTau_smSum)

plotAHtoMuTau_centralJetVeto = cms.EDAnalyzer("DQMHistPlotter",
    processes = cms.PSet(
        Ztautau = copy.deepcopy(process_Ztautau.config_dqmHistPlotter),
        Zmumu = copy.deepcopy(process_Zmumu.config_dqmHistPlotter),
        WplusJets = copy.deepcopy(process_WplusJets.config_dqmHistPlotter),
        TTplusJets = copy.deepcopy(process_TTplusJets.config_dqmHistPlotter),
        InclusivePPmuX = copy.deepcopy(process_InclusivePPmuX.config_dqmHistPlotter),
        PPmuXptGt20 = copy.deepcopy(process_PPmuXptGt20.config_dqmHistPlotter),
        qcdSum = cms.PSet(
            dqmDirectory = cms.string('harvested/qcdSum'),
            legendEntry = cms.string('QCD'),
            type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
        ),
        AHtautauSum = cms.PSet(
            dqmDirectory = cms.string('harvested/AHtautauSum'),
            legendEntry = cms.string('A/H #rightarrow #tau^{+} #tau^{-}'),
            type = cms.string('bsmMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
        ),
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
            WplusJets = copy.deepcopy(drawOption_WplusJets),
            TTplusJets = copy.deepcopy(drawOption_TTplusJets),
            qcdSum = copy.deepcopy(drawOption_QCD),
            AHtautauSum = copy.deepcopy(drawOption_AHbb),
        )
    ),
                              
    drawJobs = drawJobConfigurator_AHtoMuTau_centralJetVeto.configure(),

    canvasSizeX = cms.int32(800),
    canvasSizeY = cms.int32(640),                         

    outputFilePath = cms.string('./plots/'),
    #outputFileName = cms.string('plotsAHtoMuTau_centralJetVeto.ps')
    indOutputFileName = cms.string('plotAHtoMuTau_centralJetVeto_#PLOT#.eps')
)

plotAHtoMuTau_centralJetBtag = plotAHtoMuTau_centralJetVeto.clone(
    drawJobs = drawJobConfigurator_AHtoMuTau_centralJetBtag.configure(),
    #outputFileName = cms.string('plotsAHtoMuTau_centralJetBtag.ps')
    indOutputFileName = cms.string('plotAHtoMuTau_centralJetBtag_#PLOT#.eps')
)

plotAHtoMuTau = cms.Sequence(plotAHtoMuTau_centralJetVeto * plotAHtoMuTau_centralJetBtag)

saveAHtoMuTau = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('plotsAHtoMuTau_all.root')
)


  
