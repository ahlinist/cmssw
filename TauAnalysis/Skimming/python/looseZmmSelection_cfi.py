import FWCore.ParameterSet.Config as cms

import copy

#--------------------------------------------------------------------------------
# Loose selection of Z --> mu+ mu- candidates for the purpose of measuring
#  o muon reconstuction
#  o muon identification
#  o muon isolation
#  o muon trigger
# efficiencies and determination of systematic uncertainties related to
# missing transverse momentum reconstruction (in particular due to pile-up)
#--------------------------------------------------------------------------------

from TauAnalysis.Skimming.goldenZmmSelectionVBTFnoMuonIsolation_cfi import *
from CommonTools.RecoAlgos.TrackWithVertexRefSelector_cfi import *

#--------------------------------------------------------------------------------
# select isolated and non-isolated muons of different types
selStandAloneMuons = cms.EDFilter("PATMuonSelector",
    src = cms.InputTag("patMuons"),
    cut = cms.string(
      'pt > 10 & abs(eta) < 2.5 & isStandAloneMuon' \
     + ' & standAloneMuon.hitPattern.numberOfValidMuonHits > 0 & numberOfMatches > 1'
    ),
    filter = cms.bool(False)
)

selIsoStandAloneMuons = goodIsoMuons.clone(
    src = cms.InputTag("selStandAloneMuons"),
    filter = cms.bool(False)
)

selGlobalMuons = cms.EDFilter("PATMuonSelector",
    src = cms.InputTag("patMuons"),
    cut = cms.string(
      'pt > 10 & abs(eta) < 2.5 & isGlobalMuon' \
     + ' & innerTrack.hitPattern.numberOfValidTrackerHits > 9 & innerTrack.hitPattern.numberOfValidPixelHits > 0' \
     + ' & abs(dB) < 0.2 & globalTrack.normalizedChi2 < 10' \
     + ' & globalTrack.hitPattern.numberOfValidMuonHits > 0 & numberOfMatches > 1'
    ),
    filter = cms.bool(False)
)

selIsoGlobalMuons = goodIsoMuons.clone(
    src = cms.InputTag("selGlobalMuons"),
    filter = cms.bool(False)
)

selMuonCandidates = cms.Sequence(
    patMuons
   * goodMuons * goodIsoMuons
   * selStandAloneMuons * selIsoStandAloneMuons
   * selGlobalMuons * selIsoGlobalMuons
)    
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# select isolated tracks
allChargedParticles = cms.EDProducer("ChargedRefCandidateProducer",
    src = cms.InputTag("trackWithVertexRefSelector"),
    particleType = cms.string("mu+") # only needed for mass hypotheses when building particle four-vector
)                                  

selChargedParticles = cms.EDFilter("ChargedRefCandidateSelector",
    src = cms.InputTag("allChargedParticles"),
    cut = cms.string(
        'pt > 10 & abs(eta) < 2.5' \
     + ' & track.normalizedChi2 < 10' \
     + ' & track.hitPattern.numberOfValidTrackerHits > 9 & track.hitPattern.numberOfValidPixelHits > 0'
    ),
    filter = cms.bool(False)
)

from TauAnalysis.RecoTools.patLeptonPFIsolationSelector_cfi import patMuonPFIsolationSelector
selIsoChargedParticles = cms.EDFilter("ChargedRefCandidatePFIsolationSelector",
    src = cms.InputTag("selChargedParticles"),
    pfCandidateSource = patMuonPFIsolationSelector.pfCandidateSource,
    chargedParticleIso = patMuonPFIsolationSelector.chargedParticleIso,
    neutralHadronIso = patMuonPFIsolationSelector.neutralHadronIso,
    photonIso = patMuonPFIsolationSelector.photonIso,
    sumPtMax = patMuonPFIsolationSelector.sumPtMax,
    sumPtMethod = patMuonPFIsolationSelector.sumPtMethod
)                                   

selChargedParticleCandidates = cms.Sequence(
    allChargedParticles
   * selChargedParticles * selIsoChargedParticles
)    
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# build Z --> mu+ mu- hypotheses for different combinations
# of isolated/non-isolated muons and isolated tracks
selZmumuCandidatesForMuonInnerTrackRecoEff = goldenZmumuCandidatesGe0IsoMuons.clone(
    decay = cms.string("goodIsoMuons@+ selIsoStandAloneMuons@-")
)

selZmumuCandidatesForMuonStandAloneTrackRecoEff = goldenZmumuCandidatesGe0IsoMuons.clone(
    decay = cms.string("goodIsoMuons@+ selIsoChargedParticles@-")
)

selZmumuCandidatesForMuonGlobalTrackRecoEff = goldenZmumuCandidatesGe0IsoMuons.clone(
    decay = cms.string("goodIsoMuons@+ selIsoChargedParticles@-")
)

selZmumuCandidatesForMuonIdEff = goldenZmumuCandidatesGe0IsoMuons.clone(
    decay = cms.string("goodIsoMuons@+ selIsoGlobalMuons@-")
)

selZmumuCandidatesForMuonIsoEff = goldenZmumuCandidatesGe0IsoMuons.clone(
    decay = cms.string("goodIsoMuons@+ goodMuons@-")
)

selZmumuCandidatesForMuonTriggerEff = goldenZmumuCandidatesGe0IsoMuons.clone(
    decay = cms.string("goodIsoMuons@+ goodIsoMuons@-")
)

selZmumuCandidates = cms.Sequence(
    selZmumuCandidatesForMuonInnerTrackRecoEff
   * selZmumuCandidatesForMuonStandAloneTrackRecoEff
   * selZmumuCandidatesForMuonGlobalTrackRecoEff
   * selZmumuCandidatesForMuonIdEff
   * selZmumuCandidatesForMuonIsoEff
   * selZmumuCandidatesForMuonTriggerEff
)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# defined filters for skimming events
# having at least one Z --> mu+ mu- hypothesis of any type
selZmumuFilterForMuonInnerTrackRecoEff = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("selZmumuCandidatesForMuonInnerTrackRecoEff"),
    minNumber = cms.uint32(1)
)

selZmumuFilterForMuonStandAloneTrackRecoEff = selZmumuFilterForMuonInnerTrackRecoEff.clone(
    src = cms.InputTag("selZmumuCandidatesForMuonStandAloneTrackRecoEff")
)

selZmumuFilterForMuonGlobalTrackRecoEff = selZmumuFilterForMuonInnerTrackRecoEff.clone(
    src = cms.InputTag("selZmumuCandidatesForMuonGlobalTrackRecoEff")
)

selZmumuFilterForMuonIdEff = selZmumuFilterForMuonInnerTrackRecoEff.clone(
    src = cms.InputTag("selZmumuCandidatesForMuonIdEff")
)

selZmumuFilterForMuonIsoEff = selZmumuFilterForMuonInnerTrackRecoEff.clone(
    src = cms.InputTag("selZmumuCandidatesForMuonIsoEff")
)

selZmumuFilterForMuonTriggerEff = selZmumuFilterForMuonInnerTrackRecoEff.clone(
    src = cms.InputTag("selZmumuCandidatesForMuonTriggerEff")
)
#--------------------------------------------------------------------------------

# Selection sequence
looseZmumuProductionSequence = cms.Sequence(
     zmmHLTFilter
    * goodVertex
    * pfNoPileUpSequence * trackWithVertexRefSelector
    * selMuonCandidates
    * selChargedParticleCandidates
    * goldenZmumuCandidatesGe0IsoMuons * goldenZmumuCandidatesGe1IsoMuons * goldenZmumuCandidatesGe2IsoMuons 
    * selZmumuCandidates
)
