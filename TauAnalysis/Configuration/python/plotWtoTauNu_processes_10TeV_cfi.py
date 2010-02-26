import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.Configuration.plotterProcessDefinitions_cfi import *
from TauAnalysis.Configuration.recoSampleDefinitionsWtoTauNu_10TeV_cfi import *

#--------------------------------------------------------------------------------
# define for W --> nu tau-jet analysis names of .root files containing histograms
#--------------------------------------------------------------------------------

processWtoTauNu_Wtaunu = copy.deepcopy(process_Wtaunu)
processWtoTauNu_Wtaunu.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsWtoTauNu_Wtaunu_10TeV_part01.root',
    'plotsWtoTauNu_Wtaunu_10TeV_part02.root',
    'plotsWtoTauNu_Wtaunu_10TeV_part03.root',    
    'plotsWtoTauNu_Wtaunu_10TeV_part04.root',    
    'plotsWtoTauNu_Wtaunu_10TeV_part05.root'    
)

processWtoTauNu_Wtaunu.config_dqmFileLoader.scaleFactor = cms.double(corrFactorWtoTauNu_Wtaunu_10TeV*intLumiWtoTauNu_Data_10TeV/intLumiWtoTauNu_Wtaunu_10TeV)

processWtoTauNu_WtaunuSum = copy.deepcopy(process_Wtaunu)
processWtoTauNu_WtaunuSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsWtoTauNu_WtaunuSum.root'
)
processWtoTauNu_WtaunuSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processWtoTauNu_WtaunuSum.config_dqmFileLoader.scaleFactor = cms.double(1.)

#--------------------------------------------------------------------------------

processWtoTauNu_Wenu = copy.deepcopy(process_Wenu)
processWtoTauNu_Wenu.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsWtoTauNu_Wenu_10TeV_part01.root',
    'plotsWtoTauNu_Wenu_10TeV_part02.root',
    'plotsWtoTauNu_Wenu_10TeV_part03.root',    
    'plotsWtoTauNu_Wenu_10TeV_part04.root',    
    'plotsWtoTauNu_Wenu_10TeV_part05.root',  
    'plotsWtoTauNu_Wenu_10TeV_part06.root',
    'plotsWtoTauNu_Wenu_10TeV_part07.root',
    'plotsWtoTauNu_Wenu_10TeV_part08.root',    
    'plotsWtoTauNu_Wenu_10TeV_part09.root',    
    'plotsWtoTauNu_Wenu_10TeV_part10.root',  
    'plotsWtoTauNu_Wenu_10TeV_part11.root'  
)

processWtoTauNu_Wenu.config_dqmFileLoader.scaleFactor = cms.double(corrFactorWtoTauNu_Wenu_10TeV*intLumiWtoTauNu_Data_10TeV/intLumiWtoTauNu_Wenu_10TeV)

processWtoTauNu_WenuSum = copy.deepcopy(process_Wenu)
processWtoTauNu_WenuSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsWtoTauNu_WenuSum.root'
)
processWtoTauNu_WenuSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processWtoTauNu_WenuSum.config_dqmFileLoader.scaleFactor = cms.double(1.)

#--------------------------------------------------------------------------------

processWtoTauNu_Wmunu = copy.deepcopy(process_Wmunu)
processWtoTauNu_Wmunu.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsWtoTauNu_Wmunu_10TeV_part01.root',
    'plotsWtoTauNu_Wmunu_10TeV_part02.root',
    'plotsWtoTauNu_Wmunu_10TeV_part03.root',    
    'plotsWtoTauNu_Wmunu_10TeV_part04.root',    
    'plotsWtoTauNu_Wmunu_10TeV_part05.root',   
    'plotsWtoTauNu_Wmunu_10TeV_part06.root',
    'plotsWtoTauNu_Wmunu_10TeV_part07.root',
    'plotsWtoTauNu_Wmunu_10TeV_part08.root',    
    'plotsWtoTauNu_Wmunu_10TeV_part09.root',    
    'plotsWtoTauNu_Wmunu_10TeV_part10.root', 
    'plotsWtoTauNu_Wmunu_10TeV_part11.root',
    'plotsWtoTauNu_Wmunu_10TeV_part12.root',
    'plotsWtoTauNu_Wmunu_10TeV_part13.root',    
    'plotsWtoTauNu_Wmunu_10TeV_part14.root'    
)

processWtoTauNu_Wmunu.config_dqmFileLoader.scaleFactor = cms.double(corrFactorWtoTauNu_Wmunu_10TeV*intLumiWtoTauNu_Data_10TeV/intLumiWtoTauNu_Wmunu_10TeV)

processWtoTauNu_WmunuSum = copy.deepcopy(process_Wmunu)
processWtoTauNu_WmunuSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsWtoTauNu_WmunuSum.root'
)
processWtoTauNu_WmunuSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processWtoTauNu_WmunuSum.config_dqmFileLoader.scaleFactor = cms.double(1.)

#--------------------------------------------------------------------------------
processWtoTauNu_qcd_W = copy.deepcopy(process_qcd_W)
processWtoTauNu_qcd_W.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsWtoTauNu_qcd_W_10TeV_part01.root',
    'plotsWtoTauNu_qcd_W_10TeV_part02.root',
    'plotsWtoTauNu_qcd_W_10TeV_part03.root',    
    'plotsWtoTauNu_qcd_W_10TeV_part04.root',    
    'plotsWtoTauNu_qcd_W_10TeV_part05.root',   
    'plotsWtoTauNu_qcd_W_10TeV_part06.root',
    'plotsWtoTauNu_qcd_W_10TeV_part07.root',
    'plotsWtoTauNu_qcd_W_10TeV_part08.root',    
    'plotsWtoTauNu_qcd_W_10TeV_part09.root',    
    'plotsWtoTauNu_qcd_W_10TeV_part10.root', 
    'plotsWtoTauNu_qcd_W_10TeV_part11.root',
    'plotsWtoTauNu_qcd_W_10TeV_part12.root',
    'plotsWtoTauNu_qcd_W_10TeV_part13.root',    
    'plotsWtoTauNu_qcd_W_10TeV_part14.root',    
    'plotsWtoTauNu_qcd_W_10TeV_part15.root',
    'plotsWtoTauNu_qcd_W_10TeV_part16.root',    
    'plotsWtoTauNu_qcd_W_10TeV_part17.root'  
)

processWtoTauNu_qcd_W.config_dqmFileLoader.scaleFactor = cms.double(corrFactorWtoTauNu_qcd_W_10TeV*intLumiWtoTauNu_Data_10TeV/intLumiWtoTauNu_qcd_W_10TeV)

processWtoTauNu_qcd_WSum = copy.deepcopy(process_qcd_W)
processWtoTauNu_qcd_WSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsWtoTauNu_qcd_WSum.root'
)
processWtoTauNu_qcd_WSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processWtoTauNu_qcd_WSum.config_dqmFileLoader.scaleFactor = cms.double(1.)

#--------------------------------------------------------------------------------
