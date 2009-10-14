import FWCore.ParameterSet.Config as cms
import copy

# define configuration parameters for submission of Z --> e + tau-jet jobs to CERN batch system
# (running over skimmed samples stored on CASTOR)

bgEstSampleDirectoryName = cms.string("rfio:/castor/cern.ch/user/v/veelken/bgEstSkim/ZtoElecTau_looseElectronSel/")

# definitions for producing skimmed samples
bgEstSampleOutputFileNameZtautau = cms.untracked.string('bgEstSampleZtoElecTau_Ztautau_partXX.root')

# definitions for reading skimmed samples
bgEstSampleFileNamesZtautau_part01 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Ztautau_part01.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Ztautau_part02.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Ztautau_part03.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Ztautau_part04.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Ztautau_part05.root'
)

bgEstSampleFileNamesZtautau_part02 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Ztautau_part06.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Ztautau_part07.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Ztautau_part08.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Ztautau_part09.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Ztautau_part10.root'
)

# definitions for producing ntuples
bgEstNtupleOutputFileNameZtautau = cms.string('bgEstNtupleZtoElecTau_Ztautau_partXX.root')

#--------------------------------------------------------------------------------

# definitions for producing skimmed samples
bgEstSampleOutputFileNameZee = cms.untracked.string('bgEstSampleZtoElecTau_Zee_partXX.root')

# definitions for reading skimmed samples
bgEstSampleFileNamesZee_part01 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Zee_part01.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Zee_part02.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Zee_part03.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Zee_part04.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Zee_part05.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Zee_part06.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Zee_part07.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Zee_part08.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Zee_part09.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Zee_part10.root'
)

bgEstSampleFileNamesZee_part02 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Zee_part11.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Zee_part12.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Zee_part13.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Zee_part14.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Zee_part15.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Zee_part16.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Zee_part17.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Zee_part18.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Zee_part19.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Zee_part20.root'
)

bgEstSampleFileNamesZee_part03 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Zee_part21.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Zee_part22.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Zee_part23.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Zee_part24.root'
)

# definitions for producing ntuples
bgEstNtupleOutputFileNameZee = cms.string('bgEstNtupleZtoElecTau_Zee_partXX.root')

#--------------------------------------------------------------------------------

# definitions for producing skimmed samples
bgEstSampleOutputFileNameZtautauPlusJets = cms.untracked.string('bgEstSampleZtoElecTau_ZtautauPlusJets_partXX.root')

bgEstSampleOutputFileNameZeePlusJets = cms.untracked.string('bgEstSampleZtoElecTau_ZeePlusJets_partXX.root')

# definitions for reading skimmed samples
bgEstSampleFileNamesZtautauPlusJets_part01 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZtautauPlusJets_part01.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZtautauPlusJets_part02.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZtautauPlusJets_part03.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZtautauPlusJets_part04.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZtautauPlusJets_part05.root'
)

bgEstSampleFileNamesZtautauPlusJets_part02 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZtautauPlusJets_part06.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZtautauPlusJets_part07.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZtautauPlusJets_part08.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZtautauPlusJets_part09.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZtautauPlusJets_part10.root'
)

bgEstSampleFileNamesZtautauPlusJets_part03 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZtautauPlusJets_part11.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZtautauPlusJets_part12.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZtautauPlusJets_part13.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZtautauPlusJets_part14.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZtautauPlusJets_part15.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZtautauPlusJets_part16.root'
)

bgEstSampleFileNamesZeePlusJets_part01 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZeePlusJets_part01.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZeePlusJets_part02.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZeePlusJets_part03.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZeePlusJets_part04.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZeePlusJets_part05.root'
)

bgEstSampleFileNamesZeePlusJets_part02 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZeePlusJets_part06.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZeePlusJets_part07.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZeePlusJets_part08.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZeePlusJets_part09.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZeePlusJets_part10.root'
)

bgEstSampleFileNamesZeePlusJets_part03 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZeePlusJets_part11.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZeePlusJets_part12.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZeePlusJets_part13.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZeePlusJets_part14.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZeePlusJets_part15.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZeePlusJets_part16.root'
)

# definitions for producing ntuples
bgEstNtupleOutputFileNameZtautauPlusJets = cms.string('bgEstNtupleZtoElecTau_ZtautauPlusJets_partXX.root')

