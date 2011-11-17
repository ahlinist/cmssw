import FWCore.ParameterSet.Config as cms

# import definition of FEVTSIM (RAW + RECO + Gen/Sim) event content
from Configuration.EventContent.EventContent_cff import *

goldenZmumuEventContent = FEVTSIMEventContent.clone()

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

# keep event counters for Mauro's "self baby-sitting" technology
keepEventCounters = cms.PSet(
    outputCommands = cms.untracked.vstring(
        'keep EventAux_*_*_*',
        'keep LumiSummary_*_*_*',                       
        'keep edmMergeableCounter_*_*_*',
    )
)
goldenZmumuEventContent.outputCommands.extend(keepEventCounters.outputCommands)
