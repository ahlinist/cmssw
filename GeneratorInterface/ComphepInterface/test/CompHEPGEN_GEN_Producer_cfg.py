import FWCore.ParameterSet.Config as cms

process = cms.Process("PROD")
process.load("FWCore.Framework.test.cmsExceptionsFatal_cff")
process.load("Configuration.Generator.PythiaUESettings_cfi")

process.MessageLogger = cms.Service("MessageLogger",
    cout = cms.untracked.PSet(
        default = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        )
    ),
    destinations = cms.untracked.vstring('cout')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)


process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    comphep = cms.PSet(
        initialSeed = cms.untracked.uint32(98765)
    )
)

process.comphep = cms.EDProducer("ComphepProducer",
    pythiaPylistVerbosity = cms.untracked.int32(0),
    filterEfficiency = cms.untracked.double(0.4614),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    ComphepInputFile = cms.untracked.vstring('file:Mixed.pev'),
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



process.source = cms.Source("EmptySource")
process.GEN = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('file:comphep_producer.root')
)

process.p1 = cms.Path(process.comphep)
process.e = cms.EndPath(process.GEN)
