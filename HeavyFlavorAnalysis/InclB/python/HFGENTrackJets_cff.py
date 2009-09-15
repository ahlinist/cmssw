import FWCore.ParameterSet.Config as cms






myPartons = cms.EDFilter("PartonSelector",
    withLeptons = cms.bool(False)
)

genflavourByRef = cms.EDFilter("JetPartonMatcher",
    jets = cms.InputTag("sisCone5GenJets"),
    coneSizeToAssociate = cms.double(0.5),
    partons = cms.InputTag("myPartons")
)

genjetDump = cms.EDFilter("HFDumpGenJets",
    genjetsLabel = cms.untracked.string('sisCone5GenJets'),
    sourceByRefer = cms.InputTag("genflavourByRef"),
    verbose = cms.untracked.int32(1),
    genparticlesLabel = cms.untracked.string('genParticles')
)


GenJetDump = cms.Sequence(myPartons*genflavourByRef*genjetDump)
