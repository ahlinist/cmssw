import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.Configuration.plotterProcessDefinitions_cfi import *
from TauAnalysis.Configuration.recoSampleDefinitionsZtoElecTau_cfi import *

#--------------------------------------------------------------------------------
# define for Z --> e + tau-jet analysis names of .root files containing histograms
#--------------------------------------------------------------------------------

processZtoElecTau_Ztautau = copy.deepcopy(process_Ztautau)
processZtoElecTau_Ztautau.config_dqmFileLoader.inputFileNames = cms.vstring('/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_Ztautau.root')
processZtoElecTau_Ztautau.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtautau*intLumiData/intLumiZtautau)

#--------------------------------------------------------------------------------
processZtoElecTau_Zee = copy.deepcopy(process_Zee)
processZtoElecTau_Zee.config_dqmFileLoader.inputFileNames = cms.vstring('/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_Zee.root')
processZtoElecTau_Zee.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZee*intLumiData/intLumiZee)

#--------------------------------------------------------------------------------
processZtoElecTau_gammaPlusJets_Pt15to20 = copy.deepcopy(process_gammaPlusJets)
processZtoElecTau_gammaPlusJets_Pt15to20.config_dqmFileLoader.inputFileNames = cms.vstring('/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_PhotonJets_Pt15to20.root')
processZtoElecTau_gammaPlusJets_Pt15to20.config_dqmFileLoader.scaleFactor = cms.double(corrFactorPhotonJets_Pt15to20*intLumiData/intLumiPhotonJets_Pt15to20)

processZtoElecTau_gammaPlusJets_Pt20to25 = copy.deepcopy(process_gammaPlusJets)
processZtoElecTau_gammaPlusJets_Pt20to25.config_dqmFileLoader.inputFileNames = cms.vstring('/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_PhotonJets_Pt20to25.root')
processZtoElecTau_gammaPlusJets_Pt20to25.config_dqmFileLoader.scaleFactor = cms.double(corrFactorPhotonJets_Pt20to25*intLumiData/intLumiPhotonJets_Pt20to25)

processZtoElecTau_gammaPlusJets_Pt25to30 = copy.deepcopy(process_gammaPlusJets)
processZtoElecTau_gammaPlusJets_Pt25to30.config_dqmFileLoader.inputFileNames = cms.vstring('/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_PhotonJets_Pt25to30.root')
processZtoElecTau_gammaPlusJets_Pt25to30.config_dqmFileLoader.scaleFactor = cms.double(corrFactorPhotonJets_Pt25to30*intLumiData/intLumiPhotonJets_Pt25to30)

processZtoElecTau_gammaPlusJets_Pt30to35 = copy.deepcopy(process_gammaPlusJets)
processZtoElecTau_gammaPlusJets_Pt30to35.config_dqmFileLoader.inputFileNames = cms.vstring('/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_PhotonJets_Pt30to35.root')
processZtoElecTau_gammaPlusJets_Pt30to35.config_dqmFileLoader.scaleFactor = cms.double(corrFactorPhotonJets_Pt30to35*intLumiData/intLumiPhotonJets_Pt30to35)

processZtoElecTau_gammaPlusJets_PtGt35 = copy.deepcopy(process_gammaPlusJets)
processZtoElecTau_gammaPlusJets_PtGt35.config_dqmFileLoader.inputFileNames = cms.vstring('/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_PhotonJets_PtGt35.root')
processZtoElecTau_gammaPlusJets_PtGt35.config_dqmFileLoader.scaleFactor = cms.double(corrFactorPhotonJets_PtGt35*intLumiData/intLumiPhotonJets_PtGt35)
#--------------------------------------------------------------------------------
processZtoElecTau_QCD_BCtoE_Pt20to30 = copy.deepcopy(process_QCD_BCtoE_Pt20to30)
processZtoElecTau_QCD_BCtoE_Pt20to30.config_dqmFileLoader.inputFileNames = cms.vstring('/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt20to30_part1.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt20to30_part2.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt20to30_part3.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt20to30_part4.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt20to30_part5.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt20to30_part6.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt20to30_part7.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt20to30_part8.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt20to30_part9.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt20to30_part10.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt20to30_part11.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt20to30_part12.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt20to30_part13.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt20to30_part14.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt20to30_part15.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt20to30_part16.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt20to30_part17.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt20to30_part18.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt20to30_part19.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt20to30_part20.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt20to30_part21.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt20to30_part22.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt20to30_part23.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt20to30_part24.root'
																										)
