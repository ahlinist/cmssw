import FWCore.ParameterSet.Config as cms
import copy

# define configuration parameters for submission of Z --> mu + tau-jet jobs to CERN batch system
# (running over skimmed samples stored on CASTOR)

intLumiZtoMuTau_Data_10TeV = float(200.)

#--------------------------------------------------------------------------------
# Z --> tau+ tau- sample generated with Pythia + Tauola (all decay modes)
#  integrated luminosity = 1063.9 pb^-1
# (no files missing)
#
intLumiZtoMuTau_Ztautau_10TeV = float(1063.9)
corrFactorZtoMuTau_Ztautau_10TeV = float(1.)

fileNamesZtoMuTau_Ztautau_10TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_1.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_2.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_3.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_4.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_5.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_6.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_7.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_8.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_9.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_10.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_11.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_12.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_13.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_14.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_15.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_16.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_17.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_18.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_19.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_20.root'
)

fileNamesZtoMuTau_Ztautau_10TeV_part02 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_21.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_22.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_23.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_24.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_25.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_26.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_27.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_28.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_29.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_30.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_31.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_32.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_33.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_34.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_35.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_36.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_37.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_38.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_39.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_40.root'
)

fileNamesZtoMuTau_Ztautau_10TeV_part03 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_41.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_42.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_43.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_44.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_45.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_46.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_47.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_48.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_49.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_50.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_51.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_52.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_53.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_54.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_55.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_56.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_57.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_58.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_59.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_60.root'
)

fileNamesZtoMuTau_Ztautau_10TeV_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_61.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_62.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_63.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_64.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_65.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_66.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_67.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_68.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_69.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_70.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_71.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_72.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_73.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_74.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_75.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_76.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_77.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_78.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_79.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_80.root'
)

fileNamesZtoMuTau_Ztautau_10TeV_part05 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_81.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_82.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_83.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_84.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_85.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_86.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_87.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_88.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_89.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_80.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_91.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_92.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_93.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_94.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_95.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_96.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_97.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_98.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_99.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_100.root'
)

fileNamesZtoMuTau_Ztautau_10TeV_part06 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_101.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_102.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_103.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_104.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_105.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_106.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_107.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_108.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_109.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_110.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_111.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_112.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_113.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_114.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_115.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_116.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_117.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_118.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_119.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_120.root'
)

fileNamesZtoMuTau_Ztautau_10TeV_part07 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_121.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_122.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_123.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_124.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_125.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_126.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_127.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_128.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_129.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_130.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_131.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_132.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_133.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_134.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_135.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_136.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_137.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_138.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_139.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_140.root'
)

fileNamesZtoMuTau_Ztautau_10TeV_part08 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_141.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_142.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_143.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_144.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_145.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_146.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_147.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_148.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_149.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_150.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_151.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_152.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_153.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_154.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_155.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_156.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_157.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_158.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_159.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_160.root'
)

fileNamesZtoMuTau_Ztautau_10TeV_part09 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_161.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_162.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_163.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_164.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_165.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_166.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_167.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_168.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_169.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_170.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_171.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_172.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_173.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_174.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_175.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_176.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_177.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_178.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_179.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_180.root'
)

fileNamesZtoMuTau_Ztautau_10TeV_part10 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_181.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_182.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_183.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_184.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_185.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_186.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_187.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_188.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_189.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_190.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_191.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_192.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_193.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_194.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_195.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_196.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_197.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_198.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_199.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_200.root'
)

fileNamesZtoMuTau_Ztautau_10TeV_part11 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_201.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_202.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_203.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_204.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_205.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_206.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_207.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_208.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_209.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_210.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_211.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_212.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_213.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_214.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_215.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_216.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_217.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_218.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_219.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_220.root'
)

fileNamesZtoMuTau_Ztautau_10TeV_part12 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_221.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_222.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_223.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_224.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_225.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_226.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_227.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_228.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_229.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_230.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_231.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_232.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_233.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_234.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_235.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_236.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_237.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_238.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_239.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_240.root'
)

