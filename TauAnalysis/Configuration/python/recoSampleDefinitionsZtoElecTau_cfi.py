import FWCore.ParameterSet.Config as cms
import copy

# define configuration parameters for submission of Z --> e + tau-jet jobs to CERN batch system
# (running over skimmed samples stored on CASTOR)
#
# loosely skimmed samples:
#   gsfElectron with abs(eta) < 2.5 and pt > 8
#	PF or calo tau (discriminant by leading pion/track Pt cut)
#   no dR cut
#
# Authors:
#	Jeff Kolb, Notre Dame
#   Christian Veelken, UC Davis
#

##################
#
# 7 TeV DATA samples
#
##################


intLumiZtoElecTau_Data = float(2.6) # in 1/pb
#intLumiZtoElecTau_Data = float(100.) # in 1/pb

corrFactorZtoElecTau_Data_7TeV = float(1.)
patTupleOutputFileNameZtoElecTau_Data_7TeV = cms.untracked.string('patTupleZtoElecTau_Data_7TeV_partXX.root')
plotsOutputFileNameZtoElecTau_Data_7TeV = cms.string('plotsZtoElecTau_Data_7TeV_partXX.root')

genPhaseSpaceCutZtoElecTau_Data_7TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

# dataset = /MinimumBias/Commissioning10-SD_EG-v9/RECO
#first run = 133863
#last run = 135802
#events before skim = 1028657
#events passing skim = 42331
#lumi_mask = /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV/StreamExpress/Cert_132440-135735_7TeV_StreamExpress_Collisions10_JSON.txt
#lumi = 8.23/nb

fileNamesZtoElecTau_Data_7TeV_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_10_0.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_11_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_12_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_13_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_14_0.root'
)
fileNamesZtoElecTau_Data_7TeV_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_15_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_16_0.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_17_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_18_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_19_1.root'
)
fileNamesZtoElecTau_Data_7TeV_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_1_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_20_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_21_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_22_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_23_0.root'
)
fileNamesZtoElecTau_Data_7TeV_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_24_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_25_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_26_0.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_27_0.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_28_0.root'
)
fileNamesZtoElecTau_Data_7TeV_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_29_0.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_2_0.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_30_0.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_31_0.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_32_0.root'
)
fileNamesZtoElecTau_Data_7TeV_part06 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_33_0.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_34_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_35_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_36_0.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_37_1.root'
)
fileNamesZtoElecTau_Data_7TeV_part07 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_38_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_39_0.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_3_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_40_0.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_41_0.root'
)
fileNamesZtoElecTau_Data_7TeV_part08 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_42_0.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_43_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_44_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_45_0.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_46_0.root'
)
fileNamesZtoElecTau_Data_7TeV_part09 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_47_0.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_48_0.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_49_0.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_4_0.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_50_1.root'
)
fileNamesZtoElecTau_Data_7TeV_part10 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_51_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_52_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_53_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_54_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_55_1.root'
)
fileNamesZtoElecTau_Data_7TeV_part11 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_56_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_57_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_58_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_59_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_5_1.root'
)
fileNamesZtoElecTau_Data_7TeV_part12 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_60_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_61_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_62_0.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_63_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_64_1.root'
)
fileNamesZtoElecTau_Data_7TeV_part13 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_65_0.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_66_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_67_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_68_0.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_6_1.root'
)
fileNamesZtoElecTau_Data_7TeV_part14 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_7_1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_8_0.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_SD_take7/skimElecTau_9_1.root'
)

# dataset = /EG/Run2010A-June14thReReco_v1/RECO
#first run = 135821
#last run = 137436
#events before skim = 1067264
#events passed = 26274
#lumi_mask = /afs/cernch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV/Reprocessing/Cert_132440-137028_7TeV_June14thReReco_Collisions10_JSON.txt
#lumi = 4.924/nb

fileNamesZtoElecTau_Data_7TeV_part15 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJune14th_take0/skimElecTau_10_1_V8Q.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJune14th_take0/skimElecTau_11_1_Kig.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJune14th_take0/skimElecTau_12_1_IQa.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJune14th_take0/skimElecTau_13_1_QOf.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJune14th_take0/skimElecTau_14_1_ZVg.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJune14th_take0/skimElecTau_15_1_Gfw.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJune14th_take0/skimElecTau_16_1_0Xr.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJune14th_take0/skimElecTau_17_1_MsZ.root'
)
fileNamesZtoElecTau_Data_7TeV_part16 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJune14th_take0/skimElecTau_18_1_ZRS.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJune14th_take0/skimElecTau_19_1_LGA.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJune14th_take0/skimElecTau_1_1_YMC.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJune14th_take0/skimElecTau_20_1_HkB.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJune14th_take0/skimElecTau_21_1_8xl.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJune14th_take0/skimElecTau_22_1_K0i.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJune14th_take0/skimElecTau_23_1_lYj.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJune14th_take0/skimElecTau_24_1_hzo.root'
)
fileNamesZtoElecTau_Data_7TeV_part17 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJune14th_take0/skimElecTau_25_1_bEX.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJune14th_take0/skimElecTau_2_1_j7t.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJune14th_take0/skimElecTau_3_1_Rcm.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJune14th_take0/skimElecTau_4_1_gmW.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJune14th_take0/skimElecTau_5_1_LW9.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJune14th_take0/skimElecTau_6_1_4NS.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJune14th_take0/skimElecTau_7_1_7Eb.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJune14th_take0/skimElecTau_8_1_hB3.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJune14th_take0/skimElecTau_9_1_LNX.root'
)

# dataset = /EG/Run2010A-PromptReco-v4/RECO
#first run = 137437
#last run = 139558
#events before skim = 4538057
#events passed = 302640
#lumi_mask = /afs/cernch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV/StreamExpress/Cert_132440-140182_7TeV_StreamExpress_Collisions10_JSON.txt
#lumi = 60.8/nb

#  6.8/nb/job

fileNamesZtoElecTau_Data_7TeV_part18 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_10_1_4NU.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_11_1_P6D.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_12_1_pGF.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_13_1_9gf.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_14_1_wIB.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_15_1_N3Y.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_16_1_CQm.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_17_1_b3y.root'
)
fileNamesZtoElecTau_Data_7TeV_part19 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_18_1_bVq.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_19_1_Ped.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_1_1_DVs.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_20_1_aKY.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_21_1_i8T.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_22_1_kw5.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_23_1_gTA.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_24_1_hMp.root'
)
fileNamesZtoElecTau_Data_7TeV_part20 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_25_1_EDb.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_26_1_TBz.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_27_1_X9P.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_28_1_EF6.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_29_1_PzN.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_2_2_RTz.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_30_1_RlF.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_31_1_wEw.root'
)
fileNamesZtoElecTau_Data_7TeV_part21 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_32_1_T0E.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_33_1_D3M.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_34_1_2Sz.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_35_1_ee4.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_36_1_ser.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_37_1_b9N.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_38_1_GHa.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_39_1_0UE.root'
)
fileNamesZtoElecTau_Data_7TeV_part22 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_3_1_xvC.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_40_1_MZl.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_41_1_pnI.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_42_1_EGe.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_43_1_GJH.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_44_1_Ekx.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_45_1_Xfq.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_46_1_qz8.root'
)
fileNamesZtoElecTau_Data_7TeV_part23 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_47_1_I63.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_48_2_unp.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_49_1_A2R.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_4_1_Vt2.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_50_1_KAn.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_51_1_S0w.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_52_1_o6j.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_53_1_zq4.root'
)
fileNamesZtoElecTau_Data_7TeV_part24 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_54_1_JxX.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_55_1_JOf.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_56_1_gfL.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_57_1_GQE.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_58_1_auv.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_59_1_GXV.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_5_1_C4c.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_60_1_ay7.root'
)
fileNamesZtoElecTau_Data_7TeV_part25 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_61_1_9fM.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_62_1_vzN.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_63_1_lN3.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_64_1_nrs.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_65_1_Z2X.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_66_1_s8C.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_67_1_aGw.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_68_1_5RW.root'
)
fileNamesZtoElecTau_Data_7TeV_part26 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_69_1_1J9.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_6_1_23S.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_70_1_HWB.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_71_1_JKD.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_72_1_WpG.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_73_1_C0K.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_74_1_SZK.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_75_1_HrX.root'
)
fileNamesZtoElecTau_Data_7TeV_part27 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_76_1_Q8O.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_77_1_5Gh.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_78_1_RWB.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_79_1_cS7.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_7_1_4v2.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_80_1_Tyh.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_8_1_d4r.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part1/skimElecTau_9_1_OQY.root'
)

# dataset = /EG/Run2010A-Jul16thReReco-v2/RECO
#first run = 139559
#last run = 140159
#events before skim = 3876978
#events passed = 420640
#lumi_mask = /afs/cernch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV/Reprocessing/Cert_139779-140159_7TeV_July16thReReco_Collisions10_JSON.txt
#lumi = 118.9/nb

# 9.2/nb/job

