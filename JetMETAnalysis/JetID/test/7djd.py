################################################################################
#
# run_CaloJetIDAnalyzer_cfg.py
# ----------------------------
#
# over 7TeV MC
#
################################################################################

import FWCore.ParameterSet.Config as cms
import os.path
import sys

sample = sys.argv[-1]

#!
#! PROCESS
#!
process = cms.Process("JETID")

#!
#! INPUT
#!
from di_jet_7_mc_samples import *
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( -1 ) )
process.source = cms.Source( "PoolSource", fileNames = files[sample] ) 

#!
#! SERVICES
#!
process.MessageLogger = cms.Service(
    "MessageLogger",
    destinations = cms.untracked.vstring('cout'),
    cout         = cms.untracked.PSet( threshold = cms.untracked.string('WARNING') )
    )
process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string('/uscms_data/d2/aharel/7djd'+sample+'.root'),
)
#process.Timing = cms.Service("Timing")
#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck")

#!
#! MODULES
#!

# Jet RECO
process.load('Configuration/StandardSequences/Geometry_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load("RecoJets.JetProducers.ak5CaloJets_cfi")
process.recoAK5 = cms.Sequence( process.ak5CaloJets )


# KINEMATIC JET PRESELECTION
process.ak5CaloJetsEtaPt = cms.EDProducer(
    "CaloJetSelector",
    src    = cms.InputTag('ak5CaloJets'),
    cut    = cms.string('pt>0.0 && abs(eta)<9.9')
    )

# JET-TO-TRACKS ASSOCIATION
process.ak5CaloJets2trksAtVertex = cms.EDProducer(
    "JetTracksAssociatorAtVertex",
    jets     = cms.InputTag('ak5CaloJetsEtaPt'),
    tracks   = cms.InputTag('generalTracks'),
    coneSize = cms.double(0.5)
    )
process.load("TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAlong_cfi")
process.ak5CaloJets2trksAtCalo = cms.EDProducer(
    "JetTracksAssociatorAtCaloFace",
    jets     = cms.InputTag('ak5CaloJetsEtaPt'),
    tracks   = cms.InputTag('generalTracks'),
    coneSize = cms.double(0.5)
    )

# TRACK-JET RECONSTRUCTION
process.load("RecoJets.JetProducers.TracksForJets_cff")
process.load("RecoJets.JetProducers.ak5TrackJets_cfi")

# JEC
process.load("JetMETCorrections.Configuration.L2L3Corrections_Summer09_7TeV_ReReco332_cff")
process.prefer("L2L3JetCorrectorAK5Calo")

# HCAL noise removal, courtesy of John Paul
process.load("RecoMET.METProducers.hcalnoiseinfoproducer_cfi")
process.hcalnoise.refillRefVectors = cms.bool(True)
process.hcalnoise.requirePedestals = cms.bool(False)
process.hcalnoise.hcalNoiseRBXCollName = "hcalnoise"

# generated truth analysis
process.load('RecoJets.Configuration.GenJetParticles_cff')
process.genJetParticlesNoNu = cms.Sequence( process.genParticlesForJetsNoNu )

# JETID
process.load('JetMETAnalysis.JetID.CaloJetIDAnalyzer_cfi')
process.ak5JetID = process.jetid.clone(
    bookAll              = True,
#    debug                = 1,
    srcCaloJets          = 'ak5CaloJetsEtaPt',
    srcTrackJets         = 'ak5TrackJets',
    ptHatBins = (0, 15, 20, 30, 50,                 80, 120, 170, 230, 300,   
                 380, 470, 600, 800, 1000,      1400, 1800, 2200, 2600, 3000,    
                 999999),
    mcXsecs = (            4.844e+10, 5.794e+08, 2.361e+08, 5.311e+07,    6.358e+06, 7.849e+05, 1.151e+05, 2.014e+04, 4.094e+03,
                9.346e+02, 2.338e+02, 7.021e+01, 1.557e+01, 1.843e+00,    3.318e-01, 1.086e-02, 3.499e-04, 7.549e-06, 6.465e-08,
                6.295e-11 ),
    )
#!
#! PATHS
#!
process.ak5Jets = cms.Sequence(
    process.recoAK5
    * process.ak5CaloJetsEtaPt
    * process.ak5CaloJets2trksAtVertex
    * process.ak5CaloJets2trksAtCalo
    * process.tracksForJets
    * process.ak5TrackJets
    )

process.analysis = cms.Path(
    process.ak5Jets
    * process.hcalnoise
    * process.genJetParticlesNoNu
    * process.ak5JetID
    )

