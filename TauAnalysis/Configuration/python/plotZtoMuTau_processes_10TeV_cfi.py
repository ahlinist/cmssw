import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.Configuration.plotterProcessDefinitions_cfi import *
from TauAnalysis.Configuration.recoSampleDefinitionsZtoMuTau_10TeV_cfi import *

#--------------------------------------------------------------------------------
# define for Z --> mu + tau-jet analysis @ 10 TeV centre-of-mass energy
# names of .root files containing histograms
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
processZtoMuTau_Ztautau = copy.deepcopy(process_Ztautau)
processZtoMuTau_Ztautau.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoMuTau_Ztautau_10TeV_part01.root',
    'plotsZtoMuTau_Ztautau_10TeV_part02.root',
    'plotsZtoMuTau_Ztautau_10TeV_part03.root',
    'plotsZtoMuTau_Ztautau_10TeV_part04.root',
    'plotsZtoMuTau_Ztautau_10TeV_part05.root',
    'plotsZtoMuTau_Ztautau_10TeV_part06.root',
    'plotsZtoMuTau_Ztautau_10TeV_part07.root',
    'plotsZtoMuTau_Ztautau_10TeV_part08.root',
    'plotsZtoMuTau_Ztautau_10TeV_part09.root',
    'plotsZtoMuTau_Ztautau_10TeV_part10.root',
    'plotsZtoMuTau_Ztautau_10TeV_part11.root',
    'plotsZtoMuTau_Ztautau_10TeV_part12.root',
    'plotsZtoMuTau_Ztautau_10TeV_part13.root',
    'plotsZtoMuTau_Ztautau_10TeV_part14.root',
    'plotsZtoMuTau_Ztautau_10TeV_part15.root'    
)
processZtoMuTau_Ztautau.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoMuTau_Ztautau_10TeV*intLumiZtoMuTau_Data_10TeV/intLumiZtoMuTau_Ztautau_10TeV)

processZtoMuTau_ZtautauSum = copy.deepcopy(process_Ztautau)
processZtoMuTau_ZtautauSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoMuTau_ZtautauSum.root'
)
processZtoMuTau_ZtautauSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processZtoMuTau_ZtautauSum.config_dqmFileLoader.scaleFactor = cms.double(1.)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
processZtoMuTau_Zmumu = copy.deepcopy(process_Zmumu)
processZtoMuTau_Zmumu.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoMuTau_Zmumu_10TeV_part01.root',
    'plotsZtoMuTau_Zmumu_10TeV_part02.root',
    'plotsZtoMuTau_Zmumu_10TeV_part03.root',
    'plotsZtoMuTau_Zmumu_10TeV_part04.root',
    'plotsZtoMuTau_Zmumu_10TeV_part05.root',
    'plotsZtoMuTau_Zmumu_10TeV_part06.root',
    'plotsZtoMuTau_Zmumu_10TeV_part07.root',
    'plotsZtoMuTau_Zmumu_10TeV_part08.root',
    'plotsZtoMuTau_Zmumu_10TeV_part09.root',
    'plotsZtoMuTau_Zmumu_10TeV_part10.root',
    'plotsZtoMuTau_Zmumu_10TeV_part11.root',
    'plotsZtoMuTau_Zmumu_10TeV_part12.root',
    'plotsZtoMuTau_Zmumu_10TeV_part13.root',
    'plotsZtoMuTau_Zmumu_10TeV_part14.root',
    'plotsZtoMuTau_Zmumu_10TeV_part15.root',
    'plotsZtoMuTau_Zmumu_10TeV_part16.root',
    'plotsZtoMuTau_Zmumu_10TeV_part17.root'
)
processZtoMuTau_Zmumu.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoMuTau_Zmumu_10TeV*intLumiZtoMuTau_Data_10TeV/intLumiZtoMuTau_Zmumu_10TeV)

