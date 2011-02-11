import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.Configuration.plotterProcessDefinitions_cfi import *
from TauAnalysis.Configuration.recoSampleDefinitionsWtoTauNu_7TeV_cfi import *

#--------------------------------------------------------------------------------
# define for W --> nu tau-jet analysis names of .root files containing histograms
#--------------------------------------------------------------------------------

processWtoTauNu_Wtaunu = copy.deepcopy(process_Wtaunu)
processWtoTauNu_Wtaunu.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsWtoTauNu_Wtaunu_part01.root',
    'plotsWtoTauNu_Wtaunu_part02.root',
    'plotsWtoTauNu_Wtaunu_part03.root',    
    'plotsWtoTauNu_Wtaunu_part04.root',    
    'plotsWtoTauNu_Wtaunu_part05.root',
    'plotsWtoTauNu_Wtaunu_part06.root',
    'plotsWtoTauNu_Wtaunu_part07.root',
    'plotsWtoTauNu_Wtaunu_part08.root',
    'plotsWtoTauNu_Wtaunu_part09.root',
    'plotsWtoTauNu_Wtaunu_part10.root'
)

processWtoTauNu_Wtaunu.config_dqmFileLoader.scaleFactor = cms.double(corrFactorWtoTauNu_Wtaunu_7TeV*intLumiWtoTauNu_Data_7TeV/intLumiWtoTauNu_Wtaunu_7TeV)

processWtoTauNu_WtaunuSum = copy.deepcopy(process_Wtaunu)
processWtoTauNu_WtaunuSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsWtoTauNu_WtaunuSum.root'
)
processWtoTauNu_WtaunuSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processWtoTauNu_WtaunuSum.config_dqmFileLoader.scaleFactor = cms.double(1.)

#--------------------------------------------------------------------------------

processWtoTauNu_ZplusJets = copy.deepcopy(process_ZplusJets)
processWtoTauNu_ZplusJets.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsWtoTauNu_ZplusJets_part01.root',
    'plotsWtoTauNu_ZplusJets_part02.root',
    'plotsWtoTauNu_ZplusJets_part03.root',    
    'plotsWtoTauNu_ZplusJets_part04.root',    
    'plotsWtoTauNu_ZplusJets_part05.root',
    'plotsWtoTauNu_ZplusJets_part06.root',
    'plotsWtoTauNu_ZplusJets_part07.root',
    'plotsWtoTauNu_ZplusJets_part08.root',
    'plotsWtoTauNu_ZplusJets_part09.root',
    'plotsWtoTauNu_ZplusJets_part10.root',
    'plotsWtoTauNu_ZplusJets_part11.root',
    'plotsWtoTauNu_ZplusJets_part12.root',
    'plotsWtoTauNu_ZplusJets_part13.root',
    'plotsWtoTauNu_ZplusJets_part14.root',
    'plotsWtoTauNu_ZplusJets_part15.root',
    'plotsWtoTauNu_ZplusJets_part16.root',
    'plotsWtoTauNu_ZplusJets_part17.root',
    'plotsWtoTauNu_ZplusJets_part18.root',
#    'plotsWtoTauNu_ZplusJets_part19.root',
    'plotsWtoTauNu_ZplusJets_part20.root',
    'plotsWtoTauNu_ZplusJets_part21.root',
    'plotsWtoTauNu_ZplusJets_part22.root',
    'plotsWtoTauNu_ZplusJets_part23.root',
    'plotsWtoTauNu_ZplusJets_part24.root',
    'plotsWtoTauNu_ZplusJets_part25.root',
    'plotsWtoTauNu_ZplusJets_part26.root',
    'plotsWtoTauNu_ZplusJets_part27.root',
    'plotsWtoTauNu_ZplusJets_part28.root',
    'plotsWtoTauNu_ZplusJets_part29.root'    
)

processWtoTauNu_ZplusJets.config_dqmFileLoader.scaleFactor = cms.double(corrFactorWtoTauNu_ZplusJets_7TeV*intLumiWtoTauNu_Data_7TeV/intLumiWtoTauNu_ZplusJets_7TeV)

processWtoTauNu_ZplusJetsSum = copy.deepcopy(process_ZplusJets)
processWtoTauNu_ZplusJetsSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsWtoTauNu_ZplusJetsSum.root'
)
processWtoTauNu_ZplusJetsSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processWtoTauNu_ZplusJetsSum.config_dqmFileLoader.scaleFactor = cms.double(1.)

