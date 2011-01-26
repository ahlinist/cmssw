import FWCore.ParameterSet.Config as cms

### HLT filter
import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
ZToMMGHltFilter = copy.deepcopy(hltHighLevel)
ZToMMGHltFilter.throw = cms.bool(False)
ZToMMGHltFilter.HLTPaths = ["*Mu*"]

ZToMMGLooseMuons = cms.EDFilter("MuonViewRefSelector",
    src = cms.InputTag("muons"),
    cut = cms.string("""
        pt > 10 &
        (isGlobalMuon | isTrackerMuon) &
        abs(innerTrack().dxy) < 1.0
        """),
    filter = cms.bool(True)
    )

ZToMMGTightMuons = ZToMMGLooseMuons.clone(
    src = "ZToMMGLooseMuons",
    cut = "pt > 15"
    )

ZToMMGDimuons = cms.EDProducer("CandViewShallowClonePtrCombiner",
    decay = cms.string("ZToMMGTightMuons ZToMMGLooseMuons"),
    checkCharge = cms.bool(False),
    cut = cms.string("20 < mass")
    )

ZToMMGDimuonsFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("ZToMMGDimuons"),
    minNumber = cms.uint32(1)
    )

ZToMMGSkimFilterSequence = cms.Sequence(
    ZToMMGHltFilter *
    ZToMMGLooseMuons *
    ZToMMGTightMuons *
    ZToMMGDimuons *
    ZToMMGDimuonsFilter
    )
