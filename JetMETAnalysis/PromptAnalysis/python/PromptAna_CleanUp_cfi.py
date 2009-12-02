import FWCore.ParameterSet.Config as cms

promptanacleanup = cms.EDProducer("PromptAna_CleanUp",
                                  Prefix    = cms.string(''),
                                  Suffix    = cms.string(''),
                                  HcalNoiseSummary        = cms.InputTag("hcalnoise"),
                                  JetCollectionLabel = cms.InputTag("iterativeCone5CaloJets")
                                  )
