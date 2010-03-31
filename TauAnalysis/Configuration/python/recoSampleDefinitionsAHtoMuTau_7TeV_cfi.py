import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.Configuration.recoSampleDefinitionsZtoMuTau_7TeV_cfi import *

# define configuration parameters for submission of MSSM Higgs A/H --> mu + tau-jet jobs to CERN batch system
# (running over skimmed samples stored on CASTOR)

intLumiAHtoMuTau_Data_7TeV = copy.deepcopy(intLumiZtoMuTau_Data_7TeV)

#--------------------------------------------------------------------------------
# Z --> tau+ tau- sample generated with Pythia + Tauola (all decay modes)
#  integrated luminosity = 1686.9 pb^-1
# (corrected by scale factor of 1. for missing files)
#
intLumiAHtoMuTau_Ztautau_7TeV = copy.deepcopy(intLumiZtoMuTau_Ztautau_7TeV)
corrFactorAHtoMuTau_Ztautau_7TeV = copy.deepcopy(corrFactorZtoMuTau_Ztautau_7TeV)

fileNamesAHtoMuTau_Ztautau_7TeV_part01 = copy.deepcopy(fileNamesZtoMuTau_Ztautau_7TeV_part01)
fileNamesAHtoMuTau_Ztautau_7TeV_part02 = copy.deepcopy(fileNamesZtoMuTau_Ztautau_7TeV_part02)
fileNamesAHtoMuTau_Ztautau_7TeV_part03 = copy.deepcopy(fileNamesZtoMuTau_Ztautau_7TeV_part03)
fileNamesAHtoMuTau_Ztautau_7TeV_part04 = copy.deepcopy(fileNamesZtoMuTau_Ztautau_7TeV_part04)
fileNamesAHtoMuTau_Ztautau_7TeV_part05 = copy.deepcopy(fileNamesZtoMuTau_Ztautau_7TeV_part05)
fileNamesAHtoMuTau_Ztautau_7TeV_part06 = copy.deepcopy(fileNamesZtoMuTau_Ztautau_7TeV_part06)
fileNamesAHtoMuTau_Ztautau_7TeV_part07 = copy.deepcopy(fileNamesZtoMuTau_Ztautau_7TeV_part07)
fileNamesAHtoMuTau_Ztautau_7TeV_part08 = copy.deepcopy(fileNamesZtoMuTau_Ztautau_7TeV_part08)
fileNamesAHtoMuTau_Ztautau_7TeV_part09 = copy.deepcopy(fileNamesZtoMuTau_Ztautau_7TeV_part09)
fileNamesAHtoMuTau_Ztautau_7TeV_part10 = copy.deepcopy(fileNamesZtoMuTau_Ztautau_7TeV_part10)
fileNamesAHtoMuTau_Ztautau_7TeV_part11 = copy.deepcopy(fileNamesZtoMuTau_Ztautau_7TeV_part11)
fileNamesAHtoMuTau_Ztautau_7TeV_part12 = copy.deepcopy(fileNamesZtoMuTau_Ztautau_7TeV_part12)
fileNamesAHtoMuTau_Ztautau_7TeV_part13 = copy.deepcopy(fileNamesZtoMuTau_Ztautau_7TeV_part13)
fileNamesAHtoMuTau_Ztautau_7TeV_part14 = copy.deepcopy(fileNamesZtoMuTau_Ztautau_7TeV_part14)
fileNamesAHtoMuTau_Ztautau_7TeV_part15 = copy.deepcopy(fileNamesZtoMuTau_Ztautau_7TeV_part15)
fileNamesAHtoMuTau_Ztautau_7TeV_part16 = copy.deepcopy(fileNamesZtoMuTau_Ztautau_7TeV_part16)

genPhaseSpaceCutAHtoMuTau_Ztautau_7TeV = copy.deepcopy(genPhaseSpaceCutZtoMuTau_Ztautau_7TeV)

plotsOutputFileNameAHtoMuTau_Ztautau_7TeV = cms.string('plotsAHtoMuTau_Ztautau_7TeV_partXX.root')
patTupleOutputFileNameAHtoMuTau_Ztautau_7TeV = cms.untracked.string('patTupleAHtoMuTau_Ztautau_7TeV_partXX.root')
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# Z --> mu+ mu- sample generated with Pythia
#  integrated luminosity = 1604.1 pb^-1
# (corrected by scale factor of 1. for missing files)
#
intLumiAHtoMuTau_Zmumu_7TeV = copy.deepcopy(intLumiZtoMuTau_Zmumu_7TeV)
corrFactorAHtoMuTau_Zmumu_7TeV = copy.deepcopy(corrFactorZtoMuTau_Zmumu_7TeV)

