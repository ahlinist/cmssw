import FWCore.ParameterSet.Config as cms
import copy

# define configuration parameters for submission of Z --> mu + tau-jet jobs to CERN batch system
# (running over skimmed samples stored on CASTOR)

bgEstSampleDirectoryName = cms.string("rfio:/castor/cern.ch/user/v/veelken/CMSSW_3_1_2/bgEstSkim/ZtoMuTau/10TeV/")

#--------------------------------------------------------------------------------
# definitions for producing skimmed samples
bgEstSampleOutputFileNameZtoMuTau_Ztautau_10TeV = cms.untracked.string('bgEstSampleZtoMuTau_Ztautau_10TeV_partXX.root')

# definitions for reading skimmed samples
bgEstSampleFileNamesZtoMuTau_Ztautau_10TeV_part01 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_Ztautau_10TeV_part01.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_Ztautau_10TeV_part02.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_Ztautau_10TeV_part03.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_Ztautau_10TeV_part04.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_Ztautau_10TeV_part05.root'
)

bgEstSampleFileNamesZtoMuTau_Ztautau_10TeV_part02 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_Ztautau_10TeV_part06.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_Ztautau_10TeV_part07.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_Ztautau_10TeV_part08.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_Ztautau_10TeV_part09.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_Ztautau_10TeV_part10.root'
)

bgEstSampleFileNamesZtoMuTau_Ztautau_10TeV_part03 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_Ztautau_10TeV_part11.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_Ztautau_10TeV_part12.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_Ztautau_10TeV_part13.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_Ztautau_10TeV_part14.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_Ztautau_10TeV_part15.root'
)

# definitions for producing ntuples
bgEstNtupleOutputFileNameZtoMuTau_Ztautau_10TeV = cms.string('bgEstNtupleZtoMuTau_Ztautau_10TeV_partXX.root')
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# definitions for producing skimmed samples
bgEstSampleOutputFileNameZtoMuTau_Zmumu_10TeV = cms.untracked.string('bgEstSampleZtoMuTau_Zmumu_10TeV_partXX.root')

# definitions for reading skimmed samples
bgEstSampleFileNamesZtoMuTau_Zmumu_10TeV_part01 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_Zmumu_10TeV_part01.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_Zmumu_10TeV_part02.root'
)

bgEstSampleFileNamesZtoMuTau_Zmumu_10TeV_part02 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_Zmumu_10TeV_part03.root',    
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_Zmumu_10TeV_part04.root'
)

bgEstSampleFileNamesZtoMuTau_Zmumu_10TeV_part03 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_Zmumu_10TeV_part05.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_Zmumu_10TeV_part06.root'    
)

bgEstSampleFileNamesZtoMuTau_Zmumu_10TeV_part04 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_Zmumu_10TeV_part07.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_Zmumu_10TeV_part08.root'
)

bgEstSampleFileNamesZtoMuTau_Zmumu_10TeV_part05 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_Zmumu_10TeV_part09.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_Zmumu_10TeV_part10.root'
)

bgEstSampleFileNamesZtoMuTau_Zmumu_10TeV_part06 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_Zmumu_10TeV_part11.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_Zmumu_10TeV_part12.root'    
)

bgEstSampleFileNamesZtoMuTau_Zmumu_10TeV_part07 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_Zmumu_10TeV_part13.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_Zmumu_10TeV_part14.root'
)

bgEstSampleFileNamesZtoMuTau_Zmumu_10TeV_part08 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_Zmumu_10TeV_part15.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_Zmumu_10TeV_part16.root'
)

bgEstSampleFileNamesZtoMuTau_Zmumu_10TeV_part09 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_Zmumu_10TeV_part17.root'
)

# definitions for producing ntuples
bgEstNtupleOutputFileNameZtoMuTau_Zmumu_10TeV = cms.string('bgEstNtupleZtoMuTau_Zmumu_10TeV_partXX.root')
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# definitions for producing skimmed samples
bgEstSampleOutputFileNameZtoMuTau_WplusJets_10TeV = cms.untracked.string('bgEstSampleZtoMuTau_WplusJets_10TeV_partXX.root')

# definitions for reading skimmed samples
bgEstSampleFileNamesZtoMuTau_WplusJets_10TeV_part01 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_WplusJets_10TeV_part01.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_WplusJets_10TeV_part02.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_WplusJets_10TeV_part03.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_WplusJets_10TeV_part04.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_WplusJets_10TeV_part05.root'
)

bgEstSampleFileNamesZtoMuTau_WplusJets_10TeV_part02 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_WplusJets_10TeV_part06.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_WplusJets_10TeV_part07.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_WplusJets_10TeV_part08.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_WplusJets_10TeV_part09.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_WplusJets_10TeV_part10.root'
)

