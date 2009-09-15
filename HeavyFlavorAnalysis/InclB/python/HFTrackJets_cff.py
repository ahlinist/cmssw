import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Reconstruction_cff import *
from RecoJets.JetProducers.FastjetParameters_cfi import *
from RecoJets.JetProducers.SISConeJetParameters_cfi import *


trkjetflavourByRef = cms.EDFilter("JetPartonMatcher",
    jets = cms.InputTag("sis5TrackJets"),
    coneSizeToAssociate = cms.double(0.5),
    partons = cms.InputTag("myPartons") #myPartons defined in HFJets_cff.py
)

selectTracks = cms.EDFilter("TrackSelector",
    src = cms.InputTag("generalTracks"),
    cut = cms.string('pt > 0.3 & pt<500 & numberOfValidHits > 0')
)
trackCandidates = cms.EDProducer("ConcreteChargedCandidateProducer",
    src = cms.InputTag("selectTracks"),
    particleType = cms.string('pi+')
)

trackjetDump = cms.EDFilter("HFDumpTrackJets",
    verbose = cms.untracked.int32(0),
    jetsLabel = cms.untracked.string('sis5TrackJets'),
    tracksLabel = cms.untracked.string('trackCandidates'),
    sourceByRefer = cms.InputTag("trkjetflavourByRef"),
    genparticlesLabel = cms.untracked.string('genParticles')
)

sisTrackJetParameters = cms.PSet( 
    src = cms.InputTag("trackCandidates"),
    verbose = cms.untracked.bool(False),
    inputEtMin = cms.double(0.3),
    seedThreshold = cms.double(1.0),
    debugLevel = cms.untracked.int32(0),
    jetType = cms.untracked.string('BasicJet'),
    inputEMin = cms.double(0.0)
)

sis5TrackJets = cms.EDProducer("SISConeJetProducer",
    sisTrackJetParameters,
    FastjetNoPU,
    SISConeJetParameters,
    alias = cms.untracked.string('sis5TrackJets'),
    jetPtMin = cms.double( 1.0 ),
    coneRadius = cms.double(0.5)
)

TrackJetDump = cms.Sequence(selectTracks*trackCandidates*sis5TrackJets*trkjetflavourByRef*trackjetDump)
