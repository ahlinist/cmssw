import FWCore.ParameterSet.Config as cms

VProducerChargedLeptons = cms.EDProducer('LazyCandViewCombiner',
                            decay = cms.string('muons@+ muons@-'),
                            cut = cms.string('mass > 0')
                            )

VPtrSelection= cms.EDProducer('LazyCandViewPtrSelector',
                              src = cms.InputTag('ZMuMuCandidatesGG'),
                              cut = cms.string('mass > 0')
                              )

VRefSelection= cms.EDProducer('LazyCandViewRefSelector',
                              src = cms.InputTag('ZMuMuCandidatesGG'),
                              cut = cms.string('mass > 0')
                              )

VSelection = cms.EDProducer('ZCandViewSelector',
                            src = cms.InputTag('ZMuMuCandidatesGG'),
                            cut = cms.string('mass > 0')
                            )


