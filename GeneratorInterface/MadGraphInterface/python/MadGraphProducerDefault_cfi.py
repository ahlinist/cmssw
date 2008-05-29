import FWCore.ParameterSet.Config as cms

madg = cms.EDProducer("MadGraphProducer",
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    maxEventsToPrint = cms.untracked.int32(1),
    pythiaPylistVerbosity = cms.untracked.int32(0),
    PythiaParameters = cms.PSet(
        # pythiaCMSDefaults as used in other cfi files
        pythiaCMSDefaults = cms.vstring('PMAS(5,1)=4.4   ! b quarks mass', 
            'PMAS(6,1)=172.3 ! t quarks mass', 
            'MSTJ(11)=3     ! Choice of the fragmentation function', 
            'MSTJ(22)=2     ! Decay those unstable particles', 
            'PARJ(71)=10 .  ! for which ctau  10 mm', 
            'MSTP(2)=1      ! which order running alphaS', 
            'MSTP(33)=0     ! no K factors in hard cross sections', 
            'MSTP(51)=7     ! structure function chosen', 
            'MSTP(81)=1     ! multiple parton interactions 1 is Pythia default', 
            'MSTP(82)=4     ! Defines the multi-parton model', 
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
            'MSEL=0         ! User defined processes/Full user control'),
        # This is a vector of ParameterSet names to be read, in this order
        # The first is general default pythia parameters, the second are own additions
        parameterSets = cms.vstring('pythiaCMSDefaults')
    )
)


