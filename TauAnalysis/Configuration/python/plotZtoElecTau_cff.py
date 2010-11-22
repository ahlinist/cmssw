import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------
#
# Plot histograms for Z --> e + tau-jet channel
#
# Author: Christian Veelken, UC Davis
#         Jeff Kolb, Notre Dame
# 
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.plotZtoElecTau_processes_cfi import *
from TauAnalysis.Configuration.plotZtoElecTau_drawJobs_cfi import *
from TauAnalysis.DQMTools.plotterStyleDefinitions_cfi import *

loadZtoElecTau = cms.EDAnalyzer("DQMFileLoader",
	#verbosity = cms.bool(True),
	Data = copy.deepcopy(processZtoElecTau_Data_Sum.config_dqmFileLoader),
	#Data_7TeV = copy.deepcopy(processZtoElecTau_Data_7TeV_SumSum.config_dqmFileLoader),
	#Data2010B = copy.deepcopy(processZtoElecTau_Data2010B_SumSum.config_dqmFileLoader),
	Ztautau = copy.deepcopy(processZtoElecTau_Ztautau_Sum.config_dqmFileLoader),
	Zee = copy.deepcopy(processZtoElecTau_Zee_SumSum.config_dqmFileLoader),
	#Zee = copy.deepcopy(processZtoElecTau_Zee_Sum.config_dqmFileLoader),
	gammaPlusJets_Pt15to30 = copy.deepcopy(processZtoElecTau_gammaPlusJets_Pt15to30_Sum.config_dqmFileLoader),
	gammaPlusJets_Pt30to50 = copy.deepcopy(processZtoElecTau_gammaPlusJets_Pt30to50_Sum.config_dqmFileLoader),
	gammaPlusJets_Pt50to80 = copy.deepcopy(processZtoElecTau_gammaPlusJets_Pt50to80_Sum.config_dqmFileLoader),
	WtoENu = copy.deepcopy(processZtoElecTau_WtoENu_SumSum.config_dqmFileLoader),
	#WtoENu = copy.deepcopy(processZtoElecTau_WtoENu_Sum.config_dqmFileLoader),
	WtoTauNu = copy.deepcopy(processZtoElecTau_WtoTauNu_Sum.config_dqmFileLoader),
	QCD_BCtoE_Pt20to30 = copy.deepcopy(processZtoElecTau_QCD_BCtoE_Pt20to30_SumSum.config_dqmFileLoader),
	QCD_BCtoE_Pt30to80 = copy.deepcopy(processZtoElecTau_QCD_BCtoE_Pt30to80_SumSum.config_dqmFileLoader),
	QCD_BCtoE_Pt80to170 = copy.deepcopy(processZtoElecTau_QCD_BCtoE_Pt80to170_SumSum.config_dqmFileLoader),
	QCD_EMenriched_Pt20to30 = copy.deepcopy(processZtoElecTau_QCD_EM_Pt20to30_SumSum.config_dqmFileLoader),
	QCD_EMenriched_Pt30to80 = copy.deepcopy(processZtoElecTau_QCD_EM_Pt30to80_SumSum.config_dqmFileLoader),
	QCD_EMenriched_Pt80to170 = copy.deepcopy(processZtoElecTau_QCD_EM_Pt80to170_SumSum.config_dqmFileLoader)
	#TTplusJets = copy.deepcopy(processZtoElecTau_TTplusJets_Sum.config_dqmFileLoader)

)

addZtoElecTau_wPlusJetsSum = cms.EDAnalyzer("DQMHistAdder",
	wPlusJetsSum = cms.PSet(
		dqmDirectories_input = cms.vstring(
			'summed/harvested/WtoENu',
			'summed/harvested/WtoTauNu'
		),
		dqmDirectory_output = cms.string('summed/harvested/wPlusJetsSum')
	)                          
)
addZtoElecTau_qcdSum = cms.EDAnalyzer("DQMHistAdder",
	qcdSum = cms.PSet(
		dqmDirectories_input = cms.vstring(
			'summed/harvested/QCD_BCtoE_Pt20to30',
			'summed/harvested/QCD_BCtoE_Pt30to80',
			'summed/harvested/QCD_BCtoE_Pt80to170',
			'summed/harvested/QCD_EMenriched_Pt20to30',
			'summed/harvested/QCD_EMenriched_Pt30to80',
			'summed/harvested/QCD_EMenriched_Pt80to170'
		),
		dqmDirectory_output = cms.string('summed/harvested/qcdSum')
	)                          
)

