import FWCore.ParameterSet.Config as cms
import copy

intLumiWtoTauNu_Data_7TeV = float(50.)

#--------------------------------------------------------------------------------
#Grouping done so that each group contains < 20 files and 20-50k events

#W --> tau nu skimmed sample (in total 196531 events)
intLumiWtoTauNu_Wtaunu_7TeV = float(273.77) # NrEventsSkimmed/sigma_WTauNu = 2162540/7899
corrFactorWtoTauNu_Wtaunu_7TeV = float(1.) #16.3) # NrSkimmed/NrRun

fileNamesWtoTauNu_Wtaunu_7TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wtaunu7TeV_PFCaloTauMet/wTauNuSkim_1.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wtaunu7TeV_PFCaloTauMet/wTauNuSkim_2.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wtaunu7TeV_PFCaloTauMet/wTauNuSkim_3.root'
    )
fileNamesWtoTauNu_Wtaunu_7TeV_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wtaunu7TeV_PFCaloTauMet/wTauNuSkim_4.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wtaunu7TeV_PFCaloTauMet/wTauNuSkim_5.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wtaunu7TeV_PFCaloTauMet/wTauNuSkim_6.root'
    )
fileNamesWtoTauNu_Wtaunu_7TeV_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wtaunu7TeV_PFCaloTauMet/wTauNuSkim_7.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wtaunu7TeV_PFCaloTauMet/wTauNuSkim_8.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wtaunu7TeV_PFCaloTauMet/wTauNuSkim_9.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wtaunu7TeV_PFCaloTauMet/wTauNuSkim_10.root'
    )
fileNamesWtoTauNu_Wtaunu_7TeV_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wtaunu7TeV_PFCaloTauMet/wTauNuSkim_11.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wtaunu7TeV_PFCaloTauMet/wTauNuSkim_12.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wtaunu7TeV_PFCaloTauMet/wTauNuSkim_13.root'
    )
fileNamesWtoTauNu_Wtaunu_7TeV_part05 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wtaunu7TeV_PFCaloTauMet/wTauNuSkim_14.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wtaunu7TeV_PFCaloTauMet/wTauNuSkim_16.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wtaunu7TeV_PFCaloTauMet/wTauNuSkim_17.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wtaunu7TeV_PFCaloTauMet/wTauNuSkim_18.root'
    )

plotsOutputFileNameWtoTauNu_Wtaunu_7TeV = cms.string('plotsWtoTauNu_Wtaunu_7TeV_partXX.root')
patTupleOutputFileNameWtoTauNu_Wtaunu_7TeV = cms.string('patTupleWtoTauNu_Wtaunu_7TeV_partXX.root')

#Z+jets skimmed sample (in total 196531 events)
intLumiWtoTauNu_ZplusJets_7TeV = float(435.88) # NrEventsSkimmed/sigma_WZplusJets = 1046123/2400
corrFactorWtoTauNu_ZplusJets_7TeV = float(1.) # NrSkimmed/NrRun

fileNamesWtoTauNu_ZplusJets_7TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/ZplusJets7TeV_PFCaloTauMet/zPlusJetsSkim_1_1.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/ZplusJets7TeV_PFCaloTauMet/zPlusJetsSkim_2_1.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/ZplusJets7TeV_PFCaloTauMet/zPlusJetsSkim_3_1.root'
    )
fileNamesWtoTauNu_ZplusJets_7TeV_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/ZplusJets7TeV_PFCaloTauMet/zPlusJetsSkim_4_1.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/ZplusJets7TeV_PFCaloTauMet/zPlusJetsSkim_5_1.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/ZplusJets7TeV_PFCaloTauMet/zPlusJetsSkim_6_1.root'
    )
fileNamesWtoTauNu_ZplusJets_7TeV_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/ZplusJets7TeV_PFCaloTauMet/zPlusJetsSkim_7_1.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/ZplusJets7TeV_PFCaloTauMet/zPlusJetsSkim_8_1.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/ZplusJets7TeV_PFCaloTauMet/zPlusJetsSkim_9_1.root'
    )
