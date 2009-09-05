import FWCore.ParameterSet.Config as cms
import copy

intLumiData = float(200.)

#--------------------------------------------------------------------------------
#Grouping done so that each group contains < 20 files and 20-50k events

#W --> tau nu skimmed sample (in total 196531 events)
intLumiWtaunu = float(91.97) # NrDBS/sigma_WTauNu = 1100000/11960
corrFactorWtaunu = float(1.065) #1100000/1032590

fileNamesWtaunu_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WtaunuSkim_PFCaloTauMet/WtaunuSkim_1.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WtaunuSkim_PFCaloTauMet/WtaunuSkim_2.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WtaunuSkim_PFCaloTauMet/WtaunuSkim_3.root',
    )
fileNamesWtaunu_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WtaunuSkim_PFCaloTauMet/WtaunuSkim_4.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WtaunuSkim_PFCaloTauMet/WtaunuSkim_5.root',
    )
fileNamesWtaunu_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WtaunuSkim_PFCaloTauMet/WtaunuSkim_6.root',    
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WtaunuSkim_PFCaloTauMet/WtaunuSkim_7.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WtaunuSkim_PFCaloTauMet/WtaunuSkim_8.root',
    )
fileNamesWtaunu_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WtaunuSkim_PFCaloTauMet/WtaunuSkim_9.root', 
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WtaunuSkim_PFCaloTauMet/WtaunuSkim_10.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WtaunuSkim_PFCaloTauMet/WtaunuSkim_11.root'
    )

plotsOutputFileNameWtaunu = cms.string('plotsWtoTauNu_Wtaunu_partXX.root')
patTupleOutputFileNameWtaunu = cms.string('patTupleWtoTauNu_Wtaunu_partXX.root')

#------------------------------------------------------------------------------------
# qcd skimmed sample (in total 1974949 events)
intLumiqcd_W = float(0.137) # (6729821/0.03375)/1.457E9
corrFactorqcd_W = float(1.) #ok
fileNamesqcd_W_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_1.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_2.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_3.root'
)
fileNamesqcd_W_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_4.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_5.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_6.root'
)
fileNamesqcd_W_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_7.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_8.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_9.root'
)
fileNamesqcd_W_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_10.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_11.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_12.root'
)
fileNamesqcd_W_part05 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_13.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_14.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_15.root'
)
fileNamesqcd_W_part06 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_16.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_17.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_18.root'
)
fileNamesqcd_W_part07 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_19.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_20.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_21.root'
)
fileNamesqcd_W_part08 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_22.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_23.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_24.root'
)
fileNamesqcd_W_part09 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_25.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_26.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_27.root'
)
fileNamesqcd_W_part10 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_28.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_29.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_30.root'
)
fileNamesqcd_W_part11 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_31.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_32.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_33.root'
)
fileNamesqcd_W_part12 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_34.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_35.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_36.root'
)
fileNamesqcd_W_part13 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_37.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_38.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_39.root'
)
fileNamesqcd_W_part14 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_40.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_41.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_42.root'
)
fileNamesqcd_W_part15 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_43.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_44.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_45.root'
)
fileNamesqcd_W_part16 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_46.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_47.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_48.root'
)
fileNamesqcd_W_part17 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_49.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_50.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_51.root'
)
fileNamesqcd_W_part18 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_52.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_53.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_54.root'
)
fileNamesqcd_W_part19 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_55.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_56.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_57.root'
)
fileNamesqcd_W_part20 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_58.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_59.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_60.root'
)
fileNamesqcd_W_part21 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_61.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_62.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_63.root'
)
fileNamesqcd_W_part22 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_64.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_65.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_66.root'
)
fileNamesqcd_W_part23 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_67.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_68.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_69.root'
)
fileNamesqcd_W_part24 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_70.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_71.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_72.root'
)
fileNamesqcd_W_part25 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_73.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_74.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_75.root'
)
fileNamesqcd_W_part26 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_76.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_77.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_78.root'
)
fileNamesqcd_W_part27 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_79.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_80.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_81.root'
)
fileNamesqcd_W_part28 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_82.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_83.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_84.root'
)
fileNamesqcd_W_part29 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_85.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_86.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_87.root'
)
fileNamesqcd_W_part30 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_88.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_89.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_90.root'
)
fileNamesqcd_W_part31 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_91.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_92.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_93.root'
)
fileNamesqcd_W_part32 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_94.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_95.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_96.root'
)
fileNamesqcd_W_part33 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_97.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_98.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/qcdSkim_PFCaloTauMet/qcdSkim_99.root'
)

plotsOutputFileNameqcd_W = cms.string('plotsWtoTauNu_qcd_W_partXX.root')
patTupleOutputFileNameqcd_W = cms.string('patTupleWtoTauNu_qcd_W_partXX.root')

#--------------------------------------------------------------------------------------
# W --> mu + nu skimmed sample (in total 887237 events)
intLumiWmunu = float(92.5) #1098386/11865
corrFactorWmunu = float(1.077) # 1098386/1019686