fileNamesZtoElecTau_Data_7TeV_part28 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_10_2_shM.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_11_1_waY.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_12_1_LpV.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_13_1_X7y.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_14_1_iaU.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_15_1_Lo2.root'
)
fileNamesZtoElecTau_Data_7TeV_part29 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_16_1_up3.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_17_1_ogL.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_18_1_yXl.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_19_1_S1g.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_1_1_7g9.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_20_1_T1U.root'
)
fileNamesZtoElecTau_Data_7TeV_part30 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_21_1_eof.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_22_1_z0G.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_23_1_ISy.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_24_1_LtK.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_25_1_CuX.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_26_1_3Ws.root'
)
fileNamesZtoElecTau_Data_7TeV_part31 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_27_1_mg1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_28_1_Fjl.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_29_1_COf.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_2_1_jba.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_30_1_evl.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_31_2_DnN.root'
)
fileNamesZtoElecTau_Data_7TeV_part32 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_32_1_x1e.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_33_1_uxz.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_34_1_Sqg.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_35_1_lw7.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_36_1_uOF.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_37_1_7k6.root'
)
fileNamesZtoElecTau_Data_7TeV_part33 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_38_1_UMz.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_39_1_2uH.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_3_1_hNN.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_40_1_e4N.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_41_1_ngw.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_42_1_ANq.root'
)
fileNamesZtoElecTau_Data_7TeV_part34 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_43_1_k04.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_44_1_bm3.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_45_1_HWJ.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_46_1_WG3.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_47_1_0E0.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_48_1_gaC.root'
)
fileNamesZtoElecTau_Data_7TeV_part35 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_49_1_tiH.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_4_1_mpG.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_50_1_bs5.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_51_1_cV5.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_52_1_hu9.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_53_1_Nys.root'
)
fileNamesZtoElecTau_Data_7TeV_part36 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_54_1_gQQ.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_55_1_TlN.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_56_1_WtD.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_57_1_FdN.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_58_1_34p.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_59_1_TGf.root'
)
fileNamesZtoElecTau_Data_7TeV_part37 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_5_1_OCO.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_60_1_kq6.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_61_1_1k5.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_62_1_tLk.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_63_1_dAd.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_64_1_P33.root'
)
fileNamesZtoElecTau_Data_7TeV_part38 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_65_1_nQX.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_66_1_UKO.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_67_1_0yG.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_68_1_plo.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_69_1_tmg.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_6_1_DtC.root'
)
fileNamesZtoElecTau_Data_7TeV_part39 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_70_1_qnE.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_71_1_QW5.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_72_1_UtS.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_73_1_A6j.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_74_1_zb1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_75_1_LRY.root'
)
fileNamesZtoElecTau_Data_7TeV_part40 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_76_1_mmZ.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_77_1_xXv.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_78_1_7ia.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_79_1_Hp3.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_7_1_Wsi.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_80_1_9Di.root'
)
fileNamesZtoElecTau_Data_7TeV_part41 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_81_1_nET.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_82_1_JQg.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_83_1_IXP.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_8_1_ggw.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_ReRecoJul16th/skimElecTau_9_1_JAu.root'
)

# dataset = /EG/Run2010A-PromptReco-v4/RECO
# first run = 140160
# last run = 140399
# events before skim = 1586011
# events passed = 203141
# lumi_mask = /afs/cernch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV/StreamExpress/Cert_132440-140399_7TeV_StreamExpress_Collisions10_JSON.txt
# lumi = 61.2/nb

# 7.7/nb/job

fileNamesZtoElecTau_Data_7TeV_part42 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_10_1_4ah.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_11_1_hn7.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_12_1_pSE.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_13_1_0Hy.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_14_1_sDO.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_15_1_j1E.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_16_1_wWF.root'
)
fileNamesZtoElecTau_Data_7TeV_part43 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_17_1_64M.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_18_1_439.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_19_1_Cpf.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_1_1_7d3.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_20_1_RYA.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_21_1_FCf.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_22_1_wq8.root'
)
fileNamesZtoElecTau_Data_7TeV_part44 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_23_1_2qS.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_24_1_fqb.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_25_1_gKS.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_26_1_fx2.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_27_1_JsX.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_28_1_c7W.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_29_1_08d.root'
)
fileNamesZtoElecTau_Data_7TeV_part45 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_2_1_wHO.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_30_1_HL4.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_31_1_twJ.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_32_1_Rh4.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_33_1_QE4.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_34_1_c7h.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_35_1_gU4.root'
)
fileNamesZtoElecTau_Data_7TeV_part46 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_36_1_Q2p.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_37_1_l58.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_38_1_wgG.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_39_1_CLu.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_3_1_y1Q.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_40_1_fcs.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_41_1_rD9.root'
)
fileNamesZtoElecTau_Data_7TeV_part47 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_42_1_sjh.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_43_1_ePO.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_44_1_e0t.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_45_1_8hB.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_46_1_wry.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_47_1_j2e.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_48_1_WGr.root'
)
fileNamesZtoElecTau_Data_7TeV_part48 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_49_1_KVt.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_4_1_CBI.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_50_1_Ao0.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_51_1_YHh.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_52_1_KXN.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_53_1_U4P.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_54_1_HK0.root'
)
fileNamesZtoElecTau_Data_7TeV_part49 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_55_1_tvd.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_56_1_yyw.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_57_1_bFy.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_58_1_8JK.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_59_1_XxI.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_5_1_XJ0.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_60_1_8Zh.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_61_1_kvc.root'
)
fileNamesZtoElecTau_Data_7TeV_part50 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_62_1_Z5O.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_63_1_gVR.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_64_1_seW.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_65_1_86V.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_6_1_JVk.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_7_1_PCA.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_8_1_CGV.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part2/skimElecTau_9_1_oio.root'
)

# dataset = /EG/Run2010A-PromptReco-v4/RECO
# first run = 140400
# last run = 141961
# events before skim = 678124
# events passed = 117145
# lumi_mask = /afs/cernch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV/StreamExpress/Cert_132440-141961_7TeV_StreamExpress_Collisions10_JSON.txt
# lumi = 48.5/nb

# 8.1/nb/job

fileNamesZtoElecTau_Data_7TeV_part51 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_10_1_oj7.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_11_1_Loy.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_12_1_RqL.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_13_1_hsq.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_14_1_oUM.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_15_1_QpB.root'
)
fileNamesZtoElecTau_Data_7TeV_part52 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_16_1_ybn.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_17_1_zxL.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_18_1_q4Q.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_19_1_vEK.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_1_1_c0V.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_20_1_n1G.root'
)
fileNamesZtoElecTau_Data_7TeV_part53 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_21_1_O7A.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_22_1_QJv.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_23_1_Gsm.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_24_1_yA3.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_25_1_XQL.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_26_1_YPd.root'
)
fileNamesZtoElecTau_Data_7TeV_part54 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_27_1_rYR.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_28_1_w0J.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_29_1_bLJ.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_2_1_M5G.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_30_1_Hzk.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_31_1_Wei.root'
)
fileNamesZtoElecTau_Data_7TeV_part55 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_32_1_3TS.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_33_1_v2c.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_34_1_3Yu.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_35_1_0Nz.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_36_1_0r1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_37_1_0r9.root'
)
fileNamesZtoElecTau_Data_7TeV_part56 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_38_1_JrQ.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_39_1_2AG.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_3_1_J02.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_40_1_yg0.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_41_1_w4c.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_4_1_9S7.root'
)
fileNamesZtoElecTau_Data_7TeV_part57 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_5_1_16c.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_6_1_GKZ.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_7_1_4lY.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_8_1_HqG.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part3/skimElecTau_9_1_mYj.root'
)

#
#  part 4
#

# dataset = /EG/Run2010A-PromptReco-v4/RECO
# first run = 142035
# last run = 142461
# events before skim = 
# events passed = 1228169
# lumi_mask = /afs/cernch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV/StreamExpress/Cert_132440-142664_7TeV_StreamExpress_Collisions10_JSON.txt
# lumi = 527/nb

#  12.3/nb/job