fileNamesWtoTauNu_ZplusJets_7TeV_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/ZplusJets7TeV_PFCaloTauMet/zPlusJetsSkim_10_1.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/ZplusJets7TeV_PFCaloTauMet/zPlusJetsSkim_11_1.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/ZplusJets7TeV_PFCaloTauMet/zPlusJetsSkim_12_1.root'
    )
fileNamesWtoTauNu_ZplusJets_7TeV_part05 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/ZplusJets7TeV_PFCaloTauMet/zPlusJetsSkim_13_1.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/ZplusJets7TeV_PFCaloTauMet/zPlusJetsSkim_14_1.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/ZplusJets7TeV_PFCaloTauMet/zPlusJetsSkim_15_1.root'
    )

plotsOutputFileNameWtoTauNu_ZplusJets_7TeV = cms.string('plotsWtoTauNu_ZplusJets_7TeV_partXX.root')
patTupleOutputFileNameWtoTauNu_ZplusJets_7TeV = cms.string('patTupleWtoTauNu_ZplusJets_7TeV_partXX.root')




#W --> e nu skimmed sample (in total 196531 events)
intLumiWtoTauNu_Wenu_7TeV = float(136.51) # (NrSkimmed/prefilter)/sigma_Wenu = (840000/0.779)/7899
corrFactorWtoTauNu_Wenu_7TeV = float(1.064) #16.3) # NrSkimmed/nrAtCastor

fileNamesWtoTauNu_Wenu_7TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wenu7TeV_PFCaloTauMet/wENuSkim_1.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wenu7TeV_PFCaloTauMet/wENuSkim_2.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wenu7TeV_PFCaloTauMet/wENuSkim_3.root'
    )
fileNamesWtoTauNu_Wenu_7TeV_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wenu7TeV_PFCaloTauMet/wENuSkim_4.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wenu7TeV_PFCaloTauMet/wENuSkim_5.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wenu7TeV_PFCaloTauMet/wENuSkim_6.root'
    )
fileNamesWtoTauNu_Wenu_7TeV_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wenu7TeV_PFCaloTauMet/wENuSkim_7.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wenu7TeV_PFCaloTauMet/wENuSkim_8.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wenu7TeV_PFCaloTauMet/wENuSkim_9.root'
    )
fileNamesWtoTauNu_Wenu_7TeV_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wenu7TeV_PFCaloTauMet/wENuSkim_10.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wenu7TeV_PFCaloTauMet/wENuSkim_11.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wenu7TeV_PFCaloTauMet/wENuSkim_12.root'
    )
fileNamesWtoTauNu_Wenu_7TeV_part05 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wenu7TeV_PFCaloTauMet/wENuSkim_13.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wenu7TeV_PFCaloTauMet/wENuSkim_14.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wenu7TeV_PFCaloTauMet/wENuSkim_15.root'
    )
fileNamesWtoTauNu_Wenu_7TeV_part06 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wenu7TeV_PFCaloTauMet/wENuSkim_16.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wenu7TeV_PFCaloTauMet/wENuSkim_17.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wenu7TeV_PFCaloTauMet/wENuSkim_18.root'
    )
fileNamesWtoTauNu_Wenu_7TeV_part07 = cms.untracked.vstring(
        'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wenu7TeV_PFCaloTauMet/wENuSkim_19.root',
        'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wenu7TeV_PFCaloTauMet/wENuSkim_20.root',
        'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wenu7TeV_PFCaloTauMet/wENuSkim_21.root'
        )
fileNamesWtoTauNu_Wenu_7TeV_part08 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wenu7TeV_PFCaloTauMet/wENuSkim_22.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wenu7TeV_PFCaloTauMet/wENuSkim_23.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wenu7TeV_PFCaloTauMet/wENuSkim_24.root'
    )
