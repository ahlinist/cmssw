# The following comments couldn't be translated into the new config version:

# random number generator

import FWCore.ParameterSet.Config as cms

process = cms.Process("MATCHINGTTBAR")
#keep the logging output to a nice level
process.load("FWCore.MessageLogger.MessageLogger_cfi")

# MadGraph Interface
process.load("Configuration.Spring08Production.MadGraphSourceDefault_cfi")

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.2 $'),
    name = cms.untracked.string('$Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/Spring08Production/data/Spring08_MadGraph_Qcut30.cfg,v $'),
    annotation = cms.untracked.string('Example CFG for MadGraph')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    sourceSeed = cms.untracked.uint32(824177121)
)

process.GEN = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN')
    ),
    #using FEVTSIMEventContent
    fileName = cms.untracked.string('file:MGtest.root')
)

process.p = cms.EndPath(process.GEN)
process.MadGraphSource.fileNames = cms.VInputTag(cms.InputTag("file","events.lhe"))
process.MadGraphSource.MEMAIN_qcut = 30.
process.MadGraphSource.PythiaParameters = cms.PSet(
    parameterSets = cms.vstring('pythiaMatching'),
    pythiaMatching = cms.vstring('MSTP(143)=1    ! MUST BE 1 FOR THE MATCHING ROUTINE TO RUN!!!!')
)

