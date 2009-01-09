import FWCore.ParameterSet.Config as cms

process = cms.Process("Ana")
process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("PhysicsTools.HepMCCandAlgos.genParticleCandidatesFast_cfi")

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

#process.load("JetMETCorrections.Configuration.L2L3Corrections_iCSA08_1PBV2_cff")

process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    moduleSeeds = cms.PSet(
        g4SimHits = cms.untracked.uint32(11),
        mix = cms.untracked.uint32(12345),
        VtxSmeared = cms.untracked.uint32(98765432)
    ),
    sourceSeed = cms.untracked.uint32(123456789)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/relval/CMSSW_2_1_0/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/IDEAL_V5_v1/0000/00E298BC-6560-DD11-8FA5-001617E30F4C.root')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(20)
)
#process.prefer("L2JetCorrectorScone7")
process.analyzer = cms.EDAnalyzer("InclusiveJetAnalyzer",
    ptBins = cms.untracked.vdouble(0.0, 50.0, 60.0, 80.0, 95.0, 
        120.0, 145.0, 170.0, 195.0, 230.0, 
        250.0, 300.0, 335.0, 380.0, 425.0, 
        470.0, 520.0, 600.0, 700.0, 800.0, 
        900.0, 1000.0, 1200.0, 1400.0, 1600.0, 
        1800.0, 2000.0, 2200.0, 2400.0, 2600.0, 
        2800.0, 3000.0, 3250.0, 3500.0, 3750.0, 
        4000.0),
    ET30Stat_ptHats = cms.untracked.vuint32(0, 3641046, 429238, 45234, 12677, 
        3405),
    totStat_ptHats = cms.untracked.vuint32(3131233, 4342174, 4022000, 2554579, 3848194, 
        5213220),
    rapBins = cms.untracked.vdouble(0.55, 1.1, 1.7, 2.5, 3.2, 
        5.0),
    ptHatMins = cms.untracked.vdouble(30.0, 45.0, 75.0, 120.0, 160.0, 
        220.0, 7000.0),
    ET20Stat_ptHats = cms.untracked.vuint32(3131233, 701128, 82474, 8799, 2294, 
        672),
    ET110Stat_ptHats = cms.untracked.vuint32(0, 0, 0, 0, 3132223, 
        847777),
    samplesXsection = cms.untracked.vdouble(101600000.0, 21550000.0, 2484000.0, 323700.0, 88730.0, 
        17120.0),
    outFile = cms.untracked.string('Output.root'),
    noGo = cms.untracked.vstring('MET'),
    totStat_samples = cms.untracked.vuint32(3926600, 4131600, 4010400, 2891200, 3980000, 
        4171600),
    mustHave = cms.untracked.vstring('kt6', 
        'Kt6'),
    ET80Stat_ptHats = cms.untracked.vuint32(0, 0, 0, 2131885, 597572, 
        161743),
    ptMin = cms.untracked.double(50.0),
    ET50Stat_ptHats = cms.untracked.vuint32(0, 0, 3510288, 368661, 103428, 
        28023),
    ET150Stat_ptHats = cms.untracked.vuint32(0, 0, 0, 0, 0, 
        4171600),
    #	possible modes: noWeight|xSection|xSectionCsaOverlay                         
    mode = cms.untracked.string('noWeight')
)

process.p = cms.Path(process.analyzer)