fileNamesWtoTauNu_Wenu_7TeV_part09 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wenu7TeV_PFCaloTauMet/wENuSkim_25.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wenu7TeV_PFCaloTauMet/wENuSkim_26.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wenu7TeV_PFCaloTauMet/wENuSkim_27.root'
    )
fileNamesWtoTauNu_Wenu_7TeV_part10 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wenu7TeV_PFCaloTauMet/wENuSkim_28.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wenu7TeV_PFCaloTauMet/wENuSkim_29.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wenu7TeV_PFCaloTauMet/wENuSkim_30.root'
    )

plotsOutputFileNameWtoTauNu_Wenu_7TeV = cms.string('plotsWtoTauNu_Wenu_7TeV_partXX.root')
patTupleOutputFileNameWtoTauNu_Wenu_7TeV = cms.string('patTupleWtoTauNu_Wenu_7TeV_partXX.root')


#W --> mu nu skimmed sample (in total 196531 events)
intLumiWtoTauNu_Wmunu_7TeV = float(151.31) # (NrSkimmed/preFilterEff)/sigma_WTauNu = (886833/0.742)/7899
corrFactorWtoTauNu_Wmunu_7TeV = float(1.) #16.3) # NrSkimmed/NrAtCastor 

fileNamesWtoTauNu_Wmunu_7TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wmunu7TeV_PFCaloTauMet/wMuNuSkim_1.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wmunu7TeV_PFCaloTauMet/wMuNuSkim_2.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wmunu7TeV_PFCaloTauMet/wMuNuSkim_3.root'
    )
fileNamesWtoTauNu_Wmunu_7TeV_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wmunu7TeV_PFCaloTauMet/wMuNuSkim_4.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wmunu7TeV_PFCaloTauMet/wMuNuSkim_5.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wmunu7TeV_PFCaloTauMet/wMuNuSkim_6.root'
    )
fileNamesWtoTauNu_Wmunu_7TeV_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wmunu7TeV_PFCaloTauMet/wMuNuSkim_7.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wmunu7TeV_PFCaloTauMet/wMuNuSkim_8.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wmunu7TeV_PFCaloTauMet/wMuNuSkim_9.root'
    )
fileNamesWtoTauNu_Wmunu_7TeV_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wmunu7TeV_PFCaloTauMet/wMuNuSkim_10.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wmunu7TeV_PFCaloTauMet/wMuNuSkim_11.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wmunu7TeV_PFCaloTauMet/wMuNuSkim_12.root'
    )
fileNamesWtoTauNu_Wmunu_7TeV_part05 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wmunu7TeV_PFCaloTauMet/wMuNuSkim_13.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wmunu7TeV_PFCaloTauMet/wMuNuSkim_14.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wmunu7TeV_PFCaloTauMet/wMuNuSkim_15.root'
    )
fileNamesWtoTauNu_Wmunu_7TeV_part06 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wmunu7TeV_PFCaloTauMet/wMuNuSkim_16.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wmunu7TeV_PFCaloTauMet/wMuNuSkim_17.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wmunu7TeV_PFCaloTauMet/wMuNuSkim_18.root'
    )
fileNamesWtoTauNu_Wmunu_7TeV_part07 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wmunu7TeV_PFCaloTauMet/wMuNuSkim_19.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wmunu7TeV_PFCaloTauMet/wMuNuSkim_20.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wmunu7TeV_PFCaloTauMet/wMuNuSkim_21.root'
    )
fileNamesWtoTauNu_Wmunu_7TeV_part08 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wmunu7TeV_PFCaloTauMet/wMuNuSkim_22.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wmunu7TeV_PFCaloTauMet/wMuNuSkim_23.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wmunu7TeV_PFCaloTauMet/wMuNuSkim_24.root'
    )