processZtoMuTau_ZmumuSum = copy.deepcopy(process_Zmumu)
processZtoMuTau_ZmumuSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoMuTau_ZmumuSum.root'
)
processZtoMuTau_ZmumuSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processZtoMuTau_ZmumuSum.config_dqmFileLoader.scaleFactor = cms.double(1.)
#-----------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
processZtoMuTau_WplusJets = copy.deepcopy(process_WplusJets)
processZtoMuTau_WplusJets.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoMuTau_WplusJets_10TeV_part01.root',
    'plotsZtoMuTau_WplusJets_10TeV_part02.root',
    'plotsZtoMuTau_WplusJets_10TeV_part03.root',
    'plotsZtoMuTau_WplusJets_10TeV_part04.root',
    'plotsZtoMuTau_WplusJets_10TeV_part05.root',
    'plotsZtoMuTau_WplusJets_10TeV_part06.root',
    'plotsZtoMuTau_WplusJets_10TeV_part07.root',
    'plotsZtoMuTau_WplusJets_10TeV_part08.root',
    'plotsZtoMuTau_WplusJets_10TeV_part09.root',
    'plotsZtoMuTau_WplusJets_10TeV_part10.root',
    'plotsZtoMuTau_WplusJets_10TeV_part11.root',
    'plotsZtoMuTau_WplusJets_10TeV_part12.root',
    'plotsZtoMuTau_WplusJets_10TeV_part13.root',
    'plotsZtoMuTau_WplusJets_10TeV_part14.root',
    'plotsZtoMuTau_WplusJets_10TeV_part15.root',
    'plotsZtoMuTau_WplusJets_10TeV_part16.root',
    'plotsZtoMuTau_WplusJets_10TeV_part17.root',
    'plotsZtoMuTau_WplusJets_10TeV_part18.root',
    'plotsZtoMuTau_WplusJets_10TeV_part19.root',
    'plotsZtoMuTau_WplusJets_10TeV_part20.root',
    'plotsZtoMuTau_WplusJets_10TeV_part21.root',
    'plotsZtoMuTau_WplusJets_10TeV_part22.root',
    'plotsZtoMuTau_WplusJets_10TeV_part23.root',
    'plotsZtoMuTau_WplusJets_10TeV_part24.root',
    'plotsZtoMuTau_WplusJets_10TeV_part25.root'
    #'plotsZtoMuTau_WplusJets_10TeV_part26.root'
)

processZtoMuTau_WplusJets.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoMuTau_WplusJets_10TeV*intLumiZtoMuTau_Data_10TeV/intLumiZtoMuTau_WplusJets_10TeV)

processZtoMuTau_WplusJetsSum = copy.deepcopy(process_WplusJets)
processZtoMuTau_WplusJetsSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoMuTau_WplusJetsSum.root'
)
processZtoMuTau_WplusJetsSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processZtoMuTau_WplusJetsSum.config_dqmFileLoader.scaleFactor = cms.double(1.)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
processZtoMuTau_InclusivePPmuX = copy.deepcopy(process_InclusivePPmuX)
processZtoMuTau_InclusivePPmuX.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoMuTau_InclusivePPmuX_10TeV_part01.root',
    'plotsZtoMuTau_InclusivePPmuX_10TeV_part02.root',
    'plotsZtoMuTau_InclusivePPmuX_10TeV_part03.root',
    'plotsZtoMuTau_InclusivePPmuX_10TeV_part04.root',
    'plotsZtoMuTau_InclusivePPmuX_10TeV_part05.root',
    'plotsZtoMuTau_InclusivePPmuX_10TeV_part06.root',
    'plotsZtoMuTau_InclusivePPmuX_10TeV_part07.root',
    'plotsZtoMuTau_InclusivePPmuX_10TeV_part08.root',
    'plotsZtoMuTau_InclusivePPmuX_10TeV_part09.root',
    'plotsZtoMuTau_InclusivePPmuX_10TeV_part10.root',
    'plotsZtoMuTau_InclusivePPmuX_10TeV_part11.root',
    'plotsZtoMuTau_InclusivePPmuX_10TeV_part12.root',
    'plotsZtoMuTau_InclusivePPmuX_10TeV_part13.root',
    'plotsZtoMuTau_InclusivePPmuX_10TeV_part14.root',
    'plotsZtoMuTau_InclusivePPmuX_10TeV_part15.root',
    'plotsZtoMuTau_InclusivePPmuX_10TeV_part16.root',
    'plotsZtoMuTau_InclusivePPmuX_10TeV_part17.root',
    'plotsZtoMuTau_InclusivePPmuX_10TeV_part18.root',
    'plotsZtoMuTau_InclusivePPmuX_10TeV_part19.root',
    'plotsZtoMuTau_InclusivePPmuX_10TeV_part20.root',
    'plotsZtoMuTau_InclusivePPmuX_10TeV_part21.root'
)

