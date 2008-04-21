# The following comments couldn't be translated into the new config version:

#MCfilter &

#MCfilter &

# Reconstruction sequence
import FWCore.ParameterSet.Config as cms

process = cms.Process("HLT")
process.load("FastSimulation.Configuration.CommonIncludesForProd_cff")

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

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
    name = cms.untracked.string('$Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/Spring08Production/data/Spring08_MadGraph_Qcut30_FAST.cfg,v $'),
    annotation = cms.untracked.string('generation of Madgraph, CTEQ 6L1 used')
)
process.Timing = cms.Service("Timing")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)
process.source = cms.Source("MadGraphSource",
    # parameters related to ME-PS matching
    produceEventTreeFile = cms.untracked.bool(False),
    pythiaPylistVerbosity = cms.untracked.int32(0),
    # turn to standard sources way of inputting filename
    fileNames = cms.untracked.vstring('file:events.lhe'),
    MEMAIN_qcut = cms.untracked.double(30.0),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    # values for the MEMAIN routine (matching). if set to 0. default values will be chosen from the interface
    MEMAIN_etaclmax = cms.untracked.double(5.0),
    # for reading non-MG LHE files
    minimalLH = cms.untracked.bool(False),
    # general parameters
    firstEvent = cms.untracked.uint32(0),
    MEMAIN_iexcfile = cms.untracked.uint32(0), ## only set to 1 if need to perform exclusive matching

    maxEventsToPrint = cms.untracked.int32(5),
    # for reading from castor
    getInputFromMCDB = cms.untracked.bool(False),
    MCDBArticleID = cms.int32(0),
    # PYTHIA
    PythiaParameters = cms.PSet(
        pythiaCMSDefaults = cms.vstring('PMAS(5,1)=4.4  ! b quarks mass', 
            'PMAS(6,1)=175  ! t quarks mass', 
            'MSTJ(1)=1      !...Fragmentation/hadronization on or off', 
            'MSTP(61)=1     ! Parton showering on or off', 
            'MSTP(143)=1    ! MUST BE 1 FOR THE MATCHING ROUTINE TO RUN!!!!', 
            'MSEL=0         ! User defined processes/Full user control'),
        pythiaUESettings = cms.vstring('MSTJ(11)=3     ! Choice of the fragmentation function', 
            'MSTJ(22)=2     ! Decay those unstable particles', 
            'PARJ(71)=10 .  ! for which ctau  10 mm', 
            'MSTP(2)=1      ! which order running alphaS', 
            'MSTP(33)=0     ! no K factors in hard cross sections', 
            'MSTP(51)=10042     ! CTEQ6L1 structure function chosen', 
            'MSTP(52)=2     ! work with LHAPDF', 
            'MSTP(81)=1     ! multiple parton interactions 1 is Pythia default', 
            'MSTP(82)=4     ! Defines the multi-parton model', 
            'MSTU(21)=1     ! Check on possible errors during program execution', 
            'PARP(82)=1.8387   ! pt cutoff for multiparton interactions', 
            'PARP(89)=1960. ! sqrts for which PARP82 is set', 
            'PARP(83)=0.5   ! Multiple interactions: matter distrbn parameter', 
            'PARP(84)=0.4   ! Multiple interactions: matter distribution parameter', 
            'PARP(90)=0.16  ! Multiple interactions: rescaling power', 
            'PARP(67)=2.5    ! amount of initial-state radiation', 
            'PARP(85)=1.0  ! gluon prod. mechanism in MI', 
            'PARP(86)=1.0  ! gluon prod. mechanism in MI', 
            'PARP(62)=1.25   ! ', 
            'PARP(64)=0.2    ! ', 
            'MSTP(91)=1     !', 
            'PARP(91)=2.1   ! kt distribution', 
            'PARP(93)=15.0  ! '),
        # This is a vector of ParameterSet names to be read, in this order
        # The first is general default pythia parameters, the second are own additions
        parameterSets = cms.vstring('pythiaUESettings', 
            'pythiaCMSDefaults')
    )
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
process.MessageLogger.destinations = ['detailedInfo.txt']