fileNamesZtoElecTau_Data_7TeV_part58 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_10_1_dom.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_11_1_Djh.root'
)
fileNamesZtoElecTau_Data_7TeV_part59 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_12_1_hBU.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_13_1_HYH.root'
)
fileNamesZtoElecTau_Data_7TeV_part60 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_14_1_iuQ.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_15_1_USa.root'
)
fileNamesZtoElecTau_Data_7TeV_part61 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_16_1_G47.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_17_1_LvP.root'
)
fileNamesZtoElecTau_Data_7TeV_part62 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_18_1_y6y.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_19_1_NnN.root'
)
fileNamesZtoElecTau_Data_7TeV_part63 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_1_1_9Q2.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_20_1_Gcy.root'
)
fileNamesZtoElecTau_Data_7TeV_part64 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_21_1_792.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_22_1_95p.root'
)
fileNamesZtoElecTau_Data_7TeV_part65 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_23_2_7Eb.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_24_1_aTk.root'
)
fileNamesZtoElecTau_Data_7TeV_part66 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_25_1_Gcb.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_26_1_k23.root'
)
fileNamesZtoElecTau_Data_7TeV_part67 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_27_1_y7G.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_28_1_T49.root'
)
fileNamesZtoElecTau_Data_7TeV_part68 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_29_1_flv.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_2_1_2DC.root'
)
fileNamesZtoElecTau_Data_7TeV_part69 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_30_1_k43.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_31_1_6wI.root'
)
fileNamesZtoElecTau_Data_7TeV_part70 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_32_1_uTX.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_33_1_E1l.root'
)
fileNamesZtoElecTau_Data_7TeV_part71 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_34_1_sz2.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_35_1_atV.root'
)
fileNamesZtoElecTau_Data_7TeV_part72 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_36_1_yxO.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_37_1_W0c.root'
)
fileNamesZtoElecTau_Data_7TeV_part73 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_38_1_71K.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_39_1_ir4.root'
)
fileNamesZtoElecTau_Data_7TeV_part74 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_3_1_1LF.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_40_1_sLF.root'
)
fileNamesZtoElecTau_Data_7TeV_part75 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_41_1_49e.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_42_1_heH.root'
)
fileNamesZtoElecTau_Data_7TeV_part76 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_43_1_xxa.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_44_1_bar.root'
)
fileNamesZtoElecTau_Data_7TeV_part77 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_45_1_TVg.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_46_1_DcO.root'
)
fileNamesZtoElecTau_Data_7TeV_part78 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_47_1_gUg.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_48_1_Ekc.root'
)
fileNamesZtoElecTau_Data_7TeV_part79 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_49_1_1QZ.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_4_1_6nk.root'
)
fileNamesZtoElecTau_Data_7TeV_part80 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_50_1_MAP.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_51_1_hQP.root'
)
fileNamesZtoElecTau_Data_7TeV_part81 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_52_1_Cs5.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_53_1_dGN.root'
)
fileNamesZtoElecTau_Data_7TeV_part82 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_54_2_3pP.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_55_1_j3u.root'
)
fileNamesZtoElecTau_Data_7TeV_part83 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_56_2_f2g.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_57_1_Pkq.root'
)
fileNamesZtoElecTau_Data_7TeV_part84 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_58_1_4ih.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_59_1_FhK.root'
)
fileNamesZtoElecTau_Data_7TeV_part85 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_5_1_zCb.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_60_2_e6D.root'
)
fileNamesZtoElecTau_Data_7TeV_part86 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_61_1_Il2.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_62_1_iPa.root'
)
fileNamesZtoElecTau_Data_7TeV_part87 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_63_2_SGr.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_64_1_D5X.root'
)
fileNamesZtoElecTau_Data_7TeV_part88 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_65_1_D4o.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_66_1_TyG.root'
)
fileNamesZtoElecTau_Data_7TeV_part89 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_67_1_2tT.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_68_1_1R8.root'
)
fileNamesZtoElecTau_Data_7TeV_part90 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_69_1_RYt.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_6_1_BIx.root'
)
fileNamesZtoElecTau_Data_7TeV_part91 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_70_1_adc.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_71_1_AHf.root'
)
fileNamesZtoElecTau_Data_7TeV_part92 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_72_1_1aX.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_73_1_Ij4.root'
)
fileNamesZtoElecTau_Data_7TeV_part93 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_74_1_ZgL.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_75_1_jy5.root'
)
fileNamesZtoElecTau_Data_7TeV_part94 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_76_1_day.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_77_1_1r5.root'
)
fileNamesZtoElecTau_Data_7TeV_part95 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_78_2_Xgh.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_79_1_zs8.root'
)
fileNamesZtoElecTau_Data_7TeV_part96 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_7_1_Rye.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_80_1_ZxZ.root'
)
fileNamesZtoElecTau_Data_7TeV_part97 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_81_1_nrU.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_82_1_Nlw.root'
)
fileNamesZtoElecTau_Data_7TeV_part98 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_83_1_Uis.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_84_1_J8l.root'
)
fileNamesZtoElecTau_Data_7TeV_part99 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_85_1_0Fc.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_86_1_7wI.root'
)
fileNamesZtoElecTau_Data_7TeV_part100 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_87_1_cwX.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_8_1_rLF.root'
)
fileNamesZtoElecTau_Data_7TeV_part101 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part4/skimElecTau_9_1_Ejq.root'
)

#   part5
#   255.2/nb  -> 4/nb/job

fileNamesZtoElecTau_Data_7TeV_part102 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_100_1_7Vs.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_101_1_pCT.root'
)
fileNamesZtoElecTau_Data_7TeV_part103 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_102_1_u5z.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_103_1_wpK.root'
)
fileNamesZtoElecTau_Data_7TeV_part104 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_104_1_wSc.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_105_1_3wk.root'
)
fileNamesZtoElecTau_Data_7TeV_part105 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_106_1_IDl.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_107_1_Blt.root'
)
fileNamesZtoElecTau_Data_7TeV_part106 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_108_1_h4E.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_109_1_OyA.root'
)
fileNamesZtoElecTau_Data_7TeV_part107 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_10_1_g0U.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_110_1_qot.root'
)
fileNamesZtoElecTau_Data_7TeV_part108 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_111_1_Q0t.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_112_1_xFb.root'
)
fileNamesZtoElecTau_Data_7TeV_part109 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_113_1_Ajd.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_114_1_9ul.root'
)
fileNamesZtoElecTau_Data_7TeV_part110 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_115_1_R2W.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_116_1_YDV.root'
)
fileNamesZtoElecTau_Data_7TeV_part111 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_117_1_Ozf.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_118_1_IZs.root'
)
fileNamesZtoElecTau_Data_7TeV_part112 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_119_1_MMQ.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_11_1_fCY.root'
)
fileNamesZtoElecTau_Data_7TeV_part113 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_120_1_MyT.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_121_1_gsn.root'
)
fileNamesZtoElecTau_Data_7TeV_part114 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_122_1_c9l.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_123_1_1GE.root'
)
fileNamesZtoElecTau_Data_7TeV_part115 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_124_1_0XE.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_125_1_6WS.root'
)
fileNamesZtoElecTau_Data_7TeV_part116 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_126_1_RKh.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_127_1_467.root'
)
fileNamesZtoElecTau_Data_7TeV_part117 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_128_1_QKw.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_129_1_YyI.root'
)
fileNamesZtoElecTau_Data_7TeV_part118 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_12_1_6wV.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_130_1_6Sg.root'
)
fileNamesZtoElecTau_Data_7TeV_part119 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_131_1_fds.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_13_1_qQb.root'
)
fileNamesZtoElecTau_Data_7TeV_part120 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_14_1_atG.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_15_1_T7E.root'
)
fileNamesZtoElecTau_Data_7TeV_part121 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_16_1_aQS.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_17_1_wn6.root'
)
fileNamesZtoElecTau_Data_7TeV_part122 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_18_1_ekk.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_19_1_wfR.root'
)
fileNamesZtoElecTau_Data_7TeV_part123 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_1_1_b93.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_20_1_skC.root'
)
fileNamesZtoElecTau_Data_7TeV_part124 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_21_1_wHP.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_22_1_UkO.root'
)
fileNamesZtoElecTau_Data_7TeV_part125 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_23_1_AVm.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_24_1_QBu.root'
)
fileNamesZtoElecTau_Data_7TeV_part126 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_25_1_eRg.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_26_1_7RH.root'
)
fileNamesZtoElecTau_Data_7TeV_part127 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_27_1_EVK.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_28_1_PBw.root'
)
fileNamesZtoElecTau_Data_7TeV_part128 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_29_1_I5V.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_2_1_6of.root'
)
fileNamesZtoElecTau_Data_7TeV_part129 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_30_1_BAA.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_31_1_E29.root'
)
fileNamesZtoElecTau_Data_7TeV_part130 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_32_1_tzi.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_33_1_hWj.root'
)
fileNamesZtoElecTau_Data_7TeV_part131 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_34_1_cGI.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_35_1_TVA.root'
)
fileNamesZtoElecTau_Data_7TeV_part132 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_36_1_ouq.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_37_1_D75.root'
)
fileNamesZtoElecTau_Data_7TeV_part133 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_38_1_jas.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_39_1_V7o.root'
)
fileNamesZtoElecTau_Data_7TeV_part134 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_3_1_n4s.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_40_1_1P6.root'
)
fileNamesZtoElecTau_Data_7TeV_part135 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_41_1_46O.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_42_1_PFT.root'
)
fileNamesZtoElecTau_Data_7TeV_part136 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_43_1_bAJ.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_44_1_DyL.root'
)
fileNamesZtoElecTau_Data_7TeV_part137 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_45_1_LG5.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_46_1_2Uk.root'
)
fileNamesZtoElecTau_Data_7TeV_part138 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_47_1_Zv2.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_48_1_IBI.root'
)
fileNamesZtoElecTau_Data_7TeV_part139 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_49_1_7nb.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_4_1_JeL.root'
)
fileNamesZtoElecTau_Data_7TeV_part140 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_50_1_SOU.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_51_1_yLU.root'
)
fileNamesZtoElecTau_Data_7TeV_part141 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_52_1_16M.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_53_1_4E2.root'
)
fileNamesZtoElecTau_Data_7TeV_part142 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_54_1_Pmg.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_55_1_0dS.root'
)
fileNamesZtoElecTau_Data_7TeV_part143 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_56_1_ixM.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_57_1_zpQ.root'
)
fileNamesZtoElecTau_Data_7TeV_part144 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_58_1_MCZ.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_59_1_jao.root'
)
fileNamesZtoElecTau_Data_7TeV_part145 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_5_1_CTE.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_60_1_7Oc.root'
)
fileNamesZtoElecTau_Data_7TeV_part146 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_61_1_218.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_62_1_3ne.root'
)
fileNamesZtoElecTau_Data_7TeV_part147 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_63_1_dDF.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_64_1_63S.root'
)
fileNamesZtoElecTau_Data_7TeV_part148 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_65_1_hlF.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_66_1_q7x.root'
)
fileNamesZtoElecTau_Data_7TeV_part149 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_67_1_AGN.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_68_1_6bV.root'
)
fileNamesZtoElecTau_Data_7TeV_part150 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_69_1_PCa.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_6_1_cD9.root'
)
fileNamesZtoElecTau_Data_7TeV_part151 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_70_1_Tx9.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_71_1_xAZ.root'
)
fileNamesZtoElecTau_Data_7TeV_part152 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_72_1_rkL.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_73_1_uwO.root'
)
fileNamesZtoElecTau_Data_7TeV_part153 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_74_1_ZVf.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_75_1_dl8.root'
)
fileNamesZtoElecTau_Data_7TeV_part154 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_76_1_NOq.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_77_1_arw.root'
)
fileNamesZtoElecTau_Data_7TeV_part155 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_78_1_Kgt.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_79_1_i4a.root'
)
fileNamesZtoElecTau_Data_7TeV_part156 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_7_1_4R3.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_80_1_LKc.root'
)
fileNamesZtoElecTau_Data_7TeV_part157 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_81_1_weH.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_82_1_YeO.root'
)
fileNamesZtoElecTau_Data_7TeV_part158 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_83_1_iYK.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_84_1_bwM.root'
)
fileNamesZtoElecTau_Data_7TeV_part159 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_85_1_Wyh.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_86_1_DNV.root'
)
fileNamesZtoElecTau_Data_7TeV_part160 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_87_1_CMR.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_88_1_153.root'
)
fileNamesZtoElecTau_Data_7TeV_part161 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_89_1_R1g.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_8_1_1uT.root'
)
fileNamesZtoElecTau_Data_7TeV_part162 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_90_1_p02.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_91_1_DJA.root'
)
fileNamesZtoElecTau_Data_7TeV_part163 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_92_1_9b6.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_93_1_Php.root'
)
fileNamesZtoElecTau_Data_7TeV_part164 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_94_1_c6P.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_95_1_mFv.root'
)
fileNamesZtoElecTau_Data_7TeV_part165 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_96_1_ErY.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_97_1_I4C.root'
)
fileNamesZtoElecTau_Data_7TeV_part166 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_98_1_sYe.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_99_1_KIv.root'
)
fileNamesZtoElecTau_Data_7TeV_part167 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part5/skimElecTau_9_1_ZMP.root'
)

