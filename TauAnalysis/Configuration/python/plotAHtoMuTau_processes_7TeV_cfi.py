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
processAHtoMuTau_ZplusJets = copy.deepcopy(process_ZplusJets)
processAHtoMuTau_ZplusJets.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsAHtoMuTau_ZplusJets_7TeV_part01.root',
    #'plotsAHtoMuTau_ZplusJets_7TeV_part02.root',
    'plotsAHtoMuTau_ZplusJets_7TeV_part03.root',
    'plotsAHtoMuTau_ZplusJets_7TeV_part04.root',
    'plotsAHtoMuTau_ZplusJets_7TeV_part05.root',
    'plotsAHtoMuTau_ZplusJets_7TeV_part06.root',
    'plotsAHtoMuTau_ZplusJets_7TeV_part07.root',
    'plotsAHtoMuTau_ZplusJets_7TeV_part08.root'
)
processAHtoMuTau_ZplusJets.config_dqmFileLoader.scaleFactor = cms.double(corrFactorAHtoMuTau_ZplusJets_7TeV*intLumiAHtoMuTau_Data_7TeV/intLumiAHtoMuTau_ZplusJets_7TeV)

processAHtoMuTau_ZplusJetsSum = copy.deepcopy(process_ZplusJets)
processAHtoMuTau_ZplusJetsSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsAHtoMuTau_ZplusJetsSum.root'
)
processAHtoMuTau_ZplusJetsSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processAHtoMuTau_ZplusJetsSum.config_dqmFileLoader.scaleFactor = cms.double(1.)
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
processAHtoMuTau_Vqq = copy.deepcopy(process_Vqq)
processAHtoMuTau_Vqq.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsAHtoMuTau_Vqq_7TeV_part01.root',
    #'plotsAHtoMuTau_Vqq_7TeV_part02.root',
    'plotsAHtoMuTau_Vqq_7TeV_part03.root',
    'plotsAHtoMuTau_Vqq_7TeV_part04.root',
    #'plotsAHtoMuTau_Vqq_7TeV_part05.root',
    'plotsAHtoMuTau_Vqq_7TeV_part06.root',
    'plotsAHtoMuTau_Vqq_7TeV_part07.root',
    'plotsAHtoMuTau_Vqq_7TeV_part08.root',
    'plotsAHtoMuTau_Vqq_7TeV_part09.root',
    #'plotsAHtoMuTau_Vqq_7TeV_part10.root',
    'plotsAHtoMuTau_Vqq_7TeV_part11.root',
    'plotsAHtoMuTau_Vqq_7TeV_part12.root',
    'plotsAHtoMuTau_Vqq_7TeV_part13.root',
    'plotsAHtoMuTau_Vqq_7TeV_part14.root',
    'plotsAHtoMuTau_Vqq_7TeV_part15.root',
    #'plotsAHtoMuTau_Vqq_7TeV_part16.root',
    'plotsAHtoMuTau_Vqq_7TeV_part17.root',
    'plotsAHtoMuTau_Vqq_7TeV_part18.root',
    'plotsAHtoMuTau_Vqq_7TeV_part19.root',
    'plotsAHtoMuTau_Vqq_7TeV_part20.root',
    #'plotsAHtoMuTau_Vqq_7TeV_part21.root',
    'plotsAHtoMuTau_Vqq_7TeV_part22.root',
    'plotsAHtoMuTau_Vqq_7TeV_part23.root'
)
processAHtoMuTau_Vqq.config_dqmFileLoader.scaleFactor = cms.double(corrFactorAHtoMuTau_Vqq_7TeV*intLumiAHtoMuTau_Data_7TeV/intLumiAHtoMuTau_Vqq_7TeV)

processAHtoMuTau_VqqSum = copy.deepcopy(process_Vqq)
processAHtoMuTau_VqqSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsAHtoMuTau_VqqSum.root'
)
processAHtoMuTau_VqqSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processAHtoMuTau_VqqSum.config_dqmFileLoader.scaleFactor = cms.double(1.)
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
processAHtoMuTau_AH120_tautau = copy.deepcopy(process_AH120_tautau)
processAHtoMuTau_AH120_tautau.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsAHtoMuTau_AH120_tautau_7TeV_part01.root',
    'plotsAHtoMuTau_AH120_tautau_7TeV_part02.root',
    'plotsAHtoMuTau_AH120_tautau_7TeV_part03.root',
    'plotsAHtoMuTau_AH120_tautau_7TeV_part04.root',
    'plotsAHtoMuTau_AH120_tautau_7TeV_part05.root',
    'plotsAHtoMuTau_AH120_tautau_7TeV_part06.root',
    'plotsAHtoMuTau_AH120_tautau_7TeV_part07.root',
    'plotsAHtoMuTau_AH120_tautau_7TeV_part08.root'
)
processAHtoMuTau_AH120_tautau.config_dqmFileLoader.scaleFactor = cms.double(corrFactorAHtoMuTau_AH120_tautau_7TeV*intLumiAHtoMuTau_Data_7TeV/intLumiAHtoMuTau_AH120_tautau_7TeV)

processAHtoMuTau_AH120_tautauSum = copy.deepcopy(process_AH120_tautau)
processAHtoMuTau_AH120_tautauSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsAHtoMuTau_AH120_tautauSum.root'
)
processAHtoMuTau_AH120_tautauSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processAHtoMuTau_AH120_tautauSum.config_dqmFileLoader.scaleFactor = cms.double(1.)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
processAHtoMuTau_AHbb120_tautau = copy.deepcopy(process_AHbb120_tautau)
processAHtoMuTau_AHbb120_tautau.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsAHtoMuTau_AHbb120_tautau_7TeV_part01.root',
    'plotsAHtoMuTau_AHbb120_tautau_7TeV_part02.root',
    'plotsAHtoMuTau_AHbb120_tautau_7TeV_part03.root',
    'plotsAHtoMuTau_AHbb120_tautau_7TeV_part04.root'    
)
processAHtoMuTau_AHbb120_tautau.config_dqmFileLoader.scaleFactor = cms.double(corrFactorAHtoMuTau_AHbb120_tautau_7TeV*intLumiAHtoMuTau_Data_7TeV/intLumiAHtoMuTau_AHbb120_tautau_7TeV)

processAHtoMuTau_AHbb120_tautauSum = copy.deepcopy(process_AHbb120_tautau)
processAHtoMuTau_AHbb120_tautauSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsAHtoMuTau_AHbb120_tautauSum.root'
)
processAHtoMuTau_AHbb120_tautauSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processAHtoMuTau_AHbb120_tautauSum.config_dqmFileLoader.scaleFactor = cms.double(1.)
#--------------------------------------------------------------------------------
