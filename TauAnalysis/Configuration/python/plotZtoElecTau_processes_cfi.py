import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.Configuration.plotterProcessDefinitions_cfi import *
from TauAnalysis.Configuration.recoSampleDefinitionsZtoElecTau_cfi import *

#--------------------------------------------------------------------------------
# define for Z --> e + tau-jet analysis names of .root files containing histograms
#--------------------------------------------------------------------------------

processZtoElecTau_Ztautau = copy.deepcopy(process_Ztautau)
processZtoElecTau_Ztautau.config_dqmFileLoader.inputFileNames = cms.vstring('plotsZtoElecTau_Ztautau_part01.root',
                                                                            'plotsZtoElecTau_Ztautau_part02.root',
                                                                            'plotsZtoElecTau_Ztautau_part03.root',
                                                                            'plotsZtoElecTau_Ztautau_part04.root',
                                                                            'plotsZtoElecTau_Ztautau_part05.root')
processZtoElecTau_Ztautau.config_dqmFileLoader.scaleFactor = cms.double(intLumiData/intLumiZtautau)

#--------------------------------------------------------------------------------
processZtoElecTau_gammaPlusJets_Pt15to20 = copy.deepcopy(process_gammaPlusJets)
processZtoElecTau_gammaPlusJets_Pt15to20.config_dqmFileLoader.inputFileNames = cms.vstring('plotsZtoElecTau_PhotonJets_Pt15to20.root')
processZtoElecTau_gammaPlusJets_Pt15to20.config_dqmFileLoader.scaleFactor = cms.double(intLumiData/intLumiPhotonJets_Pt15to20)

processZtoElecTau_gammaPlusJets_Pt20to25 = copy.deepcopy(process_gammaPlusJets)
processZtoElecTau_gammaPlusJets_Pt20to25.config_dqmFileLoader.inputFileNames = cms.vstring('plotsZtoElecTau_PhotonJets_Pt20to25.root')
processZtoElecTau_gammaPlusJets_Pt20to25.config_dqmFileLoader.scaleFactor = cms.double(intLumiData/intLumiPhotonJets_Pt20to25)

processZtoElecTau_gammaPlusJets_Pt25to30 = copy.deepcopy(process_gammaPlusJets)
processZtoElecTau_gammaPlusJets_Pt25to30.config_dqmFileLoader.inputFileNames = cms.vstring('plotsZtoElecTau_PhotonJets_Pt25to30.root')
processZtoElecTau_gammaPlusJets_Pt25to30.config_dqmFileLoader.scaleFactor = cms.double(intLumiData/intLumiPhotonJets_Pt25to30)

processZtoElecTau_gammaPlusJets_Pt30to35 = copy.deepcopy(process_gammaPlusJets)
processZtoElecTau_gammaPlusJets_Pt30to35.config_dqmFileLoader.inputFileNames = cms.vstring('plotsZtoElecTau_PhotonJets_Pt30to35.root')
processZtoElecTau_gammaPlusJets_Pt30to35.config_dqmFileLoader.scaleFactor = cms.double(intLumiData/intLumiPhotonJets_Pt30to35)

processZtoElecTau_gammaPlusJets_PtGt35 = copy.deepcopy(process_gammaPlusJets)
processZtoElecTau_gammaPlusJets_PtGt35.config_dqmFileLoader.inputFileNames = cms.vstring('plotsZtoElecTau_PhotonJets_PtGt35.root')
processZtoElecTau_gammaPlusJets_PtGt35.config_dqmFileLoader.scaleFactor = cms.double(intLumiData/intLumiPhotonJets_PtGt35)
#--------------------------------------------------------------------------------

#processZtoElecTau_WplusJets = copy.deepcopy(process_WplusJets)
#processZtoElecTau_WplusJets.config_dqmFileLoader.inputFileNames = cms.vstring('plotsZtoElecTau_WplusJets_part01.root',
#                                                                              'plotsZtoElecTau_WplusJets_part02.root',
#                                                                              'plotsZtoElecTau_WplusJets_part03.root',
#                                                                              'plotsZtoElecTau_WplusJets_part04.root',
#                                                                              'plotsZtoElecTau_WplusJets_part05.root',
#                                                                              'plotsZtoElecTau_WplusJets_part06.root')
#processZtoElecTau_WplusJets.config_dqmFileLoader.scaleFactor = cms.double(intLumiData/intLumiWplusJets)

