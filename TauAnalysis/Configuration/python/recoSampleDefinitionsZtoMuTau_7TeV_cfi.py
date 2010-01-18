import FWCore.ParameterSet.Config as cms
import copy

# define configuration parameters for submission of Z --> mu + tau-jet jobs to CERN batch system
# (running over skimmed samples stored on CASTOR)

intLumiZtoMuTau_Data_7TeV = float(200.)

#--------------------------------------------------------------------------------
# Z --> tau+ tau- sample generated with Pythia + Tauola (all decay modes)
#  integrated luminosity = 1686.9 pb^-1
# (corrected by scale factor of 1. for missing files)
#
intLumiZtoMuTau_Ztautau_7TeV = float(1686.9)
corrFactorZtoMuTau_Ztautau_7TeV = float(1.)

fileNamesZtoMuTau_Ztautau_7TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_0.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_1.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_2.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_3.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_4.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_5.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_6.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_7.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_8.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_9.root'
)

fileNamesZtoMuTau_Ztautau_7TeV_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_10.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_11.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_12.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_13.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_14.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_15.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_16.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_17.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_18.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_19.root'
)
    
fileNamesZtoMuTau_Ztautau_7TeV_part03 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_20.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_21.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_22.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_23.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_24.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_25.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_26.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_27.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_28.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_29.root'
)

fileNamesZtoMuTau_Ztautau_7TeV_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_30.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_31.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_32.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_33.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_34.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_35.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_36.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_37.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_38.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_39.root'
)

fileNamesZtoMuTau_Ztautau_7TeV_part05 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_40.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_41.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_42.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_43.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_44.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_45.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_46.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_47.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_48.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_49.root'
)

fileNamesZtoMuTau_Ztautau_7TeV_part06 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_50.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_51.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_52.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_53.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_54.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_55.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_56.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_57.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_58.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_59.root'
)

fileNamesZtoMuTau_Ztautau_7TeV_part07 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_60.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_61.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_62.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_63.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_64.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_65.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_66.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_67.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_68.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_69.root'
)

fileNamesZtoMuTau_Ztautau_7TeV_part08 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_70.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_71.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_72.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_73.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_74.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_75.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_76.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_77.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_78.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_79.root'
)
    
fileNamesZtoMuTau_Ztautau_7TeV_part09 = cms.untracked.vstring(       
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_80.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_81.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_82.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_83.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_84.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_85.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_86.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_87.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_88.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_89.root'
)

fileNamesZtoMuTau_Ztautau_7TeV_part10 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_90.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_91.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_92.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_93.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_94.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_95.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_96.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_97.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_98.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_99.root'
)

fileNamesZtoMuTau_Ztautau_7TeV_part11 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_100.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_101.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_102.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_103.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_104.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_105.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_106.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_107.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_108.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_109.root'
)

fileNamesZtoMuTau_Ztautau_7TeV_part12 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_110.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_111.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_112.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_113.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_114.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_115.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_116.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_117.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_118.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_119.root'
)

fileNamesZtoMuTau_Ztautau_7TeV_part13 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_120.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_121.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_122.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_123.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_124.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_125.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_126.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_127.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_128.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_129.root'
)

fileNamesZtoMuTau_Ztautau_7TeV_part14 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_130.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_131.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_132.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_133.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_134.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_135.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_136.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_137.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_138.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_139.root'
)

fileNamesZtoMuTau_Ztautau_7TeV_part15 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_140.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_141.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_142.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_143.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_144.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_145.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_146.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_147.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_148.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_149.root'
)

fileNamesZtoMuTau_Ztautau_7TeV_part16 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_150.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_151.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_152.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_153.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_154.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_155.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_156.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_157.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_158.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_159.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ztautau_7TeV/muTauSkim_ztautau_7TeV_160.root'
)

genPhaseSpaceCutZtoMuTau_Ztautau_7TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

plotsOutputFileNameZtoMuTau_Ztautau_7TeV = cms.string('plotsZtoMuTau_Ztautau_7TeV_partXX.root')
patTupleOutputFileNameZtoMuTau_Ztautau_7TeV = cms.untracked.string('patTupleZtoMuTau_Ztautau_7TeV_partXX.root')
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# Z --> mu+ mu- sample generated with Pythia
#  integrated luminosity = 1604.1 pb^-1
# (corrected by scale factor of 1. for missing files)
#
intLumiZtoMuTau_Zmumu_7TeV = float(1604.1)
corrFactorZtoMuTau_Zmumu_7TeV = float(1.)

fileNamesZtoMuTau_Zmumu_7TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_161.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_162.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_163.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_164.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_165.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_166.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_167.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_168.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_169.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_170.root'
)

fileNamesZtoMuTau_Zmumu_7TeV_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_171.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_172.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_173.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_174.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_175.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_176.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_177.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_178.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_179.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_180.root'
)

fileNamesZtoMuTau_Zmumu_7TeV_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_181.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_182.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_183.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_184.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_185.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_186.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_187.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_188.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_189.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_190.root'
)

fileNamesZtoMuTau_Zmumu_7TeV_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_191.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_192.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_193.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_194.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_195.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_196.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_197.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_198.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_199.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_200.root'
)

fileNamesZtoMuTau_Zmumu_7TeV_part05 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_201.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_202.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_203.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_204.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_205.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_206.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_207.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_208.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_209.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_210.root'
)

