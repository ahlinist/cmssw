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

   '/store/data/Run2011A/MuOnia/AOD/PromptReco-v4/000/167/898/F24D11E7-41A3-E011-8B9C-BCAEC5329708.root',
   '/store/data/Run2011A/MuOnia/AOD/PromptReco-v4/000/166/763/BA40C1BB-1B94-E011-8F3C-0030487C6A66.root'

    )
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


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
process.load("HeavyFlavorAnalysis.Bs2MuMu.HFLambdas_cff")
process.load("HeavyFlavorAnalysis.Bs2MuMu.HFPhysicsDeclared_cff")
process.load("HeavyFlavorAnalysis.Bs2MuMu.HFSkipEvents_cff")

process.runlsevtfilter = cms.EDFilter("HFRunLsEvtFilter")
process.runlsevtfilter.infile = cms.FileInPath("HeavyFlavorAnalysis/Lb2JpsiL0/test/runlsevtFilterText.txt")

# ----------------------------------------------------------------------
process.p = cms.Path(
    process.skipEvents*
    process.runlsevtfilter*
    process.recoStuffSequence*
#    process.BmmSequence*
    process.HFLambdasSequence*
    process.tree
)