fileNamesZtoMuTau_Ztautau_10TeV_part13 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_241.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_242.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_243.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_244.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_245.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_246.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_247.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_248.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_249.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_250.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_251.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_252.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_253.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_254.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_255.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_256.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_257.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_258.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_259.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_260.root'
)

fileNamesZtoMuTau_Ztautau_10TeV_part14 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_261.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_262.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_263.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_264.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_265.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_266.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_267.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_268.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_269.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_270.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_271.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_272.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_273.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_274.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_275.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_276.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_277.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_278.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_279.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_280.root'
)

fileNamesZtoMuTau_Ztautau_10TeV_part15 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_281.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_282.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_283.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_284.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_285.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_286.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_287.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_288.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_289.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_290.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_291.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_292.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_293.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_294.root'
)

genPhaseSpaceCutZtoMuTau_Ztautau_10TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

plotsOutputFileNameZtoMuTau_Ztautau_10TeV = cms.string('plotsZtoMuTau_Ztautau_10TeV_partXX.root')
patTupleOutputFileNameZtoMuTau_Ztautau_10TeV = cms.untracked.string('patTupleZtoMuTau_Ztautau_10TeV_partXX.root')
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# Z --> mu+ mu- sample generated with Pythia
#  integrated luminosity = 1029.1 pb^-1
# (corrected by scale factor of 1.01 for missing files)
#
intLumiZtoMuTau_Zmumu_10TeV = float(1029.1)
corrFactorZtoMuTau_Zmumu_10TeV = float(1.01)

fileNamesZtoMuTau_Zmumu_10TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_1.root',
    #'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_2.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_3.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_4.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_5.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_6.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_7.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_8.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_9.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_10.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_11.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_12.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_13.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_14.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_15.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_16.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_17.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_18.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_19.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_20.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_21.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_22.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_23.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_24.root'
)

fileNamesZtoMuTau_Zmumu_10TeV_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_25.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_26.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_27.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_28.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_29.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_30.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_31.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_32.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_33.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_34.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_35.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_36.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_37.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_38.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_39.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_40.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_41.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_42.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_43.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_44.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_45.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_46.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_47.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_48.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_49.root'
)

fileNamesZtoMuTau_Zmumu_10TeV_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_50.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_51.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_52.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_53.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_55.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_56.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_57.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_58.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_59.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_60.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_61.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_62.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_63.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_64.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_65.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_66.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_68.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_69.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_70.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_71.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_72.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_73.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_74.root'
)

fileNamesZtoMuTau_Zmumu_10TeV_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_75.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_76.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_77.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_79.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_80.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_81.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_82.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_83.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_84.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_85.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_86.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_87.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_88.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_89.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_90.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_91.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_92.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_93.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_94.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_95.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_96.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_97.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_98.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_99.root'
)

fileNamesZtoMuTau_Zmumu_10TeV_part05 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_100.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_101.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_102.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_103.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_105.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_106.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_107.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_108.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_109.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_110.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_111.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_112.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_113.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_114.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_115.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_116.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_117.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_118.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_119.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_120.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_122.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_123.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_124.root'
)

fileNamesZtoMuTau_Zmumu_10TeV_part06 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_125.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_126.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_127.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_128.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_129.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_130.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_131.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_132.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_133.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_134.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_135.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_136.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_137.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_138.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_139.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_140.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_141.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_142.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_143.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_144.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_145.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_146.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_147.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_148.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_149.root'
)

fileNamesZtoMuTau_Zmumu_10TeV_part07 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_150.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_151.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_152.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_153.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_154.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_155.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_156.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_157.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_158.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_159.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_160.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_161.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_162.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_163.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_164.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_165.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_166.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_167.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_168.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_169.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_170.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_171.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_172.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_173.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_174.root'
)

fileNamesZtoMuTau_Zmumu_10TeV_part08 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_175.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_176.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_177.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_178.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_179.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_180.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_181.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_182.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_183.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_184.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_185.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_186.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_187.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_188.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_189.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_190.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_191.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_192.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_193.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_194.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_195.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_196.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_197.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_198.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_199.root'
)

