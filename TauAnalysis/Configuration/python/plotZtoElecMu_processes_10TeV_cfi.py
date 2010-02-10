import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.Configuration.plotterProcessDefinitions_cfi import *
from TauAnalysis.Configuration.recoSampleDefinitionsZtoElecMu_10TeV_cfi import *

#--------------------------------------------------------------------------------
# define for Z --> e + mu analysis names of .root files containing histograms
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
processZtoElecMu_Ztautau = copy.deepcopy(process_Ztautau)
processZtoElecMu_Ztautau.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecMu_Ztautau_10TeV_part01.root',
    'plotsZtoElecMu_Ztautau_10TeV_part02.root',
    'plotsZtoElecMu_Ztautau_10TeV_part03.root',
    'plotsZtoElecMu_Ztautau_10TeV_part04.root'
)
processZtoElecMu_Ztautau.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested/Ztautau')
processZtoElecMu_Ztautau.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoElecMu_Ztautau_10TeV*intLumiZtoElecMu_Data_10TeV/intLumiZtoElecMu_Ztautau_10TeV)

processZtoElecMu_ZtautauSum = copy.deepcopy(process_Ztautau)
processZtoElecMu_ZtautauSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecMu_ZtautauSum.root'
)
processZtoElecMu_ZtautauSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processZtoElecMu_ZtautauSum.config_dqmFileLoader.scaleFactor = cms.double(1.)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
processZtoElecMu_Zmumu = copy.deepcopy(process_Zmumu)
processZtoElecMu_Zmumu.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecMu_Zmumu_10TeV_part01.root',
    'plotsZtoElecMu_Zmumu_10TeV_part02.root',
    'plotsZtoElecMu_Zmumu_10TeV_part03.root',
    'plotsZtoElecMu_Zmumu_10TeV_part04.root',
    'plotsZtoElecMu_Zmumu_10TeV_part05.root'
)
processZtoElecMu_Zmumu.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested/Zmumu')
processZtoElecMu_Zmumu.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoElecMu_Zmumu_10TeV*intLumiZtoElecMu_Data_10TeV/intLumiZtoElecMu_Zmumu_10TeV)

