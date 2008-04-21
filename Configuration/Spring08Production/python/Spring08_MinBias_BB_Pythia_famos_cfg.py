# The following comments couldn't be translated into the new config version:

# Work-around because of a bug in HLT 
# Reconstruction sequence
import FWCore.ParameterSet.Config as cms

process = cms.Process("HLT")
process.load("FastSimulation.Configuration.CommonIncludesForProd_cff")

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

#SOURCE#########################
process.load("Configuration.Spring08Production.Spring08_MinBias_Pythia_cfi")

#      Don't forget to uncomment MCfilter in "simulation" and in "reconstruction" and update the PoolOutModule accordingly (?)
#
# Simulation
process.load("PhysicsTools.HepMCCandAlgos.genEventWeight_cfi")

process.load("PhysicsTools.HepMCCandAlgos.genEventScale_cfi")

# HLT paths
process.load("HLTrigger.Configuration.main.HLTpaths_cff")

# To write out events 
process.load("FastSimulation.Configuration.EventContent_cff")

# Keep the logging output to a nice level #
process.load("FWCore.MessageService.MessageLogger_cfi")

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string(''),
    name = cms.untracked.string(''),
    annotation = cms.untracked.string('b-enriched MinBias Pythia for Spring08')
)
process.Timing = cms.Service("Timing")

process.maxEvents = cms.untracked.PSet(
    output = cms.untracked.int32(10)
)
process.bfilter = cms.EDFilter("PythiaFilter",
    ParticleID = cms.untracked.int32(5)
)

process.o1 = cms.OutputModule("PoolOutputModule",
    process.AODSIMEventContent,
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('AODSIM')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('reconstruction')
    ),
    fileName = cms.untracked.string('scratch/AODWithHLT.root')
)

process.MCfilter = cms.Sequence(process.bfilter)
process.simulation = cms.Sequence(process.MCfilter+process.simulationWithFamos+process.genEventScale+process.genEventWeight)
process.hltEnd = cms.Sequence(process.dummyModule)
process.reconstruction = cms.Path(process.MCfilter+process.doCalo+process.towerMakerForAll+process.reconstructionWithFamos)
process.outpath = cms.EndPath(process.o1)
process.MessageLogger.destinations = ['detailedInfo.txt']

