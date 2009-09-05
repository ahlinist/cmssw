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
    plotDirectoryName.value() + 'plotsWtoTauNu_Wtaunu_part03.root'    
)

processWtoTauNu_Wtaunu.config_dqmFileLoader.scaleFactor = cms.double(corrFactorWtaunu*intLumiData/intLumiWtaunu)

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
    plotDirectoryName.value() + 'plotsWtoTauNu_qcd_W_part03.root'    
)

processWtoTauNu_qcd_W.config_dqmFileLoader.scaleFactor = cms.double(corrFactorqcd_W*intLumiData/intLumiqcd_W)

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
    plotDirectoryName.value() + 'plotsWtoTauNu_Wmunu_part03.root'
)


processWtoTauNu_Wmunu.config_dqmFileLoader.scaleFactor = cms.double(corrFactorWmunu*intLumiData/intLumiWmunu)

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
    plotDirectoryName.value() + 'plotsWtoTauNu_Wenu_part03.root'
)


processWtoTauNu_Wenu.config_dqmFileLoader.scaleFactor = cms.double(corrFactorWenu*intLumiData/intLumiWenu)

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
    plotDirectoryName.value() + 'plotsWtoTauNu_ZplusJets_part03.root'    
)


processWtoTauNu_ZplusJets.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZplusJets*intLumiData/intLumiZplusJets)

processWtoTauNu_ZplusJetsSum = copy.deepcopy(process_ZplusJets)
processWtoTauNu_ZplusJetsSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsWtoTauNu_ZplusJetsSum.root'
)
processWtoTauNu_ZplusJetsSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processWtoTauNu_ZplusJetsSum.config_dqmFileLoader.scaleFactor = cms.double(1.)

#--------------------------------------------------------------------------------

processWtoTauNu_TTplusJets = copy.deepcopy(process_TTplusJets)
processWtoTauNu_TTplusJets.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsWtoTauNu_TTplusJets_part01.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_TTplusJets_part02.root',
    plotDirectoryName.value() + 'plotsWtoTauNu_TTplusJets_part03.root'
)


processWtoTauNu_TTplusJets.config_dqmFileLoader.scaleFactor = cms.double(corrFactorTTplusJets*intLumiData/intLumiTTplusJets)

processWtoTauNu_TTplusJetsSum = copy.deepcopy(process_TTplusJets)
processWtoTauNu_TTplusJetsSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    plotDirectoryName.value() + 'plotsWtoTauNu_TTplusJetsSum.root'
)
processWtoTauNu_TTplusJetsSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processWtoTauNu_TTplusJetsSum.config_dqmFileLoader.scaleFactor = cms.double(1.)


