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
  HLT_Ele17_SW_TightCaloEleId_SC8HE_L1R
  """.split()


# 196        if (eleRef->dr03TkSumPt()/eleRef->pt()>eb_trIsoCut_) return false;
# 197        if (eleRef->dr03EcalRecHitSumEt()/eleRef->pt()>eb_ecalIsoCut_) return false;
# 198        if (eleRef->dr03HcalTowerSumEt()/eleRef->pt()>eb_hcalIsoCut_) return false;
# 199        if (eleRef->sigmaIetaIeta()>eb_seeCut_) return false;
# 200        if (eleRef->hcalOverEcal()>eb_hoeCut_) return false;
# if (eleRef->gsfTrack()->trackerExpectedHitsInner().numberOfHits()>missHitCut_) return false;

# 008     ptCut = cms.double(20.),
# 009     missHitsCut = cms.int32(1),

# 010     eb_trIsoCut = cms.double(0.1),
# 011     eb_ecalIsoCut = cms.double(0.1),
# 012     eb_hcalIsoCut = cms.double(0.1),
# 013     eb_hoeCut = cms.double(0.1),

# 014     eb_seeCut = cms.double(0.014),

# 015     ee_trIsoCut = cms.double(0.1),
# 016     ee_ecalIsoCut = cms.double(0.1),
# 017     ee_hcalIsoCut = cms.double(0.1),
# 018     ee_hoeCut = cms.double(0.1),

# 019     ee_seeCut = cms.double(0.035),

# 021     invMassCut = cms.double(40.)

ZToEEGLooseElectrons = cms.EDFilter("GsfElectronViewRefSelector",
    src = cms.InputTag("gsfElectrons"),
    cut = cms.string("""
        pt                                                 > 10       &
        dr03TkSumPt                                        < 0.1 * pt &
        dr03HcalTowerSumEt                                 < 0.1 * pt &
        hcalOverEcal                                       < 0.1      &
        gsfTrack().trackerExpectedHitsInner().numberOfHits > 1
        """),
    filter = cms.bool(True)
    )

ZToEEGLooseElectronsEB = cms.EDFilter("GsfElectronViewRefSelector",
    src = cms.InputTag("ZToEEGLooseElectrons"),
    cut = cms.string("isEB & sigmaIetaIeta < 0.014"),
    filter = cms.bool(False)
    )

ZToEEGLooseElectronsEE = cms.EDFilter("GsfElectronViewRefSelector",
    src = cms.InputTag("ZToEEGLooseElectrons"),
    cut = cms.string("isEE & sigmaIetaIeta < 0.035"),
    filter = cms.bool(False)
    )

ZToEEGLooseElectronsEBEE = cms.EDProducer("CandViewRefMerger",
    src = cms.VInputTag("ZToEEGLooseElectronsEB",
                        "ZToEEGLooseElectronsEE"
                        )
    )

ZToEEGTightElectrons = ZToEEGLooseElectrons.clone(
    src = "ZToEEGLooseElectronsEBEE",
    cut = "pt > 15",
    filter = cms.bool(True)
    )

ZToEEGDielectrons = cms.EDProducer("CandViewShallowClonePtrCombiner",
    decay = cms.string("ZToEEGTightElectrons@+ ZToEEGLooseElectrons@-"),
    checkCharge = cms.bool(True),
    cut = cms.string("35 < mass")
    )

ZToEEGDielectronsFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("ZToEEGDielectrons"),
    minNumber = cms.uint32(1)
    )

ZToEEGSkimFilterSequence = cms.Sequence(
    ZToEEGLooseElectrons     *
    ZToEEGLooseElectronsEB   *
    ZToEEGLooseElectronsEE   *
    ZToEEGLooseElectronsEBEE *
    ZToEEGTightElectrons     *
    ZToEEGDielectrons        *
    ZToEEGDielectronsFilter
    )