fileNamesWmunu_part01 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WmunuSkim_PFCaloTauMet/WmunuSkim_1.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WmunuSkim_PFCaloTauMet/WmunuSkim_2.root'
    )
fileNamesWmunu_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WmunuSkim_PFCaloTauMet/WmunuSkim_3.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WmunuSkim_PFCaloTauMet/WmunuSkim_4.root'
    )
fileNamesWmunu_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WmunuSkim_PFCaloTauMet/WmunuSkim_5.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WmunuSkim_PFCaloTauMet/WmunuSkim_6.root'
    )
fileNamesWmunu_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WmunuSkim_PFCaloTauMet/WmunuSkim_7.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WmunuSkim_PFCaloTauMet/WmunuSkim_8.root'
    )
fileNamesWmunu_part05 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WmunuSkim_PFCaloTauMet/WmunuSkim_9.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WmunuSkim_PFCaloTauMet/WmunuSkim_10.root'
    )
fileNamesWmunu_part06 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WmunuSkim_PFCaloTauMet/WmunuSkim_11.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WmunuSkim_PFCaloTauMet/WmunuSkim_12.root'
    )
fileNamesWmunu_part07 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WmunuSkim_PFCaloTauMet/WmunuSkim_13.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WmunuSkim_PFCaloTauMet/WmunuSkim_14.root'
    )
fileNamesWmunu_part08 = cms.untracked.vstring(     
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WmunuSkim_PFCaloTauMet/WmunuSkim_15.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WmunuSkim_PFCaloTauMet/WmunuSkim_16.root'
    )
fileNamesWmunu_part09 = cms.untracked.vstring(     
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WmunuSkim_PFCaloTauMet/WmunuSkim_17.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WmunuSkim_PFCaloTauMet/WmunuSkim_18.root'
    )
fileNamesWmunu_part10 = cms.untracked.vstring(     
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WmunuSkim_PFCaloTauMet/WmunuSkim_19.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WmunuSkim_PFCaloTauMet/WmunuSkim_20.root'
    )
fileNamesWmunu_part11 = cms.untracked.vstring(     
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WmunuSkim_PFCaloTauMet/WmunuSkim_21.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WmunuSkim_PFCaloTauMet/WmunuSkim_22.root'
    )
fileNamesWmunu_part12 = cms.untracked.vstring(     
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WmunuSkim_PFCaloTauMet/WmunuSkim_23.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WmunuSkim_PFCaloTauMet/WmunuSkim_24.root'
    )
fileNamesWmunu_part13 = cms.untracked.vstring(     
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WmunuSkim_PFCaloTauMet/WmunuSkim_25.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WmunuSkim_PFCaloTauMet/WmunuSkim_26.root'
    )

plotsOutputFileNameWmunu = cms.string('plotsWtoTauNu_Wmunu_partXX.root')
patTupleOutputFileNameWmunu = cms.string('patTupleWtoTauNu_Wmunu_partXX.root')
#--------------------------------------------------------------------------------
# W --> e + nu skimmed sample (in total 733142 events)
intLumiWenu = float(93.8) #1112967./11865
corrFactorWenu = float(1.02) #1112967/1090709

fileNamesWenu_part01 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WenuSkim_PFCaloTauMet/WenuSkim_1.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WenuSkim_PFCaloTauMet/WenuSkim_2.root'
    )
fileNamesWenu_part02 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WenuSkim_PFCaloTauMet/WenuSkim_3.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WenuSkim_PFCaloTauMet/WenuSkim_4.root'
    )
fileNamesWenu_part03 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WenuSkim_PFCaloTauMet/WenuSkim_5.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WenuSkim_PFCaloTauMet/WenuSkim_6.root'
    )
fileNamesWenu_part04 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WenuSkim_PFCaloTauMet/WenuSkim_7.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WenuSkim_PFCaloTauMet/WenuSkim_8.root'
    )
fileNamesWenu_part05 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WenuSkim_PFCaloTauMet/WenuSkim_9.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WenuSkim_PFCaloTauMet/WenuSkim_10.root'
    )
fileNamesWenu_part06 = cms.untracked.vstring(     
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WenuSkim_PFCaloTauMet/WenuSkim_11.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WenuSkim_PFCaloTauMet/WenuSkim_12.root'
    )
fileNamesWenu_part07 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WenuSkim_PFCaloTauMet/WenuSkim_13.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WenuSkim_PFCaloTauMet/WenuSkim_14.root'
    )
fileNamesWenu_part08 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WenuSkim_PFCaloTauMet/WenuSkim_15.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WenuSkim_PFCaloTauMet/WenuSkim_16.root'
    )
fileNamesWenu_part09 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WenuSkim_PFCaloTauMet/WenuSkim_17.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WenuSkim_PFCaloTauMet/WenuSkim_18.root'
    )
fileNamesWenu_part10 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WenuSkim_PFCaloTauMet/WenuSkim_19.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WenuSkim_PFCaloTauMet/WenuSkim_20.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WenuSkim_PFCaloTauMet/WenuSkim_21.root'
    )

