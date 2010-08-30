import FWCore.ParameterSet.Config as cms

def keepMeGrandMother(iDaughter, iGrandDaughter):
  cuts = """
      keep++ numberOfDaughters > %d
      daughter(%d).numberOfDaughters > %d
      daughter(%d).daughter(%d).status = 3
    """ % (iDaughter, iDaughter, iGrandDaughter, iDaughter, iGrandDaughter)
  while "  " in cuts: cuts = cuts.replace("  ", " ")
  cuts = cuts.lstrip()
  cuts = cuts.rstrip()
  cuts = cuts.split("\n")
  while "" in cuts: cuts.remove("")
  return " & ".join(cuts)

grandMotherSelection = [keepMeGrandMother(i, j)
  for i in range(5) for j in range(5)
  ]

prunedGenParticles = cms.EDProducer("GenParticlePruner",
  src = cms.InputTag("genParticles"),
  select = cms.vstring(
    "++keep++ status = 3", # hard scattering full genealogy
    "keep++ numberOfDaughters > 0 & daughter(0).status = 3",
    "keep++ numberOfDaughters > 1 & daughter(1).status = 3",
    "keep++ numberOfDaughters > 2 & daughter(2).status = 3",
    "keep++ numberOfDaughters > 3 & daughter(3).status = 3",
    "keep++ numberOfDaughters > 4 & daughter(4).status = 3",
  )
)

# These are to preserve ISR
prunedGenParticles.select += tuple(grandMotherSelection)

# Remove uninteresting particles
prunedGenParticles.select += tuple(["drop pt < 1.5 & abs(eta) > 5"])

if __name__ == "__main__": import user
