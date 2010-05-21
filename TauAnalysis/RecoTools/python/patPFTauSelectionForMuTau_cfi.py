import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.RecoTools.patPFTauSelection_cfi import *

# require tau candidate not to overlap with selected muons
# (in order to avoid double-counting one and the same physical particle
#  as muon and as tau candidate)
selectedPatTausForMuTauAntiOverlapWithMuonsVeto = cms.EDFilter("PATTauAntiOverlapSelector",
    srcNotToBeFiltered = cms.VInputTag("selectedPatMuonsGlobalIndividual"),
    dRmin = cms.double(0.5),
    filter = cms.bool(False)                                           
)

# require tau candidate to be within geometric acceptance of Pixel + SiTracker detectors
selectedPatTausForMuTauEta21 = copy.deepcopy(selectedPatTausEta21)

# require tau candidate to have transverse energy above threshold
selectedPatTausForMuTauPt20 = copy.deepcopy(selectedPatTausPt20)

# require tau candidate to have a leading track
# (track of Pt > 1. GeV within matching cone of size dR = 0.2 around jet-axis)
selectedPatTausForMuTauLeadTrk = copy.deepcopy(selectedPatTausLeadTrk)

# require leading track of tau candidate to have Pt > 5. GeV
selectedPatTausForMuTauLeadTrkPt = copy.deepcopy(selectedPatTausLeadTrkPt)

# require tau candidate to pass TaNC discriminator
selectedPatTausForMuTauTaNCdiscr = copy.deepcopy(selectedPatTausTaNCdiscr)

# require tau candidate to have no tracks of Pt > 1. GeV
# in isolation cone of size dR = 0.8, surrounding signal cone of size dR = 5./Et
selectedPatTausForMuTauTrkIso = copy.deepcopy(selectedPatTausTrkIso)

# require tau candidate to be isolated
# with respect to energy deposits in ECAL
selectedPatTausForMuTauEcalIso = copy.deepcopy(selectedPatTausEcalIso)

# require tau candidate to have either one or three tracks within signal cone
selectedPatTausForMuTauProng = copy.deepcopy(selectedPatTausProng)

# require tau candidate to have charge either +1 or -1
# (computed as sum of charges of tracks within signal cone)
selectedPatTausForMuTauCharge = copy.deepcopy(selectedPatTausCharge)

# require tau candidate to pass muon veto
selectedPatTausForMuTauMuonVeto = copy.deepcopy(selectedPatTausMuonVeto)

# require tau candidate to pass electron veto
selectedPatTausForMuTauElectronVeto = copy.deepcopy(selectedPatTausElectronVeto)
