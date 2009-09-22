import FWCore.ParameterSet.Config as cms
import copy

# define configuration parameters for submission of Z --> mu + tau-jet jobs to CERN batch system
# (running over skimmed samples stored on CASTOR)

bgEstSampleDirectoryName = cms.string("rfio:/castor/cern.ch/user/s/sunil/bgEstSkim/ZtoElecMu/")

# definitions for producing skimmed samples
bgEstSampleOutputFileNameZtautau = cms.untracked.string('bgEstSampleZtoElecMu_Ztautau_partXX.root')

# definitions for reading skimmed samples
bgEstSampleFileNamesZtautau = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_Ztautau_part01.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_Ztautau_part02.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_Ztautau_part03.root'
)

# definitions for producing ntuples
bgEstNtupleOutputFileNameZtautau = cms.string('bgEstNtupleZtoElecMu_Ztautau.root')

#--------------------------------------------------------------------------------

# definitions for producing skimmed samples
bgEstSampleOutputFileNameZmumu = cms.untracked.string('bgEstSampleZtoElecMu_Zmumu_partXX.root')

# definitions for reading skimmed samples
bgEstSampleFileNamesZmumu = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_Zmumu_part01.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_Zmumu_part02.root'
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
bgEstSampleOutputFileNameZtautauPlusJets = cms.untracked.string('bgEstSampleZtoElecMu_ZtautauPlusJets_partXX.root')

bgEstSampleOutputFileNameZmumuPlusJets = cms.untracked.string('bgEstSampleZtoElecMu_ZmumuPlusJets_partXX.root')

bgEstSampleOutputFileNameZeePlusJets = cms.untracked.string('bgEstSampleZtoElecMu_ZeePlusJets_partXX.root')

# definitions for reading skimmed samples
bgEstSampleFileNamesZtautauPlusJets = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_ZtautauPlusJets_part01.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_ZtautauPlusJets_part02.root'
)

bgEstSampleFileNamesZmumuPlusJets = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_ZmumuPlusJets_part01.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_ZmumuPlusJets_part02.root'
)

bgEstSampleFileNamesZeePlusJets = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_ZeePlusJets_part01.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_ZeePlusJets_part02.root'
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
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_WplusJets_part02.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_WplusJets_part03.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_WplusJets_part04.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_WplusJets_part05.root'

)

# definitions for producing ntuples
bgEstNtupleOutputFileNameWplusJets = cms.string('bgEstNtupleZtoElecMu_WplusJets.root')

#--------------------------------------------------------------------------------

# definitions for producing skimmed samples
#bgEstSampleOutputFileNameInclusivePPmuX = cms.untracked.string('bgEstSampleZtoElecMu_InclusivePPmuX.root')

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
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part10.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part11.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part12.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part13.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part14.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part15.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part16.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part17.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part18.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part19.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part20.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part21.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part22.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part23.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part24.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part25.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part26.root'

)

bgEstSampleFileNamesPPmuXptGt20_part02  = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part27.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part28.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part29.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part30.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part31.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part32.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part33.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part34.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part35.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part36.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part37.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part38.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part39.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part40.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part41.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part42.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part43.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part44.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part45.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part46.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part47.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part48.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part49.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part50.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part51.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_PPmuXptGt20_part52.root'

)

# definitions for producing ntuples
#bgEstNtupleOutputFileNameInclusivePPmuX = cms.string('bgEstNtupleZtoElecMu_InclusivePPmuX.root')

bgEstNtupleOutputFileNamePPmuXptGt20 = cms.string('bgEstNtupleZtoElecMu_PPmuXptGt20_partXX.root')

#--------------------------------------------------------------------------------

# definitions for producing skimmed samples
bgEstSampleOutputFileNameTTplusJets = cms.untracked.string('bgEstSampleZtoElecMu_TTplusJets_partXX.root')

# definitions for reading skimmed samples
bgEstSampleFileNamesTTplusJets_part01  = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_TTplusJets_part01.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_TTplusJets_part02.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_TTplusJets_part03.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_TTplusJets_part04.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_TTplusJets_part05.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_TTplusJets_part06.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_TTplusJets_part07.root' 
)

bgEstSampleFileNamesTTplusJets_part02  = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_TTplusJets_part08.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_TTplusJets_part09.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_TTplusJets_part10.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_TTplusJets_part11.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_TTplusJets_part12.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoElecMu_TTplusJets_part13.root'
)

# definitions for producing ntuples
bgEstNtupleOutputFileNameTTplusJets = cms.string('bgEstNtupleZtoElecMu_TTplusJets_partXX.root')