fileNamesAHtoMuTau_Zmumu_7TeV_part01 = copy.deepcopy(fileNamesZtoMuTau_Zmumu_7TeV_part01)
fileNamesAHtoMuTau_Zmumu_7TeV_part02 = copy.deepcopy(fileNamesZtoMuTau_Zmumu_7TeV_part02)
fileNamesAHtoMuTau_Zmumu_7TeV_part03 = copy.deepcopy(fileNamesZtoMuTau_Zmumu_7TeV_part03)
fileNamesAHtoMuTau_Zmumu_7TeV_part04 = copy.deepcopy(fileNamesZtoMuTau_Zmumu_7TeV_part04)
fileNamesAHtoMuTau_Zmumu_7TeV_part05 = copy.deepcopy(fileNamesZtoMuTau_Zmumu_7TeV_part05)
fileNamesAHtoMuTau_Zmumu_7TeV_part06 = copy.deepcopy(fileNamesZtoMuTau_Zmumu_7TeV_part06)
fileNamesAHtoMuTau_Zmumu_7TeV_part07 = copy.deepcopy(fileNamesZtoMuTau_Zmumu_7TeV_part07)
fileNamesAHtoMuTau_Zmumu_7TeV_part08 = copy.deepcopy(fileNamesZtoMuTau_Zmumu_7TeV_part08)
fileNamesAHtoMuTau_Zmumu_7TeV_part09 = copy.deepcopy(fileNamesZtoMuTau_Zmumu_7TeV_part09)
fileNamesAHtoMuTau_Zmumu_7TeV_part10 = copy.deepcopy(fileNamesZtoMuTau_Zmumu_7TeV_part10)
fileNamesAHtoMuTau_Zmumu_7TeV_part11 = copy.deepcopy(fileNamesZtoMuTau_Zmumu_7TeV_part11)
fileNamesAHtoMuTau_Zmumu_7TeV_part12 = copy.deepcopy(fileNamesZtoMuTau_Zmumu_7TeV_part12)
fileNamesAHtoMuTau_Zmumu_7TeV_part13 = copy.deepcopy(fileNamesZtoMuTau_Zmumu_7TeV_part13)
fileNamesAHtoMuTau_Zmumu_7TeV_part14 = copy.deepcopy(fileNamesZtoMuTau_Zmumu_7TeV_part14)
fileNamesAHtoMuTau_Zmumu_7TeV_part15 = copy.deepcopy(fileNamesZtoMuTau_Zmumu_7TeV_part15)

genPhaseSpaceCutAHtoMuTau_Zmumu_7TeV = copy.deepcopy(genPhaseSpaceCutZtoMuTau_Zmumu_7TeV)

plotsOutputFileNameAHtoMuTau_Zmumu_7TeV = cms.string('plotsAHtoMuTau_Zmumu_7TeV_partXX.root')
patTupleOutputFileNameAHtoMuTau_Zmumu_7TeV = cms.untracked.string('patTupleAHtoMuTau_Zmumu_7TeV_partXX.root')
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# Z + jets sample generated with Madgraph
#  integrated luminosity = 454.8 pb^-1
# (to be corrected for missing files)
#
# (NOTE: for Monte Carlo samples generated by Madgraph,
#        the filter efficiency is already included in the cross-sections
#        listed at https://twiki.cern.ch/twiki/bin/view/CMS/ProductionSummer2009at7TeV#MadGraph !!)
#
intLumiAHtoMuTau_ZplusJets_7TeV = float(454.8)
corrFactorAHtoMuTau_ZplusJets_7TeV = float(1.)

fileNamesAHtoMuTau_ZplusJets_7TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_0.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_1.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_2.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_3.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_4.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_5.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_6.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_7.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_8.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_9.root'
)

fileNamesAHtoMuTau_ZplusJets_7TeV_part02 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_10.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_11.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_12.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_13.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_14.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_15.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_16.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_17.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_18.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_19.root'
)

fileNamesAHtoMuTau_ZplusJets_7TeV_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_20.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_21.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_22.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_23.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_24.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_25.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_26.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_27.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_28.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_29.root'
)

fileNamesAHtoMuTau_ZplusJets_7TeV_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_30.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_31.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_32.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_33.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_34.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_35.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_36.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_37.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_38.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_39.root'
)

fileNamesAHtoMuTau_ZplusJets_7TeV_part05 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_40.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_41.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_42.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_43.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_44.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_45.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_46.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_47.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_48.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_49.root'
)

fileNamesAHtoMuTau_ZplusJets_7TeV_part06 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_50.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_51.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_52.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_53.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_54.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_55.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_56.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_57.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_58.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_59.root'
)

fileNamesAHtoMuTau_ZplusJets_7TeV_part07 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_60.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_61.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_62.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_63.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_64.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_65.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_66.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_67.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_68.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_69.root'
)

fileNamesAHtoMuTau_ZplusJets_7TeV_part08 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_70.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_71.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_72.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_73.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_74.root'
)

genPhaseSpaceCutAHtoMuTau_ZplusJets_7TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

