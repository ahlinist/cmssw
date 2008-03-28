# The following comments couldn't be translated into the new config version:

# This is a "custom" configuration of the MessageLogger which 
# I find convenient (JY)...
#

import FWCore.ParameterSet.Config as cms

process = cms.Process("RESTORE")
#include "SimG4Core/Configuration/data/SimG4Core.cff"
process.load("Configuration.StandardSequences.Simulation_cff")

process.load("Configuration.StandardSequences.FakeConditions_cff")

process.load("Configuration.StandardSequences.Geometry_cff")

process.load("Configuration.StandardSequences.MagneticField_cff")

process.load("Configuration.StandardSequences.MixingNoPileUp_cff")

#
# well, I view it as a contradiction:
# in order to run *SIMULATION* chain I need to use
# cff's from the RECO side !...
# that's because FakeConditions does replacement on
# some of the modules from the RecoLocal department...
#
process.load("RecoLocalTracker.Configuration.RecoLocalTracker_cff")

process.load("RecoLocalMuon.Configuration.RecoLocalMuon_cff")

process.load("RecoLocalCalo.Configuration.RecoLocalCalo_cff")

# standard "prescription of what to keep in edm::Event upon output
#
process.load("Configuration.EventContent.EventContent_cff")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
process.MessageLogger = cms.Service("MessageLogger",
    cout = cms.untracked.PSet(
        default = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        ),
        #untracked PSet FwkJob  = { untracked int32 limit = -1 }
        SimG4CoreApplication = cms.untracked.PSet(
            limit = cms.untracked.int32(-1)
        )
    ),
    categories = cms.untracked.vstring('FwkJob', 'SimG4CoreApplication'),
    destinations = cms.untracked.vstring('cout')
)

process.Timing = cms.Service("Timing")

process.source = cms.Source("PoolSource",
    skipEvents = cms.untracked.uint32(3), ## skip the first 3 events, 

    fileNames = cms.untracked.vstring('file:H190ZZ4mu_detsim_digi.root')
)

process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    restoreStateLabel = cms.untracked.string('rndmStore'), ## this label MUST correspond

    # to what it was in the original run
    # for the RandomEngineStateProducer
    moduleSeeds = cms.PSet(
        g4SimHits = cms.untracked.uint32(1),
        ecalUnsuppressedDigis = cms.untracked.uint32(1),
        muonCSCDigis = cms.untracked.uint32(1),
        mix = cms.untracked.uint32(1),
        siPixelDigis = cms.untracked.uint32(1),
        # Remark: it does NOT matter what seeds you put in
        #         since the seeds will be restored from RandonStateEngineProduct
        #         but something has to be in the config due to Fwk regulations
        #
        VtxSmeared = cms.untracked.uint32(1),
        hcalUnsuppressedDigis = cms.untracked.uint32(1),
        muonDTDigis = cms.untracked.uint32(1),
        siStripDigis = cms.untracked.uint32(1),
        muonRPCDigis = cms.untracked.uint32(1)
    )
)

process.RndmStoreFEVT = cms.PSet(
    outputCommands = cms.untracked.vstring('drop *_*_*_Sim', 'keep RandomEngineStates_*_*_* ')
)
process.USER = cms.OutputModule("PoolOutputModule",
    process.FEVTSIMEventContent,
    fileName = cms.untracked.string('restore_rndm_H190ZZ4mu_detsim_digi.root')
)

process.p1 = cms.Path(process.g4SimHits)
process.p2 = cms.Path(process.pdigi)
process.outpath = cms.EndPath(process.USER)
process.schedule = cms.Schedule(process.p1,process.p2,process.outpath)

process.FEVTSIMEventContent.outputCommands.extend(process.RndmStoreFEVT.outputCommands)

