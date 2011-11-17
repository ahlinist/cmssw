import FWCore.ParameterSet.Config as cms

import copy

#--------------------------------------------------------------------------------
# Selection of Z --> mu+ mu- candidate events for use with tau embedding.
# The selection is choosen to be very loose to cover the full phase space
# required by tautau analyses. The cuts can still be tightened off-line
# depending on the needs of a specific analysis.
# 
# The following requirements are made:
# 
# * 2 global muons fulfilling VBTF quality criteria
# * pt > 20 GeV for leading muon; pt > 10 GeV for subleading muon
# * |eta| < 2.5
# * rel. comb. isolation:
#     SumPt + emEt + hadEt < 0.15*pt in a cone of DeltaR=0.3
#--------------------------------------------------------------------------------

from CommonTools.ParticleFlow.pfNoPileUp_cff import *

from PhysicsTools.PatAlgos.producersLayer1.muonProducer_cfi import patMuons
patMuons.addGenMatch = cms.bool(False)
patMuons.embedHighLevelSelection = cms.bool(True)
patMuons.usePV = cms.bool(False) # compute transverse impact parameter wrt. beamspot (not event vertex)

# Vertex selection
goodVertex = cms.EDFilter("VertexSelector",
    src = cms.InputTag("offlinePrimaryVertices"),
    cut = cms.string("(!isFake) & ndof > 3 & abs(z) < 15 & position.Rho < 2"),
    filter = cms.bool(True)
)

# Cuts for subleading muon, no isolation cut applied
goodMuons = cms.EDFilter("PATMuonSelector",
  src = cms.InputTag("patMuons"),
     cut = cms.string(
           'pt > 10 & abs(eta) < 2.5 & isGlobalMuon & isTrackerMuon ' \
                 + ' & innerTrack.hitPattern.numberOfValidTrackerHits > 9 & innerTrack.hitPattern.numberOfValidPixelHits > 0' \
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

# Produce combinations of good muon pairs
#  o muon+ + muon-
goldenZmumuCandidatesGe0IsoMuons = cms.EDProducer("CandViewCombiner", # cannot be CandViewShallowCloneCombiner because otherwise isolationR03 is inaccessible in cut property in goldenZmumuCandidatesGe1IsoMuons
    checkCharge = cms.bool(True),
    # require one of the muons with pT > 20
    cut = cms.string('charge = 0 & max(daughter(0).pt,daughter(1).pt)>20'),
    decay = cms.string("goodMuons@+ goodMuons@-")
)

#  o muon+ + iso. muon- || iso. muon+ + muon-
goldenZmumuCandidatesGe1IsoMuons = goldenZmumuCandidatesGe0IsoMuons.clone(
    # Require one of the muons with pT > 20 and also one of the muons to be isolated:
    cut = cms.string('charge = 0 & max(daughter(0).pt,daughter(1).pt) > 20.0 & min((daughter(0).isolationR03().sumPt + daughter(0).isolationR03().emEt + daughter(0).isolationR03().hadEt)/daughter(0).pt, (daughter(1).isolationR03().sumPt + daughter(1).isolationR03().emEt + daughter(1).isolationR03().hadEt)/daughter(1).pt) < 0.15'),
    decay = cms.string("goodMuons@+ goodMuons@-")

    # This does not work because it creates duplicate entries:
    #decay = cms.string("goodMuons@+ goodIsoMuons@-") # charge conjugate decays included,
    #                                                 # cf. https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideCandCombiner
)

#  o iso. muon+ + iso. muon-
goldenZmumuCandidatesGe2IsoMuons = goldenZmumuCandidatesGe0IsoMuons.clone(
    # Require both of the muons to be isolated
    decay = cms.string("goodIsoMuons@+ goodIsoMuons@-")
)

#goodMuonFilter = cms.EDFilter("CandViewCountFilter",
#    src = cms.InputTag("goodMuons"),
#    minNumber = cms.uint32(2),
#    maxNumber = cms.uint32(2)
#)

# Discard all events that do not have at least an unisolated muon pair
goldenZmumuFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("goldenZmumuCandidatesGe0IsoMuons"), # loose selection 
    #src = cms.InputTag("goldenZmumuCandidatesGe1IsoMuons"),  # tight selection                            
    minNumber = cms.uint32(1)
)

goldenZmumuSelectionSequence = cms.Sequence(
    # zmmHLTFilter
    goodVertex
    #* pfNoPileUpSequence
    * patMuons * goodMuons * goodIsoMuons
    * goldenZmumuCandidatesGe0IsoMuons * goldenZmumuCandidatesGe1IsoMuons * goldenZmumuCandidatesGe2IsoMuons
    #* goodMuonFilter
    * goldenZmumuFilter
)