#--------------------------------------------------------------------------------
processWtoTauNu_Wenu = copy.deepcopy(process_Wenu)
processWtoTauNu_Wenu.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsWtoTauNu_Wenu_part01.root',
    'plotsWtoTauNu_Wenu_part02.root',
    'plotsWtoTauNu_Wenu_part03.root',
    'plotsWtoTauNu_Wenu_part04.root',
    'plotsWtoTauNu_Wenu_part05.root',
    'plotsWtoTauNu_Wenu_part06.root',
#    'plotsWtoTauNu_Wenu_part07.root',
    'plotsWtoTauNu_Wenu_part08.root',
    'plotsWtoTauNu_Wenu_part09.root',
    'plotsWtoTauNu_Wenu_part10.root',
    'plotsWtoTauNu_Wenu_part11.root',
    'plotsWtoTauNu_Wenu_part12.root',
    'plotsWtoTauNu_Wenu_part13.root',
    'plotsWtoTauNu_Wenu_part14.root',
    'plotsWtoTauNu_Wenu_part15.root',
    'plotsWtoTauNu_Wenu_part16.root',
    'plotsWtoTauNu_Wenu_part17.root',
    'plotsWtoTauNu_Wenu_part18.root',
    'plotsWtoTauNu_Wenu_part19.root',
    'plotsWtoTauNu_Wenu_part20.root',
    'plotsWtoTauNu_Wenu_part21.root',
    'plotsWtoTauNu_Wenu_part22.root',
    'plotsWtoTauNu_Wenu_part23.root',
    'plotsWtoTauNu_Wenu_part24.root',
    'plotsWtoTauNu_Wenu_part25.root',
    'plotsWtoTauNu_Wenu_part26.root',
    'plotsWtoTauNu_Wenu_part27.root',
    'plotsWtoTauNu_Wenu_part28.root',
    'plotsWtoTauNu_Wenu_part29.root',
    'plotsWtoTauNu_Wenu_part30.root',
    'plotsWtoTauNu_Wenu_part31.root',
    'plotsWtoTauNu_Wenu_part32.root'
#    'plotsWtoTauNu_Wenu_part33.root'                                                                    
    )

processWtoTauNu_Wenu.config_dqmFileLoader.scaleFactor = cms.double(corrFactorWtoTauNu_Wenu_7TeV*intLumiWtoTauNu_Data_7TeV/intLumiWtoTauNu_Wenu_7TeV)

processWtoTauNu_WenuSum = copy.deepcopy(process_Wenu)
processWtoTauNu_WenuSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsWtoTauNu_WenuSum.root'
    )
processWtoTauNu_WenuSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processWtoTauNu_WenuSum.config_dqmFileLoader.scaleFactor = cms.double(1.)

#--------------------------------------------------------------------------------
processWtoTauNu_Wmunu = copy.deepcopy(process_Wmunu)
processWtoTauNu_Wmunu.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsWtoTauNu_Wmunu_part01.root',
    'plotsWtoTauNu_Wmunu_part02.root',
    'plotsWtoTauNu_Wmunu_part03.root',
    'plotsWtoTauNu_Wmunu_part04.root',
    'plotsWtoTauNu_Wmunu_part05.root',
    'plotsWtoTauNu_Wmunu_part06.root',
    'plotsWtoTauNu_Wmunu_part07.root',
    'plotsWtoTauNu_Wmunu_part08.root',
    'plotsWtoTauNu_Wmunu_part09.root',
    'plotsWtoTauNu_Wmunu_part10.root',
    'plotsWtoTauNu_Wmunu_part11.root',
    'plotsWtoTauNu_Wmunu_part12.root',
    'plotsWtoTauNu_Wmunu_part13.root',
    'plotsWtoTauNu_Wmunu_part14.root',
    'plotsWtoTauNu_Wmunu_part15.root',
    'plotsWtoTauNu_Wmunu_part16.root',
    'plotsWtoTauNu_Wmunu_part17.root',
    'plotsWtoTauNu_Wmunu_part18.root',
    'plotsWtoTauNu_Wmunu_part19.root',
    'plotsWtoTauNu_Wmunu_part20.root',
    'plotsWtoTauNu_Wmunu_part21.root',
    'plotsWtoTauNu_Wmunu_part22.root',
    'plotsWtoTauNu_Wmunu_part23.root',
    'plotsWtoTauNu_Wmunu_part24.root',
    'plotsWtoTauNu_Wmunu_part25.root',
    'plotsWtoTauNu_Wmunu_part26.root',
    'plotsWtoTauNu_Wmunu_part27.root',
    'plotsWtoTauNu_Wmunu_part28.root',
    'plotsWtoTauNu_Wmunu_part29.root',
    'plotsWtoTauNu_Wmunu_part30.root'
    )

