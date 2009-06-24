# ----------------------------------------------------------------------
# Example cfg file to produce (filtered) generator-level 'ntuples'
# NOTE: 'MSEL = 5' implies gluon fusion only. Use 'MSEL = 1' for more.
# ----------------------------------------------------------------------

import FWCore.ParameterSet.Config as cms

process = cms.Process("HF")
process.load("FWCore.Framework.test.cmsExceptionsFatal_cff")

process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    generator = cms.PSet(
        initialSeed = cms.untracked.uint32(10000),
        engineName = cms.untracked.string('HepJamesRandom')
    )
)
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(100))

# ----------------------------------------------------------------------
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.destinations = ['cerr']
process.MessageLogger.statistics = []
process.MessageLogger.fwkJobReports = []

# ----------------------------------------------------------------------
# -- generator
# ----------------------------------------------------------------------
process.source = cms.Source("EmptySource")
process.load("Configuration.Generator.PythiaUESettings_cfi")
from Configuration.Generator.PythiaUESettings_cfi import *

process.generator = cms.EDFilter("Pythia6GeneratorFilter",
    pythiaHepMCVerbosity = cms.untracked.bool(True),
    maxEventsToPrint = cms.untracked.int32(0),
    pythiaPylistVerbosity = cms.untracked.int32(1),
    comEnergy = cms.double(10000.0),
    ExternalDecays = cms.PSet(
        EvtGen = cms.untracked.PSet(
             operates_on_particles = cms.vint32( 0 ), # 0 (zero) means default list (hardcoded)
                                                      # you can put here the list of particles (PDG IDs)
                                                      # that you want decayed by EvtGen
	     use_default_decay = cms.untracked.bool(False),
             decay_table = cms.FileInPath('GeneratorInterface/ExternalDecays/data/DECAY_NOLONGLIFE.DEC'),
             # decay_table = cms.FileInPath('GeneratorInterface/ExternalDecays/data/DECAY.DEC'),
             particle_property_file = cms.FileInPath('GeneratorInterface/ExternalDecays/data/evt.pdl'),
             user_decay_file = cms.FileInPath('HeavyFlavorAnalysis/Examples/data/Bp2JpsiKp.dec'),
             list_forced_decays = cms.vstring('MyB+','MyB-'),
             ),
        parameterSets = cms.vstring('EvtGen')
    ),
    PythiaParameters = cms.PSet(
        process.pythiaUESettingsBlock,
        bbbarSettings = cms.vstring('MSEL = 5'), 
        # This is a vector of ParameterSet names to be read, in this order
        parameterSets = cms.vstring('pythiaUESettings','bbbarSettings')
    )
)


# ----------------------------------------------------------------------
process.bfilter = cms.EDFilter("PythiaFilter",
    ParticleID = cms.untracked.int32(521)
)
process.mumugenfilter = cms.EDFilter("MCParticlePairFilter",
    moduleLabel = cms.untracked.string('generator'),
    Status = cms.untracked.vint32(1, 1),
    MinPt = cms.untracked.vdouble(2.5, 2.5),
    MaxEta = cms.untracked.vdouble(2.5, 2.5),
    MinEta = cms.untracked.vdouble(-2.5, -2.5),
    ParticleCharge = cms.untracked.int32(0),
    # Use the following to require J/psi -> mu+ mu- 
    # MaxInvMass = cms.untracked.double(3.15),
    # MinInvMass = cms.untracked.double(3.05),
    ParticleID1 = cms.untracked.vint32(13),
    ParticleID2 = cms.untracked.vint32(13)
)

# ----------------------------------------------------------------------
process.HepPDTESSource = cms.ESSource("HepPDTESSource",
    pdtFileName = cms.FileInPath('SimGeneral/HepPDTESSource/data/particle.tbl')
)
process.genParticles = cms.EDProducer("GenParticleProducer",
    saveBarCodes = cms.untracked.bool(True),
    src = cms.InputTag("generator"),
    abortOnUnknownPDGCode = cms.untracked.bool(False)
)

# ----------------------------------------------------------------------
process.genDump = cms.EDFilter("HFDumpGenerator",
    generatorCandidates = cms.untracked.string('genParticles'),
    generatorEvent = cms.untracked.string('generator')
)


# ----------------------------------------------------------------------
process.tree = cms.EDAnalyzer("HFTree",
    fileName = cms.string('test.root')
)

# ----------------------------------------------------------------------
process.p = cms.Path(process.generator*
                     process.bfilter*
                     process.mumugenfilter*
                     process.genParticles*
                     process.genDump*
                     process.tree
                     )

process.schedule = cms.Schedule(process.p)