plotsOutputFileNameWenu = cms.string('plotsWtoTauNu_Wenu_partXX.root')
patTupleOutputFileNameWenu = cms.string('patTupleWtoTauNu_Wenu_partXX.root')
#--------------------------------------------------------------------------------
# Z + jets skimmed sample (530107 events in total)
intLumiZplusJets = float(341.) #1262816./37000
corrFactorZplusJets = float(1.) #1262816/1262816

fileNamesZplusJets_part01 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/ZjetsSkim_PFCaloTauMet/zjetsSkim_1.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/ZjetsSkim_PFCaloTauMet/zjetsSkim_2.root'
    )
fileNamesZplusJets_part02 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/ZjetsSkim_PFCaloTauMet/zjetsSkim_3.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/ZjetsSkim_PFCaloTauMet/zjetsSkim_4.root'
    )
fileNamesZplusJets_part03 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/ZjetsSkim_PFCaloTauMet/zjetsSkim_5.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/ZjetsSkim_PFCaloTauMet/zjetsSkim_6.root'
    )
fileNamesZplusJets_part04 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/ZjetsSkim_PFCaloTauMet/zjetsSkim_7.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/ZjetsSkim_PFCaloTauMet/zjetsSkim_8.root',
    )
fileNamesZplusJets_part05 = cms.untracked.vstring(       
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/ZjetsSkim_PFCaloTauMet/zjetsSkim_9.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/ZjetsSkim_PFCaloTauMet/zjetsSkim_10.root'
    )
fileNamesZplusJets_part06 = cms.untracked.vstring(     
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/ZjetsSkim_PFCaloTauMet/zjetsSkim_11.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/ZjetsSkim_PFCaloTauMet/zjetsSkim_12.root'
    )
fileNamesZplusJets_part07 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/ZjetsSkim_PFCaloTauMet/zjetsSkim_13.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/ZjetsSkim_PFCaloTauMet/zjetsSkim_14.root'
    )
fileNamesZplusJets_part08 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/ZjetsSkim_PFCaloTauMet/zjetsSkim_15.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/ZjetsSkim_PFCaloTauMet/zjetsSkim_16.root'
    )
fileNamesZplusJets_part09 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/ZjetsSkim_PFCaloTauMet/zjetsSkim_17.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/ZjetsSkim_PFCaloTauMet/zjetsSkim_18.root'
    )
fileNamesZplusJets_part10 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/ZjetsSkim_PFCaloTauMet/zjetsSkim_19.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/ZjetsSkim_PFCaloTauMet/zjetsSkim_20.root'
    )

plotsOutputFileNameZplusJets = cms.string('plotsWtoTauNu_ZplusJets_partXX.root')
patTupleOutputFileNameZplusJets = cms.string('patTupleWtoTauNu_ZplusJets_partXX.root')

#--------------------------------------------------------------------------------
# ttbar skimmed sample (744020 events in total)
intLumiTTplusJets = float(2986.) #946644/317
corrFactorTTplusJets = float(1.087) #946644./870916

fileNamesTTplusJets_part01 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_1.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_2.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_3.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_4.root'
    )
fileNamesTTplusJets_part02 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_5.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_6.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_7.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_8.root'
    )
fileNamesTTplusJets_part03 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_9.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_10.root',  
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_11.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_12.root'
    )
fileNamesTTplusJets_part04 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_13.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_14.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_15.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_16.root'
    )
fileNamesTTplusJets_part05 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_17.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_18.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_19.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_20.root'
    )
fileNamesTTplusJets_part06 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_21.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_22.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_23.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_24.root'
    )
fileNamesTTplusJets_part07 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_25.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_26.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_27.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_28.root'
    )
fileNamesTTplusJets_part08 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_29.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_30.root',  
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_31.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_32.root'
    )
fileNamesTTplusJets_part09 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_33.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_34.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_35.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_36.root'
    )
fileNamesTTplusJets_part10 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_37.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_38.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_39.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_40.root'
)
fileNamesTTplusJets_part11 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_41.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_42.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_43.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_44.root'
    )
fileNamesTTplusJets_part12 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_45.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_46.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_47.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_48.root'
    )
fileNamesTTplusJets_part13 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_49.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_50.root',  
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_51.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_52.root'
    )
fileNamesTTplusJets_part14 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_53.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_54.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_55.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_56.root'
    )
fileNamesTTplusJets_part15 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_57.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_58.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_59.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_60.root'
    )
fileNamesTTplusJets_part16 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_61.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_62.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_63.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_64.root'
    )
fileNamesTTplusJets_part17 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_65.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_66.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_67.root'
    )
fileNamesTTplusJets_part18 = cms.untracked.vstring(   
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_68.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_69.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_70.root',  
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/TtbarSkim_PFCaloTauMet/ttbarSkim_71.root'
    )
  
plotsOutputFileNameTTplusJets = cms.string('plotsWtoTauNu_TTplusJets_partXX.root')
patTupleOutputFileNameTTplusJets = cms.string('patTupleWtoTauNu_TTplusJets_partXX.root')

