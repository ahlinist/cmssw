import FWCore.ParameterSet.Config as cms

from TauAnalysis.RecoTools.patLeptonPFIsolationSelector_cfi import patMuonPFIsolationSelector

#--------------------------------------------------------------------------------
# produce combinations of muon + muon pairs,
# the hypothesis being that the pair of muons results from a Z --> mu+ mu- decay
#--------------------------------------------------------------------------------

selectedPatMuonsForDYmumuHypothesesMuonTrack = cms.EDFilter("PATMuonSelector",
    src = cms.InputTag("cleanPatMuons"),
    cut = cms.string('isGlobalMuon()'),
    filter = cms.bool(False)
)

selectedPatMuonsForDYmumuHypothesesEta24 = cms.EDFilter("PATMuonSelector",
    src = cms.InputTag("selectedPatMuonsForDYmumuHypothesesMuonTrack"),
    cut = cms.string('abs(eta) < 2.4'),
    filter = cms.bool(False)
)

selectedPatMuonsForDYmumuHypotheses = cms.Sequence(
    selectedPatMuonsForDYmumuHypothesesMuonTrack
   * selectedPatMuonsForDYmumuHypothesesEta24
)    

allDiMuPairDYmumuHypotheses = cms.EDProducer("PATDiMuPairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('selectedPatMuonsTrkIPcumulative'),
    srcLeg2 = cms.InputTag('selectedPatMuonsForDYmumuHypothesesEta24'),
    dRmin12 = cms.double(0.01),
    srcMET = cms.InputTag(''),
    recoMode = cms.string(""),
    verbosity = cms.untracked.int32(0)
)

selectedDiMuPairDYmumuHypotheses = cms.EDFilter("PATDiMuPairSelector",
    src = cms.InputTag("allDiMuPairDYmumuHypotheses"),
    cut = cms.string('charge = 0 & dR12 < 1.0'),
    filter = cms.bool(False)
)

produceDiMuPairsDYmumuHypotheses = cms.Sequence(
    selectedPatMuonsForDYmumuHypotheses
   * allDiMuPairDYmumuHypotheses * selectedDiMuPairDYmumuHypotheses
)