processWtoTauNu_Wmunu.config_dqmFileLoader.scaleFactor = cms.double(corrFactorWtoTauNu_Wmunu_7TeV*intLumiWtoTauNu_Data_7TeV/intLumiWtoTauNu_Wmunu_7TeV)

processWtoTauNu_WmunuSum = copy.deepcopy(process_Wmunu)
processWtoTauNu_WmunuSum.config_dqmFileLoader.inputFileNames = cms.vstring(
     'plotsWtoTauNu_WmunuSum.root'
    )
processWtoTauNu_WmunuSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processWtoTauNu_WmunuSum.config_dqmFileLoader.scaleFactor = cms.double(1.)

#--------------------------------------------------------------------------------
processWtoTauNu_qcd_W = copy.deepcopy(process_qcd_W)
processWtoTauNu_qcd_W_part01 = copy.deepcopy(process_qcd_W)
processWtoTauNu_qcd_W_part02 = copy.deepcopy(process_qcd_W)
processWtoTauNu_qcd_W_part03 = copy.deepcopy(process_qcd_W)
processWtoTauNu_qcd_W_part04 = copy.deepcopy(process_qcd_W)
processWtoTauNu_qcd_W_part05 = copy.deepcopy(process_qcd_W)
processWtoTauNu_qcd_W_part01.config_dqmFileLoader.inputFileNames = cms.vstring(
     'plotsWtoTauNu_qcd_W_part01.root',
     'plotsWtoTauNu_qcd_W_part02.root',
     'plotsWtoTauNu_qcd_W_part03.root',
     'plotsWtoTauNu_qcd_W_part04.root',
     'plotsWtoTauNu_qcd_W_part05.root',
     'plotsWtoTauNu_qcd_W_part06.root',
     'plotsWtoTauNu_qcd_W_part07.root',
     'plotsWtoTauNu_qcd_W_part08.root',
     'plotsWtoTauNu_qcd_W_part09.root',
     'plotsWtoTauNu_qcd_W_part10.root',
     'plotsWtoTauNu_qcd_W_part11.root',
     'plotsWtoTauNu_qcd_W_part12.root',
     'plotsWtoTauNu_qcd_W_part13.root',
     'plotsWtoTauNu_qcd_W_part14.root',
     'plotsWtoTauNu_qcd_W_part15.root',
     'plotsWtoTauNu_qcd_W_part16.root',
     'plotsWtoTauNu_qcd_W_part17.root',
     'plotsWtoTauNu_qcd_W_part18.root',
     'plotsWtoTauNu_qcd_W_part19.root',
     'plotsWtoTauNu_qcd_W_part20.root',                                             
     'plotsWtoTauNu_qcd_W_part21.root',
     'plotsWtoTauNu_qcd_W_part22.root',
     'plotsWtoTauNu_qcd_W_part23.root',
     'plotsWtoTauNu_qcd_W_part24.root',
     'plotsWtoTauNu_qcd_W_part25.root',
     'plotsWtoTauNu_qcd_W_part26.root',
     'plotsWtoTauNu_qcd_W_part27.root',
     'plotsWtoTauNu_qcd_W_part28.root',
     'plotsWtoTauNu_qcd_W_part29.root',
     'plotsWtoTauNu_qcd_W_part30.root',
     'plotsWtoTauNu_qcd_W_part31.root',
     'plotsWtoTauNu_qcd_W_part32.root',
     'plotsWtoTauNu_qcd_W_part33.root',
     'plotsWtoTauNu_qcd_W_part34.root',
     'plotsWtoTauNu_qcd_W_part35.root'
)
processWtoTauNu_qcd_W_part02.config_dqmFileLoader.inputFileNames = cms.vstring(
     'plotsWtoTauNu_qcd_W_part36.root',
     'plotsWtoTauNu_qcd_W_part37.root',
     'plotsWtoTauNu_qcd_W_part38.root',
     'plotsWtoTauNu_qcd_W_part39.root',
     'plotsWtoTauNu_qcd_W_part40.root',
     'plotsWtoTauNu_qcd_W_part41.root',
     'plotsWtoTauNu_qcd_W_part42.root',
     'plotsWtoTauNu_qcd_W_part43.root',
     'plotsWtoTauNu_qcd_W_part44.root',
     'plotsWtoTauNu_qcd_W_part45.root',
     'plotsWtoTauNu_qcd_W_part46.root',
     'plotsWtoTauNu_qcd_W_part47.root',
     'plotsWtoTauNu_qcd_W_part48.root',
     'plotsWtoTauNu_qcd_W_part49.root',
     'plotsWtoTauNu_qcd_W_part50.root',
     'plotsWtoTauNu_qcd_W_part51.root',
     'plotsWtoTauNu_qcd_W_part52.root',
     'plotsWtoTauNu_qcd_W_part53.root',
     'plotsWtoTauNu_qcd_W_part54.root',
     'plotsWtoTauNu_qcd_W_part55.root',
     'plotsWtoTauNu_qcd_W_part56.root',
     'plotsWtoTauNu_qcd_W_part57.root',
     'plotsWtoTauNu_qcd_W_part58.root',
     'plotsWtoTauNu_qcd_W_part59.root',
     'plotsWtoTauNu_qcd_W_part60.root',
     'plotsWtoTauNu_qcd_W_part61.root',
     'plotsWtoTauNu_qcd_W_part62.root',
     'plotsWtoTauNu_qcd_W_part63.root',
     'plotsWtoTauNu_qcd_W_part64.root',
     'plotsWtoTauNu_qcd_W_part65.root',
     'plotsWtoTauNu_qcd_W_part66.root',
     'plotsWtoTauNu_qcd_W_part67.root',
     'plotsWtoTauNu_qcd_W_part68.root',
     'plotsWtoTauNu_qcd_W_part69.root',
     'plotsWtoTauNu_qcd_W_part70.root'
)
processWtoTauNu_qcd_W_part03.config_dqmFileLoader.inputFileNames = cms.vstring(
     'plotsWtoTauNu_qcd_W_part71.root',
     'plotsWtoTauNu_qcd_W_part72.root',
     'plotsWtoTauNu_qcd_W_part73.root',
     'plotsWtoTauNu_qcd_W_part74.root',
     'plotsWtoTauNu_qcd_W_part75.root',
     'plotsWtoTauNu_qcd_W_part76.root',
     'plotsWtoTauNu_qcd_W_part77.root',
     'plotsWtoTauNu_qcd_W_part78.root',
     'plotsWtoTauNu_qcd_W_part79.root',
     'plotsWtoTauNu_qcd_W_part80.root',
     'plotsWtoTauNu_qcd_W_part81.root',
     'plotsWtoTauNu_qcd_W_part82.root',
     'plotsWtoTauNu_qcd_W_part83.root',
     'plotsWtoTauNu_qcd_W_part84.root',
     'plotsWtoTauNu_qcd_W_part85.root',
     'plotsWtoTauNu_qcd_W_part86.root',
     'plotsWtoTauNu_qcd_W_part87.root',
     'plotsWtoTauNu_qcd_W_part88.root',
     'plotsWtoTauNu_qcd_W_part89.root',
     'plotsWtoTauNu_qcd_W_part90.root',
     'plotsWtoTauNu_qcd_W_part91.root',
     'plotsWtoTauNu_qcd_W_part92.root',
     'plotsWtoTauNu_qcd_W_part93.root',
     'plotsWtoTauNu_qcd_W_part94.root',
     'plotsWtoTauNu_qcd_W_part95.root',
     'plotsWtoTauNu_qcd_W_part96.root',
     'plotsWtoTauNu_qcd_W_part97.root',
     'plotsWtoTauNu_qcd_W_part98.root',
     'plotsWtoTauNu_qcd_W_part99.root',
     'plotsWtoTauNu_qcd_W_part100.root',
     'plotsWtoTauNu_qcd_W_part101.root',
     'plotsWtoTauNu_qcd_W_part102.root',
     'plotsWtoTauNu_qcd_W_part103.root',
     'plotsWtoTauNu_qcd_W_part104.root',
     'plotsWtoTauNu_qcd_W_part105.root'
     )
