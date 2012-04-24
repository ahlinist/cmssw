import FWCore.ParameterSet.Config as cms

from RecoEgamma.PhotonIdentification.isolationCalculator_cfi import *
from RecoEgamma.PhotonIdentification.mipVariable_cfi import *
from RecoEcal.EgammaClusterProducers.hybridSuperClusters_cfi import *
from RecoEcal.EgammaClusterProducers.multi5x5BasicClusters_cfi import *
#
# producer for photons
# $Id: photons_cfi.py,v 1.43 2012/03/23 19:09:26 askew Exp $
#
photons = cms.EDProducer("PhotonProducer",
    photonCoreProducer = cms.InputTag("photonCore"),
 #   photonCollection = cms.string(''),
    regressionWeightsFromDB =   cms.bool(True),                    
    energyRegressionWeightsFileLocation = cms.string('/afs/cern.ch/user/b/bendavid/cmspublic/regweights/gbrph.root'),
    energyRegressionWeightsDBLocation = cms.string('wgbrph'), 
    superClusterEnergyCorrFunction =  cms.string("EcalClusterEnergyCorrection"),                  
    superClusterEnergyErrorFunction = cms.string("EcalClusterEnergyUncertainty"),
    superClusterCrackEnergyCorrFunction =  cms.string("EcalClusterCrackCorrection"),                                       
    photonEcalEnergyCorrFunction = cms.string("EcalClusterEnergyCorrectionObjectSpecific"),             
    #candidateP4type = cms.string("fromRegression"),
    candidateP4type = cms.string("fromEcalEnergy"),                     
    isolationSumsCalculatorSet = cms.PSet(isolationSumsCalculator),
    mipVariableSet = cms.PSet(mipVariable), 
    usePrimaryVertex = cms.bool(True),
    primaryVertexProducer = cms.string('offlinePrimaryVerticesWithBS'),
    posCalc_t0_endcPresh = cms.double(3.6),
    posCalc_logweight = cms.bool(True),
    posCalc_w0 = cms.double(4.2),
    hbheInstance = cms.string(''),
    posCalc_t0_endc = cms.double(6.3),
    barrelEcalHits = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
    hbheModule = cms.string('hbhereco'),
    endcapEcalHits = cms.InputTag("ecalRecHit","EcalRecHitsEE"),
    hcalTowers = cms.InputTag("towerMaker"),
    runMIPTagger = cms.bool(True),
    highEt  = cms.double(100.),                       
    minR9Barrel = cms.double(0.94),
    minR9Endcap = cms.double(0.95),                     
    hOverEConeSize = cms.double(0.15),
    posCalc_x0 = cms.double(0.89),
    posCalc_t0_barl = cms.double(7.7),
    minSCEtBarrel = cms.double(10.0),
    minSCEtEndcap = cms.double(10.0),                     
    maxHoverEBarrel = cms.double(0.5),
    maxHoverEEndcap = cms.double(0.5),
    ecalRecHitSumEtOffsetBarrel = cms.double(999999999),
    ecalRecHitSumEtSlopeBarrel = cms.double(0.),
    ecalRecHitSumEtOffsetEndcap = cms.double(999999999),
    ecalRecHitSumEtSlopeEndcap = cms.double(0.),
    hcalTowerSumEtOffsetBarrel = cms.double(999999999),
    hcalTowerSumEtSlopeBarrel = cms.double(0.),
    hcalTowerSumEtOffsetEndcap = cms.double(999999999),
    hcalTowerSumEtSlopeEndcap = cms.double(0.),                      
    nTrackSolidConeBarrel =cms.double(999999999),
    nTrackSolidConeEndcap =cms.double(999999999),
    nTrackHollowConeBarrel =cms.double(999999999),
    nTrackHollowConeEndcap =cms.double(999999999),
    trackPtSumSolidConeBarrel =cms.double(999999999),
    trackPtSumSolidConeEndcap =cms.double(999999999),
    trackPtSumHollowConeBarrel =cms.double(999999999),
    trackPtSumHollowConeEndcap =cms.double(999999999),
    sigmaIetaIetaCutBarrel=cms.double(999999999),
    sigmaIetaIetaCutEndcap=cms.double(999999999),
    posCalcParameters = cms.PSet( T0_barl      = cms.double(7.4),
                                  T0_endc      = cms.double(6.3),        
                                  T0_endcPresh = cms.double(3.6),
                                  LogWeighted  = cms.bool(True),
                                  W0           = cms.double(4.2),
                                  X0           = cms.double(0.89)
                                 ),                  

    RecHitFlagToBeExcluded = cleanedHybridSuperClusters.RecHitFlagToBeExcluded,
    RecHitSeverityToBeExcluded = cleanedHybridSuperClusters.RecHitSeverityToBeExcluded,


RecHitFlagToBeExcludedEE = multi5x5BasicClustersCleaned.RecHitFlagToBeExcluded,


)


