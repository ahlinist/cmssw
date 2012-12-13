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

from TrackingTools.TransientTrack.TransientTrackBuilder_cfi import TransientTrackBuilderESProducer

from CommonTools.ParticleFlow.pfNoPileUp_cff import *
pfPileUp.Enable = cms.bool(True)
pfPileUp.checkClosestZVertex = cms.bool(True)
pfPileUp.PFCandidates = cms.InputTag('particleFlow')
pfNoPileUp.bottomCollection = cms.InputTag('particleFlow')

from CommonTools.ParticleFlow.pfParticleSelection_cff import *

from PhysicsTools.PatAlgos.producersLayer1.muonProducer_cfi import patMuons

# compute muon IsoDeposits and add muon isolation sums to pat::Muon objects
from RecoMuon.MuonIsolation.muonPFIsolation_cff import *
muPFIsoDepositChargedForPAT = muPFIsoDepositCharged.clone(
    src = cms.InputTag('muons')
)    
muPFIsoDepositNeutralForPAT = muPFIsoDepositNeutral.clone(
    src = cms.InputTag('muons')
)    
muPFIsoDepositGammaForPAT = muPFIsoDepositGamma.clone(
    src = cms.InputTag('muons')
)    
muPFIsoDepositChargedAllForPAT = muPFIsoDepositChargedAll.clone(
    src = cms.InputTag('muons')
)    
muPFIsoDepositPUforPAT = muPFIsoDepositPU.clone(
    src = cms.InputTag('muons')
)
muonPFIsolationSequenceForPAT = cms.Sequence(
    muPFIsoDepositChargedForPAT
   * muPFIsoDepositNeutralForPAT
   * muPFIsoDepositGammaForPAT
   * muPFIsoDepositChargedAllForPAT
   * muPFIsoDepositPUforPAT
)
patMuonsForGoldenZmmSelection = patMuons.clone(
    isoDeposits = cms.PSet(
        # CV: strings for IsoDeposits defined in PhysicsTools/PatAlgos/plugins/PATMuonProducer.cc
        pfChargedHadrons = cms.InputTag("muPFIsoDepositChargedForPAT"),
        pfNeutralHadrons = cms.InputTag("muPFIsoDepositNeutralForPAT"),
        pfPhotons = cms.InputTag("muPFIsoDepositGammaForPAT"),
        user = cms.VInputTag(
            cms.InputTag("muPFIsoDepositChargedAllForPAT"),
            cms.InputTag("muPFIsoDepositPUforPAT")
       )
    ),
    addGenMatch = cms.bool(False),
    embedHighLevelSelection = cms.bool(True),
    embedCaloMETMuonCorrs = cms.bool(False),
    embedTcMETMuonCorrs = cms.bool(False),
    usePV = cms.bool(False) # compute transverse impact parameter wrt. beamspot (not event vertex)
)

patMuonsForGoldenZmmSelection.userIsolation = cms.PSet(
    # CV: strings for Isolation values defined in PhysicsTools/PatAlgos/src/MultiIsolator.cc
    pfChargedHadron = cms.PSet(
        deltaR = cms.double(0.4),
        src = patMuonsForGoldenZmmSelection.isoDeposits.pfChargedHadrons,
        vetos = muPFIsoValueCharged04.deposits[0].vetos,
        skipDefaultVeto = muPFIsoValueCharged04.deposits[0].skipDefaultVeto
    ),
    pfNeutralHadron = cms.PSet(
        deltaR = cms.double(0.4),
        src = patMuonsForGoldenZmmSelection.isoDeposits.pfNeutralHadrons,
        vetos = muPFIsoValueNeutral04.deposits[0].vetos,
        skipDefaultVeto = muPFIsoValueNeutral04.deposits[0].skipDefaultVeto
    ),
    pfGamma = cms.PSet(
        deltaR = cms.double(0.4),
        src = patMuonsForGoldenZmmSelection.isoDeposits.pfPhotons,
        vetos = muPFIsoValueGamma04.deposits[0].vetos,
        skipDefaultVeto = muPFIsoValueGamma04.deposits[0].skipDefaultVeto
    ),
    user = cms.VPSet(
        cms.PSet(
            deltaR = cms.double(0.4),
            src = patMuonsForGoldenZmmSelection.isoDeposits.user[0],
            vetos = muPFIsoValueChargedAll04.deposits[0].vetos,
            skipDefaultVeto = muPFIsoValueChargedAll04.deposits[0].skipDefaultVeto
        ),
        cms.PSet(
            deltaR = cms.double(0.4),
            src = patMuonsForGoldenZmmSelection.isoDeposits.user[1],
            vetos = muPFIsoValuePU04.deposits[0].vetos,
            skipDefaultVeto = muPFIsoValuePU04.deposits[0].skipDefaultVeto
        )
    )
)

# Trigger requirements
import HLTrigger.HLTfilters.hltHighLevel_cfi
zmmHLTFilter = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
zmmHLTFilter.TriggerResultsTag = cms.InputTag("TriggerResults", "", "HLT")
zmmHLTFilter.HLTPaths = [
    # double muon triggers (2012 Run A+B+C)
    'HLT_Mu17_Mu8_v16',
    'HLT_Mu17_Mu8_v17',
    'HLT_Mu17_Mu8_v18',
    'HLT_Mu17_Mu8_v19',
    'HLT_Mu17_Mu8_v21',
    # double muon triggers (Summer'12 MC, produced with CMSSW_5_3_x)
    'HLT_Mu17_Mu8_v13'
]
zmmHLTFilter.throw = cms.bool(False)

# Vertex selection
goodVertex = cms.EDFilter("VertexSelector",
    src = cms.InputTag("offlinePrimaryVertices"),
    cut = cms.string("isValid & ndof >= 4 & abs(z) < 24 & abs(position.Rho) < 2"),
    filter = cms.bool(True)
)

# Cuts for both muons, no isolation cuts applied
goodMuons = cms.EDFilter("PATMuonSelector",
  src = cms.InputTag("patMuonsForGoldenZmmSelection"),
    cut = cms.string(
      'pt > 20 & abs(eta) < 2.5 & isGlobalMuon' \
     + ' & innerTrack.hitPattern.numberOfValidTrackerHits > 9 & innerTrack.hitPattern.numberOfValidPixelHits > 0' \
     + ' & abs(dB) < 0.2 & globalTrack.normalizedChi2 < 10' \
     + ' & globalTrack.hitPattern.numberOfValidMuonHits > 0 & numberOfMatches > 1' 
    ),
    filter = cms.bool(True)
)

# Cuts for muon leg with isolation cut applied
goodIsoMuons = cms.EDFilter("PATMuonSelector",
    src = cms.InputTag("goodMuons"),
    cut = cms.string(
        '(userIsolation("pat::User1Iso")' + \
        ' + max(0., userIsolation("pat::PfNeutralHadronIso") + userIsolation("pat::PfGammaIso")' + \
        '          - 0.5*userIsolation("pat::User2Iso"))) < 0.06*pt'
    ),                                
    filter = cms.bool(False)
)

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
    minNumber = cms.uint32(2)
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
    * pfParticleSelectionSequence
    * muonPFIsolationSequenceForPAT
    * patMuonsForGoldenZmmSelection * goodMuons * goodIsoMuons
    * goldenZmumuCandidatesGe0IsoMuons * goldenZmumuCandidatesGe1IsoMuons * goldenZmumuCandidatesGe2IsoMuons
    * goodMuonFilter * goldenZmumuFilter
    * goodMuonIsolationTagAndProbeProducer
)


