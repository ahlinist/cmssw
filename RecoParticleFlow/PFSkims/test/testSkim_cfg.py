# The following comments couldn't be translated into the new config version:

#	untracked int32	pythiaPylistVerbosity =12

# J/psi decays

# 
# source = PoolSource {
# untracked vstring fileNames = {
# "file:D0632627-8949-DC11-BCFE-000423D999CA.root"
# }
# }   
# 

import FWCore.ParameterSet.Config as cms

process = cms.Process("PROD")
# Keep the logging output to a nice level #
process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("RecoParticleFlow.PFSkims.jpsiee_SkimPaths_cff")

process.load("RecoParticleFlow.PFSkims.jpsiee_OutputModule_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
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

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)
process.evtInfo = cms.OutputModule("AsciiOutputModule")

process.end = cms.EndPath(process.evtInfo*process.jpsieeOutputModuleFEVTSIM)
process.MessageLogger.destinations = ['detailedInfo.txt']
process.jpsieeOutputModuleRECOSIM.fileName = 'jpsiee.root'


