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
    input = cms.untracked.int32(10)
)

process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)

process.source = cms.Source("ComphepSource",
    pythiaPylistVerbosity = cms.untracked.int32(1),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    fileNames = cms.untracked.vstring('rfio:bbWW_500K.PEV'),
    process = cms.untracked.string('pp --> W+,W-,b,b'),
    CompHEPFirstEvent = cms.int32(1),       
    crossSection = cms.untracked.double(651.78),#pb units
    getInputFromMCDB = cms.untracked.bool(True),
    ComphepInputFile = cms.untracked.string('rfio://castor/cern.ch/sft/mcdb/storage/76/bbWW_500K.PEV'),
    maxEventsToPrint = cms.untracked.int32(1),
    MCDBArticleID = cms.int32(76),
   ## TAUOLA Parameters
   UseExternalGenerators = cms.untracked.bool(True),
   ExternalGenerators = cms.PSet(
        Tauola = cms.untracked.PSet(
             UseTauolaPolarization = cms.bool(True),
            InputCards = cms.vstring('TAUOLA = 0 0 0   ! TAUOLA ')
        ),
        parameterSets = cms.vstring('Tauola')
    ),
  ## PYTHIA Parameters
    PythiaParameters = cms.PSet(
        process.pythiaUESettingsBlock,
        #process setting
        pythia = cms.vstring(
        'MSEL=0          ! User defined processes'),        

        parameterSets = cms.vstring('pythiaUESettings','pythia')
    )
)


# Output definition
process.output = cms.OutputModule("PoolOutputModule",
    outputCommands = process.RAWSIMEventContent.outputCommands,
    fileName = cms.untracked.string('file:comphep_tauola_mcdb.root'),
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

