import FWCore.ParameterSet.Config as cms

process = cms.Process("HLT")

# Number of events to be generated
process.maxEvents = cms.untracked.PSet(
    #input = cms.untracked.int32(1000)
    output = cms.untracked.int32(1000)
)

# Include the RandomNumberGeneratorService definition
# ATTENTION AUX SEEDS!!!
process.load("FastSimulation.Configuration.RandomServiceInitialization_cff")
# this is the main seed from the principal vertex in the fast sim.
process.RandomNumberGeneratorService.theSource.initialSeed = 131
# in-time pile-up events are generated from another seed, that must
# also be changed, otherwise we get the same pile-up events in all jobs.
# see fast sim manual
process.RandomNumberGeneratorService.famosPileUp.initialSeed = 132
# should we intialise also seeds for the rest? That is for:
# VtxSmeared
# famosSimHits
# siTrackerGaussianSmearingRecHits
# caloRecHits
# paramMuons
# l1ParamMuons
# MuonSimHits
# simMuonRPCDigis
# simMuonCSCDigis
# simMuonDTDigis

# Process to be generated
#process.load("TauAnalysis.GenSim.Sources.gensource_VBFHtautau_cfi")
#process.load("TauAnalysis.GenSim.Sources.gensource_VBFHtautau_mumu_cfi")
process.load("TauAnalysis.GenSim.Sources.gensource_VBFHtautau_muhad_cfi")
#process.load("TauAnalysis.GenSim.Sources.gensource_VBFHtoWW_cfi")
#process.load("TauAnalysis.GenSim.Sources.gensource_ZtoTaus_PYTHIA_cfi")
#process.load("TauAnalysis.GenSim.Sources.gensource_ZtoMus_PYTHIA_cfi")

process.source.maxEventsToPrint = cms.untracked.int32(10)
process.source.pythiaPylistVerbosity = cms.untracked.int32(1)

# L1 Menu and prescale factors : useful for testing all L1 paths
process.load("Configuration.StandardSequences.L1TriggerDefaultMenu_cff")

# Common inputs, with fake conditions
process.load("FastSimulation.Configuration.CommonInputs_cff")

# L1 Emulator and HLT Setup
process.load("FastSimulation.HighLevelTrigger.HLTSetup_cff")

# Famos sequences
process.load("FastSimulation.Configuration.FamosSequences_cff")

# HLT paths - defined by configDB
# This one is created on the fly by FastSimulation/Configuration/test/IntegrationTestWithHLT_py.csh
# Which paths? Do we need 14TeV low-lumi HLT table or 10GeV menu (default)?
process.load("FastSimulation.Configuration.HLT_cff")

# Only event accepted by L1 + HLT are reconstructed
process.HLTEndSequence = cms.Sequence(process.reconstructionWithFamos)

#MC filter (two taus within detector acceptance required)
process.filter2taus = cms.EDFilter("MCParticlePairFilter",
    ParticleID1 = cms.untracked.vint32(15),
    ParticleID2 = cms.untracked.vint32(-15),
    Status = cms.untracked.vint32(2, 2),
    MinPt = cms.untracked.vdouble(5.0, 5.0),
    MaxEta = cms.untracked.vdouble(2.4, 2.4),
    MinEta = cms.untracked.vdouble(-2.4, -2.4),
    MinDeltaPhi = cms.untracked.double(0.0),
    MaxDeltaPhi = cms.untracked.double(6.29),
    ParticleCharge = cms.untracked.int32(0),
    MaxInvMass = cms.untracked.double(14000.0),
    MinInvMass = cms.untracked.double(0.0)
)


# Schedule the HLT paths
process.mcFltP = cms.Path(
    process.filter2taus
    )
process.schedule = cms.Schedule()
process.schedule.insert(0, process.mcFltP)
process.schedule.extend(process.HLTSchedule)

# If uncommented : All events are reconstructed, including those rejected at L1/HLT
process.reconstruction = cms.Path(process.filter2taus*
                                  process.reconstructionWithFamos)
process.schedule.append(process.reconstruction)

# Simulation sequence
process.simulation = cms.Sequence(process.filter2taus*
                                  process.simulationWithFamos)
# You many not want to simulate everything
process.famosSimHits.SimulateCalorimetry = True
process.famosSimHits.SimulateTracking = True

# Parametrized magnetic field (new mapping, 4.0 and 3.8T)
process.load("Configuration.StandardSequences.MagneticField_40T_cff")
#process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.VolumeBasedMagneticFieldESProducer.useParametrizedTrackerField = True

# Number of pileup events per crossing
process.famosPileUp.PileUpSimulator.averageNumber = 0.0 # 4.3
# use correct files with pileup (for 14GeV)
process.famosPileUp.PileUpSimulator.fileNames = cms.untracked.vstring(
            'MinBias_001.root', 
            'MinBias_002.root', 
            'MinBias_003.root', 
            'MinBias_004.root', 
            'MinBias_005.root', 
            'MinBias_006.root', 
            'MinBias_007.root', 
            'MinBias_008.root', 
            'MinBias_009.root', 
            'MinBias_010.root')
# use proper vertex parameters (is it global or only for pileup?)
from IOMC.EventVertexGenerators.VtxSmearedParameters_cfi import *
process.myVertexGenerator = cms.PSet(
    NominalCollisionVtxSmearingParameters,
    type = cms.string('BetaFunc')
)

# Get frontier conditions   - not applied in the HCAL, see below
# where is the page listing the available conditions?
# Values for globaltag are "STARTUP_V3::All", "1PB::All", "10PB::All", "IDEAL_V4::All"
# see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions
process.GlobalTag.globaltag = "IDEAL_V11::All"

# To write out events 
process.load("FastSimulation.Configuration.EventContent_cff")
process.o1 = cms.OutputModule("PoolOutputModule",
    process.AODSIMEventContent,
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('mcFltP') ),
    fileName = cms.untracked.string('aod_fastHLT.root')
)
process.outpath = cms.EndPath(process.o1)

# Add endpaths to the schedule
process.schedule.append(process.outpath)
