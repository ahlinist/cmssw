import FWCore.ParameterSet.Config as cms

pi0Discriminator = cms.EDProducer("PhotonAsscToValMap",
    collectionSrc = cms.InputTag('photons'),
    associationSrc = cms.InputTag('piZeroDiscriminators',
                                  'PhotonPi0DiscriminatorAssociationMap'),
    label = cms.string("")
    )
