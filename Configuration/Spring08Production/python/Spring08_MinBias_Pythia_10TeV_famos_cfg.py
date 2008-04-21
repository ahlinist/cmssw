# The following comments couldn't be translated into the new config version:

#End common includes for the Spring 08 production#####

#   MCfilter &

#        MCfilter &

# Reconstruction sequence
import FWCore.ParameterSet.Config as cms

process = cms.Process("HLT")
#Begin common includes for the Spring 08 production#####
# Other Spring08 production cfgs use 
# FastSimulation/Configuration/data/CommonIncludesForProd.cff here.
# Only change is in PrescaleFactors.
# Initialize the random generators
process.load("FastSimulation.Configuration.RandomServiceInitialization_cff")

# Famos sequences (With HLT)
process.load("FastSimulation.Configuration.CommonInputsFake_cff")

process.load("FastSimulation.Configuration.FamosSequences_cff")

# L1 Emulator and HLT Setup
process.load("FastSimulation.HighLevelTrigger.common.HLTSetup_cff")

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

#SOURCE#########################
process.load("Configuration.Spring08Production.Spring08_MinBias_Pythia_10TeV_cfi")

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
    version = cms.untracked.string('$Revision: 1.2 $'),
    name = cms.untracked.string('$Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/Spring08Production/data/Spring08_MinBias_Pythia_10TeV_famos.cfg,v $'),
    annotation = cms.untracked.string('MinBias_Pythia')
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
process.l1GtFactors.PrescaleFactors = [1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 
    1, 1, 1]
process.famosSimHits.ActivateDecays.comEnergy = 10000.
process.MessageLogger.destinations = ['detailedInfo.txt']

