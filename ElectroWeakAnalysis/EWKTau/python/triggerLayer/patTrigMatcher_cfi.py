import FWCore.ParameterSet.Config as cms

from ElectroWeakAnalysis.EWKTau.triggerLayer.patTrigProducer_cfi import *
# Examples for configurations of the trigger match for various physics objects
# (cuts are NOT tuned, using old values from TQAF MC match, january 2008)

# matches to Egamma triggers
# matches to CandHLT1ElectronStartup
# matches to HLT1Electron
electronTrigMatchHLT1Electron = cms.EDFilter("PATTrigMatcher",
     src     = cms.InputTag("allLayer0Electrons"),
    matched = cms.InputTag("patHLT1Electron"),
    maxDPtRel = cms.double(1.0),
    maxDeltaR = cms.double(0.2),
    resolveAmbiguities    = cms.bool(True),
    resolveByMatchQuality = cms.bool(False),
)

 # matches to HLT1ElectronRelaxed
# including example of "wrong" match (jets which fired electron trigger),
electronTrigMatchHLT1ElectronRelaxed = cms.EDFilter("PATTrigMatcher",
     src     = cms.InputTag("allLayer0Electrons"),
     matched = cms.InputTag("patHLT1ElectronRelaxed"),
     maxDPtRel = cms.double(1.0),
     maxDeltaR = cms.double(0.2),
     resolveAmbiguities    = cms.bool(True),
     resolveByMatchQuality = cms.bool(False),
)


muonTrigMatchHLT1MuonIso = cms.EDFilter("PATTrigMatcher",
    src     = cms.InputTag("allLayer0Muons"),
    matched = cms.InputTag("patHLT1MuonIso"),
    maxDPtRel = cms.double(1.0),
    maxDeltaR = cms.double(0.2),
    resolveAmbiguities    = cms.bool(True),
    resolveByMatchQuality = cms.bool(False),
)

# matches to HLT1MuonNonIso
muonTrigMatchHLT1MuonNonIso = cms.EDFilter("PATTrigMatcher",
    src     = cms.InputTag("allLayer0Muons"),
    matched = cms.InputTag("patHLT1MuonNonIso"),
    maxDPtRel = cms.double(1.0),
    maxDeltaR = cms.double(0.2),
    resolveAmbiguities    = cms.bool(True),
    resolveByMatchQuality = cms.bool(False),
)


muonTrigMatchHLTXMuonTau = cms.EDFilter("PATTrigMatcher",
    src     = cms.InputTag("allLayer0Muons"),
    matched = cms.InputTag("patHLTXMuonTau"),
    maxDPtRel = cms.double(1.0),
    maxDeltaR = cms.double(0.2),
    resolveAmbiguities    = cms.bool(True),
    resolveByMatchQuality = cms.bool(False),
)



electronTrigMatchHLTXElectronTau = cms.EDFilter("PATTrigMatcher",
    src     = cms.InputTag("allLayer0Electrons"),
    matched = cms.InputTag("patHLTXElectronTau"),
    maxDPtRel = cms.double(1.0),
    maxDeltaR = cms.double(0.2),
    resolveAmbiguities    = cms.bool(True),
    resolveByMatchQuality = cms.bool(False),
)



muonTrigMatchHLTXElectronMuon = cms.EDFilter("PATTrigMatcher",
    src     = cms.InputTag("allLayer0Muons"),
    matched = cms.InputTag("patHLTXElectronMuon"),
    maxDPtRel = cms.double(1.0),
    maxDeltaR = cms.double(0.2),
    resolveAmbiguities    = cms.bool(True),
    resolveByMatchQuality = cms.bool(False),
)

electronTrigMatchHLTXElectronMuon = cms.EDFilter("PATTrigMatcher",
    src     = cms.InputTag("allLayer0Electrons"),
    matched = cms.InputTag("patHLTXElectronMuon"),
    maxDPtRel = cms.double(1.0),
    maxDeltaR = cms.double(0.2),
    resolveAmbiguities    = cms.bool(True),
    resolveByMatchQuality = cms.bool(False),
)



patTrigMatchHLT1MuonIso = cms.Sequence(patHLT1MuonIso * muonTrigMatchHLT1MuonIso)
patTrigMatchHLT1MuonNonIso = cms.Sequence(patHLT1MuonNonIso * muonTrigMatchHLT1MuonNonIso)
patTrigMatchHLT1Electron = cms.Sequence(patHLT1Electron * electronTrigMatchHLT1Electron)
patTrigMatchHLT1ElectronRelaxed = cms.Sequence(patHLT1ElectronRelaxed * electronTrigMatchHLT1ElectronRelaxed)
