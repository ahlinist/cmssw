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
process.GlobalTag.globaltag = "GR_P_V22::All"

# ----------------------------------------------------------------------
process.source = cms.Source(
    "PoolSource", 
    fileNames = cms.untracked.vstring(
#	 '/store/data/Run2011A/MuOnia/AOD/PromptReco-v4/000/166/021/524336D3-DE8B-E011-88A0-0030487D0D3A.root',
#	 '/store/data/Run2011A/MuOnia/AOD/PromptReco-v4/000/165/999/C654DA41-838B-E011-9596-000423D996C8.root',
#   '/store/data/Run2011A/MuOnia/AOD/PromptReco-v4/000/165/993/FC0DA45F-7A8B-E011-ABB7-0019B9F705A3.root',
#   '/store/data/Run2011A/MuOnia/AOD/PromptReco-v4/000/165/993/FAF47820-288B-E011-97A2-0019B9F730D2.root'

#       '/store/data/Run2011A/MuOnia/AOD/PromptReco-v5/000/172/619/CADB8579-12C0-E011-BB10-BCAEC518FF89.root',
#       '/store/data/Run2011A/MuOnia/AOD/PromptReco-v5/000/172/617/38692094-0CC0-E011-8599-BCAEC5364C42.root',
#       '/store/data/Run2011A/MuOnia/AOD/PromptReco-v5/000/172/615/3EFEC0FD-07C0-E011-A6A0-BCAEC53296F8.root'

#'/store/data/Run2011B/MuOnia/AOD/PromptReco-v1/000/175/834/C26E4C6A-82DB-E011-A8C5-001D09F2932B.root'
#        '/pnfs/psi.ch/cms/trivcat/store/user/frmeier/datasets/PYTHIA6_exclLambdaBtoJpsiMuMu_7TeV_cff_py_GEN_SIM_DIGI_L1_DIGI2RAW_root_HLT_quarkoniumRAW2DIGI_L1Reco_RECO_103_1_adF.root'
	'file:/scratch/frmeier/pickevents_l0211.root',
	'file:/scratch/frmeier/pickevents_l0211001.root',
	'file:/scratch/frmeier/pickevents_l0211002.root'
    )
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(500) )


# ----------------------------------------------------------------------
rootFileName = "bmm-lambda-MuOnia-Run2011A-PromptReco-v6.root"

process.tree = cms.EDAnalyzer(
    "HFTree",
    verbose      = cms.untracked.int32(0),
    requireCand  =  cms.untracked.bool(True),
    fileName     = cms.untracked.string(rootFileName)
    )

# ----------------------------------------------------------------------
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("HeavyFlavorAnalysis.Bs2MuMu.HFRecoStuff_cff")
#process.load("HeavyFlavorAnalysis.Bs2MuMu.HFBmm_cff")
process.load("HeavyFlavorAnalysis.Bs2MuMu.HFLambdas_tight_cff")
process.load("HeavyFlavorAnalysis.Bs2MuMu.HFPhysicsDeclared_cff")
process.load("HeavyFlavorAnalysis.Bs2MuMu.HFSkipEvents_cff")

# ----------------------------------------------------------------------
process.p = cms.Path(
    process.skipEvents*
    process.recoStuffSequence*
#    process.BmmSequence*
    process.HFLambdasSequence*
    process.tree
)
