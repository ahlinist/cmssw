# The following comments couldn't be translated into the new config version:

# This is a vector of ParameterSet names to be read, in this order
# The first two are in the include files below
# The last one are simply my additional parameters

import FWCore.ParameterSet.Config as cms

source = cms.Source("PythiaMuEnrichSource",
    numberEventsInRun = cms.untracked.uint32(0),
    nmu_min = cms.untracked.int32(1),
    pythiaPylistVerbosity = cms.untracked.int32(0),
    maxeta = cms.untracked.double(2.5),
    wtype = cms.untracked.int32(1),
    wbc = cms.untracked.double(0.1),
    maxEvents = cms.untracked.int32(0),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    ptmin = cms.untracked.double(1.2),
    wmin = cms.untracked.double(0.1),
    maxEventsToPrint = cms.untracked.int32(0),
    PythiaParameters = cms.PSet(
        parameterSets = cms.vstring('pythiaDefault', 'pythiaMinBias'),
        pythiaMinBias = cms.vstring('MSEL=0         ! User defined processes', 'MSUB(11)=1     ! Min bias process', 'MSUB(12)=1     ! Min bias process', 'MSUB(13)=1     ! Min bias process', 'MSUB(28)=1     ! Min bias process', 'MSUB(53)=1     ! Min bias process', 'MSUB(68)=1     ! Min bias process', 'MSUB(95)=1     ! Min bias process', 'MSTJ(11)=3     ! Choice of the fragmentation function', 'MSTJ(22)=1     ! Decay those unstable particles', 'PARJ(73)=2950  ! radius of the decay cylinder', 'PARJ(74)=5600  ! length of the decay cylinder', 'MSTP(2)=1      ! which order running alphaS', 'MSTP(33)=0     ! no K factors in hard cross sections', 'MSTP(51)=7     ! structure function chosen', 'MSTP(81)=1     ! multiple parton interactions 1 is Pythia default', 'MSTP(82)=4     ! Defines the multi-parton model', 'MSTU(21)=1     ! Check on possible errors during program execution', 'PARP(82)=1.9409   ! pt cutoff for multiparton interactions', 'PARP(89)=1960. ! sqrts for which PARP82 is set', 'PARP(83)=0.5   ! Multiple interactions: matter distrbn parameter', 'PARP(84)=0.4   ! Multiple interactions: matter distribution parameter', 'PARP(90)=0.16  ! Multiple interactions: rescaling power', 'PARP(67)=2.5    ! amount of initial-state radiation', 'PARP(85)=1.0  ! gluon prod. mechanism in MI', 'PARP(86)=1.0  ! gluon prod. mechanism in MI', 'PARP(62)=1.25   ! ', 'PARP(64)=0.2    ! ', 'MSTP(91)=1     !', 'PARP(91)=2.1   ! kt distribution', 'PARP(93)=15.0  ! ', 'CKIN(3)=0 ! pthat cut-off'),
        pythiaDefault = cms.vstring('PMAS(5,1)=4.8 ! b quark mass', 'PMAS(6,1)=172.3 ! t quark mass')
    )
)


