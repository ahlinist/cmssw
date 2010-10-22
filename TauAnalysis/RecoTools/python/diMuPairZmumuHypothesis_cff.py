import FWCore.ParameterSet.Config as cms

from TauAnalysis.RecoTools.patLeptonPFIsolationSelector_cfi import patMuonPFIsolationSelector

#--------------------------------------------------------------------------------
# produce combinations of muon + muon pairs,
# the hypothesis being that the pair of muons results from a Z --> mu+ mu- decay
#--------------------------------------------------------------------------------

# require muon candidates considered for Z --> mu+ mu- hypothesis
# to be reconstructed in muon system
# (with or without a track reconstructed in Pixel/SiStrip tracking detectors linked to it)
selectedPatMuonsForZmumuHypothesesMuonTrack = cms.EDFilter("PATMuonSelector",
    src = cms.InputTag("cleanPatMuons"),
    cut = cms.string('isGlobalMuon() | isStandAloneMuon()'),
    filter = cms.bool(False)
)

selectedPatMuonsForZmumuHypothesesPt10 = cms.EDFilter("PATMuonSelector",
    src = cms.InputTag("selectedPatMuonsForZmumuHypothesesMuonTrack"),
    cut = cms.string('pt > 10.'),
    filter = cms.bool(False)
)

selectedPatMuonsForZmumuHypothesesLoosePFRelIso = cms.EDFilter("PATMuonPFIsolationSelector",
    patMuonPFIsolationSelector.clone(
        sumPtMax = cms.double(0.25)
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
    srcLeg1 = cms.InputTag('selectedPatMuonsForZmumuHypothesesMuonTrack'),
    srcLeg2 = cms.InputTag('selectedPatMuonsForZmumuHypothesesMuonTrack'),
    dRmin12 = cms.double(0.5),
    srcMET = cms.InputTag(''),
    recoMode = cms.string(""),
    scaleFuncImprovedCollinearApprox = cms.string('1'),                                        
    verbosity = cms.untracked.int32(0)
)

selectedDiMuPairZmumuHypothesesByMass = cms.EDFilter("PATDiMuPairSelector",
    src = cms.InputTag("allDiMuPairZmumuHypothesesByMass"),                                   
    cut = cms.string('p4Vis.mass > 80. & p4Vis.mass < 100.'),
    filter = cms.bool(False)
)

allDiMuPairZmumuHypothesesByLooseIsolationAndCharge = cms.EDProducer("PATDiMuPairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('selectedPatMuonsForZmumuHypothesesLoosePFRelIso'),
    srcLeg2 = cms.InputTag('selectedPatMuonsForZmumuHypothesesLoosePFRelIso'),
    dRmin12 = cms.double(0.5),
    srcMET = cms.InputTag(''),
    recoMode = cms.string(""),
    scaleFuncImprovedCollinearApprox = cms.string('1'),                                        
    verbosity = cms.untracked.int32(0)
)

selectedDiMuPairZmumuHypothesesByLooseIsolationAndCharge = cms.EDFilter("PATDiMuPairSelector",
    src = cms.InputTag("allDiMuPairZmumuHypothesesByLooseIsolationAndCharge"),                                   
    cut = cms.string('charge = 0'),
    filter = cms.bool(False)
)

produceDiMuPairs = cms.Sequence(
    selectedPatMuonsForZmumuHypotheses
   * allDiMuPairZmumuHypothesesByMass * selectedDiMuPairZmumuHypothesesByMass
   * allDiMuPairZmumuHypothesesByLooseIsolationAndCharge * selectedDiMuPairZmumuHypothesesByLooseIsolationAndCharge
)