plotsOutputFileNameAHtoMuTau_ZplusJets_7TeV = cms.string('plotsAHtoMuTau_ZplusJets_7TeV_partXX.root')
patTupleOutputFileNameAHtoMuTau_ZplusJets_7TeV = cms.untracked.string('patTupleAHtoMuTau_ZplusJets_7TeV_partXX.root')
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# W + jets sample generated with Madgraph
#  integrated luminosity = 638.1 pb^-1
# (corrected by scale factor of 1. for missing files)
#
# (NOTE: for Monte Carlo samples generated by Madgraph,
#        the filter efficiency is already included in the cross-sections
#        listed at https://twiki.cern.ch/twiki/bin/view/CMS/ProductionSummer2009at7TeV#MadGraph !!)
#
intLumiAHtoMuTau_WplusJets_7TeV = copy.deepcopy(intLumiZtoMuTau_WplusJets_7TeV)
corrFactorAHtoMuTau_WplusJets_7TeV = copy.deepcopy(corrFactorZtoMuTau_WplusJets_7TeV)

fileNamesAHtoMuTau_WplusJets_7TeV_part01 = copy.deepcopy(fileNamesZtoMuTau_WplusJets_7TeV_part01)
fileNamesAHtoMuTau_WplusJets_7TeV_part02 = copy.deepcopy(fileNamesZtoMuTau_WplusJets_7TeV_part02)
fileNamesAHtoMuTau_WplusJets_7TeV_part03 = copy.deepcopy(fileNamesZtoMuTau_WplusJets_7TeV_part03)
fileNamesAHtoMuTau_WplusJets_7TeV_part04 = copy.deepcopy(fileNamesZtoMuTau_WplusJets_7TeV_part04)
fileNamesAHtoMuTau_WplusJets_7TeV_part05 = copy.deepcopy(fileNamesZtoMuTau_WplusJets_7TeV_part05)
fileNamesAHtoMuTau_WplusJets_7TeV_part06 = copy.deepcopy(fileNamesZtoMuTau_WplusJets_7TeV_part06)
fileNamesAHtoMuTau_WplusJets_7TeV_part07 = copy.deepcopy(fileNamesZtoMuTau_WplusJets_7TeV_part07)
fileNamesAHtoMuTau_WplusJets_7TeV_part08 = copy.deepcopy(fileNamesZtoMuTau_WplusJets_7TeV_part08)
fileNamesAHtoMuTau_WplusJets_7TeV_part09 = copy.deepcopy(fileNamesZtoMuTau_WplusJets_7TeV_part09)
fileNamesAHtoMuTau_WplusJets_7TeV_part10 = copy.deepcopy(fileNamesZtoMuTau_WplusJets_7TeV_part10)
fileNamesAHtoMuTau_WplusJets_7TeV_part11 = copy.deepcopy(fileNamesZtoMuTau_WplusJets_7TeV_part11)
fileNamesAHtoMuTau_WplusJets_7TeV_part12 = copy.deepcopy(fileNamesZtoMuTau_WplusJets_7TeV_part12)
fileNamesAHtoMuTau_WplusJets_7TeV_part13 = copy.deepcopy(fileNamesZtoMuTau_WplusJets_7TeV_part13)
fileNamesAHtoMuTau_WplusJets_7TeV_part14 = copy.deepcopy(fileNamesZtoMuTau_WplusJets_7TeV_part14)
fileNamesAHtoMuTau_WplusJets_7TeV_part15 = copy.deepcopy(fileNamesZtoMuTau_WplusJets_7TeV_part15)
fileNamesAHtoMuTau_WplusJets_7TeV_part16 = copy.deepcopy(fileNamesZtoMuTau_WplusJets_7TeV_part16)
fileNamesAHtoMuTau_WplusJets_7TeV_part17 = copy.deepcopy(fileNamesZtoMuTau_WplusJets_7TeV_part17)
fileNamesAHtoMuTau_WplusJets_7TeV_part18 = copy.deepcopy(fileNamesZtoMuTau_WplusJets_7TeV_part18)
fileNamesAHtoMuTau_WplusJets_7TeV_part19 = copy.deepcopy(fileNamesZtoMuTau_WplusJets_7TeV_part19)
fileNamesAHtoMuTau_WplusJets_7TeV_part20 = copy.deepcopy(fileNamesZtoMuTau_WplusJets_7TeV_part20)
fileNamesAHtoMuTau_WplusJets_7TeV_part21 = copy.deepcopy(fileNamesZtoMuTau_WplusJets_7TeV_part21)
fileNamesAHtoMuTau_WplusJets_7TeV_part22 = copy.deepcopy(fileNamesZtoMuTau_WplusJets_7TeV_part22)
fileNamesAHtoMuTau_WplusJets_7TeV_part23 = copy.deepcopy(fileNamesZtoMuTau_WplusJets_7TeV_part23)
fileNamesAHtoMuTau_WplusJets_7TeV_part24 = copy.deepcopy(fileNamesZtoMuTau_WplusJets_7TeV_part24)
fileNamesAHtoMuTau_WplusJets_7TeV_part25 = copy.deepcopy(fileNamesZtoMuTau_WplusJets_7TeV_part25)
fileNamesAHtoMuTau_WplusJets_7TeV_part26 = copy.deepcopy(fileNamesZtoMuTau_WplusJets_7TeV_part26)
fileNamesAHtoMuTau_WplusJets_7TeV_part27 = copy.deepcopy(fileNamesZtoMuTau_WplusJets_7TeV_part27)
fileNamesAHtoMuTau_WplusJets_7TeV_part28 = copy.deepcopy(fileNamesZtoMuTau_WplusJets_7TeV_part28)
fileNamesAHtoMuTau_WplusJets_7TeV_part29 = copy.deepcopy(fileNamesZtoMuTau_WplusJets_7TeV_part29)
fileNamesAHtoMuTau_WplusJets_7TeV_part30 = copy.deepcopy(fileNamesZtoMuTau_WplusJets_7TeV_part30)
fileNamesAHtoMuTau_WplusJets_7TeV_part31 = copy.deepcopy(fileNamesZtoMuTau_WplusJets_7TeV_part31)

