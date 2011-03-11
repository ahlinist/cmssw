import FWCore.ParameterSet.Config as cms

muonsCountFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("muons"),
    minNumber = cms.uint32(2)
    )

goodLooseMuons = cms.EDFilter("MuonViewRefSelector",
    src = cms.InputTag("muons"),
    cut = cms.string("""
            pt > 10 &
            (isGlobalMuon | isTrackerMuon) &
            abs(innerTrack().dxy) < 1.0
            """),
    filter = cms.bool(True)
    )

goodLooseMuonsCountFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("goodLooseMuons"),
    minNumber = cms.uint32(2)
    )

goodTightMuons = goodLooseMuons.clone(
    src = "goodLooseMuons",
    cut = "pt > 15"
    )

goodDimuons = cms.EDProducer("CandViewShallowClonePtrCombiner",
    decay = cms.string("goodTightMuons goodLooseMuons"),
    checkCharge = cms.bool(False),
    cut = cms.string("20 < mass")
    )

goodDimuonsFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("goodDimuons"),
    minNumber = cms.uint32(1)
    )

dimuonSkimFilterSequence = cms.Sequence(
    muonsCountFilter +
    (goodLooseMuons + goodLooseMuonsCountFilter) *
    goodTightMuons *
    goodDimuons *
    goodDimuonsFilter
    )
