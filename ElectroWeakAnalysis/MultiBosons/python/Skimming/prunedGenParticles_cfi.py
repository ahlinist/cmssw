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
    ## Keep all possible photon fakes, 8 GeV cut to help with rate
    "++keep status = 1 &     pdgId  = 21 & pt > 8 & abs(eta) < 3.1",
    "++keep status = 3 &     pdgId  = 21 & pt > 8 & abs(eta) < 3.1",
    "++keep status = 2 &     pdgId  = 111 & pt > 8 & abs(eta) < 3.1",
    "++keep status = 2 &     pdgId  = 113 & pt > 8 & abs(eta) < 3.1",
    "++keep status = 1 & abs(pdgId) = 211 & pt > 8 & abs(eta) < 3.1",
    "++keep status = 2 &     pdgId  = 221 & pt > 8 & abs(eta) < 3.1",
    "++keep status = 1 & abs(pdgId) = 1 & pt > 8 & abs(eta) < 3.1",
    "++keep status = 1 & abs(pdgId) = 2 & pt > 8 & abs(eta) < 3.1",
    "++keep status = 1 & abs(pdgId) = 3 & pt > 8 & abs(eta) < 3.1",
    "++keep status = 1 & abs(pdgId) = 4 & pt > 8 & abs(eta) < 3.1",
    "++keep status = 1 & abs(pdgId) = 5 & pt > 8 & abs(eta) < 3.1",    
    ## Drop all the soft and out of acceptance particles
    "drop (status = 1 | status = 2) & (pt < 1.5 | abs(eta) > 5)"
  )
)

if __name__ == "__main__": import user
