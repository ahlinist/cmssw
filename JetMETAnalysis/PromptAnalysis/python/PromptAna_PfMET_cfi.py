import FWCore.ParameterSet.Config as cms

promptanapfmet = cms.EDProducer( "PromptAna_PfMET"
                                 , InputTag  = cms.InputTag('pfMet')
                                 , Prefix    = cms.string('pfmet')
                                 , Suffix    = cms.string('')
                                 )
