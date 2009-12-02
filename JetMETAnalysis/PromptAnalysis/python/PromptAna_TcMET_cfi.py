import FWCore.ParameterSet.Config as cms

promptanatcmet = cms.EDProducer( "PromptAna_TcMET"
                                 , InputTag  = cms.InputTag('tcMet')
                                 , Prefix    = cms.string('tcmet')
                                 , Suffix    = cms.string('')
                                 )
