import os
import FWCore.ParameterSet.Config as cms

process = cms.Process("HFA")

# ----------------------------------------------------------------------
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )


# ----------------------------------------------------------------------
# -- Database configuration
process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.load("CondCore.DBCommon.CondDBSetup_cfi")

# -- Conditions
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = "START42_V11::All"

# ----------------------------------------------------------------------
process.source = cms.Source(
    "PoolSource", 
    fileNames = cms.untracked.vstring(
#	'/store/mc/Summer11/LambdaBToPsiMuMu_2MuPEtaFilter_7TeV-pythia6-evtgen/GEN-SIM-RECO/PU_S4_START42_V11-v1/0000/FEE34055-708F-E011-8036-0017A4771014.root'
#	'/store/user/frmeier/datasets/FEE34055-708F-E011-8036-0017A4771014.root'
#	'/store/user/frmeier/datasets/EC1D89B1-0380-E011-AA59-E0CB4EA0A939.root'

#	'/store/user/meier_f1/B0ToJpsiKs_7TeV_Pythia_EvtGen_TestFrmeier/B0ToJpsiKs_7TeV_Pythia_EvtGen_TestFrmeier/24459ce5b62880ebb72fa2c41d755869/PYTHIA6_B0ToJpsiKs_7TeV_EvtGen_GEN_SIM_DIGI_1_1_FRq.root'

#'/store/user/kaestli/B0ToJpsiKs_7TeV_Pythia_EvtGen/B0ToJpsiKs_7TeV_Pythia_EvtGen/288c0d8df8c9c406ae16960a05d70d7d/PYTHIA6_B0ToJpsiKs_7TeV_EvtGen_GEN_SIM_DIGI_99_1_XjH.root',
#        '/store/user/kaestli/B0ToJpsiKs_7TeV_Pythia_EvtGen/B0ToJpsiKs_7TeV_Pythia_EvtGen/288c0d8df8c9c406ae16960a05d70d7d/PYTHIA6_B0ToJpsiKs_7TeV_EvtGen_GEN_SIM_DIGI_98_1_c2O.root',
#        '/store/user/kaestli/B0ToJpsiKs_7TeV_Pythia_EvtGen/B0ToJpsiKs_7TeV_Pythia_EvtGen/288c0d8df8c9c406ae16960a05d70d7d/PYTHIA6_B0ToJpsiKs_7TeV_EvtGen_GEN_SIM_DIGI_97_1_qYT.root',
#        '/store/user/kaestli/B0ToJpsiKs_7TeV_Pythia_EvtGen/B0ToJpsiKs_7TeV_Pythia_EvtGen/288c0d8df8c9c406ae16960a05d70d7d/PYTHIA6_B0ToJpsiKs_7TeV_EvtGen_GEN_SIM_DIGI_96_1_ofL.root',
#        '/store/user/kaestli/B0ToJpsiKs_7TeV_Pythia_EvtGen/B0ToJpsiKs_7TeV_Pythia_EvtGen/288c0d8df8c9c406ae16960a05d70d7d/PYTHIA6_B0ToJpsiKs_7TeV_EvtGen_GEN_SIM_DIGI_95_1_lz3.root',
#        '/store/user/kaestli/B0ToJpsiKs_7TeV_Pythia_EvtGen/B0ToJpsiKs_7TeV_Pythia_EvtGen/288c0d8df8c9c406ae16960a05d70d7d/PYTHIA6_B0ToJpsiKs_7TeV_EvtGen_GEN_SIM_DIGI_94_1_Yy4.root',
#        '/store/user/kaestli/B0ToJpsiKs_7TeV_Pythia_EvtGen/B0ToJpsiKs_7TeV_Pythia_EvtGen/288c0d8df8c9c406ae16960a05d70d7d/PYTHIA6_B0ToJpsiKs_7TeV_EvtGen_GEN_SIM_DIGI_93_1_sxU.root',
#        '/store/user/kaestli/B0ToJpsiKs_7TeV_Pythia_EvtGen/B0ToJpsiKs_7TeV_Pythia_EvtGen/288c0d8df8c9c406ae16960a05d70d7d/PYTHIA6_B0ToJpsiKs_7TeV_EvtGen_GEN_SIM_DIGI_92_1_ZUK.root',
#        '/store/user/kaestli/B0ToJpsiKs_7TeV_Pythia_EvtGen/B0ToJpsiKs_7TeV_Pythia_EvtGen/288c0d8df8c9c406ae16960a05d70d7d/PYTHIA6_B0ToJpsiKs_7TeV_EvtGen_GEN_SIM_DIGI_91_1_kUG.root',
#        '/store/user/kaestli/B0ToJpsiKs_7TeV_Pythia_EvtGen/B0ToJpsiKs_7TeV_Pythia_EvtGen/288c0d8df8c9c406ae16960a05d70d7d/PYTHIA6_B0ToJpsiKs_7TeV_EvtGen_GEN_SIM_DIGI_90_1_v7O.root',
#        '/store/user/kaestli/B0ToJpsiKs_7TeV_Pythia_EvtGen/B0ToJpsiKs_7TeV_Pythia_EvtGen/288c0d8df8c9c406ae16960a05d70d7d/PYTHIA6_B0ToJpsiKs_7TeV_EvtGen_GEN_SIM_DIGI_8_1_Snm.root',
#        '/store/user/kaestli/B0ToJpsiKs_7TeV_Pythia_EvtGen/B0ToJpsiKs_7TeV_Pythia_EvtGen/288c0d8df8c9c406ae16960a05d70d7d/PYTHIA6_B0ToJpsiKs_7TeV_EvtGen_GEN_SIM_DIGI_89_1_XPi.root',
#        '/store/user/kaestli/B0ToJpsiKs_7TeV_Pythia_EvtGen/B0ToJpsiKs_7TeV_Pythia_EvtGen/288c0d8df8c9c406ae16960a05d70d7d/PYTHIA6_B0ToJpsiKs_7TeV_EvtGen_GEN_SIM_DIGI_88_1_T7c.root',
#        '/store/user/kaestli/B0ToJpsiKs_7TeV_Pythia_EvtGen/B0ToJpsiKs_7TeV_Pythia_EvtGen/288c0d8df8c9c406ae16960a05d70d7d/PYTHIA6_B0ToJpsiKs_7TeV_EvtGen_GEN_SIM_DIGI_87_1_b1Y.root',
#        '/store/user/kaestli/B0ToJpsiKs_7TeV_Pythia_EvtGen/B0ToJpsiKs_7TeV_Pythia_EvtGen/288c0d8df8c9c406ae16960a05d70d7d/PYTHIA6_B0ToJpsiKs_7TeV_EvtGen_GEN_SIM_DIGI_86_1_YRN.root',
#        '/store/user/kaestli/B0ToJpsiKs_7TeV_Pythia_EvtGen/B0ToJpsiKs_7TeV_Pythia_EvtGen/288c0d8df8c9c406ae16960a05d70d7d/PYTHIA6_B0ToJpsiKs_7TeV_EvtGen_GEN_SIM_DIGI_85_1_G3P.root',
#        '/store/user/kaestli/B0ToJpsiKs_7TeV_Pythia_EvtGen/B0ToJpsiKs_7TeV_Pythia_EvtGen/288c0d8df8c9c406ae16960a05d70d7d/PYTHIA6_B0ToJpsiKs_7TeV_EvtGen_GEN_SIM_DIGI_84_1_hCK.root',
#        '/store/user/kaestli/B0ToJpsiKs_7TeV_Pythia_EvtGen/B0ToJpsiKs_7TeV_Pythia_EvtGen/288c0d8df8c9c406ae16960a05d70d7d/PYTHIA6_B0ToJpsiKs_7TeV_EvtGen_GEN_SIM_DIGI_83_1_N5n.root',
#        '/store/user/kaestli/B0ToJpsiKs_7TeV_Pythia_EvtGen/B0ToJpsiKs_7TeV_Pythia_EvtGen/288c0d8df8c9c406ae16960a05d70d7d/PYTHIA6_B0ToJpsiKs_7TeV_EvtGen_GEN_SIM_DIGI_82_1_klb.root',
#        '/store/user/kaestli/B0ToJpsiKs_7TeV_Pythia_EvtGen/B0ToJpsiKs_7TeV_Pythia_EvtGen/288c0d8df8c9c406ae16960a05d70d7d/PYTHIA6_B0ToJpsiKs_7TeV_EvtGen_GEN_SIM_DIGI_81_1_bFU.root',
#        '/store/user/kaestli/B0ToJpsiKs_7TeV_Pythia_EvtGen/B0ToJpsiKs_7TeV_Pythia_EvtGen/288c0d8df8c9c406ae16960a05d70d7d/PYTHIA6_B0ToJpsiKs_7TeV_EvtGen_GEN_SIM_DIGI_80_1_n0N.root',
#        '/store/user/kaestli/B0ToJpsiKs_7TeV_Pythia_EvtGen/B0ToJpsiKs_7TeV_Pythia_EvtGen/288c0d8df8c9c406ae16960a05d70d7d/PYTHIA6_B0ToJpsiKs_7TeV_EvtGen_GEN_SIM_DIGI_7_1_vTt.root'
#	'/store/user/meier_f1/B0ToJpsiKs_7TeV_Pythia_EvtGen_frmeier_g0007/B0ToJpsiKs_7TeV_Pythia_EvtGen_frmeier_g0010_GENSIMRECO/3d31ade7f81abde7fe01a8379331e53e/PYTHIA6_inclB0toJpsiMuMu_7TeV_cff_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_root_RAW2DIGI_L1Reco_RECO_6_1_O7T.root"
#'/store/user/frmeier/lambda/datasets/PYTHIA6_B0ToJpsiKs_7TeV_EvtGen_Frank_cff_py_GEN_SIM_DIGI_L1_DIGI2RAW_289_1_9Xx.root'
	'/store/user/frmeier/lambda/datasets/PYTHIA6_B0ToJpsiKs_7TeV_EvtGen_Frank_cff_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_289_1_YFL.root'
    )
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )


