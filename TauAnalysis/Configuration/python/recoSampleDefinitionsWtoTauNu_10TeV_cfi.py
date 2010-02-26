import FWCore.ParameterSet.Config as cms
import copy

intLumiWtoTauNu_Data_10TeV = float(100.)

#--------------------------------------------------------------------------------
#Grouping done so that each group contains < 20 files and 20-50k events

#W --> tau nu skimmed sample (in total 325089 events)
intLumiWtoTauNu_Wtaunu_10TeV = float(174.3) # Nr_skimmed/sigma_WTauNu = 2084520/11960
corrFactorWtoTauNu_Wtaunu_10TeV = float(1.) #

fileNamesWtoTauNu_Wtaunu_10TeV_part01 = cms.untracked.vstring(
        'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WtaunuSkim_PFCaloTauMet/wTauNuSkim_1.root',
            'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WtaunuSkim_PFCaloTauMet/wTauNuSkim_2.root',
            'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WtaunuSkim_PFCaloTauMet/wTauNuSkim_3.root'
            )
fileNamesWtoTauNu_Wtaunu_10TeV_part02 = cms.untracked.vstring(
        'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WtaunuSkim_PFCaloTauMet/wTauNuSkim_4.root',
            'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WtaunuSkim_PFCaloTauMet/wTauNuSkim_5.root',
            'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WtaunuSkim_PFCaloTauMet/wTauNuSkim_6.root'
            )
fileNamesWtoTauNu_Wtaunu_10TeV_part03 = cms.untracked.vstring(
        'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WtaunuSkim_PFCaloTauMet/wTauNuSkim_8.root',
            'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WtaunuSkim_PFCaloTauMet/wTauNuSkim_9.root',
            'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WtaunuSkim_PFCaloTauMet/wTauNuSkim_10.root'
            )
fileNamesWtoTauNu_Wtaunu_10TeV_part04 = cms.untracked.vstring(
        'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WtaunuSkim_PFCaloTauMet/wTauNuSkim_11.root',
            'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WtaunuSkim_PFCaloTauMet/wTauNuSkim_12.root',
            'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WtaunuSkim_PFCaloTauMet/wTauNuSkim_13.root'
            )
fileNamesWtoTauNu_Wtaunu_10TeV_part05 = cms.untracked.vstring(
        'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WtaunuSkim_PFCaloTauMet/wTauNuSkim_14.root',
            'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WtaunuSkim_PFCaloTauMet/wTauNuSkim_15.root',
            'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WtaunuSkim_PFCaloTauMet/wTauNuSkim_16.root',
            'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WtaunuSkim_PFCaloTauMet/wTauNuSkim_17.root'
            )

plotsOutputFileNameWtoTauNu_Wtaunu_10TeV = cms.string('plotsWtoTauNu_Wtaunu_10TeV_partXX.root')
patTupleOutputFileNameWtoTauNu_Wtaunu_10TeV = cms.string('patTupleWtoTauNu_Wtaunu_10TeV_partXX.root')

#W --> mu nu skimmed sample (in total 824659 events)
intLumiWtoTauNu_Wmunu_10TeV = float(122.2) # (Nr_skimmed/pre_filter_efficiency)/sigma_WTauNu = (1000000/0.691)/11840
corrFactorWtoTauNu_Wmunu_10TeV = float(1.) #

fileNamesWtoTauNu_Wmunu_10TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_1.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_2.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_3.root'
            )
fileNamesWtoTauNu_Wmunu_10TeV_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_4.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_5.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_6.root'
    )
fileNamesWtoTauNu_Wmunu_10TeV_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_7.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_8.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_9.root'
    )
fileNamesWtoTauNu_Wmunu_10TeV_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_10.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_11.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_12.root'
    )
fileNamesWtoTauNu_Wmunu_10TeV_part05 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_13.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_14.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_15.root'
    )