processZtoElecTau_QCD_BCtoE_Pt20to30.config_dqmFileLoader.scaleFactor = cms.double(corrFactorQCD_BCtoE_Pt20to30*intLumiData/intLumiQCD_BCtoE_Pt20to30)
#--------------------------------------------------------------------------------
processZtoElecTau_QCD_BCtoE_Pt30to80 = copy.deepcopy(process_QCD_BCtoE_Pt30to80)
processZtoElecTau_QCD_BCtoE_Pt30to80.config_dqmFileLoader.inputFileNames = cms.vstring('/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt30to80_part1.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt30to80_part2.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt30to80_part3.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt30to80_part4.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt30to80_part5.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt30to80_part6.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt30to80_part7.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt30to80_part8.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt30to80_part9.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt30to80_part10.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt30to80_part11.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt30to80_part12.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt30to80_part13.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt30to80_part14.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt30to80_part15.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt30to80_part16.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt30to80_part17.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt30to80_part18.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt30to80_part19.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt30to80_part20.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt30to80_part21.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt30to80_part22.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt30to80_part23.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt30to80_part24.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt30to80_part25.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt30to80_part26.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt30to80_part27.root'
																										)
processZtoElecTau_QCD_BCtoE_Pt30to80.config_dqmFileLoader.scaleFactor = cms.double(corrFactorQCD_BCtoE_Pt30to80*intLumiData/intLumiQCD_BCtoE_Pt30to80)
#--------------------------------------------------------------------------------
processZtoElecTau_QCD_BCtoE_Pt80to170 = copy.deepcopy(process_QCD_BCtoE_Pt80to170)
processZtoElecTau_QCD_BCtoE_Pt80to170.config_dqmFileLoader.inputFileNames = cms.vstring('/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt80to170_part1.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt80to170_part2.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt80to170_part3.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt80to170_part4.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt80to170_part5.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt80to170_part6.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt80to170_part7.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt80to170_part8.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt80to170_part9.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt80to170_part10.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt80to170_part11.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt80to170_part12.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt80to170_part13.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt80to170_part14.root',
                                                                             '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_BCtoE_Pt80to170_part15.root'
																										)
processZtoElecTau_QCD_BCtoE_Pt80to170.config_dqmFileLoader.scaleFactor = cms.double(corrFactorQCD_BCtoE_Pt80to170*intLumiData/intLumiQCD_BCtoE_Pt80to170)
#--------------------------------------------------------------------------------
processZtoElecTau_QCD_EMenriched_Pt20to30 = copy.deepcopy(process_QCD_EMenriched_Pt20to30)
processZtoElecTau_QCD_EMenriched_Pt20to30.config_dqmFileLoader.inputFileNames = cms.vstring('/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt20to30_part1.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt20to30_part2.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt20to30_part3.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt20to30_part4.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt20to30_part5.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt20to30_part6.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt20to30_part7.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt20to30_part8.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt20to30_part9.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt20to30_part10.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt20to30_part11.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt20to30_part12.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt20to30_part13.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt20to30_part14.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt20to30_part15.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt20to30_part16.root'
																										)
processZtoElecTau_QCD_EMenriched_Pt20to30.config_dqmFileLoader.scaleFactor = cms.double(corrFactorQCD_EMenriched_Pt20to30*intLumiData/intLumiQCD_EMenriched_Pt20to30)
#--------------------------------------------------------------------------------
processZtoElecTau_QCD_EMenriched_Pt30to80 = copy.deepcopy(process_QCD_EMenriched_Pt30to80)
processZtoElecTau_QCD_EMenriched_Pt30to80.config_dqmFileLoader.inputFileNames = cms.vstring('/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part1.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part2.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part3.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part4.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part5.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part6.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part7.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part8.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part9.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part10.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part11.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part12.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part13.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part14.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part15.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part16.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part17.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part18.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part19.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part20.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part21.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part22.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part23.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part24.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part25.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part26.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part27.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part28.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part29.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part30.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part31.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part32.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part33.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part34.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part35.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part36.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part37.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part38.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part39.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part40.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part41.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part42.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part43.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part44.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part45.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part46.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part47.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part48.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part49.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part50.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part51.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part52.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part53.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part54.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part55.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part56.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part57.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part58.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part59.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part60.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part61.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part62.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part63.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part64.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part65.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part66.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part67.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part68.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part69.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part70.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part71.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part72.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part73.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part74.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part75.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part76.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part77.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part78.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part79.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part80.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part81.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part82.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt30to80_part83.root'
																										)
