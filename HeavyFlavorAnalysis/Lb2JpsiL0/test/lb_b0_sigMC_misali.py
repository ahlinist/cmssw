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
# -- Configure use of different alignment
from CondCore.DBCommon.CondDBSetup_cfi import *
process.trackerAlignment = cms.ESSource(
    "PoolDBESSource",
    CondDBSetup,
    toGet = cms.VPSet(cms.PSet(
	record = cms.string('TrackerAlignmentRcd'),
#	tag = cms.string('TrackerAlignment_2010Realistic_mc') # START42_V11
#	tag = cms.string('TrackerIdealGeometry210_mc') # DESIGN42_V11
	tag = cms.string('Alignments') # from SQLite file
    )),
#    connect = cms.string('sqlite_file:/shome/meier_f1/CMSSW/CMSSW_4_2_8_patch7/src/Alignment/TrackerAlignment/test/alignment_START42_V11.db') # sqlite file
     connect = cms.string('sqlite_file:alignment_START42_V11_radialEpsilon_minus.db') # sqlite file
#    connect = cms.string('frontier://FrontierProd/CMS_COND_31X_ALIGNMENT') # from Frontier service, matching START42_V11
#    connect = cms.string('frontier://FrontierProd/CMS_COND_31X_FROM21X') # from Frontier service, should be DESIGN42_V11
)

# -- do the refitting
process.es_prefer_trackerAlignment = cms.ESPrefer("PoolDBESSource", "trackerAlignment")

process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
import RecoTracker.TrackProducer.TrackRefitters_cff
process.TrackRefitter1 = RecoTracker.TrackProducer.TrackRefitter_cfi.TrackRefitter.clone()
process.TrackRefitter1.src = 'generalTracks'

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

#	'/store/user/frmeier/lambda/datasets/F4773C84-17C0-E011-8781-003048D37456.root' # one file from /MuOnia/Run2011A-PromptReco-v6/RECO
	'/store/user/frmeier/lambda/datasets/PYTHIA6_B0ToJpsiKs_7TeV_EvtGen_Frank_cff_py_GEN_SIM_DIGI_L1_DIGI2RAW_root_HLT_quarkonium_RAW2DIGI_L1Reco_RECO_427_1_gRY.root' # one file of MC
    )
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )


# ----------------------------------------------------------------------
rootFileName = "test_lb_b0_misali.root"

process.tree = cms.EDAnalyzer(
    "HFTree",
    verbose      = cms.untracked.int32(0),
    requireCand  =  cms.untracked.bool(True),
    fileName     = cms.untracked.string(rootFileName)
    )

# ----------------------------------------------------------------------
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("HeavyFlavorAnalysis.Bs2MuMu.HFMCTruth_cff")
process.load("HeavyFlavorAnalysis.Bs2MuMu.HFRecoStuff_cff")
#process.load("HeavyFlavorAnalysis.Bs2MuMu.HFTruthCandidates_cff")
#process.load("HeavyFlavorAnalysis.Bs2MuMu.HFBmm_cff")
process.load("HeavyFlavorAnalysis.Bs2MuMu.HFLambdas_tight_cff")

process.HFLambdasDump.doVcands = cms.untracked.bool(True)
process.HFLambdasDump.useAnalysisValuesForEff= cms.untracked.bool(True)

process.HFLambdasDump.tracksLabel = cms.untracked.InputTag('TrackRefitter1')


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
    process.offlineBeamSpot*process.TrackRefitter1* # refitting to use chosen alignment
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