fileNamesWtoTauNu_Wmunu_10TeV_part06 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_16.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_17.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_18.root'
            )
fileNamesWtoTauNu_Wmunu_10TeV_part07 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_19.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_20.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_21.root'
    )
fileNamesWtoTauNu_Wmunu_10TeV_part08 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_22.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_23.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_24.root'
    )
fileNamesWtoTauNu_Wmunu_10TeV_part09 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_25.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_26.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_27.root'
    )
fileNamesWtoTauNu_Wmunu_10TeV_part10 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_28.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_29.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_30.root'
    )
fileNamesWtoTauNu_Wmunu_10TeV_part11 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_31.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_32.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_33.root'
    )
fileNamesWtoTauNu_Wmunu_10TeV_part12 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_34.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_35.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_36.root'
    )
fileNamesWtoTauNu_Wmunu_10TeV_part13 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_37.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_38.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_39.root'
    )
fileNamesWtoTauNu_Wmunu_10TeV_part14 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_40.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_41.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WmunuSkim_PFCaloTauMet/wMuNuSkim_42.root'
    )

plotsOutputFileNameWtoTauNu_Wmunu_10TeV = cms.string('plotsWtoTauNu_Wmunu_10TeV_partXX.root')
patTupleOutputFileNameWtoTauNu_Wmunu_10TeV = cms.string('patTupleWtoTauNu_Wmunu_10TeV_partXX.root')

#W --> e nu skimmed sample (in total 648626 events)
intLumiWtoTauNu_Wenu_10TeV = float(114.4) # (Nr_skimmed/pre_filter_efficiency)/sigma_WTauNu = (1000000/0.738)/11840
corrFactorWtoTauNu_Wenu_10TeV = float(1.) #

fileNamesWtoTauNu_Wenu_10TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WenuSkim_PFCaloTauMet/wENuSkim_1.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WenuSkim_PFCaloTauMet/wENuSkim_2.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WenuSkim_PFCaloTauMet/wENuSkim_3.root'
            )
fileNamesWtoTauNu_Wenu_10TeV_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WenuSkim_PFCaloTauMet/wENuSkim_4.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WenuSkim_PFCaloTauMet/wENuSkim_5.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WenuSkim_PFCaloTauMet/wENuSkim_6.root'
    )
fileNamesWtoTauNu_Wenu_10TeV_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WenuSkim_PFCaloTauMet/wENuSkim_7.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WenuSkim_PFCaloTauMet/wENuSkim_8.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WenuSkim_PFCaloTauMet/wENuSkim_9.root'
    )
fileNamesWtoTauNu_Wenu_10TeV_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WenuSkim_PFCaloTauMet/wENuSkim_10.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WenuSkim_PFCaloTauMet/wENuSkim_11.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WenuSkim_PFCaloTauMet/wENuSkim_12.root'
    )
fileNamesWtoTauNu_Wenu_10TeV_part05 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WenuSkim_PFCaloTauMet/wENuSkim_13.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WenuSkim_PFCaloTauMet/wENuSkim_14.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WenuSkim_PFCaloTauMet/wENuSkim_15.root'
    )
fileNamesWtoTauNu_Wenu_10TeV_part06 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WenuSkim_PFCaloTauMet/wENuSkim_16.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WenuSkim_PFCaloTauMet/wENuSkim_17.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WenuSkim_PFCaloTauMet/wENuSkim_18.root'
            )
fileNamesWtoTauNu_Wenu_10TeV_part07 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WenuSkim_PFCaloTauMet/wENuSkim_19.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WenuSkim_PFCaloTauMet/wENuSkim_20.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WenuSkim_PFCaloTauMet/wENuSkim_21.root'
    )
fileNamesWtoTauNu_Wenu_10TeV_part08 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WenuSkim_PFCaloTauMet/wENuSkim_22.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WenuSkim_PFCaloTauMet/wENuSkim_23.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WenuSkim_PFCaloTauMet/wENuSkim_24.root'
    )