bgEstNtupleOutputFileNameZeePlusJets = cms.string('bgEstNtupleZtoElecTau_ZeePlusJets_partXX.root')

#--------------------------------------------------------------------------------

# definitions for producing skimmed samples
bgEstSampleOutputFileNameWplusJets = cms.untracked.string('bgEstSampleZtoElecTau_WplusJets_partXX.root')

# definitions for reading skimmed samples
bgEstSampleFileNamesWplusJets_part01 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_WplusJets_part01.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_WplusJets_part02.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_WplusJets_part03.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_WplusJets_part04.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_WplusJets_part05.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_WplusJets_part06.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_WplusJets_part07.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_WplusJets_part08.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_WplusJets_part09.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_WplusJets_part10.root'
)

bgEstSampleFileNamesWplusJets_part02 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_WplusJets_part11.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_WplusJets_part12.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_WplusJets_part13.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_WplusJets_part14.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_WplusJets_part15.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_WplusJets_part16.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_WplusJets_part17.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_WplusJets_part18.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_WplusJets_part19.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_WplusJets_part20.root'
)

bgEstSampleFileNamesWplusJets_part03 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_WplusJets_part21.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_WplusJets_part22.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_WplusJets_part23.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_WplusJets_part24.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_WplusJets_part25.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_WplusJets_part26.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_WplusJets_part27.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_WplusJets_part28.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_WplusJets_part29.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_WplusJets_part30.root'
)

bgEstSampleFileNamesWplusJets_part04 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_WplusJets_part31.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_WplusJets_part32.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_WplusJets_part33.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_WplusJets_part34.root'
)

# definitions for producing ntuples
bgEstNtupleOutputFileNameWplusJets = cms.string('bgEstNtupleZtoElecTau_WplusJets_partXX.root')

#--------------------------------------------------------------------------------

# definitions for producing skimmed samples
bgEstSampleOutputFileNamePhotonJets_Pt15to20 = cms.untracked.string('bgEstSampleZtoElecTau_PhotonJets_Pt15to20.root')
bgEstSampleOutputFileNamePhotonJets_Pt20to25 = cms.untracked.string('bgEstSampleZtoElecTau_PhotonJets_Pt20to25.root')
bgEstSampleOutputFileNamePhotonJets_Pt25to30 = cms.untracked.string('bgEstSampleZtoElecTau_PhotonJets_Pt25to30.root')
bgEstSampleOutputFileNamePhotonJets_Pt30to35 = cms.untracked.string('bgEstSampleZtoElecTau_PhotonJets_Pt30to35.root')
bgEstSampleOutputFileNamePhotonJets_PtGt35 = cms.untracked.string('bgEstSampleZtoElecTau_PhotonJets_PtGt35.root')

# definitions for reading skimmed samples
bgEstSampleFileNamesPhotonJets_Pt15to20 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_PhotonJets_Pt15to20.root'
)

bgEstSampleFileNamesPhotonJets_Pt20to25 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_PhotonJets_Pt20to25.root'
)

bgEstSampleFileNamesPhotonJets_Pt25to30 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_PhotonJets_Pt25to30.root'
)

bgEstSampleFileNamesPhotonJets_Pt30to35 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_PhotonJets_Pt30to35.root'
)

bgEstSampleFileNamesPhotonJets_PtGt35 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_PhotonJets_PtGt35.root'
)

# definitions for producing ntuples
bgEstNtupleOutputFileNamePhotonJets_Pt15to20 = cms.string('bgEstNtupleZtoElecTau_PhotonJets_Pt15to20.root')
bgEstNtupleOutputFileNamePhotonJets_Pt20to25 = cms.string('bgEstNtupleZtoElecTau_PhotonJets_Pt20to25.root')
bgEstNtupleOutputFileNamePhotonJets_Pt25to30 = cms.string('bgEstNtupleZtoElecTau_PhotonJets_Pt25to30.root')
bgEstNtupleOutputFileNamePhotonJets_Pt30to35 = cms.string('bgEstNtupleZtoElecTau_PhotonJets_Pt30to35.root')
bgEstNtupleOutputFileNamePhotonJets_PtGt35 = cms.string('bgEstNtupleZtoElecTau_PhotonJets_PtGt35.root')

#--------------------------------------------------------------------------------

