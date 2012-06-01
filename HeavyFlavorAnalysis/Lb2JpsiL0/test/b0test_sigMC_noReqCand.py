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
#	'/store/user/meier_f1/B0ToJpsiKs_7TeV_Pythia_EvtGen_frmeier_g0007/B0ToJpsiKs_7TeV_Pythia_EvtGen_frmeier_g0010_GENSIMRECO/3d31ade7f81abde7fe01a8379331e53e/PYTHIA6_inclB0toJpsiMuMu_7TeV_cff_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_root_RAW2DIGI_L1Reco_RECO_6_1_O7T.root'
#	'/store/user/frmeier/lambda/datasets/PYTHIA6_B0ToJpsiKs_7TeV_EvtGen_Frank_cff_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_root_RAW2DIGI_L1Reco_RECO_9_2_JTQ.root'
	'/store/user/frmeier/datasets/PYTHIA6_exclLambdaBtoJpsiMuMu_7TeV_cff_py_GEN_SIM_DIGI_L1_DIGI2RAW_root_HLT_quarkoniumRAW2DIGI_L1Reco_RECO_103_1_adF.root'
    )
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )


# ----------------------------------------------------------------------
rootFileName = "bmm-lambda-B0ToPsiMuMu_noReqCand.root"

process.tree = cms.EDAnalyzer(
    "HFTree",
    verbose      = cms.untracked.int32(0),
    requireCand  =  cms.untracked.bool(False),
    fileName     = cms.untracked.string(rootFileName)
    )

# ----------------------------------------------------------------------
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("HeavyFlavorAnalysis.Bs2MuMu.HFMCTruth_cff")
process.load("HeavyFlavorAnalysis.Bs2MuMu.HFRecoStuff_cff")
#process.load("HeavyFlavorAnalysis.Bs2MuMu.HFTruthCandidates_cff")
#process.load("HeavyFlavorAnalysis.Bs2MuMu.HFBmm_cff")
process.load("HeavyFlavorAnalysis.Bs2MuMu.HFLambdas_cff")
# ----------------------------------------------------------------------
process.genParticlePlusGEANT = cms.EDProducer("GenPlusSimParticleProducer",
        src           = cms.InputTag("g4SimHits"), # use "famosSimHits" for FAMOS
        setStatus     = cms.int32(8),             # set status = 8 for GEANT GPs
        filter        = cms.vstring("pt > 0.0"),  # just for testing (optional)
        genParticles   = cms.InputTag("genParticles") # original genParticle list
)
process.genDump.generatorCandidates = cms.untracked.string('genParticlePlusGEANT')

# ----------------------------------------------------------------------
# filter events with exactly my decay
process.decayfilter = cms.EDFilter("HFGenFilter")


# ----------------------------------------------------------------------
process.p = cms.Path(
    process.genParticlePlusGEANT*
    process.MCTruthSequence*
    process.recoStuffSequence*
#    process.BmmSequence*
    process.decayfilter*
    process.HFLambdasSequence*
#    process.truthB2JpsiSequence*
#    process.truthSignalsSequence*
    process.tree
)
