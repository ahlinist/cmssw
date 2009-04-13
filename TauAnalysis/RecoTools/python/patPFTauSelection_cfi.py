import FWCore.ParameterSet.Config as cms

# require tau candidate to be within geometric acceptance of Pixel + SiTracker detectors
selectedLayer1TausEta21 = cms.PSet(
    pluginName = cms.string("selectedLayer1TausEta21"),
    pluginType = cms.string("PATTauSelector"),
    cut = cms.string("abs(eta) < 2.1"),
    filter = cms.bool(False)                                 
)

# require tau candidate to have transverse energy above threshold
selectedLayer1TausPt20 = cms.PSet(
    pluginName = cms.string("selectedLayer1TausPt20"),
    pluginType = cms.string("PATTauSelector"),
    cut = cms.string("pt > 20."),
    filter = cms.bool(False)                                 
)

# require tau candidate to have a leading track
# (track of Pt > 1. GeV within matching cone of size dR = 0.2 around jet-axis)
selectedLayer1TausLeadTrk = cms.PSet(
    pluginName = cms.string("selectedLayer1TausLeadTrk"),
    pluginType = cms.string("PATTauSelector"),
    cut = cms.string('tauID("leadingTrackFinding") > 0.5'),
    filter = cms.bool(False)                                 
)

# require leading track of tau candidate to have Pt > 5. GeV
selectedLayer1TausLeadTrkPt = cms.PSet(
    pluginName = cms.string("selectedLayer1TausLeadTrkPt"),
    pluginType = cms.string("PATTauSelector"),
    cut = cms.string('tauID("leadingTrackPtCut") > 0.5'),
    filter = cms.bool(False)                                 
)

# require tau candidate to have no tracks of Pt > 1. GeV
# in isolation cone of size dR = 0.8, surrounding signal cone of size dR = 5./Et
selectedLayer1TausTrkIso = cms.PSet(
    pluginName = cms.string("selectedLayer1TausTrkIso"),
    pluginType = cms.string("PATTauSelector"),
    cut = cms.string('tauID("trackIsolation") > 0.5'),
    filter = cms.bool(False)                                 
)

# require tau candidate to be isolated
# with respect to energy deposits in ECAL
selectedLayer1TausEcalIso = cms.PSet(
    pluginName = cms.string("selectedLayer1TausEcalIso"),
    pluginType = cms.string("PATTauSelector"),
    cut = cms.string('tauID("ecalIsolation") > 0.5'),
    filter = cms.bool(False)                                 
)

# require tau candidate to have either one or three tracks within signal cone
selectedLayer1TausProng = cms.PSet(
    pluginName = cms.string("selectedLayer1TausProng"),
    pluginType = cms.string("PATTauSelector"),
    cut = cms.string("signalTracks.size() = 1 | signalTracks.size() = 3"),                                   
    filter = cms.bool(False)                                 
)

# require tau candidate to pass electron veto
selectedLayer1TausElectronVeto = cms.PSet(
    pluginName = cms.string("selectedLayer1TausElectronVeto"),
    pluginType = cms.string("PATTauSelector"),
    cut = cms.string('tauID("againstElectron") > 0.5'),
    filter = cms.bool(False)                                 
)

# require tau candidate to pass muon veto
selectedLayer1TausMuonVeto = cms.PSet(
    pluginName = cms.string("selectedLayer1TausMuonVeto"),
    pluginType = cms.string("PATTauSelector"),
    cut = cms.string('tauID("againstMuon") > 0.5'),
    filter = cms.bool(False)                                 
)

cleanLayer1TausSel = cms.EDProducer("PATTauSelProducer",

  leptonSource = cms.InputTag("cleanLayer1Taus"),

  selFlags = cms.PSet(
    tauAnalysisSelTauEta21 = cms.PSet(
      src = cms.InputTag('selectedLayer1TausEta21Individual')
    ),
    tauAnalysisSelTauPt20 = cms.PSet(
      src = cms.InputTag('selectedLayer1TausPt20Individual')
    ),
    tauAnalysisSelTauLeadTrk = cms.PSet(
      src = cms.InputTag('selectedLayer1TausLeadTrkIndividual')
    ),
    tauAnalysisSelTauLeadTrkPt = cms.PSet(
      src = cms.InputTag('selectedLayer1TausLeadTrkPtIndividual')
    ),
    tauAnalysisSelTauTrkIso = cms.PSet(
      src = cms.InputTag('selectedLayer1TausTrkIsoIndividual')
    ),
    tauAnalysisSelTauEcalIso = cms.PSet(
      src = cms.InputTag('selectedLayer1TausEcalIsoIndividual')
    ),
    tauAnalysisSelTauElecVeto = cms.PSet(
      src = cms.InputTag('selectedLayer1TausElectronVetoIndividual')
    ),
    tauAnalysisSelTauMuonVeto = cms.PSet(
      src = cms.InputTag('selectedLayer1TausMuonVetoIndividual')
    ),
    tauAnalysisSelTauProng = cms.PSet(
      src = cms.InputTag('selectedLayer1TausProngIndividual')
    )
  )
)

produceLayer1SelTaus = cms.Sequence( cleanLayer1TausSel )
