# The following comments couldn't be translated into the new config version:

# Work-around because of a bug in HLT 
# Reconstruction sequence
import FWCore.ParameterSet.Config as cms

process = cms.Process("HLT")
process.load("FastSimulation.Configuration.CommonIncludesForProd_cff")

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.load("Configuration.Spring08Production.Spring08_HcalIsoTrack_QCDpt30_inf_10TeV_cfi")

# 
# PART IF NEEDED###############################
#these are needed by the filter
process.load("PhysicsTools.HepMCCandAlgos.genParticles_cfi")

process.load("GeneratorInterface.GenFilters.PythiaFilterIsolatedTrack_cfi")

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
    name = cms.untracked.string('$Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/Spring08Production/data/Spring08_HcalIsoTrack_QCDpt30-inf_10TeV_famos.cfg,v $'),
    annotation = cms.untracked.string('HcalIsoTrack_QCDpt30-inf')
)
process.Timing = cms.Service("Timing")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)
process.o1 = cms.OutputModule("PoolOutputModule",
    process.RECOEventContent,
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('RECO')
    ),
    fileName = cms.untracked.string('RECOWithHLT.root')
)

process.MCfilter = cms.Sequence(process.genParticles*process.isotrack_filter)
process.simulation = cms.Sequence(process.MCfilter+process.simulationWithFamos+process.genEventScale+process.genEventWeight)
process.hltEnd = cms.Sequence(process.dummyModule)
process.reconstruction = cms.Path(process.MCfilter+process.doCalo+process.towerMakerForAll+process.reconstructionWithFamos)
process.outpath = cms.EndPath(process.o1)
process.famosSimHits.ActivateDecays.comEnergy = 10000.
process.RECOEventContent.outputCommands.extend(process.SimMuonFEVTDIGI.outputCommands)
process.RECOEventContent.outputCommands.extend(process.RecoLocalMuonFEVT.outputCommands)
process.MessageLogger.destinations = ['detailedInfo.txt']

