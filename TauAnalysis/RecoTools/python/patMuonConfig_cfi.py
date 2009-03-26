import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.cleaningLayer0.muonCleaner_cfi import *
from PhysicsTools.PatAlgos.triggerLayer0.trigMatchSequences_cff import *
from PhysicsTools.PatAlgos.producersLayer1.muonProducer_cfi import *

#--------------------------------------------------------------------------------  
# PAT layer 0 muon configuration parameters
#--------------------------------------------------------------------------------

# add isolated HLT muon trigger to PAT trigger match sequence
patTrigMatchMuon = cms.Sequence( patTrigMatchHLT1MuonNonIso + patTrigMatchHLT1MuonIso )
patTrigMatch.replace(patTrigMatchHLT1MuonNonIso, patTrigMatchMuon)

# increase size of muon isolation cone from default of deltaR = 0.3 to 0.6;
# for muons to be considered "isolated", require:
#  o sum(Pt) of tracks in isolation cone < 1 GeV
#  o sum(Pt) of ECAL energy deposits in isolation cone < 1 GeV
#  o ...
allLayer0Muons.isolation.tracker.deltaR = cms.double(0.6)
allLayer0Muons.isolation.tracker.cut = cms.double(1.0)

allLayer0Muons.isolation.ecal.deltaR = cms.double(0.6)
allLayer0Muons.isolation.ecal.cut = cms.double(1.0)

allLayer0Muons.isolation.hcal.deltaR = cms.double(0.6)
#allLayer0Muons.isolation.hcal.cut = cms.double(2.0)

allLayer0Muons.isolation.user.deltaR = cms.double(0.6)
#allLayer0Muons.isolation.user.cut = cms.double(2.0)

# flag non-isolated muons, but do not reject them
allLayer0Muons.bitsToIgnore = cms.vstring('Isolation/All')

#--------------------------------------------------------------------------------  
# PAT layer 1 muon configuration parameters
#--------------------------------------------------------------------------------

# increase size of muon isolation cone from default of deltaR = 0.3 to 0.6
allLayer1Muons.isolation.tracker.deltaR = cms.double(0.6)

allLayer1Muons.isolation.ecal.deltaR = cms.double(0.6)

allLayer1Muons.isolation.hcal.deltaR = cms.double(0.6)

allLayer1Muons.isolation.user.deltaR = cms.double(0.6)

#
# enable matching to HLT trigger information;
# match offline reconstructed muons to isolated and non-isolated HLT muon paths
#
allLayer1Muons.addTrigMatch = cms.bool(True)
allLayer1Muons.trigPrimMatch = cms.VInputTag(
    cms.InputTag("muonTrigMatchHLT1MuonNonIso"),
    cms.InputTag("muonTrigMatchHLT1MuonIso")
)
#
# enable matching to generator level information
#
allLayer1Muons.addGenMatch = cms.bool(True)
