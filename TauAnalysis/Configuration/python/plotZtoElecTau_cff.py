import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------
#
# Plot histograms for Z --> e + tau-jet channel
#
# Author: Christian Veelken, UC Davis
#         Jeff Kolb, Notre Dame
#         Nancy Marinelli, Notre Dame
# 
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.plotZtoElecTau_processes_cfi import *
from TauAnalysis.Configuration.plotZtoElecTau_drawJobs_cfi import *
from TauAnalysis.DQMTools.plotterStyleDefinitions_cfi import *

loadZtoElecTau = cms.EDAnalyzer("DQMFileLoader",
	Data = copy.deepcopy(processZtoElecTau_Data_7TeV_Sum.config_dqmFileLoader),
	#MinBias = copy.deepcopy(processZtoElecTau_MinBias_7TeV_Sum.config_dqmFileLoader),
	Ztautau = copy.deepcopy(processZtoElecTau_Ztautau_7TeV_Sum.config_dqmFileLoader),
	Zee = copy.deepcopy(processZtoElecTau_Zee_7TeV_Sum.config_dqmFileLoader),
	ZeePlusJets = copy.deepcopy(processZtoElecTau_ZeePlusJets_7TeV_Sum.config_dqmFileLoader),
	ZtautauPlusJets = copy.deepcopy(processZtoElecTau_ZtautauPlusJets_7TeV_Sum.config_dqmFileLoader),                      
	gammaPlusJets_Pt15to20 = copy.deepcopy(processZtoElecTau_gammaPlusJets_Pt15to20_7TeV.config_dqmFileLoader),
	gammaPlusJets_Pt20to30 = copy.deepcopy(processZtoElecTau_gammaPlusJets_Pt20to30_7TeV.config_dqmFileLoader),
	gammaPlusJets_Pt30to50 = copy.deepcopy(processZtoElecTau_gammaPlusJets_Pt30to50_7TeV.config_dqmFileLoader),
	gammaPlusJets_Pt50to80 = copy.deepcopy(processZtoElecTau_gammaPlusJets_Pt50to80_7TeV.config_dqmFileLoader),
	gammaPlusJets_Pt80to120 = copy.deepcopy(processZtoElecTau_gammaPlusJets_Pt80to120_7TeV.config_dqmFileLoader),
	WplusJets = copy.deepcopy(processZtoElecTau_WplusJets_7TeV_Sum.config_dqmFileLoader),
	QCD_BCtoE_Pt20to30 = copy.deepcopy(processZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_Sum.config_dqmFileLoader),
	QCD_BCtoE_Pt30to80 = copy.deepcopy(processZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_Sum.config_dqmFileLoader),
	QCD_BCtoE_Pt80to170 = copy.deepcopy(processZtoElecTau_QCD_BCtoE_Pt80to170_7TeV_Sum.config_dqmFileLoader),
	QCD_EMenriched_Pt20to30 = copy.deepcopy(processZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_Sum.config_dqmFileLoader),
	QCD_EMenriched_Pt30to80 = copy.deepcopy(processZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_Sum.config_dqmFileLoader),
	QCD_EMenriched_Pt80to170 = copy.deepcopy(processZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_Sum.config_dqmFileLoader),
	TTplusJets = copy.deepcopy(processZtoElecTau_TTplusJets_7TeV_Sum.config_dqmFileLoader)
)

addZtoElecTau_qcdSum = cms.EDAnalyzer("DQMHistAdder",
	qcdSum = cms.PSet(
		dqmDirectories_input = cms.vstring(
			'harvested/QCD_BCtoE_Pt20to30',
			'harvested/QCD_BCtoE_Pt30to80',
			'harvested/QCD_BCtoE_Pt80to170',
			'harvested/QCD_EMenriched_Pt20to30',
			'harvested/QCD_EMenriched_Pt30to80',
			'harvested/QCD_EMenriched_Pt80to170'
		),
		dqmDirectory_output = cms.string('harvested/qcdSum')
	)                          
)

addZtoElecTau_qcdBCtoESum = cms.EDAnalyzer("DQMHistAdder",
	qcdBCtoESum = cms.PSet(
		dqmDirectories_input = cms.vstring(
			'harvested/QCD_BCtoE_Pt20to30',
			'harvested/QCD_BCtoE_Pt30to80',
			'harvested/QCD_BCtoE_Pt80to170'
		),
		dqmDirectory_output = cms.string('harvested/qcdBCtoESum')
	)                          
)

