import FWCore.ParameterSet.Config as cms

process = cms.Process("L1EMU")

# The number of events to be processed.
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

process.load("FastSimulation.Configuration.RandomServiceInitialization_cff")


# Generate H -> ZZ -> l+l- l'+l'- (l,l'=e or mu), with mH=180GeV/c2
#process.load("FastSimulation/Configuration/HZZllll_cfi")
# Generate ttbar events
process.load("FastSimulation/Configuration/ttbar_cfi")
# Generate multijet events with different ptHAT bins
#  process.load("FastSimulation/Configuration/QCDpt80-120_cfi")
#  process.load("FastSimulation/Configuration/QCDpt600-800_cfi")
# Generate Minimum Bias Events
#  process.load("FastSimulation/Configuration/MinBiasEvents_cfi")
# Generate muons with a flat pT particle gun, and with pT=10.
# process.load("FastSimulation/Configuration/FlatPtMuonGun_cfi")
# replace FlatRandomPtGunSource.PGunParameters.PartID={130}
# Generate di-electrons with pT=35 GeV
#process.load("FastSimulation/Configuration/DiElectrons_cfi")




# Famos sequences (Frontier conditions)
process.load("FastSimulation/Configuration/CommonInputs_cff")
process.load("FastSimulation/Configuration/FamosSequences_cff")
process.GlobalTag.globaltag = "IDEAL_V11::All"

# Parametrized magnetic field (new mapping, 4.0 and 3.8T)
#process.load("Configuration.StandardSequences.MagneticField_40T_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.VolumeBasedMagneticFieldESProducer.useParametrizedTrackerField = True

# If you want to turn on/off pile-up
process.famosPileUp.PileUpSimulator.averageNumber = 100.0
# You may not want to simulate everything for your study
process.famosSimHits.SimulateCalorimetry = True
process.famosSimHits.SimulateTracking = False
# process.famosSimHits.SimulateMuons = False


#Do digitization so that you can create trigger primitives
process.load("FastSimulation.CaloRecHitsProducer.CaloRecHits_cff")
process.caloRecHits.RecHitsFactory.doDigis = True


#Load Scales
process.load("L1TriggerConfig.L1ScalesProducers.L1CaloInputScalesConfig_cff")
process.load("L1TriggerConfig.L1ScalesProducers.L1CaloScalesConfig_cff")

process.load("SLHCUpgradeSimulations.L1Trigger.L1SLHCEmulator_cff")

process.p1 = cms.Path(process.simulationWithFamos+process.caloJetMetGen+
                      process.simEcalTriggerPrimitiveDigis+
                      process.simHcalTriggerPrimitiveDigis+
                      process.L1SLHCEmulator)

# To write out events (not need: FastSimulation _is_ fast!)
process.o1 = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string("L1outputFastSim.root"),
    outputCommands = cms.untracked.vstring('drop *',
        'keep *_*_*_L1EMU')
    )

process.outpath = cms.EndPath(process.o1)

# Keep the logging output to a nice level #
# process.Timing =  cms.Service("Timing")
# process.load("FWCore/MessageService/MessageLogger_cfi")
# process.MessageLogger.destinations = cms.untracked.vstring("pyDetailedInfo.txt")

# Make the job crash in case of missing product
process.options = cms.untracked.PSet( Rethrow = cms.untracked.vstring('ProductNotFound') )
