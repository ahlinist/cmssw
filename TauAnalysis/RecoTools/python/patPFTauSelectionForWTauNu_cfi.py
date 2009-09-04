import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.RecoTools.patPFTauSelection_cfi import *

selectedLayer1TausForWTauNuEta21 = copy.deepcopy(selectedLayer1TausEta21)
selectedLayer1TausForWTauNuPt20 = copy.deepcopy(selectedLayer1TausPt20)
selectedLayer1TausForWTauNuLeadTrk = copy.deepcopy(selectedLayer1TausLeadTrk)
selectedLayer1TausForWTauNuLeadTrkPt = copy.deepcopy(selectedLayer1TausLeadTrkPt)
selectedLayer1TausForWTauNuTrkIso = copy.deepcopy(selectedLayer1TausTrkIso)
selectedLayer1TausForWTauNuMuonVeto = copy.deepcopy(selectedLayer1TausMuonVeto)
selectedLayer1TausForWTauNuElectronVeto = copy.deepcopy(selectedLayer1TausElectronVeto)
selectedLayer1TausForWTauNuProng = copy.deepcopy(selectedLayer1TausProng)
selectedLayer1TausForWTauNuCharge = copy.deepcopy(selectedLayer1TausCharge)
