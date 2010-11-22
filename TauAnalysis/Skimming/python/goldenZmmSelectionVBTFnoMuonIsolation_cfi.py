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

from PhysicsTools.PFCandProducer.pfNoPileUp_cff import *

from PhysicsTools.PatAlgos.producersLayer1.muonProducer_cfi import patMuons
patMuons.addGenMatch = cms.bool(False)
patMuons.embedHighLevelSelection = cms.bool(True)
patMuons.usePV = cms.bool(False) # compute transverse impact parameter wrt. beamspot (not event vertex)

# Trigger requirements
import HLTrigger.HLTfilters.hltHighLevel_cfi
zmmHLTFilter = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
zmmHLTFilter.TriggerResultsTag = cms.InputTag("TriggerResults", "", "HLT")
zmmHLTFilter.HLTPaths = [ "HLT_Mu9", "HLT_IsoMu9", "HLT_Mu11", "HLT_IsoMu13_v3", "HLT_IsoMu13_v4", "HLT_Mu15_v1" ]
zmmHLTFilter.throw = cms.bool(False)

# Vertex selection
goodVertex = cms.EDFilter("VertexSelector",
    src = cms.InputTag("offlinePrimaryVertices"),
    cut = cms.string("(!isFake) & ndof > 3 & abs(z) < 15 & position.Rho < 2"),
    filter = cms.bool(True)
)

# Cuts for both muons, no isolation cuts applied
goodMuons = cms.EDFilter("PATMuonSelector",
  src = cms.InputTag("patMuons"),
    cut = cms.string(
      'pt > 20 & abs(eta) < 2.5 & isGlobalMuon' \
     + ' & innerTrack.hitPattern.numberOfValidTrackerHits > 9 & innerTrack.hitPattern.numberOfValidPixelHits > 0' \
     + ' & abs(dB) < 0.2 & globalTrack.normalizedChi2 < 10' \
     + ' & globalTrack.hitPattern.numberOfValidMuonHits > 0 & numberOfMatches > 1' 
    ),
    filter = cms.bool(True)
)

# Cuts for muon leg with isolation cut applied
from TauAnalysis.RecoTools.patLeptonPFIsolationSelector_cfi import patMuonPFIsolationSelector
goodIsoMuons = cms.EDFilter("PATMuonPFIsolationSelector",
    patMuonPFIsolationSelector.clone(
        sumPtMax = cms.double(0.06),
    ),
    src = cms.InputTag("goodMuons"),                          
    filter = cms.bool(False)
)
goodIsoMuons.chargedHadronIso.dRisoCone = cms.double(0.6)
goodIsoMuons.neutralHadronIso.dRisoCone = cms.double(0.6)
goodIsoMuons.photonIso.dRisoCone = cms.double(0.6)

# Produce combinations of good
#  o muon+ + muon-
#  o muon+ + iso. muon- || iso. muon+ + muon-
#  o iso. muon+ + iso. muon-
# with a di-muon invariant mass within the Z mass-window
goldenZmumuCandidatesGe0IsoMuons = cms.EDProducer("CandViewShallowCloneCombiner",
    checkCharge = cms.bool(True),
    cut = cms.string('mass > 60. & mass < 120. & charge = 0'),
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
    * goodVertex
    * pfNoPileUpSequence
    * patMuons * goodMuons * goodIsoMuons
    * goldenZmumuCandidatesGe0IsoMuons * goldenZmumuCandidatesGe1IsoMuons * goldenZmumuCandidatesGe2IsoMuons
    * goodMuonFilter * goldenZmumuFilter
    * goodMuonIsolationTagAndProbeProducer
)


