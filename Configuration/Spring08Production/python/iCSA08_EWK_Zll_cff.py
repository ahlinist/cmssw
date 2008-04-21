import FWCore.ParameterSet.Config as cms

source = cms.Source("PythiaSource",
    pythiaPylistVerbosity = cms.untracked.int32(0),
    filterEfficiency = cms.untracked.double(0.5445),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    # breaking news 10 TeV center of mass energy...
    comEnergy = cms.untracked.double(10000.0),
    crossSection = cms.untracked.double(2483.0),
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
        # This is a vector of ParameterSet names to be read, in this order
        parameterSets = cms.vstring('pythiaUESettings', 
            'processParameters'),
        processParameters = cms.vstring('MSEL=0            !User defined processes', 
            'MSUB(1)=1         !Incl Z0/gamma* production', 
            'MSTP(43)=3        !Both Z0 and gamma*', 
            'MDME(174,1)=0     !Z decay into d dbar', 
            'MDME(175,1)=0     !Z decay into u ubar', 
            'MDME(176,1)=0     !Z decay into s sbar', 
            'MDME(177,1)=0     !Z decay into c cbar', 
            'MDME(178,1)=0     !Z decay into b bbar', 
            'MDME(179,1)=0     !Z decay into t tbar', 
            'MDME(182,1)=1     !Z decay into e- e+', 
            'MDME(183,1)=0     !Z decay into nu_e nu_ebar', 
            'MDME(184,1)=1     !Z decay into mu- mu+', 
            'MDME(185,1)=0     !Z decay into nu_mu nu_mubar', 
            'MDME(186,1)=0     !Z decay into tau- tau+', 
            'MDME(187,1)=0     !Z decay into nu_tau nu_taubar', 
            'CKIN(1)=40.       !Minimum sqrt(s_hat) value (=Z mass)')
    )
)

eegenfilter = cms.EDFilter("MCParticlePairFilter",
    Status = cms.untracked.vint32(1, 1),
    MinPt = cms.untracked.vdouble(5.0, 5.0),
    MaxEta = cms.untracked.vdouble(2.7, 2.7),
    MinEta = cms.untracked.vdouble(-2.7, -2.7),
    ParticleCharge = cms.untracked.int32(0),
    ParticleID1 = cms.untracked.vint32(11),
    ParticleID2 = cms.untracked.vint32(11)
)

mumugenfilter = cms.EDFilter("MCParticlePairFilter",
    Status = cms.untracked.vint32(3, 3),
    MinPt = cms.untracked.vdouble(5.0, 5.0),
    MaxEta = cms.untracked.vdouble(2.7, 2.7),
    MinEta = cms.untracked.vdouble(-2.7, -2.7),
    ParticleCharge = cms.untracked.int32(0),
    ParticleID1 = cms.untracked.vint32(13),
    ParticleID2 = cms.untracked.vint32(13)
)


