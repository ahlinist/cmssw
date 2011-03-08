import FWCore.ParameterSet.Config as cms

### HLT filter
import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
ZToMMGHltFilter = copy.deepcopy(hltHighLevel)
ZToMMGHltFilter.throw = cms.bool(False)
ZToMMGHltFilter.HLTPaths = ["*Mu*"]

## Alternative HLT paths (may loose some efficiency)
# ZToMMGHltFilter.HLTPaths = """
#     HLT_Mu9
#     HLT_Mu11
#     HLT_Mu15_v1
#     HLT_IsoMu9
#     HLT_IsoMu13_v3
#     HLT_DoubleMu3_v2
#     """.split()

ZToMMGMuonsCountFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("muons"),
    minNumber = cms.uint32(2)
    )

ZToMMGLooseMuons = cms.EDFilter("MuonViewRefSelector",
    src = cms.InputTag("muons"),
    cut = cms.string("""
        pt > 10 &
        isGlobalMuon &
        isTrackerMuon &
        abs(innerTrack().dxy) < 2.0
        """),
    filter = cms.bool(True)
    )

ZToMMGLooseMuonsCountFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("ZToMMGLooseMuons"),
    minNumber = cms.uint32(2)
    )

ZToMMGTightMuons = ZToMMGLooseMuons.clone(
    src = "ZToMMGLooseMuons",
    cut = "pt > 20"
    )

ZToMMGDimuons = cms.EDProducer("CandViewShallowClonePtrCombiner",
    decay = cms.string("ZToMMGTightMuons ZToMMGLooseMuons"),
    checkCharge = cms.bool(False),
    cut = cms.string("30 < mass")
    )

ZToMMGDimuonsFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("ZToMMGDimuons"),
    minNumber = cms.uint32(1)
    )

ZToMMGSkimFilterSequence = cms.Sequence(
    ZToMMGHltFilter +
    ZToMMGMuonsCountFilter +
    (ZToMMGLooseMuons + ZToMMGLooseMuonsCountFilter) *
    ZToMMGTightMuons *
    ZToMMGDimuons *
    ZToMMGDimuonsFilter
    )
