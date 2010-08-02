import FWCore.ParameterSet.Config as cms


myPartons = cms.EDProducer("PartonSelector",
    withLeptons = cms.bool(False)
)


genflavourByRef = cms.EDProducer("JetPartonMatcher",
    jets = cms.InputTag("ak5GenJets"),
    coneSizeToAssociate = cms.double(0.5),
    partons = cms.InputTag("myPartons")
)

genjetDump = cms.EDAnalyzer("HFDumpGenJets",
    genjetsLabel = cms.untracked.string('ak5GenJets'),
    sourceByRefer = cms.InputTag("genflavourByRef"),
    verbose = cms.untracked.int32(0),
    genparticlesLabel = cms.untracked.string('genParticles')
)


GenJetDump = cms.Sequence(myPartons*genflavourByRef*genjetDump)

