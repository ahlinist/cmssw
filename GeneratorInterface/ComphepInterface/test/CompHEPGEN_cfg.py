import FWCore.ParameterSet.Config as cms

process = cms.Process("GEN")


# import of standard configurations
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/StandardSequences/Generator_cff')
process.load('Configuration/StandardSequences/VtxSmearedEarly10TeVCollision_cff')

process.load('Configuration/EventContent/EventContent_cff')
process.load("Configuration.Generator.PythiaUESettings_cfi")
process.genParticles.abortOnUnknownPDGCode = False

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)

process.source = cms.Source("ComphepSource",
    pythiaPylistVerbosity = cms.untracked.int32(0),
    filterEfficiency = cms.untracked.double(0.4614),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    fileNames = cms.untracked.vstring('file:Mixed.pev'),
    process = cms.untracked.string('p1,p1 -> m,M'),
    CompHEPFirstEvent = cms.int32(1),
    crossSection = cms.untracked.double(0.5496),
    getInputFromMCDB = cms.untracked.bool(False),
    maxEventsToPrint = cms.untracked.int32(1),
    MCDBArticleID = cms.int32(0),
        PythiaParameters = cms.PSet(
        process.pythiaUESettingsBlock,
        #process setting
        pythia = cms.vstring(
        'MSEL=0          ! User defined processes'),        

        parameterSets = cms.vstring('pythiaUESettings','pythia')
    )
)

  
process.MuMuFilter = cms.EDFilter("MCParticlePairFilter",
    Status = cms.untracked.vint32(1, 1),
    MinPt = cms.untracked.vdouble(0.0, 0.0),
    MaxEta = cms.untracked.vdouble(2.5, 2.5),
    MinEta = cms.untracked.vdouble(-2.5, -2.5),
    ParticleCharge = cms.untracked.int32(0),
    MinInvMass = cms.untracked.double(600.0),
    ParticleID1 = cms.untracked.vint32(13),
    ParticleID2 = cms.untracked.vint32(13)
)

process.ProductionFilterSequence = cms.Sequence(process.MuMuFilter)

# Path and EndPath definitions


# Output definition
process.output = cms.OutputModule("PoolOutputModule",
    outputCommands = process.RAWSIMEventContent.outputCommands,
    fileName = cms.untracked.string('file:comphep_source.root'),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN'),
        filterName = cms.untracked.string('')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    )
)

# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen)
process.out_step = cms.EndPath(process.output)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.out_step)