processZtoElecMu_ZmumuSum = copy.deepcopy(process_Zmumu)
processZtoElecMu_ZmumuSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecMu_ZmumuSum.root'
)
processZtoElecMu_ZmumuSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processZtoElecMu_ZmumuSum.config_dqmFileLoader.scaleFactor = cms.double(1.)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
processZtoElecMu_ZeePlusJets = copy.deepcopy(process_ZeePlusJets)
processZtoElecMu_ZeePlusJets.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecMu_ZeePlusJets_10TeV_part01.root',
    'plotsZtoElecMu_ZeePlusJets_10TeV_part02.root',
    'plotsZtoElecMu_ZeePlusJets_10TeV_part03.root'
)
processZtoElecMu_ZeePlusJets.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested/ZeePlusJets')
processZtoElecMu_ZeePlusJets.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoElecMu_ZeePlusJets_10TeV*intLumiZtoElecMu_Data_10TeV/intLumiZtoElecMu_ZeePlusJets_10TeV)


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
    'plotsZtoElecMu_WplusJets_10TeV_part01.root',
    'plotsZtoElecMu_WplusJets_10TeV_part02.root',
    'plotsZtoElecMu_WplusJets_10TeV_part03.root',
    'plotsZtoElecMu_WplusJets_10TeV_part04.root', 
    'plotsZtoElecMu_WplusJets_10TeV_part05.root'
)
processZtoElecMu_WplusJets.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoElecMu_WplusJets_10TeV*intLumiZtoElecMu_Data_10TeV/intLumiZtoElecMu_WplusJets_10TeV)

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
    'plotsZtoElecMu_TTplusJets_10TeV_part01.root',
    'plotsZtoElecMu_TTplusJets_10TeV_part02.root',
    'plotsZtoElecMu_TTplusJets_10TeV_part03.root',
    'plotsZtoElecMu_TTplusJets_10TeV_part04.root',
    'plotsZtoElecMu_TTplusJets_10TeV_part05.root',
    'plotsZtoElecMu_TTplusJets_10TeV_part06.root',
    'plotsZtoElecMu_TTplusJets_10TeV_part07.root',
    'plotsZtoElecMu_TTplusJets_10TeV_part08.root',
    'plotsZtoElecMu_TTplusJets_10TeV_part09.root',
    'plotsZtoElecMu_TTplusJets_10TeV_part10.root',
    'plotsZtoElecMu_TTplusJets_10TeV_part11.root',
    'plotsZtoElecMu_TTplusJets_10TeV_part12.root'
)
processZtoElecMu_TTplusJets.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoElecMu_TTplusJets_10TeV*intLumiZtoElecMu_Data_10TeV/intLumiZtoElecMu_TTplusJets_10TeV)

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
    'plotsZtoElecMu_InclusivePPmuX_10TeV_part01.root',
    'plotsZtoElecMu_InclusivePPmuX_10TeV_part02.root'
)
processZtoElecMu_InclusivePPmuX.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested/InclusivePPmuX')
processZtoElecMu_InclusivePPmuX.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoElecMu_InclusivePPmuX_10TeV*intLumiZtoElecMu_Data_10TeV/intLumiZtoElecMu_InclusivePPmuX_10TeV)

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
    'plotsZtoElecMu_PPmuXptGt20_10TeV_part01.root',
    'plotsZtoElecMu_PPmuXptGt20_10TeV_part02.root',
    'plotsZtoElecMu_PPmuXptGt20_10TeV_part03.root',
    'plotsZtoElecMu_PPmuXptGt20_10TeV_part04.root',
    'plotsZtoElecMu_PPmuXptGt20_10TeV_part05.root',
    'plotsZtoElecMu_PPmuXptGt20_10TeV_part06.root',
    'plotsZtoElecMu_PPmuXptGt20_10TeV_part07.root',
    'plotsZtoElecMu_PPmuXptGt20_10TeV_part08.root',
    'plotsZtoElecMu_PPmuXptGt20_10TeV_part09.root',
    'plotsZtoElecMu_PPmuXptGt20_10TeV_part10.root',
    'plotsZtoElecMu_PPmuXptGt20_10TeV_part11.root',
    'plotsZtoElecMu_PPmuXptGt20_10TeV_part12.root',
    'plotsZtoElecMu_PPmuXptGt20_10TeV_part13.root',
    'plotsZtoElecMu_PPmuXptGt20_10TeV_part14.root',
    'plotsZtoElecMu_PPmuXptGt20_10TeV_part15.root',
    'plotsZtoElecMu_PPmuXptGt20_10TeV_part16.root',
    'plotsZtoElecMu_PPmuXptGt20_10TeV_part17.root',
    'plotsZtoElecMu_PPmuXptGt20_10TeV_part18.root',
    'plotsZtoElecMu_PPmuXptGt20_10TeV_part19.root',
    'plotsZtoElecMu_PPmuXptGt20_10TeV_part20.root',
    'plotsZtoElecMu_PPmuXptGt20_10TeV_part21.root',
    'plotsZtoElecMu_PPmuXptGt20_10TeV_part22.root',
    'plotsZtoElecMu_PPmuXptGt20_10TeV_part23.root',
    'plotsZtoElecMu_PPmuXptGt20_10TeV_part24.root',
    'plotsZtoElecMu_PPmuXptGt20_10TeV_part25.root'
)
processZtoElecMu_PPmuXptGt20.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested/PPmuXptGt20')
processZtoElecMu_PPmuXptGt20.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoElecMu_PPmuXptGt20_10TeV*intLumiZtoElecMu_Data_10TeV/intLumiZtoElecMu_PPmuXptGt20_10TeV)

processZtoElecMu_PPmuXptGt20Sum = copy.deepcopy(process_PPmuXptGt20)
processZtoElecMu_PPmuXptGt20Sum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoElecMu_PPmuXptGt20Sum.root'
)
processZtoElecMu_PPmuXptGt20Sum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processZtoElecMu_PPmuXptGt20Sum.config_dqmFileLoader.scaleFactor = cms.double(1.)
#--------------------------------------------------------------------------------
