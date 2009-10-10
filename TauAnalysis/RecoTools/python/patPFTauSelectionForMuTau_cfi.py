import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.RecoTools.patPFTauSelection_cfi import *

# require tau candidate not to overlap with selected muons
# (in order to avoid double-counting one and the same physical particle
#  as muon and as tau candidate)
selectedLayer1TausForMuTauAntiOverlapWithMuonsVeto = cms.EDFilter("PATTauAntiOverlapSelector",
    srcNotToBeFiltered = cms.VInputTag("selectedLayer1MuonsGlobalIndividual"),
    dRmin = cms.double(0.3),
    filter = cms.bool(False)                                           
)

# require tau candidate to be within geometric acceptance of Pixel + SiTracker detectors
selectedLayer1TausForMuTauEta21 = copy.deepcopy(selectedLayer1TausEta21)

# require tau candidate to have transverse energy above threshold
selectedLayer1TausForMuTauPt20 = copy.deepcopy(selectedLayer1TausPt20)

# require tau candidate to have a leading track
# (track of Pt > 1. GeV within matching cone of size dR = 0.2 around jet-axis)
selectedLayer1TausForMuTauLeadTrk = copy.deepcopy(selectedLayer1TausLeadTrk)

# require leading track of tau candidate to have Pt > 5. GeV
selectedLayer1TausForMuTauLeadTrkPt = copy.deepcopy(selectedLayer1TausLeadTrkPt)

# require tau candidate to have no tracks of Pt > 1. GeV
# in isolation cone of size dR = 0.8, surrounding signal cone of size dR = 5./Et
selectedLayer1TausForMuTauTrkIso = copy.deepcopy(selectedLayer1TausTrkIso)

# require tau candidate to be isolated
# with respect to energy deposits in ECAL
selectedLayer1TausForMuTauEcalIso = copy.deepcopy(selectedLayer1TausEcalIso)

# require tau candidate to have either one or three tracks within signal cone
selectedLayer1TausForMuTauProng = copy.deepcopy(selectedLayer1TausProng)

# require tau candidate to have charge either +1 or -1
# (computed as sum of charges of tracks within signal cone)
selectedLayer1TausForMuTauCharge = copy.deepcopy(selectedLayer1TausCharge)

# require tau candidate to pass muon veto
selectedLayer1TausForMuTauMuonVeto = copy.deepcopy(selectedLayer1TausMuonVeto)
