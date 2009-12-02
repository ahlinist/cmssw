import FWCore.ParameterSet.Config as cms

promptanaic5calojet = cms.EDProducer("PromptAna_Jet",
                                     InputTag = cms.InputTag('iterativeCone5CaloJets'),
                                     Prefix = cms.string('ic5Jet'),
                                     Suffix = cms.string(''),
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

promptanakt4calojet = cms.EDProducer("PromptAna_Jet",
                                     InputTag = cms.InputTag('kt4CaloJets'),
                                     Prefix = cms.string('kt4Jet'),
                                     Suffix = cms.string(''),
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

promptanasc5calojet = cms.EDProducer("PromptAna_Jet",
                            InputTag = cms.InputTag('sisCone5CaloJets'),
                            Prefix = cms.string('sc5Jet'),
                            Suffix = cms.string(''),
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

promptanaakt5calojet = cms.EDProducer("PromptAna_Jet",
                            InputTag = cms.InputTag('antikt5CaloJets'),
                            Prefix = cms.string('akt5Jet'),
                            Suffix = cms.string(''),
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

