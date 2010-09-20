import FWCore.ParameterSet.Config as cms

prunedGenParticles = cms.EDProducer("GenParticlePruner",
  src = cms.InputTag("genParticles"),
  select = cms.vstring(
    ## Keep the hard scattering process with all ancesstors, children and
    ##+ grand children.
    "++keep+ status = 2 & numberOfMothers > 0 & mother(0).status = 3",
    ## Keep all the interesting leptons, neutrinos and photons together
    ##+ with all there ancestors
    "++keep status = 1 & abs(pdgId) = 11 & pt > 3 & abs(eta) < 3.1",
    "++keep status = 1 & abs(pdgId) = 12 & pt > 3 & abs(eta) < 3.1",
    "++keep status = 1 & abs(pdgId) = 13 & pt > 3 & abs(eta) < 3.1",
    "++keep status = 1 & abs(pdgId) = 14 & pt > 3 & abs(eta) < 3.1",
    "++keep              abs(pdgId) = 15 & pt > 3 & abs(eta) < 3.1",
    "++keep status = 1 & abs(pdgId) = 16 & pt > 3 & abs(eta) < 3.1",
    "++keep status = 1 &     pdgId  = 22 & pt > 3 & abs(eta) < 3.1",
    ## Drop all the soft and out of acceptance particles
    "drop pt < 1.5 | abs(eta) > 5"
  )
)

if __name__ == "__main__": import user
