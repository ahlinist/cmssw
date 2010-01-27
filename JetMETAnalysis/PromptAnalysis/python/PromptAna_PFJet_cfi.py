import FWCore.ParameterSet.Config as cms

promptanaak5pfjet = cms.EDProducer("PromptAna_PFJet",
                            InputTag = cms.InputTag('ak5PFJets'),
                            JetCorrectionService = cms.string('L2L3JetCorrectorAK5PF'),
                            Prefix = cms.string('ak5PFJet'),
                            Suffix = cms.string('')
                            )

