import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.Configuration.plotterProcessDefinitions_cfi import *
from TauAnalysis.Configuration.recoSampleDefinitionsZtoMuTau_7TeV_cfi import *

#--------------------------------------------------------------------------------
# define for Z --> mu + tau-jet analysis @ 7 TeV centre-of-mass energy
# names of .root files containing histograms
#--------------------------------------------------------------------------------

processZtoMuTau_Ztautau = copy.deepcopy(process_Ztautau)
processZtoMuTau_Ztautau.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoMuTau_Ztautau_7TeV_part01.root',
    'plotsZtoMuTau_Ztautau_7TeV_part02.root',
    'plotsZtoMuTau_Ztautau_7TeV_part03.root',
    'plotsZtoMuTau_Ztautau_7TeV_part04.root',
    'plotsZtoMuTau_Ztautau_7TeV_part05.root',
    'plotsZtoMuTau_Ztautau_7TeV_part06.root',
    'plotsZtoMuTau_Ztautau_7TeV_part07.root',
    'plotsZtoMuTau_Ztautau_7TeV_part08.root',
    'plotsZtoMuTau_Ztautau_7TeV_part09.root',
    'plotsZtoMuTau_Ztautau_7TeV_part10.root',
    'plotsZtoMuTau_Ztautau_7TeV_part11.root',
    'plotsZtoMuTau_Ztautau_7TeV_part12.root',
    'plotsZtoMuTau_Ztautau_7TeV_part13.root',
    'plotsZtoMuTau_Ztautau_7TeV_part14.root',
    'plotsZtoMuTau_Ztautau_7TeV_part15.root'    
)
processZtoMuTau_Ztautau.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoMuTau_Ztautau_7TeV*intLumiZtoMuTau_Data_7TeV/intLumiZtoMuTau_Ztautau_7TeV)

processZtoMuTau_ZtautauSum = copy.deepcopy(process_Ztautau)
processZtoMuTau_ZtautauSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoMuTau_ZtautauSum.root'
)
processZtoMuTau_ZtautauSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processZtoMuTau_ZtautauSum.config_dqmFileLoader.scaleFactor = cms.double(1.)

#--------------------------------------------------------------------------------

processZtoMuTau_Zmumu = copy.deepcopy(process_Zmumu)
processZtoMuTau_Zmumu.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoMuTau_Zmumu_7TeV_part01.root',
    'plotsZtoMuTau_Zmumu_7TeV_part02.root',
    'plotsZtoMuTau_Zmumu_7TeV_part03.root',
    'plotsZtoMuTau_Zmumu_7TeV_part04.root',
    'plotsZtoMuTau_Zmumu_7TeV_part05.root',
    'plotsZtoMuTau_Zmumu_7TeV_part06.root',
    'plotsZtoMuTau_Zmumu_7TeV_part07.root',
    'plotsZtoMuTau_Zmumu_7TeV_part08.root',
    'plotsZtoMuTau_Zmumu_7TeV_part09.root',
    'plotsZtoMuTau_Zmumu_7TeV_part10.root',
    'plotsZtoMuTau_Zmumu_7TeV_part11.root',
    'plotsZtoMuTau_Zmumu_7TeV_part12.root',
    'plotsZtoMuTau_Zmumu_7TeV_part13.root',
    'plotsZtoMuTau_Zmumu_7TeV_part14.root',
    'plotsZtoMuTau_Zmumu_7TeV_part15.root'   
)
processZtoMuTau_Zmumu.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoMuTau_Zmumu_7TeV*intLumiZtoMuTau_Data_7TeV/intLumiZtoMuTau_Zmumu_7TeV)

processZtoMuTau_ZmumuSum = copy.deepcopy(process_Zmumu)
processZtoMuTau_ZmumuSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoMuTau_ZmumuSum.root'
)
processZtoMuTau_ZmumuSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processZtoMuTau_ZmumuSum.config_dqmFileLoader.scaleFactor = cms.double(1.)

#--------------------------------------------------------------------------------