#  part 6
#  lumi = 230/nb -> 3.8/nb/job

fileNamesZtoElecTau_Data_7TeV_part168 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_100_1_Nou.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_101_1_2NX.root'
)
fileNamesZtoElecTau_Data_7TeV_part169 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_102_1_xff.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_103_1_vKh.root'
)
fileNamesZtoElecTau_Data_7TeV_part170 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_104_1_XLd.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_105_1_txF.root'
)
fileNamesZtoElecTau_Data_7TeV_part171 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_106_1_DF7.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_107_1_h2E.root'
)
fileNamesZtoElecTau_Data_7TeV_part172 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_108_1_Nne.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_109_1_lIV.root'
)
fileNamesZtoElecTau_Data_7TeV_part173 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_10_1_Rxj.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_110_1_RIX.root'
)
fileNamesZtoElecTau_Data_7TeV_part174 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_111_1_oy4.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_112_1_wbU.root'
)
fileNamesZtoElecTau_Data_7TeV_part175 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_113_1_TlT.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_114_1_iE5.root'
)
fileNamesZtoElecTau_Data_7TeV_part176 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_115_1_YtS.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_116_1_BWy.root'
)
fileNamesZtoElecTau_Data_7TeV_part177 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_117_1_1Wh.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_118_1_zQx.root'
)
fileNamesZtoElecTau_Data_7TeV_part178 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_119_1_DTd.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_11_1_3Ad.root'
)
fileNamesZtoElecTau_Data_7TeV_part179 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_120_1_ARd.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_121_1_yNO.root'
)
fileNamesZtoElecTau_Data_7TeV_part180 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_122_1_W3D.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_12_1_nVL.root'
)
fileNamesZtoElecTau_Data_7TeV_part181 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_13_1_bl6.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_14_1_59W.root'
)
fileNamesZtoElecTau_Data_7TeV_part182 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_15_1_9fF.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_16_1_1WX.root'
)
fileNamesZtoElecTau_Data_7TeV_part183 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_17_1_8HB.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_18_1_ki2.root'
)
fileNamesZtoElecTau_Data_7TeV_part184 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_19_1_WL8.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_1_1_twx.root'
)
fileNamesZtoElecTau_Data_7TeV_part185 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_20_1_6tn.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_21_1_gZ8.root'
)
fileNamesZtoElecTau_Data_7TeV_part186 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_22_1_jji.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_23_1_4M2.root'
)
fileNamesZtoElecTau_Data_7TeV_part187 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_24_1_HwX.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_25_1_zbA.root'
)
fileNamesZtoElecTau_Data_7TeV_part188 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_26_1_IEj.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_27_1_M9O.root'
)
fileNamesZtoElecTau_Data_7TeV_part189 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_28_1_m7X.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_29_1_S3g.root'
)
fileNamesZtoElecTau_Data_7TeV_part190 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_2_1_Lms.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_30_1_mxY.root'
)
fileNamesZtoElecTau_Data_7TeV_part191 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_31_1_8D5.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_32_1_uhJ.root'
)
fileNamesZtoElecTau_Data_7TeV_part192 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_33_1_bKW.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_34_1_wFu.root'
)
fileNamesZtoElecTau_Data_7TeV_part193 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_35_1_gJX.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_36_1_xwf.root'
)
fileNamesZtoElecTau_Data_7TeV_part194 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_37_1_nsP.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_38_1_jH6.root'
)
fileNamesZtoElecTau_Data_7TeV_part195 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_39_1_53S.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_3_1_2eh.root'
)
fileNamesZtoElecTau_Data_7TeV_part196 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_40_1_v29.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_41_1_VN1.root'
)
fileNamesZtoElecTau_Data_7TeV_part197 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_42_1_DS6.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_43_1_PhJ.root'
)
fileNamesZtoElecTau_Data_7TeV_part198 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_44_1_n7C.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_45_1_i1b.root'
)
fileNamesZtoElecTau_Data_7TeV_part199 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_46_1_J2W.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_47_1_Hw6.root'
)
fileNamesZtoElecTau_Data_7TeV_part200 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_48_1_Fy5.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_49_1_Dce.root'
)
fileNamesZtoElecTau_Data_7TeV_part201 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_4_1_5Xo.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_50_1_j7X.root'
)
fileNamesZtoElecTau_Data_7TeV_part202 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_51_1_6Zu.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_52_1_aND.root'
)
fileNamesZtoElecTau_Data_7TeV_part203 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_53_1_EX1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_54_1_I1K.root'
)
fileNamesZtoElecTau_Data_7TeV_part204 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_55_1_wRf.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_56_1_YZ5.root'
)
fileNamesZtoElecTau_Data_7TeV_part205 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_57_1_rZR.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_58_1_KsH.root'
)
fileNamesZtoElecTau_Data_7TeV_part206 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_59_1_Sk7.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_5_1_6MR.root'
)
fileNamesZtoElecTau_Data_7TeV_part207 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_60_1_ByI.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_61_1_aWg.root'
)
fileNamesZtoElecTau_Data_7TeV_part208 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_62_1_1Es.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_63_1_KqE.root'
)
fileNamesZtoElecTau_Data_7TeV_part209 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_64_1_4ZS.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_66_1_rZx.root'
)
fileNamesZtoElecTau_Data_7TeV_part210 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_67_1_UfM.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_68_1_keC.root'
)
fileNamesZtoElecTau_Data_7TeV_part211 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_69_1_Jxu.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_6_1_21U.root'
)
fileNamesZtoElecTau_Data_7TeV_part212 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_70_1_8vs.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_71_1_wsf.root'
)
fileNamesZtoElecTau_Data_7TeV_part213 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_72_1_Oxf.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_73_1_ddU.root'
)
fileNamesZtoElecTau_Data_7TeV_part214 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_74_1_NYq.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_75_1_FG0.root'
)
fileNamesZtoElecTau_Data_7TeV_part215 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_76_1_C1X.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_77_1_rjx.root'
)
fileNamesZtoElecTau_Data_7TeV_part216 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_78_1_HGT.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_79_1_TO0.root'
)
fileNamesZtoElecTau_Data_7TeV_part217 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_7_1_s2G.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_80_1_aJO.root'
)
fileNamesZtoElecTau_Data_7TeV_part218 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_81_1_Mdo.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_82_1_o3U.root'
)
fileNamesZtoElecTau_Data_7TeV_part219 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_83_1_PC9.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_84_1_FpI.root'
)
fileNamesZtoElecTau_Data_7TeV_part220 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_85_1_MLX.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_86_1_EU7.root'
)
fileNamesZtoElecTau_Data_7TeV_part221 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_87_1_Q2a.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_88_1_33s.root'
)
fileNamesZtoElecTau_Data_7TeV_part222 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_89_1_Plt.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_8_1_fIp.root'
)
fileNamesZtoElecTau_Data_7TeV_part223 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_90_1_0w9.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_91_1_q0K.root'
)
fileNamesZtoElecTau_Data_7TeV_part224 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_92_1_7BL.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_93_1_wnq.root'
)
fileNamesZtoElecTau_Data_7TeV_part225 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_94_1_zzi.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_95_1_UvC.root'
)
fileNamesZtoElecTau_Data_7TeV_part226 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_96_1_zs8.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_97_1_umY.root'
)
fileNamesZtoElecTau_Data_7TeV_part227 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_98_1_MYD.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_99_1_8HA.root'
)
fileNamesZtoElecTau_Data_7TeV_part228 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_9_1_jFG.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part6/skimElecTau_65_2_opf.root'
)

