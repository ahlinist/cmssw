import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.triggerLayer1.triggerMatcher_cfi import *
from PhysicsTools.PatAlgos.triggerLayer1.triggerEventProducer_cfi import *
from PhysicsTools.PatAlgos.triggerLayer1.triggerMatchEmbedder_cfi import *
from PhysicsTools.PatAlgos.producersLayer1.muonProducer_cfi import *
from PhysicsTools.PatAlgos.cleaningLayer1.muonCleaner_cfi import *

#--------------------------------------------------------------------------------  
# PAT layer 0 muon configuration parameters
#--------------------------------------------------------------------------------

# add HLT electron trigger to PAT trigger match sequence
#patTriggerMatcherMuon += muonTriggerMatchHLTIsoMu11
#patTriggerEvent.patTriggerMatches.append("muonTriggerMatchHLTIsoMu11")
#cleanPatMuonsTriggerMatch.matches = cms.VInputTag("muonTriggerMatchHLTIsoMu11")

#--------------------------------------------------------------------------------  
# PAT layer 1 muon configuration parameters
#--------------------------------------------------------------------------------

# increase size of muon isolation cone from default of deltaR = 0.3 to 0.6
patMuons.userIsolation = cms.PSet()
patMuons.userIsolation.tracker = cms.PSet(
    deltaR = cms.double(0.4),
    src = cms.InputTag("muIsoDepositTk")
)
patMuons.userIsolation.ecal = cms.PSet(
    deltaR = cms.double(0.4),
    src = cms.InputTag("muIsoDepositCalByAssociatorTowers","ecal")
)
patMuons.userIsolation.hcal = cms.PSet(
    deltaR = cms.double(0.4),
    src = cms.InputTag("muIsoDepositCalByAssociatorTowers","hcal")
)
patMuons.userIsolation.user = cms.VPSet(
    cms.PSet( 
        src = cms.InputTag("muIsoDepositCalByAssociatorTowers","ho"),
        deltaR = cms.double(0.4)
    ), 
    cms.PSet(
        src = cms.InputTag("muIsoDepositJets"),
        deltaR = cms.double(0.4)
    )
)

# add IsoDeposit objects for Track, ECAL and HCAL based isolation
patMuons.isoDeposits = cms.PSet(
   tracker          = patMuons.userIsolation.tracker.src,
   ecal             = patMuons.userIsolation.ecal.src,
   hcal             = patMuons.userIsolation.hcal.src,
   pfAllParticles   = cms.InputTag("pfmuIsoDepositPFCandidates"),
   pfChargedHadrons = cms.InputTag("pfmuIsoChDepositPFCandidates"),
   pfNeutralHadrons = cms.InputTag("pfmuIsoNeDepositPFCandidates"),
   pfPhotons        = cms.InputTag("pfmuIsoGaDepositPFCandidates")
)

# embed Pixel + SiStrip track reference in pat::Muon
# (so that analysis can access "inner" track information if running on PAT-tuples)
patMuons.embedTrack = cms.bool(True)

# enable matching to HLT trigger information;
# match offline reconstructed muons to isolated and non-isolated HLT muon paths
patMuons.embedHighLevelSelection = cms.bool(True)

# enable matching to generator level information
patMuons.addGenMatch = cms.bool(True)