bgEstSampleFileNamesZtoMuTau_WplusJets_10TeV_part03 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_WplusJets_10TeV_part11.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_WplusJets_10TeV_part12.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_WplusJets_10TeV_part13.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_WplusJets_10TeV_part14.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_WplusJets_10TeV_part15.root'
)

bgEstSampleFileNamesZtoMuTau_WplusJets_10TeV_part04 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_WplusJets_10TeV_part16.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_WplusJets_10TeV_part17.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_WplusJets_10TeV_part18.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_WplusJets_10TeV_part19.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_WplusJets_10TeV_part20.root'
)

bgEstSampleFileNamesZtoMuTau_WplusJets_10TeV_part05 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_WplusJets_10TeV_part21.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_WplusJets_10TeV_part22.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_WplusJets_10TeV_part23.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_WplusJets_10TeV_part24.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_WplusJets_10TeV_part25.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_WplusJets_10TeV_part26.root'
)

# definitions for producing ntuples
bgEstNtupleOutputFileNameZtoMuTau_WplusJets_10TeV = cms.string('bgEstNtupleZtoMuTau_WplusJets_10TeV_partXX.root')
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# definitions for producing skimmed samples
bgEstSampleOutputFileNameZtoMuTau_InclusivePPmuX_10TeV = cms.untracked.string('bgEstSampleZtoMuTau_InclusivePPmuX_10TeV_partXX.root')

bgEstSampleOutputFileNameZtoMuTau_PPmuXptGt20_10TeV = cms.untracked.string('bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_partXX.root')

# definitions for reading skimmed samples
bgEstSampleFileNamesZtoMuTau_InclusivePPmuX_10TeV_part01 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_InclusivePPmuX_10TeV_part01.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_InclusivePPmuX_10TeV_part02.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_InclusivePPmuX_10TeV_part03.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_InclusivePPmuX_10TeV_part04.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_InclusivePPmuX_10TeV_part05.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_InclusivePPmuX_10TeV_part06.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_InclusivePPmuX_10TeV_part07.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_InclusivePPmuX_10TeV_part08.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_InclusivePPmuX_10TeV_part09.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_InclusivePPmuX_10TeV_part10.root'
)

bgEstSampleFileNamesZtoMuTau_InclusivePPmuX_10TeV_part02 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_InclusivePPmuX_10TeV_part11.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_InclusivePPmuX_10TeV_part12.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_InclusivePPmuX_10TeV_part13.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_InclusivePPmuX_10TeV_part14.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_InclusivePPmuX_10TeV_part15.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_InclusivePPmuX_10TeV_part16.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_InclusivePPmuX_10TeV_part17.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_InclusivePPmuX_10TeV_part18.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_InclusivePPmuX_10TeV_part19.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_InclusivePPmuX_10TeV_part20.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_InclusivePPmuX_10TeV_part21.root'
)

bgEstSampleFileNamesZtoMuTau_PPmuXptGt20_10TeV_part01 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part01.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part02.root'
)

bgEstSampleFileNamesZtoMuTau_PPmuXptGt20_10TeV_part02 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part03.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part04.root'
)

bgEstSampleFileNamesZtoMuTau_PPmuXptGt20_10TeV_part03 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part05.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part06.root'
)

bgEstSampleFileNamesZtoMuTau_PPmuXptGt20_10TeV_part04 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part07.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part08.root'
)

bgEstSampleFileNamesZtoMuTau_PPmuXptGt20_10TeV_part05 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part09.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part10.root'
)

bgEstSampleFileNamesZtoMuTau_PPmuXptGt20_10TeV_part06 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part11.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part12.root'
)

bgEstSampleFileNamesZtoMuTau_PPmuXptGt20_10TeV_part07 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part13.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part14.root'
)

bgEstSampleFileNamesZtoMuTau_PPmuXptGt20_10TeV_part08 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part15.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part16.root'
)

bgEstSampleFileNamesZtoMuTau_PPmuXptGt20_10TeV_part09 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part17.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part18.root'
)

bgEstSampleFileNamesZtoMuTau_PPmuXptGt20_10TeV_part10 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part19.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part20.root'
)

bgEstSampleFileNamesZtoMuTau_PPmuXptGt20_10TeV_part11 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part21.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part22.root'
)

bgEstSampleFileNamesZtoMuTau_PPmuXptGt20_10TeV_part12 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part23.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part24.root'
)

bgEstSampleFileNamesZtoMuTau_PPmuXptGt20_10TeV_part13 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part25.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part26.root'
)

bgEstSampleFileNamesZtoMuTau_PPmuXptGt20_10TeV_part14 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part27.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part28.root'
)

bgEstSampleFileNamesZtoMuTau_PPmuXptGt20_10TeV_part15 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part29.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part30.root'
)