# definitions for producing skimmed samples
bgEstSampleOutputFileNameQCD_BCtoE_Pt20to30 = cms.untracked.string('bgEstSampleZtoElecTau_QCD_BCtoE_Pt20to30_partXX.root')
bgEstSampleOutputFileNameQCD_BCtoE_Pt30to80 = cms.untracked.string('bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_partXX.root')
bgEstSampleOutputFileNameQCD_BCtoE_Pt80to170 = cms.untracked.string('bgEstSampleZtoElecTau_QCD_BCtoE_Pt80to170_partXX.root')

bgEstSampleOutputFileNameQCD_EMenriched_Pt20to30 = cms.untracked.string('bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_partXX.root')
bgEstSampleOutputFileNameQCD_EMenriched_Pt30to80 = cms.untracked.string('bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_partXX.root')
bgEstSampleOutputFileNameQCD_EMenriched_Pt80to170 = cms.untracked.string('bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_partXX.root')

# definitions for reading skimmed samples
bgEstSampleFileNamesQCD_BCtoE_Pt20to30_part01 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt20to30_part01.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt20to30_part02.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt20to30_part03.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt20to30_part04.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt20to30_part05.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt20to30_part06.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt20to30_part07.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt20to30_part08.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt20to30_part09.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt20to30_part10.root'
)

bgEstSampleFileNamesQCD_BCtoE_Pt20to30_part02 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt20to30_part11.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt20to30_part12.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt20to30_part13.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt20to30_part14.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt20to30_part15.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt20to30_part16.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt20to30_part17.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt20to30_part18.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt20to30_part19.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt20to30_part20.root'
)

bgEstSampleFileNamesQCD_BCtoE_Pt20to30_part03 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt20to30_part21.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt20to30_part22.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt20to30_part23.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt20to30_part24.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt20to30_part25.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt20to30_part26.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt20to30_part27.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt20to30_part28.root'
)

bgEstSampleFileNamesQCD_BCtoE_Pt30to80_part01 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part01.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part02.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part03.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part04.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part05.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part06.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part07.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part08.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part09.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part10.root'
)

bgEstSampleFileNamesQCD_BCtoE_Pt30to80_part02 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part11.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part12.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part13.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part14.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part15.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part16.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part17.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part18.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part19.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part20.root'
)

bgEstSampleFileNamesQCD_BCtoE_Pt30to80_part03 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part21.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part22.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part23.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part24.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part25.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part26.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part27.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part28.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part29.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part30.root'
)

bgEstSampleFileNamesQCD_BCtoE_Pt30to80_part04 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part31.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part32.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part33.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part34.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part35.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part36.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part37.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part38.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part39.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part40.root'
)

bgEstSampleFileNamesQCD_BCtoE_Pt30to80_part05 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part41.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part42.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part43.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part44.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part45.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part46.root'
)

bgEstSampleFileNamesQCD_BCtoE_Pt80to170_part01 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt80to170_part01.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt80to170_part02.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt80to170_part03.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt80to170_part04.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt80to170_part05.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt80to170_part06.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt80to170_part07.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt80to170_part08.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt80to170_part09.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt80to170_part10.root'
)

bgEstSampleFileNamesQCD_BCtoE_Pt80to170_part02 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt80to170_part11.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt80to170_part12.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt80to170_part13.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt80to170_part14.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt80to170_part15.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt80to170_part16.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt80to170_part17.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt80to170_part18.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt80to170_part19.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt80to170_part20.root'
)

bgEstSampleFileNamesQCD_BCtoE_Pt80to170_part03 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt80to170_part21.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt80to170_part22.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt80to170_part23.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt80to170_part24.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt80to170_part25.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt80to170_part26.root'
)

bgEstSampleFileNamesQCD_EMenriched_Pt20to30_part01 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part01.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part02.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part03.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part04.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part05.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part06.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part07.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part08.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part09.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part10.root'
)

bgEstSampleFileNamesQCD_EMenriched_Pt20to30_part02 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part11.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part12.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part13.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part14.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part15.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part16.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part17.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part18.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part19.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part20.root'
)

bgEstSampleFileNamesQCD_EMenriched_Pt20to30_part03 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part21.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part22.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part23.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part24.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part25.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part26.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part27.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part28.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part29.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part30.root'
)

bgEstSampleFileNamesQCD_EMenriched_Pt20to30_part04 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part31.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part32.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part33.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part34.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part35.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part36.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part37.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part38.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part39.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part40.root'
)

