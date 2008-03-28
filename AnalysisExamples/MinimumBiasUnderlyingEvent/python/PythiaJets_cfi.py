import FWCore.ParameterSet.Config as cms

# Disclaimer :
#
# This set of Pythia configuration cards has been copied
# and pasted from
#   RecoTracker/RoadSearchCloudMaker/test/sim_pythia.cfg
# prepared by Oliver Gutsche (FNAL)
#
# the only change is MSTP(128)=2 - this option takes decay
# products out of doc section; decay products point at parents
# in the main section (Julia Yarba, FNAL).
source = cms.Source("PythiaSource",
    maxEvents = cms.untracked.int32(10),
    # to printout pythia event record (call pylist)
    pythiaPylistVerbosity = cms.untracked.int32(0),
    tuneModel = cms.untracked.int32(100),
    # to printout HepMC::GenEvent record (HepMC::GenEvent::print())
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    maxEventsToPrint = cms.untracked.int32(0),
    PythiaParameters = cms.PSet(
        pythiaJets = cms.vstring('MSEL=0         ! User defined processes', 'MSUB(11)=1     ! Min bias process', 'MSUB(12)=1     ! Min bias process', 'MSUB(13)=1     ! Min bias process', 'MSUB(28)=1     ! Min bias process', 'MSUB(53)=1     ! Min bias process', 'MSUB(68)=1     ! Min bias process', 'MSUB(92)=1     ! Min bias process, single diffractive', 'MSUB(93)=1     ! Min bias process, single diffractive', 'MSUB(94)=1     ! Min bias process, double diffractive', 'MSUB(95)=1     ! Min bias process', 'MSTJ(11)=3     ! Choice of the fragmentation function', 'MSTJ(22)=2     ! Decay those unstable particles', 'PARJ(71)=10 .  ! for which ctau  10 mm', 'MSTP(2)=1      ! which order running alphaS', 'MSTP(33)=0     ! no K factors in hard cross sections', 'MSTP(51)=7     ! structure function chosen', 'MSTP(81)=1     ! multiple parton interactions 1 is Pythia default', 'MSTP(82)=4     ! Defines the multi-parton model', 'MSTU(21)=1     ! Check on possible errors during program execution', 'PARP(82)=1.9409   ! pt cutoff for multiparton interactions', 'PARP(89)=1960. ! sqrts for which PARP82 is set', 'PARP(83)=0.5   ! Multiple interactions: matter distrbn parameter', 'PARP(84)=0.4   ! Multiple interactions: matter distribution parameter', 'PARP(90)=0.16  ! Multiple interactions: rescaling power', 'PARP(67)=2.5    ! amount of initial-state radiation', 'PARP(85)=1.0  ! gluon prod. mechanism in MI', 'PARP(86)=1.0  ! gluon prod. mechanism in MI', 'PARP(62)=1.25   ! ', 'PARP(64)=0.2    ! ', 'MSTP(91)=1     !', 'PARP(91)=2.1   ! kt distribution', 'PARP(93)=15.0  ! '),
        parameterSets = cms.vstring('pythiaJets')
    )
)


