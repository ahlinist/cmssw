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
#process.GlobalTag.globaltag = "GR10_P_V4::All"
#process.GlobalTag.globaltag = "START38_V12::All"
process.GlobalTag.globaltag = "START38_V14::All"


# ----------------------------------------------------------------------

# Dataset: /LambdaBToJPsiMuMu_2MuPEtaFilter_7TeV-pythia6-evtgen/

process.source = cms.Source(
    "PoolSource", 
    fileNames = cms.untracked.vstring(
        '/store/mc/Fall10/LambdaBToJPsiMuMu_2MuPEtaFilter_7TeV-pythia6-evtgen/GEN-SIM-RECO/START38_V12-v2/0000/FE52B966-1BE7-DF11-B7CA-00304867FE13.root',
        '/store/mc/Fall10/LambdaBToJPsiMuMu_2MuPEtaFilter_7TeV-pythia6-evtgen/GEN-SIM-RECO/START38_V12-v2/0000/FCEF243C-08E7-DF11-AD8C-842B2B019EA1.root',
        '/store/mc/Fall10/LambdaBToJPsiMuMu_2MuPEtaFilter_7TeV-pythia6-evtgen/GEN-SIM-RECO/START38_V12-v2/0000/FC1A9EF3-1AE7-DF11-A7F4-001E8CC04105.root',
        '/store/mc/Fall10/LambdaBToJPsiMuMu_2MuPEtaFilter_7TeV-pythia6-evtgen/GEN-SIM-RECO/START38_V12-v2/0000/FAC989A4-F9E6-DF11-BF29-842B2B1815B3.root',
        '/store/mc/Fall10/LambdaBToJPsiMuMu_2MuPEtaFilter_7TeV-pythia6-evtgen/GEN-SIM-RECO/START38_V12-v2/0000/FABFAED1-F2E6-DF11-A935-00304867FDA3.root',
        '/store/mc/Fall10/LambdaBToJPsiMuMu_2MuPEtaFilter_7TeV-pythia6-evtgen/GEN-SIM-RECO/START38_V12-v2/0000/FA7DEEC5-5FE7-DF11-AF4C-0026B93B21AE.root',
        '/store/mc/Fall10/LambdaBToJPsiMuMu_2MuPEtaFilter_7TeV-pythia6-evtgen/GEN-SIM-RECO/START38_V12-v2/0000/FA24C29D-F9E6-DF11-9C5B-842B2B181727.root',
        '/store/mc/Fall10/LambdaBToJPsiMuMu_2MuPEtaFilter_7TeV-pythia6-evtgen/GEN-SIM-RECO/START38_V12-v2/0000/F84188C5-45E7-DF11-9B95-842B2B17EE7F.root',
        '/store/mc/Fall10/LambdaBToJPsiMuMu_2MuPEtaFilter_7TeV-pythia6-evtgen/GEN-SIM-RECO/START38_V12-v2/0000/F6B853D0-77EC-DF11-9A65-001E8CC04123.root',
        '/store/mc/Fall10/LambdaBToJPsiMuMu_2MuPEtaFilter_7TeV-pythia6-evtgen/GEN-SIM-RECO/START38_V12-v2/0000/F276F156-12E7-DF11-ACE5-001A9254460D.root'
	)
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )


# ----------------------------------------------------------------------
try:
    rootFileName = os.environ["JOB"] + ".root"
except KeyError:
    rootFileName = "lambdaB_signalMC_LBeff.root"

process.tree = cms.EDAnalyzer(
    "HFTree",
    verbose      = cms.untracked.int32(0),
    requireCand  =  cms.untracked.bool(False), # its an efficiency study...
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

# specific settings for MC
process.trkDump.doTruthMatching = cms.untracked.int32(4)
process.trkDump.truthMatchingChi2cut = cms.untracked.double(20)
process.trkDump.GenCandidatesLabel = cms.untracked.InputTag('genParticlePlusGEANT')
#process.HFLambdasDumpV0.verbose = cms.untracked.int32(-1)
process.HFLambdasDump.doVcands = cms.untracked.bool(True)

#process.load("HeavyFlavorAnalysis.Bs2MuMu.HFMuonAndTrackCandidates_cff")

#process.load("HeavyFlavorAnalysis.Bs2MuMu.HFB2MuCandidates_cff")
#process.load("HeavyFlavorAnalysis.Bs2MuMu.HFDimuonsCandidates_cff")

# ----------------------------------------------------------------------
process.genParticlePlusGEANT = cms.EDProducer("GenPlusSimParticleProducer",
        src           = cms.InputTag("g4SimHits"), # use "famosSimHits" for FAMOS
        setStatus     = cms.int32(8),             # set status = 8 for GEANT GPs
        filter        = cms.vstring("pt > 0.0"),  # just for testing (optional)
        genParticles   = cms.InputTag("genParticles") # original genParticle list
)
process.genDump.generatorCandidates = cms.untracked.string('genParticlePlusGEANT')

# ----------------------------------------------------------------------
process.p = cms.Path(
	process.genParticlePlusGEANT*
	process.MCTruthSequence*
	process.recoStuffSequence*
#	process.bmmDump*
#	process.truthAllSequence*
#	process.mtDump*
	process.HFLambdasSequence*
#	process.B2JPsiSequence*
	process.tree
)




