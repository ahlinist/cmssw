# The following comments couldn't be translated into the new config version:

# Work-around because of a bug in HLT 
# Reconstruction sequence
import FWCore.ParameterSet.Config as cms

process = cms.Process("HLT")
process.load("FastSimulation.Configuration.CommonIncludesForProd_cff")

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

#SOURCE#########################
process.load("Configuration.Spring08Production.Spring08_EMEnrichingFilter_QCD_Pt_80_170_14TeV_cfi")

#END OF SOURCE#############################
# WATCH OUT !!! ##################################
# the com energy is overwritten is the Fast Sim, at least, let's overwrite it with the correct value 
#    Uncomment if not 14 TeV 
#    replace    famosSimHits.ActivateDecays.comEnergy = 10000.
# 
#FILTER PART IF NEEDED###############################
#these are needed by the filter
process.load("PhysicsTools.HepMCCandAlgos.genParticles_cfi")

process.load("GeneratorInterface.GenFilters.BCToEFilter_cfi")

process.load("GeneratorInterface.GenFilters.EMEnrichingFilter_cfi")

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
    annotation = cms.untracked.string('em-enriched QCD Pthat 80-170')
)
process.Timing = cms.Service("Timing")

process.maxEvents = cms.untracked.PSet(
    output = cms.untracked.int32(10)
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

process.MCfilter = cms.Sequence(process.genParticles*~process.bctoefilter*process.emenrichingfilter)
process.simulation = cms.Sequence(process.MCfilter+process.simulationWithFamos+process.genEventScale+process.genEventWeight)
process.hltEnd = cms.Sequence(process.dummyModule)
process.reconstruction = cms.Path(process.MCfilter+process.doCalo+process.towerMakerForAll+process.reconstructionWithFamos)
process.outpath = cms.EndPath(process.o1)
process.MessageLogger.destinations = ['detailedInfo.txt']