fileNamesZtoMuTau_Zmumu_10TeV_part09 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_200.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_201.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_202.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_203.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_204.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_205.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_206.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_207.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_208.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_209.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_210.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_211.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_212.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_213.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_214.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_215.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_216.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_217.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_218.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_219.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_220.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_221.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_222.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_223.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_224.root'
)

fileNamesZtoMuTau_Zmumu_10TeV_part10 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_225.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_226.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_227.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_228.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_229.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_230.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_231.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_232.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_233.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_234.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_235.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_236.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_237.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_238.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_239.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_240.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_241.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_242.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_243.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_244.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_245.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_246.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_247.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_248.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_249.root'
)

fileNamesZtoMuTau_Zmumu_10TeV_part11 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_250.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_251.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_252.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_253.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_254.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_255.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_256.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_257.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_258.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_259.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_260.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_261.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_262.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_263.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_264.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_265.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_266.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_267.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_268.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_269.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_270.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_271.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_272.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_273.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_274.root'
)

fileNamesZtoMuTau_Zmumu_10TeV_part12 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_275.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_276.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_277.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_278.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_279.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_280.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_281.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_282.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_283.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_284.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_285.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_286.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_287.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_288.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_289.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_290.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_291.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_292.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_293.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_294.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_295.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_296.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_297.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_298.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_299.root'
)

fileNamesZtoMuTau_Zmumu_10TeV_part13 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_300.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_301.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_302.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_303.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_304.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_305.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_306.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_307.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_308.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_309.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_310.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_311.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_312.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_313.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_314.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_315.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_316.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_317.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_318.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_319.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_320.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_321.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_322.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_323.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_324.root'
)

fileNamesZtoMuTau_Zmumu_10TeV_part14 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_325.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_326.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_327.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_328.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_329.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_330.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_331.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_332.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_333.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_334.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_335.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_336.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_337.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_338.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_339.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_340.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_341.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_342.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_343.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_344.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_345.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_346.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_347.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_348.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_349.root'
)

fileNamesZtoMuTau_Zmumu_10TeV_part15 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_350.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_351.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_352.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_353.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_354.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_355.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_356.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_357.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_358.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_359.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_360.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_361.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_362.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_363.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_364.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_365.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_366.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_367.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_368.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_369.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_370.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_371.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_372.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_373.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_374.root'
)

fileNamesZtoMuTau_Zmumu_10TeV_part16 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_375.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_376.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_377.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_378.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_379.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_380.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_381.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_382.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_383.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_384.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_385.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_386.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_387.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_388.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_389.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_390.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_391.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_392.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_393.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_394.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_395.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_396.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_397.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_398.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_399.root'
)

fileNamesZtoMuTau_Zmumu_10TeV_part17 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_400.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_401.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_402.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_403.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_404.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_405.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_406.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_407.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_408.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_409.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_410.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_411.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_412.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_413.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZmumuSkimMT314_1/muTauSkim_414.root'
)

genPhaseSpaceCutZtoMuTau_Zmumu_10TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

plotsOutputFileNameZtoMuTau_Zmumu_10TeV = cms.string('plotsZtoMuTau_Zmumu_10TeV_partXX.root')
patTupleOutputFileNameZtoMuTau_Zmumu_10TeV = cms.untracked.string('patTupleZtoMuTau_Zmumu_10TeV_partXX.root')
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# W + jets sample generated with Madgraph
#  integrated luminosity = 367.5 pb^-1
# (corrected by scale factor of 1.31 for missing files)
#
# (NOTE: for Monte Carlo samples generated by Madgraph,
#        the filter efficiency is already included in the cross-sections
#        listed at https://twiki.cern.ch/twiki/bin/view/CMS/ProductionSummer2009#MadGraph !!)
#
intLumiZtoMuTau_WplusJets_10TeV = float(367.5)
corrFactorZtoMuTau_WplusJets_10TeV = float(1.31)

fileNamesZtoMuTau_WplusJets_10TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_1.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_2.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_3.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_4.root'
)

fileNamesZtoMuTau_WplusJets_10TeV_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_7.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_8.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_9.root'
)

fileNamesZtoMuTau_WplusJets_10TeV_part03 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_10.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_13.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_14.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_15.root'
)

