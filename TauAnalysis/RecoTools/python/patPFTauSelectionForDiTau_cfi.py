import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.RecoTools.patPFTauSelection_cfi import *

# require tau candidate to be within geometric acceptance of Pixel + SiTracker detectors
selectedPatTausForDiTauEta21 = copy.deepcopy(selectedPatTausEta21)

# require tau candidate to have transverse energy above threshold
selectedPatTausForDiTauPt20 = copy.deepcopy(selectedPatTausPt20)

# require tau candidate to have a leading track
# (track of Pt > 1. GeV within matching cone of size dR = 0.2 around jet-axis)
selectedPatTausForDiTauLeadTrk = copy.deepcopy(selectedPatTausLeadTrk)

# require leading track of tau candidate to have Pt > 5. GeV
selectedPatTausForDiTauLeadTrkPt = copy.deepcopy(selectedPatTausLeadTrkPt)

# require tau candidate to pass TaNC discriminator
selectedPatTausForDiTauTaNCdiscr = copy.deepcopy(selectedPatTausTaNCdiscr)

# require tau candidate to have no tracks of Pt > 1. GeV
# in isolation cone of size dR = 0.8, surrounding signal cone of size dR = 5./Et
selectedPatTausForDiTauTrkIso = copy.deepcopy(selectedPatTausTrkIso)

# require tau candidate to be isolated
# with respect to energy deposits in ECAL
selectedPatTausForDiTauEcalIso = copy.deepcopy(selectedPatTausEcalIso)

# require tau candidate to have either one or three tracks within signal cone
selectedPatTausForDiTauProng = copy.deepcopy(selectedPatTausProng)

# require tau candidate to have charge either +1 or -1
# (computed as sum of charges of tracks within signal cone)
selectedPatTausForDiTauCharge = copy.deepcopy(selectedPatTausCharge)