bgEstSampleFileNamesQCD_EMenriched_Pt20to30_part05 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part41.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part42.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part43.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part44.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part45.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part46.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part47.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part48.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part49.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part50.root'
)

bgEstSampleFileNamesQCD_EMenriched_Pt20to30_part06 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part51.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part52.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part53.root'
)

bgEstSampleFileNamesQCD_EMenriched_Pt30to80_part01 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part01.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part02.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part03.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part04.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part05.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part06.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part07.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part08.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part09.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part10.root'
)

bgEstSampleFileNamesQCD_EMenriched_Pt30to80_part02 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part11.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part12.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part13.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part14.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part15.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part16.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part17.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part18.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part19.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part20.root'
)

bgEstSampleFileNamesQCD_EMenriched_Pt30to80_part03 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part21.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part22.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part23.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part24.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part25.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part26.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part27.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part28.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part29.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part30.root'
)

bgEstSampleFileNamesQCD_EMenriched_Pt30to80_part04 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part31.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part32.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part33.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part34.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part35.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part36.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part37.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part38.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part39.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part40.root'
)

bgEstSampleFileNamesQCD_EMenriched_Pt30to80_part05 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part41.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part42.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part43.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part44.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part45.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part46.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part47.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part48.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part49.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part50.root'
)

bgEstSampleFileNamesQCD_EMenriched_Pt30to80_part06 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part51.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part52.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part53.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part54.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part55.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part56.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part57.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part58.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part59.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part60.root'
)

bgEstSampleFileNamesQCD_EMenriched_Pt30to80_part07 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part61.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part62.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part63.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part64.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part65.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part66.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part67.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part68.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part69.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part70.root'
)

bgEstSampleFileNamesQCD_EMenriched_Pt30to80_part08 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part71.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part72.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part73.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part74.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part75.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part76.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part77.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part78.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part79.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part80.root'
)

bgEstSampleFileNamesQCD_EMenriched_Pt30to80_part09 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part81.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part82.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part83.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part84.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part85.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part86.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part87.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part88.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part89.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part90.root'
)

bgEstSampleFileNamesQCD_EMenriched_Pt30to80_part10 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part91.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part92.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part93.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part94.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part95.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part96.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part97.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part98.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part99.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part100.root'
)

bgEstSampleFileNamesQCD_EMenriched_Pt30to80_part11 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part101.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part102.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part103.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part104.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part105.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part106.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part107.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part108.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part109.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part110.root'
)

bgEstSampleFileNamesQCD_EMenriched_Pt30to80_part12 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part111.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part112.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part113.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part114.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part115.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part116.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part117.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part118.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part119.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part120.root'
)

bgEstSampleFileNamesQCD_EMenriched_Pt30to80_part13 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part121.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part122.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part123.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part124.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part125.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part126.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part127.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part128.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part129.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part130.root'
)

bgEstSampleFileNamesQCD_EMenriched_Pt30to80_part14 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part131.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part132.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part133.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part134.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part135.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part136.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part137.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part138.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part139.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part140.root'
)

bgEstSampleFileNamesQCD_EMenriched_Pt30to80_part15 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part141.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part142.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part143.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part144.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part145.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part146.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part147.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part148.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part149.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part150.root'
)

bgEstSampleFileNamesQCD_EMenriched_Pt30to80_part16 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part151.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part152.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part153.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part154.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part155.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part156.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part157.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part158.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part159.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part160.root'
)

bgEstSampleFileNamesQCD_EMenriched_Pt30to80_part17 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part161.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part162.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part163.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part164.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part165.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part166.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part167.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part168.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part169.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part170.root'
)

bgEstSampleFileNamesQCD_EMenriched_Pt30to80_part18 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part171.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part172.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part173.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part174.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part175.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part176.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part177.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part178.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part179.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part180.root'
)

bgEstSampleFileNamesQCD_EMenriched_Pt30to80_part19 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part181.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part182.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part183.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part184.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part185.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part186.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part187.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part188.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part189.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part190.root'
)

bgEstSampleFileNamesQCD_EMenriched_Pt30to80_part20 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part191.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part192.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part193.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part194.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part195.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part196.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part197.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part198.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part199.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part200.root'
)

bgEstSampleFileNamesQCD_EMenriched_Pt30to80_part21 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part201.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part202.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part203.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part204.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part205.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part206.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part207.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part208.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part209.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part210.root'
)