fileNamesZtoMuTau_WplusJets_10TeV_part04 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_16.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_19.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_20.root'
)

fileNamesZtoMuTau_WplusJets_10TeV_part05 = cms.untracked.vstring(    
    #'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_21.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_22.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_23.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_24.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_25.root'
)

fileNamesZtoMuTau_WplusJets_10TeV_part06 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_26.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_29.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_30.root'
)

fileNamesZtoMuTau_WplusJets_10TeV_part07 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_31.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_32.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_33.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_34.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_35.root'
)

fileNamesZtoMuTau_WplusJets_10TeV_part08 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_36.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_37.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_38.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_39.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_40.root'
)

fileNamesZtoMuTau_WplusJets_10TeV_part09 = cms.untracked.vstring(    
    #'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_41.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_42.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_43.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_44.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_45.root'
)

fileNamesZtoMuTau_WplusJets_10TeV_part10 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_46.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_47.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_48.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_49.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_50.root'
)

fileNamesZtoMuTau_WplusJets_10TeV_part11 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_51.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_53.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_55.root'
)

fileNamesZtoMuTau_WplusJets_10TeV_part12 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_58.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_59.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_60.root'
)

fileNamesZtoMuTau_WplusJets_10TeV_part13 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_61.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_62.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_63.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_64.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_65.root'
)

fileNamesZtoMuTau_WplusJets_10TeV_part14 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_66.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_67.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_68.root'
)

fileNamesZtoMuTau_WplusJets_10TeV_part15 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_72.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_73.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_74.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_75.root'
)

fileNamesZtoMuTau_WplusJets_10TeV_part16 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_76.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_77.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_78.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_79.root'
)

fileNamesZtoMuTau_WplusJets_10TeV_part17 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_82.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_83.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_84.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_85.root'
)

fileNamesZtoMuTau_WplusJets_10TeV_part18 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_86.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_87.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_88.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_89.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_90.root'
)

fileNamesZtoMuTau_WplusJets_10TeV_part19 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_91.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_92.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_93.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_94.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_95.root'
)

fileNamesZtoMuTau_WplusJets_10TeV_part20 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_97.root'
)

fileNamesZtoMuTau_WplusJets_10TeV_part21 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_101.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_103.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_104.root'
)

fileNamesZtoMuTau_WplusJets_10TeV_part22 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_108.root'
)

fileNamesZtoMuTau_WplusJets_10TeV_part23 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_111.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_112.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_113.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_114.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_115.root'
)

fileNamesZtoMuTau_WplusJets_10TeV_part24 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_116.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_117.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_118.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_119.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_120.root'
)

fileNamesZtoMuTau_WplusJets_10TeV_part25 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_121.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_122.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_123.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_124.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_125.root'
)

fileNamesZtoMuTau_WplusJets_10TeV_part26 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_126.root',
    'rfio:/castor/cern.ch/user/v/veelken/SkimOctober09/WplusJetsSkimMT314/muTauSkim_127.root'
)

genPhaseSpaceCutZtoMuTau_WplusJets_10TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

plotsOutputFileNameZtoMuTau_WplusJets_10TeV = cms.string('plotsZtoMuTau_WplusJets_10TeV_partXX.root')
patTupleOutputFileNameZtoMuTau_WplusJets_10TeV = cms.untracked.string('patTupleZtoMuTau_WplusJets_10TeV_partXX.root')
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# muon enriched QCD sample generated with Pythia (no cut on Pt(hat))
#  integrated luminosity = 0.0851 pb^-1
# (no files missing)
#
intLumiZtoMuTau_InclusivePPmuX_10TeV = float(0.0851)
corrFactorZtoMuTau_InclusivePPmuX_10TeV = float(1.)

fileNamesZtoMuTau_InclusivePPmuX_10TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_1.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_2.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_3.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_4.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_5.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_6.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_7.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_8.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_9.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_10.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_11.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_12.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_13.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_14.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_15.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_16.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_17.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_18.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_19.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_20.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_21.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_22.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_23.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_24.root'
)

