import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.Configuration.plotterProcessDefinitions_cfi import *
from TauAnalysis.Configuration.recoSampleDefinitionsZtoElecMu_7TeV_cfi import *

#--------------------------------------------------------------------------------
# define for Z --> e + mu analysis names of .root files containing histograms
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
processZtoElecMu_Ztautau = copy.deepcopy(process_Ztautau)
processZtoElecMu_Ztautau.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecMu_Ztautau_7TeV_part01.root',
    'plotsZtoElecMu_Ztautau_7TeV_part02.root',
    'plotsZtoElecMu_Ztautau_7TeV_part03.root',
    'plotsZtoElecMu_Ztautau_7TeV_part04.root',
    'plotsZtoElecMu_Ztautau_7TeV_part05.root',
    'plotsZtoElecMu_Ztautau_7TeV_part06.root'
)
processZtoElecMu_Ztautau.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested/Ztautau')
processZtoElecMu_Ztautau.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoElecMu_Ztautau_7TeV*intLumiZtoElecMu_Data_7TeV/intLumiZtoElecMu_Ztautau_7TeV)

processZtoElecMu_ZtautauSum = copy.deepcopy(process_Ztautau)
processZtoElecMu_ZtautauSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecMu_ZtautauSum.root'
)
processZtoElecMu_ZtautauSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processZtoElecMu_ZtautauSum.config_dqmFileLoader.scaleFactor = cms.double(1.)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
processZtoElecMu_ZtautauPlusJets = copy.deepcopy(process_ZtautauPlusJets)
processZtoElecMu_ZtautauPlusJets.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecMu_ZtautauPlusJets_7TeV_part01.root',
    'plotsZtoElecMu_ZtautauPlusJets_7TeV_part02.root'
)
processZtoElecMu_ZtautauPlusJets.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested/ZtautauPlusJets')
processZtoElecMu_ZtautauPlusJets.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoElecMu_ZtautauPlusJets_7TeV*intLumiZtoElecMu_Data_7TeV/intLumiZtoElecMu_ZtautauPlusJets_7TeV)

processZtoElecMu_ZtautauPlusJetsSum = copy.deepcopy(process_ZtautauPlusJets)
processZtoElecMu_ZtautauPlusJetsSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecMu_ZtautauPlusJetsSum.root'
)
processZtoElecMu_ZtautauPlusJetsSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processZtoElecMu_ZtautauPlusJetsSum.config_dqmFileLoader.scaleFactor = cms.double(1.)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
processZtoElecMu_ZmumuPlusJets = copy.deepcopy(process_ZmumuPlusJets)
processZtoElecMu_ZmumuPlusJets.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecMu_ZmumuPlusJets_7TeV_part01.root',
    'plotsZtoElecMu_ZmumuPlusJets_7TeV_part02.root'
)
processZtoElecMu_ZmumuPlusJets.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested/ZmumuPlusJets')
processZtoElecMu_ZmumuPlusJets.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoElecMu_ZmumuPlusJets_7TeV*intLumiZtoElecMu_Data_7TeV/intLumiZtoElecMu_ZmumuPlusJets_7TeV)

processZtoElecMu_ZmumuPlusJetsSum = copy.deepcopy(process_ZmumuPlusJets)
processZtoElecMu_ZmumuPlusJetsSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecMu_ZmumuPlusJetsSum.root'
)
processZtoElecMu_ZmumuPlusJetsSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processZtoElecMu_ZmumuPlusJetsSum.config_dqmFileLoader.scaleFactor = cms.double(1.)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
processZtoElecMu_ZeePlusJets = copy.deepcopy(process_ZeePlusJets)
processZtoElecMu_ZeePlusJets.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecMu_ZeePlusJets_7TeV_part01.root',
    'plotsZtoElecMu_ZeePlusJets_7TeV_part02.root'
)
processZtoElecMu_ZeePlusJets.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested/ZeePlusJets')
processZtoElecMu_ZeePlusJets.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoElecMu_ZeePlusJets_7TeV*intLumiZtoElecMu_Data_7TeV/intLumiZtoElecMu_ZeePlusJets_7TeV)


processZtoElecMu_ZeePlusJetsSum = copy.deepcopy(process_ZeePlusJets)
processZtoElecMu_ZeePlusJetsSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecMu_ZeePlusJetsSum.root'
)
processZtoElecMu_ZeePlusJetsSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processZtoElecMu_ZeePlusJetsSum.config_dqmFileLoader.scaleFactor = cms.double(1.)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
processZtoElecMu_WplusJets = copy.deepcopy(process_WplusJets)
processZtoElecMu_WplusJets.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecMu_WplusJets_7TeV_part01.root',
    'plotsZtoElecMu_WplusJets_7TeV_part02.root',
    'plotsZtoElecMu_WplusJets_7TeV_part03.root',
    'plotsZtoElecMu_WplusJets_7TeV_part04.root', 
    'plotsZtoElecMu_WplusJets_7TeV_part05.root',
    'plotsZtoElecMu_WplusJets_7TeV_part06.root'
)
processZtoElecMu_WplusJets.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoElecMu_WplusJets_7TeV*intLumiZtoElecMu_Data_7TeV/intLumiZtoElecMu_WplusJets_7TeV)

