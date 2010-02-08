import FWCore.ParameterSet.Config as cms
import copy

# define configuration parameters for submission of Z --> e + mu jobs to CERN batch system
# (running over skimmed samples stored on CASTOR)

intLumiZtoElecMu_Data_7TeV = float(200.)

#--------------------------------------------------------------------------------
# Z --> tau+ tau- sample
#
intLumiZtoElecMu_Ztautau_7TeV = float(1686.9)
corrFactorZtoElecMu_Ztautau_7TeV = float(1.)

fileNamesZtoElecMu_Ztautau_7TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_7TeV/elecMuSkim_2.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_7TeV/elecMuSkim_3.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_7TeV/elecMuSkim_5.root'
)

fileNamesZtoElecMu_Ztautau_7TeV_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_7TeV/elecMuSkim_6.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_7TeV/elecMuSkim_7.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_7TeV/elecMuSkim_8.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_7TeV/elecMuSkim_9.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_7TeV/elecMuSkim_10.root'
)

fileNamesZtoElecMu_Ztautau_7TeV_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_7TeV/elecMuSkim_11.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_7TeV/elecMuSkim_12.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_7TeV/elecMuSkim_13.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_7TeV/elecMuSkim_14.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_7TeV/elecMuSkim_15.root'
)

fileNamesZtoElecMu_Ztautau_7TeV_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_7TeV/elecMuSkim_16.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_7TeV/elecMuSkim_17.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_7TeV/elecMuSkim_18.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_7TeV/elecMuSkim_19.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_7TeV/elecMuSkim_20.root'
)

fileNamesZtoElecMu_Ztautau_7TeV_part05 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_7TeV/elecMuSkim_21.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_7TeV/elecMuSkim_22.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_7TeV/elecMuSkim_23.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_7TeV/elecMuSkim_25.root'
)

fileNamesZtoElecMu_Ztautau_7TeV_part06 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_7TeV/elecMuSkim_26.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_7TeV/elecMuSkim_27.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_7TeV/elecMuSkim_28.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_7TeV/elecMuSkim_29.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/Z2tautau_7TeV/elecMuSkim_30.root'
)

genPhaseSpaceCutZtoElecMu_Ztautau_7TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

plotsOutputFileNameZtoElecMu_Ztautau_7TeV = cms.string('plotsZtoElecMu_Ztautau_7TeV_partXX.root')
patTupleOutputFileNameZtoElecMu_Ztautau_7TeV = cms.string('patTupleZtoElecMu_Ztautau_7TeV_partXX.root')
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# Z + Jets sample

intLumiZtoElecMu_ZplusJets_7TeV = float(454.8)
corrFactorZtoElecMu_ZplusJets_7TeV = float(1.)

fileNamesZtoElecMu_ZplusJets_7TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/7TeV/ZJets-madgraph/akalinow-SkimElMu_7TeV_314_pass1/SkimElMu_Merged_1.root'
)

fileNamesZtoElecMu_ZplusJets_7TeV_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/7TeV/ZJets-madgraph/akalinow-SkimElMu_7TeV_314_pass1/SkimElMu_Merged_2.root'
)

intLumiZtoElecMu_ZeePlusJets_7TeV = intLumiZtoElecMu_ZplusJets_7TeV
corrFactorZtoElecMu_ZeePlusJets_7TeV = corrFactorZtoElecMu_ZplusJets_7TeV
fileNamesZtoElecMu_ZeePlusJets_7TeV_part01 = fileNamesZtoElecMu_ZplusJets_7TeV_part01
fileNamesZtoElecMu_ZeePlusJets_7TeV_part02 = fileNamesZtoElecMu_ZplusJets_7TeV_part02

patTupleOutputFileNameZtoElecMu_ZeePlusJets_7TeV = cms.untracked.string('patTupleZtoElecMu_ZeePlusJets_7TeV_partXX.root')

plotsOutputFileNameZtoElecMu_ZeePlusJets_7TeV = cms.string('plotsZtoElecMu_ZeePlusJets_7TeV_partXX.root')

genPhaseSpaceCutZtoElecMu_ZeePlusJets_7TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('genElectronsFromZs'),
    minNumber = cms.uint32(2)
)

intLumiZtoElecMu_ZmumuPlusJets_7TeV = intLumiZtoElecMu_ZplusJets_7TeV
corrFactorZtoElecMu_ZmumuPlusJets_7TeV = corrFactorZtoElecMu_ZplusJets_7TeV
fileNamesZtoElecMu_ZmumuPlusJets_7TeV_part01 = fileNamesZtoElecMu_ZplusJets_7TeV_part01
fileNamesZtoElecMu_ZmumuPlusJets_7TeV_part02 = fileNamesZtoElecMu_ZplusJets_7TeV_part02

patTupleOutputFileNameZtoElecMu_ZmumuPlusJets_7TeV = cms.untracked.string('patTupleZtoElecMu_ZmumuPlusJets_7TeV_partXX.root')