fileNamesZtoMuTau_InclusivePPmuX_10TeV_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_25.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_26.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_27.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_28.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_29.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_30.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_31.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_32.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_33.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_34.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_35.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_36.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_37.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_38.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_39.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_40.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_41.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_42.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_43.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_44.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_45.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_46.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_47.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_48.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_49.root'
)

fileNamesZtoMuTau_InclusivePPmuX_10TeV_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_50.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_51.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_52.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_53.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_54.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_55.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_56.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_57.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_58.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_59.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_60.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_61.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_62.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_63.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_64.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_65.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_66.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_67.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_68.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_69.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_70.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_71.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_72.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_73.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_74.root'
)

fileNamesZtoMuTau_InclusivePPmuX_10TeV_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_75.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_76.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_77.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_78.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_79.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_80.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_81.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_82.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_83.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_84.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_85.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_86.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_87.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_88.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_89.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_90.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_91.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_92.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_93.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_94.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_95.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_96.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_97.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_98.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_99.root'
)

fileNamesZtoMuTau_InclusivePPmuX_10TeV_part05 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_100.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_101.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_102.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_103.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_104.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_105.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_106.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_107.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_108.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_109.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_110.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_111.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_112.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_113.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_114.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_115.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_116.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_117.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_118.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_119.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_120.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_121.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_122.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_123.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_124.root'
)

fileNamesZtoMuTau_InclusivePPmuX_10TeV_part06 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_125.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_126.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_127.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_128.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_129.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_130.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_131.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_132.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_133.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_134.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_135.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_136.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_137.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_138.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_139.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_140.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_141.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_142.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_143.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_144.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_145.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_146.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_147.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_148.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_149.root'
)

fileNamesZtoMuTau_InclusivePPmuX_10TeV_part07 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_150.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_151.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_152.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_153.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_154.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_155.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_156.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_157.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_158.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_159.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_160.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_161.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_162.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_163.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_164.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_165.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_166.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_167.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_168.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_169.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_170.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_171.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_172.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_173.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_174.root'
)

fileNamesZtoMuTau_InclusivePPmuX_10TeV_part08 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_175.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_176.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_177.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_178.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_179.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_180.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_181.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_182.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_183.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_184.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_185.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_186.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_187.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_188.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_189.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_190.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_191.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_192.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_193.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_194.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_195.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_196.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_197.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_198.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_199.root'
)

fileNamesZtoMuTau_InclusivePPmuX_10TeV_part09 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_200.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_201.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_202.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_203.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_204.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_205.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_206.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_207.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_208.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_209.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_210.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_211.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_212.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_213.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_214.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_215.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_216.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_217.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_218.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_219.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_220.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_221.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_222.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_223.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_224.root'
)

fileNamesZtoMuTau_InclusivePPmuX_10TeV_part10 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_225.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_226.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_227.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_228.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_229.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_230.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_231.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_232.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_233.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_234.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_235.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_236.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_237.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_238.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_239.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_240.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_241.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_242.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_243.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_244.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_245.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_246.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_247.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_248.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_249.root'
)

fileNamesZtoMuTau_InclusivePPmuX_10TeV_part11 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_250.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_251.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_252.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_253.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_254.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_255.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_256.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_257.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_258.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_259.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_260.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_261.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_262.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_263.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_264.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_265.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_266.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_267.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_268.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_269.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_270.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_271.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_272.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_273.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_274.root'
)

fileNamesZtoMuTau_InclusivePPmuX_10TeV_part12 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_275.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_276.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_277.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_278.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_279.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_280.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_281.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_282.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_283.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_284.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_285.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_286.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_287.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_288.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_289.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_290.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_291.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_292.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_293.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_294.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_295.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_296.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_297.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_298.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_299.root'
)

fileNamesZtoMuTau_InclusivePPmuX_10TeV_part13 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_300.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_301.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_302.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_303.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_304.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_305.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_306.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_307.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_308.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_309.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_310.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_311.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_312.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_313.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_314.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_315.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_316.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_317.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_318.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_319.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_320.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_321.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_322.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_323.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_324.root'
)

fileNamesZtoMuTau_InclusivePPmuX_10TeV_part14 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_325.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_326.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_327.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_328.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_329.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_330.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_331.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_332.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_333.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_334.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_335.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_336.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_337.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_338.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_339.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_340.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_341.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_342.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_343.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_344.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_345.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_346.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_347.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_348.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_349.root'
)

