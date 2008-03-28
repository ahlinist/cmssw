# The following comments couldn't be translated into the new config version:

# Keep the hits associated to the muon tracks

# Keep the muon chamber hits
# DT

# CSC

# RPC

import FWCore.ParameterSet.Config as cms

from Configuration.EventContent.EventContent_cff import *
tevDiMuonHighPtEventContent = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_standAloneMuons_*_*', 'keep *_globalMuons_*_*', 'keep *_dt1DRecHits_*_*', 'keep *_dt2DSegments_*_*', 'keep *_dt4DSegments_*_*', 'keep *_csc2DRecHits_*_*', 'keep *_cscSegments_*_*', 'keep *_rpcRecHits_*_*')
)
tevDiMuonHighPtEventSelection = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('tevDiMuonHighPt')
    )
)
tevDiMuonHighPtEventContent.outputCommands.extend(AODSIMEventContent.outputCommands)