bgEstSampleFileNamesZtoMuTau_PPmuXptGt20_10TeV_part16 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part31.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part32.root'
)

bgEstSampleFileNamesZtoMuTau_PPmuXptGt20_10TeV_part17 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part33.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part34.root'
)

bgEstSampleFileNamesZtoMuTau_PPmuXptGt20_10TeV_part18 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part35.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part36.root'
)

bgEstSampleFileNamesZtoMuTau_PPmuXptGt20_10TeV_part19 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part37.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part38.root'
)

bgEstSampleFileNamesZtoMuTau_PPmuXptGt20_10TeV_part20 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part39.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part40.root'
)

bgEstSampleFileNamesZtoMuTau_PPmuXptGt20_10TeV_part21 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part41.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part42.root'
)

bgEstSampleFileNamesZtoMuTau_PPmuXptGt20_10TeV_part22 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part43.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part44.root'
)

bgEstSampleFileNamesZtoMuTau_PPmuXptGt20_10TeV_part23 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part45.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part46.root'
)

bgEstSampleFileNamesZtoMuTau_PPmuXptGt20_10TeV_part24 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part47.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part48.root'
)

bgEstSampleFileNamesZtoMuTau_PPmuXptGt20_10TeV_part25 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part49.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part50.root'
)

bgEstSampleFileNamesZtoMuTau_PPmuXptGt20_10TeV_part26 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part51.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part52.root'
)

bgEstSampleFileNamesZtoMuTau_PPmuXptGt20_10TeV_part27 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part53.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part54.root'
)

bgEstSampleFileNamesZtoMuTau_PPmuXptGt20_10TeV_part28 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part55.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part56.root'
)

bgEstSampleFileNamesZtoMuTau_PPmuXptGt20_10TeV_part29 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part57.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part58.root'
)

bgEstSampleFileNamesZtoMuTau_PPmuXptGt20_10TeV_part30 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part59.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part60.root'
)

bgEstSampleFileNamesZtoMuTau_PPmuXptGt20_10TeV_part31 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part61.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part62.root'
)

bgEstSampleFileNamesZtoMuTau_PPmuXptGt20_10TeV_part32 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part63.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part64.root'
)

bgEstSampleFileNamesZtoMuTau_PPmuXptGt20_10TeV_part33 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part65.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part66.root'
)

bgEstSampleFileNamesZtoMuTau_PPmuXptGt20_10TeV_part34 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part67.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_PPmuXptGt20_10TeV_part68.root'
)

# definitions for producing ntuples
bgEstNtupleOutputFileNameZtoMuTau_InclusivePPmuX_10TeV = cms.string('bgEstNtupleZtoMuTau_InclusivePPmuX_10TeV_partXX.root')

bgEstNtupleOutputFileNameZtoMuTau_PPmuXptGt20_10TeV = cms.string('bgEstNtupleZtoMuTau_PPmuXptGt20_10TeV_partXX.root')
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# definitions for producing skimmed samples
bgEstSampleOutputFileNameZtoMuTau_TTplusJets_10TeV = cms.untracked.string('bgEstSampleZtoMuTau_TTplusJets_10TeV_partXX.root')

# definitions for reading skimmed samples
bgEstSampleFileNamesZtoMuTau_TTplusJets_10TeV_part01 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_TTplusJets_10TeV_part01.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_TTplusJets_10TeV_part02.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_TTplusJets_10TeV_part03.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_TTplusJets_10TeV_part04.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_TTplusJets_10TeV_part05.root'
)

bgEstSampleFileNamesZtoMuTau_TTplusJets_10TeV_part02 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_TTplusJets_10TeV_part06.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_TTplusJets_10TeV_part07.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_TTplusJets_10TeV_part08.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_TTplusJets_10TeV_part09.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_TTplusJets_10TeV_part10.root'
)

bgEstSampleFileNamesZtoMuTau_TTplusJets_10TeV_part03 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_TTplusJets_10TeV_part11.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_TTplusJets_10TeV_part12.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_TTplusJets_10TeV_part13.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_TTplusJets_10TeV_part14.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_TTplusJets_10TeV_part15.root'
)

bgEstSampleFileNamesZtoMuTau_TTplusJets_10TeV_part04 = cms.untracked.vstring(
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_TTplusJets_10TeV_part16.root',
    bgEstSampleDirectoryName.value() + 'bgEstSampleZtoMuTau_TTplusJets_10TeV_part17.root'
)

# definitions for producing ntuples
bgEstNtupleOutputFileNameZtoMuTau_TTplusJets_10TeV = cms.string('bgEstNtupleZtoMuTau_TTplusJets_10TeV_partXX.root')
#--------------------------------------------------------------------------------