fileNamesZtoMuTau_InclusivePPmuX_10TeV_part15 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_350.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_351.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_352.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_353.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_354.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_355.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_356.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_357.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_358.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_359.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_360.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_361.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_362.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_363.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_364.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_365.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_366.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_367.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_368.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_369.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_370.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_371.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_372.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_373.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_374.root'
)

fileNamesZtoMuTau_InclusivePPmuX_10TeV_part16 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_375.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_376.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_377.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_378.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_379.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_380.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_381.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_382.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_383.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_384.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_385.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_386.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_387.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_388.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_389.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_390.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_391.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_392.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_393.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_394.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_395.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_396.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_397.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_398.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_399.root'
)

fileNamesZtoMuTau_InclusivePPmuX_10TeV_part17 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_400.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_401.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_402.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_403.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_404.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_405.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_406.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_407.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_408.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_409.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_410.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_411.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_412.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_413.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_414.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_415.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_416.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_417.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_418.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_419.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_420.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_421.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_422.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_423.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_424.root'
)

fileNamesZtoMuTau_InclusivePPmuX_10TeV_part18 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_425.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_426.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_427.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_428.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_429.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_430.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_431.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_432.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_433.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_434.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_435.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_436.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_437.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_438.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_439.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_440.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_441.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_442.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_443.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_444.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_445.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_446.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_447.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_448.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_449.root'
)

fileNamesZtoMuTau_InclusivePPmuX_10TeV_part19 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_450.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_451.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_452.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_453.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_454.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_455.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_456.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_457.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_458.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_459.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_460.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_461.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_462.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_463.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_464.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_465.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_466.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_467.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_468.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_469.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_470.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_471.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_472.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_473.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_474.root'
)

fileNamesZtoMuTau_InclusivePPmuX_10TeV_part20 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_475.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_476.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_477.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_478.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_479.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_480.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_481.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_482.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_483.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_484.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_485.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_486.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_487.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_488.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_489.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_490.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_491.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_492.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_493.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_494.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_495.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_496.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_497.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_498.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_499.root'
)

fileNamesZtoMuTau_InclusivePPmuX_10TeV_part21 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_500.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_501.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_502.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_503.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_504.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_505.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_506.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_507.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_508.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_509.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_510.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_511.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_512.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_513.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/InclusivePPmuXSkimMT314_1/muTauSkim_514.root'
)

genPhaseSpaceCutZtoMuTau_InclusivePPmuX_10TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('ptHat < 20. | leadingGenMuon.pt < 15.')
)

