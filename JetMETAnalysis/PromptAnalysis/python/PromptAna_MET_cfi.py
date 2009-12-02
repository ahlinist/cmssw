import FWCore.ParameterSet.Config as cms

promptanamet = cms.EDProducer( "PromptAna_MET"
                            , InputTag  = cms.InputTag('met')
                            , Prefix    = cms.string('calomet')
                            , Suffix    = cms.string('')
                            )

promptananohf = cms.EDProducer("PromptAna_MET",
                            InputTag = cms.InputTag('metNoHF'),
                            Prefix = cms.string('metnohf'),
                            Suffix = cms.string('Calo')
                            )

promptanatcmet = cms.EDProducer( "PromptAna_MET"
                                 , InputTag  = cms.InputTag('tcMet')
                                 , Prefix    = cms.string('tcmet')
                                 , Suffix    = cms.string('')
                                 )