plotsOutputFileNameZtoElecMu_ZmumuPlusJets_7TeV = cms.string('plotsZtoElecMu_ZmumuPlusJets_7TeV_partXX.root')

genPhaseSpaceCutZtoElecMu_ZmumuPlusJets_7TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('genMuonsFromZs'),
    minNumber = cms.uint32(2)
)

intLumiZtoElecMu_ZtautauPlusJets_7TeV = intLumiZtoElecMu_ZplusJets_7TeV
corrFactorZtoElecMu_ZtautauPlusJets_7TeV = corrFactorZtoElecMu_ZplusJets_7TeV
fileNamesZtoElecMu_ZtautauPlusJets_7TeV_part01 = fileNamesZtoElecMu_ZplusJets_7TeV_part01
fileNamesZtoElecMu_ZtautauPlusJets_7TeV_part02 = fileNamesZtoElecMu_ZplusJets_7TeV_part02

patTupleOutputFileNameZtoElecMu_ZtautauPlusJets_7TeV = cms.untracked.string('patTupleZtoElecMu_ZtautauPlusJets_7TeV_partXX.root')

plotsOutputFileNameZtoElecMu_ZtautauPlusJets_7TeV = cms.string('plotsZtoElecMu_ZtautauPlusJets_7TeV_partXX.root')

genPhaseSpaceCutZtoElecMu_ZtautauPlusJets_7TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('genTausFromZs'),
    minNumber = cms.uint32(2)
)
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# W + jets sample
#
intLumiZtoElecMu_WplusJets_7TeV = float(638.1)
corrFactorZtoElecMu_WplusJets_7TeV = float(1.)

fileNamesZtoElecMu_WplusJets_7TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/7TeV/WJets-madgraph/akalinow-SkimElMu_7TeV_314_pass1/SkimElMu_Merged_1.root'
)

fileNamesZtoElecMu_WplusJets_7TeV_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/7TeV/WJets-madgraph/akalinow-SkimElMu_7TeV_314_pass1/SkimElMu_Merged_2.root'
)

fileNamesZtoElecMu_WplusJets_7TeV_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/7TeV/WJets-madgraph/akalinow-SkimElMu_7TeV_314_pass1/SkimElMu_Merged_3.root'
)

fileNamesZtoElecMu_WplusJets_7TeV_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/7TeV/WJets-madgraph/akalinow-SkimElMu_7TeV_314_pass1/SkimElMu_Merged_4.root'
)

fileNamesZtoElecMu_WplusJets_7TeV_part05 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/7TeV/WJets-madgraph/akalinow-SkimElMu_7TeV_314_pass1/SkimElMu_Merged_5.root'
)

fileNamesZtoElecMu_WplusJets_7TeV_part06 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/7TeV/WJets-madgraph/akalinow-SkimElMu_7TeV_314_pass1/SkimElMu_Merged_6.root'
)

genPhaseSpaceCutZtoElecMu_WplusJets_7TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)   
    
plotsOutputFileNameZtoElecMu_WplusJets_7TeV = cms.string('plotsZtoElecMu_WplusJets_7TeV_partXX.root')
patTupleOutputFileNameZtoElecMu_WplusJets_7TeV = cms.string('patTupleZtoElecMu_WplusJets_7TeV_partXX.root')
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# ttbar + jets sample

intLumiZtoElecMu_TTplusJets_7TeV = float(6644.9)
corrFactorZtoElecMu_TTplusJets_7TeV = float(1.)

fileNamesZtoElecMu_TTplusJets_7TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_1.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_2.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_3.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_4.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_5.root'
)

fileNamesZtoElecMu_TTplusJets_7TeV_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_6.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_7.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_8.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_9.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_10.root'
)

fileNamesZtoElecMu_TTplusJets_7TeV_part03 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_11.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_12.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_13.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_14.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_15.root'
)

fileNamesZtoElecMu_TTplusJets_7TeV_part04 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_16.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_17.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_18.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_19.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_20.root'
)

fileNamesZtoElecMu_TTplusJets_7TeV_part05 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_21.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_22.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_23.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_24.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_25.root'
)

fileNamesZtoElecMu_TTplusJets_7TeV_part06 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_26.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_27.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_28.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_29.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_30.root'
)

fileNamesZtoElecMu_TTplusJets_7TeV_part07 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_31.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_32.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_33.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_34.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_35.root'
)

fileNamesZtoElecMu_TTplusJets_7TeV_part08 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_36.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_37.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_38.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_39.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_40.root'
)

fileNamesZtoElecMu_TTplusJets_7TeV_part09 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_41.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_42.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_43.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_44.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_45.root'
)

fileNamesZtoElecMu_TTplusJets_7TeV_part10 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_46.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_47.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_48.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_49.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_50.root'
)

fileNamesZtoElecMu_TTplusJets_7TeV_part11 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_51.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_52.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_53.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_54.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_55.root'
)

fileNamesZtoElecMu_TTplusJets_7TeV_part12 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_56.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_57.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_58.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_59.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_60.root'
)