#  part 7
# ~1235/nb

# 176 jobs -> 7.0/nb/job

fileNamesZtoElecTau_Data_7TeV_part229 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_100_3_YLm.root'
)
fileNamesZtoElecTau_Data_7TeV_part230 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_101_3_8yF.root'
)
fileNamesZtoElecTau_Data_7TeV_part231 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_102_1_uI2.root'
)
fileNamesZtoElecTau_Data_7TeV_part232 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_103_2_hO9.root'
)
fileNamesZtoElecTau_Data_7TeV_part233 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_104_3_y0s.root'
)
fileNamesZtoElecTau_Data_7TeV_part234 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_105_1_gkh.root'
)
fileNamesZtoElecTau_Data_7TeV_part235 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_106_1_ore.root'
)
fileNamesZtoElecTau_Data_7TeV_part236 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_107_1_lB9.root'
)
fileNamesZtoElecTau_Data_7TeV_part237 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_108_3_DQT.root'
)
fileNamesZtoElecTau_Data_7TeV_part238 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_109_2_IDr.root'
)
fileNamesZtoElecTau_Data_7TeV_part239 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_10_2_0WT.root'
)
fileNamesZtoElecTau_Data_7TeV_part240 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_110_2_DEh.root'
)
fileNamesZtoElecTau_Data_7TeV_part241 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_111_3_ALv.root'
)
fileNamesZtoElecTau_Data_7TeV_part242 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_112_2_XHB.root'
)
fileNamesZtoElecTau_Data_7TeV_part243 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_113_1_o10.root'
)
fileNamesZtoElecTau_Data_7TeV_part244 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_114_3_Wkp.root'
)
fileNamesZtoElecTau_Data_7TeV_part245 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_115_2_Nlf.root'
)
fileNamesZtoElecTau_Data_7TeV_part246 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_116_3_T1B.root'
)
fileNamesZtoElecTau_Data_7TeV_part247 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_117_2_TxK.root'
)
fileNamesZtoElecTau_Data_7TeV_part248 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_119_2_kHe.root'
)
fileNamesZtoElecTau_Data_7TeV_part249 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_11_2_iff.root'
)
fileNamesZtoElecTau_Data_7TeV_part250 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_120_1_HrZ.root'
)
fileNamesZtoElecTau_Data_7TeV_part251 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_121_2_aYZ.root'
)
fileNamesZtoElecTau_Data_7TeV_part252 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_122_2_7N1.root'
)
fileNamesZtoElecTau_Data_7TeV_part253 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_123_2_4NP.root'
)
fileNamesZtoElecTau_Data_7TeV_part254 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_124_3_rUV.root'
)
fileNamesZtoElecTau_Data_7TeV_part255 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_126_3_lcu.root'
)
fileNamesZtoElecTau_Data_7TeV_part256 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_127_3_Kks.root'
)
fileNamesZtoElecTau_Data_7TeV_part257 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_128_4_wzO.root'
)
fileNamesZtoElecTau_Data_7TeV_part258 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_129_2_fBv.root'
)
fileNamesZtoElecTau_Data_7TeV_part259 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_12_3_Rc2.root'
)
fileNamesZtoElecTau_Data_7TeV_part260 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_130_3_ewu.root'
)
fileNamesZtoElecTau_Data_7TeV_part261 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_131_2_lQF.root'
)
fileNamesZtoElecTau_Data_7TeV_part262 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_132_2_n7V.root'
)
fileNamesZtoElecTau_Data_7TeV_part263 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_133_3_JW7.root'
)
fileNamesZtoElecTau_Data_7TeV_part264 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_134_1_puX.root'
)
fileNamesZtoElecTau_Data_7TeV_part265 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_135_2_hmA.root'
)
fileNamesZtoElecTau_Data_7TeV_part266 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_136_2_nAY.root'
)
fileNamesZtoElecTau_Data_7TeV_part267 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_137_1_t5n.root'
)
fileNamesZtoElecTau_Data_7TeV_part268 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_138_1_jD0.root'
)
fileNamesZtoElecTau_Data_7TeV_part269 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_139_1_Zs3.root'
)
fileNamesZtoElecTau_Data_7TeV_part270 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_13_2_1LU.root'
)
fileNamesZtoElecTau_Data_7TeV_part271 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_140_2_nMC.root'
)
fileNamesZtoElecTau_Data_7TeV_part272 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_141_3_bcZ.root'
)
fileNamesZtoElecTau_Data_7TeV_part273 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_142_3_9cE.root'
)
fileNamesZtoElecTau_Data_7TeV_part274 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_143_1_mV1.root'
)
fileNamesZtoElecTau_Data_7TeV_part275 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_145_3_DdG.root'
)
fileNamesZtoElecTau_Data_7TeV_part276 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_146_1_vcE.root'
)
fileNamesZtoElecTau_Data_7TeV_part277 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_147_2_urt.root'
)
fileNamesZtoElecTau_Data_7TeV_part278 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_148_3_xHS.root'
)
fileNamesZtoElecTau_Data_7TeV_part279 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_149_2_1o8.root'
)
fileNamesZtoElecTau_Data_7TeV_part280 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_14_3_c9M.root'
)
fileNamesZtoElecTau_Data_7TeV_part281 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_150_2_ndt.root'
)
fileNamesZtoElecTau_Data_7TeV_part282 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_151_3_OtH.root'
)
fileNamesZtoElecTau_Data_7TeV_part283 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_152_2_MqS.root'
)
fileNamesZtoElecTau_Data_7TeV_part284 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_153_3_xlq.root'
)
fileNamesZtoElecTau_Data_7TeV_part285 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_154_2_alb.root'
)
fileNamesZtoElecTau_Data_7TeV_part286 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_155_2_tT8.root'
)
fileNamesZtoElecTau_Data_7TeV_part287 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_156_1_7HF.root'
)
fileNamesZtoElecTau_Data_7TeV_part288 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_157_3_KrX.root'
)
fileNamesZtoElecTau_Data_7TeV_part289 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_158_1_35E.root'
)
fileNamesZtoElecTau_Data_7TeV_part290 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_159_1_D1Y.root'
)
fileNamesZtoElecTau_Data_7TeV_part291 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_15_3_Jgo.root'
)
fileNamesZtoElecTau_Data_7TeV_part292 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_160_1_zXv.root'
)
fileNamesZtoElecTau_Data_7TeV_part293 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_161_2_yN2.root'
)
fileNamesZtoElecTau_Data_7TeV_part294 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_162_2_jZx.root'
)
fileNamesZtoElecTau_Data_7TeV_part295 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_163_1_RCj.root'
)
fileNamesZtoElecTau_Data_7TeV_part296 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_164_3_4Q7.root'
)
fileNamesZtoElecTau_Data_7TeV_part297 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_165_1_KrM.root'
)
fileNamesZtoElecTau_Data_7TeV_part298 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_166_3_t1r.root'
)
fileNamesZtoElecTau_Data_7TeV_part299 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_167_1_uln.root'
)
fileNamesZtoElecTau_Data_7TeV_part300 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_168_2_XqV.root'
)
fileNamesZtoElecTau_Data_7TeV_part301 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_169_2_yP4.root'
)
fileNamesZtoElecTau_Data_7TeV_part302 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_16_2_GqV.root'
)
fileNamesZtoElecTau_Data_7TeV_part303 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_170_1_XJy.root'
)
fileNamesZtoElecTau_Data_7TeV_part304 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_171_3_5bS.root'
)
fileNamesZtoElecTau_Data_7TeV_part305 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_172_1_nF9.root'
)
fileNamesZtoElecTau_Data_7TeV_part306 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_173_4_NQ9.root'
)
fileNamesZtoElecTau_Data_7TeV_part307 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_174_1_IYb.root'
)
fileNamesZtoElecTau_Data_7TeV_part308 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_175_1_0jE.root'
)
fileNamesZtoElecTau_Data_7TeV_part309 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_176_2_NVh.root'
)
fileNamesZtoElecTau_Data_7TeV_part310 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_17_2_oTR.root'
)
fileNamesZtoElecTau_Data_7TeV_part311 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_18_2_5QE.root'
)
fileNamesZtoElecTau_Data_7TeV_part312 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_19_3_dsy.root'
)
fileNamesZtoElecTau_Data_7TeV_part313 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_1_1_xUO.root'
)
fileNamesZtoElecTau_Data_7TeV_part314 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_20_3_3R6.root'
)
fileNamesZtoElecTau_Data_7TeV_part315 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_21_4_iAv.root'
)
fileNamesZtoElecTau_Data_7TeV_part316 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_22_2_kvW.root'
)
fileNamesZtoElecTau_Data_7TeV_part317 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_23_3_y75.root'
)
fileNamesZtoElecTau_Data_7TeV_part318 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_24_3_9kL.root'
)
fileNamesZtoElecTau_Data_7TeV_part319 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_25_2_Hsm.root'
)
fileNamesZtoElecTau_Data_7TeV_part320 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_26_4_ntN.root'
)
fileNamesZtoElecTau_Data_7TeV_part321 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_27_1_mPA.root'
)
fileNamesZtoElecTau_Data_7TeV_part322 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_28_4_Oje.root'
)
fileNamesZtoElecTau_Data_7TeV_part323 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_29_3_lFX.root'
)
fileNamesZtoElecTau_Data_7TeV_part324 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_2_3_WSv.root'
)
fileNamesZtoElecTau_Data_7TeV_part325 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_30_3_uYu.root'
)
fileNamesZtoElecTau_Data_7TeV_part326 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_31_2_gIz.root'
)
fileNamesZtoElecTau_Data_7TeV_part327 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_32_2_C25.root'
)
fileNamesZtoElecTau_Data_7TeV_part328 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_33_2_vP6.root'
)
fileNamesZtoElecTau_Data_7TeV_part329 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_34_2_xNQ.root'
)
fileNamesZtoElecTau_Data_7TeV_part330 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_35_1_B8O.root'
)
fileNamesZtoElecTau_Data_7TeV_part331 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_36_2_2zy.root'
)
fileNamesZtoElecTau_Data_7TeV_part332 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_37_3_q6p.root'
)
fileNamesZtoElecTau_Data_7TeV_part333 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_38_2_vJM.root'
)
fileNamesZtoElecTau_Data_7TeV_part334 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_39_2_S9s.root'
)
fileNamesZtoElecTau_Data_7TeV_part335 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_40_2_Svs.root'
)
fileNamesZtoElecTau_Data_7TeV_part336 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_41_3_Van.root'
)
fileNamesZtoElecTau_Data_7TeV_part337 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_42_2_IQM.root'
)
fileNamesZtoElecTau_Data_7TeV_part338 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_43_3_OCo.root'
)
fileNamesZtoElecTau_Data_7TeV_part339 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_44_4_VDE.root'
)
fileNamesZtoElecTau_Data_7TeV_part340 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_45_3_keA.root'
)
fileNamesZtoElecTau_Data_7TeV_part341 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_46_4_bUy.root'
)
fileNamesZtoElecTau_Data_7TeV_part342 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_47_1_XKN.root'
)
fileNamesZtoElecTau_Data_7TeV_part343 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_48_1_cnH.root'
)
fileNamesZtoElecTau_Data_7TeV_part344 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_49_1_Mi8.root'
)
fileNamesZtoElecTau_Data_7TeV_part345 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_4_2_uZO.root'
)
fileNamesZtoElecTau_Data_7TeV_part346 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_50_2_uTe.root'
)
fileNamesZtoElecTau_Data_7TeV_part347 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_51_1_GTV.root'
)
fileNamesZtoElecTau_Data_7TeV_part348 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_52_2_TQn.root'
)
fileNamesZtoElecTau_Data_7TeV_part349 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_53_1_Jx3.root'
)
fileNamesZtoElecTau_Data_7TeV_part350 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_54_2_9yq.root'
)
fileNamesZtoElecTau_Data_7TeV_part351 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_55_2_a9r.root'
)
fileNamesZtoElecTau_Data_7TeV_part352 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_56_3_ddn.root'
)
fileNamesZtoElecTau_Data_7TeV_part353 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_57_1_wxr.root'
)
fileNamesZtoElecTau_Data_7TeV_part354 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_58_1_vSb.root'
)
fileNamesZtoElecTau_Data_7TeV_part355 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_59_2_cwT.root'
)
fileNamesZtoElecTau_Data_7TeV_part356 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_5_3_ZIQ.root'
)
fileNamesZtoElecTau_Data_7TeV_part357 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_60_2_XFz.root'
)
fileNamesZtoElecTau_Data_7TeV_part358 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_61_4_0TF.root'
)
fileNamesZtoElecTau_Data_7TeV_part359 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_62_2_Dc9.root'
)
fileNamesZtoElecTau_Data_7TeV_part360 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_63_1_t10.root'
)
fileNamesZtoElecTau_Data_7TeV_part361 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_64_1_C4d.root'
)
fileNamesZtoElecTau_Data_7TeV_part362 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_65_3_DUD.root'
)
fileNamesZtoElecTau_Data_7TeV_part363 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_66_3_uvO.root'
)
fileNamesZtoElecTau_Data_7TeV_part364 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_68_1_KNF.root'
)
fileNamesZtoElecTau_Data_7TeV_part365 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_69_3_fd2.root'
)
fileNamesZtoElecTau_Data_7TeV_part366 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_6_2_5s0.root'
)
fileNamesZtoElecTau_Data_7TeV_part367 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_71_3_OH4.root'
)
fileNamesZtoElecTau_Data_7TeV_part368 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_72_2_5gB.root'
)
fileNamesZtoElecTau_Data_7TeV_part369 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_73_1_svB.root'
)
fileNamesZtoElecTau_Data_7TeV_part370 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_74_1_Ntb.root'
)
fileNamesZtoElecTau_Data_7TeV_part371 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_75_3_782.root'
)
fileNamesZtoElecTau_Data_7TeV_part372 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_76_1_P5k.root'
)
fileNamesZtoElecTau_Data_7TeV_part373 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_77_2_Ukr.root'
)
fileNamesZtoElecTau_Data_7TeV_part374 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_78_3_aEK.root'
)
fileNamesZtoElecTau_Data_7TeV_part375 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_79_3_WRW.root'
)
fileNamesZtoElecTau_Data_7TeV_part376 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_7_3_pMH.root'
)
fileNamesZtoElecTau_Data_7TeV_part377 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_80_3_vsr.root'
)
fileNamesZtoElecTau_Data_7TeV_part378 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_81_2_TcG.root'
)
fileNamesZtoElecTau_Data_7TeV_part379 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_82_2_7VU.root'
)
fileNamesZtoElecTau_Data_7TeV_part380 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_83_3_59M.root'
)
fileNamesZtoElecTau_Data_7TeV_part381 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_84_1_OXh.root'
)
fileNamesZtoElecTau_Data_7TeV_part382 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_85_3_hNc.root'
)
fileNamesZtoElecTau_Data_7TeV_part383 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_86_1_vvL.root'
)
fileNamesZtoElecTau_Data_7TeV_part384 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_87_3_VwU.root'
)
fileNamesZtoElecTau_Data_7TeV_part385 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_88_2_jUj.root'
)
fileNamesZtoElecTau_Data_7TeV_part386 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_89_2_5ex.root'
)
fileNamesZtoElecTau_Data_7TeV_part387 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_8_3_rWc.root'
)
fileNamesZtoElecTau_Data_7TeV_part388 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_90_2_ahU.root'
)
fileNamesZtoElecTau_Data_7TeV_part389 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_91_2_QAH.root'
)
fileNamesZtoElecTau_Data_7TeV_part390 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_92_2_Zqa.root'
)
fileNamesZtoElecTau_Data_7TeV_part391 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_93_4_yJY.root'
)
fileNamesZtoElecTau_Data_7TeV_part392 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_94_3_HqM.root'
)
fileNamesZtoElecTau_Data_7TeV_part393 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_95_3_GVk.root'
)
fileNamesZtoElecTau_Data_7TeV_part394 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_96_1_CNZ.root'
)
fileNamesZtoElecTau_Data_7TeV_part395 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_97_3_tIg.root'
)
fileNamesZtoElecTau_Data_7TeV_part396 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_98_3_UtH.root'
)
fileNamesZtoElecTau_Data_7TeV_part397 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_99_2_Ncp.root'
)

