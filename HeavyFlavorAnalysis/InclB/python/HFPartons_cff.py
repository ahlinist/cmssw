import FWCore.ParameterSet.Config as cms

myPartons = cms.EDFilter("PartonSelector",
    
    src = cms.InputTag("genParticles"),
    withLeptons = cms.bool(False)
    
)
