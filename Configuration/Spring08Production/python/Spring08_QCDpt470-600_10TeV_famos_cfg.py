# The following comments couldn't be translated into the new config version:

#   MCfilter &

#        MCfilter &

# Reconstruction sequence
import FWCore.ParameterSet.Config as cms

process = cms.Process("HLT")
process.load("FastSimulation.Configuration.CommonIncludesForProd_cff")

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.load("Configuration.Spring08Production.Spring08_QCDpt470_600_10TeV_cfi")

# 
# PART IF NEEDED###############################
#these are needed by the filter
#    include "PhysicsTools/HepMCCandAlgos/data/genParticles.cfi"
#    include "GeneratorInterface/GenFilters/data/BCToEFilter.cfi"
#    sequence MCfilter = { genParticles, bctoefilter }
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
    version = cms.untracked.string('$Revision: 1.1 $'),
    name = cms.untracked.string('$Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/Spring08Production/data/Spring08_QCDpt470-600_10TeV_famos.cfg,v $'),
    annotation = cms.untracked.string('QCDpt470-600')
)
process.Timing = cms.Service("Timing")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(50)
)
process.o1 = cms.OutputModule("PoolOutputModule",
    process.AODSIMEventContent,
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('AODSIM')
    ),
    fileName = cms.untracked.string('AODWithHLT.root')
)

process.simulation = cms.Sequence(process.simulationWithFamos+process.genEventScale+process.genEventWeight)
process.hltEnd = cms.Sequence(process.dummyModule)
process.reconstruction = cms.Path(process.doCalo+process.towerMakerForAll+process.reconstructionWithFamos)
process.outpath = cms.EndPath(process.o1)
process.famosSimHits.ActivateDecays.comEnergy = 10000.
process.MessageLogger.destinations = ['detailedInfo.txt']

