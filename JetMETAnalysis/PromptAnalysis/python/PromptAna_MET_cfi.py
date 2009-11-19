import FWCore.ParameterSet.Config as cms

promptanamet = cms.EDProducer( "PromptAna_MET"
                            , InputTag  = cms.InputTag('met')
                            , Prefix    = cms.string('CaloMET')
                            , Suffix    = cms.string('')
                            )

promptananohf = cms.EDProducer("PromptAna_MET",
                            InputTag = cms.InputTag('metNoHF'),
                            Prefix = cms.string('metnohf'),
                            Suffix = cms.string('Calo')
                            )
