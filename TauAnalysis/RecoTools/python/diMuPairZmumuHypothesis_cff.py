import FWCore.ParameterSet.Config as cms

from TauAnalysis.RecoTools.patLeptonPFIsolationSelector_cfi import patMuonPFIsolationSelector

#--------------------------------------------------------------------------------
# produce combinations of muon + muon pairs,
# the hypothesis being that the pair of muons results from a Z --> mu+ mu- decay
#--------------------------------------------------------------------------------

selectedPatMuonsForZmumuHypothesesMuonTrack = cms.EDFilter("PATMuonSelector",
    src = cms.InputTag("cleanPatMuons"),
    cut = cms.string('isGlobalMuon()'),
    filter = cms.bool(False)
)

selectedPatMuonsForZmumuHypothesesPt10 = cms.EDFilter("PATMuonSelector",
    src = cms.InputTag("selectedPatMuonsForZmumuHypothesesMuonTrack"),
    cut = cms.string('pt > 10.'),
    filter = cms.bool(False)
)

selectedPatMuonsForZmumuHypothesesLoosePFRelIso = cms.EDFilter("PATMuonPFIsolationSelector",
    patMuonPFIsolationSelector.clone(
        sumPtMax = cms.double(0.26)
    ),
    src = cms.InputTag("selectedPatMuonsForZmumuHypothesesPt10"),                                                           
    filter = cms.bool(False)
)

selectedPatMuonsForZmumuHypotheses = cms.Sequence(
    selectedPatMuonsForZmumuHypothesesMuonTrack
   * selectedPatMuonsForZmumuHypothesesPt10
   * selectedPatMuonsForZmumuHypothesesLoosePFRelIso
)    

allDiMuPairZmumuHypothesesByMass = cms.EDProducer("PATDiMuPairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('selectedPatMuonsTrkIPcumulative'),
    srcLeg2 = cms.InputTag('selectedPatMuonsForZmumuHypothesesPt10'),
    dRmin12 = cms.double(0.5),
    srcMET = cms.InputTag(''),
    recoMode = cms.string(""),
    verbosity = cms.untracked.int32(0)
)

selectedDiMuPairZmumuHypothesesByMass = cms.EDFilter("PATDiMuPairSelector",
    src = cms.InputTag("allDiMuPairZmumuHypothesesByMass"),                                   
    cut = cms.string('charge = 0 & p4Vis.mass > 80. & p4Vis.mass < 100.'),
    filter = cms.bool(False)
)

allDiMuPairZmumuHypothesesByLooseIsolation = cms.EDProducer("PATDiMuPairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('selectedPatMuonsTrkIPcumulative'),
    srcLeg2 = cms.InputTag('selectedPatMuonsForZmumuHypothesesLoosePFRelIso'),
    dRmin12 = cms.double(0.5),
    srcMET = cms.InputTag(''),
    recoMode = cms.string(""),
    verbosity = cms.untracked.int32(0)
)

selectedDiMuPairZmumuHypothesesByLooseIsolation = cms.EDFilter("PATDiMuPairSelector",
    src = cms.InputTag("allDiMuPairZmumuHypothesesByLooseIsolation"),                                   
    cut = cms.string('charge = 0'),
    filter = cms.bool(False)
)

produceDiMuPairs = cms.Sequence(
    selectedPatMuonsForZmumuHypotheses
   * allDiMuPairZmumuHypothesesByMass * selectedDiMuPairZmumuHypothesesByMass
   * allDiMuPairZmumuHypothesesByLooseIsolation * selectedDiMuPairZmumuHypothesesByLooseIsolation
)
