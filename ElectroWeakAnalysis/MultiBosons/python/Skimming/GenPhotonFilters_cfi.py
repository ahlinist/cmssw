import FWCore.ParameterSet.Config as cms

leadingPhoton = cms.EDFilter("LargestPtPatPhotonSelector",
  src = cms.InputTag("cleanPatPhotons"),
  maxNumber = cms.uint32(1)
)

## Event passes if src collection contains a candidate matching Pythia PS ISR
pythiaPartonShowerIsrFilter = cms.EDFilter("PATPhotonSelector",
  src = cms.InputTag("leadingPhoton"),
  cut = cms.string("""
    genParticlesSize > 0 &
    (genParticle.mother.pdgId = 21 | abs(genParticle.mother.pdgId) < 7)
    """),
  filter = cms.bool(True)
)

## Event passes if src collection contains a candidate matching Pythia PS FSR
pythiaPartonShowerFsrFilter = cms.EDFilter("PATPhotonSelector",
  src = cms.InputTag("leadingPhoton"),
  cut = cms.string("""
    genParticlesSize > 0 &
    (genParticle.mother.mother.pdgId = 23 | abs(genParticle.mother.mother.pdgId) = 24)
    """),
  filter = cms.bool(True)
)

## Events passes if src contains a candidate that is neither Pythia ISR nor FSR
pythiaPartonShowerPhotonVeto = cms.EDFilter("PATPhotonSelector",
  src = cms.InputTag("cleanPatPhotons"),
  cut = cms.string("""
    genParticlesSize > 0 &
    genParticle.mother.pdgId != 21 &
    abs(genParticle.mother.pdgId) >= 7 &
    genParticle.mother.mother.pdgId != 23 &
    abs(genParticle.mother.mother.pdgId) != 24
    """),
  filter = cms.bool(True)
)

if __name__ == "__main__": import user
