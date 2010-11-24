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


intLumiZtoElecTau_Data = float(32) # in 1/pb
#intLumiZtoElecTau_Data = float(100.) # in 1/pb

corrFactorZtoElecTau_Data_7TeV = float(1.)
corrFactorZtoElecTau_Data2010A = float(1.)
corrFactorZtoElecTau_Data2010B = float(1.)

patTupleOutputFileNameZtoElecTau_Data_7TeV = cms.untracked.string('patTupleZtoElecTau_Data_7TeV_partXX.root')
patTupleOutputFileNameZtoElecTau_Data2010A = cms.untracked.string('patTupleZtoElecTau_Data2010A_partXX.root')
patTupleOutputFileNameZtoElecTau_Data2010B = cms.untracked.string('patTupleZtoElecTau_Data2010B_partXX.root')

plotsOutputFileNameZtoElecTau_Data_7TeV = cms.string('plotsZtoElecTau_Data_7TeV_partXX.root')
plotsOutputFileNameZtoElecTau_Data2010A = cms.string('plotsZtoElecTau_Data2010A_partXX.root')
plotsOutputFileNameZtoElecTau_Data2010B = cms.string('plotsZtoElecTau_Data2010B_partXX.root')

genPhaseSpaceCutZtoElecTau_Data2010A = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)
genPhaseSpaceCutZtoElecTau_Data_7TeV = copy.deepcopy(genPhaseSpaceCutZtoElecTau_Data2010A)
genPhaseSpaceCutZtoElecTau_Data2010B = copy.deepcopy(genPhaseSpaceCutZtoElecTau_Data2010A)

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

#
#  EG PD - 2010A = Sept17 rereo
#

fileNamesZtoElecTau_Data2010A_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_1_1_Yi8.root'
)
fileNamesZtoElecTau_Data2010A_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_10_1_zup.root'
)
fileNamesZtoElecTau_Data2010A_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_100_1_BIV.root'
)
fileNamesZtoElecTau_Data2010A_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_101_1_2u2.root'
)
fileNamesZtoElecTau_Data2010A_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_102_1_I9Y.root'
)
fileNamesZtoElecTau_Data2010A_part06 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_103_1_djo.root'
)
fileNamesZtoElecTau_Data2010A_part07 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_104_1_EgN.root'
)
fileNamesZtoElecTau_Data2010A_part08 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_105_1_8no.root'
)
fileNamesZtoElecTau_Data2010A_part09 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_106_1_Xde.root'
)
fileNamesZtoElecTau_Data2010A_part10 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_107_1_wVi.root'
)
fileNamesZtoElecTau_Data2010A_part11 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_108_1_KIP.root'
)
fileNamesZtoElecTau_Data2010A_part12 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_109_1_jP5.root'
)
fileNamesZtoElecTau_Data2010A_part13 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_11_1_A0g.root'
)
fileNamesZtoElecTau_Data2010A_part14 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_110_1_oO2.root'
)
fileNamesZtoElecTau_Data2010A_part15 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_111_1_yum.root'
)
fileNamesZtoElecTau_Data2010A_part16 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_112_1_TFU.root'
)
fileNamesZtoElecTau_Data2010A_part17 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_113_1_Lqj.root'
)
fileNamesZtoElecTau_Data2010A_part18 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_114_1_4TU.root'
)
fileNamesZtoElecTau_Data2010A_part19 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_115_1_HNd.root'
)
fileNamesZtoElecTau_Data2010A_part20 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_116_1_Du5.root'
)
fileNamesZtoElecTau_Data2010A_part21 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_117_1_Jkc.root'
)
fileNamesZtoElecTau_Data2010A_part22 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_118_1_XAS.root'
)
fileNamesZtoElecTau_Data2010A_part23 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_119_1_k4n.root'
)
fileNamesZtoElecTau_Data2010A_part24 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_12_1_F8N.root'
)
fileNamesZtoElecTau_Data2010A_part25 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_120_1_IKB.root'
)
fileNamesZtoElecTau_Data2010A_part26 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_121_1_o5Y.root'
)
fileNamesZtoElecTau_Data2010A_part27 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_122_1_ikV.root'
)
fileNamesZtoElecTau_Data2010A_part28 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_123_1_SWp.root'
)
fileNamesZtoElecTau_Data2010A_part29 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_124_1_gf0.root'
)
fileNamesZtoElecTau_Data2010A_part30 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_125_1_WF7.root'
)
fileNamesZtoElecTau_Data2010A_part31 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_126_1_dWu.root'
)
fileNamesZtoElecTau_Data2010A_part32 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_127_1_blZ.root'
)
fileNamesZtoElecTau_Data2010A_part33 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_128_1_hQb.root'
)
fileNamesZtoElecTau_Data2010A_part34 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_129_1_Pni.root'
)
fileNamesZtoElecTau_Data2010A_part35 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_13_1_UNb.root'
)
fileNamesZtoElecTau_Data2010A_part36 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_130_1_Lhs.root'
)
fileNamesZtoElecTau_Data2010A_part37 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_131_1_Sbx.root'
)
fileNamesZtoElecTau_Data2010A_part38 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_132_1_9FO.root'
)
fileNamesZtoElecTau_Data2010A_part39 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_133_1_OBF.root'
)
fileNamesZtoElecTau_Data2010A_part40 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_134_1_i8U.root'
)
fileNamesZtoElecTau_Data2010A_part41 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_135_1_prN.root'
)
fileNamesZtoElecTau_Data2010A_part42 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_136_1_EKN.root'
)
fileNamesZtoElecTau_Data2010A_part43 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_137_1_kMM.root'
)
fileNamesZtoElecTau_Data2010A_part44 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_138_1_C7d.root'
)
fileNamesZtoElecTau_Data2010A_part45 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_139_1_z0C.root'
)
fileNamesZtoElecTau_Data2010A_part46 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_14_1_d9H.root'
)
fileNamesZtoElecTau_Data2010A_part47 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_140_1_iEs.root'
)
fileNamesZtoElecTau_Data2010A_part48 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_141_1_KB2.root'
)
fileNamesZtoElecTau_Data2010A_part49 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_142_1_S5Z.root'
)
fileNamesZtoElecTau_Data2010A_part50 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_143_1_QNO.root'
)
fileNamesZtoElecTau_Data2010A_part51 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_144_1_Kd8.root'
)
fileNamesZtoElecTau_Data2010A_part52 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_145_1_3L3.root'
)
fileNamesZtoElecTau_Data2010A_part53 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_146_1_FnY.root'
)
fileNamesZtoElecTau_Data2010A_part54 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_147_2_eOh.root'
)
fileNamesZtoElecTau_Data2010A_part55 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_148_1_tKA.root'
)
fileNamesZtoElecTau_Data2010A_part56 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_149_1_rva.root'
)
fileNamesZtoElecTau_Data2010A_part57 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_15_1_OGD.root'
)
fileNamesZtoElecTau_Data2010A_part58 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_150_1_M8S.root'
)
fileNamesZtoElecTau_Data2010A_part59 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_151_1_mBc.root'
)
fileNamesZtoElecTau_Data2010A_part60 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_152_1_7hz.root'
)
fileNamesZtoElecTau_Data2010A_part61 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_153_1_xv3.root'
)
fileNamesZtoElecTau_Data2010A_part62 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_154_1_Tsi.root'
)
fileNamesZtoElecTau_Data2010A_part63 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_155_1_spe.root'
)
fileNamesZtoElecTau_Data2010A_part64 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_156_1_WKW.root'
)
fileNamesZtoElecTau_Data2010A_part65 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_157_1_9Tb.root'
)
fileNamesZtoElecTau_Data2010A_part66 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_158_1_NaI.root'
)
fileNamesZtoElecTau_Data2010A_part67 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_159_1_QXK.root'
)
fileNamesZtoElecTau_Data2010A_part68 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_16_1_4Eh.root'
)
fileNamesZtoElecTau_Data2010A_part69 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_160_1_hvM.root'
)
fileNamesZtoElecTau_Data2010A_part70 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_161_1_tkd.root'
)
fileNamesZtoElecTau_Data2010A_part71 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_162_1_S5F.root'
)
fileNamesZtoElecTau_Data2010A_part72 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_163_1_nh6.root'
)
fileNamesZtoElecTau_Data2010A_part73 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_164_1_cxv.root'
)
fileNamesZtoElecTau_Data2010A_part74 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_165_1_sZg.root'
)
fileNamesZtoElecTau_Data2010A_part75 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_166_1_Tbu.root'
)
fileNamesZtoElecTau_Data2010A_part76 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_167_1_VYX.root'
)
fileNamesZtoElecTau_Data2010A_part77 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_168_1_tW0.root'
)
fileNamesZtoElecTau_Data2010A_part78 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_169_1_3C2.root'
)
fileNamesZtoElecTau_Data2010A_part79 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_17_1_eF3.root'
)
fileNamesZtoElecTau_Data2010A_part80 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_170_1_rh6.root'
)
fileNamesZtoElecTau_Data2010A_part81 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_171_1_eYE.root'
)
fileNamesZtoElecTau_Data2010A_part82 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_172_1_4Cy.root'
)
fileNamesZtoElecTau_Data2010A_part83 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_173_1_4Ip.root'
)
fileNamesZtoElecTau_Data2010A_part84 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_174_1_FaC.root'
)
fileNamesZtoElecTau_Data2010A_part85 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_175_1_fw2.root'
)
fileNamesZtoElecTau_Data2010A_part86 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_176_1_WVa.root'
)
fileNamesZtoElecTau_Data2010A_part87 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_177_1_mEv.root'
)
fileNamesZtoElecTau_Data2010A_part88 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_178_1_ReW.root'
)
fileNamesZtoElecTau_Data2010A_part89 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_179_1_J49.root'
)
fileNamesZtoElecTau_Data2010A_part90 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_18_1_5Aj.root'
)
fileNamesZtoElecTau_Data2010A_part91 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_180_1_dk4.root'
)
fileNamesZtoElecTau_Data2010A_part92 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_181_1_BE2.root'
)
fileNamesZtoElecTau_Data2010A_part93 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_182_1_0ND.root'
)
fileNamesZtoElecTau_Data2010A_part94 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_183_1_Bd3.root'
)
fileNamesZtoElecTau_Data2010A_part95 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_184_1_2OI.root'
)
fileNamesZtoElecTau_Data2010A_part96 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_185_1_KfL.root'
)
fileNamesZtoElecTau_Data2010A_part97 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_186_1_UOf.root'
)
fileNamesZtoElecTau_Data2010A_part98 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_187_1_n7l.root'
)
fileNamesZtoElecTau_Data2010A_part99 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_188_1_cKd.root'
)
fileNamesZtoElecTau_Data2010A_part100 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_189_1_Tws.root'
)
fileNamesZtoElecTau_Data2010A_part101 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_19_1_mmF.root'
)
fileNamesZtoElecTau_Data2010A_part102 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_190_1_MGP.root'
)
fileNamesZtoElecTau_Data2010A_part103 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_191_1_rT6.root'
)
fileNamesZtoElecTau_Data2010A_part104 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_192_1_a3a.root'
)
fileNamesZtoElecTau_Data2010A_part105 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_193_1_gZy.root'
)
fileNamesZtoElecTau_Data2010A_part106 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_194_2_bv4.root'
)
fileNamesZtoElecTau_Data2010A_part107 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_195_1_fhL.root'
)
fileNamesZtoElecTau_Data2010A_part108 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_196_1_oqJ.root'
)
fileNamesZtoElecTau_Data2010A_part109 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_197_1_Ea8.root'
)
fileNamesZtoElecTau_Data2010A_part110 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_198_1_IxH.root'
)
fileNamesZtoElecTau_Data2010A_part111 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_199_1_D2O.root'
)
fileNamesZtoElecTau_Data2010A_part112 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_2_1_CLW.root'
)
fileNamesZtoElecTau_Data2010A_part113 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_20_1_eAC.root'
)
fileNamesZtoElecTau_Data2010A_part114 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_200_1_ZOB.root'
)
fileNamesZtoElecTau_Data2010A_part115 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_201_1_KOI.root'
)
fileNamesZtoElecTau_Data2010A_part116 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_202_1_c5d.root'
)
fileNamesZtoElecTau_Data2010A_part117 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_203_1_tRa.root'
)
fileNamesZtoElecTau_Data2010A_part118 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_204_1_Nv6.root'
)
fileNamesZtoElecTau_Data2010A_part119 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_205_2_biZ.root'
)
fileNamesZtoElecTau_Data2010A_part120 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_206_1_ADi.root'
)
fileNamesZtoElecTau_Data2010A_part121 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_207_1_AHn.root'
)
fileNamesZtoElecTau_Data2010A_part122 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_208_1_ulK.root'
)
fileNamesZtoElecTau_Data2010A_part123 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_209_1_VOV.root'
)
fileNamesZtoElecTau_Data2010A_part124 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_21_1_VAx.root'
)
fileNamesZtoElecTau_Data2010A_part125 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_210_1_N6S.root'
)
fileNamesZtoElecTau_Data2010A_part126 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_211_1_Mlq.root'
)
fileNamesZtoElecTau_Data2010A_part127 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_212_1_vK4.root'
)
fileNamesZtoElecTau_Data2010A_part128 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_213_1_J2D.root'
)
fileNamesZtoElecTau_Data2010A_part129 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_214_1_GiA.root'
)
fileNamesZtoElecTau_Data2010A_part130 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_215_1_5cF.root'
)
fileNamesZtoElecTau_Data2010A_part131 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_216_1_yl6.root'
)
fileNamesZtoElecTau_Data2010A_part132 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_217_1_VlL.root'
)
fileNamesZtoElecTau_Data2010A_part133 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_218_1_QUJ.root'
)
fileNamesZtoElecTau_Data2010A_part134 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_219_1_Jrx.root'
)
fileNamesZtoElecTau_Data2010A_part135 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_22_1_5B4.root'
)
fileNamesZtoElecTau_Data2010A_part136 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_220_1_7E2.root'
)
fileNamesZtoElecTau_Data2010A_part137 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_221_1_Ohm.root'
)
fileNamesZtoElecTau_Data2010A_part138 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_222_1_e1u.root'
)
fileNamesZtoElecTau_Data2010A_part139 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_223_1_oMY.root'
)
fileNamesZtoElecTau_Data2010A_part140 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_224_1_iYt.root'
)
fileNamesZtoElecTau_Data2010A_part141 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_225_1_STv.root'
)
fileNamesZtoElecTau_Data2010A_part142 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_226_1_bO3.root'
)
fileNamesZtoElecTau_Data2010A_part143 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_227_1_QB5.root'
)
fileNamesZtoElecTau_Data2010A_part144 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_228_1_U5v.root'
)
fileNamesZtoElecTau_Data2010A_part145 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_229_1_P8U.root'
)
fileNamesZtoElecTau_Data2010A_part146 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_23_1_ZD2.root'
)
fileNamesZtoElecTau_Data2010A_part147 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_230_1_WEf.root'
)
fileNamesZtoElecTau_Data2010A_part148 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_231_1_ZJX.root'
)
fileNamesZtoElecTau_Data2010A_part149 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_232_1_Knj.root'
)
fileNamesZtoElecTau_Data2010A_part150 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_233_1_mvu.root'
)
fileNamesZtoElecTau_Data2010A_part151 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_234_1_2OH.root'
)
fileNamesZtoElecTau_Data2010A_part152 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_235_1_tEN.root'
)
fileNamesZtoElecTau_Data2010A_part153 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_236_1_W5P.root'
)
fileNamesZtoElecTau_Data2010A_part154 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_237_1_6Yp.root'
)
fileNamesZtoElecTau_Data2010A_part155 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_238_1_JWj.root'
)
fileNamesZtoElecTau_Data2010A_part156 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_239_1_Nzp.root'
)
fileNamesZtoElecTau_Data2010A_part157 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_24_1_E3J.root'
)
fileNamesZtoElecTau_Data2010A_part158 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_240_1_9lb.root'
)
fileNamesZtoElecTau_Data2010A_part159 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_241_1_wIi.root'
)
fileNamesZtoElecTau_Data2010A_part160 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_242_1_kWb.root'
)
fileNamesZtoElecTau_Data2010A_part161 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_243_1_EY4.root'
)
fileNamesZtoElecTau_Data2010A_part162 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_244_1_Dui.root'
)
fileNamesZtoElecTau_Data2010A_part163 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_245_1_XYF.root'
)
fileNamesZtoElecTau_Data2010A_part164 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_246_1_eON.root'
)
fileNamesZtoElecTau_Data2010A_part165 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_247_2_5F6.root'
)
fileNamesZtoElecTau_Data2010A_part166 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_248_1_Qd8.root'
)
fileNamesZtoElecTau_Data2010A_part167 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_249_1_QWC.root'
)
fileNamesZtoElecTau_Data2010A_part168 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_25_1_gC9.root'
)
fileNamesZtoElecTau_Data2010A_part169 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_250_1_zRX.root'
)
fileNamesZtoElecTau_Data2010A_part170 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_251_1_nMa.root'
)
fileNamesZtoElecTau_Data2010A_part171 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_252_1_9ze.root'
)
fileNamesZtoElecTau_Data2010A_part172 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_253_1_UfK.root'
)
fileNamesZtoElecTau_Data2010A_part173 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_254_1_ZVn.root'
)
fileNamesZtoElecTau_Data2010A_part174 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_255_1_h3y.root'
)
fileNamesZtoElecTau_Data2010A_part175 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_256_1_cad.root'
)
fileNamesZtoElecTau_Data2010A_part176 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_257_1_ow9.root'
)
fileNamesZtoElecTau_Data2010A_part177 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_258_1_sHF.root'
)
fileNamesZtoElecTau_Data2010A_part178 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_259_1_Owb.root'
)
fileNamesZtoElecTau_Data2010A_part179 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_26_1_NqZ.root'
)
fileNamesZtoElecTau_Data2010A_part180 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_260_1_TVO.root'
)
fileNamesZtoElecTau_Data2010A_part181 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_261_1_Anf.root'
)
fileNamesZtoElecTau_Data2010A_part182 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_262_1_wty.root'
)
fileNamesZtoElecTau_Data2010A_part183 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_263_1_hRG.root'
)
fileNamesZtoElecTau_Data2010A_part184 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_264_1_DvL.root'
)
fileNamesZtoElecTau_Data2010A_part185 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_265_1_riX.root'
)
fileNamesZtoElecTau_Data2010A_part186 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_266_1_EvL.root'
)
fileNamesZtoElecTau_Data2010A_part187 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_267_1_TG0.root'
)
fileNamesZtoElecTau_Data2010A_part188 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_268_1_uHg.root'
)
fileNamesZtoElecTau_Data2010A_part189 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_269_1_B75.root'
)
fileNamesZtoElecTau_Data2010A_part190 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_27_1_6dh.root'
)
fileNamesZtoElecTau_Data2010A_part191 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_270_1_K3L.root'
)
fileNamesZtoElecTau_Data2010A_part192 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_271_1_l3r.root'
)
fileNamesZtoElecTau_Data2010A_part193 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_272_1_KMZ.root'
)
fileNamesZtoElecTau_Data2010A_part194 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_273_1_bz5.root'
)
fileNamesZtoElecTau_Data2010A_part195 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_274_1_PeA.root'
)
fileNamesZtoElecTau_Data2010A_part196 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_275_1_TLp.root'
)
fileNamesZtoElecTau_Data2010A_part197 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_276_1_Vkk.root'
)
fileNamesZtoElecTau_Data2010A_part198 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_277_1_9iZ.root'
)
fileNamesZtoElecTau_Data2010A_part199 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_278_1_eKO.root'
)
fileNamesZtoElecTau_Data2010A_part200 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_279_1_Rfw.root'
)
fileNamesZtoElecTau_Data2010A_part201 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_28_1_32u.root'
)
fileNamesZtoElecTau_Data2010A_part202 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_280_1_Y89.root'
)
fileNamesZtoElecTau_Data2010A_part203 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_281_1_S5b.root'
)
fileNamesZtoElecTau_Data2010A_part204 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_282_1_yCK.root'
)
fileNamesZtoElecTau_Data2010A_part205 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_283_1_q88.root'
)
fileNamesZtoElecTau_Data2010A_part206 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_284_1_MB2.root'
)
fileNamesZtoElecTau_Data2010A_part207 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_285_1_Xor.root'
)
fileNamesZtoElecTau_Data2010A_part208 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_286_1_knR.root'
)
fileNamesZtoElecTau_Data2010A_part209 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_287_1_8sq.root'
)
fileNamesZtoElecTau_Data2010A_part210 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_288_1_wTE.root'
)
fileNamesZtoElecTau_Data2010A_part211 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_289_2_Vab.root'
)
fileNamesZtoElecTau_Data2010A_part212 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_29_1_Blx.root'
)
fileNamesZtoElecTau_Data2010A_part213 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_290_1_i9N.root'
)
fileNamesZtoElecTau_Data2010A_part214 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_291_1_wCG.root'
)
fileNamesZtoElecTau_Data2010A_part215 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_292_1_R3D.root'
)
fileNamesZtoElecTau_Data2010A_part216 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_293_1_9Sn.root'
)
fileNamesZtoElecTau_Data2010A_part217 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_294_1_u9d.root'
)
fileNamesZtoElecTau_Data2010A_part218 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_295_1_hCt.root'
)
fileNamesZtoElecTau_Data2010A_part219 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_296_1_RAJ.root'
)
fileNamesZtoElecTau_Data2010A_part220 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_297_1_Kyb.root'
)
fileNamesZtoElecTau_Data2010A_part221 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_298_1_Grk.root'
)
fileNamesZtoElecTau_Data2010A_part222 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_299_1_nL8.root'
)
fileNamesZtoElecTau_Data2010A_part223 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_3_1_eXJ.root'
)
fileNamesZtoElecTau_Data2010A_part224 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_30_1_qF1.root'
)
fileNamesZtoElecTau_Data2010A_part225 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_300_1_DH0.root'
)
fileNamesZtoElecTau_Data2010A_part226 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_301_1_WFk.root'
)
fileNamesZtoElecTau_Data2010A_part227 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_302_1_WUe.root'
)
fileNamesZtoElecTau_Data2010A_part228 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_303_1_Ts0.root'
)
fileNamesZtoElecTau_Data2010A_part229 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_304_1_2s5.root'
)
fileNamesZtoElecTau_Data2010A_part230 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_305_1_EMd.root'
)
fileNamesZtoElecTau_Data2010A_part231 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_306_1_KHV.root'
)
fileNamesZtoElecTau_Data2010A_part232 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_307_1_9xL.root'
)
fileNamesZtoElecTau_Data2010A_part233 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_308_1_Kor.root'
)
fileNamesZtoElecTau_Data2010A_part234 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_309_1_BLa.root'
)
fileNamesZtoElecTau_Data2010A_part235 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_31_1_Kxw.root'
)
fileNamesZtoElecTau_Data2010A_part236 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_310_1_iqJ.root'
)
fileNamesZtoElecTau_Data2010A_part237 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_311_1_Q1r.root'
)
fileNamesZtoElecTau_Data2010A_part238 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_312_1_PEC.root'
)
fileNamesZtoElecTau_Data2010A_part239 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_313_1_VQF.root'
)
fileNamesZtoElecTau_Data2010A_part240 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_314_1_raX.root'
)
fileNamesZtoElecTau_Data2010A_part241 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_315_1_05o.root'
)
fileNamesZtoElecTau_Data2010A_part242 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_316_1_wS6.root'
)
fileNamesZtoElecTau_Data2010A_part243 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_317_1_dQh.root'
)
fileNamesZtoElecTau_Data2010A_part244 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_318_1_C5W.root'
)
fileNamesZtoElecTau_Data2010A_part245 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_319_1_Fxu.root'
)
fileNamesZtoElecTau_Data2010A_part246 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_32_1_56w.root'
)
fileNamesZtoElecTau_Data2010A_part247 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_320_1_ocG.root'
)
fileNamesZtoElecTau_Data2010A_part248 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_321_1_zrJ.root'
)
fileNamesZtoElecTau_Data2010A_part249 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_322_2_bL5.root'
)
fileNamesZtoElecTau_Data2010A_part250 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_323_1_5O7.root'
)
fileNamesZtoElecTau_Data2010A_part251 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_324_1_Frx.root'
)
fileNamesZtoElecTau_Data2010A_part252 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_325_1_WZk.root'
)
fileNamesZtoElecTau_Data2010A_part253 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_326_1_O4U.root'
)
fileNamesZtoElecTau_Data2010A_part254 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_327_1_YBO.root'
)
fileNamesZtoElecTau_Data2010A_part255 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_328_1_rop.root'
)
fileNamesZtoElecTau_Data2010A_part256 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_329_1_pbL.root'
)
fileNamesZtoElecTau_Data2010A_part257 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_33_1_sos.root'
)
fileNamesZtoElecTau_Data2010A_part258 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_330_1_68d.root'
)
fileNamesZtoElecTau_Data2010A_part259 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_331_1_1Qh.root'
)
fileNamesZtoElecTau_Data2010A_part260 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_332_1_aOZ.root'
)
fileNamesZtoElecTau_Data2010A_part261 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_333_1_fea.root'
)
fileNamesZtoElecTau_Data2010A_part262 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_334_4_AAQ.root'
)
fileNamesZtoElecTau_Data2010A_part263 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_335_1_v6Q.root'
)
fileNamesZtoElecTau_Data2010A_part264 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_336_1_86I.root'
)
fileNamesZtoElecTau_Data2010A_part265 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_337_1_oOQ.root'
)
fileNamesZtoElecTau_Data2010A_part266 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_338_1_8Ps.root'
)
fileNamesZtoElecTau_Data2010A_part267 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_339_1_nqV.root'
)
fileNamesZtoElecTau_Data2010A_part268 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_34_1_2Rh.root'
)
fileNamesZtoElecTau_Data2010A_part269 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_340_1_Ggc.root'
)
fileNamesZtoElecTau_Data2010A_part270 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_341_1_Kn1.root'
)
fileNamesZtoElecTau_Data2010A_part271 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_342_1_GNU.root'
)
fileNamesZtoElecTau_Data2010A_part272 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_343_1_xfM.root'
)
fileNamesZtoElecTau_Data2010A_part273 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_344_1_VIb.root'
)
fileNamesZtoElecTau_Data2010A_part274 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_345_1_dRz.root'
)
fileNamesZtoElecTau_Data2010A_part275 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_346_1_koi.root'
)
fileNamesZtoElecTau_Data2010A_part276 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_347_1_ILa.root'
)
fileNamesZtoElecTau_Data2010A_part277 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_348_1_8g5.root'
)
fileNamesZtoElecTau_Data2010A_part278 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_349_1_rBy.root'
)
fileNamesZtoElecTau_Data2010A_part279 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_35_1_7ys.root'
)
fileNamesZtoElecTau_Data2010A_part280 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_350_1_8xe.root'
)
fileNamesZtoElecTau_Data2010A_part281 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_351_1_fte.root'
)
fileNamesZtoElecTau_Data2010A_part282 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_352_1_3WT.root'
)
fileNamesZtoElecTau_Data2010A_part283 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_353_1_Lnp.root'
)
fileNamesZtoElecTau_Data2010A_part284 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_354_1_jHb.root'
)
fileNamesZtoElecTau_Data2010A_part285 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_355_1_wVm.root'
)
fileNamesZtoElecTau_Data2010A_part286 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_356_1_C4d.root'
)
fileNamesZtoElecTau_Data2010A_part287 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_357_1_8w7.root'
)
fileNamesZtoElecTau_Data2010A_part288 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_358_1_j6I.root'
)
fileNamesZtoElecTau_Data2010A_part289 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_359_1_hnN.root'
)
fileNamesZtoElecTau_Data2010A_part290 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_36_1_9Mb.root'
)
fileNamesZtoElecTau_Data2010A_part291 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_360_1_sa1.root'
)
fileNamesZtoElecTau_Data2010A_part292 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_361_1_nA9.root'
)
fileNamesZtoElecTau_Data2010A_part293 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_362_1_lty.root'
)
fileNamesZtoElecTau_Data2010A_part294 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_363_1_cYg.root'
)
fileNamesZtoElecTau_Data2010A_part295 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_364_4_cIN.root'
)
fileNamesZtoElecTau_Data2010A_part296 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_365_1_0Ak.root'
)
fileNamesZtoElecTau_Data2010A_part297 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_366_1_dB0.root'
)
fileNamesZtoElecTau_Data2010A_part298 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_367_1_wtr.root'
)
fileNamesZtoElecTau_Data2010A_part299 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_368_1_MtD.root'
)
fileNamesZtoElecTau_Data2010A_part300 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_369_1_x24.root'
)
fileNamesZtoElecTau_Data2010A_part301 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_37_1_ri5.root'
)
fileNamesZtoElecTau_Data2010A_part302 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_370_1_NEJ.root'
)
fileNamesZtoElecTau_Data2010A_part303 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_371_1_Cei.root'
)
fileNamesZtoElecTau_Data2010A_part304 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_372_1_XGa.root'
)
fileNamesZtoElecTau_Data2010A_part305 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_373_1_VvP.root'
)
fileNamesZtoElecTau_Data2010A_part306 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_374_1_8CL.root'
)
fileNamesZtoElecTau_Data2010A_part307 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_375_1_Tgm.root'
)
fileNamesZtoElecTau_Data2010A_part308 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_376_1_e0c.root'
)
fileNamesZtoElecTau_Data2010A_part309 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_377_1_Sl4.root'
)
fileNamesZtoElecTau_Data2010A_part310 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_378_1_wf9.root'
)
fileNamesZtoElecTau_Data2010A_part311 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_379_1_dRm.root'
)
fileNamesZtoElecTau_Data2010A_part312 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_38_1_4pE.root'
)
fileNamesZtoElecTau_Data2010A_part313 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_380_1_s9p.root'
)
fileNamesZtoElecTau_Data2010A_part314 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_381_1_xp3.root'
)
fileNamesZtoElecTau_Data2010A_part315 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_382_1_vpW.root'
)
fileNamesZtoElecTau_Data2010A_part316 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_383_1_3RK.root'
)
fileNamesZtoElecTau_Data2010A_part317 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_384_1_Wrs.root'
)
fileNamesZtoElecTau_Data2010A_part318 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_385_1_Lzy.root'
)
fileNamesZtoElecTau_Data2010A_part319 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_386_1_vW2.root'
)
fileNamesZtoElecTau_Data2010A_part320 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_387_1_W9B.root'
)
fileNamesZtoElecTau_Data2010A_part321 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_388_1_eTT.root'
)
fileNamesZtoElecTau_Data2010A_part322 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_389_1_G3x.root'
)
fileNamesZtoElecTau_Data2010A_part323 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_39_1_Cnf.root'
)
fileNamesZtoElecTau_Data2010A_part324 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_390_1_9ls.root'
)
fileNamesZtoElecTau_Data2010A_part325 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_391_1_QSq.root'
)
fileNamesZtoElecTau_Data2010A_part326 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_392_1_I4L.root'
)
fileNamesZtoElecTau_Data2010A_part327 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_393_1_eaQ.root'
)
fileNamesZtoElecTau_Data2010A_part328 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_394_1_Kqc.root'
)
fileNamesZtoElecTau_Data2010A_part329 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_395_1_R9q.root'
)
fileNamesZtoElecTau_Data2010A_part330 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_396_1_olH.root'
)
fileNamesZtoElecTau_Data2010A_part331 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_397_1_y56.root'
)
fileNamesZtoElecTau_Data2010A_part332 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_398_1_BWJ.root'
)
fileNamesZtoElecTau_Data2010A_part333 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_399_1_czV.root'
)
fileNamesZtoElecTau_Data2010A_part334 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_4_1_fR7.root'
)
fileNamesZtoElecTau_Data2010A_part335 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_40_1_CyP.root'
)
fileNamesZtoElecTau_Data2010A_part336 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_400_1_ett.root'
)
fileNamesZtoElecTau_Data2010A_part337 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_401_1_pJ5.root'
)
fileNamesZtoElecTau_Data2010A_part338 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_402_2_gLm.root'
)
fileNamesZtoElecTau_Data2010A_part339 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_41_1_CKI.root'
)
fileNamesZtoElecTau_Data2010A_part340 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_42_1_4lI.root'
)
fileNamesZtoElecTau_Data2010A_part341 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_43_1_NcH.root'
)
fileNamesZtoElecTau_Data2010A_part342 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_44_1_LcJ.root'
)
fileNamesZtoElecTau_Data2010A_part343 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_45_1_LNd.root'
)
fileNamesZtoElecTau_Data2010A_part344 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_46_1_JoY.root'
)
fileNamesZtoElecTau_Data2010A_part345 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_47_1_riH.root'
)
fileNamesZtoElecTau_Data2010A_part346 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_48_1_uJf.root'
)
fileNamesZtoElecTau_Data2010A_part347 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_49_1_KeA.root'
)
fileNamesZtoElecTau_Data2010A_part348 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_5_1_QVb.root'
)
fileNamesZtoElecTau_Data2010A_part349 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_50_1_cSS.root'
)
fileNamesZtoElecTau_Data2010A_part350 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_51_1_fwD.root'
)
fileNamesZtoElecTau_Data2010A_part351 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_52_1_PkQ.root'
)
fileNamesZtoElecTau_Data2010A_part352 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_53_1_8fC.root'
)
fileNamesZtoElecTau_Data2010A_part353 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_54_1_oVj.root'
)
fileNamesZtoElecTau_Data2010A_part354 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_55_1_fqj.root'
)
fileNamesZtoElecTau_Data2010A_part355 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_56_1_t55.root'
)
fileNamesZtoElecTau_Data2010A_part356 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_57_1_6kJ.root'
)
fileNamesZtoElecTau_Data2010A_part357 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_58_2_RnJ.root'
)
fileNamesZtoElecTau_Data2010A_part358 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_59_1_dvg.root'
)
fileNamesZtoElecTau_Data2010A_part359 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_6_1_scP.root'
)
fileNamesZtoElecTau_Data2010A_part360 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_60_1_mKE.root'
)
fileNamesZtoElecTau_Data2010A_part361 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_61_1_JXm.root'
)
fileNamesZtoElecTau_Data2010A_part362 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_62_1_DgF.root'
)
fileNamesZtoElecTau_Data2010A_part363 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_63_1_UOF.root'
)
fileNamesZtoElecTau_Data2010A_part364 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_64_1_clF.root'
)
fileNamesZtoElecTau_Data2010A_part365 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_65_1_ZvC.root'
)
fileNamesZtoElecTau_Data2010A_part366 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_66_1_0qx.root'
)
fileNamesZtoElecTau_Data2010A_part367 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_67_1_D7n.root'
)
fileNamesZtoElecTau_Data2010A_part368 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_68_1_XEE.root'
)
fileNamesZtoElecTau_Data2010A_part369 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_69_1_vvv.root'
)
fileNamesZtoElecTau_Data2010A_part370 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_7_1_jew.root'
)
fileNamesZtoElecTau_Data2010A_part371 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_70_1_HEB.root'
)
fileNamesZtoElecTau_Data2010A_part372 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_71_1_NjQ.root'
)
fileNamesZtoElecTau_Data2010A_part373 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_72_1_7Fb.root'
)
fileNamesZtoElecTau_Data2010A_part374 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_73_1_Z8I.root'
)
fileNamesZtoElecTau_Data2010A_part375 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_74_2_u31.root'
)
fileNamesZtoElecTau_Data2010A_part376 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_75_1_nEE.root'
)
fileNamesZtoElecTau_Data2010A_part377 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_76_1_a4m.root'
)
fileNamesZtoElecTau_Data2010A_part378 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_77_1_OBv.root'
)
fileNamesZtoElecTau_Data2010A_part379 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_78_1_hX5.root'
)
fileNamesZtoElecTau_Data2010A_part380 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_79_1_p67.root'
)
fileNamesZtoElecTau_Data2010A_part381 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_8_1_dLE.root'
)
fileNamesZtoElecTau_Data2010A_part382 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_80_1_lzQ.root'
)
fileNamesZtoElecTau_Data2010A_part383 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_81_1_H7i.root'
)
fileNamesZtoElecTau_Data2010A_part384 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_82_1_Z2z.root'
)
fileNamesZtoElecTau_Data2010A_part385 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_83_1_vhi.root'
)
fileNamesZtoElecTau_Data2010A_part386 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_84_1_WXD.root'
)
fileNamesZtoElecTau_Data2010A_part387 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_85_1_2Kl.root'
)
fileNamesZtoElecTau_Data2010A_part388 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_86_2_daZ.root'
)
fileNamesZtoElecTau_Data2010A_part389 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_87_1_haz.root'
)
fileNamesZtoElecTau_Data2010A_part390 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_88_1_9j7.root'
)
fileNamesZtoElecTau_Data2010A_part391 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_89_1_CZT.root'
)
fileNamesZtoElecTau_Data2010A_part392 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_9_1_aQN.root'
)
fileNamesZtoElecTau_Data2010A_part393 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_90_1_jDm.root'
)
fileNamesZtoElecTau_Data2010A_part394 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_91_1_UuI.root'
)
fileNamesZtoElecTau_Data2010A_part395 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_92_1_0wW.root'
)
fileNamesZtoElecTau_Data2010A_part396 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_93_1_9aZ.root'
)
fileNamesZtoElecTau_Data2010A_part397 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_94_1_2Ii.root'
)
fileNamesZtoElecTau_Data2010A_part398 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_95_1_IEx.root'
)
fileNamesZtoElecTau_Data2010A_part399 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_96_1_Tdm.root'
)
fileNamesZtoElecTau_Data2010A_part400 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_97_1_b1k.root'
)
fileNamesZtoElecTau_Data2010A_part401 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_98_1_I8Y.root'
)
fileNamesZtoElecTau_Data2010A_part402 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EG_2010A_Sep17ReReco/skimElecTau_99_1_IYu.root'
)