genPhaseSpaceCutAHtoMuTau_WplusJets_7TeV = copy.deepcopy(genPhaseSpaceCutZtoMuTau_WplusJets_7TeV)

plotsOutputFileNameAHtoMuTau_WplusJets_7TeV = cms.string('plotsAHtoMuTau_WplusJets_7TeV_partXX.root')
patTupleOutputFileNameAHtoMuTau_WplusJets_7TeV = cms.untracked.string('patTupleAHtoMuTau_WplusJets_7TeV_partXX.root')
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# muon enriched QCD sample generated with Pythia (no cut on Pt(hat))
#  integrated luminosity = 0.183 pb^-1
# (corrected by scale factor of 1. for missing files)
#
intLumiAHtoMuTau_InclusivePPmuX_7TeV = copy.deepcopy(intLumiZtoMuTau_InclusivePPmuX_7TeV)
corrFactorAHtoMuTau_InclusivePPmuX_7TeV = copy.deepcopy(corrFactorZtoMuTau_InclusivePPmuX_7TeV)

fileNamesAHtoMuTau_InclusivePPmuX_7TeV_part01 = copy.deepcopy(fileNamesZtoMuTau_InclusivePPmuX_7TeV_part01)
fileNamesAHtoMuTau_InclusivePPmuX_7TeV_part02 = copy.deepcopy(fileNamesZtoMuTau_InclusivePPmuX_7TeV_part02)
fileNamesAHtoMuTau_InclusivePPmuX_7TeV_part03 = copy.deepcopy(fileNamesZtoMuTau_InclusivePPmuX_7TeV_part03)
fileNamesAHtoMuTau_InclusivePPmuX_7TeV_part04 = copy.deepcopy(fileNamesZtoMuTau_InclusivePPmuX_7TeV_part04)
fileNamesAHtoMuTau_InclusivePPmuX_7TeV_part05 = copy.deepcopy(fileNamesZtoMuTau_InclusivePPmuX_7TeV_part05)
fileNamesAHtoMuTau_InclusivePPmuX_7TeV_part06 = copy.deepcopy(fileNamesZtoMuTau_InclusivePPmuX_7TeV_part06)
fileNamesAHtoMuTau_InclusivePPmuX_7TeV_part07 = copy.deepcopy(fileNamesZtoMuTau_InclusivePPmuX_7TeV_part07)
fileNamesAHtoMuTau_InclusivePPmuX_7TeV_part08 = copy.deepcopy(fileNamesZtoMuTau_InclusivePPmuX_7TeV_part08)
fileNamesAHtoMuTau_InclusivePPmuX_7TeV_part09 = copy.deepcopy(fileNamesZtoMuTau_InclusivePPmuX_7TeV_part09)
fileNamesAHtoMuTau_InclusivePPmuX_7TeV_part10 = copy.deepcopy(fileNamesZtoMuTau_InclusivePPmuX_7TeV_part10)
fileNamesAHtoMuTau_InclusivePPmuX_7TeV_part11 = copy.deepcopy(fileNamesZtoMuTau_InclusivePPmuX_7TeV_part11)
fileNamesAHtoMuTau_InclusivePPmuX_7TeV_part12 = copy.deepcopy(fileNamesZtoMuTau_InclusivePPmuX_7TeV_part12)
fileNamesAHtoMuTau_InclusivePPmuX_7TeV_part13 = copy.deepcopy(fileNamesZtoMuTau_InclusivePPmuX_7TeV_part13)
fileNamesAHtoMuTau_InclusivePPmuX_7TeV_part14 = copy.deepcopy(fileNamesZtoMuTau_InclusivePPmuX_7TeV_part14)
fileNamesAHtoMuTau_InclusivePPmuX_7TeV_part15 = copy.deepcopy(fileNamesZtoMuTau_InclusivePPmuX_7TeV_part15)
fileNamesAHtoMuTau_InclusivePPmuX_7TeV_part16 = copy.deepcopy(fileNamesZtoMuTau_InclusivePPmuX_7TeV_part16)
fileNamesAHtoMuTau_InclusivePPmuX_7TeV_part17 = copy.deepcopy(fileNamesZtoMuTau_InclusivePPmuX_7TeV_part17)
fileNamesAHtoMuTau_InclusivePPmuX_7TeV_part18 = copy.deepcopy(fileNamesZtoMuTau_InclusivePPmuX_7TeV_part18)
fileNamesAHtoMuTau_InclusivePPmuX_7TeV_part19 = copy.deepcopy(fileNamesZtoMuTau_InclusivePPmuX_7TeV_part19)
fileNamesAHtoMuTau_InclusivePPmuX_7TeV_part20 = copy.deepcopy(fileNamesZtoMuTau_InclusivePPmuX_7TeV_part20)
fileNamesAHtoMuTau_InclusivePPmuX_7TeV_part21 = copy.deepcopy(fileNamesZtoMuTau_InclusivePPmuX_7TeV_part21)
fileNamesAHtoMuTau_InclusivePPmuX_7TeV_part22 = copy.deepcopy(fileNamesZtoMuTau_InclusivePPmuX_7TeV_part22)
fileNamesAHtoMuTau_InclusivePPmuX_7TeV_part23 = copy.deepcopy(fileNamesZtoMuTau_InclusivePPmuX_7TeV_part23)
fileNamesAHtoMuTau_InclusivePPmuX_7TeV_part24 = copy.deepcopy(fileNamesZtoMuTau_InclusivePPmuX_7TeV_part24)
fileNamesAHtoMuTau_InclusivePPmuX_7TeV_part25 = copy.deepcopy(fileNamesZtoMuTau_InclusivePPmuX_7TeV_part25)
fileNamesAHtoMuTau_InclusivePPmuX_7TeV_part26 = copy.deepcopy(fileNamesZtoMuTau_InclusivePPmuX_7TeV_part26)
fileNamesAHtoMuTau_InclusivePPmuX_7TeV_part27 = copy.deepcopy(fileNamesZtoMuTau_InclusivePPmuX_7TeV_part27)
fileNamesAHtoMuTau_InclusivePPmuX_7TeV_part28 = copy.deepcopy(fileNamesZtoMuTau_InclusivePPmuX_7TeV_part28)