processWtoTauNu_qcd_W_part04.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsWtoTauNu_qcd_W_part106.root',
    'plotsWtoTauNu_qcd_W_part107.root',
    'plotsWtoTauNu_qcd_W_part108.root',
    'plotsWtoTauNu_qcd_W_part109.root',
    'plotsWtoTauNu_qcd_W_part110.root',
    'plotsWtoTauNu_qcd_W_part111.root',
    'plotsWtoTauNu_qcd_W_part112.root',
    'plotsWtoTauNu_qcd_W_part113.root',
    'plotsWtoTauNu_qcd_W_part114.root',
    'plotsWtoTauNu_qcd_W_part115.root',
    'plotsWtoTauNu_qcd_W_part116.root',
    'plotsWtoTauNu_qcd_W_part117.root',
    'plotsWtoTauNu_qcd_W_part118.root',
    'plotsWtoTauNu_qcd_W_part119.root',
    'plotsWtoTauNu_qcd_W_part120.root',                                             
    'plotsWtoTauNu_qcd_W_part121.root',
    'plotsWtoTauNu_qcd_W_part122.root',
    'plotsWtoTauNu_qcd_W_part123.root',
    'plotsWtoTauNu_qcd_W_part124.root',
    'plotsWtoTauNu_qcd_W_part125.root',
    'plotsWtoTauNu_qcd_W_part126.root',
    'plotsWtoTauNu_qcd_W_part127.root',
    'plotsWtoTauNu_qcd_W_part128.root',
    'plotsWtoTauNu_qcd_W_part129.root',
    'plotsWtoTauNu_qcd_W_part130.root',
    'plotsWtoTauNu_qcd_W_part131.root',
    'plotsWtoTauNu_qcd_W_part132.root',
    'plotsWtoTauNu_qcd_W_part133.root',
    'plotsWtoTauNu_qcd_W_part134.root',
    'plotsWtoTauNu_qcd_W_part135.root',
    'plotsWtoTauNu_qcd_W_part136.root',
    'plotsWtoTauNu_qcd_W_part137.root',
    'plotsWtoTauNu_qcd_W_part138.root',
    'plotsWtoTauNu_qcd_W_part139.root',
    'plotsWtoTauNu_qcd_W_part140.root'
    )
