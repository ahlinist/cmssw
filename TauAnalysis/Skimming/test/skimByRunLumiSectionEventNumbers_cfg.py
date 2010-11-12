import FWCore.ParameterSet.Config as cms

process = cms.Process("skimByRunLumiSectionEventNumbers")

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100
#process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('GR_R_38X_V13A::All')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

castorFilePath = 'rfio:/castor/cern.ch/user/v/veelken/CMSSW_3_8_x/skims/tauCommissioning'

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_10_3_m6H.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_11_1_jF3.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_12_1_0Sb.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_13_1_YCL.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_13_2_X3Q.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_14_1_4RV.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_15_3_3R8.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_16_1_qys.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_17_1_6lu.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_18_1_nay.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_19_1_6My.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_1_1_tcu.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_20_1_n2p.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_21_1_Jke.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_22_1_Epp.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_23_1_POc.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_24_1_ouP.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_25_1_pl8.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_26_1_6Uz.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_27_1_pR8.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_28_1_pSx.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_29_1_r0b.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_2_1_G2i.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_30_1_wzw.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_31_1_Nh7.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_32_1_F28.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_33_1_aJQ.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_34_1_cnw.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_35_1_UHW.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_36_3_Xnr.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_37_1_aNp.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_38_1_Kg3.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_39_1_Tro.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_3_1_1dT.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_40_1_80Q.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_41_1_KE4.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_42_1_S6l.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_43_1_57W.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_44_1_J3E.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_45_1_K9l.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_46_1_5z9.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_47_1_uez.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_48_1_pg8.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_49_1_r1m.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_4_3_Bua.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_51_1_MpI.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_52_1_UmR.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_53_1_CXI.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_54_1_6TY.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_55_1_K0A.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_56_1_0o3.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_57_1_BeO.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_58_3_N6L.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_59_1_Hka.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_5_3_dLp.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_60_1_sxd.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_61_2_IiC.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_62_1_PLk.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_63_1_Rh1.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_64_1_JeQ.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_65_1_4Lc.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_66_1_gBK.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_67_1_tq3.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_67_2_oto.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_68_1_rxX.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_69_1_Ga6.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_6_1_mCO.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_70_1_y2V.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_71_1_AXs.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_72_1_rJe.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_73_1_BrY.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_74_1_uvo.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_76_1_CGB.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_77_1_LKr.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_78_1_w2u.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_79_1_Xym.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_7_1_HdD.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_80_1_MY9.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_81_1_FlR.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_82_1_XuE.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_83_1_8W6.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_84_1_16t.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_85_1_ufT.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_86_1_pZ7.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_87_3_Vhr.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_88_1_WkI.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_89_3_CLs.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_8_1_zR1.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_90_1_bGd.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_91_1_3TP.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_92_1_cyg.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_93_1_JQ0.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_94_1_sck.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_95_1_xs9.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_96_0_FSn.root',
        castorFilePath + '/' + 'goldenZmumuEvents_runs132440to144114_RAW_RECO_9_1_blz.root'
    )                          
)

process.selectEventsByRunLumiSectionEventNumber = cms.EDFilter("RunLumiSectionEventNumberFilter",
    runLumiSectionEventNumberFileName = cms.string(
        '/afs/cern.ch/user/v/veelken/public/selEvents_AHtoMuTau_woBtag_runs132440to145761.txt'
    ),
    separator = cms.string(':')
)

process.skimPath = cms.Path( process.selectEventsByRunLumiSectionEventNumber )

eventSelection = cms.untracked.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('skimPath')
    )
)

process.skimOutputModule = cms.OutputModule("PoolOutputModule",
    eventSelection,                                 
    fileName = cms.untracked.string('selEvents_AHtoMuTau_woBtag_runs132440to145761_RECO.root')
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system
#
# import configuration parameters for submission of jobs to CERN batch system
# (running over skimmed samples stored on CASTOR)
#__from TauAnalysis.Configuration.#recoSampleDefinitionsFileName# import *
#
#__process.source.fileNames = #inputFileNames#
#__process.maxEvents.input = cms.untracked.int32(#maxEvents#)
#__process.selectEventsByRunEventNumber.runEventNumberFileName = cms.string('#runEventNumberFileName#')
#__process.skimOutputModule.fileName = cms.untracked.string('#outputFileName#')
#
#--------------------------------------------------------------------------------

process.o = cms.EndPath( process.skimOutputModule )

