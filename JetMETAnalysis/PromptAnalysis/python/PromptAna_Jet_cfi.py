import FWCore.ParameterSet.Config as cms

promptanaic5calojet = cms.EDProducer("PromptAna_Jet",
                            InputTag = cms.InputTag('iterativeCone5CaloJets'),
                            Prefix = cms.string('ic5Jet'),
                            Suffix = cms.string('')
                            )

promptanakt4calojet = cms.EDProducer("PromptAna_Jet",
                            InputTag = cms.InputTag('kt4CaloJets'),
                            Prefix = cms.string('kt4Jet'),
                            Suffix = cms.string('')
                            )