fileNamesZtoMuTau_Zmumu_7TeV_part06 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_211.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_212.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_213.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_214.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_215.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_216.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_217.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_218.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_219.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_220.root'
)

fileNamesZtoMuTau_Zmumu_7TeV_part07 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_221.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_222.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_223.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_224.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_225.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_226.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_227.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_228.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_229.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_230.root'
)

fileNamesZtoMuTau_Zmumu_7TeV_part08 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_231.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_232.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_233.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_234.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_235.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_236.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_237.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_238.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_239.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_240.root'
)

fileNamesZtoMuTau_Zmumu_7TeV_part09 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_241.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_242.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_243.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_244.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_245.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_246.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_247.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_248.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_249.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_250.root'
)

fileNamesZtoMuTau_Zmumu_7TeV_part10 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_251.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_252.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_253.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_254.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_255.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_256.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_257.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_258.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_259.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_260.root'
)

fileNamesZtoMuTau_Zmumu_7TeV_part11 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_261.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_262.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_263.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_264.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_265.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_266.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_267.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_268.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_269.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_270.root'
)

fileNamesZtoMuTau_Zmumu_7TeV_part12 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_271.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_272.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_273.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_274.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_275.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_276.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_277.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_278.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_280.root'
)

fileNamesZtoMuTau_Zmumu_7TeV_part13 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_281.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_282.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_283.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_284.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_285.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_286.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_287.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_288.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_289.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_290.root'
)

fileNamesZtoMuTau_Zmumu_7TeV_part14 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_291.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_292.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_293.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_294.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_295.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_296.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_297.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_298.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_299.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_300.root'
)

fileNamesZtoMuTau_Zmumu_7TeV_part15 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_301.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_302.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_303.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_304.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_305.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_306.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_307.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_308.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_309.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zmumu_7TeV/muTauSkim_zmumu_7TeV_310.root'
)    

genPhaseSpaceCutZtoMuTau_Zmumu_7TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

plotsOutputFileNameZtoMuTau_Zmumu_7TeV = cms.string('plotsZtoMuTau_Zmumu_7TeV_partXX.root')
patTupleOutputFileNameZtoMuTau_Zmumu_7TeV = cms.untracked.string('patTupleZtoMuTau_Zmumu_7TeV_partXX.root')
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
intLumiZtoMuTau_ZplusJets_7TeV = float(454.8)
corrFactorZtoMuTau_ZplusJets_7TeV = float(1.)

