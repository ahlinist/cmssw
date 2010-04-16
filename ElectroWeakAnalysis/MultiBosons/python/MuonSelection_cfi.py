import FWCore.ParameterSet.Config as cms

muonPtrSelection = cms.EDProducer('MuonViewPtrSelector',
                               src = cms.InputTag('muons'),
                               cut = cms.string('pt > 0')
                               )

muonSelection = cms.EDProducer('MuonViewSelector',
                               src = cms.InputTag('muons'),
                               cut = cms.string('pt > 0')
                               )