processZtoElecMu_WplusJetsSum = copy.deepcopy(process_WplusJets)
processZtoElecMu_WplusJetsSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecMu_WplusJetsSum.root'
)
processZtoElecMu_WplusJetsSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processZtoElecMu_WplusJetsSum.config_dqmFileLoader.scaleFactor = cms.double(1.)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
processZtoElecMu_TTplusJets = copy.deepcopy(process_TTplusJets)
processZtoElecMu_TTplusJets.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecMu_TTplusJets_7TeV_part01.root',
    'plotsZtoElecMu_TTplusJets_7TeV_part02.root',
    'plotsZtoElecMu_TTplusJets_7TeV_part03.root',
    'plotsZtoElecMu_TTplusJets_7TeV_part04.root',
    'plotsZtoElecMu_TTplusJets_7TeV_part05.root',
    'plotsZtoElecMu_TTplusJets_7TeV_part06.root',
    'plotsZtoElecMu_TTplusJets_7TeV_part07.root',
    'plotsZtoElecMu_TTplusJets_7TeV_part08.root',
    'plotsZtoElecMu_TTplusJets_7TeV_part09.root',
    'plotsZtoElecMu_TTplusJets_7TeV_part10.root',
    'plotsZtoElecMu_TTplusJets_7TeV_part11.root',
    'plotsZtoElecMu_TTplusJets_7TeV_part12.root',
    'plotsZtoElecMu_TTplusJets_7TeV_part13.root',
    'plotsZtoElecMu_TTplusJets_7TeV_part14.root',
    'plotsZtoElecMu_TTplusJets_7TeV_part15.root',
    'plotsZtoElecMu_TTplusJets_7TeV_part16.root',
    'plotsZtoElecMu_TTplusJets_7TeV_part17.root',
    'plotsZtoElecMu_TTplusJets_7TeV_part18.root',
    'plotsZtoElecMu_TTplusJets_7TeV_part19.root',
    'plotsZtoElecMu_TTplusJets_7TeV_part20.root'
)
processZtoElecMu_TTplusJets.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoElecMu_TTplusJets_7TeV*intLumiZtoElecMu_Data_7TeV/intLumiZtoElecMu_TTplusJets_7TeV)

processZtoElecMu_TTplusJetsSum = copy.deepcopy(process_TTplusJets)
processZtoElecMu_TTplusJetsSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecMu_TTplusJetsSum.root'
)
processZtoElecMu_TTplusJetsSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processZtoElecMu_TTplusJetsSum.config_dqmFileLoader.scaleFactor = cms.double(1.)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
processZtoElecMu_InclusivePPmuX = copy.deepcopy(process_InclusivePPmuX)
processZtoElecMu_InclusivePPmuX.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecMu_InclusivePPmuX_7TeV_part01.root',
    'plotsZtoElecMu_InclusivePPmuX_7TeV_part02.root'
)
processZtoElecMu_InclusivePPmuX.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested/InclusivePPmuX')
processZtoElecMu_InclusivePPmuX.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoElecMu_InclusivePPmuX_7TeV*intLumiZtoElecMu_Data_7TeV/intLumiZtoElecMu_InclusivePPmuX_7TeV)

processZtoElecMu_InclusivePPmuXsum = copy.deepcopy(process_InclusivePPmuX)
processZtoElecMu_InclusivePPmuXsum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecMu_InclusivePPmuXSum.root'
)
processZtoElecMu_InclusivePPmuXsum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processZtoElecMu_InclusivePPmuXsum.config_dqmFileLoader.scaleFactor = cms.double(1.)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
processZtoElecMu_PPmuXptGt20 = copy.deepcopy(process_PPmuXptGt20)
processZtoElecMu_PPmuXptGt20.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecMu_PPmuXptGt20_7TeV_part01.root',
    'plotsZtoElecMu_PPmuXptGt20_7TeV_part02.root',
    'plotsZtoElecMu_PPmuXptGt20_7TeV_part03.root',
    'plotsZtoElecMu_PPmuXptGt20_7TeV_part04.root',
    'plotsZtoElecMu_PPmuXptGt20_7TeV_part05.root',
    'plotsZtoElecMu_PPmuXptGt20_7TeV_part06.root',
    'plotsZtoElecMu_PPmuXptGt20_7TeV_part07.root',
    'plotsZtoElecMu_PPmuXptGt20_7TeV_part08.root',
    'plotsZtoElecMu_PPmuXptGt20_7TeV_part09.root',
    'plotsZtoElecMu_PPmuXptGt20_7TeV_part10.root',
    'plotsZtoElecMu_PPmuXptGt20_7TeV_part11.root',
    'plotsZtoElecMu_PPmuXptGt20_7TeV_part12.root',
    'plotsZtoElecMu_PPmuXptGt20_7TeV_part13.root',
    'plotsZtoElecMu_PPmuXptGt20_7TeV_part14.root'
)
processZtoElecMu_PPmuXptGt20.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested/PPmuXptGt20')
processZtoElecMu_PPmuXptGt20.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoElecMu_PPmuXptGt20_7TeV*intLumiZtoElecMu_Data_7TeV/intLumiZtoElecMu_PPmuXptGt20_7TeV)

processZtoElecMu_PPmuXptGt20Sum = copy.deepcopy(process_PPmuXptGt20)
processZtoElecMu_PPmuXptGt20Sum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecMu_PPmuXptGt20Sum.root'
)
processZtoElecMu_PPmuXptGt20Sum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processZtoElecMu_PPmuXptGt20Sum.config_dqmFileLoader.scaleFactor = cms.double(1.)
#--------------------------------------------------------------------------------
