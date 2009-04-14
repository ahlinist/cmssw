import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.RecoTools.patPFTauSelection_cfi import *

# require tau candidate not to overlap with selected electrons
# (in order to avoid double-counting one and the same physical particle
#  as electron and as tau candidate)
selectedLayer1TausForElecTauAntiOverlapWithElectronsVeto = cms.EDFilter("PATTauAntiOverlapSelector",
    srcNotToBeFiltered = cms.VInputTag("selectedLayer1ElectronsTrkIPcumulative"),
    dRmin = cms.double(0.3),
    filter = cms.bool(False)                                           
)

# require tau candidate to be within geometric acceptance of Pixel + SiTracker detectors
selectedLayer1TausForElecTauEta21 = copy.deepcopy(selectedLayer1TausEta21)

# require tau candidate to have transverse energy above threshold
selectedLayer1TausForElecTauPt20 = copy.deepcopy(selectedLayer1TausPt20)

# require tau candidate to have a leading track
# (track of Pt > 1. GeV within matching cone of size dR = 0.2 around jet-axis)
selectedLayer1TausForElecTauLeadTrk = copy.deepcopy(selectedLayer1TausLeadTrk)

# require leading track of tau candidate to have Pt > 5. GeV
selectedLayer1TausForElecTauLeadTrkPt = copy.deepcopy(selectedLayer1TausLeadTrkPt)

# require tau candidate to have no tracks of Pt > 1. GeV
# in isolation cone of size dR = 0.8, surrounding signal cone of size dR = 5./Et
selectedLayer1TausForElecTauTrkIso = copy.deepcopy(selectedLayer1TausTrkIso)

# require tau candidate to be isolated
# with respect to energy deposits in ECAL
selectedLayer1TausForElecTauEcalIso = copy.deepcopy(selectedLayer1TausEcalIso)

# require tau candidate to have either one or three tracks within signal cone
selectedLayer1TausForElecTauProng = copy.deepcopy(selectedLayer1TausProng)

# require tau candidate to pass electron veto
selectedLayer1TausForElecTauElectronVeto = copy.deepcopy(selectedLayer1TausElectronVeto)