fileNamesWtoTauNu_Wmunu_7TeV_part09 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wmunu7TeV_PFCaloTauMet/wMuNuSkim_25.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wmunu7TeV_PFCaloTauMet/wMuNuSkim_26.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wmunu7TeV_PFCaloTauMet/wMuNuSkim_27.root'
    )
fileNamesWtoTauNu_Wmunu_7TeV_part10 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wmunu7TeV_PFCaloTauMet/wMuNuSkim_28.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wmunu7TeV_PFCaloTauMet/wMuNuSkim_29.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wmunu7TeV_PFCaloTauMet/wMuNuSkim_30.root'
    )
fileNamesWtoTauNu_Wmunu_7TeV_part11 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wmunu7TeV_PFCaloTauMet/wMuNuSkim_31.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wmunu7TeV_PFCaloTauMet/wMuNuSkim_32.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wmunu7TeV_PFCaloTauMet/wMuNuSkim_33.root'
    )
fileNamesWtoTauNu_Wmunu_7TeV_part12 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wmunu7TeV_PFCaloTauMet/wMuNuSkim_34.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wmunu7TeV_PFCaloTauMet/wMuNuSkim_35.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/Wmunu7TeV_PFCaloTauMet/wMuNuSkim_36.root'
    )

plotsOutputFileNameWtoTauNu_Wmunu_7TeV = cms.string('plotsWtoTauNu_Wmunu_7TeV_partXX.root')
patTupleOutputFileNameWtoTauNu_Wmunu_7TeV = cms.string('patTupleWtoTauNu_Wmunu_7TeV_partXX.root')

#qcd skimmed sample (in total 819519 events)
intLumiWtoTauNu_qcd_W_7TeV = float(0.204) # (NrDBS/preFilterEff)/sigma = (6023696/0.0338)/8.74e8
corrFactorWtoTauNu_qcd_W_7TeV = float(1.) # NrSkimmed/NrAtCastor (event losses)

fileNamesWtoTauNu_qcd_W_7TeV_part01 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_1.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_2.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_3.root'
    )
fileNamesWtoTauNu_qcd_W_7TeV_part02 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_4.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_5.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_6.root'
    )
fileNamesWtoTauNu_qcd_W_7TeV_part03 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_7.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_8.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_9.root'
    )
fileNamesWtoTauNu_qcd_W_7TeV_part04 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_10.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_11.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_12.root'
    )
fileNamesWtoTauNu_qcd_W_7TeV_part05 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_13.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_14.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_15.root'
    )
fileNamesWtoTauNu_qcd_W_7TeV_part06 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_16.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_17.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_18.root'
    )
fileNamesWtoTauNu_qcd_W_7TeV_part07 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_19.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_20.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_21.root'
    )
fileNamesWtoTauNu_qcd_W_7TeV_part08 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_22.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_23.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_24.root'
    )
fileNamesWtoTauNu_qcd_W_7TeV_part09 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_25.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_26.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_27.root'
    )
fileNamesWtoTauNu_qcd_W_7TeV_part10 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_28.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_29.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_30.root'
    )
fileNamesWtoTauNu_qcd_W_7TeV_part11 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_31.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_32.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_33.root'
    )
fileNamesWtoTauNu_qcd_W_7TeV_part12 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_34.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_35.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_36.root'
    )
fileNamesWtoTauNu_qcd_W_7TeV_part13 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_37.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_38.root'
    )
fileNamesWtoTauNu_qcd_W_7TeV_part14 = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_39.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_40.root',
    'rfio:/castor/cern.ch/user/l/liis/SkimNov09/qcd7TeV_PFCaloTauMet/qcdSkim_41.root'
    )

plotsOutputFileNameWtoTauNu_qcd_W_7TeV = cms.string('plotsWtoTauNu_qcd_W_7TeV_partXX.root')
patTupleOutputFileNameWtoTauNu_qcd_W_7TeV = cms.string('patTupleWtoTauNu_qcd_W_7TeV_partXX.root')
