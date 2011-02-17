import FWCore.ParameterSet.Config as cms

# import definition of FEVT (RAW + RECO) event content
from Configuration.EventContent.EventContent_cff import *

goldenZmumuEventContent = FEVTEventContent.clone()

# keep extras
keepExtras = cms.PSet(
    outputCommands = cms.untracked.vstring(
        'keep *_genParticles_*_*',
        'keep *_particleFlow_*_*',
        'keep *_generator_*_*',
    )
)
goldenZmumuEventContent.outputCommands.extend(keepExtras.outputCommands)

# keep goodMuons
keepGoodMuons = cms.PSet(
    outputCommands = cms.untracked.vstring(
        'keep *_goodMuons_*_*',
        'keep *_goodIsoMuons_*_*'
    )
)
goldenZmumuEventContent.outputCommands.extend(keepGoodMuons.outputCommands)

# keep goldenZmumuCandidates
keepGoldenZmumuCandidates = cms.PSet(
    outputCommands = cms.untracked.vstring(
        'keep *_goldenZmumuCandidatesGe0IsoMuons_*_*',
        'keep *_goldenZmumuCandidatesGe1IsoMuons_*_*',
        'keep *_goldenZmumuCandidatesGe2IsoMuons_*_*'
    )
)
goldenZmumuEventContent.outputCommands.extend(keepGoldenZmumuCandidates.outputCommands)

# keep "tag & "probe" flags
keepMuonIsolationTagAndProbeFlags = cms.PSet(
    outputCommands = cms.untracked.vstring(
        'keep *_goodMuonIsolationTagAndProbeProducer_*_*'
    )
)
goldenZmumuEventContent.outputCommands.extend(keepMuonIsolationTagAndProbeFlags.outputCommands)