genPhaseSpaceCutAHtoMuTau_InclusivePPmuX_7TeV = copy.deepcopy(genPhaseSpaceCutZtoMuTau_InclusivePPmuX_7TeV)

plotsOutputFileNameAHtoMuTau_InclusivePPmuX_7TeV = cms.string('plotsAHtoMuTau_InclusivePPmuX_7TeV_partXX.root')
patTupleOutputFileNameAHtoMuTau_InclusivePPmuX_7TeV = cms.untracked.string('patTupleAHtoMuTau_InclusivePPmuX_7TeV_partXX.root')
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# muon enriched QCD sample generated with Pythia (Pt(hat)> 20 GeV && PtMuon > 15 GeV)
#  integrated luminosity = 48.7 pb^-1
# (corrected by estimated scale factor of 1. for missing files)
#
intLumiAHtoMuTau_PPmuXptGt20_7TeV = copy.deepcopy(intLumiZtoMuTau_PPmuXptGt20_7TeV)
corrFactorAHtoMuTau_PPmuXptGt20_7TeV = copy.deepcopy(corrFactorZtoMuTau_PPmuXptGt20_7TeV)

fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part01 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part01)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part02 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part02)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part03 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part03)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part04 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part04)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part05 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part05)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part06 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part06)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part07 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part07)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part08 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part08)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part09 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part09)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part10 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part10)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part11 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part11)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part12 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part12)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part13 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part13)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part14 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part14)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part15 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part15)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part16 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part16)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part17 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part17)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part18 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part18)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part19 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part19)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part20 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part20)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part21 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part21)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part22 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part22)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part23 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part23)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part24 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part24)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part25 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part25)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part26 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part26)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part27 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part27)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part28 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part28)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part29 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part29)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part30 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part30)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part31 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part31)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part32 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part32)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part33 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part33)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part34 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part34)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part35 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part35)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part36 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part36)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part37 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part37)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part38 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part38)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part39 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part39)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part40 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part40)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part41 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part41)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part42 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part42)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part43 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part43)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part44 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part44)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part45 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part45)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part46 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part46)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part47 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part47)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part48 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part48)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part49 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part49)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part50 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part50)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part51 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part51)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part52 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part52)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part53 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part53)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part54 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part54)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part55 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part55)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part56 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part56)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part57 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part57)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part58 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part58)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part59 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part59)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part60 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part60)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part61 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part61)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part62 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part62)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part63 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part63)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part64 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part64)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part65 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part65)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part66 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part66)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part67 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part67)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part68 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part68)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part69 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part69)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part70 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part70)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part71 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part71)
fileNamesAHtoMuTau_PPmuXptGt20_7TeV_part72 = copy.deepcopy(fileNamesZtoMuTau_PPmuXptGt20_7TeV_part72)

