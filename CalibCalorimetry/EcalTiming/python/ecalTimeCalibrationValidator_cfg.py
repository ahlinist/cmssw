import FWCore.ParameterSet.Config as cms

process = cms.Process("EcalTimeCalibrationValidator")

# shaping our Message logger to suit our needs
process.MessageLogger = cms.Service("MessageLogger",
    #suppressWarning = cms.untracked.vstring('ecalEBunpacker', 'ecalUncalibHit', 'ecalRecHit', 'ecalCosmicsHists'),
    #suppressInfo = cms.untracked.vstring('ecalEBunpacker', 'ecalUncalibHit', 'ecalRecHit', 'ecalCosmicsHists'),
    cout = cms.untracked.PSet(threshold = cms.untracked.string('ERROR')),
    categories = cms.untracked.vstring('ecalDigis','ecalDccDigis','uncalibHitMaker','ecalDetIdToBeRecovered','ecalRecHit'),
    destinations = cms.untracked.vstring('cout')
)


# Trivial Conds
from CalibCalorimetry.EcalTrivialCondModules.EcalTrivialCondRetriever_cfi import *
process.myCond = EcalTrivialConditionRetriever.clone()
process.es_prefer = cms.ESPrefer("EcalTrivialConditionRetriever","myCond")
process.myCond.timeCalibErrorsFile = cms.untracked.string('timingCalibs2.calibs.test.txt')
process.myCond.timeCalibConstantsFile = cms.untracked.string('timingCalibs2.calibs.test.txt')
process.myCond.producedEcalTimeCalibConstants = True
process.myCond.producedEcalTimeCalibErrors = True
#process.myCond.producedEcalTimeCalibConstants = False
#process.myCond.producedEcalTimeCalibErrors = False

process.myCond.producedEcalChannelStatus = False
process.myCond.weightsForTB = False
process.myCond.producedEcalPedestals = False
process.myCond.getWeightsFromFile = False
process.myCond.producedEcalWeights = False
process.myCond.producedEcalIntercalibConstants = False
process.myCond.producedEcalIntercalibConstantsMC = False
process.myCond.producedEcalIntercalibErrors = False
process.myCond.producedEcalLaserCorrection = False
process.myCond.producedEcalGainRatios = False
process.myCond.producedEcalADCToGeVConstant = False
process.myCond.producedEcalClusterLocalContCorrParameters = False
process.myCond.producedEcalClusterCrackCorrParameters = False
process.myCond.producedEcalClusterEnergyCorrectionParameters = cms.untracked.bool(False)
process.myCond.producedEcalClusterEnergyUncertaintyParameters = cms.untracked.bool(False)
process.myCond.producedEcalMappingElectronics = False
process.myCond.producedEnergyUncertaintyParameters = False
process.myCond.produceEnergyCorrectionParameters = False

process.load("CalibCalorimetry.EcalLaserCorrection.ecalLaserCorrectionService_cfi")

process.expressValidator = cms.EDAnalyzer("EcalTimeCalibrationValidator",
  InputTreeFileName = cms.string("file:/data2/scooper/data/OutOfTimeBuckets_August2010/cafJobs/fill1089/EcalTimePi0Tree_135149_30_1_la6.root"),
  OutputTreeFileName = cms.string("file:test.tree.root"),
  MaxTreeEntriesToProcess = cms.untracked.int32(100)
)

process.maxEvents = cms.untracked.PSet(  input = cms.untracked.int32(-1) )
process.source = cms.Source("EmptySource")

process.p = cms.Path(process.expressValidator)