processZtoMuTau_WplusJets = copy.deepcopy(process_WplusJets)
processZtoMuTau_WplusJets.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoMuTau_WplusJets_7TeV_part01.root',
    'plotsZtoMuTau_WplusJets_7TeV_part02.root',
    'plotsZtoMuTau_WplusJets_7TeV_part03.root',
    'plotsZtoMuTau_WplusJets_7TeV_part04.root',
    'plotsZtoMuTau_WplusJets_7TeV_part05.root',
    'plotsZtoMuTau_WplusJets_7TeV_part06.root',
    'plotsZtoMuTau_WplusJets_7TeV_part07.root',
    'plotsZtoMuTau_WplusJets_7TeV_part08.root',
    'plotsZtoMuTau_WplusJets_7TeV_part09.root',
    'plotsZtoMuTau_WplusJets_7TeV_part10.root',
    'plotsZtoMuTau_WplusJets_7TeV_part11.root',
    'plotsZtoMuTau_WplusJets_7TeV_part12.root',
    'plotsZtoMuTau_WplusJets_7TeV_part13.root',
    'plotsZtoMuTau_WplusJets_7TeV_part14.root',
    'plotsZtoMuTau_WplusJets_7TeV_part15.root',
    'plotsZtoMuTau_WplusJets_7TeV_part16.root',
    'plotsZtoMuTau_WplusJets_7TeV_part17.root',
    'plotsZtoMuTau_WplusJets_7TeV_part18.root',
    'plotsZtoMuTau_WplusJets_7TeV_part19.root',
    'plotsZtoMuTau_WplusJets_7TeV_part20.root',
    'plotsZtoMuTau_WplusJets_7TeV_part21.root',
    'plotsZtoMuTau_WplusJets_7TeV_part22.root',
    'plotsZtoMuTau_WplusJets_7TeV_part23.root',
    'plotsZtoMuTau_WplusJets_7TeV_part24.root',
    'plotsZtoMuTau_WplusJets_7TeV_part25.root',
    'plotsZtoMuTau_WplusJets_7TeV_part26.root',
    'plotsZtoMuTau_WplusJets_7TeV_part27.root',
    'plotsZtoMuTau_WplusJets_7TeV_part28.root',
    'plotsZtoMuTau_WplusJets_7TeV_part29.root',
    'plotsZtoMuTau_WplusJets_7TeV_part30.root',
    'plotsZtoMuTau_WplusJets_7TeV_part31.root'    
)

processZtoMuTau_WplusJets.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoMuTau_WplusJets_7TeV*intLumiZtoMuTau_Data_7TeV/intLumiZtoMuTau_WplusJets_7TeV)

processZtoMuTau_WplusJetsSum = copy.deepcopy(process_WplusJets)
processZtoMuTau_WplusJetsSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoMuTau_WplusJetsSum.root'
)
processZtoMuTau_WplusJetsSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processZtoMuTau_WplusJetsSum.config_dqmFileLoader.scaleFactor = cms.double(1.)

#--------------------------------------------------------------------------------

processZtoMuTau_InclusivePPmuX = copy.deepcopy(process_InclusivePPmuX)
processZtoMuTau_InclusivePPmuX.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoMuTau_InclusivePPmuX_7TeV_part01.root',
    'plotsZtoMuTau_InclusivePPmuX_7TeV_part02.root',
    'plotsZtoMuTau_InclusivePPmuX_7TeV_part03.root',
    'plotsZtoMuTau_InclusivePPmuX_7TeV_part04.root',
    'plotsZtoMuTau_InclusivePPmuX_7TeV_part05.root',
    'plotsZtoMuTau_InclusivePPmuX_7TeV_part06.root',
    'plotsZtoMuTau_InclusivePPmuX_7TeV_part07.root',
    'plotsZtoMuTau_InclusivePPmuX_7TeV_part08.root',
    'plotsZtoMuTau_InclusivePPmuX_7TeV_part09.root',
    'plotsZtoMuTau_InclusivePPmuX_7TeV_part10.root',
    'plotsZtoMuTau_InclusivePPmuX_7TeV_part11.root',
    'plotsZtoMuTau_InclusivePPmuX_7TeV_part12.root',
    'plotsZtoMuTau_InclusivePPmuX_7TeV_part13.root',
    'plotsZtoMuTau_InclusivePPmuX_7TeV_part14.root',
    'plotsZtoMuTau_InclusivePPmuX_7TeV_part15.root',
    'plotsZtoMuTau_InclusivePPmuX_7TeV_part16.root',
    'plotsZtoMuTau_InclusivePPmuX_7TeV_part17.root',
    'plotsZtoMuTau_InclusivePPmuX_7TeV_part18.root',
    'plotsZtoMuTau_InclusivePPmuX_7TeV_part19.root',
    'plotsZtoMuTau_InclusivePPmuX_7TeV_part20.root',
    'plotsZtoMuTau_InclusivePPmuX_7TeV_part21.root',
    'plotsZtoMuTau_InclusivePPmuX_7TeV_part22.root',
    'plotsZtoMuTau_InclusivePPmuX_7TeV_part23.root',
    'plotsZtoMuTau_InclusivePPmuX_7TeV_part24.root',
    'plotsZtoMuTau_InclusivePPmuX_7TeV_part25.root',
    'plotsZtoMuTau_InclusivePPmuX_7TeV_part26.root',
    'plotsZtoMuTau_InclusivePPmuX_7TeV_part27.root',
    'plotsZtoMuTau_InclusivePPmuX_7TeV_part28.root'    
)

processZtoMuTau_InclusivePPmuX.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoMuTau_InclusivePPmuX_7TeV*intLumiZtoMuTau_Data_7TeV/intLumiZtoMuTau_InclusivePPmuX_7TeV)

