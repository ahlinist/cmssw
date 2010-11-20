import FWCore.ParameterSet.Config as cms

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
selectedPatTausForMuTauEta21 = selectedPatTausEta21.clone()

# require tau candidate to have transverse energy above threshold
selectedPatTausForMuTauPt20 = selectedPatTausPt20.clone()

# require tau candidate to have a leading track
# (track of Pt > 1. GeV within matching cone of size dR = 0.2 around jet-axis)
selectedPatTausForMuTauLeadTrk = selectedPatTausLeadTrk.clone()

# require leading track of tau candidate to have Pt > 5. GeV
selectedPatTausForMuTauLeadTrkPt = selectedPatTausLeadTrkPt.clone()

# require tau candidate to pass TaNC discriminator
selectedPatTausForMuTauTaNCdiscr = selectedPatTausTaNCdiscr.clone()

# require tau candidate to have no tracks of Pt > 1. GeV
# in isolation cone of size dR = 0.8, surrounding signal cone of size dR = 5./Et
selectedPatTausForMuTauTrkIso = selectedPatTausTrkIso.clone()

# require tau candidate to be isolated
# with respect to energy deposits in ECAL
selectedPatTausForMuTauEcalIso = selectedPatTausEcalIso.clone()

# require tau candidate to have either one or three tracks within signal cone
selectedPatTausForMuTauProng = selectedPatTausProng.clone()

# require tau candidate to have charge either +1 or -1
# (computed as sum of charges of tracks within signal cone)
selectedPatTausForMuTauCharge = selectedPatTausCharge.clone()

# require tau candidate to pass muon veto
selectedPatTausForMuTauMuonVeto = selectedPatTausMuonVeto.clone()

# require tau candidate to pass veto against calorimeter muon/MIP signature
selectedPatTausForMuTauCaloMuonVeto = cms.EDFilter("PATTauSelector",
    cut = cms.string('tauID("againstCaloMuon") > 0.5'),
    filter = cms.bool(False)                                 
)

# require tau candidate to pass electron veto
selectedPatTausForMuTauElectronVeto = selectedPatTausElectronVeto.clone()