plotsOutputFileNameZtoMuTau_InclusivePPmuX_10TeV = cms.string('plotsZtoMuTau_InclusivePPmuX_10TeV_partXX.root')
patTupleOutputFileNameZtoMuTau_InclusivePPmuX_10TeV = cms.untracked.string('patTupleZtoMuTau_InclusivePPmuX_10TeV_partXX.root')
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# muon enriched QCD sample generated with Pythia (Pt(hat)> 20 GeV && PtMuon > 15 GeV)
#  integrated luminosity = 44.8 pb^-1
# (corrected for missing files by estimated scale factor of 1.03)
#
intLumiZtoMuTau_PPmuXptGt20_10TeV = float(44.8)
corrFactorZtoMuTau_PPmuXptGt20_10TeV = float(1.03)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_1.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_2.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_3.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_4.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_5.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_6.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_7.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_8.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_9.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part03 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_10.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_11.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_12.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_13.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_14.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_15.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_16.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_17.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_18.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_19.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part05 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_20.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_21.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_22.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_23.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_24.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part06 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_25.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_26.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_27.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_28.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_29.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part07 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_30.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_31.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_32.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_33.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_34.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part08 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_35.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_36.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_37.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_38.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_39.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part09 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_40.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_41.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_42.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_43.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_44.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part10 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_45.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_46.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_47.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_48.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_49.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part11 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_50.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_51.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_52.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_53.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_54.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part12 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_55.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_56.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_57.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_58.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_59.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part13 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_60.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_61.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_62.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_63.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part14 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_65.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_66.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_67.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_68.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_69.root'
 )

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part15 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_70.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_71.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_72.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_73.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_74.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part16 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_75.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_76.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_77.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_78.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_79.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part17 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_80.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_81.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_82.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_83.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_84.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part18 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_85.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_86.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_87.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_88.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_89.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part19 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_90.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_91.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_92.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_93.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_94.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part20 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_95.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_96.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_97.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_98.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_99.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part21 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_100.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_101.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_102.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_103.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_104.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part22 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_105.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_106.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_107.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_108.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_109.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part23 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_110.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_111.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_112.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_113.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_114.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part24 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_115.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_116.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_117.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_118.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_119.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part25 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_120.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_121.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_122.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_123.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_124.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part26 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_125.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_126.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_127.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_128.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_129.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part27 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_130.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_131.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_132.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_133.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_134.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part28 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_135.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_136.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_137.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_138.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_139.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part29 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_140.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_141.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_142.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_143.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_144.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part30 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_145.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_146.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_147.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_148.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_149.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part31 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_150.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_151.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_152.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_153.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_154.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part32 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_155.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_156.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_157.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_158.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_159.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part33 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_160.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_161.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_162.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_163.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_164.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part34 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_165.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_166.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_167.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_168.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_169.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part35 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_170.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_171.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_172.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_173.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_174.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part36 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_175.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_177.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_178.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_179.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part37 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_180.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_181.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_182.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_183.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_184.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part38 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_185.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_186.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_187.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_188.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_189.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part39 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_190.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_191.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_192.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_193.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_194.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part40 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_195.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_196.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_197.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_198.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_199.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part41 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_200.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_201.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_202.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_203.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_204.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part42 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_205.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_206.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_207.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_208.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_209.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part43 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_210.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_211.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_212.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_213.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_214.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part44 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_215.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_216.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_217.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_218.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_219.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part45 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_220.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_221.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_222.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_223.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_224.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part46 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_225.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_226.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_227.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_228.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_229.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part47 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_230.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_231.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_232.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_233.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_234.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part48 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_235.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_236.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_237.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_238.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_239.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part49 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_240.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_241.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_242.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_243.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_244.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part50 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_245.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_246.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_247.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_248.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_249.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part51 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_250.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_251.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_252.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_253.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_254.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part52 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_255.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_256.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_257.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_258.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_259.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part53 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_260.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_261.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_262.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_263.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_264.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part54 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_265.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_266.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_267.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_268.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_269.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part55 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_271.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_272.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_273.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_274.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part56 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_275.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_276.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_277.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_278.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_279.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part57 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_280.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_281.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_282.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_283.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_284.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part58 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_285.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_286.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_287.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_288.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_289.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part59 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_291.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_292.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_293.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_294.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part60 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_295.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_296.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_297.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_298.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_299.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part61 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_300.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_301.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_302.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_304.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part62 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_305.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_306.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_307.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_308.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_309.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part63 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_310.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_311.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_312.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_313.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_314.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part64 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_315.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_316.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_317.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_318.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_319.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part65 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_320.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_321.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_322.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_323.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_324.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part66 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_325.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_326.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_327.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_328.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_329.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part67 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_330.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_331.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_332.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_333.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_334.root'
)

fileNamesZtoMuTau_PPmuXptGt20_10TeV_part68 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_335.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_336.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_337.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_338.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/IncMu15QCDSkimMT314_1/muTauSkim_339.root'
)

genPhaseSpaceCutZtoMuTau_PPmuXptGt20_10TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

plotsOutputFileNameZtoMuTau_PPmuXptGt20_10TeV = cms.string('plotsZtoMuTau_PPmuXptGt20_10TeV_partXX.root')
patTupleOutputFileNameZtoMuTau_PPmuXptGt20_10TeV = cms.untracked.string('patTupleZtoMuTau_PPmuXptGt20_10TeV_partXX.root')
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# ttbar + jets sample generated with Madgraph
#  integrated luminosity = 2181.1 pb^-1
# (corrected by scale factor of 1.18 for missing files)
#
intLumiZtoMuTau_TTplusJets_10TeV = float(2181.1)
corrFactorZtoMuTau_TTplusJets_10TeV = float(1.18)