#
#  Electron PD - part1
#  

fileNamesZtoElecTau_Data2010B_part418 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_100_1_jMn.root'
)
fileNamesZtoElecTau_Data2010B_part419 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_101_1_h9A.root'
)
fileNamesZtoElecTau_Data2010B_part420 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_102_1_4OF.root'
)
fileNamesZtoElecTau_Data2010B_part421 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_103_1_vOj.root'
)
fileNamesZtoElecTau_Data2010B_part422 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_104_1_YQp.root'
)
fileNamesZtoElecTau_Data2010B_part423 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_105_1_6FI.root'
)
fileNamesZtoElecTau_Data2010B_part424 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_106_1_F6I.root'
)
fileNamesZtoElecTau_Data2010B_part425 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_107_1_o78.root'
)
fileNamesZtoElecTau_Data2010B_part426 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_108_1_vWK.root'
)
fileNamesZtoElecTau_Data2010B_part427 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_109_1_9Cz.root'
)
fileNamesZtoElecTau_Data2010B_part428 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_10_1_vGR.root'
)
fileNamesZtoElecTau_Data2010B_part429 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_110_1_0pG.root'
)
fileNamesZtoElecTau_Data2010B_part430 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_111_1_c3a.root'
)
fileNamesZtoElecTau_Data2010B_part431 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_112_1_tvj.root'
)
fileNamesZtoElecTau_Data2010B_part432 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_113_1_Zba.root'
)
fileNamesZtoElecTau_Data2010B_part433 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_114_1_FZa.root'
)
fileNamesZtoElecTau_Data2010B_part434 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_115_1_zsS.root'
)
fileNamesZtoElecTau_Data2010B_part435 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_116_1_5eW.root'
)
fileNamesZtoElecTau_Data2010B_part436 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_117_1_HiB.root'
)
fileNamesZtoElecTau_Data2010B_part437 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_118_1_u1p.root'
)
fileNamesZtoElecTau_Data2010B_part438 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_119_1_6Wu.root'
)
fileNamesZtoElecTau_Data2010B_part439 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_11_1_fr3.root'
)
fileNamesZtoElecTau_Data2010B_part440 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_120_1_xMe.root'
)
fileNamesZtoElecTau_Data2010B_part441 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_121_1_YpV.root'
)
fileNamesZtoElecTau_Data2010B_part442 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_122_1_K0A.root'
)
fileNamesZtoElecTau_Data2010B_part443 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_123_1_AN3.root'
)
fileNamesZtoElecTau_Data2010B_part444 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_12_1_gnJ.root'
)
fileNamesZtoElecTau_Data2010B_part445 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_13_1_y6z.root'
)
fileNamesZtoElecTau_Data2010B_part446 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_14_1_EEI.root'
)
fileNamesZtoElecTau_Data2010B_part447 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_15_1_uAM.root'
)
fileNamesZtoElecTau_Data2010B_part448 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_16_1_ip0.root'
)
fileNamesZtoElecTau_Data2010B_part449 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_17_1_Tgh.root'
)
fileNamesZtoElecTau_Data2010B_part450 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_18_1_A8e.root'
)
fileNamesZtoElecTau_Data2010B_part451 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_19_1_bXm.root'
)
fileNamesZtoElecTau_Data2010B_part452 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_1_1_5f5.root'
)
fileNamesZtoElecTau_Data2010B_part453 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_20_1_D5f.root'
)
fileNamesZtoElecTau_Data2010B_part454 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_21_1_8pV.root'
)
fileNamesZtoElecTau_Data2010B_part455 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_22_1_5nx.root'
)
fileNamesZtoElecTau_Data2010B_part456 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_23_1_RIU.root'
)
fileNamesZtoElecTau_Data2010B_part457 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_24_1_2RS.root'
)
fileNamesZtoElecTau_Data2010B_part458 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_25_1_TFO.root'
)
fileNamesZtoElecTau_Data2010B_part459 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_26_1_8NI.root'
)
fileNamesZtoElecTau_Data2010B_part460 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_27_1_AnH.root'
)
fileNamesZtoElecTau_Data2010B_part461 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_28_1_XSc.root'
)
fileNamesZtoElecTau_Data2010B_part462 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_29_1_9OL.root'
)
fileNamesZtoElecTau_Data2010B_part463 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_2_1_TfR.root'
)
fileNamesZtoElecTau_Data2010B_part464 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_30_1_087.root'
)
fileNamesZtoElecTau_Data2010B_part465 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_31_1_z3T.root'
)
fileNamesZtoElecTau_Data2010B_part466 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_32_1_Y9s.root'
)
fileNamesZtoElecTau_Data2010B_part467 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_33_1_et3.root'
)
fileNamesZtoElecTau_Data2010B_part468 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_34_1_5VE.root'
)
fileNamesZtoElecTau_Data2010B_part469 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_35_1_uFK.root'
)
fileNamesZtoElecTau_Data2010B_part470 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_36_1_3Ek.root'
)
fileNamesZtoElecTau_Data2010B_part471 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_37_1_uyb.root'
)
fileNamesZtoElecTau_Data2010B_part472 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_38_1_xnA.root'
)
fileNamesZtoElecTau_Data2010B_part473 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_39_1_HtZ.root'
)
fileNamesZtoElecTau_Data2010B_part474 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_3_1_LVw.root'
)
fileNamesZtoElecTau_Data2010B_part475 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_40_1_gtp.root'
)
fileNamesZtoElecTau_Data2010B_part476 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_41_1_w1j.root'
)
fileNamesZtoElecTau_Data2010B_part477 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_42_1_RYw.root'
)
fileNamesZtoElecTau_Data2010B_part478 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_43_1_YcD.root'
)
fileNamesZtoElecTau_Data2010B_part479 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_44_1_mxY.root'
)
fileNamesZtoElecTau_Data2010B_part480 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_45_1_VNS.root'
)
fileNamesZtoElecTau_Data2010B_part481 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_46_1_wr2.root'
)
fileNamesZtoElecTau_Data2010B_part482 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_47_1_7dJ.root'
)
fileNamesZtoElecTau_Data2010B_part483 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_48_1_yOr.root'
)
fileNamesZtoElecTau_Data2010B_part484 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_49_1_leL.root'
)
fileNamesZtoElecTau_Data2010B_part485 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_4_1_02A.root'
)
fileNamesZtoElecTau_Data2010B_part486 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_50_1_CTc.root'
)
fileNamesZtoElecTau_Data2010B_part487 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_51_1_htb.root'
)
fileNamesZtoElecTau_Data2010B_part488 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_52_1_GuN.root'
)
fileNamesZtoElecTau_Data2010B_part489 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_53_1_59K.root'
)
fileNamesZtoElecTau_Data2010B_part490 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_54_1_otk.root'
)
fileNamesZtoElecTau_Data2010B_part491 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_55_1_9kz.root'
)
fileNamesZtoElecTau_Data2010B_part492 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_56_1_177.root'
)
fileNamesZtoElecTau_Data2010B_part493 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_57_2_pmM.root'
)
fileNamesZtoElecTau_Data2010B_part494 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_58_1_JBw.root'
)
fileNamesZtoElecTau_Data2010B_part495 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_59_1_Bky.root'
)
fileNamesZtoElecTau_Data2010B_part496 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_5_1_QeK.root'
)
fileNamesZtoElecTau_Data2010B_part497 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_60_1_gya.root'
)
fileNamesZtoElecTau_Data2010B_part498 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_61_1_4k3.root'
)
fileNamesZtoElecTau_Data2010B_part499 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_62_1_z5U.root'
)
fileNamesZtoElecTau_Data2010B_part500 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_63_1_eQw.root'
)
fileNamesZtoElecTau_Data2010B_part501 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_64_1_0lz.root'
)
fileNamesZtoElecTau_Data2010B_part502 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_65_1_LPA.root'
)
fileNamesZtoElecTau_Data2010B_part503 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_66_1_wWR.root'
)
fileNamesZtoElecTau_Data2010B_part504 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_67_2_Y3P.root'
)
fileNamesZtoElecTau_Data2010B_part505 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_68_1_V1T.root'
)
fileNamesZtoElecTau_Data2010B_part506 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_69_1_9mc.root'
)
fileNamesZtoElecTau_Data2010B_part507 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_6_1_Kyl.root'
)
fileNamesZtoElecTau_Data2010B_part508 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_70_1_0OP.root'
)
fileNamesZtoElecTau_Data2010B_part509 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_71_1_rTP.root'
)
fileNamesZtoElecTau_Data2010B_part510 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_72_1_Rim.root'
)
fileNamesZtoElecTau_Data2010B_part511 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_73_1_J8N.root'
)
fileNamesZtoElecTau_Data2010B_part512 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_74_1_SeW.root'
)
fileNamesZtoElecTau_Data2010B_part513 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_75_1_MiF.root'
)
fileNamesZtoElecTau_Data2010B_part514 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_76_1_dsF.root'
)
fileNamesZtoElecTau_Data2010B_part515 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_77_1_fEw.root'
)
fileNamesZtoElecTau_Data2010B_part516 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_78_1_wfy.root'
)
fileNamesZtoElecTau_Data2010B_part517 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_79_1_HFD.root'
)
fileNamesZtoElecTau_Data2010B_part518 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_7_1_qy1.root'
)
fileNamesZtoElecTau_Data2010B_part519 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_80_1_Ovw.root'
)
fileNamesZtoElecTau_Data2010B_part520 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_81_1_sBf.root'
)
fileNamesZtoElecTau_Data2010B_part521 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_82_1_6MC.root'
)
fileNamesZtoElecTau_Data2010B_part522 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_83_1_0G7.root'
)
fileNamesZtoElecTau_Data2010B_part523 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_84_1_6A3.root'
)
fileNamesZtoElecTau_Data2010B_part524 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_85_1_9U1.root'
)
fileNamesZtoElecTau_Data2010B_part525 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_86_1_40j.root'
)
fileNamesZtoElecTau_Data2010B_part526 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_87_1_aty.root'
)
fileNamesZtoElecTau_Data2010B_part527 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_88_1_jIf.root'
)
fileNamesZtoElecTau_Data2010B_part528 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_89_1_qCS.root'
)
fileNamesZtoElecTau_Data2010B_part529 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_8_1_rAq.root'
)
fileNamesZtoElecTau_Data2010B_part530 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_90_1_4c6.root'
)
fileNamesZtoElecTau_Data2010B_part531 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_91_1_9GA.root'
)
fileNamesZtoElecTau_Data2010B_part532 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_92_1_HjY.root'
)
fileNamesZtoElecTau_Data2010B_part533 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_93_1_UG1.root'
)
fileNamesZtoElecTau_Data2010B_part534 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_94_1_Jq2.root'
)
fileNamesZtoElecTau_Data2010B_part535 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_95_1_ABw.root'
)
fileNamesZtoElecTau_Data2010B_part536 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_96_1_OQb.root'
)
fileNamesZtoElecTau_Data2010B_part537 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_97_1_bnR.root'
)
fileNamesZtoElecTau_Data2010B_part538 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_98_1_9RQ.root'
)
fileNamesZtoElecTau_Data2010B_part539 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_99_1_MZs.root'
)
fileNamesZtoElecTau_Data2010B_part540 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part1/skimElecTau_9_1_MCD.root'
)

#  Data2010B - part2

