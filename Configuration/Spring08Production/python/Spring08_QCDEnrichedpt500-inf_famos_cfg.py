# The following comments couldn't be translated into the new config version:

#
# No filtering on Gamma pt, just on eta, parton-gamma angle etc

# Work-around because of a bug in HLT 
# Reconstruction sequence
import FWCore.ParameterSet.Config as cms

process = cms.Process("HLT")
process.load("FastSimulation.Configuration.CommonIncludesForProd_cff")

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.load("Configuration.Spring08Production.Spring08_QCDEnrichedpt500_inf_cfi")

#END OF SOURCE#############################
# WATCH OUT !!! ##################################
# the com energy is overwritten is the Fast Sim, at least, let's overwrite it with the correct value 
#    Uncomment if not 14 TeV 
#    replace    famosSimHits.ActivateDecays.comEnergy = 10000.
# 
# PART IF NEEDED###############################
#these are needed by the filter
#    include "PhysicsTools/HepMCCandAlgos/data/genParticles.cfi"
#    include "GeneratorInterface/GenFilters/data/BCToEFilter.cfi"
#    sequence MCfilter = { genParticles, bctoefilter }
#      Don't forget to uncomment MCfilter in "simulation" and in "reconstruction" and update the PoolOutModule accordingly (?)
process.load("GeneratorInterface.GenFilters.PythiaFilterGammaJetWithBg_cfi")

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
    name = cms.untracked.string('$Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/Spring08Production/data/Spring08_QCDEnrichedpt500-inf_famos.cfg,v $'),
    annotation = cms.untracked.string('QCDEnrichedpt500-inf')
)
process.Timing = cms.Service("Timing")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)
process.o1 = cms.OutputModule("PoolOutputModule",
    process.AODSIMEventContent,
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('AODSIM')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('reconstruction')
    ),
    fileName = cms.untracked.string('AODWithHLT.root')
)

process.MCfilter = cms.Sequence(process.gj_filter)
process.simulation = cms.Sequence(process.MCfilter+process.simulationWithFamos+process.genEventScale+process.genEventWeight)
process.hltEnd = cms.Sequence(process.dummyModule)
process.reconstruction = cms.Path(process.MCfilter+process.doCalo+process.towerMakerForAll+process.reconstructionWithFamos)
process.outpath = cms.EndPath(process.o1)
process.gj_filter.MinPhotonPt = 0.
process.gj_filter.MaxPhotonPt = 10000000000.
process.MessageLogger.destinations = ['detailedInfo.txt']