bgEstSampleFileNamesQCD_EMenriched_Pt30to80_part22 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part211.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part212.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part213.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part214.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part215.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part216.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part217.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part218.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part219.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part220.root'
)

bgEstSampleFileNamesQCD_EMenriched_Pt80to170_part01 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part01.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part02.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part03.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part04.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part05.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part06.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part07.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part08.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part09.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part10.root'
)    

bgEstSampleFileNamesQCD_EMenriched_Pt80to170_part02 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part11.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part12.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part13.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part14.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part15.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part16.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part17.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part18.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part19.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part20.root'
)

bgEstSampleFileNamesQCD_EMenriched_Pt80to170_part03 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part21.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part22.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part23.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part24.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part25.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part26.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part27.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part28.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part29.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part30.root'
) 
bgEstSampleFileNamesQCD_EMenriched_Pt80to170_part04 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part31.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part32.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part33.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part34.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part35.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part36.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part37.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part38.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part39.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part40.root'
) 

bgEstSampleFileNamesQCD_EMenriched_Pt80to170_part05 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part41.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part42.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part43.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part44.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part45.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part46.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part47.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part48.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part49.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part50.root'
) 

bgEstSampleFileNamesQCD_EMenriched_Pt80to170_part06 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part51.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part52.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part53.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part54.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part55.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part56.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part57.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part58.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt80to170_part59.root'
) 

# definitions for producing ntuples
bgEstNtupleOutputFileNameQCD_BCtoE_Pt20to30 = cms.string('bgEstNtupleZtoElecTau_QCD_BCtoE_Pt20to30_partXX.root')
bgEstNtupleOutputFileNameQCD_BCtoE_Pt30to80 = cms.string('bgEstNtupleZtoElecTau_QCD_BCtoE_Pt30to80_partXX.root')
bgEstNtupleOutputFileNameQCD_BCtoE_Pt80to170 = cms.string('bgEstNtupleZtoElecTau_QCD_BCtoE_Pt80to170_partXX.root')

bgEstNtupleOutputFileNameQCD_EMenriched_Pt20to30 = cms.string('bgEstNtupleZtoElecTau_QCD_EMenriched_Pt20to30_partXX.root')
bgEstNtupleOutputFileNameQCD_EMenriched_Pt30to80 = cms.string('bgEstNtupleZtoElecTau_QCD_EMenriched_Pt30to80_partXX.root')
bgEstNtupleOutputFileNameQCD_EMenriched_Pt80to170 = cms.string('bgEstNtupleZtoElecTau_QCD_EMenriched_Pt80to170_partXX.root')

#--------------------------------------------------------------------------------

# definitions for producing skimmed samples
bgEstSampleOutputFileNameTTplusJets = cms.untracked.string('bgEstSampleZtoElecTau_TTplusJets_partXX.root')

# definitions for reading skimmed samples
bgEstSampleFileNamesTTplusJets_part01  = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_TTplusJets_part01.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_TTplusJets_part02.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_TTplusJets_part03.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_TTplusJets_part04.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_TTplusJets_part05.root'
)

bgEstSampleFileNamesTTplusJets_part02  = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_TTplusJets_part06.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_TTplusJets_part07.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_TTplusJets_part08.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_TTplusJets_part09.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_TTplusJets_part10.root'
)

bgEstSampleFileNamesTTplusJets_part03  = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_TTplusJets_part11.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_TTplusJets_part12.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_TTplusJets_part13.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_TTplusJets_part14.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_TTplusJets_part15.root'
)

bgEstSampleFileNamesTTplusJets_part04  = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_TTplusJets_part16.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_TTplusJets_part17.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_TTplusJets_part18.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_TTplusJets_part19.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_TTplusJets_part20.root'
)

bgEstSampleFileNamesTTplusJets_part05  = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_TTplusJets_part21.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_TTplusJets_part22.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_TTplusJets_part23.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_TTplusJets_part24.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_TTplusJets_part25.root'
)

bgEstSampleFileNamesTTplusJets_part06  = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_TTplusJets_part26.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_TTplusJets_part27.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_TTplusJets_part28.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_TTplusJets_part29.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_TTplusJets_part30.root'
)

bgEstSampleFileNamesTTplusJets_part07  = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_TTplusJets_part31.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_TTplusJets_part32.root'
)

# definitions for producing ntuples
bgEstNtupleOutputFileNameTTplusJets = cms.string('bgEstNtupleZtoElecTau_TTplusJets_partXX.root')

