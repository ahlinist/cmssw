
import FWCore.ParameterSet.Config as cms

promptanamet = cms.EDProducer( "PromptAna_MET"
#                            , InputTag  = cms.InputTag('met::USER')
                            , InputTag  = cms.InputTag('met')
                            , Prefix    = cms.string('calomet')
                            , Suffix    = cms.string('')
                            )

promptananohf = cms.EDProducer("PromptAna_MET",
#                            InputTag = cms.InputTag('metNoHF::USER'),
                            InputTag = cms.InputTag('metNoHF'),
                            Prefix = cms.string('metnohf'),
                            Suffix = cms.string('Calo')
                            )

#promptanametdefault = cms.EDProducer("PromptAna_MET",
#                            InputTag  = cms.InputTag('met::RECO'),
#                            Prefix = cms.string('calometDflt'),
#                            Suffix = cms.string('')
#                            )

# promptanamet = cms.EDProducer( "PromptAna_MET"
#                             , InputTag  = cms.InputTag('met')
#                             , Prefix    = cms.string('calomet')
#                             , Suffix    = cms.string('')
#                             )

# promptananohf = cms.EDProducer("PromptAna_MET",
#                             InputTag = cms.InputTag('metNoHF'),
#                             Prefix = cms.string('metnohf'),
#                             Suffix = cms.string('Calo')
#                             )

#promptanatcmet = cms.EDProducer( "PromptAna_MET"
#                                 , InputTag  = cms.InputTag('tcMet')
#                                 , Prefix    = cms.string('tcmet')
#                                 , Suffix    = cms.string('')
#                                 )
