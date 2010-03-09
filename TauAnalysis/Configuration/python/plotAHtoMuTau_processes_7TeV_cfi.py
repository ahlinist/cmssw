import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.Configuration.plotterProcessDefinitions_cfi import *
from TauAnalysis.Configuration.plotZtoMuTau_processes_7TeV_cfi import *
from TauAnalysis.Configuration.tools.replacementTools import replaceStrings
from TauAnalysis.Configuration.recoSampleDefinitionsAHtoMuTau_7TeV_cfi import *

#--------------------------------------------------------------------------------
# define for MSSM Higgs A/H --> mu + tau-jet analysis @ 7 TeV centre-of-mass energy
# names of .root files containing histograms
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
processAHtoMuTau_Ztautau = copy.deepcopy(processZtoMuTau_Ztautau)
processAHtoMuTau_Ztautau.config_dqmFileLoader.inputFileNames = \
  replaceStrings(processAHtoMuTau_Ztautau.config_dqmFileLoader.inputFileNames, "ZtoMuTau", "AHtoMuTau")
processAHtoMuTau_ZtautauSum = copy.deepcopy(processZtoMuTau_ZtautauSum)
processAHtoMuTau_ZtautauSum.config_dqmFileLoader.inputFileNames = \
  replaceStrings(processAHtoMuTau_ZtautauSum.config_dqmFileLoader.inputFileNames, "ZtoMuTau", "AHtoMuTau")
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
processAHtoMuTau_Zmumu = copy.deepcopy(processZtoMuTau_Zmumu)
processAHtoMuTau_Zmumu.config_dqmFileLoader.inputFileNames = \
  replaceStrings(processAHtoMuTau_Zmumu.config_dqmFileLoader.inputFileNames, "ZtoMuTau", "AHtoMuTau")
processAHtoMuTau_ZmumuSum = copy.deepcopy(processZtoMuTau_ZmumuSum)
processAHtoMuTau_ZmumuSum.config_dqmFileLoader.inputFileNames = \
  replaceStrings(processAHtoMuTau_ZmumuSum.config_dqmFileLoader.inputFileNames, "ZtoMuTau", "AHtoMuTau")
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
processAHtoMuTau_WplusJets = copy.deepcopy(processZtoMuTau_WplusJets)
processAHtoMuTau_WplusJets.config_dqmFileLoader.inputFileNames = \
  replaceStrings(processAHtoMuTau_WplusJets.config_dqmFileLoader.inputFileNames, "ZtoMuTau", "AHtoMuTau")
processAHtoMuTau_WplusJetsSum = copy.deepcopy(processZtoMuTau_WplusJetsSum)
processAHtoMuTau_WplusJetsSum.config_dqmFileLoader.inputFileNames = \
  replaceStrings(processAHtoMuTau_WplusJetsSum.config_dqmFileLoader.inputFileNames, "ZtoMuTau", "AHtoMuTau")
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
processAHtoMuTau_InclusivePPmuX = copy.deepcopy(processZtoMuTau_InclusivePPmuX)
processAHtoMuTau_InclusivePPmuX.config_dqmFileLoader.inputFileNames = \
  replaceStrings(processAHtoMuTau_InclusivePPmuX.config_dqmFileLoader.inputFileNames, "ZtoMuTau", "AHtoMuTau")
processAHtoMuTau_InclusivePPmuXsum = copy.deepcopy(processZtoMuTau_InclusivePPmuXsum)
processAHtoMuTau_InclusivePPmuXsum.config_dqmFileLoader.inputFileNames = \
  replaceStrings(processAHtoMuTau_InclusivePPmuXsum.config_dqmFileLoader.inputFileNames, "ZtoMuTau", "AHtoMuTau")
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
processAHtoMuTau_PPmuXptGt20_part01 = copy.deepcopy(processZtoMuTau_PPmuXptGt20_part01)
processAHtoMuTau_PPmuXptGt20_part01.config_dqmFileLoader.inputFileNames = \
  replaceStrings(processAHtoMuTau_PPmuXptGt20_part01.config_dqmFileLoader.inputFileNames, "ZtoMuTau", "AHtoMuTau")
processAHtoMuTau_PPmuXptGt20_part02 = copy.deepcopy(processZtoMuTau_PPmuXptGt20_part02)
processAHtoMuTau_PPmuXptGt20_part02.config_dqmFileLoader.inputFileNames = \
  replaceStrings(processAHtoMuTau_PPmuXptGt20_part02.config_dqmFileLoader.inputFileNames, "ZtoMuTau", "AHtoMuTau")
processAHtoMuTau_PPmuXptGt20_part03 = copy.deepcopy(processZtoMuTau_PPmuXptGt20_part03)
processAHtoMuTau_PPmuXptGt20_part03.config_dqmFileLoader.inputFileNames = \
  replaceStrings(processAHtoMuTau_PPmuXptGt20_part03.config_dqmFileLoader.inputFileNames, "ZtoMuTau", "AHtoMuTau")
processAHtoMuTau_PPmuXptGt20Sum = copy.deepcopy(processZtoMuTau_PPmuXptGt20Sum)
processAHtoMuTau_PPmuXptGt20Sum.config_dqmFileLoader.inputFileNames = \
  replaceStrings(processAHtoMuTau_PPmuXptGt20Sum.config_dqmFileLoader.inputFileNames, "ZtoMuTau", "AHtoMuTau")
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
processAHtoMuTau_TTplusJets = copy.deepcopy(processZtoMuTau_TTplusJets)
processAHtoMuTau_TTplusJets.config_dqmFileLoader.inputFileNames = \
  replaceStrings(processAHtoMuTau_TTplusJets.config_dqmFileLoader.inputFileNames, "ZtoMuTau", "AHtoMuTau")
processAHtoMuTau_TTplusJetsSum = copy.deepcopy(processZtoMuTau_TTplusJetsSum)
processAHtoMuTau_TTplusJetsSum.config_dqmFileLoader.inputFileNames = \
  replaceStrings(processAHtoMuTau_TTplusJetsSum.config_dqmFileLoader.inputFileNames, "ZtoMuTau", "AHtoMuTau")
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
processAHtoMuTau_AH_tautau = copy.deepcopy(process_AH_tautau)
processAHtoMuTau_AH_tautau.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsAHtoMuTau_AH_tautau_7TeV.root'
)
processAHtoMuTau_AH_tautau.config_dqmFileLoader.scaleFactor = cms.double(corrFactorAHtoMuTau_AH_tautau_7TeV*intLumiAHtoMuTau_Data_7TeV/intLumiAHtoMuTau_AH_tautau_7TeV)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
processAHtoMuTau_AHbb_tautau = copy.deepcopy(process_AHbb_tautau)
processAHtoMuTau_AHbb_tautau.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsAHtoMuTau_AHbb_tautau_7TeV.root'
)
processAHtoMuTau_AHbb_tautau.config_dqmFileLoader.scaleFactor = cms.double(corrFactorAHtoMuTau_AHbb_tautau_7TeV*intLumiAHtoMuTau_Data_7TeV/intLumiAHtoMuTau_AHbb_tautau_7TeV)
#--------------------------------------------------------------------------------
