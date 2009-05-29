import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.Configuration.plotterProcessDefinitions_cfi import *
from TauAnalysis.Configuration.recoSampleDefinitionsZtoElecTau_cfi import *

#--------------------------------------------------------------------------------
# define for Z --> e + tau-jet analysis names of .root files containing histograms
#--------------------------------------------------------------------------------
#dataPath = cms.string('/data/jkolb/elecTauAnalysis/data/')
dataPath = cms.string('/castor/cern.ch/user/j/jkolb/elecTauAnalysis/data/')

processZtoElecTau_Ztautau = copy.deepcopy(process_Ztautau)
processZtoElecTau_Ztautau.config_dqmFileLoader.inputFileNames = cms.vstring(dataPath.value() + 'plotsZtoElecTau_Ztautau.root')
processZtoElecTau_Ztautau.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtautau*intLumiData/intLumiZtautau)

#--------------------------------------------------------------------------------

processZtoElecTau_Zee = copy.deepcopy(process_Zee)
processZtoElecTau_Zee.config_dqmFileLoader.inputFileNames = cms.vstring(dataPath.value() + 'plotsZtoElecTau_Zee.root')
processZtoElecTau_Zee.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZee*intLumiData/intLumiZee)

#--------------------------------------------------------------------------------

processZtoElecTau_gammaPlusJets_Pt15to20 = copy.deepcopy(process_gammaPlusJets)
processZtoElecTau_gammaPlusJets_Pt15to20.config_dqmFileLoader.inputFileNames = cms.vstring(dataPath.value() + 'plotsZtoElecTau_PhotonJets_Pt15to20.root')
processZtoElecTau_gammaPlusJets_Pt15to20.config_dqmFileLoader.scaleFactor = cms.double(corrFactorPhotonJets_Pt15to20*intLumiData/intLumiPhotonJets_Pt15to20)

processZtoElecTau_gammaPlusJets_Pt20to25 = copy.deepcopy(process_gammaPlusJets)
processZtoElecTau_gammaPlusJets_Pt20to25.config_dqmFileLoader.inputFileNames = cms.vstring(dataPath.value() + 'plotsZtoElecTau_PhotonJets_Pt20to25.root')
processZtoElecTau_gammaPlusJets_Pt20to25.config_dqmFileLoader.scaleFactor = cms.double(corrFactorPhotonJets_Pt20to25*intLumiData/intLumiPhotonJets_Pt20to25)

processZtoElecTau_gammaPlusJets_Pt25to30 = copy.deepcopy(process_gammaPlusJets)
processZtoElecTau_gammaPlusJets_Pt25to30.config_dqmFileLoader.inputFileNames = cms.vstring(dataPath.value() + 'plotsZtoElecTau_PhotonJets_Pt25to30.root')
processZtoElecTau_gammaPlusJets_Pt25to30.config_dqmFileLoader.scaleFactor = cms.double(corrFactorPhotonJets_Pt25to30*intLumiData/intLumiPhotonJets_Pt25to30)

processZtoElecTau_gammaPlusJets_Pt30to35 = copy.deepcopy(process_gammaPlusJets)
processZtoElecTau_gammaPlusJets_Pt30to35.config_dqmFileLoader.inputFileNames = cms.vstring(dataPath.value() + 'plotsZtoElecTau_PhotonJets_Pt30to35.root')
processZtoElecTau_gammaPlusJets_Pt30to35.config_dqmFileLoader.scaleFactor = cms.double(corrFactorPhotonJets_Pt30to35*intLumiData/intLumiPhotonJets_Pt30to35)

processZtoElecTau_gammaPlusJets_PtGt35 = copy.deepcopy(process_gammaPlusJets)
processZtoElecTau_gammaPlusJets_PtGt35.config_dqmFileLoader.inputFileNames = cms.vstring(dataPath.value() + 'plotsZtoElecTau_PhotonJets_PtGt35.root')
processZtoElecTau_gammaPlusJets_PtGt35.config_dqmFileLoader.scaleFactor = cms.double(corrFactorPhotonJets_PtGt35*intLumiData/intLumiPhotonJets_PtGt35)

#--------------------------------------------------------------------------------

