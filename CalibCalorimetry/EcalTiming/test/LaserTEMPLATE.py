import FWCore.ParameterSet.Config as cms

process = cms.Process("ECALTIMING")
process.load("EventFilter.EcalRawToDigiDev.EcalUnpackerMapping_cfi")

process.load("EventFilter.EcalRawToDigiDev.EcalUnpackerData_cfi")

process.load("Geometry.CaloEventSetup.CaloTopology_cfi")

process.load("Geometry.EcalCommonData.EcalOnly_cfi")

process.load("Geometry.CaloEventSetup.CaloGeometry_cff")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.GlobalTag.globaltag = 'GR09_31X_V4P::All'


process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(999999)
        )
process.source = cms.Source("PoolSource",
                                debugFlag = cms.untracked.bool(True),
                                debugVebosity = cms.untracked.uint32(10),
                                skipEvents = cms.untracked.uint32(0),
                                fileNames = cms.untracked.vstring('/store/data/Commissioning08/TestEnables/RAW/v1/000/064/735/8868E0F8-8594-DD11-9EC6-000423D98930.root',
                                                                          '/store/data/Commissioning08/TestEnables/RAW/v1/000/064/735/9A01DEFD-8794-DD11-A1F5-001617E30F48.root',
                                                                          '/store/data/Commissioning08/TestEnables/RAW/v1/000/064/735/C0427F8D-8694-DD11-8E98-001617DBCF1E.root')
                            )

#process.src1 = cms.ESSource("EcalTrivialConditionRetriever",
#                                jittWeights = cms.untracked.vdouble(0.04, 0.04, 0.04, 0.0, 1.32,
#                                                                            -0.05, -0.5, -0.5, -0.4, 0.0),
#                                pedWeights = cms.untracked.vdouble(0.333, 0.333, 0.333, 0.0, 0.0,
#                                                                           0.0, 0.0, 0.0, 0.0, 0.0),
#                                amplWeights = cms.untracked.vdouble(-0.333, -0.333, -0.333, 0.0, 0.0,
#                                                                            1.0, 0.0, 0.0, 0.0, 0.0)
#                            )

process.uncalibHitMaker = cms.EDProducer("EcalUncalibRecHitProducer",
                                             EEdigiCollection = cms.InputTag("ecalDccDigis","eeDigiSkim"),
                                             betaEE = cms.double(1.37),
                                             alphaEE = cms.double(1.63),
                                             EBdigiCollection = cms.InputTag("ecalDccDigis","ebDigiSkim"),
                                             EEhitCollection = cms.string('EcalUncalibRecHitsEE'),
                                             AlphaBetaFilename = cms.untracked.string('NOFILE'),
                                             betaEB = cms.double(1.7),
                                             MinAmplEndcap = cms.double(14.0),
                                             MinAmplBarrel = cms.double(8.0),
                                             alphaEB = cms.double(1.2),
                                             UseDynamicPedestal = cms.bool(True),
                                             EBhitCollection = cms.string('EcalUncalibRecHitsEB'),
                                             algo = cms.string("EcalUncalibRecHitWorkerFixedAlphaBetaFit")
                                         )

process.ecalDccDigis = cms.EDFilter("EcalDccDigiSkimer",
                                        EEdigiCollectionOut = cms.string('eeDigiSkim'),
                                        EEdigiCollection = cms.InputTag("ecalEBunpacker","eeDigis"),
                                        EBdigiCollectionOut = cms.string('ebDigiSkim'),
                                        EBdigiCollection = cms.InputTag("ecalEBunpacker","ebDigis"),
                                        DigiUnpacker = cms.InputTag("ecalEBunpacker"),
                                        DigiType = cms.string('Laser')
                                    )

process.timing = cms.EDFilter("EcalTimingAnalysis",
                                  rootfile = cms.untracked.string('Laser_RUNNUMBER.root'),
                                  CorrectBH = cms.untracked.bool(False),
                                  hitProducer = cms.string('uncalibHitMaker'),
                                  minNumEvt = cms.untracked.double(20.0),
                                  FromFileName = cms.untracked.string('Emptyfile.root'),
                                  TTPeakTime = cms.untracked.string('TTPeakPositionFileLaser.txt'),
                                  SMAverages = cms.untracked.vdouble(5.0703, 5.2278, 5.2355, 5.1548, 5.1586,
                                                                             5.1912, 5.1576, 5.1625, 5.1269, 5.643,
                                                                             5.6891, 5.588, 5.5978, 5.6508, 5.6363,
                                                                             5.5972, 5.6784, 5.6108, 5.6866, 5.6523,
                                                                             5.6666, 5.7454, 5.729, 5.7751, 5.7546,
                                                                             5.7835, 5.7529, 5.5691, 5.6677, 5.5662,
                                                                             5.6308, 5.7097, 5.6773, 5.76, 5.8025,
                                                                             5.9171, 5.8771, 5.8926, 5.9011, 5.8447,
                                                                             5.8142, 5.8475, 5.7123, 5.6216, 5.6713,
                                                                             5.3747, 5.3564, 5.39, 5.8081, 5.8081,
                                                                             5.1818, 5.1125, 5.1334, 5.2581),
                                  hitProducerEE = cms.string('uncalibHitMaker'),
                                  amplThr = cms.untracked.double(25.0),
                                  SMCorrections = cms.untracked.vdouble(0.0, 0.0, 0.0, 0.0, 0.0,
                                                                                0.0, 0.0, 0.0, 0.0, 0.2411,
                                                                                0.2411, 0.2221, 0.2221, -0.1899, -0.1899,
                                                                                -0.1509, -0.1509, 0.0451, 0.0451, -0.1709,
                                                                                -0.1709, 0.2221, 0.2221, -0.1899, -0.1899,
                                                                                -0.1359, -0.1359, -0.1359, -0.1359, 0.2221,
                                                                                0.2221, -0.2099, -0.2099, 0.2531, 0.2531,
                                                                                -0.1359, -0.1359, -0.2099, -0.2099, 0.2411,
                                                                                0.2411, 0.2531, 0.2531, -0.1709, -0.1709,
                                                                                0.0, 0.0, 0.0, 0.0, 0.0,
                                                                                0.0, 0.0, 0.0, 0.0),
                                  BeamHaloPlus = cms.untracked.bool(True),
                                  hitCollectionEE = cms.string('EcalUncalibRecHitsEE'),
                                  ChPeakTime = cms.untracked.string('ChPeakTimeLaser.txt'),
                                  hitCollection = cms.string('EcalUncalibRecHitsEB'),
                                  digiProducer = cms.string('ecalEBunpacker'),
                                  CorrectEcalReadout = cms.untracked.bool(False),
                                  FromFile = cms.untracked.bool(False),
                                  RunStart = cms.untracked.double(1215192037.0),
                                  RunLength = cms.untracked.double(2.0),
                                  EBRadius = cms.untracked.double(1.4)
                              )

process.p = cms.Path(process.ecalEBunpacker*process.ecalDccDigis*process.uncalibHitMaker*process.timing)

