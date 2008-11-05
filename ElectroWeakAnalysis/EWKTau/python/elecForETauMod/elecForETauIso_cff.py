import FWCore.ParameterSet.Config as cms
import copy
#from RecoEgamma.EgammaIsolationAlgos.eleIsoDeposits_cff import *

from RecoEgamma.EgammaIsolationAlgos.egammaIsoSetup_cff import *
from RecoEgamma.EgammaIsolationAlgos.egammaSuperClusterMerger_cfi import *
from RecoEgamma.EgammaIsolationAlgos.egammaBasicClusterMerger_cfi import *

egammaSuperClusterMerger.src = cms.VInputTag(
    cms.InputTag('hybridSuperClusters'), 
   #cms.InputTag('correctedHybridSuperClusters'), 
    cms.InputTag('multi5x5SuperClusters', 'multi5x5EndcapSuperClusters'),
   #cms.InputTag('multi5x5SuperClustersWithPreshower'),
   #cms.InputTag('correctedMulti5x5SuperClustersWithPreshower')
)
egammaBasicClusterMerger.src = cms.VInputTag(
    cms.InputTag('hybridSuperClusters',  'hybridBarrelBasicClusters'), 
    cms.InputTag('multi5x5BasicClusters','multi5x5EndcapBasicClusters')
)

from RecoEgamma.EgammaIsolationAlgos.eleIsoDepositTk_cff import *
EleIsoTrackExtractorBlock.DR_Max = 1.
from RecoEgamma.EgammaIsolationAlgos.eleEcalExtractorBlocks_cff import *
EleIsoEcalFromHitsExtractorBlock.extRadius = 1.
EleIsoEcalFromClustsExtractorBlock.extRadius = 1.
EleIsoEcalSCVetoFromClustsExtractorBlock.extRadius = 1.
from RecoEgamma.EgammaIsolationAlgos.eleHcalExtractorBlocks_cff import *
EleIsoHcalFromHitsExtractorBlock.extRadius = 1.
EleIsoHcalFromTowersExtractorBlock.extRadius =1.

eleIsoDepositForETauTk = cms.EDProducer("CandIsoDepositProducer",
    src = cms.InputTag("pixelMatchGsfElectrons"),
    trackType = cms.string('candidate'),
    MultipleDepositsFlag = cms.bool(False),
    ExtractorPSet = cms.PSet(EleIsoTrackExtractorBlock)
)

eleIsoDepositForETauEcalSCVetoFromClusts = cms.EDProducer("CandIsoDepositProducer",
    src = cms.InputTag("pixelMatchGsfElectrons"),
    MultipleDepositsFlag = cms.bool(False),
    trackType = cms.string('candidate'),
    ExtractorPSet = cms.PSet( EleIsoEcalSCVetoFromClustsExtractorBlock )
)

eleIsoDepositForETauEcalFromClusts = cms.EDProducer("CandIsoDepositProducer",
    src = cms.InputTag("pixelMatchGsfElectrons"),
    MultipleDepositsFlag = cms.bool(False),
    trackType = cms.string('candidate'),
    ExtractorPSet = cms.PSet( EleIsoEcalFromClustsExtractorBlock )
)

eleIsoDepositForETauEcalFromHits = cms.EDProducer("CandIsoDepositProducer",
    src = cms.InputTag("pixelMatchGsfElectrons"),
    MultipleDepositsFlag = cms.bool(False),
    trackType = cms.string('candidate'),
    ExtractorPSet = cms.PSet( EleIsoEcalFromHitsExtractorBlock )
)

eleIsoDepositForETauHcalFromTowers = cms.EDProducer("CandIsoDepositProducer",
    src = cms.InputTag("pixelMatchGsfElectrons"),
    MultipleDepositsFlag = cms.bool(False),
    trackType = cms.string('candidate'),
    ExtractorPSet = cms.PSet( EleIsoHcalFromTowersExtractorBlock )
)

eleIsoDepositForETauHcalFromHits = cms.EDProducer("CandIsoDepositProducer",
    src = cms.InputTag("pixelMatchGsfElectrons"),
    MultipleDepositsFlag = cms.bool(False),
    trackType = cms.string('candidate'),
    ExtractorPSet = cms.PSet(EleIsoHcalFromHitsExtractorBlock )
)

# define module labels for old (tk-based isodeposit) POG isolation
patAODElectronIsolationLabels = cms.VInputTag(
        cms.InputTag("eleIsoDepositForETauTk"),
        cms.InputTag("eleIsoDepositForETauEcalFromHits"),
        cms.InputTag("eleIsoDepositForETauHcalFromHits"),
        cms.InputTag("eleIsoDepositForETauEcalFromClusts"),       # try these two if you want to compute them from AOD
        cms.InputTag("eleIsoDepositForETauHcalFromTowers"),       # instead of reading the values computed in RECO
        cms.InputTag("eleIsoDepositForETauEcalSCVetoFromClusts"), # somebody might want this one for ECAL instead
)

# read and convert to ValueMap<IsoDeposit> keyed to Candidate
patAODElectronIsolations = cms.EDFilter("MultipleIsoDepositsToValueMaps",
    collection   = cms.InputTag("pixelMatchGsfElectrons"),
    associations = patAODElectronIsolationLabels,
)

# re-key ValueMap<IsoDeposit> to Layer 0 output
layer0ElectronIsolations = cms.EDFilter("CandManyValueMapsSkimmerIsoDeposits",
    collection   = cms.InputTag("allLayer0ElecForETau"),
    backrefs     = cms.InputTag("allLayer0ElecForETau"),
    commonLabel  = cms.InputTag("patAODElectronIsolations"),
    associations = patAODElectronIsolationLabels,
)

# selecting POG modules that can run on top of AOD
eleIsoDepositAOD = cms.Sequence(
    eleIsoDepositForETauTk * eleIsoDepositForETauEcalFromClusts * eleIsoDepositForETauEcalFromHits *
    eleIsoDepositForETauHcalFromTowers * eleIsoDepositForETauHcalFromHits *
    eleIsoDepositForETauEcalSCVetoFromClusts
    )

# sequence to run on AOD before PAT
patAODElectronIsolation = cms.Sequence(
        egammaSuperClusterMerger *  ## 
        egammaBasicClusterMerger *  ## 
        eleIsoDepositAOD *          ## Not needed any more, we use values from RECO
        patAODElectronIsolations)

# sequence to run after the PAT cleaners
patLayer0ElectronIsolation = cms.Sequence(layer0ElectronIsolations)

