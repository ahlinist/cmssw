import FWCore.ParameterSet.Config as cms

process = cms.Process("PROD")

from  FastSimulation.Calorimetry.Calorimetry_cff import *
FamosCalorimetryBlock.Calorimetry.CalorimeterProperties.RespCorrP = (
        1.0, 1.5, 1.5,
       10.0, 1.3, 1.3,
      100.0, 1.2, 1.2,
     1000.0, 1.1, 1.1)


# The number of events to be processed.
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10))


#  PYTHIA6 Generator 
process.load("Configuration.Generator.QCD_Pt_120_170_cfi")

# Include the RandomNumberGeneratorService definition
process.load("FastSimulation/Configuration/RandomServiceInitialization_cff")

# Famos sequences (Frontier conditions)
process.load("FastSimulation/Configuration/CommonInputs_cff")
process.GlobalTag.globaltag = "IDEAL_30X::All"
process.load("FastSimulation/Configuration/FamosSequences_cff")


# Parametrized magnetic field (new mapping, 4.0 and 3.8T)
#process.load("Configuration.StandardSequences.MagneticField_40T_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
#process.VolumeBasedMagneticFieldESProducer.useParametrizedTrackerField = True
#process.load("Configuration.StandardSequences.MagneticField_0T_cff")


# If you want to turn on/off pile-up
process.famosPileUp.PileUpSimulator.averageNumber = 0.0    
# You may not want to simulate everything for your study
process.famosSimHits.SimulateCalorimetry = True
process.famosSimHits.SimulateTracking = False
# process.famosSimHits.SimulateMuons = False


# Produce
process.p1 = cms.Path(process.generator * process.famosWithJets)

# To write out events
process.load("FastSimulation.Configuration.EventContent_cff")
process.o1 = cms.OutputModule("PoolOutputModule",
    process.AODSIMEventContent,
    fileName = cms.untracked.string('fastsim.root')
)

process.outpath = cms.EndPath(process.o1)

# Make the job crash in case of missing product
process.options = cms.untracked.PSet( Rethrow = cms.untracked.vstring('ProductNotFound') )
