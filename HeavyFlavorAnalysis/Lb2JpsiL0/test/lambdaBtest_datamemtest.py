# ######################################################################
# t3ui02
# /shome/ursl/dana/CMSSW_3_5_6/src/HeavyFlavorAnalysis/Bs2MuMu/test/test-data/goodcoll
# mkPyFiles -f 100413.t1 -t ../dana-XXXX.py -e 200000 -s v05 -r
# ./dana-v05-132605-0000.py with 186277 events
# ######################################################################
import os
import FWCore.ParameterSet.Config as cms

process = cms.Process("HFA")

# ----------------------------------------------------------------------
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.MessageLogger.categories.append('HLTrigReport')
process.MessageLogger.categories.append('L1GtTrigReport')
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
#process.GlobalTag.globaltag = "GR10_P_V9::All"
#process.GlobalTag.globaltag = "FT_R_38X_V14A::All"
process.GlobalTag.globaltag = "GR_P_V17::All"


# ----------------------------------------------------------------------

# Dataset: /Mu/Run2010A-PromptReco-v4/RECO

process.source = cms.Source(
    "PoolSource", 
    fileNames = cms.untracked.vstring(
#	'/store/data/Run2010A/Mu/RECO/v4/000/144/114/9C954151-32B4-DF11-BB88-001D09F27003.root',
#	'/store/data/Run2010A/Mu/RECO/v4/000/144/114/5C9CA515-20B4-DF11-9D62-0030487A3DE0.root',
#	'/store/data/Run2010A/Mu/RECO/v4/000/144/114/00CA69A9-1EB4-DF11-B869-0030487CD6D2.root'
#	'/store/mc/Spring10/LambdaBToJPsiMuMu_2MuPEtaFilter_7TeV-pythia6-evtgen/GEN-SIM-RECO/START3X_V26_S09-v1/0004/F8255609-684D-DF11-B02F-0017A477040C.root'
#	'/store/data/Run2010B/MuOnia/AOD/Nov4ReReco_v1/0185/CAA3257D-85F1-DF11-A936-00215E21D570.root',
#	'/store/data/Run2010B/MuOnia/AOD/Nov4ReReco_v1/0185/B457A05F-85F1-DF11-AA6A-00215E2223B2.root',
#	'/store/data/Run2010B/MuOnia/AOD/Nov4ReReco_v1/0185/7A4AA8EF-84F1-DF11-BE03-00215E22229E.root',
#	'/store/data/Run2010B/MuOnia/AOD/Nov4ReReco_v1/0185/74A33CF4-84F1-DF11-B9DF-00215E221218.root'
#	'/store/data/Run2011A/MuOnia/AOD/PromptReco-v1/000/161/312/EC4F121D-E257-E011-A167-0030487A18D8.root',
#	'/store/data/Run2011A/MuOnia/AOD/PromptReco-v1/000/161/312/E89CCBE0-E957-E011-A53B-0019B9F709A4.root',
#	'/store/data/Run2011A/MuOnia/AOD/PromptReco-v1/000/161/312/E2414F32-E457-E011-8CF9-001617C3B710.root',
#	'/store/data/Run2011A/MuOnia/AOD/PromptReco-v1/000/161/312/D407945E-ED57-E011-A36D-003048F0258C.root',
#	'/store/data/Run2011A/MuOnia/AOD/PromptReco-v1/000/161/312/CEB587EC-F057-E011-B9DC-001617E30E2C.root'
	 '/store/data/Run2011A/MuOnia/AOD/PromptReco-v2/000/164/235/1AB785FF-B679-E011-A07F-0030487C8CB6.root',
         '/store/data/Run2011A/MuOnia/AOD/PromptReco-v2/000/164/234/881E513E-B479-E011-89A9-0030487C635A.root',
	 '/store/data/Run2011A/MuOnia/AOD/PromptReco-v2/000/164/232/E6AC8859-B179-E011-B5F4-0030487CD6E6.root',
	 '/store/data/Run2011A/MuOnia/AOD/PromptReco-v2/000/164/224/A09611CB-A479-E011-99E8-0030487A18A4.root',
	 '/store/data/Run2011A/MuOnia/AOD/PromptReco-v2/000/163/869/B4C142A1-D377-E011-8C23-00304879BAB2.root'
    )
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-100) )


# ----------------------------------------------------------------------
try:
    rootFileName = os.environ["JOB"] + ".root"
except KeyError:
    rootFileName = "lambdaB-XXXX.root"

process.tree = cms.EDAnalyzer(
    "HFTree",
    verbose      = cms.untracked.int32(0),
    requireCand  =  cms.untracked.bool(True),
    fileName     = cms.untracked.string(rootFileName)
    )

# ----------------------------------------------------------------------
process.load("HeavyFlavorAnalysis.Bs2MuMu.HFMCTruth_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("HeavyFlavorAnalysis.Bs2MuMu.HFRecoStuff_cff")
process.trkDump.verbose = cms.untracked.int32(0)
process.load("HeavyFlavorAnalysis.Bs2MuMu.HFTruthCandidates_cff")
#process.load("HeavyFlavorAnalysis.Bs2MuMu.HFB2JpsiCandidates_cff")
process.load("HeavyFlavorAnalysis.Bs2MuMu.HFLambdas_cff")
process.HFLambdasDump.verbose = cms.untracked.int32(0)
process.HFLambdasDump.maxDoca = cms.untracked.double(.1)
process.HFLambdasDump.minPocaJpsi = cms.untracked.double(.1)
process.HFLambdasDump.minPocaL0 = cms.untracked.double(1)
process.HFLambdasDump.psiMuons = cms.untracked.int32(2)
process.HFLambdasDump.L0Window = cms.untracked.double(0.2)
process.HFLambdasDump.pAngle = cms.untracked.double(0.02)
#process.load("HeavyFlavorAnalysis.Bs2MuMu.HFMuonAndTrackCandidates_cff")

#process.load("HeavyFlavorAnalysis.Bs2MuMu.HFB2MuCandidates_cff")
#process.load("HeavyFlavorAnalysis.Bs2MuMu.HFDimuonsCandidates_cff")

# ----------------------------------------------------------------------
process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
#    useJobReport = cms.untracked.bool(True)
#    dump = cms.untracked.bool(False)
)
process.SimpleMemoryCheck.moduleMemorySummary = cms.untracked.bool(True)
process.SimpleMemoryCheck.oncePerEventMode = cms.untracked.bool(True)

# ----------------------------------------------------------------------
process.p = cms.Path(
#    process.MCTruthSequence*
    process.recoStuffSequence*
#    process.bmmDump*
#    process.truthAllSequence*
#    process.mtDump*
    process.HFLambdasSequence*
#    process.B2JPsiSequence*
    process.tree
)




