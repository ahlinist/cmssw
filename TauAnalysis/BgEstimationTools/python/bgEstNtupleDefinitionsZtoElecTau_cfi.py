import FWCore.ParameterSet.Config as cms
import copy

# define Ntuple files used for data-driven background estimation methods
# in Z --> e + tau-jet channel

bgEstNtupleDirectoryName = cms.string("rfio:/castor/cern.ch/user/v/veelken/bgEstNtuples/ZtoElecTauII/")

# Z --> tau+ tau- sample
fileNames_Ztautau = cms.vstring(
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_Ztautau.root'
)

# Z --> e+ e- sample
fileNames_Zee = cms.vstring(
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_Zee_part01.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_Zee_part02.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_Zee_part03.root'
)

#--------------------------------------------------------------------------------

# Z --> tau+ tau- + jets sample
fileNames_ZtautauPlusJets = cms.vstring(
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_ZtautauPlusJets_part01.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_ZtautauPlusJets_part02.root'
)

# Z --> e+ e- + jets sample
fileNames_ZeePlusJets = cms.vstring(
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_ZeePlusJets_part01.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_ZeePlusJets_part02.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_ZeePlusJets_part03.root'
)

#--------------------------------------------------------------------------------

# W + jets sample
fileNames_WplusJets = cms.vstring(
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_WplusJets_part01.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_WplusJets_part02.root'
)

#--------------------------------------------------------------------------------

# gamma + jets samples
fileNames_gammaPlusJetsSum = cms.vstring(
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_PhotonJets_Pt15to20.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_PhotonJets_Pt20to25.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_PhotonJets_Pt25to30.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_PhotonJets_Pt30to35.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_PhotonJets_PtGt35.root'
)

#--------------------------------------------------------------------------------

# QCD b/c and em. enriched samples
fileNames_qcdSum = cms.vstring(
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_BCtoE_Pt20to30_part01.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_BCtoE_Pt20to30_part02.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_BCtoE_Pt20to30_part03.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_BCtoE_Pt30to80_part01.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_BCtoE_Pt30to80_part02.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_BCtoE_Pt30to80_part03.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_BCtoE_Pt80to170_part01.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_BCtoE_Pt80to170_part02.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt20to30_part01.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt20to30_part02.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt30to80_part01.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt30to80_part02.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt30to80_part03.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt30to80_part04.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt30to80_part05.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt30to80_part06.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt30to80_part07.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt30to80_part08.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt30to80_part09.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt80to170_part01.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt80to170_part02.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt80to170_part03.root'
)

#--------------------------------------------------------------------------------

# ttbar + jets sample
fileNames_TTplusJets = cms.vstring(
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_TTplusJets_part01.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_TTplusJets_part02.root'
)

#--------------------------------------------------------------------------------

# (pseudo)Data sample = sum of all signal + background Monte Carlo samples
fileNames_pseudoData = cms.vstring()
fileNames_pseudoData.extend(fileNames_Ztautau)
fileNames_pseudoData.extend(fileNames_ZeePlusJets)
fileNames_pseudoData.extend(fileNames_WplusJets)
fileNames_pseudoData.extend(fileNames_TTplusJets)
#fileNames_pseudoData.extend(fileNames_gammaPlusJetsSum)
fileNames_pseudoData.extend(fileNames_qcdSum)