genPhaseSpaceCutAHtoMuTau_PPmuXptGt20_7TeV = copy.deepcopy(genPhaseSpaceCutZtoMuTau_PPmuXptGt20_7TeV)

plotsOutputFileNameAHtoMuTau_PPmuXptGt20_7TeV = cms.string('plotsAHtoMuTau_PPmuXptGt20_7TeV_partXX.root')
patTupleOutputFileNameAHtoMuTau_PPmuXptGt20_7TeV = cms.untracked.string('patTupleAHtoMuTau_PPmuXptGt20_7TeV_partXX.root')
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# TTbar sample generated with Pythia
#
intLumiAHtoMuTau_TTplusJets_7TeV = copy.deepcopy(intLumiZtoMuTau_TTplusJets_7TeV)
corrFactorAHtoMuTau_TTplusJets_7TeV = copy.deepcopy(corrFactorZtoMuTau_TTplusJets_7TeV)

fileNamesAHtoMuTau_TTplusJets_7TeV_part01 = copy.deepcopy(fileNamesZtoMuTau_TTplusJets_7TeV_part01)
fileNamesAHtoMuTau_TTplusJets_7TeV_part02 = copy.deepcopy(fileNamesZtoMuTau_TTplusJets_7TeV_part02)
fileNamesAHtoMuTau_TTplusJets_7TeV_part03 = copy.deepcopy(fileNamesZtoMuTau_TTplusJets_7TeV_part03)
fileNamesAHtoMuTau_TTplusJets_7TeV_part04 = copy.deepcopy(fileNamesZtoMuTau_TTplusJets_7TeV_part04)
fileNamesAHtoMuTau_TTplusJets_7TeV_part05 = copy.deepcopy(fileNamesZtoMuTau_TTplusJets_7TeV_part05)
fileNamesAHtoMuTau_TTplusJets_7TeV_part06 = copy.deepcopy(fileNamesZtoMuTau_TTplusJets_7TeV_part06)
fileNamesAHtoMuTau_TTplusJets_7TeV_part07 = copy.deepcopy(fileNamesZtoMuTau_TTplusJets_7TeV_part07)
fileNamesAHtoMuTau_TTplusJets_7TeV_part08 = copy.deepcopy(fileNamesZtoMuTau_TTplusJets_7TeV_part08)
fileNamesAHtoMuTau_TTplusJets_7TeV_part09 = copy.deepcopy(fileNamesZtoMuTau_TTplusJets_7TeV_part09)
fileNamesAHtoMuTau_TTplusJets_7TeV_part10 = copy.deepcopy(fileNamesZtoMuTau_TTplusJets_7TeV_part10)

genPhaseSpaceCutAHtoMuTau_TTplusJets_7TeV = copy.deepcopy(genPhaseSpaceCutZtoMuTau_TTplusJets_7TeV)

plotsOutputFileNameAHtoMuTau_TTplusJets_7TeV = cms.string('plotsAHtoMuTau_TTplusJets_7TeV_partXX.root')
patTupleOutputFileNameAHtoMuTau_TTplusJets_7TeV = cms.untracked.string('patTupleAHtoMuTau_TTplusJets_7TeV_partXX.root')
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# gluon + gluon --> A/H --> tau+ tau- sample
#
# NOTE: luminosity is computed for cross-section of 30.1 pb,
#       corresponding to tan(beta) = 30
#      (cf. CMS AN-2009/143, table 4)
#
intLumiAHtoMuTau_AH120_tautau_7TeV = float(2335.9)
corrFactorAHtoMuTau_AH120_tautau_7TeV = float(1.81)

fileNamesAHtoMuTau_AH120_tautau_7TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/AH_M120SkimMT314/muTauSkim_1.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/AH_M120SkimMT314/muTauSkim_2.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/AH_M120SkimMT314/muTauSkim_3.root'
)

fileNamesAHtoMuTau_AH120_tautau_7TeV_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/AH_M120SkimMT314/muTauSkim_4.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/AH_M120SkimMT314/muTauSkim_5.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/AH_M120SkimMT314/muTauSkim_6.root'
)

fileNamesAHtoMuTau_AH120_tautau_7TeV_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/AH_M120SkimMT314/muTauSkim_7.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/AH_M120SkimMT314/muTauSkim_8.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/AH_M120SkimMT314/muTauSkim_9.root'
)

