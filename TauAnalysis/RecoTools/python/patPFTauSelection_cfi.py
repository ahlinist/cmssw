import FWCore.ParameterSet.Config as cms
import copy

# require tau candidate to be within geometric acceptance of Pixel + SiTracker detectors
selectedLayer1TausEta21Individual = cms.EDFilter("PATTauSelector",
    src = cms.InputTag("cleanLayer1Taus"),
    cut = cms.string("abs(eta) < 2.1"),
    filter = cms.bool(False)                                 
)

# require tau candidate to have transverse energy above threshold
selectedLayer1TausPt20Individual = cms.EDFilter("PATTauSelector",
    src = selectedLayer1TausEta21Individual.src,
    cut = cms.string("pt > 20."),
    filter = cms.bool(False)                                 
)

# require tau candidate to have a leading track
# (track of Pt > 1. GeV within matching cone of size dR = 0.2 around jet-axis)
selectedLayer1TausLeadTrkIndividual = cms.EDFilter("PATTauSelector",
    src = selectedLayer1TausEta21Individual.src,                                                   
    cut = cms.string('tauID("leadingTrackFinding") > 0.5'),
    filter = cms.bool(False)                                 
)

# require leading track of tau candidate to have Pt > 5. GeV
selectedLayer1TausLeadTrkPtIndividual = cms.EDFilter("PATTauSelector",
    src = selectedLayer1TausEta21Individual.src,
    cut = cms.string('tauID("leadingTrackPtCut") > 0.5'),
    filter = cms.bool(False)                                 
)

# require tau candidate to have no tracks of Pt > 1. GeV
# in isolation cone of size dR = 0.8, surrounding signal cone of size dR = 5./Et
selectedLayer1TausTrkIsoIndividual = cms.EDFilter("PATTauSelector",
    src = selectedLayer1TausEta21Individual.src,
    cut = cms.string('tauID("trackIsolation") > 0.5'),
    filter = cms.bool(False)                                 
)

# require tau candidate to be isolated
# with respect to energy deposits in ECAL
selectedLayer1TausEcalIsoIndividual = cms.EDFilter("PATTauSelector",
    src = selectedLayer1TausEta21Individual.src,
    cut = cms.string('tauID("ecalIsolation") > 0.5'),
    filter = cms.bool(False)                                 
)

# require tau candidate to have either one or three tracks within signal cone
selectedLayer1TausProngIndividual = cms.EDFilter("PATTauSelector",
    src = selectedLayer1TausEta21Individual.src,
    cut = cms.string("signalTracks.size() = 1 | signalTracks.size() = 3"),                                   
    filter = cms.bool(False)                                 
)

# require tau candidate to pass electron veto
selectedLayer1TausElectronVetoIndividual = cms.EDFilter("PATTauSelector",
    src = selectedLayer1TausEta21Individual.src,
    cut = cms.string('tauID("againstElectron") > 0.5'),
    filter = cms.bool(False)                                 
)

# require tau candidate to pass muon veto
selectedLayer1TausMuonVetoIndividual = cms.EDFilter("PATTauSelector",
    src = selectedLayer1TausEta21Individual.src,
    cut = cms.string('tauID("againstMuon") > 0.5'),
    filter = cms.bool(False)                                 
)

selectLayer1Taus = cms.Sequence( selectedLayer1TausEta21Individual
                                *selectedLayer1TausPt20Individual 
                                *selectedLayer1TausLeadTrkIndividual 
                                *selectedLayer1TausLeadTrkPtIndividual 
                                *selectedLayer1TausTrkIsoIndividual
                                *selectedLayer1TausEcalIsoIndividual
                                *selectedLayer1TausProngIndividual
                                *selectedLayer1TausElectronVetoIndividual
                                *selectedLayer1TausMuonVetoIndividual )

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

produceLayer1SelTaus = cms.Sequence( selectLayer1Taus * cleanLayer1TausSel )