#  extra files

fileNamesZtoElecTau_Data_7TeV_part398 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_9_3_7IW.root'
)
fileNamesZtoElecTau_Data_7TeV_part399 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_67_3_4Zs.root'
)
fileNamesZtoElecTau_Data_7TeV_part400 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_118_3_Sq4.root'
)
fileNamesZtoElecTau_Data_7TeV_part401 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_3_3_05X.root'
)
fileNamesZtoElecTau_Data_7TeV_part402 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_70_3_Mfr.root'
)
fileNamesZtoElecTau_Data_7TeV_part403 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_125_3_pws.root'
)
fileNamesZtoElecTau_Data_7TeV_part404 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part7/skimElecTau_144_3_H9L.root'
)


#  part 8
#  86.8/nb

#  13 jobs -> 6.7/nb/job

fileNamesZtoElecTau_Data_7TeV_part405 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part8/skimElecTau_10_1_52D.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part8/skimElecTau_11_1_EUb.root'
)
fileNamesZtoElecTau_Data_7TeV_part406 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part8/skimElecTau_12_1_o6K.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part8/skimElecTau_13_1_KEU.root'
)
fileNamesZtoElecTau_Data_7TeV_part407 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part8/skimElecTau_14_1_RQL.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part8/skimElecTau_15_1_5VW.root'
)
fileNamesZtoElecTau_Data_7TeV_part408 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part8/skimElecTau_16_1_htE.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part8/skimElecTau_17_1_Z7v.root'
)
fileNamesZtoElecTau_Data_7TeV_part409 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part8/skimElecTau_18_1_jnt.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part8/skimElecTau_19_1_Efw.root'
)
fileNamesZtoElecTau_Data_7TeV_part410 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part8/skimElecTau_1_1_6rN.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part8/skimElecTau_20_1_XSm.root'
)
fileNamesZtoElecTau_Data_7TeV_part411 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part8/skimElecTau_21_1_b5k.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part8/skimElecTau_22_1_89U.root'
)
fileNamesZtoElecTau_Data_7TeV_part412 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part8/skimElecTau_23_1_eX3.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part8/skimElecTau_24_1_Y5N.root'
)
fileNamesZtoElecTau_Data_7TeV_part413 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part8/skimElecTau_25_1_o1y.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part8/skimElecTau_26_1_CtT.root'
)
fileNamesZtoElecTau_Data_7TeV_part414 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part8/skimElecTau_2_1_kSR.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part8/skimElecTau_3_1_lX0.root'
)
fileNamesZtoElecTau_Data_7TeV_part415 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part8/skimElecTau_4_1_HSZ.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part8/skimElecTau_5_1_u09.root'
)
fileNamesZtoElecTau_Data_7TeV_part416 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part8/skimElecTau_6_1_45j.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part8/skimElecTau_7_1_JUf.root'
)
fileNamesZtoElecTau_Data_7TeV_part417 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part8/skimElecTau_8_1_Ggc.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/spring10/data/EG_PD_PR_v4_part8/skimElecTau_9_1_Dsd.root'
)