addZtoElecTau_qcdBCtoESum = cms.EDAnalyzer("DQMHistAdder",
	qcdBCtoESum = cms.PSet(
		dqmDirectories_input = cms.vstring(
			'summed/harvested/QCD_BCtoE_Pt20to30',
			'summed/harvested/QCD_BCtoE_Pt30to80',
			'summed/harvested/QCD_BCtoE_Pt80to170'
		),
		dqmDirectory_output = cms.string('summed/harvested/qcdBCtoESum')
	)                          
)

addZtoElecTau_qcdEMenrichedSum = cms.EDAnalyzer("DQMHistAdder",
	qcdEMenrichedSum = cms.PSet(
		dqmDirectories_input = cms.vstring(
			'summed/harvested/QCD_EMenriched_Pt20to30',
			'summed/harvested/QCD_EMenriched_Pt30to80',
			'summed/harvested/QCD_EMenriched_Pt80to170'
		),
		dqmDirectory_output = cms.string('summed/harvested/qcdEMenrichedSum')
	)                          
)

addZtoElecTau_gammaPlusJetsSum = cms.EDAnalyzer("DQMHistAdder",
    gammaPlusJetsSum = cms.PSet(
	dqmDirectories_input = cms.vstring(
            'summed/harvested/gammaPlusJets_Pt15to30',
            'summed/harvested/gammaPlusJets_Pt30to50',
            'summed/harvested/gammaPlusJets_Pt50to80'
        ),
	dqmDirectory_output = cms.string('summed/harvested/gammaPlusJetsSum')
    )                          
)

addZtoElecTau = cms.Sequence(addZtoElecTau_qcdBCtoESum + addZtoElecTau_qcdEMenrichedSum + addZtoElecTau_gammaPlusJetsSum + addZtoElecTau_wPlusJetsSum)

plotZtoElecTau = cms.EDAnalyzer("DQMHistPlotter",
    processes = cms.PSet(
		#Data = copy.deepcopy(process_Data.config_dqmHistPlotter),
		Data = cms.PSet(
			dqmDirectory = cms.string('summed/harvested/Data'),
			legendEntry = cms.string('Data'),
			type = cms.string('Data') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
		),
		#Ztautau = copy.deepcopy(process_Ztautau.config_dqmHistPlotter),
		Ztautau = cms.PSet(
			dqmDirectory = cms.string('summed/harvested/Ztautau'),
			legendEntry = cms.string('Z #rightarrow #tau^{+} #tau^{-}'),
			type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
		),
		#Zee = copy.deepcopy(process_Zee.config_dqmHistPlotter),
		Zee = cms.PSet(
			dqmDirectory = cms.string('summed/harvested/Zee'),
			legendEntry = cms.string('Z #rightarrow e^{+} e^{-}'),
			type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
		),
		wPlusJetsSum = cms.PSet(
			dqmDirectory = cms.string('summed/harvested/wPlusJetsSum'),
			legendEntry = cms.string('W -> e/tau nu'),
			type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
		),
		#TTplusJets = copy.deepcopy(process_TTplusJets.config_dqmHistPlotter),
		gammaPlusJetsSum = cms.PSet(
			dqmDirectory = cms.string('summed/harvested/gammaPlusJetsSum'),
			legendEntry = cms.string('#gamma + Jets'),
			type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
		),
		qcdBCtoESum = cms.PSet(
			dqmDirectory = cms.string('summed/harvested/qcdBCtoESum'),
			legendEntry = cms.string('QCD bc->e'),
			type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
		),
		qcdEMenrichedSum = cms.PSet(
			dqmDirectory = cms.string('summed/harvested/qcdEMenrichedSum'),
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
			Ztautau = copy.deepcopy(drawOption_Ztautau),
			Zee = copy.deepcopy(drawOption_Zee),
			#WplusJets = copy.deepcopy(drawOption_WplusJets)
			wPlusJetsSum = copy.deepcopy(drawOption_WplusJets),
			gammaPlusJetsSum = copy.deepcopy(drawOption_gammaPlusJets),
			qcdBCtoESum = copy.deepcopy(drawOption_QCD),
			qcdEMenrichedSum = copy.deepcopy(drawOption_darkBlue_stacked)
			#TTplusJets = copy.deepcopy(drawOption_TTplusJets)
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



