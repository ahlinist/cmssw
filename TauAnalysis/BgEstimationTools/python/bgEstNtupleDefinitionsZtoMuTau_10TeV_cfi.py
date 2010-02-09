import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------
# define Ntuple files used for data-driven background estimation methods
# in Z --> mu + tau-jet channel
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# CV: need to be **very** careful with commenting-out corrupt files,
#     as in that case exceptions of the type
#
#       A cms::Exception is going through WorkerT<EDAnalyzer>:
#
#       %MSG
#
#     will be thrown, without any indication that an input file (and which one) is corrupt !!
#--------------------------------------------------------------------------------

bgEstNtupleDirectoryName = cms.string("rfio:/castor/cern.ch/user/v/veelken/CMSSW_3_1_2/bgEstNtuples/ZtoMuTau/10TeVii/")

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
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_WplusJets_10TeV_part03.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_WplusJets_10TeV_part04.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_WplusJets_10TeV_part05.root'
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
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_PPmuXptGt20_10TeV_part14.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_PPmuXptGt20_10TeV_part15.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_PPmuXptGt20_10TeV_part16.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_PPmuXptGt20_10TeV_part17.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_PPmuXptGt20_10TeV_part18.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_PPmuXptGt20_10TeV_part19.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_PPmuXptGt20_10TeV_part20.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_PPmuXptGt20_10TeV_part21.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_PPmuXptGt20_10TeV_part22.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_PPmuXptGt20_10TeV_part23.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_PPmuXptGt20_10TeV_part24.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_PPmuXptGt20_10TeV_part25.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_PPmuXptGt20_10TeV_part26.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_PPmuXptGt20_10TeV_part27.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_PPmuXptGt20_10TeV_part28.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_PPmuXptGt20_10TeV_part29.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_PPmuXptGt20_10TeV_part30.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_PPmuXptGt20_10TeV_part31.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_PPmuXptGt20_10TeV_part32.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_PPmuXptGt20_10TeV_part33.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoMuTau_PPmuXptGt20_10TeV_part34.root'
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