fileNamesZtoElecTau_Data2010B_part541 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_100_1_1ZD.root'
)
fileNamesZtoElecTau_Data2010B_part542 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_101_1_6vO.root'
)
fileNamesZtoElecTau_Data2010B_part543 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_102_1_6ul.root'
)
fileNamesZtoElecTau_Data2010B_part544 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_103_1_Jvv.root'
)
fileNamesZtoElecTau_Data2010B_part545 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_104_1_0PK.root'
)
fileNamesZtoElecTau_Data2010B_part546 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_105_1_ivu.root'
)
fileNamesZtoElecTau_Data2010B_part547 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_106_1_dR9.root'
)
fileNamesZtoElecTau_Data2010B_part548 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_107_1_S9d.root'
)
fileNamesZtoElecTau_Data2010B_part549 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_108_1_cPi.root'
)
fileNamesZtoElecTau_Data2010B_part550 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_109_1_JRG.root'
)
fileNamesZtoElecTau_Data2010B_part551 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_10_1_zA6.root'
)
fileNamesZtoElecTau_Data2010B_part552 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_110_1_tK6.root'
)
fileNamesZtoElecTau_Data2010B_part553 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_111_1_Q3s.root'
)
fileNamesZtoElecTau_Data2010B_part554 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_112_1_Vjh.root'
)
fileNamesZtoElecTau_Data2010B_part555 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_113_1_n68.root'
)
fileNamesZtoElecTau_Data2010B_part556 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_114_2_UPE.root'
)
fileNamesZtoElecTau_Data2010B_part557 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_115_1_yPW.root'
)
fileNamesZtoElecTau_Data2010B_part558 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_116_1_p09.root'
)
fileNamesZtoElecTau_Data2010B_part559 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_117_1_Guo.root'
)
fileNamesZtoElecTau_Data2010B_part560 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_118_1_9kj.root'
)
fileNamesZtoElecTau_Data2010B_part561 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_119_1_7ji.root'
)
fileNamesZtoElecTau_Data2010B_part562 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_11_1_Ew1.root'
)
fileNamesZtoElecTau_Data2010B_part563 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_120_1_msv.root'
)
fileNamesZtoElecTau_Data2010B_part564 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_121_1_HZ7.root'
)
fileNamesZtoElecTau_Data2010B_part565 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_122_1_b08.root'
)
fileNamesZtoElecTau_Data2010B_part566 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_123_1_tjr.root'
)
fileNamesZtoElecTau_Data2010B_part567 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_124_1_XU6.root'
)
fileNamesZtoElecTau_Data2010B_part568 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_125_1_2Oz.root'
)
fileNamesZtoElecTau_Data2010B_part569 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_126_1_xyo.root'
)
fileNamesZtoElecTau_Data2010B_part570 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_127_1_FOO.root'
)
fileNamesZtoElecTau_Data2010B_part571 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_128_1_Ps9.root'
)
fileNamesZtoElecTau_Data2010B_part572 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_129_2_YyX.root'
)
fileNamesZtoElecTau_Data2010B_part573 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_12_2_TAV.root'
)
fileNamesZtoElecTau_Data2010B_part574 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_130_1_X0V.root'
)
fileNamesZtoElecTau_Data2010B_part575 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_131_1_bIX.root'
)
fileNamesZtoElecTau_Data2010B_part576 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_132_1_QEQ.root'
)
fileNamesZtoElecTau_Data2010B_part577 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_133_1_mz4.root'
)
fileNamesZtoElecTau_Data2010B_part578 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_134_1_cPE.root'
)
fileNamesZtoElecTau_Data2010B_part579 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_135_1_XbU.root'
)
fileNamesZtoElecTau_Data2010B_part580 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_136_1_wFa.root'
)
fileNamesZtoElecTau_Data2010B_part581 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_137_1_qjm.root'
)
fileNamesZtoElecTau_Data2010B_part582 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_138_1_Wji.root'
)
fileNamesZtoElecTau_Data2010B_part583 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_139_1_0wZ.root'
)
fileNamesZtoElecTau_Data2010B_part584 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_13_1_IuQ.root'
)
fileNamesZtoElecTau_Data2010B_part585 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_140_1_x8L.root'
)
fileNamesZtoElecTau_Data2010B_part586 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_141_1_Aji.root'
)
fileNamesZtoElecTau_Data2010B_part587 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_142_1_s0E.root'
)
fileNamesZtoElecTau_Data2010B_part588 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_143_1_IE3.root'
)
fileNamesZtoElecTau_Data2010B_part589 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_144_1_bsb.root'
)
fileNamesZtoElecTau_Data2010B_part590 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_145_1_rdf.root'
)
fileNamesZtoElecTau_Data2010B_part591 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_146_1_nS5.root'
)
fileNamesZtoElecTau_Data2010B_part592 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_147_1_TPO.root'
)
fileNamesZtoElecTau_Data2010B_part593 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_148_1_dWk.root'
)
fileNamesZtoElecTau_Data2010B_part594 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_149_1_Cbo.root'
)
fileNamesZtoElecTau_Data2010B_part595 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_14_1_tqo.root'
)
fileNamesZtoElecTau_Data2010B_part596 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_150_1_OIR.root'
)
fileNamesZtoElecTau_Data2010B_part597 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_151_1_OX5.root'
)
fileNamesZtoElecTau_Data2010B_part598 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_152_1_o3V.root'
)
fileNamesZtoElecTau_Data2010B_part599 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_153_1_ohl.root'
)
fileNamesZtoElecTau_Data2010B_part600 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_154_1_gj7.root'
)
fileNamesZtoElecTau_Data2010B_part601 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_155_1_KPw.root'
)
fileNamesZtoElecTau_Data2010B_part602 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_156_1_nrF.root'
)
fileNamesZtoElecTau_Data2010B_part603 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_157_1_NcE.root'
)
fileNamesZtoElecTau_Data2010B_part604 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_158_1_oKI.root'
)
fileNamesZtoElecTau_Data2010B_part605 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_159_1_gSb.root'
)
fileNamesZtoElecTau_Data2010B_part606 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_15_1_L33.root'
)
fileNamesZtoElecTau_Data2010B_part607 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_160_1_y0l.root'
)
fileNamesZtoElecTau_Data2010B_part608 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_161_1_c0X.root'
)
fileNamesZtoElecTau_Data2010B_part609 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_162_1_YXR.root'
)
fileNamesZtoElecTau_Data2010B_part610 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_163_1_sja.root'
)
fileNamesZtoElecTau_Data2010B_part611 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_164_1_8VF.root'
)
fileNamesZtoElecTau_Data2010B_part612 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_165_1_jrw.root'
)
fileNamesZtoElecTau_Data2010B_part613 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_166_2_chl.root'
)
fileNamesZtoElecTau_Data2010B_part614 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_167_2_eX8.root'
)
fileNamesZtoElecTau_Data2010B_part615 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_168_2_2n1.root'
)
fileNamesZtoElecTau_Data2010B_part616 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_169_2_LYa.root'
)
fileNamesZtoElecTau_Data2010B_part617 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_16_1_8bE.root'
)
fileNamesZtoElecTau_Data2010B_part618 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_170_1_mBK.root'
)
fileNamesZtoElecTau_Data2010B_part619 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_171_1_3Mz.root'
)
fileNamesZtoElecTau_Data2010B_part620 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_172_1_psZ.root'
)
fileNamesZtoElecTau_Data2010B_part621 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_173_1_beE.root'
)
fileNamesZtoElecTau_Data2010B_part622 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_174_1_PT8.root'
)
fileNamesZtoElecTau_Data2010B_part623 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_175_1_264.root'
)
fileNamesZtoElecTau_Data2010B_part624 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_176_1_gEv.root'
)
fileNamesZtoElecTau_Data2010B_part625 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_177_1_bip.root'
)
fileNamesZtoElecTau_Data2010B_part626 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_178_1_HIb.root'
)
fileNamesZtoElecTau_Data2010B_part627 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_179_1_I8j.root'
)
fileNamesZtoElecTau_Data2010B_part628 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_17_1_Jg4.root'
)
fileNamesZtoElecTau_Data2010B_part629 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_180_1_tzE.root'
)
fileNamesZtoElecTau_Data2010B_part630 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_181_1_c2n.root'
)
fileNamesZtoElecTau_Data2010B_part631 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_182_1_XFZ.root'
)
fileNamesZtoElecTau_Data2010B_part632 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_183_1_qqT.root'
)
fileNamesZtoElecTau_Data2010B_part633 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_184_1_ZlX.root'
)
fileNamesZtoElecTau_Data2010B_part634 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_185_1_jgY.root'
)
fileNamesZtoElecTau_Data2010B_part635 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_186_1_Mn8.root'
)
fileNamesZtoElecTau_Data2010B_part636 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_187_2_gUb.root'
)
fileNamesZtoElecTau_Data2010B_part637 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_188_1_eGH.root'
)
fileNamesZtoElecTau_Data2010B_part638 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_189_1_75D.root'
)
fileNamesZtoElecTau_Data2010B_part639 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_18_1_nqC.root'
)
fileNamesZtoElecTau_Data2010B_part640 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_190_1_DGb.root'
)
fileNamesZtoElecTau_Data2010B_part641 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_191_2_NIq.root'
)
fileNamesZtoElecTau_Data2010B_part642 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_192_1_6Cp.root'
)
fileNamesZtoElecTau_Data2010B_part643 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_193_1_XZ1.root'
)
fileNamesZtoElecTau_Data2010B_part644 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_194_1_DKV.root'
)
fileNamesZtoElecTau_Data2010B_part645 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_195_1_lyN.root'
)
fileNamesZtoElecTau_Data2010B_part646 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_196_1_iCw.root'
)
fileNamesZtoElecTau_Data2010B_part647 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_197_1_mRM.root'
)
fileNamesZtoElecTau_Data2010B_part648 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_198_1_h2Z.root'
)
fileNamesZtoElecTau_Data2010B_part649 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_199_1_w2r.root'
)
fileNamesZtoElecTau_Data2010B_part650 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_19_1_W2D.root'
)
fileNamesZtoElecTau_Data2010B_part651 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_1_1_0HY.root'
)
fileNamesZtoElecTau_Data2010B_part652 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_200_1_hp3.root'
)
fileNamesZtoElecTau_Data2010B_part653 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_201_1_G7n.root'
)
fileNamesZtoElecTau_Data2010B_part654 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_202_1_mLU.root'
)
fileNamesZtoElecTau_Data2010B_part655 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_203_1_AAs.root'
)
fileNamesZtoElecTau_Data2010B_part656 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_204_1_Z17.root'
)
fileNamesZtoElecTau_Data2010B_part657 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_205_1_ecT.root'
)
fileNamesZtoElecTau_Data2010B_part658 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_20_1_2vU.root'
)
fileNamesZtoElecTau_Data2010B_part659 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_21_1_2tl.root'
)
fileNamesZtoElecTau_Data2010B_part660 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_22_1_rJk.root'
)
fileNamesZtoElecTau_Data2010B_part661 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_23_1_2nN.root'
)
fileNamesZtoElecTau_Data2010B_part662 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_24_1_CDd.root'
)
fileNamesZtoElecTau_Data2010B_part663 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_25_1_A0p.root'
)
fileNamesZtoElecTau_Data2010B_part664 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_26_1_h5P.root'
)
fileNamesZtoElecTau_Data2010B_part665 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_27_1_aab.root'
)
fileNamesZtoElecTau_Data2010B_part666 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_28_1_U4I.root'
)
fileNamesZtoElecTau_Data2010B_part667 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_29_1_dGq.root'
)
fileNamesZtoElecTau_Data2010B_part668 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_2_1_wdZ.root'
)
fileNamesZtoElecTau_Data2010B_part669 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_30_1_eog.root'
)
fileNamesZtoElecTau_Data2010B_part670 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_31_1_cs4.root'
)
fileNamesZtoElecTau_Data2010B_part671 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_32_1_W8d.root'
)
fileNamesZtoElecTau_Data2010B_part672 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_33_1_y9E.root'
)
fileNamesZtoElecTau_Data2010B_part673 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_34_1_rCn.root'
)
fileNamesZtoElecTau_Data2010B_part674 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_35_1_XBQ.root'
)
fileNamesZtoElecTau_Data2010B_part675 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_36_1_AVj.root'
)
fileNamesZtoElecTau_Data2010B_part676 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_37_1_g3n.root'
)
fileNamesZtoElecTau_Data2010B_part677 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_38_1_C8W.root'
)
fileNamesZtoElecTau_Data2010B_part678 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_39_1_XUm.root'
)
fileNamesZtoElecTau_Data2010B_part679 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_3_1_tIn.root'
)
fileNamesZtoElecTau_Data2010B_part680 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_40_1_7RK.root'
)
fileNamesZtoElecTau_Data2010B_part681 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_41_1_n1s.root'
)
fileNamesZtoElecTau_Data2010B_part682 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_42_1_6Rh.root'
)
fileNamesZtoElecTau_Data2010B_part683 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_43_1_xG7.root'
)
fileNamesZtoElecTau_Data2010B_part684 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_44_1_HJd.root'
)
fileNamesZtoElecTau_Data2010B_part685 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_45_1_zqn.root'
)
fileNamesZtoElecTau_Data2010B_part686 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_46_1_Ehu.root'
)
fileNamesZtoElecTau_Data2010B_part687 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_47_1_Z8S.root'
)
fileNamesZtoElecTau_Data2010B_part688 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_48_1_cOw.root'
)
fileNamesZtoElecTau_Data2010B_part689 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_49_1_37D.root'
)
fileNamesZtoElecTau_Data2010B_part690 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_4_2_7ZW.root'
)
fileNamesZtoElecTau_Data2010B_part691 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_50_1_46C.root'
)
fileNamesZtoElecTau_Data2010B_part692 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_51_1_7sb.root'
)
fileNamesZtoElecTau_Data2010B_part693 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_52_1_QL7.root'
)
fileNamesZtoElecTau_Data2010B_part694 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_53_1_i9l.root'
)
fileNamesZtoElecTau_Data2010B_part695 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_54_1_wTL.root'
)
fileNamesZtoElecTau_Data2010B_part696 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_55_1_d7Y.root'
)
fileNamesZtoElecTau_Data2010B_part697 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_56_1_KAO.root'
)
fileNamesZtoElecTau_Data2010B_part698 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_57_1_OLG.root'
)
fileNamesZtoElecTau_Data2010B_part699 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_58_1_3pB.root'
)
fileNamesZtoElecTau_Data2010B_part700 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_59_1_Lu9.root'
)
fileNamesZtoElecTau_Data2010B_part701 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_5_1_1ny.root'
)
fileNamesZtoElecTau_Data2010B_part702 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_60_2_Ztx.root'
)
fileNamesZtoElecTau_Data2010B_part703 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_61_2_X6O.root'
)
fileNamesZtoElecTau_Data2010B_part704 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_62_1_6S2.root'
)
fileNamesZtoElecTau_Data2010B_part705 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_63_1_eBs.root'
)
fileNamesZtoElecTau_Data2010B_part706 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_64_1_2MP.root'
)
fileNamesZtoElecTau_Data2010B_part707 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_65_2_GSr.root'
)
fileNamesZtoElecTau_Data2010B_part708 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_66_2_wfp.root'
)
fileNamesZtoElecTau_Data2010B_part709 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_67_1_R4A.root'
)
fileNamesZtoElecTau_Data2010B_part710 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_68_1_zeS.root'
)
fileNamesZtoElecTau_Data2010B_part711 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_69_2_OGi.root'
)
fileNamesZtoElecTau_Data2010B_part712 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_6_1_Xce.root'
)
fileNamesZtoElecTau_Data2010B_part713 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_70_1_yhS.root'
)
fileNamesZtoElecTau_Data2010B_part714 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_71_1_X4x.root'
)
fileNamesZtoElecTau_Data2010B_part715 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_72_1_uDW.root'
)
fileNamesZtoElecTau_Data2010B_part716 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_73_1_drz.root'
)
fileNamesZtoElecTau_Data2010B_part717 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_74_1_79P.root'
)
fileNamesZtoElecTau_Data2010B_part718 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_75_1_uN2.root'
)
fileNamesZtoElecTau_Data2010B_part719 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_76_1_OAb.root'
)
fileNamesZtoElecTau_Data2010B_part720 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_77_1_NZh.root'
)
fileNamesZtoElecTau_Data2010B_part721 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_78_1_AGv.root'
)
fileNamesZtoElecTau_Data2010B_part722 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_79_1_2JP.root'
)
fileNamesZtoElecTau_Data2010B_part723 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_7_1_u3C.root'
)
fileNamesZtoElecTau_Data2010B_part724 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_80_1_zfR.root'
)
fileNamesZtoElecTau_Data2010B_part725 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_81_1_FEp.root'
)
fileNamesZtoElecTau_Data2010B_part726 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_82_1_5rt.root'
)
fileNamesZtoElecTau_Data2010B_part727 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_83_1_w69.root'
)
fileNamesZtoElecTau_Data2010B_part728 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_84_1_TgL.root'
)
fileNamesZtoElecTau_Data2010B_part729 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_85_1_CP7.root'
)
fileNamesZtoElecTau_Data2010B_part730 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_86_1_xdG.root'
)
fileNamesZtoElecTau_Data2010B_part731 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_87_1_19B.root'
)
fileNamesZtoElecTau_Data2010B_part732 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_88_1_Bdg.root'
)
fileNamesZtoElecTau_Data2010B_part733 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_89_1_GOm.root'
)
fileNamesZtoElecTau_Data2010B_part734 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_8_1_Enn.root'
)
fileNamesZtoElecTau_Data2010B_part735 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_90_1_7u7.root'
)
fileNamesZtoElecTau_Data2010B_part736 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_91_1_R6i.root'
)
fileNamesZtoElecTau_Data2010B_part737 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_92_1_qgk.root'
)
fileNamesZtoElecTau_Data2010B_part738 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_93_1_mSR.root'
)
fileNamesZtoElecTau_Data2010B_part739 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_94_1_ATu.root'
)
fileNamesZtoElecTau_Data2010B_part740 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_95_1_9N7.root'
)
fileNamesZtoElecTau_Data2010B_part741 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_96_2_tt9.root'
)
fileNamesZtoElecTau_Data2010B_part742 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_97_1_rMK.root'
)
fileNamesZtoElecTau_Data2010B_part743 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_98_1_s6T.root'
)
fileNamesZtoElecTau_Data2010B_part744 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_99_1_dsZ.root'
)
fileNamesZtoElecTau_Data2010B_part745 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part2_2/skimElecTau_9_1_LMA.root'
)

# Electron PD - prompt reco - part3
# run range: 147117-147454

fileNamesZtoElecTau_Data2010B_part746 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_100_1_WVM.root'
)
fileNamesZtoElecTau_Data2010B_part747 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_101_1_u2f.root'
)
fileNamesZtoElecTau_Data2010B_part748 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_102_1_Efn.root'
)
fileNamesZtoElecTau_Data2010B_part749 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_103_1_vD0.root'
)
fileNamesZtoElecTau_Data2010B_part750 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_104_1_h9H.root'
)
fileNamesZtoElecTau_Data2010B_part751 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_105_1_pnh.root'
)
fileNamesZtoElecTau_Data2010B_part752 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_106_1_fXM.root'
)
fileNamesZtoElecTau_Data2010B_part753 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_107_1_rg4.root'
)
fileNamesZtoElecTau_Data2010B_part754 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_108_1_RA9.root'
)
fileNamesZtoElecTau_Data2010B_part755 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_109_1_u6Q.root'
)
fileNamesZtoElecTau_Data2010B_part756 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_10_1_7bC.root'
)
fileNamesZtoElecTau_Data2010B_part757 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_110_1_j7K.root'
)
fileNamesZtoElecTau_Data2010B_part758 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_111_1_1BL.root'
)
fileNamesZtoElecTau_Data2010B_part759 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_112_1_w9b.root'
)
fileNamesZtoElecTau_Data2010B_part760 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_113_1_BIg.root'
)
fileNamesZtoElecTau_Data2010B_part761 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_114_1_und.root'
)
fileNamesZtoElecTau_Data2010B_part762 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_115_1_DDe.root'
)
fileNamesZtoElecTau_Data2010B_part763 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_116_1_oiz.root'
)
fileNamesZtoElecTau_Data2010B_part764 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_117_1_VW8.root'
)
fileNamesZtoElecTau_Data2010B_part765 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_118_1_88r.root'
)
fileNamesZtoElecTau_Data2010B_part766 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_119_1_LMo.root'
)
fileNamesZtoElecTau_Data2010B_part767 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_11_1_lzC.root'
)
fileNamesZtoElecTau_Data2010B_part768 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_120_1_L86.root'
)
fileNamesZtoElecTau_Data2010B_part769 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_121_1_TUA.root'
)
fileNamesZtoElecTau_Data2010B_part770 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_122_1_661.root'
)
fileNamesZtoElecTau_Data2010B_part771 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_123_1_UGg.root'
)
fileNamesZtoElecTau_Data2010B_part772 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_124_1_tsd.root'
)
fileNamesZtoElecTau_Data2010B_part773 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_125_1_uCM.root'
)
fileNamesZtoElecTau_Data2010B_part774 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_126_1_Cqi.root'
)
fileNamesZtoElecTau_Data2010B_part775 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_127_1_zEm.root'
)
fileNamesZtoElecTau_Data2010B_part776 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_128_1_uaQ.root'
)
fileNamesZtoElecTau_Data2010B_part777 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_129_1_g89.root'
)
fileNamesZtoElecTau_Data2010B_part778 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_12_1_zmc.root'
)
fileNamesZtoElecTau_Data2010B_part779 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_130_1_PrX.root'
)
fileNamesZtoElecTau_Data2010B_part780 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_131_1_K4c.root'
)
fileNamesZtoElecTau_Data2010B_part781 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_132_1_Zh6.root'
)
fileNamesZtoElecTau_Data2010B_part782 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_133_1_wq3.root'
)
fileNamesZtoElecTau_Data2010B_part783 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_134_1_QXN.root'
)
fileNamesZtoElecTau_Data2010B_part784 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_135_1_7vy.root'
)
fileNamesZtoElecTau_Data2010B_part785 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_136_1_9Rz.root'
)
fileNamesZtoElecTau_Data2010B_part786 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_137_1_D46.root'
)
fileNamesZtoElecTau_Data2010B_part787 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_138_1_jEr.root'
)
fileNamesZtoElecTau_Data2010B_part788 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_139_1_Ile.root'
)
fileNamesZtoElecTau_Data2010B_part789 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_13_1_ugg.root'
)
fileNamesZtoElecTau_Data2010B_part790 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_140_1_PhH.root'
)
fileNamesZtoElecTau_Data2010B_part791 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_14_1_iYW.root'
)
fileNamesZtoElecTau_Data2010B_part792 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_15_1_XL6.root'
)
fileNamesZtoElecTau_Data2010B_part793 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_16_1_gNk.root'
)
fileNamesZtoElecTau_Data2010B_part794 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_17_1_Kqc.root'
)
fileNamesZtoElecTau_Data2010B_part795 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_18_1_IrJ.root'
)
fileNamesZtoElecTau_Data2010B_part796 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_19_1_Hds.root'
)
fileNamesZtoElecTau_Data2010B_part797 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_1_1_sqZ.root'
)
fileNamesZtoElecTau_Data2010B_part798 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_20_1_zTO.root'
)
fileNamesZtoElecTau_Data2010B_part799 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_21_1_vRz.root'
)
fileNamesZtoElecTau_Data2010B_part800 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_22_1_3AZ.root'
)
fileNamesZtoElecTau_Data2010B_part801 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_23_1_UaF.root'
)
fileNamesZtoElecTau_Data2010B_part802 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_24_1_xzG.root'
)
fileNamesZtoElecTau_Data2010B_part803 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_25_1_1Xn.root'
)
fileNamesZtoElecTau_Data2010B_part804 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_26_1_Zco.root'
)
fileNamesZtoElecTau_Data2010B_part805 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_27_1_XxN.root'
)
fileNamesZtoElecTau_Data2010B_part806 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_28_1_UiI.root'
)
fileNamesZtoElecTau_Data2010B_part807 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_29_1_nOF.root'
)
fileNamesZtoElecTau_Data2010B_part808 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_2_1_Mfn.root'
)
fileNamesZtoElecTau_Data2010B_part809 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_30_1_5ma.root'
)
fileNamesZtoElecTau_Data2010B_part810 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_31_1_CHo.root'
)
fileNamesZtoElecTau_Data2010B_part811 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_32_1_Vq9.root'
)
fileNamesZtoElecTau_Data2010B_part812 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_33_1_uHu.root'
)
fileNamesZtoElecTau_Data2010B_part813 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_34_1_qwQ.root'
)
fileNamesZtoElecTau_Data2010B_part814 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_35_1_qoV.root'
)
fileNamesZtoElecTau_Data2010B_part815 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_36_1_g55.root'
)
fileNamesZtoElecTau_Data2010B_part816 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_37_1_0vj.root'
)
fileNamesZtoElecTau_Data2010B_part817 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_38_1_iKo.root'
)
fileNamesZtoElecTau_Data2010B_part818 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_39_1_9yc.root'
)
fileNamesZtoElecTau_Data2010B_part819 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_3_1_1Hh.root'
)
fileNamesZtoElecTau_Data2010B_part820 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_40_1_y5i.root'
)
fileNamesZtoElecTau_Data2010B_part821 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_41_1_GFj.root'
)
fileNamesZtoElecTau_Data2010B_part822 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_42_1_WZn.root'
)
fileNamesZtoElecTau_Data2010B_part823 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_43_1_APq.root'
)
fileNamesZtoElecTau_Data2010B_part824 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_44_1_ZtQ.root'
)
fileNamesZtoElecTau_Data2010B_part825 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_45_1_Fh7.root'
)
fileNamesZtoElecTau_Data2010B_part826 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_46_1_GDc.root'
)
fileNamesZtoElecTau_Data2010B_part827 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_47_1_U6d.root'
)
fileNamesZtoElecTau_Data2010B_part828 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_48_1_O9F.root'
)
fileNamesZtoElecTau_Data2010B_part829 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_49_1_OdW.root'
)
fileNamesZtoElecTau_Data2010B_part830 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_4_1_VCU.root'
)
fileNamesZtoElecTau_Data2010B_part831 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_50_1_Y1Y.root'
)
fileNamesZtoElecTau_Data2010B_part832 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_51_1_k4X.root'
)
fileNamesZtoElecTau_Data2010B_part833 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_52_1_PHc.root'
)
fileNamesZtoElecTau_Data2010B_part834 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_53_1_iKq.root'
)
fileNamesZtoElecTau_Data2010B_part835 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_54_1_CNN.root'
)
fileNamesZtoElecTau_Data2010B_part836 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_55_1_Hwv.root'
)
fileNamesZtoElecTau_Data2010B_part837 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_56_1_foz.root'
)
fileNamesZtoElecTau_Data2010B_part838 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_57_1_uSg.root'
)
fileNamesZtoElecTau_Data2010B_part839 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_58_1_D4s.root'
)
fileNamesZtoElecTau_Data2010B_part840 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_59_1_Pdv.root'
)
fileNamesZtoElecTau_Data2010B_part841 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_5_1_9Sr.root'
)
fileNamesZtoElecTau_Data2010B_part842 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_60_1_oH2.root'
)
fileNamesZtoElecTau_Data2010B_part843 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_61_1_HZv.root'
)
fileNamesZtoElecTau_Data2010B_part844 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_62_1_6FN.root'
)
fileNamesZtoElecTau_Data2010B_part845 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_63_1_buM.root'
)
fileNamesZtoElecTau_Data2010B_part846 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_64_1_Liz.root'
)
fileNamesZtoElecTau_Data2010B_part847 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_65_1_YMy.root'
)
fileNamesZtoElecTau_Data2010B_part848 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_66_1_5fl.root'
)
fileNamesZtoElecTau_Data2010B_part849 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_67_1_rfe.root'
)
fileNamesZtoElecTau_Data2010B_part850 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_68_1_Od2.root'
)
fileNamesZtoElecTau_Data2010B_part851 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_69_1_XWN.root'
)
fileNamesZtoElecTau_Data2010B_part852 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_6_1_TbD.root'
)
fileNamesZtoElecTau_Data2010B_part853 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_70_1_8Za.root'
)
fileNamesZtoElecTau_Data2010B_part854 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_71_1_aeS.root'
)
fileNamesZtoElecTau_Data2010B_part855 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_72_1_lu7.root'
)
fileNamesZtoElecTau_Data2010B_part856 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_73_1_lB0.root'
)
fileNamesZtoElecTau_Data2010B_part857 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_74_1_dR5.root'
)
fileNamesZtoElecTau_Data2010B_part858 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_75_1_fm9.root'
)
fileNamesZtoElecTau_Data2010B_part859 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_76_1_ghV.root'
)
fileNamesZtoElecTau_Data2010B_part860 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_77_1_qzR.root'
)
fileNamesZtoElecTau_Data2010B_part861 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_78_1_XX8.root'
)
fileNamesZtoElecTau_Data2010B_part862 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_79_1_rbw.root'
)
fileNamesZtoElecTau_Data2010B_part863 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_7_1_95J.root'
)
fileNamesZtoElecTau_Data2010B_part864 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_80_1_4H9.root'
)
fileNamesZtoElecTau_Data2010B_part865 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_81_1_F0j.root'
)
fileNamesZtoElecTau_Data2010B_part866 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_82_1_H6Q.root'
)
fileNamesZtoElecTau_Data2010B_part867 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_83_1_vtp.root'
)
fileNamesZtoElecTau_Data2010B_part868 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_84_1_ZP6.root'
)
fileNamesZtoElecTau_Data2010B_part869 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_85_1_YSq.root'
)
fileNamesZtoElecTau_Data2010B_part870 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_86_1_JVw.root'
)
fileNamesZtoElecTau_Data2010B_part871 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_87_1_eL6.root'
)
fileNamesZtoElecTau_Data2010B_part872 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_88_1_aUW.root'
)
fileNamesZtoElecTau_Data2010B_part873 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_89_1_Vvg.root'
)
fileNamesZtoElecTau_Data2010B_part874 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_8_1_EQX.root'
)
fileNamesZtoElecTau_Data2010B_part875 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_90_1_d1v.root'
)
fileNamesZtoElecTau_Data2010B_part876 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_91_1_o38.root'
)
fileNamesZtoElecTau_Data2010B_part877 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_92_1_s7G.root'
)
fileNamesZtoElecTau_Data2010B_part878 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_93_1_Idv.root'
)
fileNamesZtoElecTau_Data2010B_part879 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_94_1_eUd.root'
)
fileNamesZtoElecTau_Data2010B_part880 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_95_1_nLu.root'
)
fileNamesZtoElecTau_Data2010B_part881 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_96_1_Xwo.root'
)
fileNamesZtoElecTau_Data2010B_part882 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_97_1_BF8.root'
)
fileNamesZtoElecTau_Data2010B_part883 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_98_1_Xbq.root'
)
fileNamesZtoElecTau_Data2010B_part884 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_99_1_KWx.root'
)
fileNamesZtoElecTau_Data2010B_part885 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part3/skimElecTau_9_1_723.root'
)

#  EPD_PR part 4

fileNamesZtoElecTau_Data2010B_part886 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_10_1_h4X.root'
)
fileNamesZtoElecTau_Data2010B_part887 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_11_1_OkH.root'
)
fileNamesZtoElecTau_Data2010B_part888 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_12_1_sVp.root'
)
fileNamesZtoElecTau_Data2010B_part889 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_13_1_oO9.root'
)
fileNamesZtoElecTau_Data2010B_part890 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_14_1_vlZ.root'
)
fileNamesZtoElecTau_Data2010B_part891 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_15_1_32Q.root'
)
fileNamesZtoElecTau_Data2010B_part892 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_16_1_9ym.root'
)
fileNamesZtoElecTau_Data2010B_part893 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_17_1_6Ro.root'
)
fileNamesZtoElecTau_Data2010B_part894 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_18_1_2Tv.root'
)
fileNamesZtoElecTau_Data2010B_part895 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_19_1_FNc.root'
)
fileNamesZtoElecTau_Data2010B_part896 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_1_1_Mgt.root'
)
fileNamesZtoElecTau_Data2010B_part897 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_20_1_Okn.root'
)
fileNamesZtoElecTau_Data2010B_part898 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_21_1_NyO.root'
)
fileNamesZtoElecTau_Data2010B_part899 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_22_1_vD2.root'
)
fileNamesZtoElecTau_Data2010B_part900 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_23_1_rnW.root'
)
fileNamesZtoElecTau_Data2010B_part901 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_24_1_Vm4.root'
)
fileNamesZtoElecTau_Data2010B_part902 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_25_1_q3p.root'
)
fileNamesZtoElecTau_Data2010B_part903 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_26_1_B5X.root'
)
fileNamesZtoElecTau_Data2010B_part904 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_27_1_HH8.root'
)
fileNamesZtoElecTau_Data2010B_part905 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_28_1_imX.root'
)
fileNamesZtoElecTau_Data2010B_part906 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_29_1_zwd.root'
)
fileNamesZtoElecTau_Data2010B_part907 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_2_1_wWE.root'
)
fileNamesZtoElecTau_Data2010B_part908 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_30_1_OmC.root'
)
fileNamesZtoElecTau_Data2010B_part909 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_31_1_gvy.root'
)
fileNamesZtoElecTau_Data2010B_part910 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_32_1_WT0.root'
)
fileNamesZtoElecTau_Data2010B_part911 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_33_1_dMD.root'
)
fileNamesZtoElecTau_Data2010B_part912 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_34_1_IB4.root'
)
fileNamesZtoElecTau_Data2010B_part913 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_35_1_dnO.root'
)
fileNamesZtoElecTau_Data2010B_part914 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_36_1_ux9.root'
)
fileNamesZtoElecTau_Data2010B_part915 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_37_1_tRg.root'
)
fileNamesZtoElecTau_Data2010B_part916 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_38_1_k5h.root'
)
fileNamesZtoElecTau_Data2010B_part917 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_39_1_xrI.root'
)
fileNamesZtoElecTau_Data2010B_part918 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_3_1_2j4.root'
)
fileNamesZtoElecTau_Data2010B_part919 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_40_1_7TD.root'
)
fileNamesZtoElecTau_Data2010B_part920 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_41_1_6c5.root'
)
fileNamesZtoElecTau_Data2010B_part921 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_42_1_aqa.root'
)
fileNamesZtoElecTau_Data2010B_part922 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_43_1_68p.root'
)
fileNamesZtoElecTau_Data2010B_part923 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_44_1_qVm.root'
)
fileNamesZtoElecTau_Data2010B_part924 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_45_1_5KH.root'
)
fileNamesZtoElecTau_Data2010B_part925 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_46_1_bs5.root'
)
fileNamesZtoElecTau_Data2010B_part926 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_47_1_S3s.root'
)
fileNamesZtoElecTau_Data2010B_part927 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_48_1_MTs.root'
)
fileNamesZtoElecTau_Data2010B_part928 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_49_3_w1y.root'
)
fileNamesZtoElecTau_Data2010B_part929 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_4_1_KZ7.root'
)
fileNamesZtoElecTau_Data2010B_part930 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_50_1_iJo.root'
)
fileNamesZtoElecTau_Data2010B_part931 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_51_1_pUr.root'
)
fileNamesZtoElecTau_Data2010B_part932 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_52_1_SAl.root'
)
fileNamesZtoElecTau_Data2010B_part933 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_53_1_PN4.root'
)
fileNamesZtoElecTau_Data2010B_part934 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_54_1_Ffq.root'
)
fileNamesZtoElecTau_Data2010B_part935 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_55_1_nwU.root'
)
fileNamesZtoElecTau_Data2010B_part936 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_56_1_rvR.root'
)
fileNamesZtoElecTau_Data2010B_part937 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_57_1_BaH.root'
)
fileNamesZtoElecTau_Data2010B_part938 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_58_1_WwR.root'
)
fileNamesZtoElecTau_Data2010B_part939 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_59_1_ZTB.root'
)
fileNamesZtoElecTau_Data2010B_part940 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_5_1_oCx.root'
)
fileNamesZtoElecTau_Data2010B_part941 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_60_1_CEf.root'
)
fileNamesZtoElecTau_Data2010B_part942 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_61_1_xSi.root'
)
fileNamesZtoElecTau_Data2010B_part943 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_62_1_QNf.root'
)
fileNamesZtoElecTau_Data2010B_part944 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_63_1_voT.root'
)
fileNamesZtoElecTau_Data2010B_part945 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_64_1_SUb.root'
)
fileNamesZtoElecTau_Data2010B_part946 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_65_1_eEh.root'
)
fileNamesZtoElecTau_Data2010B_part947 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_66_1_fQi.root'
)
fileNamesZtoElecTau_Data2010B_part948 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_67_1_7gL.root'
)
fileNamesZtoElecTau_Data2010B_part949 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_68_1_SKr.root'
)
fileNamesZtoElecTau_Data2010B_part950 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_69_1_k7A.root'
)
fileNamesZtoElecTau_Data2010B_part951 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_6_1_SMl.root'
)
fileNamesZtoElecTau_Data2010B_part952 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_70_1_mSh.root'
)
fileNamesZtoElecTau_Data2010B_part953 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_71_1_HwB.root'
)
fileNamesZtoElecTau_Data2010B_part954 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_72_1_jQt.root'
)
fileNamesZtoElecTau_Data2010B_part955 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_73_1_f9Q.root'
)
fileNamesZtoElecTau_Data2010B_part956 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_74_1_okr.root'
)
fileNamesZtoElecTau_Data2010B_part957 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_7_1_e1g.root'
)
fileNamesZtoElecTau_Data2010B_part958 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_8_1_asf.root'
)
fileNamesZtoElecTau_Data2010B_part959 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part4/skimElecTau_9_1_1ms.root'
)

