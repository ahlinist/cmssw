import FWCore.ParameterSet.Config as cms

### HLT filter
import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
ZToEEGHltFilter = copy.deepcopy(hltHighLevel)
ZToEEGHltFilter.throw = cms.bool(False)
ZToEEGHltFilter.HLTPaths = """
  HLT_Ele15_LW_L1R
  HLT_Ele15_SW_L1R
  HLT_Ele15_SW_CaloEleId_L1R
  HLT_Ele17_SW_CaloEleId_L1R
  HLT_Ele17_SW_L1R
  HLT_Ele17_SW_TightEleId_L1R
  HLT_Ele17_SW_TighterEleIdIsol_L1R_v2
  HLT_Ele17_SW_TighterEleIdIsol_L1R_v3
  HLT_Ele17_SW_TightCaloEleId_SC8HE_L1R
  HLT_DoubleEle10_SW_L1R
  HLT_DoubleEle15_SW_L1R_v1
  HLT_DoubleEle17_SW_L1R_v1
  HLT_Ele17_SW_TightCaloEleId_SC8HE_L1R_v1
  HLT_Ele17_SW_TightCaloEleId_Ele8HE_L1R_v1
  HLT_Ele17_SW_TightCaloEleId_Ele8HE_L1R_v2
  """.split()

ZToEEGLooseElectrons = cms.EDFilter("GsfElectronViewRefSelector",
    src = cms.InputTag("gsfElectrons"),
    cut = cms.string("""
        pt                                                 > 10       &
        dr03TkSumPt                                        < 0.1 * pt &
        dr03HcalTowerSumEt                                 < 0.1 * pt &
        hcalOverEcal                                       < 0.1      &
        gsfTrack().trackerExpectedHitsInner().numberOfHits > 1        &
        (
          (isEB & sigmaIetaIeta < 0.014) |
          (isEE & sigmaIetaIeta < 0.035)
        )
        """),
    filter = cms.bool(True)
    )

ZToEEGTightElectrons = cms.EDFilter("CandViewRefSelector",
    src = cms.InputTag("ZToEEGLooseElectrons"),
    cut = cms.string("pt > 15"),
    filter = cms.bool(True)
    )

ZToEEGDielectrons = cms.EDProducer("CandViewShallowClonePtrCombiner",
    decay = cms.string("ZToEEGTightElectrons ZToEEGLooseElectrons"),
    checkCharge = cms.bool(False),
    cut = cms.string("35 < mass")
    )

ZToEEGDielectronsFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("ZToEEGDielectrons"),
    minNumber = cms.uint32(1)
    )

ZToEEGSkimFilterSequence = cms.Sequence(
    ZToEEGHltFilter          *
    ZToEEGLooseElectrons     *
    ZToEEGTightElectrons     *
    ZToEEGDielectrons        *
    ZToEEGDielectronsFilter
    )