fileNamesZtoElecMu_TTplusJets_7TeV_part13 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_61.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_62.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_63.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_64.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_65.root'
)

fileNamesZtoElecMu_TTplusJets_7TeV_part14 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_66.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_67.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_68.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_69.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_70.root'
)

fileNamesZtoElecMu_TTplusJets_7TeV_part15 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_71.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_72.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_73.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_74.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_75.root'
)

fileNamesZtoElecMu_TTplusJets_7TeV_part16 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_76.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_77.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_78.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_79.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_80.root'
)

fileNamesZtoElecMu_TTplusJets_7TeV_part17 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_81.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_82.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_83.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_84.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_85.root'
)

fileNamesZtoElecMu_TTplusJets_7TeV_part18 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_86.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_87.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_88.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_89.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_90.root'
)

fileNamesZtoElecMu_TTplusJets_7TeV_part19 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_91.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_92.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_93.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_94.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_95.root'
)

fileNamesZtoElecMu_TTplusJets_7TeV_part20 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_96.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_97.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/TTbar_7TeV/elecMuSkim_98.root'
)

genPhaseSpaceCutZtoElecMu_TTplusJets_7TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

plotsOutputFileNameZtoElecMu_TTplusJets_7TeV = cms.string('plotsZtoElecMu_TTplusJets_7TeV_partXX.root')
patTupleOutputFileNameZtoElecMu_TTplusJets_7TeV = cms.string('patTupleZtoElecMu_TTplusJets_7TeV_partXX.root')
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# muon enriched QCD sample generated with Pythia (no cut on Pt(hat))

intLumiZtoElecMu_InclusivePPmuX_7TeV = float(0.183)
corrFactorZtoElecMu_InclusivePPmuX_7TeV = float(1.)

fileNamesZtoElecMu_InclusivePPmuX_7TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_7TeV/elecMuSkim_1.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_7TeV/elecMuSkim_2.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_7TeV/elecMuSkim_3.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_7TeV/elecMuSkim_4.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_7TeV/elecMuSkim_5.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_7TeV/elecMuSkim_6.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_7TeV/elecMuSkim_9.root ',   
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_7TeV/elecMuSkim_10.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_7TeV/elecMuSkim_11.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_7TeV/elecMuSkim_12.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_7TeV/elecMuSkim_13.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_7TeV/elecMuSkim_15.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_7TeV/elecMuSkim_16.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_7TeV/elecMuSkim_17.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_7TeV/elecMuSkim_18.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_7TeV/elecMuSkim_19.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_7TeV/elecMuSkim_20.root'
)

fileNamesZtoElecMu_InclusivePPmuX_7TeV_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_7TeV/elecMuSkim_21.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_7TeV/elecMuSkim_22.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_7TeV/elecMuSkim_23.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_7TeV/elecMuSkim_24.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_7TeV/elecMuSkim_25.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_7TeV/elecMuSkim_26.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_7TeV/elecMuSkim_27.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_7TeV/elecMuSkim_28.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_7TeV/elecMuSkim_29.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_7TeV/elecMuSkim_30.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_7TeV/elecMuSkim_31.root',
    'rfio:/castor/cern.ch/user/s/sunil/SkimNov09/ppMuX_7TeV/elecMuSkim_32.root'
)

genPhaseSpaceCutZtoElecMu_InclusivePPmuX_7TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('ptHat < 20. | leadingGenMuon.pt < 15.')
)

plotsOutputFileNameZtoElecMu_InclusivePPmuX_7TeV = cms.string('plotsZtoElecMu_InclusivePPmuX_7TeV_partXX.root')
patTupleOutputFileNameZtoElecMu_InclusivePPmuX_7TeV = cms.untracked.string('patTupleZtoElecMu_InclusivePPmuX_7TeV_partXX.root')
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# muon enriched QCD sample (Pt(hat) > 20 GeV && muon with Pt > 15 GeV)
#
# NOTE: the submission of the PPmuXptGt20 sample is split into four parts,
#       in order to reduce the execution time of individual cmsRun jobs
#       (and also because the length of vstrings is limited to 255 entries)
#
intLumiZtoElecMu_PPmuXptGt20_7TeV = float(48.7)
corrFactorZtoElecMu_PPmuXptGt20_7TeV = float(1.)

fileNamesZtoElecMu_PPmuXptGt20_7TeV_part01 = cms.untracked.vstring(
    # CV: not yet skimmed (as of 2010/02/07)
)

genPhaseSpaceCutZtoElecMu_PPmuXptGt20_7TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

plotsOutputFileNameZtoElecMu_PPmuXptGt20_7TeV = cms.string('plotsZtoElecMu_PPmuXptGt20_7TeV_partXX.root')
patTupleOutputFileNameZtoElecMu_PPmuXptGt20_7TeV = cms.string('patTupleZtoElecMu_PPmuXptGt20_7TeV_partXX.root')
#--------------------------------------------------------------------------------
