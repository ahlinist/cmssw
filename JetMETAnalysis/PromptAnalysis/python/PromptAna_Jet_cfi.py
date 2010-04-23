import FWCore.ParameterSet.Config as cms

promptanaic5calojet = cms.EDProducer("PromptAna_Jet",
                                     InputTag = cms.InputTag('iterativeCone5CaloJets'),
                                     TracksInputTag = cms.InputTag('generalTracks'),
                                   #  JetCorrectionService = cms.string('L2L3JetCorrectorIC5Calo'),
                                     JetCorrectionService = cms.string('ic5CaloL2L3'),
                                     JetTracksAtCalo=cms.InputTag("iterativeCone5JetTracksAssociatorAtCaloFace"),
                                     JetTracksAtVertex=cms.InputTag("iterativeCone5JetTracksAssociatorAtVertex"),
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
                                     JetCorrectionService = cms.string('kt4CaloL2L3'),
                                     JetTracksAtCalo=cms.InputTag("kt4JetTracksAssociatorAtCaloFace"),
                                     JetTracksAtVertex=cms.InputTag("kt4JetTracksAssociatorAtVertex"),
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

## promptanasc5calojet = cms.EDProducer("PromptAna_Jet",
##                             InputTag = cms.InputTag('sisCone5CaloJets'),
##                             TracksInputTag = cms.InputTag('generalTracks'),
##                             JetCorrectionService = cms.string(''),
##                             JetTracksAtCalo=cms.InputTag("sisCone5JetTracksAssociatorAtCaloFace"),
##                             JetTracksAtVertex=cms.InputTag("sisCone5JetTracksAssociatorAtVertex"),     
##                             Prefix = cms.string('sc5Jet'),
##                             Suffix = cms.string(''),
##                             # Jet ID configuration
##                             jetID = cms.PSet(
##                                useRecHits = cms.bool(True),
##                                hbheRecHitsColl = cms.InputTag("hbhereco"),
##                                hoRecHitsColl   = cms.InputTag("horeco"),
##                                hfRecHitsColl   = cms.InputTag("hfreco"),
##                                ebRecHitsColl   = cms.InputTag("ecalRecHit", "EcalRecHitsEB"),
##                                eeRecHitsColl   = cms.InputTag("ecalRecHit", "EcalRecHitsEE")
##                             )
##                             )

promptanaak5calojet = cms.EDProducer("PromptAna_Jet",
                            InputTag = cms.InputTag('ak5CaloJets'),
                            TracksInputTag = cms.InputTag('generalTracks'),
                       #     JetCorrectionService = cms.string('L2L3JetCorrectorAK5Calo'),
                              JetCorrectionService = cms.string('ak5CaloL2L3'),        
                            JetTracksAtCalo=cms.InputTag("ak5JetTracksAssociatorAtCaloFace"),
                            JetTracksAtVertex=cms.InputTag("ak5JetTracksAssociatorAtVertex"),  
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