fileNamesZtoMuTau_ZplusJets_7TeV_part01 = cms.untracked.vstring(
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

fileNamesZtoMuTau_ZplusJets_7TeV_part02 = cms.untracked.vstring(    
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

fileNamesZtoMuTau_ZplusJets_7TeV_part03 = cms.untracked.vstring(
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

fileNamesZtoMuTau_ZplusJets_7TeV_part04 = cms.untracked.vstring(
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

fileNamesZtoMuTau_ZplusJets_7TeV_part05 = cms.untracked.vstring(
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

fileNamesZtoMuTau_ZplusJets_7TeV_part06 = cms.untracked.vstring(
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

fileNamesZtoMuTau_ZplusJets_7TeV_part07 = cms.untracked.vstring(
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

fileNamesZtoMuTau_ZplusJets_7TeV_part08 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_70.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_71.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_72.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_73.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/zjets_7TeV/muTauSkim_zjets_7TeV_74.root'
)

corrFactorZtoMuTau_ZeePlusJets = corrFactorZtoMuTau_ZplusJets_7TeV
intLumiZtoMuTau_ZeePlusJets = intLumiZtoMuTau_ZplusJets_7TeV

fileNamesZtoMuTau_ZeePlusJets_part01 = fileNamesZtoMuTau_ZplusJets_7TeV_part01
fileNamesZtoMuTau_ZeePlusJets_part02 = fileNamesZtoMuTau_ZplusJets_7TeV_part02
fileNamesZtoMuTau_ZeePlusJets_part03 = fileNamesZtoMuTau_ZplusJets_7TeV_part03
fileNamesZtoMuTau_ZeePlusJets_part04 = fileNamesZtoMuTau_ZplusJets_7TeV_part04
fileNamesZtoMuTau_ZeePlusJets_part05 = fileNamesZtoMuTau_ZplusJets_7TeV_part05
fileNamesZtoMuTau_ZeePlusJets_part06 = fileNamesZtoMuTau_ZplusJets_7TeV_part06
fileNamesZtoMuTau_ZeePlusJets_part07 = fileNamesZtoMuTau_ZplusJets_7TeV_part07
fileNamesZtoMuTau_ZeePlusJets_part08 = fileNamesZtoMuTau_ZplusJets_7TeV_part08

patTupleOutputFileNameZtoMuTau_ZeePlusJets = cms.untracked.string('patTupleZtoMuTau_ZeePlusJets_7TeV_partXX.root')

plotsOutputFileNameZtoMuTau_ZeePlusJets = cms.string('plotsZtoMuTau_ZeePlusJets_7TeV_partXX.root')

genPhaseSpaceCutZtoMuTau_ZeePlusJets = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('genElectronsFromZs'),
    minNumber = cms.uint32(2)
)

corrFactorZtoMuTau_ZmumuPlusJets = corrFactorZtoMuTau_ZplusJets_7TeV
intLumiZtoMuTau_ZmumuPlusJets = intLumiZtoMuTau_ZplusJets_7TeV

fileNamesZtoMuTau_ZmumuPlusJets_part01 = fileNamesZtoMuTau_ZplusJets_7TeV_part01
fileNamesZtoMuTau_ZmumuPlusJets_part02 = fileNamesZtoMuTau_ZplusJets_7TeV_part02
fileNamesZtoMuTau_ZmumuPlusJets_part03 = fileNamesZtoMuTau_ZplusJets_7TeV_part03
fileNamesZtoMuTau_ZmumuPlusJets_part04 = fileNamesZtoMuTau_ZplusJets_7TeV_part04
fileNamesZtoMuTau_ZmumuPlusJets_part05 = fileNamesZtoMuTau_ZplusJets_7TeV_part05
fileNamesZtoMuTau_ZmumuPlusJets_part06 = fileNamesZtoMuTau_ZplusJets_7TeV_part06
fileNamesZtoMuTau_ZmumuPlusJets_part07 = fileNamesZtoMuTau_ZplusJets_7TeV_part07
fileNamesZtoMuTau_ZmumuPlusJets_part08 = fileNamesZtoMuTau_ZplusJets_7TeV_part08

patTupleOutputFileNameZtoMuTau_ZmumuPlusJets = cms.untracked.string('patTupleZtoMuTau_ZmumuPlusJets_7TeV_partXX.root')

plotsOutputFileNameZtoMuTau_ZmumuPlusJets = cms.string('plotsZtoMuTau_ZmumuPlusJets_7TeV_partXX.root')

genPhaseSpaceCutZtoMuTau_ZmumuPlusJets = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('genMuonsFromZs'),
    minNumber = cms.uint32(2)
)

corrFactorZtoMuTau_ZtautauPlusJets = corrFactorZtoMuTau_ZplusJets_7TeV
intLumiZtoMuTau_ZtautauPlusJets = intLumiZtoMuTau_ZplusJets_7TeV

fileNamesZtoMuTau_ZtautauPlusJets_part01 = fileNamesZtoMuTau_ZplusJets_7TeV_part01
fileNamesZtoMuTau_ZtautauPlusJets_part02 = fileNamesZtoMuTau_ZplusJets_7TeV_part02
fileNamesZtoMuTau_ZtautauPlusJets_part03 = fileNamesZtoMuTau_ZplusJets_7TeV_part03
fileNamesZtoMuTau_ZtautauPlusJets_part04 = fileNamesZtoMuTau_ZplusJets_7TeV_part04
fileNamesZtoMuTau_ZtautauPlusJets_part05 = fileNamesZtoMuTau_ZplusJets_7TeV_part05
fileNamesZtoMuTau_ZtautauPlusJets_part06 = fileNamesZtoMuTau_ZplusJets_7TeV_part06
fileNamesZtoMuTau_ZtautauPlusJets_part07 = fileNamesZtoMuTau_ZplusJets_7TeV_part07
fileNamesZtoMuTau_ZtautauPlusJets_part08 = fileNamesZtoMuTau_ZplusJets_7TeV_part08

patTupleOutputFileNameZtoMuTau_ZtautauPlusJets = cms.untracked.string('patTupleZtoMuTau_ZtautauPlusJets_7TeV_partXX.root')

plotsOutputFileNameZtoMuTau_ZtautauPlusJets = cms.string('plotsZtoMuTau_ZtautauPlusJets_7TeV_partXX.root')

genPhaseSpaceCutZtoMuTau_ZtautauPlusJets = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('genTausFromZs'),
    minNumber = cms.uint32(2)
)
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
intLumiZtoMuTau_WplusJets_7TeV = float(638.1)
corrFactorZtoMuTau_WplusJets_7TeV = float(1.)

fileNamesZtoMuTau_WplusJets_7TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_75.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_76.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_77.root',    
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_78.root',    
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_79.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_80.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_81.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_82.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_83.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_84.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_85.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_86.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_87.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_88.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_89.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_90.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_91.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_92.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_93.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_94.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_95.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_96.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_97.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_98.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_99.root'
)

fileNamesZtoMuTau_WplusJets_7TeV_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_100.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_101.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_102.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_103.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_104.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_105.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_106.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_107.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_108.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_109.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_110.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_111.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_112.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_113.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_114.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_115.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_116.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_117.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_118.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_119.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_120.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_121.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_122.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_123.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_124.root'
)

fileNamesZtoMuTau_WplusJets_7TeV_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_125.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_126.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_127.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_128.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_129.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_130.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_131.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_132.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_133.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_134.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_135.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_136.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_137.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_138.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_139.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_140.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_141.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_142.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_143.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_144.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_145.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_146.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_147.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_148.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_149.root'
)

fileNamesZtoMuTau_WplusJets_7TeV_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_150.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_151.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_152.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_153.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_154.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_155.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_156.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_157.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_158.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_159.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_160.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_161.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_162.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_163.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_164.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_165.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_166.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_167.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_168.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_169.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_170.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_171.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_172.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_173.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_174.root'
)

fileNamesZtoMuTau_WplusJets_7TeV_part05 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_175.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_176.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_177.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_178.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_179.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_180.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_181.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_182.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_183.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_184.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_185.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_186.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_187.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_188.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_189.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_190.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_191.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_192.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_193.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_194.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_195.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_196.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_197.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_198.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_199.root'
)

