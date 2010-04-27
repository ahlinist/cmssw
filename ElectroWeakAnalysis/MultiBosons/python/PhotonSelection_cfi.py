import FWCore.ParameterSet.Config as cms

photonPtrSelection = cms.EDProducer('PatPhotonViewPtrSelector',
                               src = cms.InputTag('selectedPatPhotons'),
                               cut = cms.string('et > 0')
                               )

photonRefSelection = cms.EDProducer('PatPhotonViewRefSelector',
                               src = cms.InputTag('selectedPatPhotons'),
                               cut = cms.string('et > 0')
                               )

photonSelection = cms.EDProducer('PatPhotonViewSelector',
                               src = cms.InputTag('selectedPatPhotons'),
                               cut = cms.string('et > 0')
                               )
