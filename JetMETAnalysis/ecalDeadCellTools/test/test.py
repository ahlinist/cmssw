# Auto generated configuration file
# using:
# Revision: 1.149
# Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v
# with command line options: reco -s RAW2DIGI,RECO --conditions FrontierCoitions_GlobalTag,MC_31X_V9::All --eventcontent=FEVT --no_output --no_exec
import FWCore.ParameterSet.Config as cms

#whichType = "RECO"
#whichType = "TWOFILEDATA"
whichType = "TWOFILEMC"

#For the combined filter, use filterSelector to choose which filter to be used
# 0 : TP||BE   1 : TP   2 : BE
filterSelector = 0

process = cms.Process('Test')

# import of standard configurations
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/StandardSequences/MixingNoPileUp_cff')
process.load('Configuration/StandardSequences/GeometryExtended_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load('Configuration/StandardSequences/RawToDigi_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/StandardSequences/EndOfProcess_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration/EventContent/EventContent_cff')

process.configurationMetadata = cms.untracked.PSet(
   version = cms.untracked.string('$Revision: 1.3 $'),
   annotation = cms.untracked.string('reco nevts:1'),
   name = cms.untracked.string('PyReleaseValidation')
)
process.maxEvents = cms.untracked.PSet(
   input = cms.untracked.int32(4000)
)
process.options = cms.untracked.PSet(
   Rethrow = cms.untracked.vstring('ProductNotFound')
)

process.MessageLogger.cerr.FwkReport.reportEvery = 1
process.MessageLogger.cerr.default.limit = 100

# summary
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# Input source
process.source = cms.Source("PoolSource",
   fileNames = cms.untracked.vstring(
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_9_1_LgQ.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_99_1_Rl5.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_98_1_UXV.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_97_1_Dcf.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_96_1_lid.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_95_1_XzP.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_94_1_wFY.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_93_1_1kB.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_92_1_1qF.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_91_1_hwO.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_90_1_nLD.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_8_1_H5B.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_89_1_RVi.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_88_1_VJo.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_87_1_WOI.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_86_1_ube.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_85_1_loz.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_84_1_ZM4.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_83_1_dYo.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_82_1_znD.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_81_1_SoS.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_80_1_n6w.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_7_1_RS1.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_79_1_yBp.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_78_1_nJv.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_77_1_3s9.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_76_1_7JL.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_75_1_PZ9.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_74_1_yWS.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_73_1_ZEx.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_72_1_XhQ.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_71_1_a0w.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_70_1_qr0.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_6_1_XHU.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_69_1_LOQ.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_68_1_bHO.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_67_1_iTt.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_66_1_ocO.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_65_1_nYp.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_64_1_1NW.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_63_1_Vfg.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_62_1_8rc.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_61_1_j9K.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_60_1_0oo.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_5_1_Huc.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_59_1_3UD.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_58_1_7yd.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_57_1_s47.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_56_1_T9k.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_55_1_rDk.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_54_1_ygj.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_53_1_oEW.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_52_1_APT.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_51_1_sPQ.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_50_1_9Oa.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_4_1_IzB.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_49_1_sxs.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_48_1_ANi.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_47_1_KNe.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_46_1_spR.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_45_1_Seu.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_455_1_XZC.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_454_1_J1k.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_453_1_GJ3.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_452_1_ii2.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_451_1_atY.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_450_1_ilt.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_44_1_ySY.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_449_1_5tm.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_448_1_MRT.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_447_1_qRp.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_446_1_hhV.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_445_1_JxP.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_444_1_pWc.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_443_1_S1D.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_442_1_hdj.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_441_1_jvY.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_440_1_gN3.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_43_1_X4i.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_439_1_gKq.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_438_1_v3z.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_437_1_vHQ.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_436_1_NTJ.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_435_1_19R.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_434_1_n4T.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_433_1_9rE.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_432_1_1mg.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_431_1_M93.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_430_1_Eq2.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_42_1_8Gn.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_429_1_Jgg.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_428_1_kgH.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_427_1_2Dy.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_426_1_VA9.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_425_1_pdX.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_424_1_Uvo.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_423_1_7ZW.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_422_1_TCk.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_421_1_lwI.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_420_1_gox.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_41_1_VmQ.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_419_1_zR8.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_418_1_tsU.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_417_1_lQM.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_416_1_0NQ.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_415_1_aTb.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_414_1_3EG.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_413_1_PqD.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_412_1_Xfo.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_411_1_h9U.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_410_1_Wxw.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_40_1_TTR.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_409_1_afZ.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_408_1_cgX.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_407_1_oss.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_406_1_xqf.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_405_1_hW3.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_404_1_L8i.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_403_1_HBw.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_402_1_DhO.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_401_1_PUv.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_400_1_a3d.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_3_1_VCb.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_39_1_Xdx.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_399_1_Jx4.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_398_1_utT.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_397_1_0ms.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_396_1_e5X.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_395_1_iIL.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_394_1_1bA.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_393_1_Qol.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_392_1_0cC.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_391_1_c8r.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_390_1_oSg.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_38_1_b7H.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_389_1_LOQ.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_388_1_VHx.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_387_1_Pmg.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_386_1_Nht.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_385_1_LFJ.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_384_1_UfR.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_383_1_kzF.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_382_1_nCI.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_381_1_YzN.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_380_1_HzF.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_37_1_QpE.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_379_1_D8v.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_378_1_yo5.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_377_1_kqq.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_376_1_J2P.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_375_1_3b6.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_374_1_Bda.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_373_1_gxZ.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_372_1_mDt.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_371_1_nS6.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_370_1_agt.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_36_1_3El.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_369_1_Fto.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_368_1_rMg.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_367_1_Pt9.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_366_1_rl0.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_365_1_6Ap.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_364_1_lAa.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_363_1_rkj.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_362_1_CYi.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_361_1_LEr.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_360_1_lQ1.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_35_1_Prd.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_359_1_6H0.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_358_1_paR.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_357_1_wLi.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_356_1_vgq.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_355_1_Zw6.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_354_1_jmt.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_353_1_t1a.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_352_1_vy3.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_351_1_FAV.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_350_1_RO7.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_34_1_EB9.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_349_1_slN.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_348_1_Nkl.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_347_1_R9P.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_346_1_9lx.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_345_1_03I.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_344_1_h7t.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_343_1_cr7.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_342_1_c6F.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_341_1_KC2.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_340_1_pAS.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_33_1_ZZ1.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_339_1_XmI.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_338_1_8nn.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_337_1_5Zi.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_336_1_xwi.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_335_1_Vq1.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_334_1_qve.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_333_1_V1l.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_332_1_bLd.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_331_1_i1J.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_330_1_nfm.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_32_1_h0k.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_329_1_hwS.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_328_1_Izr.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_327_1_pcS.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_326_1_abI.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_325_1_f26.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_324_1_BPz.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_323_1_i0t.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_322_1_LkI.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_321_1_Go6.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_320_1_OD3.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_31_1_1xT.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_319_1_Mus.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_318_1_1re.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_317_1_w9O.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_316_1_Cxc.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_315_1_H0t.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_314_1_FhL.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_313_1_Kbe.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_312_1_k4J.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_311_1_BkD.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_310_1_29S.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_30_1_kWG.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_309_1_QyQ.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_308_1_Ks4.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_307_1_k5D.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_306_1_1iT.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_305_1_SnS.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_304_1_iIs.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_303_1_moP.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_302_1_5tM.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_301_1_b8d.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_300_1_u1L.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_2_1_kQt.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_29_1_HVZ.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_299_1_xz0.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_298_1_c3t.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_297_1_JjT.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_296_1_0ey.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_295_1_snV.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_294_1_dPA.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_293_1_U8S.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_292_1_Agp.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_291_1_Zwg.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_290_1_BEt.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_28_1_Ads.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_289_1_Gl5.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_288_1_QFw.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_287_1_pDp.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_286_1_SI1.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_285_1_xE7.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_284_1_eTU.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_283_1_yLQ.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_282_1_VXs.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/redigi-madgraph-QCD1000toInf-MC_38Y-RECO-382p1/e79a9e4429b9d6c1f7093685e0243736/rereco_ECALRecovery_281_1_9CT.root'
   ),
#   noEventSort = cms.untracked.bool(False),
)

if whichType == "TWOFILEDATA":
   process.source.secondaryFileNames = cms.untracked.vstring(

   )
elif whichType == "TWOFILEMC":
   process.source.secondaryFileNames = cms.untracked.vstring(
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_9_1_6Iv.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_8_1_qiQ.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_7_1_ncr.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_6_1_UC1.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_5_1_3an.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_51_1_Ayo.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_50_1_WRs.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_4_1_Fcx.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_49_1_fye.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_48_1_NpY.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_47_1_wA7.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_46_1_Wrq.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_45_2_94A.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_43_1_w8B.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_42_1_iA0.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_41_1_79v.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_40_1_vUP.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_3_1_8uU.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_39_1_0ni.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_38_1_vL7.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_37_1_jcE.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_36_1_2eJ.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_35_1_gOr.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_34_1_q1f.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_33_1_2qQ.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_32_1_yPU.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_31_1_taK.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_30_1_t4O.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_2_1_zVs.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_29_1_6K3.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_28_1_heS.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_27_1_o1B.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_26_1_gDr.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_25_1_23a.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_24_1_lHL.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_23_1_5Qe.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_22_1_t5Z.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_21_1_1f4.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_20_1_r7T.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_1_1_mIx.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_19_1_t93.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_18_1_Z6X.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_17_1_244.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_16_1_LXp.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_15_1_1Pn.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_14_1_wSX.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_13_1_Vd6.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_12_1_Y3s.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_11_1_ji3.root',
       '/store/user/lhx/QCD_Pt1000toInf-madgraph/skimRAW-madgraph-QCD1000toInf-ecalMETandMHTtail_v3/678ace6c5bc4724caf84a9e19ebf02b2/catMETandMHTskimsRAW_10_1_kQW.root'
   )


process.GlobalTag.globaltag = 'MC_38Y_V10::All'

#For BE filter
process.load('PhysicsTools.EcalAnomalousEventFilter.ecalanomalouseventfilter_cfi')

process.load("UserCode.EcalDeadCellEventFilter.EcalDeadCellEventFilter_cfi")
process.EcalDeadCellEventFilter.debug = False
process.EcalDeadCellEventFilter.filterSelector = filterSelector
process.EcalDeadCellEventFilter.makeProfileRoot = False
process.EcalDeadCellEventFilter.profileRootName = "deadCellFilterProfile.root"

if whichType == "TWOFILEMC":
   process.EcalDeadCellEventFilter.tpDigiCollection = cms.InputTag("simEcalTriggerPrimitiveDigis")

   process.load("SimCalorimetry.Configuration.ecalDigiSequence_cff")
   process.load("SimGeneral.MixingModule.mixNoPU_cfi")

   process.simEcalUnsuppressedDigis.hitsProducer = cms.string("g4SimHits")
   process.ecalTPsimDigiSeq = cms.Sequence(process.mix*process.simEcalUnsuppressedDigis*process.simEcalTriggerPrimitiveDigis)

elif whichType == "TWOFILEDATA":
   process.EcalDeadCellEventFilter.tpDigiCollection = cms.InputTag("ecalDigis:EcalTriggerPrimitives")

elif whichType == "RECO":
   process.EcalDeadCellEventFilter.tpDigiCollection = cms.InputTag("ecalTPSkim")

if filterSelector ==1:
   process.TPfilterSeq = cms.Sequence(process.EcalDeadCellEventFilter)
else: 
   process.TPfilterSeq = cms.Sequence(process.EcalAnomalousEventFilter*process.EcalDeadCellEventFilter)

process.ecalTPsimDigi_step = cms.Path(process.ecalTPsimDigiSeq)
process.filter_step = cms.Path(process.TPfilterSeq)
process.endjob_step = cms.Path(process.endOfProcess)

if whichType == "TWOFILEMC":
   if filterSelector ==2:
      process.schedule = cms.Schedule(process.filter_step, process.endjob_step)
   else:
      process.schedule = cms.Schedule(process.ecalTPsimDigi_step, process.filter_step, process.endjob_step)
else:
   process.schedule = cms.Schedule(process.filter_step, process.endjob_step)
