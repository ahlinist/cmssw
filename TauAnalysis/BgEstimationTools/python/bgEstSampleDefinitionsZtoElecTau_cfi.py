import FWCore.ParameterSet.Config as cms
import copy

# define configuration parameters for submission of Z --> e + tau-jet jobs to CERN batch system
# (running over skimmed samples stored on CASTOR)

bgEstSampleDirectoryName = cms.string("rfio:/castor/cern.ch/user/v/veelken/bgEstSkim/ZtoElecTau/")

# definitions for producing skimmed samples
bgEstSampleOutputFileNameZtautau = cms.untracked.string('bgEstSampleZtoElecTau_Ztautau_partXX.root')

# definitions for reading skimmed samples
bgEstSampleFileNamesZtautau = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Ztautau_part01.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Ztautau_part02.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Ztautau_part03.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Ztautau_part04.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Ztautau_part05.root'
)

# definitions for producing ntuples
bgEstNtupleOutputFileNameZtautau = cms.string('bgEstNtupleZtoElecTau_Ztautau.root')

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
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Zee_part24.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Zee_part25.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Zee_part26.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Zee_part27.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_Zee_part28.root'
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
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZtautauPlusJets_part05.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZtautauPlusJets_part06.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZtautauPlusJets_part07.root'
    
)

bgEstSampleFileNamesZtautauPlusJets_part02 = cms.untracked.vstring(
    #bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZtautauPlusJets_part08.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZtautauPlusJets_part09.root',
    #bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZtautauPlusJets_part10.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZtautauPlusJets_part11.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZtautauPlusJets_part12.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZtautauPlusJets_part13.root'
)

bgEstSampleFileNamesZeePlusJets_part01 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZeePlusJets_part01.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZeePlusJets_part02.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZeePlusJets_part03.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZeePlusJets_part04.root'
)

bgEstSampleFileNamesZeePlusJets_part02 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZeePlusJets_part05.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZeePlusJets_part06.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZeePlusJets_part07.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZeePlusJets_part08.root'
    
)

bgEstSampleFileNamesZeePlusJets_part03 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZeePlusJets_part09.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZeePlusJets_part10.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZeePlusJets_part11.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZeePlusJets_part12.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_ZeePlusJets_part13.root'
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
    #bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_WplusJets_part13.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_WplusJets_part14.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_WplusJets_part15.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_WplusJets_part16.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_WplusJets_part17.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_WplusJets_part18.root'
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
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt20to30_part24.root'
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
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt30to80_part28.root'
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
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_BCtoE_Pt80to170_part15.root'
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
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt20to30_part16.root'
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
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_QCD_EMenriched_Pt30to80_part83.root'
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
    #bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_TTplusJets_part04.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_TTplusJets_part05.root'
)

bgEstSampleFileNamesTTplusJets_part02  = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_TTplusJets_part06.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_TTplusJets_part07.root',
    #bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_TTplusJets_part08.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_TTplusJets_part09.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecTau_TTplusJets_part10.root'
)

# definitions for producing ntuples
bgEstNtupleOutputFileNameTTplusJets = cms.string('bgEstNtupleZtoElecTau_TTplusJets_partXX.root')

