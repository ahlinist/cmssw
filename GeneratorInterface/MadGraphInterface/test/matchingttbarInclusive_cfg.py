#
import FWCore.ParameterSet.Config as cms

process = cms.Process("MATCHINGTTBARINCL")
# MadGraph Interface
process.load("GeneratorInterface.MadGraphInterface.MadGraphSourceDefault_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    sourceSeed = cms.untracked.uint32(824177121)
)

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('MGtest.root')
)

process.p = cms.EndPath(process.out)
process.MadGraphSource.fileNames = ['file:ttbarInclusive_xqcut20.lhe'] # rfcp /castor/cern.ch/user/d/dkcira/MadGraph/Samples/ttbarInclusive_xqcut20.lhe .
# process.MadGraphSource.produceEventTreeFile = True # used for checking matching but should be turned off in production, it messes up the status of particles
process.MadGraphSource.MEMAIN_etaclmax = 5.
process.MadGraphSource.MEMAIN_qcut = 30.
process.MadGraphSource.MEMAIN_iexcfile = 0 # only set to 1 if need to perform exclusive matching
process.MadGraphSource.PythiaParameters = cms.PSet(
    parameterSets = cms.vstring('pythiaMatching'),
    pythiaMatching = cms.vstring('PMAS(5,1)=4.4  ! b quarks mass', 
        'PMAS(6,1)=174.2! t quarks mass', 
        'MSTJ(1)=1      !...Fragmentation/hadronization on or off', 
        'MSTJ(11)=3     ! Choice of the fragmentation function', 
        'MSTJ(22)=2     ! Decay those unstable particles', 
        'PARJ(71)=10.   ! for which ctau  10 mm', 
        'MSTP(2)=1      ! which order running alphaS', 
        'MSTP(33)=0     ! no K factors in hard cross sections', 
        'MSTP(51)=7     ! structure function chosen', 
        'MSTP(61)=1     ! Parton showering on or off', 
        'MSTP(71)=1     !', 
        'MSTP(81)=0     ! multiple parton interactions 1 is Pythia default', 
        'MSTP(82)=0     ! Defines the multi-parton model', 
        'MSTP(143)=1    ! MUST BE 1 FOR THE MATCHING ROUTINE TO RUN!!!!', 
        'MSTU(21)=1     ! Check on possible errors during program execution', 
        'PARP(82)=1.9   ! pt cutoff for multiparton interactions', 
        'PARP(89)=1000. ! sqrts for which PARP82 is set', 
        'PARP(83)=0.5   ! Multiple interactions: matter distrbn parameter', 
        'PARP(84)=0.4   ! Multiple interactions: matter distribution parameter', 
        'PARP(90)=0.16  ! Multiple interactions: rescaling power', 
        'PARP(67)=1.    ! amount of initial-state radiation', 
        'PARP(85)=0.33  ! gluon prod. mechanism in MI', 
        'PARP(86)=0.66  ! gluon prod. mechanism in MI', 
        'PARP(87)=0.7   ! ', 
        'PARP(88)=0.5   ! ', 
        'PARP(91)=1.0   ! kt distribution', 
        'MSEL=0         ! User defined processes/Full user control')
)