processZtoMuTau_InclusivePPmuX.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoMuTau_InclusivePPmuX_10TeV*intLumiZtoMuTau_Data_10TeV/intLumiZtoMuTau_InclusivePPmuX_10TeV)

processZtoMuTau_InclusivePPmuXsum = copy.deepcopy(process_InclusivePPmuX)
processZtoMuTau_InclusivePPmuXsum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoMuTau_InclusivePPmuXSum.root'
)
processZtoMuTau_InclusivePPmuXsum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processZtoMuTau_InclusivePPmuXsum.config_dqmFileLoader.scaleFactor = cms.double(1.)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
processZtoMuTau_PPmuXptGt20_part01 = copy.deepcopy(process_PPmuXptGt20)
processZtoMuTau_PPmuXptGt20_part01.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part01.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part02.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part03.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part04.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part05.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part06.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part07.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part08.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part09.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part10.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part11.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part12.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part13.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part14.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part15.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part16.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part17.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part18.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part19.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part20.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part21.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part22.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part23.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part24.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part25.root'
)
processZtoMuTau_PPmuXptGt20_part01.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoMuTau_PPmuXptGt20_10TeV*intLumiZtoMuTau_Data_10TeV/intLumiZtoMuTau_PPmuXptGt20_10TeV)

processZtoMuTau_PPmuXptGt20_part02 = copy.deepcopy(process_PPmuXptGt20)
processZtoMuTau_PPmuXptGt20_part02.config_dqmFileLoader.inputFileNames = cms.vstring(    
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part26.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part27.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part28.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part29.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part30.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part31.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part32.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part33.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part34.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part35.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part36.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part37.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part38.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part39.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part40.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part41.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part42.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part43.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part44.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part45.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part46.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part47.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part48.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part49.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part50.root'
)
processZtoMuTau_PPmuXptGt20_part02.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoMuTau_PPmuXptGt20_10TeV*intLumiZtoMuTau_Data_10TeV/intLumiZtoMuTau_PPmuXptGt20_10TeV)

processZtoMuTau_PPmuXptGt20_part03 = copy.deepcopy(process_PPmuXptGt20)
processZtoMuTau_PPmuXptGt20_part03.config_dqmFileLoader.inputFileNames = cms.vstring( 
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part51.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part52.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part53.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part54.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part55.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part56.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part57.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part58.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part59.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part60.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part61.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part62.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part63.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part64.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part65.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part66.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part67.root',
    #'plotsZtoMuTau_PPmuXptGt20_10TeV_part68.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part69.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part70.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part71.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part72.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part73.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part74.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part75.root'
)
processZtoMuTau_PPmuXptGt20_part03.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoMuTau_PPmuXptGt20_10TeV*intLumiZtoMuTau_Data_10TeV/intLumiZtoMuTau_PPmuXptGt20_10TeV)

processZtoMuTau_PPmuXptGt20_part04 = copy.deepcopy(process_PPmuXptGt20)
processZtoMuTau_PPmuXptGt20_part04.config_dqmFileLoader.inputFileNames = cms.vstring( 
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part76.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part77.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part78.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part79.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part80.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part81.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part82.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part83.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part84.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part85.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part86.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part87.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part88.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part89.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part90.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part91.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part92.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part93.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part94.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part95.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part96.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part97.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part98.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part99.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part100.root'
)
processZtoMuTau_PPmuXptGt20_part04.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoMuTau_PPmuXptGt20_10TeV*intLumiZtoMuTau_Data_10TeV/intLumiZtoMuTau_PPmuXptGt20_10TeV)

processZtoMuTau_PPmuXptGt20_part05 = copy.deepcopy(process_PPmuXptGt20)
processZtoMuTau_PPmuXptGt20_part05.config_dqmFileLoader.inputFileNames = cms.vstring( 
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part101.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part102.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part103.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part104.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part105.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part106.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part107.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part108.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part109.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part110.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part111.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part112.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part113.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part114.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part115.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part116.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part117.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part118.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part119.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part120.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part121.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part122.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part123.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part124.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part125.root'
)

processZtoMuTau_PPmuXptGt20_part05.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoMuTau_PPmuXptGt20_10TeV*intLumiZtoMuTau_Data_10TeV/intLumiZtoMuTau_PPmuXptGt20_10TeV)