fileNamesZtoMuTau_WplusJets_7TeV_part06 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_200.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_201.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_202.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_203.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_204.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_205.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_206.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_207.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_208.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_209.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_210.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_211.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_212.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_213.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_214.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_215.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_216.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_217.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_218.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_219.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_220.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_221.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_222.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_223.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_224.root'
)

fileNamesZtoMuTau_WplusJets_7TeV_part07 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_225.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_226.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_227.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_228.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_229.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_230.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_231.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_232.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_233.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_234.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_235.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_236.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_237.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_238.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_239.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_240.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_241.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_242.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_243.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_244.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_245.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_246.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_247.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_248.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_249.root'
)

fileNamesZtoMuTau_WplusJets_7TeV_part08 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_250.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_251.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_252.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_253.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_254.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_255.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_256.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_257.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_258.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_259.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_260.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_261.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_262.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_263.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_264.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_265.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_266.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_267.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_268.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_269.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_270.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_271.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_272.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_273.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_274.root'
)

fileNamesZtoMuTau_WplusJets_7TeV_part09 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_275.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_276.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_277.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_278.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_279.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_280.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_281.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_282.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_283.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_284.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_285.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_286.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_287.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_288.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_289.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_290.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_291.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_292.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_293.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_294.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_295.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_296.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_297.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_298.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_299.root'
)

fileNamesZtoMuTau_WplusJets_7TeV_part10 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_300.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_301.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_302.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_303.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_304.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_305.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_306.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_307.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_308.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_309.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_310.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_311.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_312.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_313.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_314.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_315.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_316.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_317.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_318.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_319.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_320.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_321.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_322.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_323.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_324.root'
)

fileNamesZtoMuTau_WplusJets_7TeV_part11 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_325.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_326.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_327.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_328.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_329.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_330.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_331.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_332.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_333.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_334.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_335.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_336.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_337.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_338.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_339.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_340.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_341.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_342.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_343.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_344.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_345.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_346.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_347.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_348.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_349.root'
)

fileNamesZtoMuTau_WplusJets_7TeV_part12 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_350.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_351.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_352.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_353.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_354.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_355.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_356.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_357.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_358.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_359.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_360.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_361.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_362.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_363.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_364.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_365.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_366.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_367.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_368.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_369.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_370.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_371.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_372.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_373.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_374.root'
)

fileNamesZtoMuTau_WplusJets_7TeV_part13 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_375.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_376.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_377.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_378.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_379.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_380.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_381.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_382.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_383.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_384.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_385.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_386.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_387.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_388.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_389.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_390.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_391.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_392.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_393.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_394.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_395.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_396.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_397.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_398.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_399.root'
)

fileNamesZtoMuTau_WplusJets_7TeV_part14 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_400.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_401.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_402.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_403.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_404.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_405.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_406.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_407.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_408.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_409.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_410.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_411.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_412.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_413.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_414.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_415.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_416.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_417.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_418.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_419.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_420.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_421.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_422.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_423.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_424.root'
)

fileNamesZtoMuTau_WplusJets_7TeV_part15 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_425.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_426.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_427.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_428.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_429.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_430.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_431.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_432.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_433.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_434.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_435.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_436.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_437.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_438.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_439.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_440.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_441.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_442.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_443.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_444.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_445.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_446.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_447.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_448.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_449.root'
)

fileNamesZtoMuTau_WplusJets_7TeV_part16 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_450.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_451.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_452.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_453.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_454.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_455.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_456.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_457.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_458.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_459.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_460.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_461.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_462.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_463.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_464.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_465.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_466.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_467.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_468.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_469.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_470.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_471.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_472.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_473.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_474.root'
)

fileNamesZtoMuTau_WplusJets_7TeV_part17 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_475.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_476.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_477.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_478.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_479.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_480.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_481.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_482.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_483.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_484.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_485.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_486.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_487.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_488.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_489.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_490.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_491.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_492.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_493.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_494.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_495.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_496.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_497.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_498.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_499.root'
)

fileNamesZtoMuTau_WplusJets_7TeV_part18 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_500.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_501.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_502.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_503.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_504.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_505.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_506.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_507.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_508.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_509.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_510.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_511.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_512.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_513.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_514.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_515.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_516.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_517.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_518.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_519.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_520.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_521.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_522.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_523.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_524.root'
)

fileNamesZtoMuTau_WplusJets_7TeV_part19 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_525.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_526.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_527.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_528.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_529.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_530.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_531.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_532.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_533.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_534.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_535.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_536.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_537.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_538.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_539.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_540.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_541.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_542.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_543.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_544.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_545.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_546.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_547.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_548.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_549.root'
)

fileNamesZtoMuTau_WplusJets_7TeV_part20 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_550.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_551.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_552.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_553.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_554.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_555.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_556.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_557.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_558.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_559.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_560.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_561.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_562.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_563.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_564.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_565.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_566.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_567.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_568.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_569.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_570.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_571.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_572.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_573.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_574.root'
)

fileNamesZtoMuTau_WplusJets_7TeV_part21 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_575.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_576.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_577.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_578.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_579.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_580.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_581.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_582.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_583.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_584.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_585.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_586.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_587.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_588.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_589.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_590.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_591.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_592.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_593.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_594.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_595.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_596.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_597.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_598.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_599.root'
)