# part 5

fileNamesZtoElecTau_Data2010B_part960 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_10_1_fdz.root'
)
fileNamesZtoElecTau_Data2010B_part961 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_11_1_IfE.root'
)
fileNamesZtoElecTau_Data2010B_part962 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_12_1_HiK.root'
)
fileNamesZtoElecTau_Data2010B_part963 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_13_1_GA0.root'
)
fileNamesZtoElecTau_Data2010B_part964 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_14_1_tbM.root'
)
fileNamesZtoElecTau_Data2010B_part965 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_15_1_Xh3.root'
)
fileNamesZtoElecTau_Data2010B_part966 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_16_1_HIH.root'
)
fileNamesZtoElecTau_Data2010B_part967 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_17_1_7oA.root'
)
fileNamesZtoElecTau_Data2010B_part968 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_18_1_41j.root'
)
fileNamesZtoElecTau_Data2010B_part969 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_19_1_rnF.root'
)
fileNamesZtoElecTau_Data2010B_part970 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_1_1_5Fp.root'
)
fileNamesZtoElecTau_Data2010B_part971 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_20_1_7Sa.root'
)
fileNamesZtoElecTau_Data2010B_part972 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_21_1_kos.root'
)
fileNamesZtoElecTau_Data2010B_part973 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_22_1_Dk4.root'
)
fileNamesZtoElecTau_Data2010B_part974 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_23_1_p0v.root'
)
fileNamesZtoElecTau_Data2010B_part975 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_24_1_HPJ.root'
)
fileNamesZtoElecTau_Data2010B_part976 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_25_1_VES.root'
)
fileNamesZtoElecTau_Data2010B_part977 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_26_1_3nV.root'
)
fileNamesZtoElecTau_Data2010B_part978 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_27_1_CWO.root'
)
fileNamesZtoElecTau_Data2010B_part979 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_28_1_nrK.root'
)
fileNamesZtoElecTau_Data2010B_part980 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_29_1_m40.root'
)
fileNamesZtoElecTau_Data2010B_part981 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_2_1_q5h.root'
)
fileNamesZtoElecTau_Data2010B_part982 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_30_1_ZbU.root'
)
fileNamesZtoElecTau_Data2010B_part983 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_31_1_Vme.root'
)
fileNamesZtoElecTau_Data2010B_part984 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_32_1_Jg4.root'
)
fileNamesZtoElecTau_Data2010B_part985 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_33_1_3Of.root'
)
fileNamesZtoElecTau_Data2010B_part986 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_34_1_cuX.root'
)
fileNamesZtoElecTau_Data2010B_part987 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_35_1_M1R.root'
)
fileNamesZtoElecTau_Data2010B_part988 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_36_1_zqN.root'
)
fileNamesZtoElecTau_Data2010B_part989 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_37_1_eWV.root'
)
fileNamesZtoElecTau_Data2010B_part990 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_38_1_38h.root'
)
fileNamesZtoElecTau_Data2010B_part991 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_39_1_aPW.root'
)
fileNamesZtoElecTau_Data2010B_part992 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_3_1_ZrM.root'
)
fileNamesZtoElecTau_Data2010B_part993 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_40_1_IPr.root'
)
fileNamesZtoElecTau_Data2010B_part994 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_41_1_gqk.root'
)
fileNamesZtoElecTau_Data2010B_part995 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_42_1_JTp.root'
)
fileNamesZtoElecTau_Data2010B_part996 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_43_1_oPJ.root'
)
fileNamesZtoElecTau_Data2010B_part997 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_44_1_h3L.root'
)
fileNamesZtoElecTau_Data2010B_part998 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_45_2_Enf.root'
)
fileNamesZtoElecTau_Data2010B_part999 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_46_1_KSD.root'
)
fileNamesZtoElecTau_Data2010B_part1000 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_47_1_NDb.root'
)
fileNamesZtoElecTau_Data2010B_part1001 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_48_1_DWA.root'
)
fileNamesZtoElecTau_Data2010B_part1002 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_49_1_39P.root'
)
fileNamesZtoElecTau_Data2010B_part1003 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_4_1_PUT.root'
)
fileNamesZtoElecTau_Data2010B_part1004 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_50_1_BoE.root'
)
fileNamesZtoElecTau_Data2010B_part1005 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_51_1_bJB.root'
)
fileNamesZtoElecTau_Data2010B_part1006 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_51_2_4ud.root'
)
fileNamesZtoElecTau_Data2010B_part1007 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_52_1_wB9.root'
)
fileNamesZtoElecTau_Data2010B_part1008 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_53_1_ais.root'
)
fileNamesZtoElecTau_Data2010B_part1009 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_54_1_SEB.root'
)
fileNamesZtoElecTau_Data2010B_part1010 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_55_1_KkM.root'
)
fileNamesZtoElecTau_Data2010B_part1011 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_56_1_ee0.root'
)
fileNamesZtoElecTau_Data2010B_part1012 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_57_1_Mab.root'
)
fileNamesZtoElecTau_Data2010B_part1013 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_58_1_2jx.root'
)
fileNamesZtoElecTau_Data2010B_part1014 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_59_1_5dN.root'
)
fileNamesZtoElecTau_Data2010B_part1015 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_5_1_nQy.root'
)
fileNamesZtoElecTau_Data2010B_part1016 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_60_1_YtY.root'
)
fileNamesZtoElecTau_Data2010B_part1017 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_61_1_7wm.root'
)
fileNamesZtoElecTau_Data2010B_part1018 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_62_1_lIr.root'
)
fileNamesZtoElecTau_Data2010B_part1019 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_63_1_amK.root'
)
fileNamesZtoElecTau_Data2010B_part1020 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_64_1_ZgX.root'
)
fileNamesZtoElecTau_Data2010B_part1021 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_65_1_hGf.root'
)
fileNamesZtoElecTau_Data2010B_part1022 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_66_1_Da9.root'
)
fileNamesZtoElecTau_Data2010B_part1023 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_67_1_tPa.root'
)
fileNamesZtoElecTau_Data2010B_part1024 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_68_1_BSg.root'
)
fileNamesZtoElecTau_Data2010B_part1025 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_69_1_2tu.root'
)
fileNamesZtoElecTau_Data2010B_part1026 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_6_1_0zU.root'
)
fileNamesZtoElecTau_Data2010B_part1027 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_70_1_AtL.root'
)
fileNamesZtoElecTau_Data2010B_part1028 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_71_1_Ecy.root'
)
fileNamesZtoElecTau_Data2010B_part1029 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_72_1_iB2.root'
)
fileNamesZtoElecTau_Data2010B_part1030 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_73_1_KLC.root'
)
fileNamesZtoElecTau_Data2010B_part1031 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_7_1_ec4.root'
)
fileNamesZtoElecTau_Data2010B_part1032 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_8_1_XAw.root'
)
fileNamesZtoElecTau_Data2010B_part1033 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part5/skimElecTau_9_1_TgG.root'
)

#  part 6  - 12.8/pb

fileNamesZtoElecTau_Data2010B_part1034 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_26_1_kFt.root'
)
fileNamesZtoElecTau_Data2010B_part1035 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_27_1_HAU.root'
)
fileNamesZtoElecTau_Data2010B_part1036 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_28_1_Upb.root'
)
fileNamesZtoElecTau_Data2010B_part1037 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_29_1_wQQ.root'
)
fileNamesZtoElecTau_Data2010B_part1038 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_30_1_Mkz.root'
)
fileNamesZtoElecTau_Data2010B_part1039 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_31_1_fad.root'
)
fileNamesZtoElecTau_Data2010B_part1040 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_32_1_60C.root'
)
fileNamesZtoElecTau_Data2010B_part1041 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_33_1_eN2.root'
)
fileNamesZtoElecTau_Data2010B_part1042 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_34_1_bE0.root'
)
fileNamesZtoElecTau_Data2010B_part1043 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_35_1_23R.root'
)
fileNamesZtoElecTau_Data2010B_part1044 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_36_1_1ep.root'
)
fileNamesZtoElecTau_Data2010B_part1045 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_37_1_tTv.root'
)
fileNamesZtoElecTau_Data2010B_part1046 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_38_1_tHM.root'
)
fileNamesZtoElecTau_Data2010B_part1047 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_39_1_H49.root'
)
fileNamesZtoElecTau_Data2010B_part1048 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_40_1_sSp.root'
)
fileNamesZtoElecTau_Data2010B_part1049 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_41_1_NUW.root'
)
fileNamesZtoElecTau_Data2010B_part1050 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_42_1_ghP.root'
)
fileNamesZtoElecTau_Data2010B_part1051 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_43_1_m9C.root'
)
fileNamesZtoElecTau_Data2010B_part1052 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_44_1_Wnq.root'
)
fileNamesZtoElecTau_Data2010B_part1053 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_45_1_T5S.root'
)
fileNamesZtoElecTau_Data2010B_part1054 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_46_1_swz.root'
)
fileNamesZtoElecTau_Data2010B_part1055 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_47_1_muf.root'
)
fileNamesZtoElecTau_Data2010B_part1056 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_48_1_WP1.root'
)
fileNamesZtoElecTau_Data2010B_part1057 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_49_1_kvS.root'
)
fileNamesZtoElecTau_Data2010B_part1058 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_50_1_ogO.root'
)
fileNamesZtoElecTau_Data2010B_part1059 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_51_1_4rW.root'
)
fileNamesZtoElecTau_Data2010B_part1060 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_52_1_hp7.root'
)
fileNamesZtoElecTau_Data2010B_part1061 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_53_1_HHn.root'
)
fileNamesZtoElecTau_Data2010B_part1062 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_54_1_9yu.root'
)
fileNamesZtoElecTau_Data2010B_part1063 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_55_1_5oV.root'
)
fileNamesZtoElecTau_Data2010B_part1064 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_56_1_k4C.root'
)
fileNamesZtoElecTau_Data2010B_part1065 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_57_1_9Ji.root'
)
fileNamesZtoElecTau_Data2010B_part1066 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_58_2_ZEB.root'
)
fileNamesZtoElecTau_Data2010B_part1067 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_59_1_hRF.root'
)
fileNamesZtoElecTau_Data2010B_part1068 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_60_1_bnM.root'
)
fileNamesZtoElecTau_Data2010B_part1069 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_61_1_5lD.root'
)
fileNamesZtoElecTau_Data2010B_part1070 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_62_1_HK6.root'
)
fileNamesZtoElecTau_Data2010B_part1071 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_63_1_bds.root'
)
fileNamesZtoElecTau_Data2010B_part1072 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_64_1_8L2.root'
)
fileNamesZtoElecTau_Data2010B_part1073 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_65_1_Gfp.root'
)
fileNamesZtoElecTau_Data2010B_part1074 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_66_1_qsX.root'
)
fileNamesZtoElecTau_Data2010B_part1075 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_67_1_HrI.root'
)
fileNamesZtoElecTau_Data2010B_part1076 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_68_1_vj6.root'
)
fileNamesZtoElecTau_Data2010B_part1077 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_69_1_EhV.root'
)
fileNamesZtoElecTau_Data2010B_part1078 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_70_1_NcT.root'
)
fileNamesZtoElecTau_Data2010B_part1079 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_71_1_MXk.root'
)
fileNamesZtoElecTau_Data2010B_part1080 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_72_1_NLi.root'
)
fileNamesZtoElecTau_Data2010B_part1081 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_73_1_t0S.root'
)
fileNamesZtoElecTau_Data2010B_part1082 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_74_1_9Uq.root'
)
fileNamesZtoElecTau_Data2010B_part1083 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_75_1_G1L.root'
)
fileNamesZtoElecTau_Data2010B_part1084 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_76_1_mkJ.root'
)
fileNamesZtoElecTau_Data2010B_part1085 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_77_1_s4P.root'
)
fileNamesZtoElecTau_Data2010B_part1086 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_78_1_OCR.root'
)
fileNamesZtoElecTau_Data2010B_part1087 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_79_1_i5w.root'
)
fileNamesZtoElecTau_Data2010B_part1088 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_80_1_9zw.root'
)
fileNamesZtoElecTau_Data2010B_part1089 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_81_1_TIU.root'
)
fileNamesZtoElecTau_Data2010B_part1090 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_82_1_OYg.root'
)
fileNamesZtoElecTau_Data2010B_part1091 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_83_1_XzR.root'
)
fileNamesZtoElecTau_Data2010B_part1092 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_84_1_UuZ.root'
)
fileNamesZtoElecTau_Data2010B_part1093 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_85_1_kto.root'
)
fileNamesZtoElecTau_Data2010B_part1094 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_86_1_6Yu.root'
)
fileNamesZtoElecTau_Data2010B_part1095 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_87_1_pgn.root'
)
fileNamesZtoElecTau_Data2010B_part1096 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_88_1_qxd.root'
)
fileNamesZtoElecTau_Data2010B_part1097 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/data/EPD_PR_part6/skimElecTau_89_1_R8C.root'
)

#--------------------------------------------------------------------------------

##################
#
# 7 TeV MC samples
#
##################



#--------------------------------------------------------------------------------
#
#  Ztautau 
#
intLumiZtoElecTau_Ztautau = float(1582.7)
corrFactorZtoElecTau_Ztautau = float(1.)

patTupleOutputFileNameZtoElecTau_Ztautau = cms.untracked.string('patTupleZtoElecTau_Ztautau_partXX.root')
plotsOutputFileNameZtoElecTau_Ztautau = cms.string('plotsZtoElecTau_Ztautau_partXX.root')
genPhaseSpaceCutZtoElecTau_Ztautau = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

fileNamesZtoElecTau_Ztautau_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/Ztautau/skimElecTau_10_1_rr0.root'
)
fileNamesZtoElecTau_Ztautau_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/Ztautau/skimElecTau_11_1_Lle.root'
)
fileNamesZtoElecTau_Ztautau_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/Ztautau/skimElecTau_12_1_RDs.root'
)
fileNamesZtoElecTau_Ztautau_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/Ztautau/skimElecTau_13_1_geW.root'
)
fileNamesZtoElecTau_Ztautau_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/Ztautau/skimElecTau_14_1_Hox.root'
)
fileNamesZtoElecTau_Ztautau_part06 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/Ztautau/skimElecTau_15_1_jm7.root'
)
fileNamesZtoElecTau_Ztautau_part07 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/Ztautau/skimElecTau_16_1_3po.root'
)
fileNamesZtoElecTau_Ztautau_part08 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/Ztautau/skimElecTau_17_1_mBE.root'
)
fileNamesZtoElecTau_Ztautau_part09 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/Ztautau/skimElecTau_18_1_8Ck.root'
)
fileNamesZtoElecTau_Ztautau_part10 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/Ztautau/skimElecTau_19_1_QmP.root'
)
fileNamesZtoElecTau_Ztautau_part11 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/Ztautau/skimElecTau_1_1_6h9.root'
)
fileNamesZtoElecTau_Ztautau_part12 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/Ztautau/skimElecTau_20_1_Ajq.root'
)
fileNamesZtoElecTau_Ztautau_part13 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/Ztautau/skimElecTau_21_1_jGF.root'
)
fileNamesZtoElecTau_Ztautau_part14 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/Ztautau/skimElecTau_2_1_LLB.root'
)
fileNamesZtoElecTau_Ztautau_part15 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/Ztautau/skimElecTau_3_1_oXg.root'
)
fileNamesZtoElecTau_Ztautau_part16 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/Ztautau/skimElecTau_4_1_pwH.root'
)
fileNamesZtoElecTau_Ztautau_part17 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/Ztautau/skimElecTau_5_1_EgY.root'
)
fileNamesZtoElecTau_Ztautau_part18 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/Ztautau/skimElecTau_6_1_4mf.root'
)
fileNamesZtoElecTau_Ztautau_part19 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/Ztautau/skimElecTau_7_1_Ls5.root'
)
fileNamesZtoElecTau_Ztautau_part20 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/Ztautau/skimElecTau_8_1_tum.root'
)
fileNamesZtoElecTau_Ztautau_part21 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/Ztautau/skimElecTau_9_2_DaL.root'
)

#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# Z --> e+ e- sample generated with Pythia
#
intLumiZtoElecTau_Zee = float(1637)
corrFactorZtoElecTau_Zee = float(1.) 

patTupleOutputFileNameZtoElecTau_Zee = cms.untracked.string('patTupleZtoElecTau_Zee_partXX.root')

plotsOutputFileNameZtoElecTau_Zee = cms.string('plotsZtoElecTau_Zee_partXX.root')

genPhaseSpaceCutZtoElecTau_Zee = cms.PSet(
  pluginName = cms.string('genPhaseSpaceCut'),
  pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

fileNamesZtoElecTau_Zee_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_1_2_XTU.root'
)
fileNamesZtoElecTau_Zee_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_10_5_L6F.root'
)
fileNamesZtoElecTau_Zee_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_11_3_OVx.root'
)
fileNamesZtoElecTau_Zee_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_12_5_QyT.root'
)
fileNamesZtoElecTau_Zee_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_13_2_Cwe.root'
)
fileNamesZtoElecTau_Zee_part06 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_14_2_eTe.root'
)
fileNamesZtoElecTau_Zee_part07 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_15_2_skF.root'
)
fileNamesZtoElecTau_Zee_part08 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_16_3_ENl.root'
)
fileNamesZtoElecTau_Zee_part09 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_17_2_pQW.root'
)
fileNamesZtoElecTau_Zee_part10 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_18_5_2Zm.root'
)
fileNamesZtoElecTau_Zee_part11 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_19_6_HaV.root'
)
fileNamesZtoElecTau_Zee_part12 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_2_2_Ym8.root'
)
fileNamesZtoElecTau_Zee_part13 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_20_3_Kno.root'
)
fileNamesZtoElecTau_Zee_part14 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_21_7_9mm.root'
)
fileNamesZtoElecTau_Zee_part15 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_22_2_95X.root'
)
fileNamesZtoElecTau_Zee_part16 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_23_2_crz.root'
)
fileNamesZtoElecTau_Zee_part17 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_24_5_yOv.root'
)
fileNamesZtoElecTau_Zee_part18 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_25_3_4VQ.root'
)
fileNamesZtoElecTau_Zee_part19 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_26_2_CYb.root'
)
fileNamesZtoElecTau_Zee_part20 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_27_6_ZAq.root'
)
fileNamesZtoElecTau_Zee_part21 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_28_3_M3Z.root'
)
fileNamesZtoElecTau_Zee_part22 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_29_5_6yS.root'
)
fileNamesZtoElecTau_Zee_part23 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_3_3_wzn.root'
)
fileNamesZtoElecTau_Zee_part24 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_30_3_Q6u.root'
)
fileNamesZtoElecTau_Zee_part25 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_32_2_iO2.root'
)
fileNamesZtoElecTau_Zee_part26 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_33_3_NZt.root'
)
fileNamesZtoElecTau_Zee_part27 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_34_3_L7V.root'
)
fileNamesZtoElecTau_Zee_part28 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_35_3_yAU.root'
)
fileNamesZtoElecTau_Zee_part29 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_36_2_AsV.root'
)
fileNamesZtoElecTau_Zee_part30 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_37_3_qDm.root'
)
fileNamesZtoElecTau_Zee_part31 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_38_2_AI3.root'
)
fileNamesZtoElecTau_Zee_part32 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_39_2_faK.root'
)
fileNamesZtoElecTau_Zee_part33 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_4_2_4Jd.root'
)
fileNamesZtoElecTau_Zee_part34 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_40_2_AI1.root'
)
fileNamesZtoElecTau_Zee_part35 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_41_2_zgx.root'
)
fileNamesZtoElecTau_Zee_part36 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_42_2_HxY.root'
)
fileNamesZtoElecTau_Zee_part37 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_43_2_t9G.root'
)
fileNamesZtoElecTau_Zee_part38 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_44_2_cKv.root'
)
fileNamesZtoElecTau_Zee_part39 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_45_2_NGZ.root'
)
fileNamesZtoElecTau_Zee_part40 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_46_2_i91.root'
)
fileNamesZtoElecTau_Zee_part41 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_47_2_R27.root'
)
fileNamesZtoElecTau_Zee_part42 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_48_2_yZI.root'
)
fileNamesZtoElecTau_Zee_part43 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_49_2_Uu8.root'
)
fileNamesZtoElecTau_Zee_part44 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_5_2_aLM.root'
)
fileNamesZtoElecTau_Zee_part45 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_50_2_Ljq.root'
)
fileNamesZtoElecTau_Zee_part46 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_51_2_c9t.root'
)
fileNamesZtoElecTau_Zee_part47 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_52_2_iBt.root'
)
fileNamesZtoElecTau_Zee_part48 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_53_2_rSd.root'
)
fileNamesZtoElecTau_Zee_part49 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_54_2_ZMA.root'
)
fileNamesZtoElecTau_Zee_part50 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_55_2_SYD.root'
)
fileNamesZtoElecTau_Zee_part51 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_56_2_mrW.root'
)
fileNamesZtoElecTau_Zee_part52 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_57_2_vq6.root'
)
fileNamesZtoElecTau_Zee_part53 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_58_2_4Wd.root'
)
fileNamesZtoElecTau_Zee_part54 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_59_2_jXB.root'
)
fileNamesZtoElecTau_Zee_part55 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_6_2_L6r.root'
)
fileNamesZtoElecTau_Zee_part56 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_60_2_qBd.root'
)
fileNamesZtoElecTau_Zee_part57 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_61_2_Bcx.root'
)
fileNamesZtoElecTau_Zee_part58 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_62_2_6Kc.root'
)
fileNamesZtoElecTau_Zee_part59 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_63_2_3jz.root'
)
fileNamesZtoElecTau_Zee_part60 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_64_3_fdf.root'
)
fileNamesZtoElecTau_Zee_part61 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_65_2_UO9.root'
)
fileNamesZtoElecTau_Zee_part62 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_66_2_IYy.root'
)
fileNamesZtoElecTau_Zee_part63 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_67_2_QcP.root'
)
fileNamesZtoElecTau_Zee_part64 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_68_2_1R1.root'
)
fileNamesZtoElecTau_Zee_part65 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_69_2_5Mj.root'
)
fileNamesZtoElecTau_Zee_part66 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_7_2_yMO.root'
)
fileNamesZtoElecTau_Zee_part67 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_70_2_o54.root'
)
fileNamesZtoElecTau_Zee_part68 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_71_2_biu.root'
)
fileNamesZtoElecTau_Zee_part69 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_72_2_w60.root'
)
fileNamesZtoElecTau_Zee_part70 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_73_2_HyI.root'
)
fileNamesZtoElecTau_Zee_part71 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_74_2_DYD.root'
)
fileNamesZtoElecTau_Zee_part72 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_75_2_NqJ.root'
)
fileNamesZtoElecTau_Zee_part73 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_76_2_SVw.root'
)
fileNamesZtoElecTau_Zee_part74 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_77_2_MbW.root'
)
fileNamesZtoElecTau_Zee_part75 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_78_2_Eu3.root'
)
fileNamesZtoElecTau_Zee_part76 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_79_2_VI3.root'
)
fileNamesZtoElecTau_Zee_part77 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_8_6_q3P.root'
)
fileNamesZtoElecTau_Zee_part78 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_80_2_uy9.root'
)
fileNamesZtoElecTau_Zee_part79 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_81_2_sOH.root'
)
fileNamesZtoElecTau_Zee_part80 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_9_2_zt8.root'
)
fileNamesZtoElecTau_Zee_part81 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//Zee/skimElecTau_31_7_p75.root'
)



#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# EM enriched QCD sample generated with Pythia (20 GeV < Pt(hat) < 30 GeV)
#
intLumiZtoElecTau_QCD_EM_Pt20to30 = float(15.14)
corrFactorZtoElecTau_QCD_EM_Pt20to30 = float(1.) 

patTupleOutputFileNameZtoElecTau_QCD_EM_Pt20to30 = cms.untracked.string('patTupleZtoElecTau_QCD_EM_Pt20to30_partXX.root')

plotsOutputFileNameZtoElecTau_QCD_EM_Pt20to30 = cms.string('plotsZtoElecTau_QCD_EM_Pt20to30_partXX.root')

genPhaseSpaceCutZtoElecTau_QCD_EM_Pt20to30 = cms.PSet(
  pluginName = cms.string('genPhaseSpaceCut'),
  pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

fileNamesZtoElecTau_QCD_EM_Pt20to30_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_1_1_nrH.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_10_1_vO9.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_100_1_46Z.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_101_1_WJK.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_102_1_Ou0.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part06 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_103_1_jNi.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part07 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_104_1_S2w.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part08 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_105_1_PcD.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part09 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_106_1_ggP.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part10 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_107_1_MrE.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part11 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_108_1_vW4.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part12 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_109_1_SOA.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part13 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_11_2_V9h.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part14 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_110_1_RlW.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part15 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_111_1_aV6.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part16 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_112_1_DGX.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part17 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_113_3_Uv9.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part18 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_114_1_qmg.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part19 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_115_1_EIZ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part20 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_116_1_CGG.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part21 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_117_1_4Wr.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part22 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_118_1_lm5.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part23 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_119_2_IBy.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part24 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_12_1_DhT.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part25 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_120_1_a5g.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part26 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_121_1_Pir.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part27 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_122_1_8Qs.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part28 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_123_1_5KI.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part29 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_124_1_hT8.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part30 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_125_1_1s6.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part31 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_126_1_Tmo.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part32 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_127_1_YfJ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part33 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_128_1_jar.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part34 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_129_3_coN.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part35 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_13_1_kFZ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part36 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_130_1_Hqh.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part37 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_131_1_pZN.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part38 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_132_1_mNR.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part39 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_133_2_eHS.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part40 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_134_2_vnT.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part41 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_135_1_E39.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part42 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_136_1_9kT.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part43 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_137_1_6Ym.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part44 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_138_2_try.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part45 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_139_1_ZMn.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part46 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_14_1_3Sg.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part47 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_140_1_8ax.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part48 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_141_1_M0p.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part49 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_142_2_TRj.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part50 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_143_1_V1O.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part51 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_144_1_gKD.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part52 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_145_1_1Vm.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part53 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_146_1_Ow8.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part54 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_147_1_HXG.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part55 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_148_1_eC1.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part56 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_149_2_1fV.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part57 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_15_1_BkQ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part58 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_150_1_uLp.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part59 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_151_1_zpy.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part60 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_152_1_0jC.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part61 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_153_2_dwT.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part62 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_154_1_YhK.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part63 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_155_1_utX.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part64 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_156_1_Wkp.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part65 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_157_2_wAs.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part66 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_158_1_5JA.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part67 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_159_1_B7M.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part68 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_16_1_MZr.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part69 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_160_1_UTJ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part70 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_161_1_qPu.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part71 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_162_1_vpy.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part72 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_163_1_iNx.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part73 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_164_1_5qU.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part74 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_165_2_Bb6.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part75 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_166_3_ggH.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part76 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_167_2_VdZ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part77 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_168_1_lZq.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part78 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_169_3_XP5.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part79 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_17_1_2Nc.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part80 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_170_1_IRd.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part81 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_171_1_exn.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part82 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_172_1_PU6.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part83 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_173_3_OPr.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part84 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_174_1_Gdg.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part85 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_175_1_fbY.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part86 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_176_2_DWJ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part87 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_177_1_laX.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part88 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_178_1_FOx.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part89 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_179_1_rCl.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part90 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_18_1_XpM.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part91 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_180_1_1hD.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part92 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_181_1_yGc.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part93 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_182_1_ill.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part94 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_183_1_EHX.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part95 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_184_1_U8L.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part96 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_185_1_EDB.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part97 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_186_1_qu1.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part98 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_187_1_xqs.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part99 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_188_2_oE9.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part100 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_189_1_Rwq.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part101 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_19_1_8nQ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part102 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_190_1_myN.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part103 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_191_3_Pqc.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part104 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_192_1_m6a.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part105 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_193_1_JJj.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part106 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_194_1_XPM.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part107 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_195_3_mhl.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part108 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_196_1_O7X.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part109 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_197_1_NcP.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part110 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_198_1_gfz.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part111 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_199_3_9aQ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part112 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_2_1_0K6.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part113 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_20_1_rs5.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part114 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_200_3_laS.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part115 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_201_2_z4v.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part116 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_202_1_5rL.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part117 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_203_1_vRu.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part118 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_204_2_Yqh.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part119 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_205_1_COJ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part120 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_206_1_uVL.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part121 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_207_2_MfN.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part122 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_208_1_Pwc.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part123 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_209_1_oCN.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part124 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_21_1_Tfl.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part125 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_210_2_Sby.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part126 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_211_1_HRW.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part127 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_212_1_Prk.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part128 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_213_1_KFt.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part129 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_214_1_AUa.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part130 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_215_2_CNj.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part131 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_216_1_626.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part132 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_217_1_cMa.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part133 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_218_1_aw6.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part134 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_219_2_Lgu.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part135 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_22_1_dM2.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part136 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_220_1_PCy.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part137 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_221_3_gWm.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part138 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_222_1_Wnl.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part139 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_223_1_4XW.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part140 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_224_1_wvL.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part141 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_225_1_5Jh.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part142 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_226_1_zRn.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part143 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_23_1_yPP.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part144 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_24_1_FiX.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part145 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_25_1_HxA.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part146 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_26_1_qIh.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part147 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_27_1_PlB.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part148 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_28_1_jUO.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part149 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_29_2_wg4.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part150 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_3_1_Vto.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part151 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_30_1_Cw1.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part152 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_31_1_eat.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part153 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_32_3_T1T.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part154 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_33_1_5Mx.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part155 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_34_2_87x.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part156 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_35_2_IjF.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part157 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_36_1_2KR.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part158 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_37_2_1Ja.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part159 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_38_2_gEs.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part160 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_39_1_Lx0.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part161 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_4_1_6PH.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part162 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_40_1_ODN.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part163 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_41_2_Okd.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part164 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_42_1_jXn.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part165 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_43_1_sFm.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part166 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_44_1_Xfo.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part167 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_45_1_RFQ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part168 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_46_1_49c.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part169 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_47_1_HYy.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part170 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_48_1_x3f.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part171 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_49_1_F7M.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part172 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_5_1_LwP.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part173 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_50_3_nyq.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part174 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_51_3_4bM.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part175 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_52_1_0N0.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part176 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_53_1_I1A.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part177 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_54_1_QEA.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part178 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_55_3_zfX.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part179 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_56_3_iVg.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part180 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_57_2_f2U.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part181 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_58_1_DY3.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part182 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_59_1_DAq.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part183 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_6_1_W67.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part184 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_60_1_n6P.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part185 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_61_1_NNV.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part186 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_62_1_Xwj.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part187 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_63_3_eTA.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part188 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_64_1_0qm.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part189 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_65_1_x5V.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part190 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_66_1_rzw.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part191 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_67_1_2bR.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part192 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_68_1_0ML.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part193 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_69_1_vKf.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part194 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_7_1_KHn.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part195 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_70_1_fpU.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part196 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_71_1_KZK.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part197 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_72_1_teu.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part198 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_73_1_DQ6.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part199 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_74_1_JOw.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part200 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_75_1_Hv3.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part201 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_76_1_mQ9.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part202 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_77_1_CKl.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part203 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_78_1_Lo3.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part204 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_79_1_01I.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part205 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_8_1_eNW.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part206 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_80_2_QxZ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part207 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_81_1_Z08.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part208 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_82_2_Ib7.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part209 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_83_2_m9n.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part210 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_84_1_0BJ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part211 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_85_1_5OG.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part212 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_86_1_1w5.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part213 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_87_1_oso.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part214 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_88_2_Sm8.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part215 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_89_1_1ZQ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part216 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_9_1_qPr.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part217 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_90_1_9SL.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part218 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_91_1_vlK.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part219 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_92_1_dS7.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part220 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_93_1_rlu.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part221 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_94_1_TmK.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part222 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_95_1_eAy.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part223 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_96_1_op9.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part224 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_97_1_iAg.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part225 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_98_1_Fx0.root'
)
fileNamesZtoElecTau_QCD_EM_Pt20to30_part226 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt20to30/skimElecTau_99_1_CcN.root'
)