fileNamesAHtoMuTau_AH120_tautau_7TeV_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/AH_M120SkimMT314/muTauSkim_10.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/AH_M120SkimMT314/muTauSkim_11.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/AH_M120SkimMT314/muTauSkim_12.root'
)

fileNamesAHtoMuTau_AH120_tautau_7TeV_part05 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/AH_M120SkimMT314/muTauSkim_13.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/AH_M120SkimMT314/muTauSkim_14.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/AH_M120SkimMT314/muTauSkim_15.root'
)

fileNamesAHtoMuTau_AH120_tautau_7TeV_part06 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/AH_M120SkimMT314/muTauSkim_16.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/AH_M120SkimMT314/muTauSkim_17.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/AH_M120SkimMT314/muTauSkim_18.root'
)

fileNamesAHtoMuTau_AH120_tautau_7TeV_part07 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/AH_M120SkimMT314/muTauSkim_19.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/AH_M120SkimMT314/muTauSkim_20.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/AH_M120SkimMT314/muTauSkim_21.root'
)

fileNamesAHtoMuTau_AH120_tautau_7TeV_part08 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/AH_M120SkimMT314/muTauSkim_22.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/AH_M120SkimMT314/muTauSkim_23.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/AH_M120SkimMT314/muTauSkim_24.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/AH_M120SkimMT314/muTauSkim_25.root'
)

genPhaseSpaceCutAHtoMuTau_AH120_tautau_7TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

plotsOutputFileNameAHtoMuTau_AH120_tautau_7TeV = cms.string('plotsAHtoMuTau_AH120_tautau_7TeV_partXX.root')
patTupleOutputFileNameAHtoMuTau_AH120_tautau_7TeV = cms.untracked.string('patTupleAHtoMuTau_AH120_tautau_7TeV_partXX.root')
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# gluon + gluon --> A/H + b bbar --> tau+ tau- + X sample
#
# NOTE: luminosity is computed for cross-section of 55.9 pb,
#       corresponding to tan(beta) = 30
#      (cf. CMS AN-2009/143, table 4)
#
intLumiAHtoMuTau_AHbb120_tautau_7TeV = float(1808.4)
corrFactorAHtoMuTau_AHbb120_tautau_7TeV = float(1.11)

fileNamesAHtoMuTau_AHbb120_tautau_7TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/AHbb_M120SkimMT314/muTauSkim_1.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/AHbb_M120SkimMT314/muTauSkim_3.root'
)

fileNamesAHtoMuTau_AHbb120_tautau_7TeV_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/AHbb_M120SkimMT314/muTauSkim_4.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/AHbb_M120SkimMT314/muTauSkim_5.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/AHbb_M120SkimMT314/muTauSkim_6.root'
)

fileNamesAHtoMuTau_AHbb120_tautau_7TeV_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/AHbb_M120SkimMT314/muTauSkim_7.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/AHbb_M120SkimMT314/muTauSkim_8.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/AHbb_M120SkimMT314/muTauSkim_9.root'
)

fileNamesAHtoMuTau_AHbb120_tautau_7TeV_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/AHbb_M120SkimMT314/muTauSkim_10.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/AHbb_M120SkimMT314/muTauSkim_11.root'
)

genPhaseSpaceCutAHtoMuTau_AHbb120_tautau_7TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

plotsOutputFileNameAHtoMuTau_AHbb120_tautau_7TeV = cms.string('plotsAHtoMuTau_AHbb120_tautau_7TeV_partXX.root')
patTupleOutputFileNameAHtoMuTau_AHbb120_tautau_7TeV = cms.untracked.string('patTupleAHtoMuTau_AHbb120_tautau_7TeV_partXX.root')
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# W/Z + c cbar/b bbar sample generated with Madgraph
# (all W/Z decay modes)
#  integrated luminosity = 26165.9 pb^-1
# (corrected by scale factor of 1. for missing files)
#
# (NOTE: for Monte Carlo samples generated by Madgraph,
#        the filter efficiency is already included in the cross-sections
#        listed at https://twiki.cern.ch/twiki/bin/view/CMS/ProductionSummer2009at7TeV#MadGraph !!)
#
intLumiAHtoMuTau_Vqq_7TeV = float(26165.9)
corrFactorAHtoMuTau_Vqq_7TeV = float(1.)

fileNamesAHtoMuTau_Vqq_7TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/7TeV/VqqJets-madgraph/akalinow-SkimMuTau_7TeV_314_pass4/SkimMuTau_Merged_1.root'
)

fileNamesAHtoMuTau_Vqq_7TeV_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/7TeV/VqqJets-madgraph/akalinow-SkimMuTau_7TeV_314_pass4/SkimMuTau_Merged_2.root'
)

fileNamesAHtoMuTau_Vqq_7TeV_part03 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/7TeV/VqqJets-madgraph/akalinow-SkimMuTau_7TeV_314_pass4/SkimMuTau_Merged_3.root'
)

