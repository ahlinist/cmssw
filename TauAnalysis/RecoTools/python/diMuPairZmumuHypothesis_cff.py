import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# produce combinations of muon + muon pairs,
# the hypothesis being that the pair of muons results from a Z --> mu+ mu- decay
#--------------------------------------------------------------------------------

# require muon candidates considered for Z --> mu+ mu- hypothesis
# to be reconstructed in muon system
# (with or without a track reconstructed in Pixel/SiStrip tracking detectors linked to it)
selectedLayer1MuonsForZmumuHypotheses = cms.EDFilter("PATMuonSelector",
    src = cms.InputTag("cleanLayer1Muons"),
    cut = cms.string('isGlobalMuon() | isStandAloneMuon()'),
    filter = cms.bool(False)
)

allDiMuPairZmumuHypotheses = cms.EDProducer("DiCandidatePairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('selectedLayer1MuonsForZmumuHypotheses'),
    srcLeg2 = cms.InputTag('selectedLayer1MuonsForZmumuHypotheses'),
    dRmin12 = cms.double(0.5),
    srcMET = cms.InputTag(''),
    recoMode = cms.string(""),
    scaleFuncImprovedCollinearApprox = cms.string('1'),                                        
    verbosity = cms.untracked.int32(0)
)

selectedDiMuPairZmumuHypotheses = cms.EDFilter("DiCandidatePairSelector",
    src = cms.InputTag("allDiMuPairZmumuHypotheses"),                                   
    cut = cms.string('p4Vis.mass > 80. & p4Vis.mass < 100.'),
    filter = cms.bool(False)
)

produceDiMuPairs = cms.Sequence( selectedLayer1MuonsForZmumuHypotheses * allDiMuPairZmumuHypotheses * selectedDiMuPairZmumuHypotheses )
