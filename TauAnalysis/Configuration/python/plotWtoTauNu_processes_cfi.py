import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.Configuration.plotterProcessDefinitions_cfi import *
from TauAnalysis.Configuration.recoSampleDefinitionsWtoTauNu_cfi import *

#read data when harvesting +read harvested data when plotting
plotDirectoryName = cms.string("rfio:/castor/cern.ch/user/l/liis/WTauNuPlots/")
#plotDirectoryName = cms.string("/afs/cern.ch/user/l/liis/scratch0/CMSSW_2_2_13/src/TauAnalysis/Configuration/test/TESTING/") 

#--------------------------------------------------------------------------------
# define for W --> nu tau-jet analysis names of .root files containing histograms
#--------------------------------------------------------------------------------

processWtoTauNu_Wtaunu = copy.deepcopy(process_Wtaunu)
processWtoTauNu_Wtaunu.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsWtoTauNu_Wtaunu_part01.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_Wtaunu_part02.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_Wtaunu_part03.root',    
    plotDirectoryName.value() + 'plotsWtoTauNu_Wtaunu_part04.root'    
)

processWtoTauNu_Wtaunu.config_dqmFileLoader.scaleFactor = cms.double(corrFactorWtoTauNu_Wtaunu*intLumiWtoTauNu_Data/intLumiWtoTauNu_Wtaunu)

processWtoTauNu_WtaunuSum = copy.deepcopy(process_Wtaunu)
processWtoTauNu_WtaunuSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsWtoTauNu_WtaunuSum.root'
)
processWtoTauNu_WtaunuSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')

processWtoTauNu_WtaunuSum.config_dqmFileLoader.scaleFactor = cms.double(1.)

#--------------------------------------------------------------------------------

processWtoTauNu_qcd_W = copy.deepcopy(process_qcd_W)
processWtoTauNu_qcd_W.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsWtoTauNu_qcd_W_part01.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_qcd_W_part02.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_qcd_W_part03.root', 
    plotDirectoryName.value() + 'plotsWtoTauNu_qcd_W_part04.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_qcd_W_part05.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_qcd_W_part06.root', 
    plotDirectoryName.value() + 'plotsWtoTauNu_qcd_W_part07.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_qcd_W_part08.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_qcd_W_part09.root', 
    plotDirectoryName.value() + 'plotsWtoTauNu_qcd_W_part10.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_qcd_W_part11.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_qcd_W_part12.root', 
    plotDirectoryName.value() + 'plotsWtoTauNu_qcd_W_part13.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_qcd_W_part14.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_qcd_W_part15.root', 
    plotDirectoryName.value() + 'plotsWtoTauNu_qcd_W_part16.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_qcd_W_part17.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_qcd_W_part18.root', 
    plotDirectoryName.value() + 'plotsWtoTauNu_qcd_W_part19.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_qcd_W_part20.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_qcd_W_part21.root', 
    plotDirectoryName.value() + 'plotsWtoTauNu_qcd_W_part22.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_qcd_W_part23.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_qcd_W_part24.root', 
    plotDirectoryName.value() + 'plotsWtoTauNu_qcd_W_part25.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_qcd_W_part26.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_qcd_W_part27.root', 
    plotDirectoryName.value() + 'plotsWtoTauNu_qcd_W_part28.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_qcd_W_part29.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_qcd_W_part30.root', 
    plotDirectoryName.value() + 'plotsWtoTauNu_qcd_W_part31.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_qcd_W_part32.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_qcd_W_part33.root'
    )

processWtoTauNu_qcd_W.config_dqmFileLoader.scaleFactor = cms.double(corrFactorWtoTauNu_qcd_W*intLumiWtoTauNu_Data/intLumiWtoTauNu_qcd_W)

processWtoTauNu_qcd_WSum = copy.deepcopy(process_qcd_W)
processWtoTauNu_qcd_WSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsWtoTauNu_qcd_WSum.root'
)
processWtoTauNu_qcd_WSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')

processWtoTauNu_qcd_WSum.config_dqmFileLoader.scaleFactor = cms.double(1.)

#--------------------------------------------------------------------------------

processWtoTauNu_Wmunu = copy.deepcopy(process_Wmunu)
processWtoTauNu_Wmunu.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsWtoTauNu_Wmunu_part01.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_Wmunu_part02.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_Wmunu_part03.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_Wmunu_part04.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_Wmunu_part05.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_Wmunu_part06.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_Wmunu_part07.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_Wmunu_part08.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_Wmunu_part09.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_Wmunu_part10.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_Wmunu_part11.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_Wmunu_part12.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_Wmunu_part13.root'
)


processWtoTauNu_Wmunu.config_dqmFileLoader.scaleFactor = cms.double(corrFactorWtoTauNu_Wmunu*intLumiWtoTauNu_Data/intLumiWtoTauNu_Wmunu)

processWtoTauNu_WmunuSum = copy.deepcopy(process_Wmunu)
processWtoTauNu_WmunuSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsWtoTauNu_WmunuSum.root'
)
processWtoTauNu_WmunuSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processWtoTauNu_WmunuSum.config_dqmFileLoader.scaleFactor = cms.double(1.)

#--------------------------------------------------------------------------------

processWtoTauNu_Wenu = copy.deepcopy(process_Wenu)
processWtoTauNu_Wenu.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsWtoTauNu_Wenu_part01.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_Wenu_part02.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_Wenu_part03.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_Wenu_part04.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_Wenu_part05.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_Wenu_part06.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_Wenu_part07.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_Wenu_part08.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_Wenu_part09.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_Wenu_part10.root'
)


processWtoTauNu_Wenu.config_dqmFileLoader.scaleFactor = cms.double(corrFactorWtoTauNu_Wenu*intLumiWtoTauNu_Data/intLumiWtoTauNu_Wenu)

processWtoTauNu_WenuSum = copy.deepcopy(process_Wenu)
processWtoTauNu_WenuSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsWtoTauNu_WenuSum.root'
)
processWtoTauNu_WenuSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processWtoTauNu_WenuSum.config_dqmFileLoader.scaleFactor = cms.double(1.)

#--------------------------------------------------------------------------------

processWtoTauNu_ZplusJets = copy.deepcopy(process_ZplusJets)
processWtoTauNu_ZplusJets.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsWtoTauNu_ZplusJets_part01.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_ZplusJets_part02.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_ZplusJets_part03.root',  
    plotDirectoryName.value() + 'plotsWtoTauNu_ZplusJets_part04.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_ZplusJets_part05.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_ZplusJets_part06.root', 
    plotDirectoryName.value() + 'plotsWtoTauNu_ZplusJets_part07.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_ZplusJets_part08.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_ZplusJets_part09.root',   
    plotDirectoryName.value() + 'plotsWtoTauNu_ZplusJets_part10.root'
)


processWtoTauNu_ZplusJets.config_dqmFileLoader.scaleFactor = cms.double(corrFactorWtoTauNu_ZplusJets*intLumiWtoTauNu_Data/intLumiWtoTauNu_ZplusJets)

processWtoTauNu_ZplusJetsSum = copy.deepcopy(process_ZplusJets)
processWtoTauNu_ZplusJetsSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsWtoTauNu_ZplusJetsSum.root'
)
processWtoTauNu_ZplusJetsSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processWtoTauNu_ZplusJetsSum.config_dqmFileLoader.scaleFactor = cms.double(1.)

#--------------------------------------------------------------------------------