fileNamesZtoMuTau_WplusJets_7TeV_part22 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_600.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_601.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_602.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_603.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_604.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_605.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_606.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_607.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_608.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_609.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_610.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_611.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_612.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_613.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_614.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_615.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_616.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_617.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_618.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_619.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_620.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_621.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_622.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_623.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_624.root'
)

fileNamesZtoMuTau_WplusJets_7TeV_part23 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_625.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_626.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_627.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_628.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_629.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_630.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_631.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_632.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_633.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_634.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_635.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_636.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_637.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_638.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_639.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_640.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_641.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_642.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_643.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_644.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_645.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_646.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_647.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_648.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_649.root'
)

fileNamesZtoMuTau_WplusJets_7TeV_part24 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_650.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_651.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_652.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_653.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_654.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_655.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_656.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_657.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_658.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_659.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_660.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_661.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_662.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_663.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_664.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_665.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_666.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_667.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_668.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_669.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_670.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_671.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_672.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_673.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_674.root'
)

fileNamesZtoMuTau_WplusJets_7TeV_part25 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_675.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_676.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_677.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_678.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_679.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_680.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_681.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_682.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_683.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_684.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_685.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_686.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_687.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_688.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_689.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_690.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_691.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_692.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_693.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_694.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_695.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_696.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_697.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_698.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_699.root'
)

fileNamesZtoMuTau_WplusJets_7TeV_part26 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_700.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_701.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_702.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_703.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_704.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_705.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_706.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_707.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_708.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_709.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_710.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_711.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_712.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_713.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_714.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_715.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_716.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_717.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_718.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_719.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_720.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_721.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_722.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_723.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_724.root'
)

fileNamesZtoMuTau_WplusJets_7TeV_part27 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_725.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_726.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_727.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_728.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_729.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_730.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_731.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_732.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_733.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_734.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_735.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_736.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_737.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_738.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_739.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_740.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_741.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_742.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_743.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_744.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_745.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_746.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_747.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_748.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_749.root'
)

fileNamesZtoMuTau_WplusJets_7TeV_part28 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_750.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_751.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_752.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_753.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_754.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_755.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_756.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_757.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_758.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_759.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_760.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_761.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_762.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_763.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_764.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_765.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_766.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_767.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_768.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_769.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_770.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_771.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_772.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_773.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_774.root'
)

fileNamesZtoMuTau_WplusJets_7TeV_part29 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_775.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_776.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_777.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_778.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_779.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_780.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_781.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_782.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_783.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_784.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_785.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_786.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_787.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_788.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_789.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_790.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_791.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_792.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_793.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_794.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_795.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_796.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_797.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_798.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_799.root'
)

fileNamesZtoMuTau_WplusJets_7TeV_part30 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_800.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_801.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_802.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_803.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_804.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_805.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_806.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_807.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_808.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_809.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_810.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_811.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_812.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_813.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_814.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_815.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_816.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_817.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_818.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_819.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_820.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_821.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_822.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_823.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_824.root'
)

fileNamesZtoMuTau_WplusJets_7TeV_part31 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_825.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_826.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_827.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_828.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_829.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_830.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_831.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_832.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_833.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_834.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/wjets_7TeV/muTauSkim_wjets_7TeV_835.root'
)

genPhaseSpaceCutZtoMuTau_WplusJets_7TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

plotsOutputFileNameZtoMuTau_WplusJets_7TeV = cms.string('plotsZtoMuTau_WplusJets_7TeV_partXX.root')
patTupleOutputFileNameZtoMuTau_WplusJets_7TeV = cms.untracked.string('patTupleZtoMuTau_WplusJets_7TeV_partXX.root')
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# muon enriched QCD sample generated with Pythia (no cut on Pt(hat))
#  integrated luminosity = 0.183 pb^-1
# (corrected by scale factor of 1. for missing files)
#
intLumiZtoMuTau_InclusivePPmuX_7TeV = float(0.183)
corrFactorZtoMuTau_InclusivePPmuX_7TeV = float(1.)

fileNamesZtoMuTau_InclusivePPmuX_7TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_671.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_672.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_673.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_674.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_675.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_676.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_677.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_678.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_679.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_680.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_681.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_682.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_683.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_684.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_685.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_686.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_687.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_688.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_689.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_690.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_691.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_692.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_693.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_694.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_695.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_696.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_697.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_698.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_699.root'
)

fileNamesZtoMuTau_InclusivePPmuX_7TeV_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_700.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_701.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_702.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_703.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_704.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_705.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_706.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_707.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_708.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_709.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_710.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_711.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_712.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_713.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_714.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_715.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_716.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_717.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_718.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_719.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_720.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_721.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_722.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_723.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_724.root'
)

fileNamesZtoMuTau_InclusivePPmuX_7TeV_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_725.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_726.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_727.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_728.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_729.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_730.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_731.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_732.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_733.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_734.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_735.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_736.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_737.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_738.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_739.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_740.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_741.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_742.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_743.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_744.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_745.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_746.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_747.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_748.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_749.root'
)

fileNamesZtoMuTau_InclusivePPmuX_7TeV_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_750.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_751.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_752.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_753.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_754.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_755.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_756.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_757.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_758.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_759.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_760.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_761.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_762.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_763.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_764.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_765.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_766.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_767.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_768.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_769.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_770.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_771.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_772.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_773.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_774.root'
)