processWtoTauNu_qcd_W_part05.config_dqmFileLoader.inputFileNames = cms.vstring(
     'plotsWtoTauNu_qcd_W_part141.root',
     'plotsWtoTauNu_qcd_W_part142.root',
     'plotsWtoTauNu_qcd_W_part143.root',
     'plotsWtoTauNu_qcd_W_part144.root',
     'plotsWtoTauNu_qcd_W_part145.root',
     'plotsWtoTauNu_qcd_W_part146.root',
     'plotsWtoTauNu_qcd_W_part147.root',
     'plotsWtoTauNu_qcd_W_part148.root',
     'plotsWtoTauNu_qcd_W_part149.root',
     'plotsWtoTauNu_qcd_W_part150.root',
     'plotsWtoTauNu_qcd_W_part151.root',
     'plotsWtoTauNu_qcd_W_part152.root',
     'plotsWtoTauNu_qcd_W_part153.root',
     'plotsWtoTauNu_qcd_W_part154.root',
     'plotsWtoTauNu_qcd_W_part155.root',
     'plotsWtoTauNu_qcd_W_part156.root',
     'plotsWtoTauNu_qcd_W_part157.root',
     'plotsWtoTauNu_qcd_W_part158.root',
     'plotsWtoTauNu_qcd_W_part159.root',
     'plotsWtoTauNu_qcd_W_part160.root',
     'plotsWtoTauNu_qcd_W_part161.root',
     'plotsWtoTauNu_qcd_W_part162.root',
     'plotsWtoTauNu_qcd_W_part163.root',
     'plotsWtoTauNu_qcd_W_part164.root',
     'plotsWtoTauNu_qcd_W_part165.root',
     'plotsWtoTauNu_qcd_W_part166.root',
     'plotsWtoTauNu_qcd_W_part167.root',
     'plotsWtoTauNu_qcd_W_part168.root',
     'plotsWtoTauNu_qcd_W_part169.root',
     'plotsWtoTauNu_qcd_W_part170.root',
     'plotsWtoTauNu_qcd_W_part171.root',
      'plotsWtoTauNu_qcd_W_part172.root',
     'plotsWtoTauNu_qcd_W_part173.root',
     'plotsWtoTauNu_qcd_W_part174.root'     
    )

processWtoTauNu_qcd_W.config_dqmFileLoader.scaleFactor = cms.double(corrFactorWtoTauNu_qcd_W_7TeV*intLumiWtoTauNu_Data_7TeV/intLumiWtoTauNu_qcd_W_7TeV)

processWtoTauNu_qcd_WSum = copy.deepcopy(process_qcd_W)
processWtoTauNu_qcd_WSum.config_dqmFileLoader.inputFileNames = cms.vstring(
     'plotsWtoTauNu_qcd_W_part01Sum.root',
     'plotsWtoTauNu_qcd_W_part02Sum.root',
     'plotsWtoTauNu_qcd_W_part03Sum.root',
     'plotsWtoTauNu_qcd_W_part04Sum.root',
     'plotsWtoTauNu_qcd_W_part05Sum.root'     
    )
processWtoTauNu_qcd_WSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processWtoTauNu_qcd_WSum.config_dqmFileLoader.scaleFactor = cms.double(1.)

#--------------------------------------------------------------------------------

