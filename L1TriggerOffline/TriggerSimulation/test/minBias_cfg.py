import FWCore.ParameterSet.Config as cms

process = cms.Process("PROD")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)
process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    moduleSeeds = cms.PSet(
        siTrackerGaussianSmearingRecHits = cms.untracked.uint32(24680),
        caloRecHits = cms.untracked.uint32(654321),
        paramMuons = cms.untracked.uint32(54525),
        famosSimHits = cms.untracked.uint32(13579),
        VtxSmeared = cms.untracked.uint32(123456789)
    ),
    sourceSeed = cms.untracked.uint32(123456789)
)

process.source = cms.Source("PythiaSource",
    pythiaPylistVerbosity = cms.untracked.int32(0),
    filterEfficiency = cms.untracked.double(1.0),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    comEnergy = cms.untracked.double(10000.0),
    maxEventsToPrint = cms.untracked.int32(0),
    PythiaParameters = cms.PSet(
        pythiaUESettings = cms.vstring('MSTJ(11)=3     ! Choice of the fragmentation function', 
            'MSTJ(22)=2     ! Decay those unstable particles', 
            'PARJ(71)=10 .  ! for which ctau  10 mm', 
            'MSTP(2)=1      ! which order running alphaS', 
            'MSTP(33)=0     ! no K factors in hard cross sections', 
            'MSTP(51)=10042 ! structure function chosen (external PDF CTEQ6L1)', 
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
            'MSTP(91)=1      !', 
            'PARP(91)=2.1   ! kt distribution', 
            'PARP(93)=15.0  ! '),
        processParameters = cms.vstring('MSEL=0         ! User defined processes', 
            'MSUB(11)=1     ! Min bias process', 
            'MSUB(12)=1     ! Min bias process', 
            'MSUB(13)=1     ! Min bias process', 
            'MSUB(28)=1     ! Min bias process', 
            'MSUB(53)=1     ! Min bias process', 
            'MSUB(68)=1     ! Min bias process', 
            'MSUB(92)=1     ! Min bias process, single diffractive', 
            'MSUB(93)=1     ! Min bias process, single diffractive', 
            'MSUB(94)=1     ! Min bias process, double diffractive', 
            'MSUB(95)=1     ! Min bias process'),
        parameterSets = cms.vstring('pythiaUESettings', 
            'processParameters')
    )
)


# track trigger
process.load("SLHCUpgradeSimulations.L1Trigger.TrackTriggerNaiveGeometry_cff")
process.load("SLHCUpgradeSimulations.L1Trigger.trackTriggerHitsFromMC_cfi")

#process.load("SLHCUpgradeSimulations.L1Trigger.dumpL1TrackHits_cfi")


process.tester = cms.EDFilter("EventSetupRecordDataGetter",
    verbose = cms.untracked.bool(True),
    toGet = cms.VPSet(
        record = cms.vstring("TrackTriggerNaiveGeometryRcd"),
        data = cms.vstring("TrackTriggerNaiveGeometry")
    )
)


process.output = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_trackTriggerHits_*_*', 
        'keep *_trackTriggerPrimitives_*_*'),
    fileName = cms.untracked.string('minBias.root')
)

process.p = cms.Path(process.tester*process.trackTriggerHits)
process.outpath = cms.EndPath(process.output)
process.trackTriggerHits.inputTag = 'source'
process.trackTriggerHits.doPileUp = False