processZtoMuTau_PPmuXptGt20_part06 = copy.deepcopy(process_PPmuXptGt20)
processZtoMuTau_PPmuXptGt20_part06.config_dqmFileLoader.inputFileNames = cms.vstring( 
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part126.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part127.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part128.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part129.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part130.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part131.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part132.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part133.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part134.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part135.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part136.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part137.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part138.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part139.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part140.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part141.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part142.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part143.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part144.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part145.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part146.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part147.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part148.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part149.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part150.root'
)
processZtoMuTau_PPmuXptGt20_part06.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoMuTau_PPmuXptGt20_10TeV*intLumiZtoMuTau_Data_10TeV/intLumiZtoMuTau_PPmuXptGt20_10TeV)

processZtoMuTau_PPmuXptGt20_part07 = copy.deepcopy(process_PPmuXptGt20)
processZtoMuTau_PPmuXptGt20_part07.config_dqmFileLoader.inputFileNames = cms.vstring( 
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part151.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part152.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part153.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part154.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part155.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part156.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part157.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part158.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part159.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part160.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part161.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part162.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part163.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part164.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part165.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part166.root',
    'plotsZtoMuTau_PPmuXptGt20_10TeV_part167.root'
)

processZtoMuTau_PPmuXptGt20_part07.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoMuTau_PPmuXptGt20_10TeV*intLumiZtoMuTau_Data_10TeV/intLumiZtoMuTau_PPmuXptGt20_10TeV)

processZtoMuTau_PPmuXptGt20Sum = copy.deepcopy(process_PPmuXptGt20)
processZtoMuTau_PPmuXptGt20Sum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoMuTau_PPmuXptGt20_part01Sum.root',
    'plotsZtoMuTau_PPmuXptGt20_part02Sum.root',
    'plotsZtoMuTau_PPmuXptGt20_part03Sum.root',
    'plotsZtoMuTau_PPmuXptGt20_part04Sum.root',
    'plotsZtoMuTau_PPmuXptGt20_part05Sum.root',
    'plotsZtoMuTau_PPmuXptGt20_part06Sum.root',
    'plotsZtoMuTau_PPmuXptGt20_part07Sum.root'
)
processZtoMuTau_PPmuXptGt20Sum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processZtoMuTau_PPmuXptGt20Sum.config_dqmFileLoader.scaleFactor = cms.double(1.)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
processZtoMuTau_TTplusJets = copy.deepcopy(process_TTplusJets)
processZtoMuTau_TTplusJets.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoMuTau_TTplusJets_10TeV_part01.root',
    'plotsZtoMuTau_TTplusJets_10TeV_part02.root',
    'plotsZtoMuTau_TTplusJets_10TeV_part03.root',
    'plotsZtoMuTau_TTplusJets_10TeV_part04.root',
    'plotsZtoMuTau_TTplusJets_10TeV_part05.root',
    'plotsZtoMuTau_TTplusJets_10TeV_part06.root',
    'plotsZtoMuTau_TTplusJets_10TeV_part07.root',
    'plotsZtoMuTau_TTplusJets_10TeV_part08.root',
    'plotsZtoMuTau_TTplusJets_10TeV_part09.root',
    'plotsZtoMuTau_TTplusJets_10TeV_part10.root',
    'plotsZtoMuTau_TTplusJets_10TeV_part11.root',
    'plotsZtoMuTau_TTplusJets_10TeV_part12.root',
    'plotsZtoMuTau_TTplusJets_10TeV_part13.root',
    'plotsZtoMuTau_TTplusJets_10TeV_part14.root',
    'plotsZtoMuTau_TTplusJets_10TeV_part15.root',
    'plotsZtoMuTau_TTplusJets_10TeV_part16.root',
    'plotsZtoMuTau_TTplusJets_10TeV_part17.root'
)
processZtoMuTau_TTplusJets.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoMuTau_TTplusJets_10TeV*intLumiZtoMuTau_Data_10TeV/intLumiZtoMuTau_TTplusJets_10TeV)

processZtoMuTau_TTplusJetsSum = copy.deepcopy(process_TTplusJets)
processZtoMuTau_TTplusJetsSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoMuTau_TTplusJetsSum.root'
)
processZtoMuTau_TTplusJetsSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processZtoMuTau_TTplusJetsSum.config_dqmFileLoader.scaleFactor = cms.double(1.)
#--------------------------------------------------------------------------------
