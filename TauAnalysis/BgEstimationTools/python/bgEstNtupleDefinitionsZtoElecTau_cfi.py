import FWCore.ParameterSet.Config as cms
import copy

# define Ntuple files used for data-driven background estimation methods
# in Z --> e + tau-jet channel

bgEstNtupleDirectoryName = cms.string("rfio:/castor/cern.ch/user/v/veelken/bgEstNtuples/ZtoElecTau_looseElectronSel/")

# Z --> tau+ tau- sample
fileNamesZtoElecTau_Ztautau = cms.vstring(
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_Ztautau_part01.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_Ztautau_part02.root'
)

# Z --> e+ e- sample
fileNamesZtoElecTau_Zee = cms.vstring(
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_Zee_part01.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_Zee_part02.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_Zee_part03.root'
)

#--------------------------------------------------------------------------------

# Z --> tau+ tau- + jets sample
fileNamesZtoElecTau_ZtautauPlusJets = cms.vstring(
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_ZtautauPlusJets_part01.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_ZtautauPlusJets_part02.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_ZtautauPlusJets_part03.root'
)

# Z --> e+ e- + jets sample
fileNamesZtoElecTau_ZeePlusJets = cms.vstring(
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_ZeePlusJets_part01.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_ZeePlusJets_part02.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_ZeePlusJets_part03.root'
)

#--------------------------------------------------------------------------------

# W + jets sample
fileNamesZtoElecTau_WplusJets = cms.vstring(
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_WplusJets_part01.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_WplusJets_part02.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_WplusJets_part03.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_WplusJets_part04.root'
)

#--------------------------------------------------------------------------------

# gamma + jets samples
fileNamesZtoElecTau_gammaPlusJetsSum = cms.vstring(
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_PhotonJets_Pt15to20.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_PhotonJets_Pt20to25.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_PhotonJets_Pt25to30.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_PhotonJets_Pt30to35.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_PhotonJets_PtGt35.root'
)

#--------------------------------------------------------------------------------

# QCD b/c and em. enriched samples
fileNamesZtoElecTau_qcdSum = cms.vstring(
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_BCtoE_Pt20to30_part01.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_BCtoE_Pt20to30_part02.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_BCtoE_Pt20to30_part03.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_BCtoE_Pt30to80_part01.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_BCtoE_Pt30to80_part02.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_BCtoE_Pt30to80_part03.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_BCtoE_Pt30to80_part04.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_BCtoE_Pt30to80_part05.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_BCtoE_Pt80to170_part01.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_BCtoE_Pt80to170_part02.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_BCtoE_Pt80to170_part03.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt20to30_part01.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt20to30_part02.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt20to30_part03.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt20to30_part04.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt20to30_part05.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt20to30_part06.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt30to80_part01.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt30to80_part02.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt30to80_part03.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt30to80_part04.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt30to80_part05.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt30to80_part06.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt30to80_part07.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt30to80_part08.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt30to80_part09.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt30to80_part10.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt30to80_part11.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt30to80_part12.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt30to80_part13.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt30to80_part14.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt30to80_part15.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt30to80_part16.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt30to80_part17.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt30to80_part18.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt30to80_part19.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt30to80_part20.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt30to80_part21.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt30to80_part22.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt80to170_part01.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt80to170_part02.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt80to170_part03.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt80to170_part04.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt80to170_part05.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_QCD_EMenriched_Pt80to170_part06.root'
)

#--------------------------------------------------------------------------------

# ttbar + jets sample
fileNamesZtoElecTau_TTplusJets = cms.vstring(
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_TTplusJets_part01.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_TTplusJets_part02.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_TTplusJets_part03.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_TTplusJets_part04.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_TTplusJets_part05.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_TTplusJets_part06.root',
    bgEstNtupleDirectoryName.value() + 'bgEstNtupleZtoElecTau_TTplusJets_part07.root',
)

#--------------------------------------------------------------------------------

# (pseudo)Data sample = sum of all signal + background Monte Carlo samples
fileNamesZtoElecTau_pseudoData = cms.vstring()
fileNamesZtoElecTau_pseudoData.extend(fileNamesZtoElecTau_Ztautau)
fileNamesZtoElecTau_pseudoData.extend(fileNamesZtoElecTau_ZeePlusJets)
fileNamesZtoElecTau_pseudoData.extend(fileNamesZtoElecTau_WplusJets)
fileNamesZtoElecTau_pseudoData.extend(fileNamesZtoElecTau_TTplusJets)
fileNamesZtoElecTau_pseudoData.extend(fileNamesZtoElecTau_gammaPlusJetsSum)
fileNamesZtoElecTau_pseudoData.extend(fileNamesZtoElecTau_qcdSum)
