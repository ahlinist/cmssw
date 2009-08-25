import FWCore.ParameterSet.Config as cms
import copy

# define configuration parameters for submission of Z --> mu + tau-jet jobs to CERN batch system
# (running over skimmed samples stored on CASTOR)

bgEstSampleDirectoryName = cms.string("rfio:/castor/cern.ch/user/v/veelken/bgEstSkim/ZtoElecMu/")

# definitions for producing skimmed samples
bgEstSampleOutputFileNameZtautau = cms.untracked.string('bgEstSampleZtoElecMu_Ztautau.root')

# definitions for reading skimmed samples
bgEstSampleFileNamesZtautau = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_Ztautau.root'
)

# definitions for producing ntuples
bgEstNtupleOutputFileNameZtautau = cms.string('bgEstNtupleZtoElecMu_Ztautau.root')

#--------------------------------------------------------------------------------

# definitions for producing skimmed samples
bgEstSampleOutputFileNameZmumu = cms.untracked.string('bgEstSampleZtoElecMu_Zmumu.root')

# definitions for reading skimmed samples
bgEstSampleFileNamesZmumu = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_Zmumu.root'
)

# definitions for producing ntuples
bgEstNtupleOutputFileNameZmumu = cms.string('bgEstNtupleZtoElecMu_Zmumu.root')

#--------------------------------------------------------------------------------

# definitions for producing skimmed samples
bgEstSampleOutputFileNameZee = cms.untracked.string('bgEstSampleZtoElecMu_Zee.root')

# definitions for reading skimmed samples
bgEstSampleFileNamesZee = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_Zee.root'
)

# definitions for producing ntuples
bgEstNtupleOutputFileNameZee = cms.string('bgEstNtupleZtoElecMu_Zee.root')

#--------------------------------------------------------------------------------

# definitions for producing skimmed samples
bgEstSampleOutputFileNameZtautauPlusJets = cms.untracked.string('bgEstSampleZtoElecMu_ZtautauPlusJets.root')

bgEstSampleOutputFileNameZmumuPlusJets = cms.untracked.string('bgEstSampleZtoElecMu_ZmumuPlusJets.root')

bgEstSampleOutputFileNameZeePlusJets = cms.untracked.string('bgEstSampleZtoElecMu_ZeePlusJets.root')

# definitions for reading skimmed samples
bgEstSampleFileNamesZtautauPlusJets = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_ZtautauPlusJets.root'
)

bgEstSampleFileNamesZmumuPlusJets_part01 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_ZmumuPlusJets.root'
)

bgEstSampleFileNamesZeePlusJets = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_ZeePlusJets.root'
)

# definitions for producing ntuples
bgEstNtupleOutputFileNameZtautauPlusJets = cms.string('bgEstNtupleZtoElecMu_ZtautauPlusJets.root')

bgEstNtupleOutputFileNameZmumuPlusJets = cms.string('bgEstNtupleZtoElecMu_ZmumuPlusJets.root')

bgEstNtupleOutputFileNameZeePlusJets = cms.string('bgEstNtupleZtoElecMu_ZeePlusJets.root')

#--------------------------------------------------------------------------------

# definitions for producing skimmed samples
bgEstSampleOutputFileNameWplusJets = cms.untracked.string('bgEstSampleZtoElecMu_WplusJets_partXX.root')

# definitions for reading skimmed samples
bgEstSampleFileNamesWplusJets = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_WplusJets_part01.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_WplusJets_part02.root'
)

# definitions for producing ntuples
bgEstNtupleOutputFileNameWplusJets = cms.string('bgEstNtupleZtoElecMu_WplusJets.root')

#--------------------------------------------------------------------------------

# definitions for producing skimmed samples
bgEstSampleOutputFileNameInclusivePPmuX = cms.untracked.string('bgEstSampleZtoElecMu_InclusivePPmuX.root')

bgEstSampleOutputFileNamePPmuXptGt20 = cms.untracked.string('bgEstSampleZtoElecMu_PPmuXptGt20_partXX.root')

# definitions for reading skimmed samples
bgEstSampleFileNamesPPmuXptGt20_part01  = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part01.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part02.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part03.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part04.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part05.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part06.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part07.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part08.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part09.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part10.root'
)

bgEstSampleFileNamesPPmuXptGt20_part02  = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part11.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part12.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part13.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part14.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part15.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part16.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part17.root',
    #bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part18.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part19.root',
    #bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part20.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part21.root'
)

# definitions for producing ntuples
bgEstNtupleOutputFileNameInclusivePPmuX = cms.string('bgEstNtupleZtoElecMu_InclusivePPmuX.root')

bgEstNtupleOutputFileNamePPmuXptGt20 = cms.string('bgEstNtupleZtoElecMu_PPmuXptGt20_partXX.root')

#--------------------------------------------------------------------------------

# definitions for producing skimmed samples
bgEstSampleOutputFileNameTTplusJets = cms.untracked.string('bgEstSampleZtoElecMu_TTplusJets_partXX.root')

# definitions for reading skimmed samples
bgEstSampleFileNamesTTplusJets_part01  = cms.untracked.vstring(
    #bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_TTplusJets_part01.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_TTplusJets_part02.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_TTplusJets_part03.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_TTplusJets_part04.root'
    #bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_TTplusJets_part05.root'
)

bgEstSampleFileNamesTTplusJets_part02  = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_TTplusJets_part06.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_TTplusJets_part07.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_TTplusJets_part08.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_TTplusJets_part09.root'
)

# definitions for producing ntuples
bgEstNtupleOutputFileNameTTplusJets = cms.string('bgEstNtupleZtoElecMu_TTplusJets_partXX.root')