fileNamesWtoTauNu_Wenu_10TeV_part09 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WenuSkim_PFCaloTauMet/wENuSkim_25.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WenuSkim_PFCaloTauMet/wENuSkim_26.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WenuSkim_PFCaloTauMet/wENuSkim_27.root'
    )
fileNamesWtoTauNu_Wenu_10TeV_part10 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WenuSkim_PFCaloTauMet/wENuSkim_28.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WenuSkim_PFCaloTauMet/wENuSkim_29.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WenuSkim_PFCaloTauMet/wENuSkim_30.root'
    )
fileNamesWtoTauNu_Wenu_10TeV_part11 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WenuSkim_PFCaloTauMet/wENuSkim_31.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WenuSkim_PFCaloTauMet/wENuSkim_32.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/WenuSkim_PFCaloTauMet/wENuSkim_33.root'
    )

plotsOutputFileNameWtoTauNu_Wenu_10TeV = cms.string('plotsWtoTauNu_Wenu_10TeV_partXX.root')
patTupleOutputFileNameWtoTauNu_Wenu_10TeV = cms.string('patTupleWtoTauNu_Wenu_10TeV_partXX.root')

#qcd skimmed sample (in total 985996 events)
intLumiWtoTauNu_qcd_W_10TeV = float(0.127) # (Nr_skimmed/pre_filter_efficiency)/sigma_WTauNu = (6240513/0.0338)/1.457e9
corrFactorWtoTauNu_qcd_W_10TeV = float(1.) #

fileNamesWtoTauNu_qcd_W_10TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_1.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd_WSkim_PFCaloTauMet/qcdSkim_2.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd_WSkim_PFCaloTauMet/qcdSkim_3.root'
            )
fileNamesWtoTauNu_qcd_W_10TeV_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_4.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_5.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_6.root'
    )
fileNamesWtoTauNu_qcd_W_10TeV_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_7.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_8.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_9.root'
    )
fileNamesWtoTauNu_qcd_W_10TeV_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_10.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_11.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_12.root'
    )
fileNamesWtoTauNu_qcd_W_10TeV_part05 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_13.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_14.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_15.root'
    )
fileNamesWtoTauNu_qcd_W_10TeV_part06 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_16.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_17.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_18.root'
            )
fileNamesWtoTauNu_qcd_W_10TeV_part07 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_19.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_20.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_21.root'
    )
fileNamesWtoTauNu_qcd_W_10TeV_part08 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_22.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_23.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_24.root'
    )
fileNamesWtoTauNu_qcd_W_10TeV_part09 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_25.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_26.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_27.root'
    )
fileNamesWtoTauNu_qcd_W_10TeV_part10 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_28.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_29.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_30.root'
    )
fileNamesWtoTauNu_qcd_W_10TeV_part11 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_31.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_32.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_33.root'
    )
fileNamesWtoTauNu_qcd_W_10TeV_part12 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_34.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_35.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_36.root'
    )
fileNamesWtoTauNu_qcd_W_10TeV_part13 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_37.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_38.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_39.root'
    )
fileNamesWtoTauNu_qcd_W_10TeV_part14 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_40.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_41.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_42.root'
    )
fileNamesWtoTauNu_qcd_W_10TeV_part15 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_43.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_44.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_45.root'
    )
fileNamesWtoTauNu_qcd_W_10TeV_part16 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_46.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_47.root'
    )
fileNamesWtoTauNu_qcd_W_10TeV_part17 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_48.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_49.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcdSkim_PFCaloTauMet/qcdSkim_50.root'
    )

plotsOutputFileNameWtoTauNu_qcd_W_10TeV = cms.string('plotsWtoTauNu_qcd_W_10TeV_partXX.root')
patTupleOutputFileNameWtoTauNu_qcd_W_10TeV = cms.string('patTupleWtoTauNu_qcd_W_10TeV_partXX.root')