fileNamesZtoMuTau_InclusivePPmuX_7TeV_part05 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_775.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_776.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_777.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_778.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_779.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_780.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_781.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_782.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_783.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_784.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_785.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_786.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_787.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_788.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_789.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_790.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_791.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_792.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_793.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_794.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_795.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_796.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_797.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_798.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_799.root'
)

fileNamesZtoMuTau_InclusivePPmuX_7TeV_part06 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_800.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_801.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_802.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_803.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_804.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_805.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_806.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_807.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_808.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_809.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_810.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_811.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_812.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_813.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_814.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_815.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_816.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_817.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_818.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_819.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_820.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_821.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_822.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_823.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_824.root'
)

fileNamesZtoMuTau_InclusivePPmuX_7TeV_part07 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_825.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_826.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_827.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_828.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_829.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_830.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_831.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_832.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_833.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_834.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_835.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_836.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_837.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_838.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_839.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_840.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_841.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_842.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_843.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_844.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_845.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_846.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_847.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_848.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_849.root'
)

fileNamesZtoMuTau_InclusivePPmuX_7TeV_part08 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_850.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_851.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_852.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_853.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_854.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_855.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_856.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_857.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_858.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_859.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_860.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_861.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_862.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_863.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_864.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_865.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_866.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_867.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_868.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_869.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_870.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_871.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_872.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_873.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_874.root'
)

fileNamesZtoMuTau_InclusivePPmuX_7TeV_part09 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_875.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_876.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_877.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_878.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_879.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_880.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_881.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_882.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_883.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_884.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_885.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_886.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_887.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_888.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_889.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_890.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_891.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_892.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_893.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_894.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_895.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_896.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_897.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_898.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_899.root'
)

fileNamesZtoMuTau_InclusivePPmuX_7TeV_part10 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_900.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_901.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_902.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_903.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_904.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_905.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_906.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_907.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_908.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_909.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_910.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_911.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_912.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_913.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_914.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_915.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_916.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_917.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_918.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_919.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_920.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_921.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_922.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_923.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_924.root'
)

fileNamesZtoMuTau_InclusivePPmuX_7TeV_part11 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_925.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_926.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_927.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_928.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_929.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_930.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_931.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_932.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_933.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_934.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_935.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_936.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_937.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_938.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_939.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_940.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_941.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_942.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_943.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_944.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_945.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_946.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_947.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_948.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_949.root'
)

fileNamesZtoMuTau_InclusivePPmuX_7TeV_part12 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_950.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_951.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_952.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_953.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_954.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_955.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_956.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_957.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_958.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_959.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_960.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_961.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_962.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_963.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_964.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_965.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_966.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_967.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_968.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_969.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_970.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_971.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_972.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_973.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_974.root'
)

fileNamesZtoMuTau_InclusivePPmuX_7TeV_part13 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_975.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_976.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_977.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_978.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_979.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_980.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_981.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_982.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_983.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_984.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_985.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_986.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_987.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_988.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_989.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_990.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_991.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_992.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_993.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_994.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_995.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_996.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_997.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_998.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_999.root'
)

fileNamesZtoMuTau_InclusivePPmuX_7TeV_part14 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1000.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1001.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1002.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1003.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1004.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1005.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1006.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1007.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1008.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1009.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1010.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1011.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1012.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1013.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1014.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1015.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1016.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1017.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1018.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1019.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1020.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1021.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1022.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1023.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1024.root'
)

fileNamesZtoMuTau_InclusivePPmuX_7TeV_part15 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1025.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1026.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1027.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1028.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1029.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1030.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1031.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1032.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1033.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1034.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1035.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1036.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1037.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1038.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1039.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1040.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1041.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1042.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1043.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1044.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1045.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1046.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1047.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1048.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1049.root'
)

fileNamesZtoMuTau_InclusivePPmuX_7TeV_part16 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1050.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1051.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1052.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1053.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1054.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1055.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1056.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1057.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1058.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1059.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1060.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1061.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1062.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1063.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1064.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1065.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1066.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1067.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1068.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1069.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1070.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1071.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1072.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1073.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1074.root'
)

fileNamesZtoMuTau_InclusivePPmuX_7TeV_part17 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1075.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1076.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1077.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1078.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1079.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1080.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1081.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1082.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1083.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1084.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1085.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1086.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1087.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1088.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1089.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1090.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1091.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1092.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1093.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1094.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1095.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1096.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1097.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1098.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1099.root'
)

fileNamesZtoMuTau_InclusivePPmuX_7TeV_part18 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1100.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1101.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1102.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1103.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1104.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1105.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1106.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1107.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1108.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1109.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1110.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1111.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1112.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1113.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1114.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1115.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1116.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1117.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1118.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1119.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1120.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1121.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1122.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1123.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1124.root'
)

fileNamesZtoMuTau_InclusivePPmuX_7TeV_part19 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1125.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1126.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1127.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1128.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1129.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1130.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1131.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1132.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1133.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1134.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1135.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1136.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1137.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1138.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1139.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1140.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1141.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1142.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1143.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1144.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1145.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1146.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1147.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1148.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1149.root'
)

fileNamesZtoMuTau_InclusivePPmuX_7TeV_part20 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1150.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1151.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1152.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1153.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1154.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1155.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1156.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1157.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1158.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1159.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1160.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1161.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1162.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1163.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1164.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1165.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1166.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1167.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1168.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1169.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1170.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1171.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1172.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1173.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1174.root'
)

