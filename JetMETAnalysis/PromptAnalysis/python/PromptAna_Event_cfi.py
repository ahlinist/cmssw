import FWCore.ParameterSet.Config as cms

promptanaevent = cms.EDProducer("PromptAna_Event",
                                TRGTag = cms.InputTag("TriggerResults","","HLT"))