#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# EM enriched QCD sample generated with Pythia (30 GeV < Pt(hat) < 80 GeV)
#
intLumiZtoElecTau_QCD_EM_Pt30to80 = float(18.58)
corrFactorZtoElecTau_QCD_EM_Pt30to80 = float(1.055)

patTupleOutputFileNameZtoElecTau_QCD_EM_Pt30to80 = cms.untracked.string('patTupleZtoElecTau_QCD_EM_Pt30to80_partXX.root')

plotsOutputFileNameZtoElecTau_QCD_EM_Pt30to80 = cms.string('plotsZtoElecTau_QCD_EM_Pt30to80_partXX.root')

genPhaseSpaceCutZtoElecTau_QCD_EM_Pt30to80 = cms.PSet(
  pluginName = cms.string('genPhaseSpaceCut'),
  pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

fileNamesZtoElecTau_QCD_EM_Pt30to80_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_1_1_rUc.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_10_2_oQe.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_100_5_LmB.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_101_4_Ie9.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_102_5_6H4.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part06 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_103_4_FjD.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part07 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_104_5_yml.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part08 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_105_4_JaK.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part09 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_106_2_knn.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part10 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_107_2_TBN.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part11 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_108_5_x7c.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part12 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_109_4_QFL.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part13 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_11_3_MHQ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part14 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_110_4_6OI.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part15 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_111_2_UrC.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part16 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_112_2_PDT.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part17 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_113_3_Y0q.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part18 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_114_2_ThB.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part19 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_115_2_wTj.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part20 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_116_4_IBH.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part21 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_117_4_XBe.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part22 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_118_4_jsg.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part23 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_119_4_FOQ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part24 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_12_4_8b9.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part25 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_120_4_Yz8.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part26 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_121_3_yTh.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part27 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_122_4_s3z.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part28 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_123_4_yi0.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part29 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_124_4_y1A.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part30 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_125_2_WY8.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part31 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_126_2_5Dg.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part32 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_127_2_WaJ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part33 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_128_4_uCf.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part34 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_129_4_sHe.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part35 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_13_5_bcV.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part36 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_130_2_aRT.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part37 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_131_5_fhk.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part38 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_132_4_F79.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part39 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_133_2_a1t.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part40 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_134_3_ypK.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part41 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_135_5_BXv.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part42 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_136_4_0Mn.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part43 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_137_1_u4h.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part44 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_138_2_igl.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part45 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_139_4_QyR.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part46 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_14_2_PIS.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part47 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_140_2_ylK.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part48 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_141_5_cCI.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part49 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_142_5_BRZ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part50 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_143_4_xqU.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part51 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_144_3_qsb.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part52 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_145_2_qhf.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part53 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_146_4_6yZ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part54 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_147_2_90u.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part55 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_148_3_P0B.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part56 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_149_2_d6q.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part57 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_15_2_P1n.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part58 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_150_5_5pk.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part59 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_151_5_97p.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part60 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_152_1_Alw.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part61 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_153_4_hxH.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part62 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_154_4_T5f.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part63 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_155_3_ZgH.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part64 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_156_2_GYY.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part65 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_157_4_4hc.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part66 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_158_4_wmN.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part67 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_159_4_9fj.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part68 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_16_1_tHd.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part69 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_160_4_J9c.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part70 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_161_5_kZ8.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part71 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_162_3_kTp.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part72 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_163_2_pt3.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part73 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_164_2_lvP.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part74 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_165_4_tEv.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part75 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_166_2_lkm.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part76 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_167_2_aPO.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part77 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_168_4_KZo.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part78 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_169_3_g09.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part79 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_17_5_XuR.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part80 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_170_3_aMF.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part81 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_171_2_4UP.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part82 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_172_5_fLJ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part83 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_173_4_AVC.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part84 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_174_4_enM.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part85 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_175_3_shl.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part86 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_176_3_6Wf.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part87 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_177_3_J0t.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part88 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_178_4_6nV.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part89 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_179_3_EDF.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part90 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_18_3_cD2.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part91 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_180_2_oum.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part92 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_181_4_Vbi.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part93 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_182_1_RpL.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part94 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_183_2_1Um.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part95 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_184_4_1Zr.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part96 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_185_4_37r.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part97 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_186_4_SpK.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part98 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_187_5_SOc.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part99 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_188_4_fx0.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part100 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_189_3_4JS.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part101 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_19_4_Zh2.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part102 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_190_4_akq.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part103 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_191_5_BZR.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part104 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_192_4_kgG.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part105 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_193_2_Raz.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part106 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_194_4_Hzi.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part107 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_195_4_pAf.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part108 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_196_3_2sW.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part109 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_197_3_w03.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part110 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_198_2_4O9.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part111 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_199_4_yqo.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part112 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_2_2_IbX.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part113 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_20_3_lcY.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part114 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_200_2_Jjb.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part115 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_201_3_jJJ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part116 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_202_2_aLa.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part117 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_203_4_GOb.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part118 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_204_4_Vjk.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part119 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_205_4_Lft.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part120 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_206_4_7cB.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part121 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_207_5_s2P.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part122 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_208_4_sem.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part123 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_209_1_rZw.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part124 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_21_4_G7R.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part125 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_210_4_9ov.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part126 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_211_4_8WR.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part127 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_212_5_any.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part128 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_213_4_VcV.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part129 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_214_3_wcO.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part130 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_215_4_pRC.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part131 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_216_2_KwR.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part132 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_217_2_oDr.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part133 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_218_2_76d.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part134 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_219_4_gTG.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part135 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_22_5_OvQ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part136 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_220_2_f4a.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part137 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_221_5_FjU.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part138 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_222_2_2v7.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part139 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_223_2_if6.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part140 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_224_1_qEZ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part141 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_225_4_hWS.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part142 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_226_2_4F3.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part143 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_227_5_ny2.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part144 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_228_2_vl7.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part145 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_229_2_xTe.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part146 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_23_4_g1B.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part147 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_230_4_ztG.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part148 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_231_4_LnO.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part149 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_232_2_BIf.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part150 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_233_4_Ybg.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part151 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_234_4_9Zs.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part152 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_235_4_Kmr.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part153 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_236_2_QSe.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part154 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_237_4_27n.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part155 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_238_2_jAz.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part156 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_239_1_FgJ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part157 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_24_4_QOK.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part158 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_240_4_0dP.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part159 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_241_4_1f2.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part160 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_242_2_mIS.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part161 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_243_2_hNq.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part162 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_244_4_HKZ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part163 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_245_2_AGd.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part164 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_246_4_eLA.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part165 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_247_4_gz8.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part166 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_248_3_DjU.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part167 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_249_2_CRA.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part168 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_25_5_G3W.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part169 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_250_2_7d2.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part170 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_251_4_tvl.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part171 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_252_4_krC.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part172 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_253_2_HNS.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part173 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_254_1_Cmn.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part174 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_255_2_0vq.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part175 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_256_4_2ic.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part176 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_257_2_xqG.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part177 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_258_2_7H7.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part178 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_259_4_e2m.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part179 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_26_2_rGE.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part180 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_260_5_665.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part181 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_261_2_p2l.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part182 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_262_2_GgN.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part183 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_263_4_Wl3.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part184 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_264_2_tJF.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part185 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_265_4_rym.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part186 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_266_4_EkD.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part187 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_267_2_Ktk.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part188 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_268_4_9bQ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part189 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_269_2_5L4.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part190 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_27_4_ZiD.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part191 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_270_4_mFY.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part192 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_271_3_hNr.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part193 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_272_2_dQE.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part194 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_273_4_CQD.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part195 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_274_5_tGJ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part196 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_275_4_T7s.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part197 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_276_2_TTl.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part198 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_277_4_dzR.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part199 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_278_2_lqr.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part200 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_279_4_tdm.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part201 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_28_4_nuL.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part202 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_280_4_kfk.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part203 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_281_2_tzn.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part204 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_282_2_sBe.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part205 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_283_4_8i3.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part206 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_284_2_fyW.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part207 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_285_4_xmp.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part208 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_286_4_56R.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part209 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_287_5_5xa.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part210 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_288_2_VUV.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part211 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_289_4_eQ4.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part212 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_29_5_GBV.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part213 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_290_5_ymS.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part214 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_291_3_Axw.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part215 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_292_3_lNa.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part216 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_293_4_prS.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part217 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_294_6_kkx.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part218 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_295_4_mVZ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part219 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_296_4_ULY.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part220 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_297_3_XKD.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part221 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_298_4_tUI.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part222 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_299_1_ZPW.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part223 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_3_2_SvX.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part224 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_30_4_Wbn.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part225 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_300_5_DzX.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part226 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_301_3_dFX.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part227 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_302_3_tVy.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part228 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_303_4_KsG.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part229 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_304_4_VsT.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part230 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_305_4_NYg.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part231 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_306_4_9j8.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part232 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_307_2_u2S.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part233 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_308_2_Yxd.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part234 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_309_4_fHw.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part235 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_31_1_eyJ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part236 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_310_5_mPi.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part237 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_311_4_PVM.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part238 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_312_4_0xL.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part239 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_313_4_3dH.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part240 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_314_3_j7v.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part241 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_315_3_UKm.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part242 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_316_3_Kdr.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part243 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_317_2_uNH.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part244 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_318_4_RQW.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part245 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_319_4_Ilk.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part246 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_32_3_gtQ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part247 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_320_2_Di7.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part248 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_321_2_Lif.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part249 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_322_2_4qv.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part250 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_323_2_wk8.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part251 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_324_4_FmX.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part252 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_325_4_R4E.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part253 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_326_5_4tk.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part254 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_327_4_O7t.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part255 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_328_2_zbt.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part256 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_329_2_R9y.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part257 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_33_2_l5b.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part258 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_330_5_pqF.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part259 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_331_2_MWC.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part260 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_332_2_vFO.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part261 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_333_2_isZ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part262 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_334_4_7bB.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part263 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_335_2_x1Z.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part264 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_336_2_1jD.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part265 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_337_4_nVE.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part266 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_338_2_WW8.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part267 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_339_4_9Tk.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part268 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_34_3_Tj7.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part269 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_340_4_V47.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part270 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_341_4_GPQ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part271 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_342_4_NWv.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part272 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_343_2_ReL.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part273 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_344_3_XfX.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part274 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_345_4_DYy.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part275 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_346_4_XgQ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part276 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_347_2_isq.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part277 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_348_4_pC9.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part278 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_349_2_lFA.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part279 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_35_4_An1.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part280 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_350_2_gdz.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part281 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_351_3_DT1.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part282 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_352_2_hjZ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part283 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_353_5_lgV.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part284 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_354_4_0XN.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part285 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_355_4_CcM.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part286 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_356_4_XqG.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part287 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_357_4_3kj.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part288 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_358_2_jhr.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part289 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_359_3_UA7.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part290 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_36_2_eON.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part291 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_360_4_Xcj.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part292 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_361_7_kqj.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part293 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_362_3_pGH.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part294 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_363_4_aU3.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part295 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_364_3_Bro.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part296 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_365_2_CnS.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part297 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_366_2_s8Y.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part298 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_367_4_gzE.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part299 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_368_4_oKH.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part300 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_369_4_vfD.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part301 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_37_2_bOw.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part302 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_370_4_RFw.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part303 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_371_4_a0L.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part304 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_372_4_HZT.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part305 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_373_5_JYk.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part306 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_374_3_PLY.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part307 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_375_5_0mM.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part308 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_376_4_rBu.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part309 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_377_5_7wZ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part310 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_378_4_gR3.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part311 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_379_4_o2t.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part312 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_38_4_Mi6.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part313 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_380_4_Bcl.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part314 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_381_2_2Ga.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part315 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_382_2_C6x.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part316 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_383_4_ADM.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part317 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_384_5_XQr.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part318 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_385_4_k8L.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part319 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_386_2_6G5.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part320 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_387_4_Q8E.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part321 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_388_2_2yc.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part322 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_389_1_nxj.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part323 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_39_4_ybn.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part324 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_390_5_gZv.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part325 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_391_4_Npj.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part326 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_392_3_sIw.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part327 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_393_2_Oc9.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part328 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_394_4_cZC.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part329 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_395_4_xyg.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part330 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_396_4_28e.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part331 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_397_5_tCR.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part332 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_398_2_SHO.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part333 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_399_3_dKB.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part334 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_4_4_Tsu.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part335 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_40_3_tmP.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part336 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_400_5_StD.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part337 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_401_5_hdB.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part338 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_402_2_viG.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part339 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_403_3_ORa.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part340 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_404_1_0Xv.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part341 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_405_4_5qv.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part342 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_406_4_Dcl.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part343 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_407_4_KAj.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part344 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_408_2_pHy.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part345 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_409_2_rB6.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part346 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_41_5_s5m.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part347 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_410_2_cRW.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part348 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_411_2_bCo.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part349 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_412_2_e5O.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part350 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_413_4_8IC.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part351 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_414_4_SLy.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part352 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_415_3_nRm.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part353 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_416_2_3Ly.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part354 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_417_2_Pzi.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part355 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_418_4_uxF.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part356 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_419_1_1QD.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part357 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_42_3_0HU.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part358 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_420_4_dTK.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part359 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_421_5_2M2.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part360 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_422_4_DYP.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part361 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_423_3_WjX.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part362 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_424_2_idJ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part363 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_425_4_ujM.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part364 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_426_4_eYc.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part365 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_427_4_muN.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part366 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_428_4_uzj.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part367 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_429_4_dX4.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part368 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_43_4_TrW.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part369 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_430_3_8IH.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part370 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_431_3_bCF.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part371 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_432_4_BhJ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part372 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_433_2_QPF.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part373 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_434_2_198.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part374 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_435_5_Gjo.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part375 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_436_4_I8e.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part376 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_437_3_hNI.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part377 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_438_4_mFo.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part378 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_439_2_Q15.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part379 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_44_5_nMw.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part380 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_440_4_noA.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part381 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_441_3_2jb.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part382 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_442_4_p8E.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part383 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_443_2_ppq.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part384 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_444_3_H6m.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part385 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_445_2_RR0.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part386 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_446_4_91F.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part387 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_447_2_MI5.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part388 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_448_2_LWi.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part389 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_449_1_wM1.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part390 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_45_4_YHb.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part391 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_450_4_p4l.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part392 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_451_3_Mcl.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part393 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_452_2_ydB.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part394 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_453_2_XSt.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part395 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_454_4_9wo.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part396 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_455_4_N4q.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part397 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_456_4_wFD.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part398 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_457_3_QaT.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part399 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_458_3_Jgg.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part400 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_459_4_TLn.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part401 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_46_2_8Cp.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part402 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_460_4_o8C.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part403 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_461_4_BWs.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part404 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_462_2_HDB.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part405 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_463_4_H6q.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part406 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_464_2_5k3.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part407 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_465_2_AK8.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part408 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_466_5_F8j.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part409 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_467_4_WBM.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part410 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_468_7_iFt.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part411 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_469_4_rLO.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part412 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_47_3_Vtx.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part413 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_470_2_Uqj.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part414 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_471_4_y1c.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part415 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_472_2_RyC.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part416 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_473_5_CWc.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part417 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_474_4_fpo.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part418 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_475_2_eeW.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part419 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_476_4_aFY.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part420 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_477_5_J8U.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part421 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_478_2_IV2.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part422 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_479_1_FlG.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part423 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_48_4_xzj.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part424 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_49_2_Yik.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part425 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_5_2_AFF.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part426 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_50_2_UCF.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part427 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_51_4_C9U.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part428 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_52_4_eH5.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part429 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_53_4_Gz1.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part430 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_54_2_KV5.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part431 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_55_2_I2t.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part432 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_56_5_XnF.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part433 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_57_2_c1U.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part434 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_58_4_EhB.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part435 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_59_3_VFB.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part436 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_6_2_5qW.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part437 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_60_3_pew.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part438 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_61_2_6Xh.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part439 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_62_5_vFn.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part440 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_63_4_0jk.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part441 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_64_4_9Xe.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part442 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_65_3_4XJ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part443 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_66_2_auW.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part444 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_67_6_5aE.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part445 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_68_4_vHw.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part446 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_69_4_VFb.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part447 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_7_2_qlo.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part448 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_70_4_5OY.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part449 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_71_4_kd5.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part450 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_72_4_cwK.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part451 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_73_2_3Dv.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part452 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_74_4_JzQ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part453 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_75_4_uMb.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part454 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_76_3_4Rh.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part455 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_77_1_zOE.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part456 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_78_4_dGs.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part457 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_79_2_gQf.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part458 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_8_3_ZdZ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part459 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_80_5_uJw.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part460 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_81_3_o5J.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part461 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_82_5_iMg.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part462 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_83_2_2kp.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part463 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_84_4_gpC.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part464 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_85_5_zFL.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part465 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_86_2_kkm.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part466 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_87_2_CPg.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part467 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_88_4_74b.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part468 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_89_4_f4F.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part469 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_9_5_ACI.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part470 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_90_2_b6V.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part471 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_91_2_Bng.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part472 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_92_1_ZKB.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part473 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_93_4_yDU.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part474 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_94_4_xYT.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part475 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_95_2_Aiw.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part476 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_96_2_AGA.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part477 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_97_4_CO8.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part478 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_98_6_LH1.root'
)
fileNamesZtoElecTau_QCD_EM_Pt30to80_part479 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_3/skimElecTau_99_2_xvv.root'
)

#  old samples

