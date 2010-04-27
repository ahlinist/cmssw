import FWCore.ParameterSet.Config as cms

muonRefSelection = cms.EDProducer('MuonViewRefSelector',
                               src = cms.InputTag('muons'),
                               cut = cms.string('pt > 0')
                               )

muonSelection = cms.EDProducer('MuonViewSelector',
                               src = cms.InputTag('muons'),
                               cut = cms.string('pt > 0')
                               )