fileNamesAHtoMuTau_Vqq_7TeV_part04 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/7TeV/VqqJets-madgraph/akalinow-SkimMuTau_7TeV_314_pass4/SkimMuTau_Merged_4.root'
)

fileNamesAHtoMuTau_Vqq_7TeV_part05 = cms.untracked.vstring(        
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/7TeV/VqqJets-madgraph/akalinow-SkimMuTau_7TeV_314_pass4/SkimMuTau_Merged_5.root'
)

fileNamesAHtoMuTau_Vqq_7TeV_part06 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/7TeV/VqqJets-madgraph/akalinow-SkimMuTau_7TeV_314_pass4/SkimMuTau_Merged_6.root'
)

fileNamesAHtoMuTau_Vqq_7TeV_part07 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/7TeV/VqqJets-madgraph/akalinow-SkimMuTau_7TeV_314_pass4/SkimMuTau_Merged_7.root'
)

fileNamesAHtoMuTau_Vqq_7TeV_part08 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/7TeV/VqqJets-madgraph/akalinow-SkimMuTau_7TeV_314_pass4/SkimMuTau_Merged_8.root'
)

fileNamesAHtoMuTau_Vqq_7TeV_part09 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/7TeV/VqqJets-madgraph/akalinow-SkimMuTau_7TeV_314_pass4/SkimMuTau_Merged_9.root'
)

fileNamesAHtoMuTau_Vqq_7TeV_part10 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/7TeV/VqqJets-madgraph/akalinow-SkimMuTau_7TeV_314_pass4/SkimMuTau_Merged_10.root'
)

fileNamesAHtoMuTau_Vqq_7TeV_part11 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/7TeV/VqqJets-madgraph/akalinow-SkimMuTau_7TeV_314_pass4/SkimMuTau_Merged_11.root'
)

fileNamesAHtoMuTau_Vqq_7TeV_part12 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/7TeV/VqqJets-madgraph/akalinow-SkimMuTau_7TeV_314_pass4/SkimMuTau_Merged_12.root'
)

fileNamesAHtoMuTau_Vqq_7TeV_part13 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/7TeV/VqqJets-madgraph/akalinow-SkimMuTau_7TeV_314_pass4/SkimMuTau_Merged_13.root'
)

fileNamesAHtoMuTau_Vqq_7TeV_part14 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/7TeV/VqqJets-madgraph/akalinow-SkimMuTau_7TeV_314_pass4/SkimMuTau_Merged_14.root'
)

fileNamesAHtoMuTau_Vqq_7TeV_part15 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/7TeV/VqqJets-madgraph/akalinow-SkimMuTau_7TeV_314_pass4/SkimMuTau_Merged_16.root'
)

fileNamesAHtoMuTau_Vqq_7TeV_part16 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/7TeV/VqqJets-madgraph/akalinow-SkimMuTau_7TeV_314_pass4/SkimMuTau_Merged_18.root'
)

fileNamesAHtoMuTau_Vqq_7TeV_part17 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/7TeV/VqqJets-madgraph/akalinow-SkimMuTau_7TeV_314_pass4/SkimMuTau_Merged_19.root'
)

fileNamesAHtoMuTau_Vqq_7TeV_part18 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/7TeV/VqqJets-madgraph/akalinow-SkimMuTau_7TeV_314_pass4/SkimMuTau_Merged_21.root'
)

fileNamesAHtoMuTau_Vqq_7TeV_part19 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/7TeV/VqqJets-madgraph/akalinow-SkimMuTau_7TeV_314_pass4/SkimMuTau_Merged_22.root'
)

fileNamesAHtoMuTau_Vqq_7TeV_part20 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/7TeV/VqqJets-madgraph/akalinow-SkimMuTau_7TeV_314_pass4/SkimMuTau_Merged_23.root'
)

fileNamesAHtoMuTau_Vqq_7TeV_part21 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/7TeV/VqqJets-madgraph/akalinow-SkimMuTau_7TeV_314_pass4/SkimMuTau_Merged_25.root'
)

fileNamesAHtoMuTau_Vqq_7TeV_part22 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/7TeV/VqqJets-madgraph/akalinow-SkimMuTau_7TeV_314_pass4/SkimMuTau_Merged_26.root'
)

fileNamesAHtoMuTau_Vqq_7TeV_part23 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/a/akalinow/CMS/ZTauTauSkims/7TeV/VqqJets-madgraph/akalinow-SkimMuTau_7TeV_314_pass4/SkimMuTau_Merged_27.root'
)

genPhaseSpaceCutAHtoMuTau_Vqq_7TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

plotsOutputFileNameAHtoMuTau_Vqq_7TeV = cms.string('plotsAHtoMuTau_Vqq_7TeV_partXX.root')
patTupleOutputFileNameAHtoMuTau_Vqq_7TeV = cms.untracked.string('patTupleAHtoMuTau_Vqq_7TeV_partXX.root')
#--------------------------------------------------------------------------------
