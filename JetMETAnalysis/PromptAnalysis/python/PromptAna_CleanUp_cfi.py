import FWCore.ParameterSet.Config as cms

promptanacleanup = cms.EDProducer("PromptAna_CleanUp",
                                  Prefix    = cms.string(''),
                                  Suffix    = cms.string(''),
                                  HcalNoiseRBXCollection  = cms.InputTag("hcalnoise"),
                                  HcalNoiseSummary        = cms.InputTag("hcalnoise"),                                  
                                  JetCollectionLabel = cms.InputTag("iterativeCone5CaloJets"),
                                  JetIDParams = cms.PSet(
        useRecHits = cms.bool(True),
        hbheRecHitsColl = cms.InputTag("hbhereco"),
        hoRecHitsColl   = cms.InputTag("horeco"),
        hfRecHitsColl   = cms.InputTag("hfreco"),
        ebRecHitsColl   = cms.InputTag("ecalRecHit", "EcalRecHitsEB"),
        eeRecHitsColl   = cms.InputTag("ecalRecHit", "EcalRecHitsEE")
        ),
                                  )