#fileNamesZtoElecTau_QCD_EM_Pt30to80_part01 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1_1_U5M.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part02 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_10_1_2RY.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part03 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_100_1_t4r.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part04 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1000_3_zel.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part05 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1001_2_OWt.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part06 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1002_2_pgF.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part07 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1005_2_QRG.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part08 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1006_2_zEr.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part09 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1008_3_tCs.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part10 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1009_2_ZYt.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part11 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_101_2_nV0.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part12 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1010_3_wtZ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part13 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1011_2_fkA.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part14 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1014_2_ZtT.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part15 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1015_2_Fk0.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part16 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1016_4_BQi.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part17 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1017_2_yXw.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part18 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1018_3_JqQ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part19 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1019_2_0VY.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part20 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_102_1_dYY.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part21 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1020_3_lJ9.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part22 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1021_3_lol.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part23 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1022_2_JY7.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part24 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1024_2_e7K.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part25 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1027_3_xUm.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part26 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1028_2_IqG.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part27 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1029_3_3Gg.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part28 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_103_2_wo8.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part29 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1031_2_kSG.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part30 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1032_2_AXe.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part31 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1034_3_jXP.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part32 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1035_2_onT.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part33 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1037_2_D61.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part34 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1038_2_aVc.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part35 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1039_2_tqu.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part36 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_104_2_XrI.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part37 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1041_2_43H.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part38 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1042_2_FOM.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part39 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1043_3_X6i.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part40 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1044_3_kzy.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part41 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1045_2_Ul8.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part42 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1047_2_CFM.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part43 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1048_2_baS.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part44 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_105_1_vTJ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part45 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1050_2_pmT.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part46 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1051_2_fv2.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part47 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1052_2_80w.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part48 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1054_2_q3G.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part49 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1055_2_Pf6.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part50 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1057_2_nXc.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part51 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1058_2_Oio.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part52 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_106_1_0Q0.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part53 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1060_2_dud.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part54 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1063_2_CJ7.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part55 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1064_2_ubZ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part56 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1065_4_4IM.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part57 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1066_2_RwI.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part58 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1067_3_MrW.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part59 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1068_3_R29.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part60 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1069_2_Y4Q.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part61 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_107_1_ltE.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part62 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1070_2_94T.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part63 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1071_2_wjp.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part64 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1073_2_yh2.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part65 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1076_3_GNw.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part66 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1077_2_Rhv.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part67 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1078_2_Hyb.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part68 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1079_2_Rsl.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part69 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_108_2_MOR.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part70 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1080_2_ADQ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part71 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1081_2_phx.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part72 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1083_2_DYK.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part73 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1084_1_u5p.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part74 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1085_1_cC5.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part75 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1086_1_nKw.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part76 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1087_1_E27.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part77 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1088_1_F9z.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part78 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1089_1_HVZ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part79 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_109_1_bJd.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part80 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1090_1_vD2.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part81 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1091_2_QJO.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part82 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1093_2_acl.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part83 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1094_1_i3a.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part84 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1095_1_Jmw.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part85 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1096_1_kmK.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part86 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1097_1_n9J.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part87 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1098_1_XcA.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part88 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1099_1_55E.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part89 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_11_2_x5u.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part90 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_110_1_0Qd.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part91 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1100_1_ng5.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part92 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1101_1_16D.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part93 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1102_1_c8T.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part94 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1103_1_COQ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part95 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1104_1_j13.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part96 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1105_1_P8H.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part97 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1106_1_sev.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part98 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1107_1_BpH.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part99 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1108_1_mIw.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part100 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1109_1_XSY.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part101 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_111_1_rdh.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part102 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1111_1_2sc.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part103 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1112_1_sl6.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part104 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1113_1_fmR.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part105 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1114_1_x9O.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part106 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1115_2_mVG.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part107 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1116_1_DMK.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part108 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1117_1_m5U.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part109 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1118_1_7u5.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part110 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1119_1_PyP.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part111 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_112_1_04r.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part112 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1120_1_e4Q.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part113 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1121_1_q2W.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part114 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1122_1_bdD.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part115 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1123_1_IAs.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part116 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1124_1_wxK.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part117 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1126_1_QuS.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part118 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1127_1_Av9.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part119 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1128_1_zGi.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part120 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1129_1_6Ty.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part121 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_113_1_Cyq.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part122 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1130_1_1GQ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part123 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1131_1_P8c.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part124 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1132_1_KJK.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part125 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1133_1_sdm.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part126 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1134_1_UOi.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part127 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1135_1_U9M.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part128 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1136_2_lNQ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part129 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1137_1_Xp9.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part130 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1138_1_wLs.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part131 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1139_1_ZQq.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part132 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_114_2_8fj.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part133 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1140_1_rI3.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part134 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1141_1_mG2.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part135 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1142_1_TKN.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part136 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1143_1_VEe.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part137 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1144_1_9i2.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part138 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1145_1_u37.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part139 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1146_1_PTM.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part140 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1147_1_AeL.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part141 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1148_2_B2B.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part142 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1149_2_2So.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part143 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_115_2_XME.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part144 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1150_1_mpT.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part145 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1152_1_L62.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part146 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1153_1_60q.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part147 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1154_1_3OD.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part148 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1155_1_SDn.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part149 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1156_1_YTt.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part150 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1157_1_XJw.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part151 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1158_1_TwJ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part152 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1159_1_KiM.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part153 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_116_1_1oG.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part154 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1160_1_yVY.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part155 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1161_1_bbG.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part156 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1162_2_Jsf.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part157 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1163_1_Uhw.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part158 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1164_1_ckw.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part159 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1165_1_833.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part160 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1167_1_Z6I.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part161 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1168_1_DCB.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part162 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1169_1_255.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part163 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1170_1_IKv.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part164 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1171_1_jb7.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part165 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1172_1_HCa.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part166 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1174_1_OCn.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part167 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1175_1_4NE.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part168 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1176_1_BOO.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part169 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1177_1_h4g.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part170 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1179_1_AtB.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part171 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_118_1_VBd.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part172 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1180_1_Csb.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part173 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1181_1_w2D.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part174 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1182_1_Gvp.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part175 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1183_1_3av.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part176 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1184_1_sHL.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part177 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1185_1_hW4.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part178 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1187_1_WAN.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part179 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1188_2_Lk4.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part180 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1189_1_XvW.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part181 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_119_1_Gt4.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part182 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1190_1_ADU.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part183 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1191_1_n7J.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part184 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1192_1_M2B.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part185 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1193_1_7U1.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part186 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1194_2_xJ8.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part187 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1195_1_YAx.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part188 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1196_1_MbB.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part189 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1198_1_6ga.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part190 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_12_2_wMn.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part191 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_120_1_Cwb.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part192 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1200_1_fyN.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part193 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1201_2_Xb0.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part194 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1202_2_Zaj.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part195 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1203_4_idF.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part196 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1204_2_mqm.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part197 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1205_2_bdz.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part198 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1206_1_Nab.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part199 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1207_1_2Id.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part200 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1208_2_hK4.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part201 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1209_2_Xbi.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part202 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_121_1_nHb.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part203 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1210_3_OzY.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part204 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1211_1_m9a.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part205 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1212_4_HVe.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part206 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1213_1_evn.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part207 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1214_1_oBq.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part208 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1215_3_9hD.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part209 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1217_6_rjg.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part210 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1218_5_JCi.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part211 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1219_3_OrU.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part212 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_122_1_Bq8.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part213 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1221_2_icW.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part214 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1223_1_JsM.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part215 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1224_2_YF3.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part216 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1225_1_L09.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part217 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1227_2_qHo.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part218 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1228_3_yfk.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part219 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1229_1_9id.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part220 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_123_1_yus.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part221 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1230_1_Tux.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part222 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1231_1_aeH.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part223 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1232_1_ALT.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part224 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1233_4_Ruo.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part225 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1234_1_pAb.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part226 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1235_2_3cd.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part227 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1236_2_Iki.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part228 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1237_2_DZg.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part229 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1238_2_1LG.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part230 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1239_1_5bz.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part231 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_124_1_Ws4.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part232 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1240_2_nuY.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part233 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1241_1_Sne.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part234 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1242_2_zR5.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part235 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1244_2_gvZ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part236 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1245_1_BG6.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part237 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1246_3_yel.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part238 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1247_1_JuT.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part239 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1248_1_YUg.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part240 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1249_1_qLN.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part241 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_125_2_ahn.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part242 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1251_4_V1X.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part243 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1252_4_dgy.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part244 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1253_2_qIW.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part245 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1255_1_Czo.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part246 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1256_2_Nhv.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part247 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1258_1_mWd.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part248 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1259_3_zSl.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part249 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_126_2_SX6.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part250 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1261_1_C7N.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part251 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1262_1_FJ1.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part252 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1263_2_CdP.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part253 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1264_4_QLB.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part254 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1265_1_7li.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part255 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1266_1_If8.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part256 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1267_1_GKc.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part257 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1268_2_TpG.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part258 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1269_1_EL1.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part259 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_127_2_iwO.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part260 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1270_1_60G.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part261 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1271_1_MGl.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part262 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1272_3_Ala.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part263 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1273_1_7qT.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part264 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1274_1_whf.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part265 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1275_3_D8j.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part266 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1276_2_bJR.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part267 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1277_2_vwU.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part268 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1278_1_lVC.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part269 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1279_4_cDO.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part270 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_128_1_r7k.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part271 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1280_2_fTW.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part272 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1281_1_58b.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part273 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1282_4_rcT.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part274 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1283_2_4Td.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part275 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1284_1_dLp.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part276 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1285_4_3RB.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part277 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1286_1_TtP.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part278 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1287_3_IRW.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part279 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1288_1_1fZ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part280 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_129_1_ELu.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part281 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1290_1_gFG.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part282 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1291_5_EaN.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part283 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1292_1_Pfv.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part284 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1293_2_cRk.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part285 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1294_4_b44.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part286 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1295_3_OeO.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part287 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1296_1_efS.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part288 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1297_2_XHn.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part289 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1298_3_p4b.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part290 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1299_2_PIw.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part291 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_13_2_cbS.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part292 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_130_1_kFx.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part293 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1300_4_fxq.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part294 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1301_1_VlU.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part295 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1302_1_pI3.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part296 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1304_3_3iR.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part297 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1305_2_dpV.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part298 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1306_4_Tha.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part299 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1307_2_DmU.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part300 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_1308_1_1xd.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part301 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_131_1_s4E.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part302 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_132_1_elx.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part303 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_133_1_nk6.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part304 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_134_1_pNs.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part305 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_135_1_Fwg.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part306 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_136_1_V6F.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part307 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_137_1_FVV.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part308 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_138_2_3XD.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part309 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_139_1_O35.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part310 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_14_2_B3F.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part311 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_140_2_M1m.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part312 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_141_2_O4B.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part313 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_142_1_tV2.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part314 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_143_1_PtT.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part315 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_144_1_ln5.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part316 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_145_2_nzR.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part317 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_146_2_fdX.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part318 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_147_1_OcK.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part319 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_148_1_jMF.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part320 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_149_2_h2m.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part321 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_15_1_DeN.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part322 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_150_3_YrT.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part323 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_151_1_IKT.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part324 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_152_2_Pqk.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part325 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_153_1_VYg.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part326 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_154_1_XsY.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part327 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_155_1_Vyl.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part328 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_156_1_FOd.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part329 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_157_1_ZIB.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part330 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_158_1_OLu.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part331 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_159_1_STN.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part332 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_16_2_Z6D.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part333 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_160_2_Rhw.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part334 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_161_2_2rd.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part335 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_162_1_UfA.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part336 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_163_2_BAR.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part337 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_164_2_hD8.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part338 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_165_1_cvl.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part339 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_166_2_TSr.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part340 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_167_2_DNC.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part341 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_168_1_lef.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part342 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_169_1_3To.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part343 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_17_2_enP.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part344 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_170_1_RII.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part345 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_171_1_XPh.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part346 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_172_1_3iv.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part347 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_173_1_4jg.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part348 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_174_1_LeK.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part349 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_175_1_Yeh.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part350 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_176_1_LIT.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part351 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_177_1_ttp.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part352 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_178_1_t5K.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part353 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_179_1_S86.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part354 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_18_2_Dyh.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part355 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_180_1_Xtj.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part356 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_181_1_edW.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part357 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_182_2_9Ui.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part358 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_183_1_Hzk.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part359 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_184_2_ade.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part360 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_185_2_7Sq.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part361 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_186_2_kCb.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part362 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_187_1_B5R.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part363 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_188_1_WEV.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part364 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_189_1_TTm.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part365 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_19_1_lUR.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part366 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_190_1_bWv.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part367 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_191_2_Vxj.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part368 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_192_1_6xI.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part369 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_193_1_uxe.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part370 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_194_2_C3l.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part371 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_195_1_mGz.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part372 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_196_1_j9Y.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part373 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_197_3_zMX.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part374 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_198_2_lGZ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part375 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_199_2_wxx.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part376 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_2_1_8hS.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part377 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_20_1_0ll.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part378 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_200_1_sZT.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part379 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_201_4_jFF.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part380 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_202_1_zf3.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part381 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_203_1_qbI.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part382 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_204_3_KR8.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part383 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_205_4_GPT.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part384 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_206_1_vqx.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part385 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_207_1_Io1.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part386 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_208_1_qao.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part387 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_209_1_nRg.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part388 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_21_1_Ecb.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part389 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_210_4_Stx.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part390 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_211_4_cWa.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part391 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_212_3_oon.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part392 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_213_1_ngl.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part393 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_214_2_cPt.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part394 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_215_1_yp8.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part395 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_216_1_V8t.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part396 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_217_4_YnI.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part397 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_218_2_tU1.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part398 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_219_3_Q4l.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part399 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_22_2_aM5.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part400 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_220_1_4fG.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part401 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_221_4_iw8.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part402 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_222_1_Zp4.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part403 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_223_3_fSZ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part404 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_224_3_9mt.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part405 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_225_4_2cO.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part406 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_226_1_msx.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part407 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_227_1_EFJ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part408 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_228_2_9v3.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part409 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_229_1_muf.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part410 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_23_1_26N.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part411 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_230_3_362.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part412 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_231_2_EYz.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part413 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_232_4_OVz.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part414 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_233_4_E6S.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part415 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_234_4_X7L.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part416 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_235_4_Vl0.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part417 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_236_2_guu.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part418 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_237_1_ZhC.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part419 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_238_3_sNY.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part420 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_239_1_DNM.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part421 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_24_1_SRN.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part422 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_240_1_YkS.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part423 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_241_4_Xkv.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part424 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_242_4_p7Y.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part425 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_243_2_G9l.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part426 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_244_3_vy5.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part427 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_245_4_Iap.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part428 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_246_1_asy.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part429 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_247_1_FWY.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part430 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_248_4_ZZV.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part431 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_249_2_Pde.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part432 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_25_2_SLd.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part433 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_250_2_7KO.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part434 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_251_1_JXw.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part435 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_252_1_W29.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part436 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_253_1_8Hp.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part437 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_254_2_z2J.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part438 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_255_1_ywC.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part439 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_256_1_9am.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part440 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_257_1_k0i.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part441 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_258_1_v84.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part442 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_259_1_SiZ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part443 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_26_2_74M.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part444 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_260_1_rup.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part445 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_261_3_oLj.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part446 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_262_4_GZZ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part447 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_263_1_Yzn.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part448 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_264_1_dzq.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part449 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_265_1_927.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part450 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_266_4_kWx.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part451 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_267_2_BAb.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part452 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_268_2_Ekd.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part453 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_269_1_FQ1.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part454 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_27_1_4L5.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part455 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_270_1_pE7.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part456 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_271_1_AWV.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part457 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_272_1_fTJ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part458 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_273_1_g0Q.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part459 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_274_2_5Q6.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part460 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_275_4_zBK.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part461 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_276_1_Y54.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part462 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_277_2_du2.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part463 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_278_2_8iu.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part464 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_279_1_l5D.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part465 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_28_1_g17.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part466 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_280_1_b2R.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part467 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_281_1_hxy.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part468 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_282_1_0rA.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part469 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_283_1_PrZ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part470 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_284_5_XpC.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part471 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_285_4_lNp.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part472 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_286_1_q3F.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part473 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_287_1_af6.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part474 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_288_1_MiG.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part475 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_289_1_RET.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part476 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_29_1_oUA.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part477 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_290_1_CaW.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part478 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_291_3_cJ6.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part479 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_292_1_WMm.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part480 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_293_4_gja.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part481 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_294_1_OPW.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part482 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_296_3_7AS.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part483 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_297_4_iQg.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part484 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_298_1_uvx.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part485 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_299_4_MxO.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part486 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_3_1_ixX.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part487 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_30_1_Obj.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part488 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_300_1_63p.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part489 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_301_1_YyT.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part490 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_302_1_xVR.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part491 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_303_1_s5f.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part492 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_304_1_hOI.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part493 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_305_4_KjZ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part494 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_306_2_dFb.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part495 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_307_2_FRQ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part496 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_308_1_zNd.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part497 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_309_3_ZyM.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part498 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_31_1_bOq.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part499 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_310_3_l14.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part500 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_311_5_2tR.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part501 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_312_1_3Tt.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part502 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_313_4_yNB.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part503 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_314_1_wAn.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part504 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_315_2_CzA.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part505 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_316_4_2u8.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part506 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_317_1_yNO.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part507 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_318_3_pTb.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part508 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_319_2_543.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part509 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_32_1_rqS.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part510 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_320_4_oru.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part511 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_321_3_e77.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part512 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_322_1_MFL.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part513 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_323_4_2T0.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part514 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_324_4_mwA.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part515 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_325_1_qla.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part516 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_326_4_TjB.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part517 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_327_2_1OM.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part518 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_328_2_tzc.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part519 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_329_3_fKs.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part520 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_33_1_3jU.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part521 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_330_2_sKN.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part522 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_331_4_HGn.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part523 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_332_1_Npb.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part524 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_333_2_gHZ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part525 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_334_4_u5F.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part526 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_335_1_sAs.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part527 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_336_4_Jpi.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part528 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_337_2_hVo.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part529 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_338_2_3mf.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part530 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_339_1_gs2.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part531 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_34_2_zIR.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part532 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_340_5_ZTB.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part533 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_341_1_sEY.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part534 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_342_1_6TW.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part535 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_343_5_mA3.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part536 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_344_1_2Mx.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part537 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_345_4_mxX.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part538 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_346_4_T8N.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part539 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_347_4_v89.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part540 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_348_1_BDx.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part541 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_349_2_czB.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part542 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_35_1_991.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part543 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_350_2_QqZ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part544 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_351_4_4hx.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part545 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_352_2_gXW.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part546 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_353_1_4pb.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part547 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_354_2_2jT.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part548 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_356_1_QMz.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part549 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_357_1_IDu.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part550 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_358_1_heR.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part551 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_359_1_68o.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part552 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_36_3_oqL.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part553 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_360_3_bjS.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part554 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_361_3_FRA.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part555 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_362_4_RYo.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part556 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_364_2_aby.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part557 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_365_1_gUT.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part558 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_366_3_Ude.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part559 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_367_2_NBY.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part560 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_368_1_Fy9.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part561 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_369_1_uQl.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part562 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_37_2_hMj.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part563 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_370_2_jt6.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part564 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_371_1_Cw8.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part565 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_372_1_it4.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part566 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_373_1_SUl.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part567 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_374_3_Spc.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part568 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_375_1_uwh.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part569 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_376_1_bRh.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part570 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_377_3_z77.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part571 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_378_4_IJS.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part572 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_379_2_d4g.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part573 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_38_1_dnw.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part574 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_380_1_aXb.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part575 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_381_2_ddv.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part576 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_382_4_dK3.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part577 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_383_2_0aO.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part578 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_384_4_46x.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part579 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_385_1_pqH.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part580 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_386_1_DK6.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part581 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_387_2_i2X.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part582 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_388_1_m46.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part583 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_389_4_X6A.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part584 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_39_1_5gx.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part585 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_390_5_yjz.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part586 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_391_1_0W8.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part587 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_392_1_KzD.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part588 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_393_3_3HX.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part589 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_394_3_LRY.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part590 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_395_1_DOl.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part591 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_396_3_Kyy.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part592 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_397_4_PxJ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part593 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_398_3_Xaf.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part594 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_399_2_Ofv.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part595 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_4_1_6Es.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part596 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_40_1_z3Y.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part597 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_400_4_xhu.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part598 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_401_2_2nD.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part599 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_402_1_VD0.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part600 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_403_1_3iZ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part601 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_404_1_ISb.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part602 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_405_1_Oyd.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part603 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_406_1_b4U.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part604 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_407_1_C7H.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part605 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_408_1_pr7.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part606 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_409_1_PLY.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part607 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_41_1_Ocj.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part608 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_410_1_pHy.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part609 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_411_1_OBL.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part610 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_412_1_lvY.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part611 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_413_1_3xp.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part612 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_414_1_ETV.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part613 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_415_2_fHI.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part614 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_416_1_S16.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part615 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_417_1_noV.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part616 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_418_2_06d.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part617 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_419_1_Peg.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part618 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_42_1_tOF.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part619 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_420_1_mNz.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part620 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_421_1_n1R.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part621 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_422_1_4ku.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part622 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_423_1_Zjz.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part623 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_424_1_VBX.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part624 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_425_1_WlH.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part625 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_426_1_pZC.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part626 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_427_1_mQ8.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part627 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_428_1_muL.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part628 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_429_1_rJd.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part629 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_43_2_1yj.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part630 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_430_1_JJw.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part631 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_431_1_h8H.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part632 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_432_1_VDd.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part633 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_433_1_x06.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part634 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_434_2_fiw.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part635 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_435_1_dNl.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part636 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_436_1_K3m.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part637 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_437_1_uE6.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part638 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_438_2_0rW.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part639 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_439_1_Tjm.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part640 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_44_2_6gd.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part641 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_440_1_jDz.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part642 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_441_1_VO4.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part643 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_442_1_PSd.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part644 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_443_1_94f.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part645 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_444_2_RRY.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part646 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_445_1_HXT.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part647 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_446_2_Yc1.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part648 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_447_1_hEj.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part649 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_448_1_y4J.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part650 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_449_1_EVB.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part651 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_45_1_pjZ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part652 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_450_1_P9p.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part653 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_451_1_o1j.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part654 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_452_1_DFy.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part655 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_453_1_KGU.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part656 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_454_1_HVw.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part657 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_455_1_oZN.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part658 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_456_1_SA9.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part659 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_457_1_qc2.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part660 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_458_1_Qic.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part661 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_459_2_qgb.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part662 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_46_1_r0B.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part663 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_460_1_LRr.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part664 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_461_1_Pmh.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part665 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_462_2_NCn.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part666 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_463_1_Ymu.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part667 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_464_2_22m.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part668 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_465_2_Bro.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part669 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_466_1_7tn.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part670 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_467_1_IiF.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part671 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_468_1_QN1.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part672 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_469_1_ntt.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part673 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_47_1_rrI.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part674 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_470_1_XPs.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part675 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_471_2_ZnJ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part676 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_472_3_6Ub.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part677 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_473_1_AkC.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part678 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_474_1_sDN.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part679 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_475_2_epz.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part680 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_476_1_yrw.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part681 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_477_1_qID.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part682 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_478_1_AW1.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part683 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_479_2_CV3.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part684 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_48_1_xzs.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part685 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_480_2_s0I.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part686 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_481_1_Vgu.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part687 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_482_1_bj3.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part688 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_483_2_hPn.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part689 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_484_1_eLX.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part690 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_485_1_tI5.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part691 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_486_1_R4s.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part692 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_487_2_IVU.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part693 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_488_1_NKr.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part694 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_489_1_eRJ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part695 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_49_1_jvJ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part696 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_490_2_YUm.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part697 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_491_1_hIL.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part698 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_492_2_71n.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part699 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_493_1_qRl.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part700 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_494_1_oHt.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part701 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_495_1_cPZ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part702 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_496_1_xQ5.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part703 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_497_1_8rj.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part704 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_498_1_Yiq.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part705 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_499_2_Xiz.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part706 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_5_1_gJ5.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part707 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_50_1_JLR.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part708 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_500_1_3SO.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part709 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_501_1_Klq.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part710 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_502_1_BpT.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part711 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_503_1_oCR.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part712 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_504_1_Jdv.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part713 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_505_2_EAC.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part714 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_506_1_Wxp.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part715 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_507_1_WVC.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part716 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_508_1_DUH.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part717 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_509_1_rc7.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part718 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_51_1_IPC.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part719 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_510_1_wLr.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part720 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_511_1_JbG.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part721 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_512_2_rMz.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part722 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_513_1_eWP.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part723 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_514_1_NBW.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part724 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_515_1_dPg.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part725 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_516_1_6Ow.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part726 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_517_1_QWe.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part727 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_518_1_z3r.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part728 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_519_1_1zy.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part729 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_52_1_aPG.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part730 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_520_2_GRK.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part731 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_521_3_jpa.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part732 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_522_1_mkw.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part733 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_523_1_SOW.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part734 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_524_1_KXn.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part735 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_525_2_ERT.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part736 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_526_1_SOz.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part737 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_527_2_Uzk.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part738 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_528_1_PNw.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part739 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_529_1_LLJ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part740 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_53_1_Uj3.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part741 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_530_1_KSF.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part742 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_531_1_4fW.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part743 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_532_1_9gZ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part744 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_533_1_inp.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part745 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_534_1_3EI.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part746 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_535_1_Ocb.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part747 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_536_1_syJ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part748 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_537_1_Dm6.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part749 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_538_1_KCT.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part750 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_539_2_S81.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part751 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_54_1_Faq.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part752 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_540_1_FLo.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part753 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_541_1_7E9.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part754 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_542_1_uaI.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part755 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_543_1_1Rb.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part756 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_544_1_fAE.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part757 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_545_1_lmE.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part758 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_546_1_pfT.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part759 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_547_1_wjC.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part760 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_548_1_EH1.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part761 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_549_2_Xx7.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part762 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_55_1_jZA.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part763 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_550_1_tG0.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part764 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_551_1_tAN.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part765 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_552_1_zax.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part766 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_553_1_ilP.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part767 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_554_1_UFO.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part768 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_555_2_o5o.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part769 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_556_2_9yq.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part770 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_557_1_poA.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part771 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_558_3_Etm.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part772 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_56_1_PKp.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part773 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_560_1_AaM.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part774 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_561_1_sVi.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part775 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_562_1_KZp.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part776 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_563_2_tIt.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part777 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_564_1_4SQ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part778 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_565_1_qr8.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part779 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_566_2_93m.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part780 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_567_1_lsm.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part781 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_568_2_vbr.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part782 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_569_1_MRR.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part783 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_57_2_fSM.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part784 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_570_1_D4H.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part785 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_571_1_FUo.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part786 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_572_1_Dzy.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part787 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_573_2_Ggt.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part788 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_574_1_weJ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part789 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_575_2_hXg.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part790 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_576_2_lob.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part791 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_577_1_iK9.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part792 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_578_2_42g.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part793 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_579_1_b55.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part794 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_58_2_XHm.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part795 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_580_1_MAn.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part796 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_581_2_ls5.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part797 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_582_1_Vj2.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part798 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_583_1_MMm.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part799 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_584_1_MIu.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part800 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_585_1_5L8.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part801 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_586_1_bCC.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part802 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_587_1_vH2.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part803 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_588_1_fnW.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part804 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_589_1_3DA.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part805 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_59_1_9Ox.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part806 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_590_1_hMu.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part807 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_591_1_Vth.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part808 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_592_2_xly.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part809 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_593_1_Ami.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part810 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_594_2_tpE.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part811 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_595_1_YYi.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part812 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_596_1_ars.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part813 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_597_1_RuO.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part814 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_598_1_WE9.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part815 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_599_1_tI8.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part816 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_6_1_TJM.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part817 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_60_1_Gpn.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part818 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_600_1_xXo.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part819 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_601_3_ofX.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part820 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_602_1_l1O.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part821 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_603_2_aUt.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part822 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_604_1_dNj.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part823 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_605_2_ycV.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part824 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_606_2_Nr3.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part825 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_607_1_W0k.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part826 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_608_3_00M.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part827 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_609_2_tni.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part828 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_61_1_F6c.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part829 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_610_2_1gp.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part830 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_611_2_BkJ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part831 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_612_1_KVu.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part832 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_613_2_l86.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part833 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_614_1_jvH.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part834 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_615_1_duu.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part835 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_616_1_GtD.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part836 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_617_3_bJe.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part837 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_618_1_xrg.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part838 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_619_1_Z3L.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part839 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_62_1_30z.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part840 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_620_1_68o.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part841 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_621_1_3nM.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part842 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_622_1_NX6.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part843 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_624_2_HIk.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part844 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_625_2_Esb.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part845 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_626_2_mMQ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part846 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_627_1_XGf.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part847 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_628_1_qTT.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part848 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_629_1_ZW7.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part849 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_63_1_U1u.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part850 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_630_1_HTm.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part851 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_631_1_s2X.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part852 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_632_1_aMU.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part853 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_633_1_aoU.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part854 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_634_3_tAU.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part855 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_635_1_lrq.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part856 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_636_2_OOt.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part857 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_637_1_8mu.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part858 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_638_2_YD9.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part859 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_639_1_MXS.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part860 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_64_1_PjY.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part861 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_640_1_rmp.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part862 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_641_1_K6p.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part863 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_642_2_jVH.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part864 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_644_1_61c.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part865 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_645_1_OWC.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part866 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_646_1_vMA.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part867 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_647_1_Rfw.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part868 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_648_2_lUs.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part869 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_649_1_1aR.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part870 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_65_1_YWK.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part871 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_651_2_PQR.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part872 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_652_1_1Mm.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part873 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_653_1_jNi.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part874 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_654_1_UP7.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part875 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_655_1_hCk.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part876 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_656_1_1TE.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part877 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_657_1_Pis.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part878 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_658_2_GOR.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part879 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_659_2_Rtr.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part880 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_66_2_1x3.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part881 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_660_1_q21.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part882 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_661_2_VRg.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part883 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_662_2_GiP.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part884 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_663_1_iEU.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part885 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_664_2_0Fj.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part886 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_665_1_PxC.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part887 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_666_2_li8.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part888 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_667_1_Bnz.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part889 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_668_1_MAB.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part890 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_669_1_2tZ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part891 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_67_1_qDK.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part892 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_670_2_jz9.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part893 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_671_2_4JN.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part894 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_672_1_MWV.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part895 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_673_1_xWu.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part896 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_674_1_lrT.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part897 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_675_2_aHz.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part898 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_676_1_1TG.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part899 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_677_1_uJt.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part900 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_678_1_gXW.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part901 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_679_1_UDk.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part902 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_68_1_xuZ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part903 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_680_1_Cxr.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part904 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_681_1_N5k.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part905 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_682_3_IFO.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part906 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_683_1_AM6.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part907 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_684_2_N5d.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part908 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_685_2_H9w.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part909 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_686_1_qmP.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part910 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_687_2_nYM.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part911 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_688_1_IIC.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part912 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_689_1_5GX.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part913 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_69_2_O8c.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part914 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_690_1_hdo.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part915 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_691_1_Qyc.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part916 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_692_1_IkD.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part917 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_693_2_KJU.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part918 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_694_1_YD0.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part919 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_695_1_jw7.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part920 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_696_2_rYB.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part921 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_697_1_Min.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part922 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_698_2_Eiv.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part923 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_699_2_h3P.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part924 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_7_1_cJZ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part925 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_70_2_zhy.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part926 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_700_3_KpT.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part927 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_701_1_tEe.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part928 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_702_2_YgG.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part929 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_703_1_Fby.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part930 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_704_1_Enj.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part931 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_705_2_ld1.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part932 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_706_1_DRQ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part933 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_707_1_vLH.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part934 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_708_1_6rR.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part935 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_709_1_N7B.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part936 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_71_1_gOp.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part937 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_710_1_cSK.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part938 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_711_1_cfG.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part939 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_712_1_kTd.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part940 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_713_2_2Eh.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part941 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_714_1_5Jg.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part942 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_715_1_Grg.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part943 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_716_1_L4a.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part944 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_717_1_vJp.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part945 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_718_2_uFV.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part946 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_719_2_02q.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part947 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_72_1_4t8.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part948 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_720_1_Voi.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part949 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_721_1_ww2.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part950 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_722_1_m8H.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part951 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_723_1_2f3.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part952 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_724_1_WBf.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part953 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_725_2_2VN.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part954 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_727_1_vIl.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part955 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_728_1_2Gx.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part956 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_73_1_Gzf.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part957 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_730_1_6ih.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part958 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_731_1_IY1.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part959 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_732_2_kbh.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part960 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_733_1_MvH.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part961 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_734_1_85z.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part962 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_735_2_GGl.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part963 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_736_1_fhK.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part964 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_737_1_4UZ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part965 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_738_1_wc3.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part966 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_739_2_TAa.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part967 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_74_1_MaT.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part968 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_740_2_B2z.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part969 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_741_1_WqP.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part970 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_742_1_kbf.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part971 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_743_1_lz9.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part972 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_744_1_puS.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part973 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_745_1_vrs.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part974 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_746_1_YgV.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part975 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_747_1_a1a.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part976 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_748_1_pYj.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part977 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_75_1_LB5.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part978 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_750_1_xXU.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part979 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_751_1_0fB.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part980 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_752_2_nSf.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part981 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_754_1_7ko.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part982 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_755_2_M9w.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part983 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_756_3_Qtc.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part984 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_757_2_lUd.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part985 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_758_1_ct8.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part986 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_759_1_1EC.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part987 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_76_1_NZf.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part988 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_760_1_5AI.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part989 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_761_1_xjt.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part990 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_762_1_y8E.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part991 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_763_3_dKi.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part992 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_764_2_mur.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part993 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_765_1_old.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part994 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_766_1_gYk.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part995 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_767_2_r1E.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part996 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_768_1_lpC.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part997 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_769_1_Vhv.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part998 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_77_2_1NZ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part999 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_770_1_3jS.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1000 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_771_1_Ot7.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1001 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_772_1_8ag.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1002 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_773_1_ouq.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1003 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_774_2_jkS.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1004 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_775_3_8mh.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1005 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_776_1_wVV.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1006 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_777_1_cqk.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1007 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_779_1_v3o.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1008 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_78_1_HXS.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1009 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_780_1_iUd.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1010 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_781_1_V4m.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1011 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_782_1_eBL.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1012 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_783_1_FuR.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1013 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_785_1_P9g.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1014 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_786_1_mko.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1015 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_787_2_odo.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1016 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_788_1_dVW.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1017 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_789_1_ja9.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1018 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_79_1_HHb.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1019 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_790_2_3Oe.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1020 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_791_1_Nvd.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1021 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_792_1_5ev.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1022 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_793_1_CgD.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1023 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_794_1_WYR.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1024 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_796_1_Xpo.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1025 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_797_1_C5x.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1026 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_798_1_DqW.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1027 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_799_1_itQ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1028 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_8_2_GUH.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1029 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_80_2_eAO.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1030 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_800_1_mGg.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1031 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_801_2_SXW.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1032 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_802_3_3jY.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1033 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_803_3_HWe.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1034 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_804_1_TXV.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1035 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_806_5_EqC.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1036 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_807_4_vy9.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1037 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_809_5_Ri9.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1038 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_81_1_8km.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1039 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_810_1_5jP.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1040 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_811_4_WZJ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1041 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_812_4_RuJ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1042 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_813_3_iph.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1043 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_815_3_FZs.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1044 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_816_1_LHn.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1045 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_817_1_x3L.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1046 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_818_3_NjK.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1047 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_819_1_AnX.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1048 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_82_1_BxX.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1049 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_820_4_ovv.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1050 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_821_4_IH7.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1051 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_822_1_xJR.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1052 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_823_2_dvr.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1053 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_824_4_krw.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1054 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_825_1_N1P.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1055 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_826_3_KdN.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1056 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_827_1_RVT.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1057 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_828_3_JlU.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1058 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_829_3_rUF.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1059 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_83_1_CGS.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1060 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_830_2_b4J.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1061 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_831_4_FIB.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1062 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_832_3_jNd.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1063 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_833_1_iGN.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1064 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_834_4_CNX.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1065 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_835_1_ggq.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1066 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_837_2_wUE.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1067 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_838_4_nhJ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1068 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_839_4_x7O.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1069 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_84_2_cJE.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1070 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_840_1_Nkk.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1071 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_841_1_BJT.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1072 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_842_3_HD4.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1073 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_843_3_sWc.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1074 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_844_3_Mkn.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1075 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_845_1_1Oy.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1076 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_846_4_qU5.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1077 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_847_2_d7G.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1078 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_848_3_idJ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1079 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_849_3_3vF.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1080 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_85_2_oTZ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1081 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_850_1_A2C.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1082 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_851_1_qpG.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1083 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_852_3_nIf.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1084 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_853_3_DJ5.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1085 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_854_1_5gY.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1086 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_855_2_Teb.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1087 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_857_1_LnO.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1088 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_858_5_68U.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1089 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_859_1_OT5.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1090 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_86_1_51O.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1091 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_861_4_ie7.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1092 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_862_1_r6C.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1093 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_863_3_Lbc.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1094 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_864_3_TxV.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1095 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_865_3_ckq.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1096 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_866_1_meI.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1097 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_867_1_4BZ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1098 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_868_2_P5B.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1099 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_869_2_693.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1100 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_87_1_jek.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1101 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_870_3_Z9Y.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1102 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_871_2_Vxu.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1103 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_872_1_aWA.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1104 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_873_3_n2C.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1105 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_874_1_Zh6.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1106 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_875_4_lVH.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1107 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_876_4_utC.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1108 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_877_1_uUt.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1109 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_878_1_mDb.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1110 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_879_3_hMd.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1111 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_88_1_lUr.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1112 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_880_1_tw4.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1113 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_881_2_sjI.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1114 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_882_1_yP4.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1115 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_883_2_2Ig.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1116 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_884_2_9GB.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1117 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_886_4_NsN.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1118 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_887_3_uPi.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1119 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_888_5_IRB.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1120 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_89_1_2et.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1121 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_890_4_2yB.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1122 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_892_4_CDK.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1123 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_894_1_xMZ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1124 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_896_2_KP8.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1125 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_898_3_VyK.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1126 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_899_2_I4z.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1127 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_90_1_aFC.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1128 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_901_1_WoM.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1129 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_902_2_SD5.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1130 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_903_3_FJ5.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1131 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_904_2_wm5.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1132 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_906_5_1h6.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1133 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_907_5_nhT.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1134 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_908_2_yZW.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1135 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_909_3_kL3.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1136 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_91_2_11O.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1137 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_911_1_JMr.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1138 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_912_3_JMu.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1139 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_913_4_ER9.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1140 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_914_1_P4n.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1141 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_916_3_QM3.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1142 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_918_3_XgU.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1143 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_919_3_oSM.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1144 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_92_2_DpV.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1145 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_921_3_Ydh.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1146 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_922_2_Ryl.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1147 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_923_3_qkP.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1148 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_924_2_Arr.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1149 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_926_2_2wE.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1150 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_927_1_7ta.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1151 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_928_3_vxz.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1152 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_929_3_SLQ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1153 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_93_2_vzu.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1154 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_931_3_QRQ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1155 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_932_3_VcV.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1156 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_933_3_89w.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1157 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_934_3_wPD.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1158 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_935_3_XAU.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1159 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_936_4_VtC.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1160 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_937_3_Jgm.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1161 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_94_2_Hnt.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1162 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_941_4_Oib.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1163 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_942_1_yJE.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1164 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_943_3_dw6.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1165 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_944_3_jpL.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1166 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_945_2_VqJ.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1167 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_946_1_geI.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1168 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_947_4_bS2.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1169 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_95_1_6qK.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1170 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_950_1_jYn.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1171 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_951_4_FAS.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1172 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_952_2_Yzq.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1173 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_953_1_tMr.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1174 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_954_4_YGD.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1175 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_955_3_ok3.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1176 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_957_2_tBh.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1177 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_958_4_TNN.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1178 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_959_4_zPM.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1179 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_96_1_jYG.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1180 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_960_3_5pL.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1181 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_961_3_ycg.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1182 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_962_3_6B1.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1183 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_963_2_tap.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1184 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_964_4_Vl7.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1185 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_965_1_sW2.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1186 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_966_3_sK6.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1187 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_967_1_UNp.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1188 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_968_3_EzD.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1189 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_969_4_wVX.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1190 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_97_1_T60.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1191 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_970_3_77P.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1192 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_971_3_eT7.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1193 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_972_4_dy1.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1194 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_973_2_TcW.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1195 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_974_4_a1E.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1196 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_975_1_2bs.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1197 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_976_2_WVp.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1198 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_978_1_HRa.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1199 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_979_3_k64.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1200 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_98_2_8Bo.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1201 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_980_1_bTB.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1202 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_981_3_gnO.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1203 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_982_4_nTF.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1204 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_983_1_OyX.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1205 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_984_2_TeL.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1206 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_985_3_A2D.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1207 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_986_3_8Vr.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1208 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_987_2_0fj.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1209 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_988_1_1OB.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1210 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_989_3_FD4.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1211 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_99_1_hGv.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1212 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_990_5_I1E.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1213 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_991_1_7ne.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1214 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_993_3_6UP.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1215 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_995_3_6GB.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1216 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_996_3_sAb.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1217 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_997_2_TX6.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1218 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_998_5_RNz.root'
#)
#fileNamesZtoElecTau_QCD_EM_Pt30to80_part1219 = cms.untracked.vstring(
#	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/QCD_EM_Pt30to80_2/skimElecTau_999_2_Xnr.root'
#)

