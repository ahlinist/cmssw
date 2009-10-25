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
patTriggerMuonMatcher += muonTriggerMatchHLTIsoMu11
patTriggerEvent.patTriggerMatches.append("muonTriggerMatchHLTIsoMu11")
cleanLayer1MuonsTriggerMatch.matches = cms.VInputTag("muonTriggerMatchHLTIsoMu11")

#--------------------------------------------------------------------------------  
# PAT layer 1 muon configuration parameters
#--------------------------------------------------------------------------------

# increase size of muon isolation cone from default of deltaR = 0.3 to 0.6
allLayer1Muons.isolation.tracker.deltaR = cms.double(0.6)

allLayer1Muons.isolation.ecal.deltaR = cms.double(0.6)

allLayer1Muons.isolation.hcal.deltaR = cms.double(0.6)

allLayer1Muons.isolation.user.deltaR = cms.double(0.6)
#
# add IsoDeposit objects for Track, ECAL and HCAL based isolation
#
allLayer1Muons.isoDeposits = cms.PSet(
   tracker          = allLayer1Muons.isolation.tracker.src,
   ecal             = allLayer1Muons.isolation.ecal.src,
   hcal             = allLayer1Muons.isolation.hcal.src,
   particle         = cms.InputTag("pfmuIsoDepositPFCandidates"),
   pfChargedHadrons = cms.InputTag("pfmuIsoChDepositPFCandidates"),
   pfNeutralHadrons = cms.InputTag("pfmuIsoNeDepositPFCandidates"),
   pfPhotons        = cms.InputTag("pfmuIsoGaDepositPFCandidates")
)
#
# enable matching to HLT trigger information;
# match offline reconstructed muons to isolated and non-isolated HLT muon paths
#
allLayer1Muons.embedHighLevelSelection = cms.bool(True)
#
# enable matching to generator level information
#
allLayer1Muons.addGenMatch = cms.bool(True)