fileNamesZtoMuTau_InclusivePPmuX_7TeV_part21 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1175.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1176.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1177.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1178.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1179.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1180.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1181.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1182.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1183.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1184.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1185.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1186.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1187.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1188.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1189.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1190.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1191.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1192.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1193.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1194.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1195.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1196.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1197.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1198.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1199.root'
)

fileNamesZtoMuTau_InclusivePPmuX_7TeV_part22 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1200.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1201.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1202.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1203.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1204.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1205.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1206.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1207.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1208.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1209.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1210.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1211.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1212.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1213.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1214.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1215.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1216.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1217.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1218.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1219.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1220.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1221.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1222.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1223.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1224.root'
)

fileNamesZtoMuTau_InclusivePPmuX_7TeV_part23 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1225.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1226.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1227.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1228.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1229.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1230.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1231.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1232.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1233.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1234.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1235.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1236.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1237.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1238.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1239.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1240.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1241.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1242.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1243.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1244.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1245.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1246.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1247.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1248.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1249.root'
)

fileNamesZtoMuTau_InclusivePPmuX_7TeV_part24 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1250.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1251.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1252.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1253.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1254.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1255.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1256.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1257.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1258.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1259.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1260.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1261.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1262.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1263.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1264.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1265.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1266.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1267.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1268.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1269.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1270.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1271.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1272.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1273.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1274.root'
)

fileNamesZtoMuTau_InclusivePPmuX_7TeV_part25 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1275.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1276.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1277.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1278.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1279.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1280.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1281.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1282.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1283.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1284.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1285.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1286.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1287.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1288.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1289.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1290.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1291.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1292.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1293.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1294.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1295.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1296.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1297.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1298.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1299.root'
)

fileNamesZtoMuTau_InclusivePPmuX_7TeV_part26 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1300.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1301.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1302.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1303.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1304.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1305.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1306.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1307.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1308.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1309.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1310.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1311.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1312.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1313.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1314.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1315.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1316.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1317.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1318.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1319.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1320.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1321.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1322.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1323.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1324.root'
)

fileNamesZtoMuTau_InclusivePPmuX_7TeV_part27 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1325.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1326.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1327.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1328.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1329.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1330.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1331.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1332.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1333.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1334.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1335.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1336.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1337.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1338.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1339.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1340.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1341.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1342.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1343.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1344.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1345.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1346.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1347.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1348.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1349.root'
)

fileNamesZtoMuTau_InclusivePPmuX_7TeV_part28 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1350.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1351.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1352.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1353.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1354.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1355.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1356.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1357.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1358.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1359.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1360.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1361.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1362.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1363.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1364.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1365.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ppMuX_7TeV/muTauSkim_ppMuX_7TeV_1366.root'
)

genPhaseSpaceCutZtoMuTau_InclusivePPmuX_7TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('ptHat < 20. | leadingGenMuon.pt < 15.')
)

