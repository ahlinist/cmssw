import FWCore.ParameterSet.Config as cms

import copy

#--------------------------------------------------------------------------------
# Selection of "golden" Z --> mu+ mu- candidate events passing VTBF selection
# documented in CMS AN-10-264
#
# Selection criteria are based on ElectroWeakAnalysis/Utilities/python/goldenZmmSelectionVBTF_cfi.py
# with the exception that muon isolation cuts are applied for only one of the two muons
# (in order not to bias the tau id. efficiency measurement via "embedding technique")
#
# Depending on whether the muons pass the isolation cut or not, the two muons are marked as "tag" and "probe".
#--------------------------------------------------------------------------------

#from PhysicsTools.PFCandProducer.pfNoPileUp_cff import *

from PhysicsTools.PatAlgos.producersLayer1.muonProducer_cfi import patMuons
patMuons.addGenMatch = cms.bool(False)
patMuons.embedHighLevelSelection = cms.bool(True)
patMuons.usePV = cms.bool(False) # compute transverse impact parameter wrt. beamspot (not event vertex)

# Trigger requirements
import HLTrigger.HLTfilters.hltHighLevel_cfi
zmmHLTFilter = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
zmmHLTFilter.TriggerResultsTag = cms.InputTag("TriggerResults", "", "HLT")
zmmHLTFilter.HLTPaths = [ "HLT_Mu9", "HLT_Mu15_v1", "HLT_Mu15_v2", "HLT_IsoMu17_v5", "HLT_IsoMu17_v7", "HLT_DoubleMu7_v1", "HLT_DoubleMu7_v2", "HLT_Mu13_Mu8_v2", "HLT_Mu17_Mu8_v2" ]
zmmHLTFilter.throw = cms.bool(False)

# Cuts for both muons, no isolation cuts applied
goodMuons = cms.EDFilter("PATMuonSelector",
  src = cms.InputTag("patMuons"),
     cut = cms.string(
           'pt > 20 & abs(eta) < 2.5 & isGlobalMuon & isTrackerMuon ' \
                 + ' & innerTrack.hitPattern.numberOfValidTrackerHits > 10 & innerTrack.hitPattern.numberOfValidPixelHits > 0' \
                 + ' & abs(dB)<0.2 & globalTrack.normalizedChi2 < 10' \
                 + ' & globalTrack.hitPattern.numberOfValidMuonHits > 0 & numberOfMatches > 1'
  ),
  filter = cms.bool(True)
)

# Cuts for muon leg with isolation cut applied
goodIsoMuons = cms.EDFilter("PATMuonSelector",
    src = cms.InputTag("goodMuons"),
    cut = cms.string('(isolationR03().sumPt+isolationR03().emEt+isolationR03().hadEt)<0.15*pt'),
    filter = cms.bool(False),
)

# Produce combinations of good
#  o muon+ + muon-
#  o muon+ + iso. muon- || iso. muon+ + muon-
#  o iso. muon+ + iso. muon-
# with a di-muon invariant mass within the Z mass-window
goldenZmumuCandidatesGe0IsoMuons = cms.EDProducer("CandViewShallowCloneCombiner",
    checkCharge = cms.bool(True),
#    cut = cms.string('mass > 60. & mass < 120. & charge = 0'),
    cut = cms.string('charge = 0'),
    decay = cms.string("goodMuons@+ goodMuons@-")
)

goldenZmumuCandidatesGe1IsoMuons = goldenZmumuCandidatesGe0IsoMuons.clone(
    decay = cms.string("goodMuons@+ goodIsoMuons@-") # charge conjugate decays included,
                                                     # cf. https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideCandCombiner
)

goldenZmumuCandidatesGe2IsoMuons = goldenZmumuCandidatesGe0IsoMuons.clone(
    decay = cms.string("goodIsoMuons@+ goodIsoMuons@-")
)

goodMuonFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("goodMuons"),
    minNumber = cms.uint32(2),                            
    maxNumber = cms.uint32(2)
)

goldenZmumuFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("goldenZmumuCandidatesGe0IsoMuons"), # loose selection 
    #src = cms.InputTag("goldenZmumuCandidatesGe1IsoMuons"),  # tight selection                            
    minNumber = cms.uint32(1)
)

# mark muons as "tag" and "probe", depending on if the muons are isolated or not:
#  o if only one of the two muons produced in the Z decay is isolated,
#    the isolated muon is marked as "tag" and the non-isolated muon is marked as "probe"
#  o if both muons are isolated, **both** muons are marked as "tag" **and** as "probe"
goodMuonIsolationTagAndProbeProducer = cms.EDProducer("MuonIsolationTagAndProbeProducer",
    srcAllMuons = cms.InputTag("muons"),
    srcGoodMuons = cms.InputTag("goodMuons"),                                                  
    srcGoodIsoMuons = cms.InputTag("goodIsoMuons")                                    
)

# Selection sequence
goldenZmumuSelectionSequence = cms.Sequence(
     zmmHLTFilter
    #* goodVertex
    #* pfNoPileUpSequence
    * patMuons * goodMuons * goodIsoMuons
    * goldenZmumuCandidatesGe0IsoMuons * goldenZmumuCandidatesGe1IsoMuons * goldenZmumuCandidatesGe2IsoMuons
    * goodMuonFilter * goldenZmumuFilter
    * goodMuonIsolationTagAndProbeProducer
)