#--------------------------------------------------------------------------------

##################
#
# 7 TeV MC samples
#
##################



#--------------------------------------------------------------------------------
# Z --> tau+ tau- sample generated with Pythia + Tauola (all decay modes)
#
#  7 TeV
#  parent dataset = /Ztautau/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO
#  skimmed dataset = /Ztautau/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-elecTauSkim/USER
#  generated events = 2195255
#  skimmed events = 405226
#
intLumiZtoElecTau_Ztautau_7TeV = float(1688.7)
# 1/21 files missing
corrFactorZtoElecTau_Ztautau_7TeV = float(1.05)

patTupleOutputFileNameZtoElecTau_Ztautau_7TeV = cms.untracked.string('patTupleZtoElecTau_Ztautau_7TeV_partXX.root')
plotsOutputFileNameZtoElecTau_Ztautau_7TeV = cms.string('plotsZtoElecTau_Ztautau_7TeV_partXX.root')
genPhaseSpaceCutZtoElecTau_Ztautau_7TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# Z --> e+ e- sample generated with Pythia
#
#  7 TeV
#  parent dataset = /Zee/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO
#  skimmed dataset = /Zee/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-elecTauSkim/USER
#  generated events = 2421575
#  skimmed events = 1662918
#
intLumiZtoElecTau_Zee_7TeV = float(1862.75)
# 6/84 files missing
corrFactorZtoElecTau_Zee_7TeV = float(1.077) 

patTupleOutputFileNameZtoElecTau_Zee_7TeV = cms.untracked.string('patTupleZtoElecTau_Zee_7TeV_partXX.root')

plotsOutputFileNameZtoElecTau_Zee_7TeV = cms.string('plotsZtoElecTau_Zee_7TeV_partXX.root')

genPhaseSpaceCutZtoElecTau_Zee_7TeV = cms.PSet(
  pluginName = cms.string('genPhaseSpaceCut'),
  pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# EM enriched QCD sample generated with Pythia (20 GeV < Pt(hat) < 30 GeV)
#
#  7 TeV
#  parent dataset = /QCD_EMEnriched_Pt20to30/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO
#  skimmed dataset = /QCD_EMEnriched_Pt20to30/akalinow-SkimTauTau_356_pass1-af7269240adb80f379a817aa7b511727-elecTauSkim/USER
#  generated events = 33889839
#  events passing skim = 9188659

intLumiZtoElecTau_QCD_EMenriched_Pt20to30_7TeV = float(19.71)
#missing 6/184
corrFactorZtoElecTau_QCD_EMenriched_Pt20to30_7TeV = float(1.034) 

patTupleOutputFileNameZtoElecTau_QCD_EMenriched_Pt20to30_7TeV = cms.untracked.string('patTupleZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_partXX.root')

plotsOutputFileNameZtoElecTau_QCD_EMenriched_Pt20to30_7TeV = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_partXX.root')

genPhaseSpaceCutZtoElecTau_QCD_EMenriched_Pt20to30_7TeV = cms.PSet(
  pluginName = cms.string('genPhaseSpaceCut'),
  pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# EM enriched QCD sample generated with Pythia (30 GeV < Pt(hat) < 80 GeV)
#
#  7 TeV
#  parent dataset = /QCD_EMEnriched_Pt30to80/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO
#  skimmed dataset = /QCD_EMEnriched_Pt30to80/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-elecTauSkim/USER
#  generated events = 39137278
#  events passing skim = 13498528
intLumiZtoElecTau_QCD_EMenriched_Pt30to80_7TeV = float(11.19)
#
corrFactorZtoElecTau_QCD_EMenriched_Pt30to80_7TeV = float(1.)

patTupleOutputFileNameZtoElecTau_QCD_EMenriched_Pt30to80_7TeV = cms.untracked.string('patTupleZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_partXX.root')

plotsOutputFileNameZtoElecTau_QCD_EMenriched_Pt30to80_7TeV = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_partXX.root')

genPhaseSpaceCutZtoElecTau_QCD_EMenriched_Pt30to80_7TeV = cms.PSet(
  pluginName = cms.string('genPhaseSpaceCut'),
  pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# EM enriched QCD sample generated with Pythia (80 GeV < Pt(hat) < 170 GeV)
#
#  7TeV
#  parent dataset = /QCD_EMEnriched_Pt80to170/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO
#  skimmed dataset = /QCD_EMEnriched_Pt80to170/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-elecTauSkim/USER
#  generated events = 5414911 
#  events passing skim = 2470823
intLumiZtoElecTau_QCD_EMenriched_Pt80to170_7TeV = float(40.38)
# 29/248 missing
corrFactorZtoElecTau_QCD_EMenriched_Pt80to170_7TeV = float(1.132)

patTupleOutputFileNameZtoElecTau_QCD_EMenriched_Pt80to170_7TeV = cms.untracked.string('patTupleZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_partXX.root')

plotsOutputFileNameZtoElecTau_QCD_EMenriched_Pt80to170_7TeV = cms.string('plotsZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_partXX.root')

genPhaseSpaceCutZtoElecTau_QCD_EMenriched_Pt80to170_7TeV = cms.PSet(
  pluginName = cms.string('genPhaseSpaceCut'),
  pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# QCD b/c --> e sample generated with Pythia (20 GeV < Pt(hat) < 30 GeV)
#
#  7 TeV
#  parent dataset = /QCD_BCtoE_Pt20to30/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO
#  skimmed dataset = /QCD_BCtoE_Pt20to30/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-elecTauSkim/USER
#  generated events = 2659560
#  events passing skim = 1976310
# 
intLumiZtoElecTau_QCD_BCtoE_Pt20to30_7TeV = float(24.55)
corrFactorZtoElecTau_QCD_BCtoE_Pt20to30_7TeV = float(1.)

patTupleOutputFileNameZtoElecTau_QCD_BCtoE_Pt20to30_7TeV = cms.untracked.string('patTupleZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_partXX.root')

plotsOutputFileNameZtoElecTau_QCD_BCtoE_Pt20to30_7TeV = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_partXX.root')

genPhaseSpaceCutZtoElecTau_QCD_BCtoE_Pt20to30_7TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)


#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# QCD b/c -->e sample generated with Pythia (30 GeV < Pt(hat) < 80 GeV)
#
#  7 TeV
#  parent dataset = /QCD_BCtoE_Pt30to80/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO
#  skimmed dataset = /QCD_BCtoE_Pt30to80/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324
#  generated events = 2075597
#  events passing skim = 1686251
intLumiZtoElecTau_QCD_BCtoE_Pt30to80_7TeV = float(14.96)
# 2/167 missing
corrFactorZtoElecTau_QCD_BCtoE_Pt30to80_7TeV = float(1.)

patTupleOutputFileNameZtoElecTau_QCD_BCtoE_Pt30to80_7TeV = cms.untracked.string('patTupleZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_partXX.root')

plotsOutputFileNameZtoElecTau_QCD_BCtoE_Pt30to80_7TeV = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_partXX.root')

genPhaseSpaceCutZtoElecTau_QCD_BCtoE_Pt30to80_7TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)



#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
#  QCD b/c -->e sample generated with Pythia (80 GeV < Pt(hat) < 170 GeV)
#
#  7 TeV
#  parent dataset = /QCD_BCtoE_Pt80to170/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO
#  skimmed dataset = /QCD_BCtoE_Pt80to170/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-elecTauSkim/USER
#  generated events = 988674
#  events passing skim = 839063
intLumiZtoElecTau_QCD_BCtoE_Pt80to170_7TeV = float(104.9)
# 
corrFactorZtoElecTau_QCD_BCtoE_Pt80to170_7TeV = float(1.)

patTupleOutputFileNameZtoElecTau_QCD_BCtoE_Pt80to170_7TeV = cms.untracked.string('patTupleZtoElecTau_QCD_BCtoE_Pt80to170_7TeV_partXX.root')

plotsOutputFileNameZtoElecTau_QCD_BCtoE_Pt80to170_7TeV = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt80to170_7TeV_partXX.root')

genPhaseSpaceCutZtoElecTau_QCD_BCtoE_Pt80to170_7TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)


#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# photon + jets samples (PYTHIA)
#
# dataset = /PhotonJet_Pt15to20/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-elecTauSkim/USER 
# generated events = 108560
# events passing skim = 5913

# dataset = /PhotonJet_Pt20to30/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-elecTauSkim/USER
# generated events = 110000
# events passing skim = 10788

# dataset = /PhotonJet_Pt30to50/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-elecTauSkim/USER
# generated events = 110000
# events passing skim = 19453

# dataset = /PhotonJet_Pt50to80/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-elecTauSkim/USER
# generated events = 109730
# events passing skim = 29023

# dataset = /PhotonJet_Pt80to120/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-elecTauSkim/USER
# generated events = 110827
# events passing skim = 36841


corrFactorZtoElecTau_PhotonPlusJets_Pt15to20_7TeV = float(1.0)
corrFactorZtoElecTau_PhotonPlusJets_Pt20to30_7TeV = float(1.0)
corrFactorZtoElecTau_PhotonPlusJets_Pt30to50_7TeV = float(1.0)
corrFactorZtoElecTau_PhotonPlusJets_Pt50to80_7TeV = float(1.0)
corrFactorZtoElecTau_PhotonPlusJets_Pt80to120_7TeV = float(1.0)

intLumiZtoElecTau_PhotonPlusJets_Pt15to20_7TeV = float(0.9465)
intLumiZtoElecTau_PhotonPlusJets_Pt20to30_7TeV = float(1.924)
intLumiZtoElecTau_PhotonPlusJets_Pt30to50_7TeV = float(6.659)
intLumiZtoElecTau_PhotonPlusJets_Pt50to80_7TeV = float(40.30)
intLumiZtoElecTau_PhotonPlusJets_Pt80to120_7TeV = float(248.4)

genPhaseSpaceCutZtoElecTau_PhotonPlusJets_Pt15to20_7TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)
genPhaseSpaceCutZtoElecTau_PhotonPlusJets_Pt20to30_7TeV = copy.deepcopy(genPhaseSpaceCutZtoElecTau_PhotonPlusJets_Pt15to20_7TeV)
genPhaseSpaceCutZtoElecTau_PhotonPlusJets_Pt30to50_7TeV = copy.deepcopy(genPhaseSpaceCutZtoElecTau_PhotonPlusJets_Pt15to20_7TeV)
genPhaseSpaceCutZtoElecTau_PhotonPlusJets_Pt50to80_7TeV = copy.deepcopy(genPhaseSpaceCutZtoElecTau_PhotonPlusJets_Pt15to20_7TeV)
genPhaseSpaceCutZtoElecTau_PhotonPlusJets_Pt80to120_7TeV = copy.deepcopy(genPhaseSpaceCutZtoElecTau_PhotonPlusJets_Pt15to20_7TeV)