processZtoMuTau_InclusivePPmuXsum = copy.deepcopy(process_InclusivePPmuX)
processZtoMuTau_InclusivePPmuXsum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoMuTau_InclusivePPmuXSum.root'
)
processZtoMuTau_InclusivePPmuXsum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processZtoMuTau_InclusivePPmuXsum.config_dqmFileLoader.scaleFactor = cms.double(1.)

#--------------------------------------------------------------------------------

processZtoMuTau_PPmuXptGt20_part01 = copy.deepcopy(process_PPmuXptGt20)
processZtoMuTau_PPmuXptGt20_part01.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part01.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part02.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part03.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part04.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part05.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part06.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part07.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part08.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part09.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part10.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part11.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part12.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part13.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part14.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part15.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part16.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part17.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part18.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part19.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part20.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part21.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part22.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part23.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part24.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part25.root'
)
processZtoMuTau_PPmuXptGt20_part01.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoMuTau_PPmuXptGt20_7TeV*intLumiZtoMuTau_Data_7TeV/intLumiZtoMuTau_PPmuXptGt20_7TeV)

processZtoMuTau_PPmuXptGt20_part02 = copy.deepcopy(process_PPmuXptGt20)
processZtoMuTau_PPmuXptGt20_part02.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part26.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part27.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part28.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part29.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part30.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part31.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part32.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part33.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part34.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part35.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part36.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part37.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part38.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part39.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part40.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part41.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part42.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part43.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part44.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part45.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part46.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part47.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part48.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part49.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part50.root'
)
processZtoMuTau_PPmuXptGt20_part02.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoMuTau_PPmuXptGt20_7TeV*intLumiZtoMuTau_Data_7TeV/intLumiZtoMuTau_PPmuXptGt20_7TeV)

processZtoMuTau_PPmuXptGt20_part03 = copy.deepcopy(process_PPmuXptGt20)
processZtoMuTau_PPmuXptGt20_part03.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part51.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part52.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part53.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part54.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part55.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part56.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part57.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part58.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part59.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part60.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part61.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part62.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part63.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part64.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part65.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part66.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part67.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part68.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part69.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part70.root',
    'plotsZtoMuTau_PPmuXptGt20_7TeV_part71.root'
)
processZtoMuTau_PPmuXptGt20_part03.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoMuTau_PPmuXptGt20_7TeV*intLumiZtoMuTau_Data_7TeV/intLumiZtoMuTau_PPmuXptGt20_7TeV)

processZtoMuTau_PPmuXptGt20Sum = copy.deepcopy(process_PPmuXptGt20)
processZtoMuTau_PPmuXptGt20Sum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoMuTau_PPmuXptGt20_part01Sum.root',
    'plotsZtoMuTau_PPmuXptGt20_part02Sum.root',
    'plotsZtoMuTau_PPmuXptGt20_part03Sum.root'
)
processZtoMuTau_PPmuXptGt20Sum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processZtoMuTau_PPmuXptGt20Sum.config_dqmFileLoader.scaleFactor = cms.double(1.)

#--------------------------------------------------------------------------------

processZtoMuTau_TTplusJets = copy.deepcopy(process_TTplusJets)
processZtoMuTau_TTplusJets.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoMuTau_TTplusJets_7TeV_part01.root',
    'plotsZtoMuTau_TTplusJets_7TeV_part02.root',
    'plotsZtoMuTau_TTplusJets_7TeV_part03.root',
    'plotsZtoMuTau_TTplusJets_7TeV_part04.root',
    'plotsZtoMuTau_TTplusJets_7TeV_part05.root',
    'plotsZtoMuTau_TTplusJets_7TeV_part06.root',
    'plotsZtoMuTau_TTplusJets_7TeV_part07.root',
    'plotsZtoMuTau_TTplusJets_7TeV_part08.root',
    'plotsZtoMuTau_TTplusJets_7TeV_part09.root',
    'plotsZtoMuTau_TTplusJets_7TeV_part10.root'
)
processZtoMuTau_TTplusJets.config_dqmFileLoader.scaleFactor = cms.double(corrFactorZtoMuTau_TTplusJets_7TeV*intLumiZtoMuTau_Data_7TeV/intLumiZtoMuTau_TTplusJets_7TeV)

processZtoMuTau_TTplusJetsSum = copy.deepcopy(process_TTplusJets)
processZtoMuTau_TTplusJetsSum.config_dqmFileLoader.inputFileNames = cms.vstring(
    'plotsZtoMuTau_TTplusJetsSum.root'
)
processZtoMuTau_TTplusJetsSum.config_dqmFileLoader.dqmDirectory_store = cms.string('harvested')
processZtoMuTau_TTplusJetsSum.config_dqmFileLoader.scaleFactor = cms.double(1.)
