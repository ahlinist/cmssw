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
    ZplusJets = copy.deepcopy(processAHtoMuTau_ZplusJetsSum.config_dqmFileLoader),
    WplusJets = copy.deepcopy(processAHtoMuTau_WplusJetsSum.config_dqmFileLoader),
    Vqq = copy.deepcopy(processAHtoMuTau_VqqSum.config_dqmFileLoader),
    InclusivePPmuX = copy.deepcopy(processAHtoMuTau_InclusivePPmuXsum.config_dqmFileLoader),
    PPmuXptGt20 = copy.deepcopy(processAHtoMuTau_PPmuXptGt20Sum.config_dqmFileLoader),
    TTplusJets = copy.deepcopy(processAHtoMuTau_TTplusJetsSum.config_dqmFileLoader),
    AH120_tautau = copy.deepcopy(processAHtoMuTau_AH120_tautauSum.config_dqmFileLoader),
    AHbb120_tautau = copy.deepcopy(processAHtoMuTau_AHbb120_tautauSum.config_dqmFileLoader)
)

addAHtoMuTau_woBtag_AHsum120_tautau = cms.EDAnalyzer("DQMHistAdder",
    AHsum120_tautau = cms.PSet(
        dqmDirectories_input = cms.vstring(
            'harvested/AH120_tautau/ahMuTauAnalyzer_woBtag',
            'harvested/AHbb120_tautau/ahMuTauAnalyzer_woBtag'
        ),
        dqmDirectory_output = cms.string('harvested/AHsum120_tautau/ahMuTauAnalyzer_woBtag')
    )
)

addAHtoMuTau_wBtag_AHsum120_tautau = cms.EDAnalyzer("DQMHistAdder",
    AHsum120_tautau = cms.PSet(
        dqmDirectories_input = cms.vstring(
            'harvested/AH120_tautau/ahMuTauAnalyzer_wBtag',
            'harvested/AHbb120_tautau/ahMuTauAnalyzer_wBtag'
        ),
        dqmDirectory_output = cms.string('harvested/AHsum120_tautau/ahMuTauAnalyzer_wBtag')
    )
)

addAHtoMuTau_AHsum120_tautau = cms.Sequence(addAHtoMuTau_woBtag_AHsum120_tautau * addAHtoMuTau_wBtag_AHsum120_tautau)

addAHtoMuTau_woBtag_qcdSum = cms.EDAnalyzer("DQMHistAdder",
    qcdSum = cms.PSet(
        dqmDirectories_input = cms.vstring(
            'harvested/InclusivePPmuX/ahMuTauAnalyzer_woBtag',
            'harvested/PPmuXptGt20/ahMuTauAnalyzer_woBtag'
        ),
        dqmDirectory_output = cms.string('harvested/qcdSum/ahMuTauAnalyzer_woBtag')
    )
)

addAHtoMuTau_wBtag_qcdSum = cms.EDAnalyzer("DQMHistAdder",
    qcdSum = cms.PSet(
        dqmDirectories_input = cms.vstring(
            'harvested/InclusivePPmuX/ahMuTauAnalyzer_wBtag',
            'harvested/PPmuXptGt20/ahMuTauAnalyzer_wBtag'
        ),
        dqmDirectory_output = cms.string('harvested/qcdSum/ahMuTauAnalyzer_wBtag')
    )
)

addAHtoMuTau_qcdSum = cms.Sequence(addAHtoMuTau_woBtag_qcdSum * addAHtoMuTau_wBtag_qcdSum)

addAHtoMuTau_woBtag_smBgSum = cms.EDAnalyzer("DQMHistAdder",
    smBgSum = cms.PSet(
        dqmDirectories_input = cms.vstring(
            'harvested/Ztautau/ahMuTauAnalyzer_woBtag',
            'harvested/Zmumu/ahMuTauAnalyzer_woBtag',
            #'harvested/ZplusJets/ahMuTauAnalyzer_woBtag',
            'harvested/WplusJets/ahMuTauAnalyzer_woBtag',
            'harvested/Vqq/ahMuTauAnalyzer_woBtag',
            'harvested/TTplusJets/ahMuTauAnalyzer_woBtag',
            'harvested/qcdSum/ahMuTauAnalyzer_woBtag'
        ),
        dqmDirectory_output = cms.string('harvested/smBgSum/ahMuTauAnalyzer_woBtag')
    )
)

addAHtoMuTau_wBtag_smBgSum = cms.EDAnalyzer("DQMHistAdder",
    smBgSum = cms.PSet(
        dqmDirectories_input = cms.vstring(
            'harvested/Ztautau/ahMuTauAnalyzer_wBtag',
            'harvested/Zmumu/ahMuTauAnalyzer_wBtag',
            #'harvested/ZplusJets/ahMuTauAnalyzer_wBtag',
            'harvested/WplusJets/ahMuTauAnalyzer_wBtag',
            'harvested/Vqq/ahMuTauAnalyzer_wBtag',
            'harvested/TTplusJets/ahMuTauAnalyzer_wBtag',
            'harvested/qcdSum/ahMuTauAnalyzer_wBtag'
        ),
        dqmDirectory_output = cms.string('harvested/smBgSum/ahMuTauAnalyzer_wBtag')
    )
)

