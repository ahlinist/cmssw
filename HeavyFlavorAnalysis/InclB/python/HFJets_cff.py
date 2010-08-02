import FWCore.ParameterSet.Config as cms

## from JetMETCorrections.Configuration.L2L3Corrections_Summer08Redigi_cff import *
## es_prefer_L2JetCorrectorScone5 = cms.ESPrefer("L2RelativeCorrectionService","L2JetCorrectorSC5Calo")
## es_prefer_L3JetCorrectorScone5 = cms.ESPrefer("L3AbsoluteCorrectionService","L3JetCorrectorSC5Calo")
myPartons = cms.EDFilter("PartonSelector",
    withLeptons = cms.bool(False)
)

flavourByRef = cms.EDFilter("JetPartonMatcher",
    jets = cms.InputTag("L2L3CorJetSC5Calo"),
    coneSizeToAssociate = cms.double(0.5),
    partons = cms.InputTag("myPartons")
)

jetDump = cms.EDFilter("HFDumpJets",
    doflavortagging = cms.untracked.int32(1),
    generatorCandidates = cms.untracked.string('genParticles'),
    sourceByRefer = cms.InputTag("flavourByRef"),
    verbose = cms.untracked.int32(0),
    jetsLabel = cms.untracked.string('L2L3CorJetSC5Calo')
)

rawjetDump = cms.EDFilter("HFDumpJets",
    doflavortagging = cms.untracked.int32(0),
    generatorCandidates = cms.untracked.string('genParticles'),
    sourceByRefer = cms.InputTag("flavourByRef"),
    verbose = cms.untracked.int32(0),
    jetsLabel = cms.untracked.string('sisCone5CaloJets')
)

genflavourByRef = cms.EDFilter("JetPartonMatcher",
    jets = cms.InputTag("sisCone5GenJets"),
    coneSizeToAssociate = cms.double(0.5),
    partons = cms.InputTag("myPartons")
)

genjetDump = cms.EDFilter("HFDumpGenJets",
    genjetsLabel = cms.untracked.string('sisCone5GenJets'),
    sourceByRefer = cms.InputTag("genflavourByRef"),
    verbose = cms.untracked.int32(0),
    genparticlesLabel = cms.untracked.string('genParticles')
)


#JetDump = cms.Sequence(L2L3CorJetSC5Calo*flavourByRef*jetDump)
#JetDumpAOD = cms.Sequence(L2L3CorJetSC5Calo*jetDump)
GenJetDump = cms.Sequence(genflavourByRef*genjetDump)
RawJetDumpAOD = cms.Sequence(rawjetDump)
