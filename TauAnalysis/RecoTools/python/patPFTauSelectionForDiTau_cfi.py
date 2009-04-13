import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.RecoTools.patPFTauSelection_cfi import *

# require tau candidate to be within geometric acceptance of Pixel + SiTracker detectors
selectedLayer1TausForDiTauEta21 = copy.deepcopy(selectedLayer1TausEta21)
selectedLayer1TausForDiTauEta21.pluginName = cms.string("selectedLayer1TausForDiTauEta21")

# require tau candidate to have transverse energy above threshold
selectedLayer1TausForDiTauPt20 = copy.deepcopy(selectedLayer1TausPt20)
selectedLayer1TausForDiTauPt20.pluginName = cms.string("selectedLayer1TausForDiTauPt20")

# require tau candidate to have a leading track
# (track of Pt > 1. GeV within matching cone of size dR = 0.2 around jet-axis)
selectedLayer1TausForDiTauLeadTrk = copy.deepcopy(selectedLayer1TausLeadTrk)
selectedLayer1TausForDiTauLeadTrk.pluginName = cms.string("selectedLayer1TausForDiTauLeadTrk")

# require leading track of tau candidate to have Pt > 5. GeV
selectedLayer1TausForDiTauLeadTrkPt = copy.deepcopy(selectedLayer1TausLeadTrkPt)
selectedLayer1TausForDiTauLeadTrkPt.pluginName = cms.string("selectedLayer1TausForDiTauLeadTrkPt")

# require tau candidate to have no tracks of Pt > 1. GeV
# in isolation cone of size dR = 0.8, surrounding signal cone of size dR = 5./Et
selectedLayer1TausForDiTauTrkIso = copy.deepcopy(selectedLayer1TausTrkIso)
selectedLayer1TausForDiTauTrkIso.pluginName = cms.string("selectedLayer1TausForDiTauTrkIso")

# require tau candidate to be isolated
# with respect to energy deposits in ECAL
selectedLayer1TausForDiTauEcalIso = copy.deepcopy(selectedLayer1TausEcalIso)
selectedLayer1TausForDiTauEcalIso.pluginName = cms.string("selectedLayer1TausForDiTauEcalIso")

# require tau candidate to have either one or three tracks within signal cone
selectedLayer1TausForDiTauProng = copy.deepcopy(selectedLayer1TausProng)
selectedLayer1TausForDiTauProng.pluginName = cms.string("selectedLayer1TausForDiTauProng")
