import FWCore.ParameterSet.Config as cms

from JetMETCorrections.Configuration.JetCorrectionsRecord_cfi import *
from RecoJets.Configuration.RecoJetAssociations_cff import *

process = cms.Process("RECO3")

process.load("Configuration.StandardSequences.Services_cff")

process.load("Configuration.StandardSequences.Reconstruction_cff")

process.load("Configuration.StandardSequences.FakeConditions_cff")

process.load("Configuration.StandardSequences.Simulation_cff")

process.load("Configuration.StandardSequences.MixingNoPileUp_cff")

process.load("Configuration.StandardSequences.VtxSmearedGauss_cff")

process.load("Configuration.StandardSequences.Geometry_cff")

process.load("Configuration.StandardSequences.MagneticField_cff")

process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.load("JetMETCorrections.Configuration.JetPlusTrackCorrections_cff")

process.load("JetMETCorrections.Configuration.ZSPJetCorrections152_cff")

# process.load("JetMETCorrections.Configuration.MCJetCorrections152_cff")

#maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
#readFiles = cms.untracked.vstring()
#secFiles = cms.untracked.vstring() 
#source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
#readFiles.extend( ( 
#       '/store/relval/CMSSW_2_1_8/RelValBJets_Pt_50_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0002/0C66A939-8F82-DD11-8442-0019DB29C614.root') );
#
#secFiles.extend( (
#               ) )

# test QCD file from 210 RelVal is on /castor/cern.ch/user/a/anikiten/jpt210qcdfile/
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)

process.source = cms.Source("PoolSource",
# cmssw210
#    fileNames = cms.untracked.vstring('file:/tmp/anikiten/FC999068-DB60-DD11-9694-001A92971B16.root')
# cmssw218
#    fileNames = cms.untracked.vstring('file:/tmp/anikiten/0C66A939-8F82-DD11-8442-0019DB29C614.root')
     fileNames = cms.untracked.vstring('/store/relval/CMSSW_2_1_8/RelValBJets_Pt_50_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0002/0C66A939-8F82-DD11-8442-0019DB29C614.root')
)

process.dump = cms.EDFilter("EventContentAnalyzer")

process.myanalysis = cms.EDFilter("JPTAnalyzer",
    HistOutFile = cms.untracked.string('analysis.root'),

    calojets = cms.string('iterativeCone5CaloJets'),
    zspjets = cms.string('ZSPJetCorJetIcone5'),
    genjets = cms.string('iterativeCone5GenJets'),
    JetCorrectionJPT = cms.string('JetPlusTrackZSPCorrectorIcone5')
#    genjets  = cms.string('iterativeCone5GenJetsNoNuBSM')
#    genjets = cms.string('iterativeCone5GenJets')
)

iterativeCone5JetTracksAssociatorAtVertex.jets = 'ZSPJetCorJetIcone5'
iterativeCone5JetTracksAssociatorAtCaloFace.jets = 'ZSPJetCorJetIcone5'
iterativeCone5JetExtender.jets = 'ZSPJetCorJetIcone5'

process.p1 = cms.Path(process.ZSPJetCorrections*recoJetAssociations*process.myanalysis)

# process.p1 = cms.Path(process.dump)
