import FWCore.ParameterSet.Config as cms

muonSelection = cms.EDProducer('MuonViewRefSelector',
                               cut = cms.string('pt > 0'),
                               )