processZtoElecTau_QCD_EMenriched_Pt30to80.config_dqmFileLoader.scaleFactor = cms.double(corrFactorQCD_EMenriched_Pt30to80*intLumiData/intLumiQCD_EMenriched_Pt30to80)
#--------------------------------------------------------------------------------
processZtoElecTau_QCD_EMenriched_Pt80to170 = copy.deepcopy(process_QCD_EMenriched_Pt80to170)
processZtoElecTau_QCD_EMenriched_Pt80to170.config_dqmFileLoader.inputFileNames = cms.vstring('/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt80to170_part1.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt80to170_part2.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt80to170_part3.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt80to170_part4.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt80to170_part5.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt80to170_part6.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt80to170_part7.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt80to170_part8.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt80to170_part9.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt80to170_part10.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt80to170_part11.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt80to170_part12.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt80to170_part13.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt80to170_part14.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt80to170_part15.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt80to170_part16.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt80to170_part17.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt80to170_part18.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt80to170_part19.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt80to170_part20.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt80to170_part21.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt80to170_part22.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt80to170_part23.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt80to170_part24.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt80to170_part25.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt80to170_part26.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt80to170_part27.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt80to170_part28.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt80to170_part29.root',
                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_QCD_EMenriched_Pt80to170_part30.root'
																										)
processZtoElecTau_QCD_EMenriched_Pt80to170.config_dqmFileLoader.scaleFactor = cms.double(corrFactorQCD_EMenriched_Pt80to170*intLumiData/intLumiQCD_EMenriched_Pt80to170)
#--------------------------------------------------------------------------------

processZtoElecTau_WplusJets = copy.deepcopy(process_WplusJets)
processZtoElecTau_WplusJets.config_dqmFileLoader.inputFileNames = cms.vstring('/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_WplusJets.root'
#                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_Wjets_madgraph_part1.root',
#                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_Wjets_madgraph_part2.root',
#                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_Wjets_madgraph_part3.root',
#                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_Wjets_madgraph_part4.root',
#                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_Wjets_madgraph_part5.root',
#                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_Wjets_madgraph_part6.root',
#                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_Wjets_madgraph_part7.root',
#                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_Wjets_madgraph_part8.root',
#                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_Wjets_madgraph_part9.root',
#                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_Wjets_madgraph_part10.root',
#                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_Wjets_madgraph_part11.root',
#                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_Wjets_madgraph_part12.root',
#                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_Wjets_madgraph_part13.root',
#                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_Wjets_madgraph_part14.root',
#                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_Wjets_madgraph_part15.root',
#                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_Wjets_madgraph_part16.root',
#                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_Wjets_madgraph_part17.root',
#                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_Wjets_madgraph_part18.root'
																										)
processZtoElecTau_WplusJets.config_dqmFileLoader.scaleFactor = cms.double(corrFactorWplusJets*intLumiData/intLumiWplusJets)

#--------------------------------------------------------------------------------

processZtoElecTau_ZplusJets = copy.deepcopy(process_ZplusJets)
processZtoElecTau_ZplusJets.config_dqmFileLoader.inputFileNames = cms.vstring('/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_ZplusJets.root'
#                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_Zjets_madgraph_part1.root',
#                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_Zjets_madgraph_part2.root',
#                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_Zjets_madgraph_part3.root',
#                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_Zjets_madgraph_part4.root',
#                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_Zjets_madgraph_part5.root',
#                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_Zjets_madgraph_part6.root',
#                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_Zjets_madgraph_part7.root',
#                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_Zjets_madgraph_part8.root',
#                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_Zjets_madgraph_part9.root',
#                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_Zjets_madgraph_part10.root',
#                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_Zjets_madgraph_part11.root',
#                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_Zjets_madgraph_part12.root',
#                                                                              '/data/jkolb/elecTauAnalysis/data/plotsZtoElecTau_Zjets_madgraph_part13.root'
																										)
processZtoElecTau_WplusJets.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZplusJets*intLumiData/intLumiZplusJets)
