import FWCore.ParameterSet.Config as cms

# import definition of FEVTSIM (RAW + RECO + Gen/Sim) event content
from Configuration.EventContent.EventContent_cff import *

tightWenuEventContent = FEVTSIMEventContent.clone()

# keep extras
keepExtras = cms.PSet(
    outputCommands = cms.untracked.vstring(
        'keep *_genParticles_*_*',
        'keep *_particleFlow_*_*',
        'keep *_generator_*_*',
    )
)
tightWenuEventContent.outputCommands.extend(keepExtras.outputCommands)

# keep selectedElectrons
keepSelectedElectrons = cms.PSet(
    outputCommands = cms.untracked.vstring(
        'keep *_selectedElectronsWP80_*_*',
        'keep *_selectedElectronsWP80conversionVeto_*_*',
        'keep *_selectedIsoElectronsWP80_*_*',
        'keep *_selectedElectronsWP95_*_*'
    )
)
tightWenuEventContent.outputCommands.extend(keepSelectedElectrons.outputCommands)

# keep WenuCandidates
keepWenuCandidates = cms.PSet(
    outputCommands = cms.untracked.vstring(
        'keep *_WenuCandidates_*_*'
    )
)
tightWenuEventContent.outputCommands.extend(keepWenuCandidates.outputCommands)

# keep event counters for Mauro's "self baby-sitting" technology
keepEventCounters = cms.PSet(
    outputCommands = cms.untracked.vstring(
        'keep EventAux_*_*_*',
        'keep LumiSummary_*_*_*',                       
        'keep edmMergeableCounter_*_*_*',
    )
)
tightWenuEventContent.outputCommands.extend(keepEventCounters.outputCommands)
