# The following comments couldn't be translated into the new config version:

#	untracked int32	pythiaPylistVerbosity =12

# J/psi decays

#  

import FWCore.ParameterSet.Config as cms

process = cms.Process("PROD")
#    service = SimpleProfiling { }
process.load("RecoParticleFlow.PFSkims.JPsieeFilter_cfi")

# Keep the logging output to a nice level #
process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10000)
)
process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    sourceSeed = cms.untracked.uint32(123456789)
)

process.source = cms.Source("PythiaSource",
    maxEventsToPrint = cms.untracked.int32(5),
    PythiaParameters = cms.PSet(
        #
        # Default cards for minimum bias events (unfiltered)
        # Name of the set is "pythiaMinBias"
        #include "IOMC/GeneratorInterface/test/pythiaMinBias.cfg"
        #
        # User cards - name is "myParameters"
        # Pythia's random generator initialization 
        myParameters = cms.vstring('MSEL=0 ! Your fate is in the following cards ', 
            'MSUB(86)=1 !gg->J/Psi g', 
            'MDME(858,1)=1 ! J/psi->e+e-', 
            'MDME(859,1)=0 ! J/psi->mumu', 
            'MDME(860,1)=0'),
        # This is a vector of ParameterSet names to be read, in this order
        # The first two are in the include files below
        # The last one are simply my additional parameters
        parameterSets = cms.vstring('myParameters')
    )
)

process.DaqMonitorROOTBackEnd = cms.Service("DaqMonitorROOTBackEnd")

process.Timing = cms.Service("Timing")

process.o1 = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring('keep *', 
        'drop *_mix_*_*'),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('p1')
    ),
    fileName = cms.untracked.string('test.root')
)

process.p1 = cms.Path(process.jpsiupsilon_filter)
process.outpath = cms.EndPath(process.o1)
process.MessageLogger.destinations = ['detailedInfo.txt']


