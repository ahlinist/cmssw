import FWCore.ParameterSet.Config as cms

VGammaProducer = cms.EDProducer('LazyCandViewShallowCloneCombiner',
                                decay = cms.string('ZEECandidates photons'),
                                cut = cms.string('mass > 0')
                                )

VGammaRefSelection = cms.EDProducer('LazyCandViewRefSelector',
                                    src = cms.InputTag('ZGammaMuMuCandidatesGG'),
                                    cut = cms.string('mass > 0')
                                    )

VGammaPtrSelection = cms.EDProducer('LazyCandViewPtrSelector',
                                    src = cms.InputTag('ZGammaMuMuCandidatesGG'),
                                    cut = cms.string('mass > 0')
                                    )

VGammaSelection = cms.EDProducer('LazyCandViewSelector',
                                 src = cms.InputTag('ZGammaMuMuCandidatesGG'),
                                 cut = cms.string('mass > 0')
                                 )
