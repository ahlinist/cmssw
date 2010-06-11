import FWCore.ParameterSet.Config as cms


promptanaJPTak5 = cms.EDProducer("PromptAna_JPT",
  InputTag = cms.InputTag('JetPlusTrackZSPCorJetAntiKt5'),
  JetCorrectionService = cms.string('ak5JPTL2L3'),
  Prefix = cms.string('JPTak5Jet'),
  Suffix = cms.string(''),
  AllVariables = cms.bool(True),
  # Jet ID configuration
  jetID = cms.PSet(
    useRecHits = cms.bool(True),
    hbheRecHitsColl = cms.InputTag("hbhereco"),
    hoRecHitsColl   = cms.InputTag("horeco"),
    hfRecHitsColl   = cms.InputTag("hfreco"),
    ebRecHitsColl   = cms.InputTag("ecalRecHit", "EcalRecHitsEB"),
    eeRecHitsColl   = cms.InputTag("ecalRecHit", "EcalRecHitsEE")
  )
)
