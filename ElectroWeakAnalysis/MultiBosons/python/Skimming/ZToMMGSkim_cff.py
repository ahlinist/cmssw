import FWCore.ParameterSet.Config as cms

### HLT filter
import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
ZToMMGHltFilter = copy.deepcopy(hltHighLevel)
ZToMMGHltFilter.throw = cms.bool(False)
ZToMMGHltFilter.HLTPaths = """
  HLT_Mu9
  HLT_Mu11
  HLT_Mu15
  HLT_Mu15_v*

  HLT_DoubleMu3_v2
  HLT_DoubleMu5_v1
  HLT_IsoMu9_PFTau15_v1
  HLT_L2DoubleMu20_NoVertex_v1
  HLT_Mu11_PFTau15_v1
  HLT_Mu15_v1
  HLT_Mu3_Ele8_HT70U_v1
  HLT_Mu5_HT70U_v1
  HLT_Mu5_Jet50U_v1
  HLT_Mu5_MET45_v1
  OpenHLT_DoubleMu3_HT50U
  OpenHLT_IsoMu15
  OpenHLT_IsoMu17
  OpenHLT_Mu11_Ele8
  OpenHLT_Mu17
  OpenHLT_Mu19
  OpenHLT_Mu21
  OpenHLT_Mu30_NoVertex
  OpenHLT_Mu5_Ele13
  OpenHLT_Mu5_Ele17
  OpenHLT_Mu5_HT100U
  OpenHLT_Mu5_Jet70U
  OpenHLT_Mu7_Photon13_L1R
  OpenHLT_Mu8_Ele8
  OpenHLT_IsoMu13
  """.split()


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
    ZToMMGLooseMuons *
    ZToMMGTightMuons *
    ZToMMGDimuons *
    ZToMMGDimuonsFilter
    )
