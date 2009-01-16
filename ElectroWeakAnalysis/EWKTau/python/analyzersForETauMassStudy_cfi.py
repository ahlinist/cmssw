
import FWCore.ParameterSet.Config as cms
import copy

from ElectroWeakAnalysis.EWKTau.electronForETauAnalyzer_cfi import *
from ElectroWeakAnalysis.EWKTau.tauForETauAnalyzer_cfi import *
from ElectroWeakAnalysis.EWKTau.massForETauAnalyzer_cfi import *
from ElectroWeakAnalysis.EWKTau.etauMcAnalyzer_cfi import *

analyzeElectronForMassStudy=copy.deepcopy(analyzeElectronForETau)
analyzeElectronForMassStudy.ElecInputTags = cms.VInputTag(
    "allLayer1ElecForETau"
    ,"selectKinElecForETau"
    ,"selectMchHLTElecForETau"
    ,"selectTrkIsoAfterMchHLTElecForETau"
    ,"selectEcalIsoAfterTrkIsoElecForETau"
    ,"selectHcalIsoAfterEcalIsoElecForETau"
    ,"selectIdAfterIsoElecForETau"
    ,"selectIpAfterIsoElecForETau"
    )
analyzeElectronForMassStudy.RefInputTags = cms.InputTag("NoMatching")
analyzeElectronForMassStudy.DoMatching = False
analyzeElectronForMassStudy.DoTuning = cms.bool(True)
analyzeElectronForMassStudy.DoVeto = cms.bool(True)
analyzeElectronForMassStudy.EventWeight = cms.double(15.)

analyzeAntiElectronForMassStudy=copy.deepcopy(analyzeElectronForETau)
analyzeAntiElectronForMassStudy.ElecInputTags = cms.VInputTag(
    "allLayer1ElecForETau"
    ,"selectKinElecForETau"
    ,"selectMchHLTElecForETau"
    ,"selectAntiTrkIsoAfterMchHLTElecForETau"
    ,"selectAntiEcalIsoAfterTrkIsoElecForETau"
    ,"selectAntiHcalIsoAfterEcalIsoElecForETau"
    ,"selectAntiIdAfterIsoElecForETau"
    ,"selectAntiIpAfterIsoElecForETau"
    )
analyzeAntiElectronForMassStudy.RefInputTags = cms.InputTag("NoMatching")
analyzeAntiElectronForMassStudy.DoMatching = False
analyzeAntiElectronForMassStudy.DoTuning = cms.bool(True)
analyzeAntiElectronForMassStudy.DoVeto = cms.bool(True)
analyzeAntiElectronForMassStudy.EventWeight = cms.double(15.)

analyzeTauForMassStudy=copy.deepcopy(analyzeTauForETau)
analyzeTauForMassStudy.TauInputTags = cms.VInputTag(
    "selectorPfTausForETauKinEff",
    "selectorPfTausForETauElecSepEff",
    "selectorPfTausForETauLdgTrkEff",
    "selectorPfTausForETauLdgTrkPtEff",
    "selectorPfTausForETauEclIsodEff",
    "selectorPfTausForETauTrkIsodEff",
    "selectorPfTausForETauElecRejEff",
    "selectorPfTausForETauProngEff",
    "selectorPfTausForETauOSSSChargeSepEff"
    )
analyzeTauForMassStudy.FinalElecTag = "selectIpAfterIsoElecForETau"
analyzeTauForMassStudy.RefInputTags = ("NoMatching")
analyzeTauForMassStudy.DoMatching = cms.bool(False)
analyzeTauForMassStudy.DoTuning = cms.bool(True)
analyzeTauForMassStudy.DoTauIdAfterElecId = cms.bool(True)
analyzeTauForMassStudy.EventWeight = cms.double(15.)

analyzeAntiElecTauForMassStudy=copy.deepcopy(analyzeTauForETau)
analyzeAntiElecTauForMassStudy.TauInputTags = cms.VInputTag(
    "selectorPfTausForETauKinEff",
    "selectorPfTausForETauElecSepEff",
    "selectorPfTausForETauLdgTrkEff",
    "selectorPfTausForETauLdgTrkPtEff",
    "selectorPfTausForETauEclIsodEff",
    "selectorPfTausForETauTrkIsodEff",
    "selectorPfTausForETauElecRejEff",
    "selectorPfTausForETauProngEff",
    "selectorPfTausForETauOSSSChargeSepAntiEff"
    )
analyzeAntiElecTauForMassStudy.FinalElecTag = "selectAntiIpAfterIsoElecForETau"
analyzeAntiElecTauForMassStudy.RefInputTags = ("NoMatching")
analyzeAntiElecTauForMassStudy.DoMatching = cms.bool(False)
analyzeAntiElecTauForMassStudy.DoTuning = cms.bool(True)
analyzeAntiElecTauForMassStudy.DoTauIdAfterElecId = cms.bool(True)
analyzeAntiElecTauForMassStudy.EventWeight = cms.double(15.)


analyzeMassForMassStudy=copy.deepcopy(analyzeMassForETau)
analyzeMassForMassStudy.TauTag = cms.InputTag("selectorPfTausForETauLdgTrkPtEff")
analyzeMassForMassStudy.ElecTag = cms.InputTag("selectIpAfterIsoElecForETau")
analyzeMassForMassStudy.MetTag = cms.InputTag("selectedLayer1METs")
analyzeMassForMassStudy.RefInputTags = cms.VInputTag("NoMatching",
                                                     "NoMatching",
                                                     "NoMatching")
analyzeMassForMassStudy.DoMatching = cms.bool(False)
analyzeMassForMassStudy.EventWeight = cms.double(15.)

analyzeAntiElecMassForMassStudy=copy.deepcopy(analyzeMassForETau)
analyzeAntiElecMassForMassStudy.TauTag = cms.InputTag("selectorPfTausForETauLdgTrkPtEff")
analyzeAntiElecMassForMassStudy.ElecTag = cms.InputTag("selectAntiIpAfterIsoElecForETau")
analyzeAntiElecMassForMassStudy.MetTag = cms.InputTag("selectedLayer1METs")
analyzeAntiElecMassForMassStudy.RefInputTags = cms.VInputTag("NoMatching",
                                                             "NoMatching",
                                                             "NoMatching")
analyzeAntiElecMassForMassStudy.DoMatching = cms.bool(False)
analyzeAntiElecMassForMassStudy.EventWeight = cms.double(15.)