plotsOutputFileNameZtoMuTau_InclusivePPmuX_7TeV = cms.string('plotsZtoMuTau_InclusivePPmuX_7TeV_partXX.root')
patTupleOutputFileNameZtoMuTau_InclusivePPmuX_7TeV = cms.untracked.string('patTupleZtoMuTau_InclusivePPmuX_7TeV_partXX.root')
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# muon enriched QCD sample generated with Pythia (Pt(hat)> 20 GeV && PtMuon > 15 GeV)
#  integrated luminosity = 48.7 pb^-1
# (corrected by estimated scale factor of 1. for missing files)
#
intLumiZtoMuTau_PPmuXptGt20_7TeV = float(48.7)
corrFactorZtoMuTau_PPmuXptGt20_7TeV = float(1.)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_311.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_312.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_313.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_314.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_315.root'
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_316.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_317.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_318.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_319.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_320.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_321.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_322.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_323.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_324.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_325.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_326.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_327.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_328.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_329.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part05 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_330.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_331.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_332.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_333.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_334.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part06 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_335.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_336.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_337.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_338.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_339.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part07 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_340.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_341.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_342.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_343.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_344.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part08 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_345.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_346.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_347.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_348.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_349.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part09 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_350.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_351.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_352.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_353.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_354.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part10 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_355.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_356.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_357.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_358.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_359.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part11 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_360.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_361.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_362.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_363.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_364.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_365.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_366.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_367.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_368.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_369.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part12 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_370.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_371.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_372.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_373.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_374.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part13 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_375.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_376.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_377.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_378.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_379.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part14 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_380.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_381.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_382.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_383.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_384.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part15 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_385.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_386.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_387.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_388.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_389.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part16 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_390.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_391.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_392.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_393.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_394.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part17 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_395.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_396.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_397.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_398.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_399.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part18 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_400.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_401.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_402.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_403.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_404.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part19 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_405.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_406.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_407.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_408.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_409.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part20 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_410.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_411.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_412.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_413.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_414.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part21 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_415.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_416.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_417.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_418.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_419.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part22 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_420.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_421.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_422.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_423.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_424.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part23 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_425.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_426.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_427.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_428.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_429.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part24 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_430.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_431.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_432.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_433.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_434.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part25 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_435.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_436.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_437.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_438.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_439.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part26 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_440.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_441.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_442.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_443.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_444.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part27 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_445.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_446.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_447.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_448.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_449.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part28 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_450.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_451.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_452.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_453.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_454.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part29 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_455.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_456.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_457.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_458.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_459.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part30 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_460.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_461.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_462.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_463.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_464.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part31 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_465.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_466.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_467.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_468.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_469.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part32 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_470.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_471.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_472.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_473.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_474.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part33 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_475.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_476.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_477.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_478.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_479.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part34 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_480.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_481.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_482.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_483.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_484.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part35 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_485.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_486.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_487.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_488.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_489.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part36 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_490.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_491.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_492.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_493.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_494.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part37 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_495.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_496.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_497.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_498.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_499.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part38 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_500.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_501.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_502.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_503.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_504.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part39 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_505.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_506.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_507.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_508.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_509.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part40 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_510.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_511.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_512.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_513.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_514.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part41 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_515.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_516.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_517.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_518.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_519.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part42 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_520.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_521.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_522.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_523.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_524.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part43 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_525.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_526.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_527.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_528.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_529.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part44 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_530.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_531.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_532.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_533.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_534.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part45 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_535.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_536.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_537.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_538.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_539.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part46 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_540.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_541.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_542.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_543.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_544.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part47 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_545.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_546.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_547.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_548.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_549.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part48 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_550.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_551.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_552.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_553.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_554.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part49 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_555.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_556.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_557.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_558.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_559.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part50 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_560.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_561.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_562.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_563.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_564.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part51 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_565.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_566.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_567.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_568.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_569.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part52 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_570.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_571.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_572.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_573.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_574.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part53 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_575.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_576.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_577.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_578.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_579.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part54 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_580.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_581.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_582.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_583.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_584.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part55 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_585.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_586.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_587.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_588.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_589.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part56 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_590.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_591.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_592.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_593.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_594.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part57 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_595.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_596.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_597.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_598.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_599.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part58 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_600.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_601.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_602.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_603.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_604.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part59 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_605.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_606.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_607.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_608.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_609.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part60 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_610.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_611.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_612.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_613.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_614.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part61 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_615.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_616.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_617.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_618.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_619.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part62 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_620.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_621.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_622.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_623.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_624.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part63 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_625.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_626.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_627.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_628.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_629.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part64 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_630.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_631.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_632.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_633.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_634.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part65 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_635.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_636.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_637.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_638.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_639.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part66 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_640.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_641.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_642.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_643.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_644.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part67 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_645.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_646.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_647.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_648.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_649.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part68 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_650.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_651.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_652.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_653.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_654.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part69 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_655.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_656.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_657.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_658.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_659.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part70 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_660.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_661.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_662.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_663.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_664.root'
)

fileNamesZtoMuTau_PPmuXptGt20_7TeV_part71 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_665.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_666.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_667.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_668.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_669.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/inclmu15_7TeV/muTauSkim_inclmu15_7TeV_670.root',
)

genPhaseSpaceCutZtoMuTau_PPmuXptGt20_7TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('ptHat > 20.')
)

plotsOutputFileNameZtoMuTau_PPmuXptGt20_7TeV = cms.string('plotsZtoMuTau_PPmuXptGt20_7TeV_partXX.root')
patTupleOutputFileNameZtoMuTau_PPmuXptGt20_7TeV = cms.untracked.string('patTupleZtoMuTau_PPmuXptGt20_7TeV_partXX.root')
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# TTbar sample generated with Pythia
#
intLumiZtoMuTau_TTplusJets_7TeV = float(6644.9)
corrFactorZtoMuTau_TTplusJets_7TeV = float(1.)

fileNamesZtoMuTau_TTplusJets_7TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_0.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_1.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_2.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_3.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_4.root'
)

fileNamesZtoMuTau_TTplusJets_7TeV_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_5.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_6.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_7.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_8.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_9.root'
)

fileNamesZtoMuTau_TTplusJets_7TeV_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_10.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_11.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_12.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_13.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_14.root'
)

fileNamesZtoMuTau_TTplusJets_7TeV_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_15.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_16.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_17.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_18.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_19.root'
)

fileNamesZtoMuTau_TTplusJets_7TeV_part05 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_20.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_21.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_22.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_23.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_24.root'
)

fileNamesZtoMuTau_TTplusJets_7TeV_part06 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_25.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_26.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_27.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_28.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_29.root'
)    

fileNamesZtoMuTau_TTplusJets_7TeV_part07 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_30.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_31.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_32.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_33.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_34.root'
)

fileNamesZtoMuTau_TTplusJets_7TeV_part08 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_35.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_36.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_37.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_38.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_39.root'
)    

fileNamesZtoMuTau_TTplusJets_7TeV_part09 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_40.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_41.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_42.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_43.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_44.root'
)

fileNamesZtoMuTau_TTplusJets_7TeV_part10 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_45.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_46.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_47.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_48.root',
    'rfio:/castor/cern.ch/user/z/zeise/official/summer09/mutauskim_v1/ttbar_7TeV/muTauSkim_ttbar_7TeV_49.root'
)    

genPhaseSpaceCutZtoMuTau_TTplusJets_7TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

plotsOutputFileNameZtoMuTau_TTplusJets_7TeV = cms.string('plotsZtoMuTau_TTplusJets_7TeV_partXX.root')
patTupleOutputFileNameZtoMuTau_TTplusJets_7TeV = cms.untracked.string('patTupleZtoMuTau_TTplusJets_7TeV_partXX.root')
#--------------------------------------------------------------------------------
