import FWCore.ParameterSet.Config as cms
import copy

# define Ntuple files used for data-driven background estimation methods
# in Z --> mu + tau-jet channel

bgEstNtupleDirectoryName = cms.string("rfio:/castor/cern.ch/user/v/veelken/bgEstNtuples/ZtoElecMu/")

# Z --> tau tau sample
fileNames_Ztautau = cms.vstring(
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecMu_Ztautau.root'
)

# Z --> mu mu sample
fileNames_Zmumu = cms.vstring(
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecMu_Zmumu.root'
)

# Z --> e e sample
fileNames_Zee = cms.vstring(
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecMu_Zee.root'
)

#--------------------------------------------------------------------------------

# Z --> tau tau + jets sample
fileNames_ZtautauPlusJets = cms.vstring(
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecMu_ZtautauPlusJets.root'
)

# Z --> mu mu + jets sample
fileNames_ZmumuPlusJets = cms.vstring(
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecMu_ZmumuPlusJets.root'
)

# Z --> e e + jets sample
fileNames_ZeePlusJets = cms.vstring(
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecMu_ZeePlusJets.root'
)

#--------------------------------------------------------------------------------

# W + jets sample
fileNames_WplusJets = cms.vstring(
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecMu_WplusJets.root'
)

#--------------------------------------------------------------------------------

# pp --> mu X QCD sample
fileNames_qcdSum = cms.vstring(
    #bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecMu_InclusivePPmuX.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecMu_PPmuXptGt20_part01.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecMu_PPmuXptGt20_part02.root'
)

#--------------------------------------------------------------------------------

# ttbar + jets sample
fileNames_TTplusJets = cms.vstring(
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecMu_TTplusJets_part01.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecMu_TTplusJets_part02.root'
)

#--------------------------------------------------------------------------------

# (pseudo)Data sample = sum of all signal + background Monte Carlo samples
fileNames_pseudoData = cms.vstring()
fileNames_pseudoData.extend(fileNames_Ztautau)
fileNames_pseudoData.extend(fileNames_ZeePlusJets)
fileNames_pseudoData.extend(fileNames_ZmumuPlusJets)
fileNames_pseudoData.extend(fileNames_WplusJets)
fileNames_pseudoData.extend(fileNames_TTplusJets)
fileNames_pseudoData.extend(fileNames_qcdSum)
