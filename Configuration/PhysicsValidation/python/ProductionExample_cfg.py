import FWCore.ParameterSet.Config as cms

process = cms.Process("Rec1")
process.load("Configuration.StandardSequences.Reconstruction_cff")

process.load("Configuration.StandardSequences.Simulation_cff")

process.load("Configuration.StandardSequences.MixingNoPileUp_cff")

process.load("Configuration.StandardSequences.VtxSmearedGauss_cff")

# Event output
process.load("Configuration.EventContent.EventContent_cff")

process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    moduleSeeds = cms.PSet(
        g4SimHits = cms.untracked.uint32(9876),
        VtxSmeared = cms.untracked.uint32(123456789)
    ),
    sourceSeed = cms.untracked.uint32(98765)
)

process.source = cms.Source("FlatRandomPtGunSource",
    maxEvents = cms.untracked.int32(10),
    PGunParameters = cms.untracked.PSet(
        MaxPt = cms.untracked.double(100.01),
        MinPt = cms.untracked.double(99.99),
        PartID = cms.untracked.vint32(13),
        MaxEta = cms.untracked.double(2.5),
        MaxPhi = cms.untracked.double(3.14159265359),
        MinEta = cms.untracked.double(-2.5),
        MinPhi = cms.untracked.double(-3.14159265359) ## in radians

    ),
    Verbosity = cms.untracked.int32(0), ## set to 1 (or greater)  for printouts

    psethack = cms.string('single mu pt 100 neg'),
    firstRun = cms.untracked.uint32(1)
)

process.FEVT = cms.OutputModule("PoolOutputModule",
    process.FEVTSIMEventContent,
    datasets = cms.untracked.PSet(
        dataset1 = cms.untracked.PSet(
            dataTier = cms.untracked.string('FEVT')
        )
    ),
    fileName = cms.untracked.string('reco.root')
)

process.p1 = cms.Path(process.psim)
process.p2 = cms.Path(process.pdigi)
process.p3 = cms.Path(process.reconstruction)
process.outpath = cms.EndPath(process.FEVT)
process.schedule = cms.Schedule(process.p1,process.p2,process.p3,process.outpath)


