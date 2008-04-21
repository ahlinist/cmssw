import FWCore.ParameterSet.Config as cms

source = cms.Source("PythiaSource",
    pythiaPylistVerbosity = cms.untracked.int32(0),
    filterEfficiency = cms.untracked.double(0.728),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    # breaking news 10 TeV center of mass energy...
    comEnergy = cms.untracked.double(10000.0),
    crossSection = cms.untracked.double(23690.0),
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
        processParameters = cms.vstring('MSEL        = 0    !User defined processes', 
            'MSUB(2)     = 1    !W production', 
            'MDME(190,1) = 0    !W decay into dbar u', 
            'MDME(191,1) = 0    !W decay into dbar c', 
            'MDME(192,1) = 0    !W decay into dbar t', 
            'MDME(194,1) = 0    !W decay into sbar u', 
            'MDME(195,1) = 0    !W decay into sbar c', 
            'MDME(196,1) = 0    !W decay into sbar t', 
            'MDME(198,1) = 0    !W decay into bbar u', 
            'MDME(199,1) = 0    !W decay into bbar c', 
            'MDME(200,1) = 0    !W decay into bbar t', 
            'MDME(205,1) = 0    !W decay into bbar tp', 
            'MDME(206,1) = 1    !W decay into e+ nu_e', 
            'MDME(207,1) = 1    !W decay into mu+ nu_mu', 
            'MDME(208,1) = 0    !W decay into tau+ nu_tau')
    )
)

egenfilter = cms.EDFilter("PythiaFilter",
    Status = cms.untracked.int32(3),
    MotherID = cms.untracked.int32(24),
    MinPt = cms.untracked.double(5.0),
    ParticleID = cms.untracked.int32(11),
    MaxEta = cms.untracked.double(2.7),
    MinEta = cms.untracked.double(-2.7)
)

mugenfilter = cms.EDFilter("PythiaFilter",
    Status = cms.untracked.int32(3),
    MotherID = cms.untracked.int32(24),
    MinPt = cms.untracked.double(5.0),
    ParticleID = cms.untracked.int32(13),
    MaxEta = cms.untracked.double(2.7),
    MinEta = cms.untracked.double(-2.7)
)

configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1 $'),
    name = cms.untracked.string('$Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/Spring08Production/data/iCSA08_EWK_Wlnu.cff,v $'),
    annotation = cms.untracked.string('Wenu evts for iCSA08')
)
TmpSequence = cms.Sequence(~egenfilter+~mugenfilter)
ProductionFilterSequence = cms.Sequence(~TmpSequence)