addZtoElecTau_qcdEMenrichedSum = cms.EDAnalyzer("DQMHistAdder",
	qcdEMenrichedSum = cms.PSet(
		dqmDirectories_input = cms.vstring(
			'harvested/QCD_EMenriched_Pt20to30',
			'harvested/QCD_EMenriched_Pt30to80',
			'harvested/QCD_EMenriched_Pt80to170'
		),
		dqmDirectory_output = cms.string('harvested/qcdEMenrichedSum')
	)                          
)

addZtoElecTau_gammaPlusJetsSum = cms.EDAnalyzer("DQMHistAdder",
    gammaPlusJetsSum = cms.PSet(
	dqmDirectories_input = cms.vstring(
            'harvested/gammaPlusJets_Pt15to20',
            'harvested/gammaPlusJets_Pt20to30',
            'harvested/gammaPlusJets_Pt30to50',
            'harvested/gammaPlusJets_Pt50to80',
            'harvested/gammaPlusJets_Pt80to120'
        ),
	dqmDirectory_output = cms.string('harvested/gammaPlusJetsSum')
    )                          
)

addZtoElecTau_smSum = cms.EDAnalyzer("DQMHistAdder",
	smSum = cms.PSet(
		dqmDirectories_input = cms.vstring(
			'harvested/Ztautau',
			#'harvested/ZtautauPlusJets',
			#'harvested/ZeePlusJets', 
			'harvested/Zee',
			'harvested/gammaPlusJetsSum',
			'harvested/qcdSum', 
			'harvested/WplusJets',
			'harvested/TTplusJets'
		),
		dqmDirectory_output = cms.string('harvested/smSum')
	)
)

addZtoElecTau = cms.Sequence(addZtoElecTau_qcdBCtoESum + addZtoElecTau_qcdEMenrichedSum + addZtoElecTau_gammaPlusJetsSum)

plotZtoElecTau = cms.EDAnalyzer("DQMHistPlotter",
    processes = cms.PSet(
		Data = copy.deepcopy(process_Data.config_dqmHistPlotter),
		#MinBias = copy.deepcopy(process_MinBias.config_dqmHistPlotter),
		Ztautau = copy.deepcopy(process_Ztautau.config_dqmHistPlotter),
		#ZtautauPlusJets = copy.deepcopy(process_ZtautauPlusJets.config_dqmHistPlotter),
		Zee = copy.deepcopy(process_Zee.config_dqmHistPlotter),
		#ZeePlusJets = copy.deepcopy(process_ZeePlusJets.config_dqmHistPlotter),
		WplusJets = copy.deepcopy(process_WplusJets.config_dqmHistPlotter),
		TTplusJets = copy.deepcopy(process_TTplusJets.config_dqmHistPlotter),
		gammaPlusJetsSum = cms.PSet(
		    dqmDirectory = cms.string('harvested/gammaPlusJetsSum'),
		    legendEntry = cms.string('#gamma + Jets'),
		    type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
		),
		qcdBCtoESum = cms.PSet(
		    dqmDirectory = cms.string('harvested/qcdBCtoESum'),
		    legendEntry = cms.string('QCD bc->e'),
		    type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
		),
		qcdEMenrichedSum = cms.PSet(
		    dqmDirectory = cms.string('harvested/qcdEMenrichedSum'),
		    legendEntry = cms.string('QCD EM-enriched'),
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
			Data = copy.deepcopy(drawOption_Data),
			#MinBias = copy.deepcopy(drawOption_MinBias),
			Ztautau = copy.deepcopy(drawOption_Ztautau),
			#ZtautauPlusJets = copy.deepcopy(drawOption_ZtautauPlusJets),
			Zee = copy.deepcopy(drawOption_Zee),
			#ZeePlusJets = copy.deepcopy(drawOption_ZeePlusJets),
			WplusJets = copy.deepcopy(drawOption_WplusJets),
			gammaPlusJetsSum = copy.deepcopy(drawOption_gammaPlusJets),
			qcdBCtoESum = copy.deepcopy(drawOption_QCD),
			qcdEMenrichedSum = copy.deepcopy(drawOption_darkBlue_stacked),
			TTplusJets = copy.deepcopy(drawOption_TTplusJets)
		)
    ),

    drawJobs = drawJobConfigurator_ZtoElecTau.configure(),

    canvasSizeX = cms.int32(800),
    canvasSizeY = cms.int32(640),                         

    outputFilePath = cms.string('./plots/'),
    #outputFileName = cms.string('plotsZtoElecTau.ps'),
    indOutputFileName = cms.string('plotZtoElecTau_#PLOT#.png')
)

saveZtoElecTau = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('plotsZtoElecTau_all.root')
)