plotsOutputFileNameZtoElecTau_PhotonPlusJets_Pt15to20_7TeV = cms.string('plotsZtoElecTau_PhotonPlusJets_Pt15to20_7TeV.root')
plotsOutputFileNameZtoElecTau_PhotonPlusJets_Pt20to30_7TeV = cms.string('plotsZtoElecTau_PhotonPlusJets_Pt20to30_7TeV.root')
plotsOutputFileNameZtoElecTau_PhotonPlusJets_Pt30to50_7TeV = cms.string('plotsZtoElecTau_PhotonPlusJets_Pt30to50_7TeV.root')
plotsOutputFileNameZtoElecTau_PhotonPlusJets_Pt50to80_7TeV = cms.string('plotsZtoElecTau_PhotonPlusJets_Pt50to80_7TeV.root')
plotsOutputFileNameZtoElecTau_PhotonPlusJets_Pt80to120_7TeV = cms.string('plotsZtoElecTau_PhotonPlusJets_Pt80to120_7TeV.root')

patTupleOutputFileNameZtoElecTau_PhotonPlusJets_Pt15to20_7TeV = cms.untracked.string('patTupleZtoElecTau_PhotonPlusJets_Pt15to20_7TeV_part01.root')
patTupleOutputFileNameZtoElecTau_PhotonPlusJets_Pt20to30_7TeV = cms.untracked.string('patTupleZtoElecTau_PhotonPlusJets_Pt20to30_7TeV_part01.root')
patTupleOutputFileNameZtoElecTau_PhotonPlusJets_Pt30to50_7TeV = cms.untracked.string('patTupleZtoElecTau_PhotonPlusJets_Pt30to50_7TeV_part01.root')
patTupleOutputFileNameZtoElecTau_PhotonPlusJets_Pt50to80_7TeV = cms.untracked.string('patTupleZtoElecTau_PhotonPlusJets_Pt50to80_7TeV_part01.root')
patTupleOutputFileNameZtoElecTau_PhotonPlusJets_Pt80to120_7TeV = cms.untracked.string('patTupleZtoElecTau_PhotonPlusJets_Pt80to120_7TeV_part01.root')
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
#  W + jets sample generated with Madgraph
# 
#  7 TeV
#  parent dataset = /WJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO
#  skimmed dataset = /WJets-madgraph/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-elecTauSkim/USER
#  generated events = 9008895
#  events passing skim = 2449042
#
intLumiZtoElecTau_WplusJets_7TeV = float(372.7)
# missing 1/82
corrFactorZtoElecTau_WplusJets_7TeV = float(1.012)

patTupleOutputFileNameZtoElecTau_WplusJets_7TeV = cms.untracked.string('patTupleZtoElecTau_WplusJets_7TeV_partXX.root')

plotsOutputFileNameZtoElecTau_WplusJets_7TeV = cms.string('plotsZtoElecTau_WplusJets_7TeV_partXX.root')

genPhaseSpaceCutZtoElecTau_WplusJets_7TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)


#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
#  Z + jets sample generated with Madgraph (excluding Z --> tau+ tau- decays)
#
#  parent dataset = /ZJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO
#  skimmed dataset = /ZJets-madgraph/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-elecTauSkim/USER
#  generated events = 1064921
#  events passing skim = 415500
#
intLumiZtoElecTau_ZJets_7TeV = float(443.7)
corrFactorZtoElecTau_ZJets_7TeV = float(1.0)

corrFactorZtoElecTau_ZeePlusJets_7TeV = float(1.)
intLumiZtoElecTau_ZeePlusJets_7TeV = intLumiZtoElecTau_ZJets_7TeV

corrFactorZtoElecTau_ZtautauPlusJets_7TeV = float(1.)
intLumiZtoElecTau_ZtautauPlusJets_7TeV = intLumiZtoElecTau_ZJets_7TeV

patTupleOutputFileNameZtoElecTau_ZeePlusJets_7TeV = cms.untracked.string('patTupleZtoElecTau_ZeePlusJets_7TeV_partXX.root')

plotsOutputFileNameZtoElecTau_ZeePlusJets_7TeV = cms.string('plotsZtoElecTau_ZeePlusJets_7TeV_partXX.root')

genPhaseSpaceCutZtoElecTau_ZeePlusJets_7TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('genElectronsFromZs'),
    minNumber = cms.uint32(2)
)

patTupleOutputFileNameZtoElecTau_ZtautauPlusJets_7TeV = cms.untracked.string('patTupleZtoElecTau_ZtautauPlusJets_7TeV_partXX.root')

plotsOutputFileNameZtoElecTau_ZtautauPlusJets_7TeV = cms.string('plotsZtoElecTau_ZtautauPlusJets_7TeV_partXX.root')

genPhaseSpaceCutZtoElecTau_ZtautauPlusJets_7TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('genTausFromZs'),
    minNumber = cms.uint32(2)
)
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# TTbar + Jets madgraph sample
#  
#  parent dataset = /TTbarJets_Tauola-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO
#  skimmed dataset = /TTbarJets_Tauola-madgraph/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-elecTauSkim/USER
#  generated events = 1412199
#  events passing skim = 1019856
#
#
intLumiZtoElecTau_TTplusJets_7TeV = float(14865)
#
corrFactorZtoElecTau_TTplusJets_7TeV = float(1.)

genPhaseSpaceCutZtoElecTau_TTplusJets_7TeV = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

patTupleOutputFileNameZtoElecTau_TTplusJets_7TeV = cms.untracked.string('patTupleZtoElecTau_TTplusJets_7TeV_partXX.root')

plotsOutputFileNameZtoElecTau_TTplusJets_7TeV = cms.string('plotsZtoElecTau_TTplusJets_7TeV_partXX.root')


#--------------------------------------------------------------------------------

