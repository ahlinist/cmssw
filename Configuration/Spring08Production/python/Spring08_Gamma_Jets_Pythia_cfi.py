import FWCore.ParameterSet.Config as cms

source = cms.Source("PythiaSource",
    pythiaPylistVerbosity = cms.untracked.int32(0),
    # put here the efficiency of your filter (1. if no filter) dummy
    filterEfficiency = cms.untracked.double(0.0089),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    # breaking news 10 TeV center of mass energy...
    comEnergy = cms.untracked.double(10000.0),
    # put here the cross section of your process (in pb) dummy
    crossSection = cms.untracked.double(58700.0),
    maxEventsToPrint = cms.untracked.int32(0),
    PythiaParameters = cms.PSet(
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
        parameterSets = cms.vstring('pythiaUESettings', 
            'pythiaHgg'),
        pythiaHgg = cms.vstring('MSEL=0                  ! User defined processes', 
            'MSUB(14) = 1            ! q qbar -> g gam', 
            'MSUB(29) = 1            ! q g    -> q gam', 
            'MSUB(115) = 1           ! g g    -> g gam', 
            'CKIN(3)=25              ! pthat min for 2->2 interaction', 
            'CKIN(4)=-1              ! pthat max for 2->2 interaction')
    )
)


