import FWCore.ParameterSet.Config as cms

# import configs for PAT layer 0 and PAT layer 1 muon cleaners/producers
from PhysicsTools.PatAlgos.mcMatchLayer0.muonMatch_cfi import *
from PhysicsTools.PatAlgos.recoLayer0.muonIsolation_cff import *
from PhysicsTools.PatAlgos.cleaningLayer0.muonCleaner_cfi import *
from PhysicsTools.PatAlgos.producersLayer1.muonProducer_cfi import *

# set isolation but don't reject non-isolated muons
allLayer0MuForEWKTau = allLayer0Muons.copy()
# increase size of muon isolation cone from default of deltaR = 0.3 to 0.7;
# for muons to be considered "isolated", require:
#  o sum(Pt) of tracks in isolation cone < 1 GeV
#  o sum(Pt) of ECAL energy deposits in isolation cone < 1 GeV
#  o ...
allLayer0MuForEWKTau.isolation.tracker.deltaR = cms.double(0.7)
allLayer0MuForEWKTau.isolation.tracker.cut = cms.double(1.0)
allLayer0MuForEWKTau.isolation.ecal.deltaR = cms.double(0.7)
allLayer0MuForEWKTau.isolation.ecal.cut = cms.double(1.0)
allLayer0MuForEWKTau.isolation.hcal.deltaR = cms.double(0.7)
#allLayer0MuForEWKTau.isolation.hcal.cut = cms.double(2.0)
allLayer0MuForEWKTau.isolation.user.deltaR = cms.double(0.7)
#allLayer0MuForEWKTau.isolation.user.cut = cms.double(2.0)
# flag non-isolated muons; 
# keep all isolated and non-isolated muons in the event
allLayer0MuForEWKTau.bitsToIgnore = cms.vstring('Isolation/All')

allLayer1MuForEWKTau = allLayer1Muons.copy()
# set PAT layer 0 muon collections to EWK tau specific collections
allLayer1MuForEWKTau.muonSource = cms.InputTag("allLayer0MuForEWKTau")
# increase size of muon isolation cone from default of deltaR = 0.3 to 0.7 
allLayer1MuForEWKTau.isolation.hcal.deltaR = cms.double(0.7)
allLayer1MuForEWKTau.isolation.tracker.deltaR = cms.double(0.7)
allLayer1MuForEWKTau.isolation.user.deltaR = cms.double(0.7)
allLayer1MuForEWKTau.isolation.ecal.deltaR = cms.double(0.7)
# match offline reconstructed muons to isolated and non-isolated HLT muon paths
allLayer1MuForEWKTau.trigPrimMatch = cms.VInputTag(cms.InputTag("muonTrigMatchHLT1MuonNonIso"), cms.InputTag("muonTrigMatchHLT1MuonIso"))
# disable matching to generator level information
#allLayer1MuForEWKTau.addGenMatch = cms.bool(False)
