import FWCore.ParameterSet.Config as cms

promptanaic5calojet = cms.EDProducer("PromptAna_Jet",
                                     InputTag = cms.InputTag('iterativeCone5CaloJets'),
                                     TracksInputTag = cms.InputTag('generalTracks'),
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
                                     TracksInputTag = cms.InputTag('generalTracks'),
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
                                     TracksInputTag = cms.InputTag('generalTracks'),
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

promptanaak5calojet = cms.EDProducer("PromptAna_Jet",
                            InputTag = cms.InputTag('ak5CaloJets'),
                            TracksInputTag = cms.InputTag('generalTracks'),
                            Prefix = cms.string('ak5Jet'),
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

