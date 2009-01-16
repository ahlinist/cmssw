
import FWCore.ParameterSet.Config as cms
import copy

from ElectroWeakAnalysis.EWKTau.electronForETauAnalyzer_cfi import *
from ElectroWeakAnalysis.EWKTau.tauForETauAnalyzer_cfi import *
from ElectroWeakAnalysis.EWKTau.massForETauAnalyzer_cfi import *
from ElectroWeakAnalysis.EWKTau.etauMcAnalyzer_cfi import *

analyzeElectronForETau=copy.deepcopy(analyzeElectronForETau)
analyzeElectronForETau.ElecInputTags = cms.VInputTag(
    "allLayer1ElecForETau"
    ,"selectKinElecForETau"
    ,"selectMchHLTElecForETau"
    ,"selectTrkIsoAfterMchHLTElecForETau"
    ,"selectEcalIsoAfterTrkIsoElecForETau"
    ,"selectHcalIsoAfterEcalIsoElecForETau"
    ,"selectIdAfterIsoElecForETau"
    ,"selectIpAfterIsoElecForETau"
    )
analyzeElectronForETau.RefInputTags = cms.InputTag("NoMatching")
analyzeElectronForETau.DoMatching = False
analyzeElectronForETau.DoTuning = cms.bool(True)
analyzeElectronForETau.DoVeto = cms.bool(True)
analyzeElectronForETau.EventWeight = cms.double(15.)

analyzeElectronForLooseETau=copy.deepcopy(analyzeElectronForETau)
analyzeElectronForLooseETau.ElecInputTags = cms.VInputTag(
    "allLayer1ElecForETau"
    ,"selectKinElecForETau"
    ,"selectMchHLTElecForETau"
    ,"selectAntiTrkIsoAfterMchHLTElecForETau"
    ,"selectAntiEcalIsoAfterTrkIsoElecForETau"
    ,"selectAntiHcalIsoAfterEcalIsoElecForETau"
    ,"selectAntiIdAfterIsoElecForETau"
    ,"selectAntiIpAfterIsoElecForETau"
    )
analyzeElectronForLooseETau.RefInputTags = cms.InputTag("NoMatching")
analyzeElectronForLooseETau.DoMatching = False
analyzeElectronForLooseETau.DoTuning = cms.bool(True)
analyzeElectronForLooseETau.DoVeto = cms.bool(True)
analyzeElectronForLooseETau.EventWeight = cms.double(15.)

analyzeTauForETau=copy.deepcopy(analyzeTauForETau)
analyzeTauForETau.TauInputTags = cms.VInputTag(
    "selectorPfTausForETauKinEff",
    "selectorPfTausForETauElecSepEff",
    "selectorPfTausForETauLdgTrkEff",
    "selectorPfTausForETauLdgTrkPtEff",
    "selectorPfTausForETauEclIsodEff",
    "selectorPfTausForETauTrkIsodEff",
    "selectorPfTausForETauElecRejEff",
    "selectorPfTausForETauProngEff",
    "selectorPfTausForOSSSChargeSepEff"
    )
analyzeTauForETau.FinalElecTag = "selectIpAfterIsoElecForETau"
analyzeTauForETau.RefInputTags = ("NoMatching")
analyzeTauForETau.DoMatching = cms.bool(False)
analyzeTauForETau.DoTuning = cms.bool(True)
analyzeTauForETau.DoTauIdAfterElecId = cms.bool(True)
analyzeTauForETau.EventWeight = cms.double(15.)

analyzeTauForLooseETau=copy.deepcopy(analyzeTauForETau)
analyzeTauForLooseETau.TauInputTags = cms.VInputTag(
    "selectorPfTausForETauKinEff",
    "selectorPfTausForETauElecSepEff",
    "selectorPfTausForETauLdgTrkEff",
    "selectorPfTausForETauLdgTrkPtEff",
    "selectorPfTausForETauEclIsodEff",
    "selectorPfTausForETauTrkIsodEff",
    "selectorPfTausForETauElecRejEff",
    "selectorPfTausForETauProngEff",
    "selectorPfTausForOSSSChargeSepAntiEff"
    )
analyzeTauForLooseETau.FinalElecTag = "selectAntiIpAfterIsoElecForETau"
analyzeTauForLooseETau.RefInputTags = ("NoMatching")
analyzeTauForLooseETau.DoMatching = cms.bool(False)
analyzeTauForLooseETau.DoTuning = cms.bool(True)
analyzeTauForLooseETau.DoTauIdAfterElecId = cms.bool(True)
analyzeTauForLooseETau.EventWeight = cms.double(15.)


analyzeMassForETau=copy.deepcopy(analyzeMassForETau)
analyzeMassForETau.TauTag = cms.InputTag("selectorPfTausForOSSSChargeSepEff")
analyzeMassForETau.ElecTag = cms.InputTag("selectIpAfterIsoElecForETau")
analyzeMassForETau.MetTag = cms.InputTag("selectedLayer1METs")
analyzeMassForETau.RefInputTags = cms.VInputTag("NoMatching",
                                                "NoMatching",
                                                "NoMatching")
analyzeMassForETau.DoMatching = cms.bool(False)
analyzeMassForETau.EventWeight = cms.double(15.)

analyzeMassForLooseETau=copy.deepcopy(analyzeMassForETau)
analyzeMassForLooseETau.TauTag = cms.InputTag("selectorPfTausForOSSSChargeSepAntiEff")
analyzeMassForLooseETau.ElecTag = cms.InputTag("selectAntiIpAfterIsoElecForETau")
analyzeMassForLooseETau.MetTag = cms.InputTag("selectedLayer1METs")
analyzeMassForLooseETau.RefInputTags = cms.VInputTag("NoMatching",
                                                     "NoMatching",
                                                     "NoMatching")
analyzeMassForLooseETau.DoMatching = cms.bool(False)
analyzeMassForLooseETau.EventWeight = cms.double(15.)