fileNamesZtoMuTau_TTplusJets_10TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/TTbarSkimMT314_1/muTauSkim_1.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/TTbarSkimMT314_1/muTauSkim_2.root'
)

fileNamesZtoMuTau_TTplusJets_10TeV_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/TTbarSkimMT314_1/muTauSkim_3.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/TTbarSkimMT314_1/muTauSkim_4.root'
)

fileNamesZtoMuTau_TTplusJets_10TeV_part03 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/TTbarSkimMT314_1/muTauSkim_5.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/TTbarSkimMT314_1/muTauSkim_6.root'
)

fileNamesZtoMuTau_TTplusJets_10TeV_part04 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/TTbarSkimMT314_1/muTauSkim_7.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/TTbarSkimMT314_1/muTauSkim_8.root'
)

fileNamesZtoMuTau_TTplusJets_10TeV_part05 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/TTbarSkimMT314_1/muTauSkim_9.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/TTbarSkimMT314_1/muTauSkim_10.root'
)

fileNamesZtoMuTau_TTplusJets_10TeV_part06 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/TTbarSkimMT314_1/muTauSkim_11.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/TTbarSkimMT314_1/muTauSkim_12.root'
 )

fileNamesZtoMuTau_TTplusJets_10TeV_part07 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/TTbarSkimMT314_1/muTauSkim_13.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/TTbarSkimMT314_1/muTauSkim_14.root'
 )

fileNamesZtoMuTau_TTplusJets_10TeV_part08 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/TTbarSkimMT314_1/muTauSkim_15.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/TTbarSkimMT314_1/muTauSkim_16.root'
)

fileNamesZtoMuTau_TTplusJets_10TeV_part09 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/TTbarSkimMT314_1/muTauSkim_17.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/TTbarSkimMT314_1/muTauSkim_18.root'
)

fileNamesZtoMuTau_TTplusJets_10TeV_part10 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/TTbarSkimMT314_1/muTauSkim_19.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/TTbarSkimMT314_1/muTauSkim_20.root'
)

fileNamesZtoMuTau_TTplusJets_10TeV_part11 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/TTbarSkimMT314_1/muTauSkim_21.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/TTbarSkimMT314_1/muTauSkim_22.root'
)

fileNamesZtoMuTau_TTplusJets_10TeV_part12 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/TTbarSkimMT314_1/muTauSkim_23.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/TTbarSkimMT314_1/muTauSkim_24.root'
)

fileNamesZtoMuTau_TTplusJets_10TeV_part13 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/TTbarSkimMT314_1/muTauSkim_25.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/TTbarSkimMT314_1/muTauSkim_26.root'
)

fileNamesZtoMuTau_TTplusJets_10TeV_part14 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/TTbarSkimMT314_1/muTauSkim_27.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/TTbarSkimMT314_1/muTauSkim_28.root'
)

fileNamesZtoMuTau_TTplusJets_10TeV_part15 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/TTbarSkimMT314_1/muTauSkim_29.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/TTbarSkimMT314_1/muTauSkim_30.root'
)

fileNamesZtoMuTau_TTplusJets_10TeV_part16 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/TTbarSkimMT314_1/muTauSkim_31.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/TTbarSkimMT314_1/muTauSkim_32.root'
)

fileNamesZtoMuTau_TTplusJets_10TeV_part17 = cms.untracked.vstring(    
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/TTbarSkimMT314_1/muTauSkim_33.root',
    'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/TTbarSkimMT314_1/muTauSkim_34.root'
)

genPhaseSpaceCutZtoMuTau_TTplusJets_10TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

plotsOutputFileNameZtoMuTau_TTplusJets_10TeV = cms.string('plotsZtoMuTau_TTplusJets_10TeV_partXX.root')
patTupleOutputFileNameZtoMuTau_TTplusJets_10TeV = cms.untracked.string('patTupleZtoMuTau_TTplusJets_10TeV_partXX.root')
#--------------------------------------------------------------------------------
