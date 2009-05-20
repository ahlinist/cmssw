import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.Configuration.plotterProcessDefinitions_cfi import *

#--------------------------------------------------------------------------------
# define for Z --> e + tau-jet analysis names of .root files containing histograms
#--------------------------------------------------------------------------------

processZtoElecTau_Ztautau = copy.deepcopy(process_Ztautau)
processZtoElecTau_Ztautau.config_dqmFileLoader.inputFileNames = cms.vstring('plotsZtoElecTau_Ztautau_part01.root')
                                                              

processZtoElecTau_gammaPlusJets = copy.deepcopy(process_gammaPlusJets)
processZtoElecTau_gammaPlusJets.config_dqmFileLoader.inputFileNames = cms.vstring('plotsZtoElecTau_PhotonJets_Pt15to20.root',
                                                                               'plotsZtoElecTau_PhotonJets_Pt20to25.root',
                                                                               'plotsZtoElecTau_PhotonJets_Pt25to30.root'
                                                                              )


#processZtoElecTau_WplusJets = copy.deepcopy(process_WplusJets)
#processZtoElecTau_WplusJets.config_dqmFileLoader.inputFileNames = cms.vstring('plotsZtoElecTau_WplusJets_part01.root',
#                                                                            'plotsZtoElecTau_WplusJets_part02.root',
#                                                                            'plotsZtoElecTau_WplusJets_part03.root',
#                                                                            'plotsZtoElecTau_WplusJets_part04.root',
#                                                                            'plotsZtoElecTau_WplusJets_part05.root',
#                                                                            'plotsZtoElecTau_WplusJets_part06.root')

#processZtoElecTau_InclusivePPmuX = copy.deepcopy(process_InclusivePPmuX)
#processZtoElecTau_InclusivePPmuX.config_dqmFileLoader.inputFileNames = cms.vstring('plotsZtoElecTau_InclusivePPmuX.root')

#processZtoElecTau_PPmuXptGt20 = copy.deepcopy(process_PPmuXptGt20)
#processZtoElecTau_PPmuXptGt20.config_dqmFileLoader.inputFileNames = cms.vstring('plotsZtoElecTau_PPmuXptGt20_part01.root',
#                                                                              'plotsZtoElecTau_PPmuXptGt20_part02.root',
#                                                                              'plotsZtoElecTau_PPmuXptGt20_part03.root',
#                                                                              'plotsZtoElecTau_PPmuXptGt20_part04.root',
#                                                                              'plotsZtoElecTau_PPmuXptGt20_part05.root',
#                                                                              'plotsZtoElecTau_PPmuXptGt20_part06.root',
#                                                                              'plotsZtoElecTau_PPmuXptGt20_part07.root',
#                                                                              'plotsZtoElecTau_PPmuXptGt20_part08.root',
#                                                                              'plotsZtoElecTau_PPmuXptGt20_part09.root',
#                                                                              'plotsZtoElecTau_PPmuXptGt20_part10.root',
#                                                                              'plotsZtoElecTau_PPmuXptGt20_part11.root',
#                                                                              'plotsZtoElecTau_PPmuXptGt20_part12.root',
#                                                                              'plotsZtoElecTau_PPmuXptGt20_part13.root',
#                                                                              'plotsZtoElecTau_PPmuXptGt20_part14.root',
#                                                                              #'plotsZtoElecTau_PPmuXptGt20_part15.root',
#                                                                              'plotsZtoElecTau_PPmuXptGt20_part16.root',
#                                                                              #'plotsZtoElecTau_PPmuXptGt20_part17.root',
#                                                                              'plotsZtoElecTau_PPmuXptGt20_part18.root',
#                                                                              'plotsZtoElecTau_PPmuXptGt20_part19.root',
#                                                                              #'plotsZtoElecTau_PPmuXptGt20_part20.root',
 #                                                                             'plotsZtoElecTau_PPmuXptGt20_part21.root',
 #                                                                             #'plotsZtoElecTau_PPmuXptGt20_part22.root',
#                                                                              #'plotsZtoElecTau_PPmuXptGt20_part23.root',
#                                                                              #'plotsZtoElecTau_PPmuXptGt20_part24.root',
#                                                                              #'plotsZtoElecTau_PPmuXptGt20_part25.root',
#                                                                              #'plotsZtoElecTau_PPmuXptGt20_part26.root',
#                                                                              'plotsZtoElecTau_PPmuXptGt20_part27.root',
#                                                                              'plotsZtoElecTau_PPmuXptGt20_part28.root',
#                                                                              #'plotsZtoElecTau_PPmuXptGt20_part29.root',
#                                                                              'plotsZtoElecTau_PPmuXptGt20_part30.root',
#                                                                              'plotsZtoElecTau_PPmuXptGt20_part31.root',
#                                                                              #'plotsZtoElecTau_PPmuXptGt20_part32.root',
#                                                                              #'plotsZtoElecTau_PPmuXptGt20_part33.root',
#                                                                              #'plotsZtoElecTau_PPmuXptGt20_part34.root',
#                                                                              'plotsZtoElecTau_PPmuXptGt20_part35.root')
# correct scaleFactor for missing PPmuXptGt20 Monte Carlo files
#processZtoElecTau_PPmuXptGt20.config_dqmFileLoader.scaleFactor = cms.double(10.5*dataIntLumi/49.7)
#processZtoElecTau_PPmuXptGt20.config_dqmFileLoader.scaleFactor = value(processZtoElecTau_PPmuXptGt20.config_dqmFileLoader.scaleFactor)

