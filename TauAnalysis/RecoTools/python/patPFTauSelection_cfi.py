import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------  
# produce collections of pat::Taus passing selection criteria
#
# NOTE: the final cut values are (re)defined in
#
#         TauAnalysis/RecoTools/python/patLeptonSelection_cff.py
#
#--------------------------------------------------------------------------------

# require tau candidate to be within geometric acceptance of Pixel + SiTracker detectors
selectedPatTausEta23 = cms.EDFilter("PATTauSelector",
    cut = cms.string("abs(eta) < 2.3"),
    filter = cms.bool(False)                                 
)

# require tau candidate to have transverse energy above threshold
selectedPatTausPt20 = cms.EDFilter("PATTauSelector",
    cut = cms.string("pt > 20."),
    filter = cms.bool(False)                                 
)

# require tau candidate to have a leading track
# (track of Pt > 1. GeV within matching cone of size dR = 0.2 around jet-axis)
selectedPatTausLeadTrk = cms.EDFilter("PATTauSelector",
    cut = cms.string('tauID("leadingTrackFinding") > 0.5'),
    filter = cms.bool(False)                                 
)

# require leading track of tau candidate to have Pt > 5. GeV
selectedPatTausLeadTrkPt = cms.EDFilter("PATTauSelector",
    cut = cms.string('tauID("leadingTrackPtCut") > 0.5'),
    filter = cms.bool(False)                                 
)

# require tau candidate to pass TaNC discriminator
selectedPatTausTaNCdiscr = cms.EDFilter("PATTauSelector",
    cut = cms.string('tauID("byTaNCmedium") > 0.5'),
    filter = cms.bool(False)                                 
)

# require tau candidate to have either one or three tracks within signal cone
selectedPatTausProng = cms.EDFilter("PATTauSelector",
    cut = cms.string('signalPFChargedHadrCands.size() = 1 | signalPFChargedHadrCands.size() = 3'),                                   
    filter = cms.bool(False)                                 
)

# require tau candidate to have charge either +1 or -1
# (computed as sum of charges of tracks within signal cone)
selectedPatTausCharge = cms.EDFilter("PATTauSelector",
    cut = cms.string('abs(charge) > 0.5 & abs(charge) < 1.5'),
    filter = cms.bool(False)                                 
)

# require tau candidate to pass electron veto
selectedPatTausElectronVeto = cms.EDFilter("PATTauSelector",
    cut = cms.string('tauID("againstElectron") > 0.5'),
    filter = cms.bool(False)                                 
)

# require tau candidate not to be in ECAL barrel/endcap crack
selectedPatTausEcalCrackVeto = cms.EDFilter("PATTauSelector",
    cut = cms.string("abs(eta) < 1.460 | abs(eta) > 1.558"),
    filter = cms.bool(False)                                 
)

# require tau candidate to pass muon veto
selectedPatTausMuonVeto = cms.EDFilter("PATTauSelector",
    cut = cms.string('tauID("againstMuon") > 0.5'),
    filter = cms.bool(False)                                 
)