addAHtoMuTau_smBgSum = cms.Sequence(addAHtoMuTau_woBtag_smBgSum * addAHtoMuTau_wBtag_smBgSum)

addAHtoMuTau_woBtag_smSum = cms.EDAnalyzer("DQMHistAdder",
    smSum = cms.PSet(
        dqmDirectories_input = cms.vstring(
            ##'harvested/Ztautau/ahMuTauAnalyzer_woBtag',
            'harvested/smBgSum/ahMuTauAnalyzer_woBtag'
        ),
        dqmDirectory_output = cms.string('harvested/smSum/ahMuTauAnalyzer_woBtag')
    )
)

addAHtoMuTau_wBtag_smSum = cms.EDAnalyzer("DQMHistAdder",
    smSum = cms.PSet(
        dqmDirectories_input = cms.vstring(
            ##'harvested/Ztautau/ahMuTauAnalyzer_wBtag',
            'harvested/smBgSum/ahMuTauAnalyzer_wBtag'
        ),
        dqmDirectory_output = cms.string('harvested/smSum/ahMuTauAnalyzer_wBtag')
    )
)

addAHtoMuTau_smSum = cms.Sequence(addAHtoMuTau_woBtag_smSum * addAHtoMuTau_wBtag_smSum)

addAHtoMuTau = cms.Sequence(addAHtoMuTau_AHsum120_tautau * addAHtoMuTau_qcdSum * addAHtoMuTau_smBgSum * addAHtoMuTau_smSum)

plotAHtoMuTau_woBtag = cms.EDAnalyzer("DQMHistPlotter",
    processes = cms.PSet(
        Ztautau = copy.deepcopy(process_Ztautau.config_dqmHistPlotter),
        Zmumu = copy.deepcopy(process_Zmumu.config_dqmHistPlotter),
        #ZplusJets = copy.deepcopy(process_ZplusJets.config_dqmHistPlotter),
        WplusJets = copy.deepcopy(process_WplusJets.config_dqmHistPlotter),
        Vqq = copy.deepcopy(process_Vqq.config_dqmHistPlotter),
        TTplusJets = copy.deepcopy(process_TTplusJets.config_dqmHistPlotter),
        InclusivePPmuX = copy.deepcopy(process_InclusivePPmuX.config_dqmHistPlotter),
        PPmuXptGt20 = copy.deepcopy(process_PPmuXptGt20.config_dqmHistPlotter),
        qcdSum = cms.PSet(
            dqmDirectory = cms.string('harvested/qcdSum'),
            legendEntry = cms.string('QCD'),
            type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
        ),
        AHsum120_tautau = cms.PSet(
            dqmDirectory = cms.string('harvested/AHsum120_tautau'),
            legendEntry = cms.string('A/H #rightarrow #tau^{+} #tau^{-}'),
            type = cms.string('bsmMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
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
        MassRebin = copy.deepcopy(xAxis_massRebin),
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
            #ZplusJets = copy.deepcopy(drawOption_ZplusJets),
            WplusJets = copy.deepcopy(drawOption_WplusJets),
            Vqq = copy.deepcopy(drawOption_Vqq),
            TTplusJets = copy.deepcopy(drawOption_TTplusJets),
            qcdSum = copy.deepcopy(drawOption_QCD),
            AHsum120_tautau = copy.deepcopy(drawOption_AHbb_separate)
        )
    ),

    drawJobs = drawJobConfigurator_AHtoMuTau_woBtag.configure(),

    canvasSizeX = cms.int32(800),
    canvasSizeY = cms.int32(640),

    outputFilePath = cms.string('./plots/'),
    #outputFileName = cms.string('plotsAHtoMuTau_woBtag.ps')
    indOutputFileName = cms.string('plotAHtoMuTau_woBtag_#PLOT#.pdf')
)

plotAHtoMuTau_wBtag = plotAHtoMuTau_woBtag.clone(
    drawJobs = drawJobConfigurator_AHtoMuTau_wBtag.configure(),
    #outputFileName = cms.string('plotsAHtoMuTau_wBtag.ps')
    indOutputFileName = cms.string('plotAHtoMuTau_wBtag_#PLOT#.pdf')
)

plotAHtoMuTau = cms.Sequence(plotAHtoMuTau_woBtag * plotAHtoMuTau_wBtag)

saveAHtoMuTau = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('plotsAHtoMuTau_all.root')
)