processZtoElecTau_QCD_BCtoE_Pt20to30 = copy.deepcopy(process_QCD_BCtoE_Pt20to30)
processZtoElecTau_QCD_BCtoE_Pt20to30.config_dqmFileLoader.inputFileNames = cms.vstring(dataPath.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt20to30.root')
processZtoElecTau_QCD_BCtoE_Pt20to30.config_dqmFileLoader.scaleFactor = cms.double(corrFactorQCD_BCtoE_Pt20to30*intLumiData/intLumiQCD_BCtoE_Pt20to30)

processZtoElecTau_QCD_BCtoE_Pt30to80 = copy.deepcopy(process_QCD_BCtoE_Pt30to80)
processZtoElecTau_QCD_BCtoE_Pt30to80.config_dqmFileLoader.inputFileNames = cms.vstring(dataPath.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt30to80.root')
processZtoElecTau_QCD_BCtoE_Pt30to80.config_dqmFileLoader.scaleFactor = cms.double(corrFactorQCD_BCtoE_Pt30to80*intLumiData/intLumiQCD_BCtoE_Pt30to80)

processZtoElecTau_QCD_BCtoE_Pt80to170 = copy.deepcopy(process_QCD_BCtoE_Pt80to170)
processZtoElecTau_QCD_BCtoE_Pt80to170.config_dqmFileLoader.inputFileNames = cms.vstring(dataPath.value() + 'plotsZtoElecTau_QCD_BCtoE_Pt80to170.root')
processZtoElecTau_QCD_BCtoE_Pt80to170.config_dqmFileLoader.scaleFactor = cms.double(corrFactorQCD_BCtoE_Pt80to170*intLumiData/intLumiQCD_BCtoE_Pt80to170)

processZtoElecTau_QCD_EMenriched_Pt20to30 = copy.deepcopy(process_QCD_EMenriched_Pt20to30)
processZtoElecTau_QCD_EMenriched_Pt20to30.config_dqmFileLoader.inputFileNames = cms.vstring(dataPath.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt20to30_part1.root')
processZtoElecTau_QCD_EMenriched_Pt20to30.config_dqmFileLoader.scaleFactor = cms.double(corrFactorQCD_EMenriched_Pt20to30*intLumiData/intLumiQCD_EMenriched_Pt20to30)

processZtoElecTau_QCD_EMenriched_Pt30to80 = copy.deepcopy(process_QCD_EMenriched_Pt30to80)
processZtoElecTau_QCD_EMenriched_Pt30to80.config_dqmFileLoader.inputFileNames = cms.vstring(dataPath.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt30to80_part1.root')
processZtoElecTau_QCD_EMenriched_Pt30to80.config_dqmFileLoader.scaleFactor = cms.double(corrFactorQCD_EMenriched_Pt30to80*intLumiData/intLumiQCD_EMenriched_Pt30to80)

processZtoElecTau_QCD_EMenriched_Pt80to170 = copy.deepcopy(process_QCD_EMenriched_Pt80to170)
processZtoElecTau_QCD_EMenriched_Pt80to170.config_dqmFileLoader.inputFileNames = cms.vstring(dataPath.value() + 'plotsZtoElecTau_QCD_EMenriched_Pt80to170.root')
processZtoElecTau_QCD_EMenriched_Pt80to170.config_dqmFileLoader.scaleFactor = cms.double(corrFactorQCD_EMenriched_Pt80to170*intLumiData/intLumiQCD_EMenriched_Pt80to170)

#--------------------------------------------------------------------------------

processZtoElecTau_WplusJets = copy.deepcopy(process_WplusJets)
processZtoElecTau_WplusJets.config_dqmFileLoader.inputFileNames = cms.vstring(dataPath.value() + 'plotsZtoElecTau_WplusJets.root')
processZtoElecTau_WplusJets.config_dqmFileLoader.scaleFactor = cms.double(corrFactorWplusJets*intLumiData/intLumiWplusJets)

#--------------------------------------------------------------------------------

processZtoElecTau_ZplusJets = copy.deepcopy(process_ZplusJets)
processZtoElecTau_ZplusJets.config_dqmFileLoader.inputFileNames = cms.vstring(dataPath.value() + 'plotsZtoElecTau_ZplusJets.root')
processZtoElecTau_ZplusJets.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZplusJets*intLumiData/intLumiZplusJets)

processZtoElecTau_ZeePlusJets = copy.deepcopy(process_ZeePlusJets)
processZtoElecTau_ZeePlusJets.config_dqmFileLoader.inputFileNames = cms.vstring(dataPath.value() + 'plotsZtoElecTau_ZeePlusJets.root')
processZtoElecTau_ZeePlusJets.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZeePlusJets*intLumiData/intLumiZeePlusJets)

processZtoElecTau_ZtautauPlusJets = copy.deepcopy(process_ZtautauPlusJets)
processZtoElecTau_ZtautauPlusJets.config_dqmFileLoader.inputFileNames = cms.vstring(dataPath.value() + 'plotsZtoElecTau_ZtautauPlusJets.root')
processZtoElecTau_ZtautauPlusJets.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtautauPlusJets*intLumiData/intLumiZtautauPlusJets)

#--------------------------------------------------------------------------------

processZtoElecTau_TTplusJets = copy.deepcopy(process_TTplusJets)
processZtoElecTau_TTplusJets.config_dqmFileLoader.inputFileNames = cms.vstring(dataPath.value() + 'plotsZtoElecTau_TTplusJets.root')
processZtoElecTau_TTplusJets.config_dqmFileLoader.scaleFactor = cms.double(corrFactorTTplusJets*intLumiData/intLumiTTplusJets)
