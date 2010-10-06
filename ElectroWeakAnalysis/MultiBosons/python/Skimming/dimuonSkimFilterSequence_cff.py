import FWCore.ParameterSet.Config as cms

goodLooseMuons = cms.EDFilter("CandViewRefSelector",
    src = cms.InputTag("muons"),
    cut = cms.string("""
            pt > 10 &
            (isGlobalMuon | isTrackerMuon) &
            abs(innerTrack().dxy) < 1.0
            """),
    filter = cms.bool(True)
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
    goodLooseMuons *
    goodTightMuons *
    goodDimuons *
    goodDimuonsFilter
    )
