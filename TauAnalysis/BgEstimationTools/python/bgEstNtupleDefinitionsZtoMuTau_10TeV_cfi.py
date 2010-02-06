import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------
# define Ntuple files used for data-driven background estimation methods
# in Z --> mu + tau-jet channel
#--------------------------------------------------------------------------------

bgEstNtupleDirectoryName = cms.string("rfio:/castor/cern.ch/user/v/veelken/CMSSW_3_1_2/bgEstNtuples/ZtoMuTau/10TeV/")

#--------------------------------------------------------------------------------
# Z --> tau+ tau+ sample
fileNamesZtoMuTau_Ztautau_10TeV = cms.vstring(
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_Ztautau_10TeV_part01.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_Ztautau_10TeV_part02.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_Ztautau_10TeV_part03.root'   
)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# Z --> mu+ mu- sample
fileNamesZtoMuTau_Zmumu_10TeV = cms.vstring(
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_Zmumu_10TeV_part01.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_Zmumu_10TeV_part02.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_Zmumu_10TeV_part03.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_Zmumu_10TeV_part04.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_Zmumu_10TeV_part05.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_Zmumu_10TeV_part06.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_Zmumu_10TeV_part07.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_Zmumu_10TeV_part08.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_Zmumu_10TeV_part09.root'
)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# W + jets sample
fileNamesZtoMuTau_WplusJets_10TeV = cms.vstring(
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_WplusJets_10TeV_part01.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_WplusJets_10TeV_part02.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_WplusJets_10TeV_part03.root'
)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# pp --> mu X QCD sample
fileNamesZtoMuTau_qcdSum_10TeV = cms.vstring(
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_InclusivePPmuX_10TeV_part01.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_InclusivePPmuX_10TeV_part02.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_PPmuXptGt20_10TeV_part01.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_PPmuXptGt20_10TeV_part02.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_PPmuXptGt20_10TeV_part03.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_PPmuXptGt20_10TeV_part04.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_PPmuXptGt20_10TeV_part05.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_PPmuXptGt20_10TeV_part06.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_PPmuXptGt20_10TeV_part07.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_PPmuXptGt20_10TeV_part08.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_PPmuXptGt20_10TeV_part09.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_PPmuXptGt20_10TeV_part10.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_PPmuXptGt20_10TeV_part11.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_PPmuXptGt20_10TeV_part12.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_PPmuXptGt20_10TeV_part13.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_PPmuXptGt20_10TeV_part14.root'
)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# ttbar + jets sample
fileNamesZtoMuTau_TTplusJets_10TeV = cms.vstring(
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_TTplusJets_10TeV_part01.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_TTplusJets_10TeV_part02.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_TTplusJets_10TeV_part03.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_TTplusJets_10TeV_part04.root'
)
#--------------------------------------------------------------------------------

# (pseudo)Data sample = sum of all signal + background Monte Carlo samples
fileNamesZtoMuTau_pseudoData_10TeV = cms.vstring()
fileNamesZtoMuTau_pseudoData_10TeV.extend(fileNamesZtoMuTau_Ztautau_10TeV)
fileNamesZtoMuTau_pseudoData_10TeV.extend(fileNamesZtoMuTau_Zmumu_10TeV)
fileNamesZtoMuTau_pseudoData_10TeV.extend(fileNamesZtoMuTau_WplusJets_10TeV)
fileNamesZtoMuTau_pseudoData_10TeV.extend(fileNamesZtoMuTau_TTplusJets_10TeV)
fileNamesZtoMuTau_pseudoData_10TeV.extend(fileNamesZtoMuTau_qcdSum_10TeV)