#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# EM enriched QCD sample generated with Pythia (80 GeV < Pt(hat) < 170 GeV)
#
intLumiZtoElecTau_QCD_EM_Pt80to170 = float(7.48)
corrFactorZtoElecTau_QCD_EM_Pt80to170 = float(1.01)

patTupleOutputFileNameZtoElecTau_QCD_EM_Pt80to170 = cms.untracked.string('patTupleZtoElecTau_QCD_EM_Pt80to170_partXX.root')

plotsOutputFileNameZtoElecTau_QCD_EM_Pt80to170 = cms.string('plotsZtoElecTau_QCD_EM_Pt80to170_partXX.root')

genPhaseSpaceCutZtoElecTau_QCD_EM_Pt80to170 = cms.PSet(
  pluginName = cms.string('genPhaseSpaceCut'),
  pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

fileNamesZtoElecTau_QCD_EM_Pt80to170_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_1_1_aXl.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_10_1_SkO.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_100_1_1kg.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_101_1_Z1W.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_102_1_Z2Z.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part06 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_103_1_Qxb.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part07 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_104_1_tNU.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part08 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_105_1_B0e.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part09 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_106_1_2KV.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part10 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_107_1_gb4.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part11 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_108_1_RKO.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part12 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_109_1_0gU.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part13 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_11_1_vDC.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part14 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_110_1_ANf.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part15 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_111_1_819.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part16 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_112_1_GDH.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part17 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_113_1_p16.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part18 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_114_1_quQ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part19 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_115_1_Uoz.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part20 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_116_1_LPz.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part21 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_117_1_d1y.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part22 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_118_1_7UD.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part23 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_119_1_01n.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part24 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_12_1_J6X.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part25 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_120_1_ImF.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part26 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_121_1_B24.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part27 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_122_1_mgm.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part28 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_123_1_R2Q.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part29 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_124_1_Am2.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part30 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_125_1_WkB.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part31 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_126_1_GZi.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part32 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_127_1_gCr.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part33 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_128_1_Msf.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part34 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_129_1_PO3.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part35 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_13_1_1tt.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part36 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_130_1_Aut.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part37 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_131_1_JKw.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part38 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_132_1_MMN.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part39 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_133_1_LZP.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part40 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_134_1_x3V.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part41 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_135_2_Hur.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part42 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_136_1_qpw.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part43 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_137_1_vBL.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part44 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_138_1_qQO.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part45 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_139_1_ge7.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part46 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_14_1_b5A.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part47 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_140_1_iJt.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part48 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_141_1_qPV.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part49 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_142_1_B2H.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part50 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_143_1_fXW.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part51 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_144_1_2Fi.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part52 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_145_1_cbv.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part53 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_146_1_Vwu.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part54 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_147_1_7Fr.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part55 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_148_1_Euv.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part56 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_149_1_6ZX.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part57 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_15_1_9dK.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part58 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_150_1_8RZ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part59 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_151_1_abJ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part60 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_152_1_5NM.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part61 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_153_1_ZzB.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part62 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_155_1_AKt.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part63 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_156_1_3lG.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part64 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_157_1_vcA.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part65 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_158_1_2Gb.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part66 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_159_1_GTi.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part67 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_16_1_lZP.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part68 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_160_1_6WW.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part69 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_161_1_kaS.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part70 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_162_1_DDy.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part71 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_163_1_2qh.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part72 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_164_2_fvF.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part73 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_165_1_j1N.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part74 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_166_1_bCe.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part75 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_167_1_9ru.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part76 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_168_2_Gvp.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part77 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_169_1_MrL.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part78 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_17_1_XIn.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part79 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_170_1_IeK.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part80 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_171_1_4wW.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part81 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_172_1_RXm.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part82 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_173_1_1Zo.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part83 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_174_1_tpj.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part84 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_175_1_DHW.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part85 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_176_1_rNM.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part86 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_177_1_rIM.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part87 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_178_1_BDR.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part88 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_179_1_QoV.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part89 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_18_1_zcg.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part90 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_180_1_3MU.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part91 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_181_1_S6e.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part92 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_182_1_6nA.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part93 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_183_1_87X.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part94 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_184_1_DQ1.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part95 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_185_1_Akd.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part96 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_186_1_T2k.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part97 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_187_1_uOx.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part98 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_188_1_bet.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part99 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_189_1_TgO.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part100 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_19_1_PEx.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part101 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_190_1_5dV.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part102 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_191_1_KqR.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part103 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_192_1_Ldh.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part104 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_193_1_Qjv.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part105 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_194_1_b7V.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part106 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_195_1_nMY.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part107 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_196_2_nVi.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part108 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_197_1_GaU.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part109 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_198_1_xhy.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part110 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_199_1_G6y.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part111 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_2_1_MEc.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part112 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_20_2_QAH.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part113 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_200_1_WEE.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part114 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_21_1_o0F.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part115 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_22_1_cNR.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part116 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_23_1_frB.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part117 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_24_1_E14.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part118 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_25_1_kK3.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part119 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_26_1_Xc2.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part120 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_27_1_aKg.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part121 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_28_1_oqM.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part122 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_29_1_qR8.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part123 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_3_1_nb2.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part124 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_30_1_KXD.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part125 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_31_1_NG7.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part126 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_32_1_oBa.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part127 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_33_1_VYC.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part128 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_34_1_XBN.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part129 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_35_1_xR8.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part130 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_36_1_DU8.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part131 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_37_1_8jw.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part132 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_38_1_pVq.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part133 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_39_1_rBr.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part134 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_4_2_w5N.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part135 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_40_1_W45.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part136 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_41_1_VkM.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part137 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_42_1_MPq.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part138 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_43_1_wsD.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part139 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_44_1_KOE.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part140 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_45_1_A2S.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part141 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_46_1_DyA.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part142 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_47_1_Hdp.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part143 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_48_1_sPE.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part144 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_49_1_9e6.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part145 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_5_1_HYx.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part146 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_50_1_7te.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part147 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_51_1_ckY.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part148 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_52_1_2IT.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part149 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_53_1_aHQ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part150 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_54_1_WbE.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part151 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_55_1_8eK.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part152 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_56_1_YUB.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part153 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_57_1_eAr.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part154 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_58_1_28x.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part155 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_59_1_3MZ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part156 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_6_1_p4S.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part157 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_60_1_xCP.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part158 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_61_1_KQF.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part159 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_62_1_pgU.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part160 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_63_1_reN.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part161 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_64_1_PcB.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part162 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_65_1_XTw.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part163 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_66_1_BFV.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part164 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_67_1_JgG.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part165 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_68_1_IMX.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part166 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_69_1_xqU.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part167 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_7_1_vaO.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part168 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_70_2_f1e.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part169 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_71_1_Txo.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part170 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_72_1_WtC.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part171 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_73_1_oh6.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part172 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_74_1_qjg.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part173 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_75_1_EfE.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part174 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_76_1_fhN.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part175 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_77_1_mW5.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part176 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_78_1_fim.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part177 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_79_1_CFd.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part178 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_8_1_a6n.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part179 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_80_1_Yd6.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part180 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_81_1_SSr.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part181 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_82_2_qJ5.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part182 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_83_1_A1o.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part183 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_84_1_Dvt.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part184 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_85_1_QMV.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part185 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_86_1_PrI.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part186 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_87_1_aF4.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part187 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_88_1_eIJ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part188 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_89_1_yB6.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part189 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_9_1_WYY.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part190 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_90_1_oVh.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part191 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_91_1_zyU.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part192 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_92_1_oW0.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part193 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_93_1_VHJ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part194 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_94_1_qJn.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part195 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_95_1_DEB.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part196 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_96_1_Hkc.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part197 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_97_1_gC0.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part198 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_98_1_QJ2.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part199 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_99_1_7J2.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part200 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_154_3_iHQ.root'
)
fileNamesZtoElecTau_QCD_EM_Pt80to170_part201 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_EM_Pt80to170/skimElecTau_201_3_7cN.root'
)


#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# QCD b/c --> e sample generated with Pythia (20 GeV < Pt(hat) < 30 GeV)

intLumiZtoElecTau_QCD_BCtoE_Pt20to30 = float(16.98)
corrFactorZtoElecTau_QCD_BCtoE_Pt20to30 = float(1.)

patTupleOutputFileNameZtoElecTau_QCD_BCtoE_Pt20to30 = cms.untracked.string('patTupleZtoElecTau_QCD_BCtoE_Pt20to30_partXX.root')

plotsOutputFileNameZtoElecTau_QCD_BCtoE_Pt20to30 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt20to30_partXX.root')

genPhaseSpaceCutZtoElecTau_QCD_BCtoE_Pt20to30 = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_100_1_xza.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_101_1_8OY.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_102_1_D2E.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_103_1_Pm2.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_104_1_4oZ.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_105_1_QHY.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_106_1_uht.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_107_1_mRW.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_108_1_oPl.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_109_1_6gS.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part06 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_10_1_WSt.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_110_1_5Wr.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part07 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_111_1_IXp.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_112_1_D2n.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part08 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_113_1_uaX.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_114_1_12O.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part09 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_115_1_n0y.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_116_1_4cq.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part10 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_117_1_9wZ.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_118_1_srO.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part11 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_119_1_I9l.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_11_1_mjz.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part12 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_120_1_4ei.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_121_1_Sm4.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part13 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_122_1_1CR.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_123_1_dZG.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part14 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_124_1_KWd.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_125_1_VDZ.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part15 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_126_1_cW8.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_127_1_OZJ.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part16 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_128_1_STV.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_129_1_rxu.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part17 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_12_1_ndH.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_130_1_8Io.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part18 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_131_1_sSo.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_132_1_lQ2.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part19 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_133_1_yIF.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_134_1_Q6r.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part20 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_135_1_FJk.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_136_1_3MS.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part21 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_137_1_Qj5.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_138_1_X0Y.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part22 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_139_1_YIb.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_13_1_lpm.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part23 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_140_1_Mun.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_141_1_814.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part24 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_142_1_rhs.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_143_1_uNS.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part25 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_144_1_sXU.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_145_1_7RL.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part26 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_146_1_bSk.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_147_1_QXy.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part27 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_148_1_wUn.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_149_1_H70.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part28 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_14_1_5oy.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_150_1_HTF.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part29 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_151_1_Nim.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_152_1_HNi.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part30 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_153_1_H2S.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_154_1_tYq.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part31 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_155_1_ROr.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_156_1_Ej4.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part32 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_157_1_NGr.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_158_1_R4S.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part33 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_159_1_uXI.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_15_1_CCI.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part34 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_160_1_PdX.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_161_1_cnO.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part35 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_162_1_O7C.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_163_1_udJ.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part36 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_164_1_Dit.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_165_1_crH.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part37 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_166_1_cQ4.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_167_1_gbu.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part38 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_168_1_gCQ.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_169_1_mIK.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part39 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_16_1_CVj.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_170_1_wQB.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part40 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_171_1_VYZ.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_172_1_JVb.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part41 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_173_1_QSZ.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_174_1_nLR.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part42 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_175_1_mdN.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_176_1_SDR.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part43 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_177_1_3oP.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_178_1_2NF.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part44 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_179_1_2oH.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_17_1_9cb.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part45 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_180_1_uVf.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_181_1_12d.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part46 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_182_1_p7M.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_183_1_2JX.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part47 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_184_1_MdZ.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_185_1_KCT.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part48 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_186_1_SFp.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_187_1_Ykq.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part49 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_188_1_LcB.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_189_1_NZ9.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part50 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_18_1_17O.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_190_1_N0r.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part51 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_191_1_Yaw.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_192_1_Q9N.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part52 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_193_1_8ST.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_194_1_dDP.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part53 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_195_1_8FL.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_196_1_JVb.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part54 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_197_1_Ant.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_198_1_YxQ.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part55 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_199_1_8CW.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_19_1_oFL.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part56 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_1_1_sCz.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_200_1_12Y.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part57 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_201_1_jfG.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_202_1_B08.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part58 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_20_1_YOq.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_21_1_eAl.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part59 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_22_1_SVX.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_23_1_eln.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part60 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_24_1_SdK.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_25_1_Np0.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part61 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_26_1_Rbu.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_27_1_Nj8.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part62 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_28_1_O3g.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_29_1_Jk1.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part63 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_2_1_JlO.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_30_1_dcn.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part64 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_31_1_enS.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_32_1_MWo.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part65 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_33_1_oO3.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_34_1_NjH.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part66 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_35_1_iLu.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_36_1_Zuj.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part67 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_37_1_hD7.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_38_1_xHU.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part68 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_39_1_baB.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_3_1_bfh.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part69 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_40_1_zHV.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_41_1_uWg.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part70 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_42_1_34m.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_43_1_E1f.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part71 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_44_1_aaa.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_45_1_vL7.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part72 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_46_1_EfX.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_47_1_WY1.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part73 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_48_1_CQC.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_49_1_j6P.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part74 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_4_1_mDp.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_50_1_vyb.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part75 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_51_1_Br5.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_52_1_MNQ.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part76 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_53_1_tno.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_54_1_tre.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part77 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_55_1_L6U.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_56_1_QNM.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part78 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_57_1_o6d.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_58_1_0fJ.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part79 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_59_1_6Ad.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_5_1_Uqg.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part80 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_60_1_RIu.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_61_1_qsZ.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part81 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_62_1_Ny2.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_63_1_12E.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part82 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_64_1_sWD.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_65_1_mig.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part83 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_66_1_toQ.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_67_1_tM2.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part84 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_68_1_BG6.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_69_1_khi.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part85 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_6_1_jIz.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_70_1_oap.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part86 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_71_1_0eR.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_72_1_UG2.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part87 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_73_1_Eny.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_74_1_sx5.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part88 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_75_1_Bgz.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_76_1_JLy.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part89 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_77_1_W4m.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_78_1_9LV.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part90 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_79_1_YkW.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_7_1_mNl.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part91 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_80_1_VUW.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_81_1_f3M.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part92 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_82_1_q4A.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_83_1_EiT.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part93 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_84_1_TM7.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_85_1_vXn.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part94 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_86_1_e1E.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_87_1_MX7.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part95 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_88_1_8vj.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_89_1_7T5.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part96 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_8_1_iEa.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_90_1_r0x.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part97 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_91_2_pbb.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_92_1_sKH.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part98 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_93_1_amt.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_94_1_GXI.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part99 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_95_1_SFd.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_96_1_aJk.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part100 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_97_1_cWk.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_98_1_gtW.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt20to30_part101 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_99_1_VQp.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/QCD_BCtoE_Pt20to30/skimElecTau_9_1_uqK.root'
)


#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# QCD b/c -->e sample generated with Pythia (30 GeV < Pt(hat) < 80 GeV)
#
intLumiZtoElecTau_QCD_BCtoE_Pt30to80 = float(14.59)
corrFactorZtoElecTau_QCD_BCtoE_Pt30to80 = float(1.)

patTupleOutputFileNameZtoElecTau_QCD_BCtoE_Pt30to80 = cms.untracked.string('patTupleZtoElecTau_QCD_BCtoE_Pt30to80_partXX.root')

plotsOutputFileNameZtoElecTau_QCD_BCtoE_Pt30to80 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt30to80_partXX.root')

genPhaseSpaceCutZtoElecTau_QCD_BCtoE_Pt30to80 = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_1_2_NfC.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_10_2_52o.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_100_1_Xrl.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_101_1_xn3.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_102_1_SVP.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part06 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_103_1_uy7.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part07 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_104_1_FXN.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part08 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_105_1_uOs.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part09 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_106_1_Yoh.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part10 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_107_1_a6z.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part11 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_108_1_CXI.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part12 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_109_1_g6u.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part13 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_11_2_ksi.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part14 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_110_1_y4E.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part15 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_111_1_Edy.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part16 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_112_1_Tt3.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part17 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_113_2_HMJ.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part18 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_114_1_VqD.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part19 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_115_1_zqp.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part20 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_116_1_PD7.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part21 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_117_1_dev.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part22 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_118_3_Gt1.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part23 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_119_2_HWb.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part24 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_12_1_CMY.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part25 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_120_1_VZG.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part26 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_121_1_Wc0.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part27 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_122_1_Jg4.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part28 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_123_1_lMR.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part29 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_124_1_d8L.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part30 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_125_1_6EC.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part31 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_126_1_auT.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part32 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_127_1_ZXO.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part33 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_128_1_Dnx.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part34 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_129_1_sJm.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part35 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_13_1_eFD.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part36 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_130_1_2wm.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part37 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_131_1_Hzp.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part38 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_132_1_fzX.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part39 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_133_1_0zE.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part40 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_134_1_VZd.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part41 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_135_1_3AO.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part42 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_136_1_D68.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part43 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_137_3_vm6.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part44 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_138_1_YI3.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part45 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_139_1_W2y.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part46 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_14_1_kNf.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part47 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_140_1_X1n.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part48 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_142_2_h3z.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part49 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_143_1_ZLG.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part50 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_144_1_6oB.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part51 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_145_1_SPn.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part52 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_146_1_Ew3.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part53 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_147_1_niF.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part54 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_148_1_ks5.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part55 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_149_2_Jik.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part56 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_15_1_LQl.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part57 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_150_1_Zrm.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part58 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_151_1_wIj.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part59 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_152_1_WJE.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part60 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_153_2_rNE.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part61 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_154_1_QA0.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part62 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_155_1_2h4.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part63 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_156_1_6Mn.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part64 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_157_2_GWD.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part65 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_158_1_nzY.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part66 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_159_1_5j2.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part67 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_16_1_26z.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part68 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_160_1_lB5.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part69 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_161_1_4ov.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part70 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_162_1_4jq.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part71 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_163_1_ER2.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part72 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_164_1_r0M.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part73 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_165_2_pRs.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part74 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_166_2_b2P.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part75 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_167_2_PW3.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part76 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_168_1_yNX.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part77 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_169_2_jv2.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part78 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_17_1_ABu.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part79 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_170_1_DwA.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part80 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_171_1_yvF.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part81 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_172_1_4G3.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part82 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_173_2_DT3.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part83 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_174_1_JuN.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part84 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_175_1_rA8.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part85 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_176_1_Mi6.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part86 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_177_1_Qaq.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part87 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_178_1_wtt.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part88 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_179_1_o8F.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part89 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_18_1_72l.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part90 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_180_1_oC6.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part91 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_181_1_U0i.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part92 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_182_1_I1C.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part93 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_183_1_rTX.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part94 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_184_1_sxu.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part95 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_185_1_jft.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part96 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_186_1_5lw.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part97 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_187_1_ngF.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part98 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_188_1_CfN.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part99 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_189_1_x2e.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part100 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_19_1_cng.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part101 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_190_1_sZY.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part102 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_191_2_SYU.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part103 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_192_1_fdP.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part104 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_193_1_P67.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part105 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_194_1_1fq.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part106 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_195_2_PrK.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part107 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_197_1_rdJ.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part108 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_198_1_H1T.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part109 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_199_2_wKo.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part110 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_2_1_VEW.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part111 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_20_1_9xK.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part112 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_200_2_BmM.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part113 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_201_2_mWW.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part114 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_21_2_rHc.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part115 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_22_1_aIr.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part116 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_23_1_dYx.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part117 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_24_1_CXT.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part118 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_25_1_Eqw.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part119 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_26_1_TQu.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part120 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_27_1_yh5.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part121 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_28_1_CfE.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part122 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_29_1_DGE.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part123 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_3_1_GbH.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part124 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_30_1_Uwm.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part125 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_31_2_Ywy.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part126 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_32_2_eQa.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part127 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_33_1_O65.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part128 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_34_2_mlp.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part129 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_35_2_q4z.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part130 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_36_1_p2W.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part131 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_37_2_Srb.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part132 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_38_2_O9G.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part133 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_39_1_efq.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part134 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_4_1_S7A.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part135 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_40_1_bwP.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part136 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_41_2_uKK.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part137 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_42_1_R3W.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part138 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_43_1_qjK.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part139 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_44_1_ONP.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part140 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_45_1_Yx9.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part141 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_46_1_hHk.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part142 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_47_1_SLQ.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part143 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_48_1_qiP.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part144 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_49_2_KcC.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part145 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_5_1_3kB.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part146 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_50_2_75m.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part147 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_51_2_RvI.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part148 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_52_1_AhS.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part149 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_53_1_ZVn.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part150 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_54_1_nyP.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part151 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_55_2_Frp.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part152 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_56_2_HQ8.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part153 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_57_1_TX1.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part154 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_58_1_HhG.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part155 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_59_1_67k.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part156 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_6_1_npi.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part157 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_60_1_xmK.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part158 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_61_1_U0T.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part159 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_62_1_R4y.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part160 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_63_2_Iwz.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part161 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_64_1_sxS.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part162 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_65_1_QCc.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part163 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_66_1_gxJ.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part164 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_67_1_120.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part165 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_68_2_P91.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part166 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_69_1_giH.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part167 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_7_1_JUT.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part168 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_70_1_OJd.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part169 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_71_1_dCb.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part170 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_72_1_QLK.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part171 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_73_1_3bX.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part172 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_74_1_tWe.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part173 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_75_1_Hj2.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part174 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_76_1_wIp.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part175 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_77_1_h9Q.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part176 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_78_1_k8b.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part177 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_79_1_tyy.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part178 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_8_1_GEC.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part179 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_80_2_Whb.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part180 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_81_1_mzt.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part181 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_82_2_CuW.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part182 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_83_2_aCf.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part183 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_84_1_g6b.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part184 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_85_1_FFC.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part185 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_86_1_c9T.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part186 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_87_1_IKj.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part187 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_88_2_giz.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part188 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_89_1_7bs.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part189 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_9_1_TOz.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part190 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_90_1_VtM.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part191 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_91_1_xcU.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part192 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_92_1_Zs1.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part193 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_93_1_rML.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part194 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_94_1_lsT.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part195 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_95_1_WVu.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part196 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_96_1_Gcu.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part197 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_97_1_dEd.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part198 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_98_2_VxM.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part199 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_99_3_b2K.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part200 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_141_2_Nt4.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt30to80_part201 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt30to80/skimElecTau_196_2_Vbb.root'
)


#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
#  QCD b/c -->e sample generated with Pythia (80 GeV < Pt(hat) < 170 GeV)
#
intLumiZtoElecTau_QCD_BCtoE_Pt80to170 = float(862.6)
# 
corrFactorZtoElecTau_QCD_BCtoE_Pt80to170 = float(1.003)

patTupleOutputFileNameZtoElecTau_QCD_BCtoE_Pt80to170 = cms.untracked.string('patTupleZtoElecTau_QCD_BCtoE_Pt80to170_partXX.root')

plotsOutputFileNameZtoElecTau_QCD_BCtoE_Pt80to170 = cms.string('plotsZtoElecTau_QCD_BCtoE_Pt80to170_partXX.root')