# ----------------------------------------------------------------------
rootFileName = "bmm-lambda-B0ToPsiMuMu.root"

process.tree = cms.EDAnalyzer(
    "HFTree",
    verbose      = cms.untracked.int32(1),
    requireCand  =  cms.untracked.bool(False),
    fileName     = cms.untracked.string(rootFileName)
    )

# ----------------------------------------------------------------------
process.load("HeavyFlavorAnalysis.Bs2MuMu.HFMCTruth_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("HeavyFlavorAnalysis.Bs2MuMu.HFRecoStuff_cff")
process.load("HeavyFlavorAnalysis.Bs2MuMu.HFTruthCandidates_cff")
#process.load("HeavyFlavorAnalysis.Bs2MuMu.HFBmm_cff")
process.load("HeavyFlavorAnalysis.Bs2MuMu.HFLambdas_cff")
# ----------------------------------------------------------------------
#process.genParticlePlusGEANT = cms.EDProducer("GenPlusSimParticleProducer",
#        src           = cms.InputTag("g4SimHits"), # use "famosSimHits" for FAMOS
#        setStatus     = cms.int32(8),             # set status = 8 for GEANT GPs
#        filter        = cms.vstring("pt > 0.0"),  # just for testing (optional)
#        genParticles   = cms.InputTag("genParticles") # original genParticle list
#)
#process.genDump.generatorCandidates = cms.untracked.string('genParticlePlusGEANT')

# ----------------------------------------------------------------------
# filter events with exactly my decay
process.decayfilter = cms.EDFilter("HFGenFilter")

trackList = "generalTracks"

#process.runlsevtfilter = cms.EDFilter("HFRunLsEvtFilter")
#process.runlsevtfilter.infile = cms.FileInPath("HeavyFlavorAnalysis/Lb2JpsiL0/test/runlsevtFilterText.txt")

# ----------------------------------------------------------------------
process.p = cms.Path(
#    process.runlsevtfilter*
#    process.genParticlePlusGEANT*
    process.MCTruthSequence*
#    process.recoStuffSequence*
#    process.BmmSequence*
#    process.decayfilter*
    process.truthBd2JpsiKsDump*
    process.stuffDump*
#    process.HFLambdasSequence*
#    process.truthB2JpsiSequence*
#    process.truthSignalsSequence*
    process.tree
)