genPhaseSpaceCutZtoElecTau_QCD_BCtoE_Pt80to170 = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_1_1_9sx.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_10_1_mF8.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_100_1_9B8.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_101_1_y2u.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_102_1_AId.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part06 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_103_1_cqH.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part07 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_104_1_qOJ.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part08 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_105_1_Ihw.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part09 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_106_1_85d.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part10 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_107_1_iVr.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part11 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_108_1_nFN.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part12 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_109_1_SX0.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part13 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_11_1_Xui.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part14 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_110_1_Iq6.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part15 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_111_1_pbq.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part16 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_112_1_2sO.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part17 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_113_1_ovm.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part18 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_114_1_Q8A.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part19 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_115_1_pLp.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part20 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_116_1_hFE.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part21 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_117_1_OJj.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part22 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_118_1_Cap.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part23 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_119_1_jJo.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part24 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_12_1_1dU.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part25 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_120_1_nn2.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part26 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_121_1_17P.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part27 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_122_1_yVa.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part28 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_123_1_Efw.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part29 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_124_1_MR1.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part30 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_125_1_QWV.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part31 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_126_1_d9i.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part32 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_127_1_7o9.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part33 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_129_1_78o.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part34 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_13_1_etP.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part35 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_130_1_AeG.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part36 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_131_1_svy.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part37 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_132_1_fsJ.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part38 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_133_1_i7F.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part39 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_134_1_tT4.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part40 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_136_1_O4F.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part41 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_137_1_8kU.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part42 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_138_1_M23.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part43 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_14_1_imh.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part44 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_140_1_1La.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part45 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_141_1_yev.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part46 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_142_1_MFk.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part47 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_143_1_D8o.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part48 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_146_1_izt.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part49 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_147_1_TVm.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part50 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_148_1_tY2.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part51 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_149_1_HKw.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part52 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_15_1_NlD.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part53 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_150_1_sNk.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part54 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_151_1_d3J.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part55 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_152_1_XDE.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part56 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_153_1_hQS.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part57 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_154_1_tai.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part58 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_155_1_tRq.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part59 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_156_1_u8A.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part60 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_157_1_QZY.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part61 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_158_1_R2V.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part62 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_159_1_Xsw.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part63 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_16_1_fnr.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part64 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_160_1_Vmx.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part65 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_161_1_tKB.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part66 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_162_1_8GS.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part67 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_163_1_WUR.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part68 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_164_1_EqJ.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part69 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_165_1_VNS.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part70 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_166_1_RJt.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part71 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_167_1_2VE.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part72 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_168_1_5sx.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part73 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_169_1_oDE.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part74 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_17_1_lqP.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part75 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_170_1_GgI.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part76 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_171_1_ooI.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part77 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_172_1_pm2.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part78 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_173_1_YlU.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part79 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_174_1_dXw.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part80 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_175_1_whT.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part81 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_176_1_2ti.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part82 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_177_1_a9v.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part83 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_178_1_WFi.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part84 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_179_1_f31.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part85 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_18_1_SZj.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part86 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_180_1_HeE.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part87 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_181_1_0gn.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part88 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_182_1_vWB.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part89 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_183_1_Jjt.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part90 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_184_1_r8F.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part91 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_185_1_0qr.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part92 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_186_1_zHg.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part93 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_187_1_soc.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part94 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_188_1_9O5.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part95 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_189_3_4Qf.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part96 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_19_1_ITt.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part97 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_190_3_R3F.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part98 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_191_3_djV.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part99 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_192_3_CzD.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part100 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_193_3_sgw.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part101 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_194_3_Y5Z.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part102 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_195_3_uUV.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part103 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_196_3_LqX.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part104 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_197_3_obw.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part105 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_198_3_BRu.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part106 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_199_3_enP.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part107 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_2_1_wIg.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part108 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_20_3_0Z7.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part109 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_200_3_dTh.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part110 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_201_3_4qc.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part111 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_202_3_jnc.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part112 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_203_3_c4j.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part113 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_204_3_BkI.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part114 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_205_3_5Fc.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part115 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_206_3_0U2.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part116 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_207_3_d15.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part117 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_21_3_JOb.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part118 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_22_3_rFH.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part119 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_23_3_2Mw.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part120 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_24_3_FPr.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part121 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_25_3_HVG.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part122 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_26_3_bHZ.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part123 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_27_3_YnY.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part124 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_28_3_dFp.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part125 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_29_3_PZQ.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part126 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_3_1_Hzg.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part127 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_30_3_uSK.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part128 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_31_3_k8C.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part129 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_32_3_it6.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part130 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_33_3_s40.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part131 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_34_3_CME.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part132 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_35_3_YXJ.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part133 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_36_3_NGE.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part134 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_37_3_V1r.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part135 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_38_3_LOm.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part136 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_39_1_FDo.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part137 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_4_1_VNZ.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part138 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_40_1_73o.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part139 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_41_1_bRU.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part140 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_42_1_4CW.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part141 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_43_1_wZN.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part142 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_44_1_xAy.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part143 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_45_1_JoB.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part144 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_46_1_7BB.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part145 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_47_1_Cf0.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part146 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_48_1_9g7.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part147 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_49_1_92Q.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part148 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_5_1_RSW.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part149 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_50_1_NRG.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part150 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_51_1_frA.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part151 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_52_1_Wpk.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part152 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_53_1_0hs.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part153 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_54_1_cbZ.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part154 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_55_1_sYG.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part155 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_56_1_JR5.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part156 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_57_1_SUG.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part157 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_58_1_PKM.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part158 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_59_1_mSj.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part159 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_6_1_ZV5.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part160 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_60_1_233.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part161 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_61_1_3ej.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part162 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_62_1_nRy.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part163 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_63_1_58R.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part164 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_64_1_Cy9.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part165 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_65_1_tWG.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part166 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_66_1_UEL.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part167 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_67_1_hU0.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part168 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_68_1_xc0.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part169 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_69_1_SWd.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part170 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_7_1_Mvi.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part171 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_70_1_vxF.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part172 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_71_1_YHp.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part173 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_72_1_KJl.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part174 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_73_1_5XJ.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part175 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_74_1_RHV.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part176 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_75_1_2ZR.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part177 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_76_1_cfP.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part178 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_77_3_PB9.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part179 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_78_3_N73.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part180 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_79_3_CkU.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part181 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_8_1_uDV.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part182 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_80_3_m9K.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part183 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_81_3_fD1.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part184 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_82_3_3f9.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part185 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_84_3_w0V.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part186 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_85_3_1OV.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part187 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_86_3_n61.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part188 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_87_3_gGk.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part189 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_88_3_rUq.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part190 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_89_3_YXo.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part191 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_9_1_ZPM.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part192 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_90_3_yEF.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part193 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_91_3_LZx.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part194 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_92_3_aaO.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part195 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_93_3_DFX.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part196 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_94_3_zbC.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part197 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_95_3_IFu.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part198 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_96_1_jVo.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part199 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_97_1_cQO.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part200 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_98_1_028.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part201 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_99_1_VTy.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part202 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_128_3_4kp.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part203 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_135_2_cAN.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part204 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_139_2_FtL.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part205 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_144_2_mzJ.root'
)
fileNamesZtoElecTau_QCD_BCtoE_Pt80to170_part206 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//QCD_BCtoE_Pt80to170/skimElecTau_145_2_UxM.root'
)


#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# photon + jets samples 
#

fileNamesZtoElecTau_PhotonPlusJets_Pt15to30_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/PhotonPlusJets_Pt15to30/skimElecTau_10_1_8zq.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/PhotonPlusJets_Pt15to30/skimElecTau_11_1_NZG.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/PhotonPlusJets_Pt15to30/skimElecTau_1_1_sTP.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/PhotonPlusJets_Pt15to30/skimElecTau_2_1_Y94.root'
)
fileNamesZtoElecTau_PhotonPlusJets_Pt15to30_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/PhotonPlusJets_Pt15to30/skimElecTau_3_1_W3i.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/PhotonPlusJets_Pt15to30/skimElecTau_4_2_Cf1.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/PhotonPlusJets_Pt15to30/skimElecTau_5_1_Uwo.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/PhotonPlusJets_Pt15to30/skimElecTau_6_1_mzO.root'
)
fileNamesZtoElecTau_PhotonPlusJets_Pt15to30_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/PhotonPlusJets_Pt15to30/skimElecTau_7_3_yGg.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/PhotonPlusJets_Pt15to30/skimElecTau_8_1_6KQ.root',
	'rfio:/castor/cern.ch/user/j/jkolb/eTauSkims/fall10/PhotonPlusJets_Pt15to30/skimElecTau_9_1_gZo.root'
)

fileNamesZtoElecTau_PhotonPlusJets_Pt30to50_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/PhotonPlusJets_Pt30to50/skimElecTau_1_1_8ge.root',
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/PhotonPlusJets_Pt30to50/skimElecTau_10_1_627.root',
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/PhotonPlusJets_Pt30to50/skimElecTau_2_2_2KR.root'
)
fileNamesZtoElecTau_PhotonPlusJets_Pt30to50_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/PhotonPlusJets_Pt30to50/skimElecTau_3_1_DBW.root',
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/PhotonPlusJets_Pt30to50/skimElecTau_4_2_Wg3.root',
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/PhotonPlusJets_Pt30to50/skimElecTau_5_1_a62.root'
)
fileNamesZtoElecTau_PhotonPlusJets_Pt30to50_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/PhotonPlusJets_Pt30to50/skimElecTau_6_1_PH8.root',
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/PhotonPlusJets_Pt30to50/skimElecTau_7_2_4gj.root',
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/PhotonPlusJets_Pt30to50/skimElecTau_8_2_o5b.root'
)
fileNamesZtoElecTau_PhotonPlusJets_Pt30to50_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/PhotonPlusJets_Pt30to50/skimElecTau_9_1_Z4F.root'
)

fileNamesZtoElecTau_PhotonPlusJets_Pt50to80_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/PhotonPlusJets_Pt50to80/skimElecTau_1_1_A09.root',
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/PhotonPlusJets_Pt50to80/skimElecTau_10_1_ZDw.root',
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/PhotonPlusJets_Pt50to80/skimElecTau_11_1_5bE.root'
)
fileNamesZtoElecTau_PhotonPlusJets_Pt50to80_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/PhotonPlusJets_Pt50to80/skimElecTau_2_1_mNL.root',
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/PhotonPlusJets_Pt50to80/skimElecTau_3_1_YO3.root',
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/PhotonPlusJets_Pt50to80/skimElecTau_4_1_R6m.root'
)
fileNamesZtoElecTau_PhotonPlusJets_Pt50to80_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/PhotonPlusJets_Pt50to80/skimElecTau_5_1_7JG.root',
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/PhotonPlusJets_Pt50to80/skimElecTau_6_2_Doo.root',
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/PhotonPlusJets_Pt50to80/skimElecTau_7_1_7Wi.root'
)
fileNamesZtoElecTau_PhotonPlusJets_Pt50to80_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/PhotonPlusJets_Pt50to80/skimElecTau_8_1_1He.root',
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/PhotonPlusJets_Pt50to80/skimElecTau_9_1_Oqq.root'
)

corrFactorZtoElecTau_PhotonPlusJets_Pt15to30 = float(1.0)
corrFactorZtoElecTau_PhotonPlusJets_Pt30to50 = float(1.0)
corrFactorZtoElecTau_PhotonPlusJets_Pt50to80 = float(1.0)

intLumiZtoElecTau_PhotonPlusJets_Pt15to30 = float(5.98)
intLumiZtoElecTau_PhotonPlusJets_Pt30to50 = float(61.4)
intLumiZtoElecTau_PhotonPlusJets_Pt50to80 = float(376.4)

genPhaseSpaceCutZtoElecTau_PhotonPlusJets_Pt15to30 = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

genPhaseSpaceCutZtoElecTau_PhotonPlusJets_Pt30to50 = copy.deepcopy(genPhaseSpaceCutZtoElecTau_PhotonPlusJets_Pt15to30)
genPhaseSpaceCutZtoElecTau_PhotonPlusJets_Pt50to80 = copy.deepcopy(genPhaseSpaceCutZtoElecTau_PhotonPlusJets_Pt15to30)

plotsOutputFileNameZtoElecTau_PhotonPlusJets_Pt15to30 = cms.string('plotsZtoElecTau_PhotonPlusJets_Pt15to30_partXX.root')
plotsOutputFileNameZtoElecTau_PhotonPlusJets_Pt30to50 = cms.string('plotsZtoElecTau_PhotonPlusJets_Pt30to50_partXX.root')
plotsOutputFileNameZtoElecTau_PhotonPlusJets_Pt50to80 = cms.string('plotsZtoElecTau_PhotonPlusJets_Pt50to80_partXX.root')

patTupleOutputFileNameZtoElecTau_PhotonPlusJets_Pt15to30 = cms.untracked.string('patTupleZtoElecTau_PhotonPlusJets_Pt15to30_partXX.root')
patTupleOutputFileNameZtoElecTau_PhotonPlusJets_Pt30to50 = cms.untracked.string('patTupleZtoElecTau_PhotonPlusJets_Pt30to50_partXX.root')
patTupleOutputFileNameZtoElecTau_PhotonPlusJets_Pt50to80 = cms.untracked.string('patTupleZtoElecTau_PhotonPlusJets_Pt50to80_partXX.root')

#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
#  W samples
# 
intLumiZtoElecTau_WtoENu = float(646.2)
corrFactorZtoElecTau_WtoENu = float(1.010)

patTupleOutputFileNameZtoElecTau_WtoENu = cms.untracked.string('patTupleZtoElecTau_WtoENu_partXX.root')

plotsOutputFileNameZtoElecTau_WtoENu = cms.string('plotsZtoElecTau_WtoENu_partXX.root')

genPhaseSpaceCutZtoElecTau_WtoENu = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

fileNamesZtoElecTau_WtoENu_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_1_2_pJ7.root'
)
fileNamesZtoElecTau_WtoENu_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_10_3_Xqc.root'
)
fileNamesZtoElecTau_WtoENu_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_11_2_Y6a.root'
)
fileNamesZtoElecTau_WtoENu_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_12_3_rtm.root'
)
fileNamesZtoElecTau_WtoENu_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_13_3_IvJ.root'
)
fileNamesZtoElecTau_WtoENu_part06 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_14_3_EVA.root'
)
fileNamesZtoElecTau_WtoENu_part07 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_15_2_JKV.root'
)
fileNamesZtoElecTau_WtoENu_part08 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_16_2_DNO.root'
)
fileNamesZtoElecTau_WtoENu_part09 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_17_2_P7F.root'
)
fileNamesZtoElecTau_WtoENu_part10 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_18_2_4Vv.root'
)
fileNamesZtoElecTau_WtoENu_part11 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_19_3_p9h.root'
)
fileNamesZtoElecTau_WtoENu_part12 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_2_2_UWb.root'
)
fileNamesZtoElecTau_WtoENu_part13 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_20_2_PcL.root'
)
fileNamesZtoElecTau_WtoENu_part14 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_21_3_Zls.root'
)
fileNamesZtoElecTau_WtoENu_part15 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_22_3_2gr.root'
)
fileNamesZtoElecTau_WtoENu_part16 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_24_3_iBZ.root'
)
fileNamesZtoElecTau_WtoENu_part17 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_25_2_PAk.root'
)
fileNamesZtoElecTau_WtoENu_part18 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_26_3_bjb.root'
)
fileNamesZtoElecTau_WtoENu_part19 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_27_3_Okc.root'
)
fileNamesZtoElecTau_WtoENu_part20 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_28_3_te8.root'
)
fileNamesZtoElecTau_WtoENu_part21 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_29_2_zOm.root'
)
fileNamesZtoElecTau_WtoENu_part22 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_3_2_TsL.root'
)
fileNamesZtoElecTau_WtoENu_part23 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_30_3_yw7.root'
)
fileNamesZtoElecTau_WtoENu_part24 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_31_3_mQI.root'
)
fileNamesZtoElecTau_WtoENu_part25 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_34_2_8S0.root'
)
fileNamesZtoElecTau_WtoENu_part26 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_35_3_i7U.root'
)
fileNamesZtoElecTau_WtoENu_part27 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_37_3_QfN.root'
)
fileNamesZtoElecTau_WtoENu_part28 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_38_3_ehB.root'
)
fileNamesZtoElecTau_WtoENu_part29 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_39_2_jZ1.root'
)
fileNamesZtoElecTau_WtoENu_part30 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_40_3_B6g.root'
)
fileNamesZtoElecTau_WtoENu_part31 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_41_2_bP0.root'
)
fileNamesZtoElecTau_WtoENu_part32 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_42_2_8mv.root'
)
fileNamesZtoElecTau_WtoENu_part33 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_43_2_49K.root'
)
fileNamesZtoElecTau_WtoENu_part34 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_44_2_1TE.root'
)
fileNamesZtoElecTau_WtoENu_part35 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_45_3_Q4m.root'
)
fileNamesZtoElecTau_WtoENu_part36 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_48_2_TvT.root'
)
fileNamesZtoElecTau_WtoENu_part37 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_49_1_cJ4.root'
)
fileNamesZtoElecTau_WtoENu_part38 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_5_3_nVf.root'
)
fileNamesZtoElecTau_WtoENu_part39 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_50_1_SUq.root'
)
fileNamesZtoElecTau_WtoENu_part40 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_51_1_26t.root'
)
fileNamesZtoElecTau_WtoENu_part41 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_52_1_p0o.root'
)
fileNamesZtoElecTau_WtoENu_part42 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_53_1_nlb.root'
)
fileNamesZtoElecTau_WtoENu_part43 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_54_1_t0W.root'
)
fileNamesZtoElecTau_WtoENu_part44 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_55_1_uqY.root'
)
fileNamesZtoElecTau_WtoENu_part45 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_56_1_99L.root'
)
fileNamesZtoElecTau_WtoENu_part46 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_57_1_0nG.root'
)
fileNamesZtoElecTau_WtoENu_part47 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_58_1_GLQ.root'
)
fileNamesZtoElecTau_WtoENu_part48 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_59_1_MIu.root'
)
fileNamesZtoElecTau_WtoENu_part49 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_60_1_fC6.root'
)
fileNamesZtoElecTau_WtoENu_part50 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_61_1_nJ8.root'
)
fileNamesZtoElecTau_WtoENu_part51 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_62_1_ZqM.root'
)
fileNamesZtoElecTau_WtoENu_part52 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_63_1_lei.root'
)
fileNamesZtoElecTau_WtoENu_part53 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_64_1_ucL.root'
)
fileNamesZtoElecTau_WtoENu_part54 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_65_1_XRM.root'
)
fileNamesZtoElecTau_WtoENu_part55 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_66_1_fvD.root'
)
fileNamesZtoElecTau_WtoENu_part56 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_67_1_jse.root'
)
fileNamesZtoElecTau_WtoENu_part57 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_68_1_xv0.root'
)
fileNamesZtoElecTau_WtoENu_part58 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_69_1_MyL.root'
)
fileNamesZtoElecTau_WtoENu_part59 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_70_1_WgJ.root'
)
fileNamesZtoElecTau_WtoENu_part60 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_71_1_EDD.root'
)
fileNamesZtoElecTau_WtoENu_part61 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_72_1_mO9.root'
)
fileNamesZtoElecTau_WtoENu_part62 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_73_1_imI.root'
)
fileNamesZtoElecTau_WtoENu_part63 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_74_1_RmG.root'
)
fileNamesZtoElecTau_WtoENu_part64 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_75_1_3ow.root'
)
fileNamesZtoElecTau_WtoENu_part65 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_76_1_WlW.root'
)
fileNamesZtoElecTau_WtoENu_part66 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_77_1_gfn.root'
)
fileNamesZtoElecTau_WtoENu_part67 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_78_1_feC.root'
)
fileNamesZtoElecTau_WtoENu_part68 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_79_1_T8W.root'
)
fileNamesZtoElecTau_WtoENu_part69 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_8_2_Gz6.root'
)
fileNamesZtoElecTau_WtoENu_part70 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_80_1_AXW.root'
)
fileNamesZtoElecTau_WtoENu_part71 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_81_1_6pL.root'
)
fileNamesZtoElecTau_WtoENu_part72 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_82_2_DiP.root'
)
fileNamesZtoElecTau_WtoENu_part73 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_9_2_vn6.root'
)
fileNamesZtoElecTau_WtoENu_part74 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_23_3_SDD.root'
)
fileNamesZtoElecTau_WtoENu_part75 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_32_3_xpH.root'
)
fileNamesZtoElecTau_WtoENu_part76 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_33_3_oCT.root'
)
fileNamesZtoElecTau_WtoENu_part77 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_46_3_KZq.root'
)
fileNamesZtoElecTau_WtoENu_part78 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_47_3_EX0.root'
)
fileNamesZtoElecTau_WtoENu_part79 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_6_3_wCt.root'
)
fileNamesZtoElecTau_WtoENu_part80 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoENu/skimElecTau_7_3_iln.root'
)

intLumiZtoElecTau_WtoTauNu = float(661)
corrFactorZtoElecTau_WtoTauNu = float(1.)

patTupleOutputFileNameZtoElecTau_WtoTauNu = cms.untracked.string('patTupleZtoElecTau_WtoTauNu_partXX.root')

plotsOutputFileNameZtoElecTau_WtoTauNu = cms.string('plotsZtoElecTau_WtoTauNu_partXX.root')

genPhaseSpaceCutZtoElecTau_WtoTauNu = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

fileNamesZtoElecTau_WtoTauNu_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoTauNu/skimElecTau_1_2_59K.root'
)
fileNamesZtoElecTau_WtoTauNu_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoTauNu/skimElecTau_10_4_tb4.root'
)
fileNamesZtoElecTau_WtoTauNu_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoTauNu/skimElecTau_11_1_o5c.root'
)
fileNamesZtoElecTau_WtoTauNu_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoTauNu/skimElecTau_12_1_SXi.root'
)
fileNamesZtoElecTau_WtoTauNu_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoTauNu/skimElecTau_13_1_qsS.root'
)
fileNamesZtoElecTau_WtoTauNu_part06 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoTauNu/skimElecTau_14_4_zbM.root'
)
fileNamesZtoElecTau_WtoTauNu_part07 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoTauNu/skimElecTau_15_4_5nb.root'
)
fileNamesZtoElecTau_WtoTauNu_part08 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoTauNu/skimElecTau_16_4_2XR.root'
)
fileNamesZtoElecTau_WtoTauNu_part09 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoTauNu/skimElecTau_17_1_NDz.root'
)
fileNamesZtoElecTau_WtoTauNu_part10 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoTauNu/skimElecTau_18_4_Sf4.root'
)
fileNamesZtoElecTau_WtoTauNu_part11 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoTauNu/skimElecTau_19_4_3tA.root'
)
fileNamesZtoElecTau_WtoTauNu_part12 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoTauNu/skimElecTau_2_1_nno.root'
)
fileNamesZtoElecTau_WtoTauNu_part13 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoTauNu/skimElecTau_20_2_Te8.root'
)
fileNamesZtoElecTau_WtoTauNu_part14 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoTauNu/skimElecTau_21_4_ZgJ.root'
)
fileNamesZtoElecTau_WtoTauNu_part15 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoTauNu/skimElecTau_22_4_4ty.root'
)
fileNamesZtoElecTau_WtoTauNu_part16 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoTauNu/skimElecTau_23_4_0WO.root'
)
fileNamesZtoElecTau_WtoTauNu_part17 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoTauNu/skimElecTau_24_2_A7o.root'
)
fileNamesZtoElecTau_WtoTauNu_part18 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoTauNu/skimElecTau_25_1_JPJ.root'
)
fileNamesZtoElecTau_WtoTauNu_part19 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoTauNu/skimElecTau_26_4_rRZ.root'
)
fileNamesZtoElecTau_WtoTauNu_part20 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoTauNu/skimElecTau_27_2_ucI.root'
)
fileNamesZtoElecTau_WtoTauNu_part21 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoTauNu/skimElecTau_28_4_qb1.root'
)
fileNamesZtoElecTau_WtoTauNu_part22 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoTauNu/skimElecTau_29_3_bc6.root'
)
fileNamesZtoElecTau_WtoTauNu_part23 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoTauNu/skimElecTau_3_2_2jx.root'
)
fileNamesZtoElecTau_WtoTauNu_part24 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoTauNu/skimElecTau_30_1_yE1.root'
)
fileNamesZtoElecTau_WtoTauNu_part25 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoTauNu/skimElecTau_31_1_bVu.root'
)
fileNamesZtoElecTau_WtoTauNu_part26 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoTauNu/skimElecTau_32_1_nf5.root'
)
fileNamesZtoElecTau_WtoTauNu_part27 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoTauNu/skimElecTau_4_1_X6R.root'
)
fileNamesZtoElecTau_WtoTauNu_part28 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoTauNu/skimElecTau_5_2_8BI.root'
)
fileNamesZtoElecTau_WtoTauNu_part29 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoTauNu/skimElecTau_6_4_BYn.root'
)
fileNamesZtoElecTau_WtoTauNu_part30 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoTauNu/skimElecTau_7_1_GHM.root'
)
fileNamesZtoElecTau_WtoTauNu_part31 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoTauNu/skimElecTau_8_2_ctV.root'
)
fileNamesZtoElecTau_WtoTauNu_part32 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10//WtoTauNu/skimElecTau_9_4_OTB.root'
)
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
#
#  TTbar pythia sample
#  
#
intLumiZtoElecTau_TTbar = float(11697)
corrFactorZtoElecTau_TTbar = float(1.)

genPhaseSpaceCutZtoElecTau_TTbar = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

patTupleOutputFileNameZtoElecTau_TTbar = cms.untracked.string('patTupleZtoElecTau_TTbar_partXX.root')

plotsOutputFileNameZtoElecTau_TTbar = cms.string('plotsZtoElecTau_TTbar_partXX.root')


fileNamesZtoElecTau_TTbar_part01 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_1_2_wmK.root'
)
fileNamesZtoElecTau_TTbar_part02 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_10_2_KCl.root'
)
fileNamesZtoElecTau_TTbar_part03 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_11_2_41C.root'
)
fileNamesZtoElecTau_TTbar_part04 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_12_2_rox.root'
)
fileNamesZtoElecTau_TTbar_part05 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_13_2_I8a.root'
)
fileNamesZtoElecTau_TTbar_part06 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_14_2_iAy.root'
)
fileNamesZtoElecTau_TTbar_part07 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_15_2_aHD.root'
)
fileNamesZtoElecTau_TTbar_part08 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_16_2_gSE.root'
)
fileNamesZtoElecTau_TTbar_part09 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_17_2_5oR.root'
)
fileNamesZtoElecTau_TTbar_part10 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_18_2_BXw.root'
)
fileNamesZtoElecTau_TTbar_part11 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_19_2_Eg1.root'
)
fileNamesZtoElecTau_TTbar_part12 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_2_2_9jo.root'
)
fileNamesZtoElecTau_TTbar_part13 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_20_2_bWi.root'
)
fileNamesZtoElecTau_TTbar_part14 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_21_2_zJL.root'
)
fileNamesZtoElecTau_TTbar_part15 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_22_2_6B2.root'
)
fileNamesZtoElecTau_TTbar_part16 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_23_2_wKG.root'
)
fileNamesZtoElecTau_TTbar_part17 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_24_2_Erk.root'
)
fileNamesZtoElecTau_TTbar_part18 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_25_4_IML.root'
)
fileNamesZtoElecTau_TTbar_part19 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_26_2_B1C.root'
)
fileNamesZtoElecTau_TTbar_part20 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_27_2_QQw.root'
)
fileNamesZtoElecTau_TTbar_part21 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_28_2_dXZ.root'
)
fileNamesZtoElecTau_TTbar_part22 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_29_2_5Q1.root'
)
fileNamesZtoElecTau_TTbar_part23 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_3_2_lXL.root'
)
fileNamesZtoElecTau_TTbar_part24 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_30_2_KcL.root'
)
fileNamesZtoElecTau_TTbar_part25 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_31_2_tmR.root'
)
fileNamesZtoElecTau_TTbar_part26 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_32_2_wqo.root'
)
fileNamesZtoElecTau_TTbar_part27 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_33_2_RrW.root'
)
fileNamesZtoElecTau_TTbar_part28 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_34_2_pDw.root'
)
fileNamesZtoElecTau_TTbar_part29 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_35_2_cnb.root'
)
fileNamesZtoElecTau_TTbar_part30 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_36_2_ndd.root'
)
fileNamesZtoElecTau_TTbar_part31 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_37_2_jC7.root'
)
fileNamesZtoElecTau_TTbar_part32 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_38_2_ecB.root'
)
fileNamesZtoElecTau_TTbar_part33 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_39_2_CLy.root'
)
fileNamesZtoElecTau_TTbar_part34 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_4_2_lDZ.root'
)
fileNamesZtoElecTau_TTbar_part35 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_40_2_seX.root'
)
fileNamesZtoElecTau_TTbar_part36 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_41_2_YKe.root'
)
fileNamesZtoElecTau_TTbar_part37 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_42_2_kQt.root'
)
fileNamesZtoElecTau_TTbar_part38 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_43_2_T9D.root'
)
fileNamesZtoElecTau_TTbar_part39 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_44_2_9PG.root'
)
fileNamesZtoElecTau_TTbar_part40 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_45_2_YsZ.root'
)
fileNamesZtoElecTau_TTbar_part41 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_46_2_OPn.root'
)
fileNamesZtoElecTau_TTbar_part42 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_47_3_7N1.root'
)
fileNamesZtoElecTau_TTbar_part43 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_48_2_bTi.root'
)
fileNamesZtoElecTau_TTbar_part44 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_49_2_IBR.root'
)
fileNamesZtoElecTau_TTbar_part45 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_5_2_k5u.root'
)
fileNamesZtoElecTau_TTbar_part46 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_50_2_m2L.root'
)
fileNamesZtoElecTau_TTbar_part47 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_51_2_3Wu.root'
)
fileNamesZtoElecTau_TTbar_part48 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_52_2_1u8.root'
)
fileNamesZtoElecTau_TTbar_part49 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_53_2_u1o.root'
)
fileNamesZtoElecTau_TTbar_part50 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_54_2_tzF.root'
)
fileNamesZtoElecTau_TTbar_part51 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_55_2_l1H.root'
)
fileNamesZtoElecTau_TTbar_part52 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_6_2_nbY.root'
)
fileNamesZtoElecTau_TTbar_part53 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_7_2_6qx.root'
)
fileNamesZtoElecTau_TTbar_part54 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_8_2_725.root'
)
fileNamesZtoElecTau_TTbar_part55 = cms.untracked.vstring(
	'rfio:/castor/cern.ch//user/j/jkolb/eTauSkims/fall10/TTbar/skimElecTau_9_2_Yd2.root'
)
#--------------------------------------------------------------------------------

