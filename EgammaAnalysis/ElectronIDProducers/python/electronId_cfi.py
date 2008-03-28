import FWCore.ParameterSet.Config as cms

from EgammaAnalysis.ElectronIDProducers.cutBasedElectronId_cfi import *
from EgammaAnalysis.ElectronIDProducers.ptdrElectronId_cfi import *
# --- ESSource for Electron ID likelihood ---
from EgammaAnalysis.ElectronIDESSources.likelihoodPdfsDB_cfi import *
from EgammaAnalysis.ElectronIDESSources.likelihoodESetup_cfi import *
# --- Neural Net Electron ID block---
#include "EgammaAnalysis/ElectronIDProducers/data/neuralNetElectronId.cfi" 
electronId = cms.EDProducer("ElectronIDProducer",
    electronLabel = cms.string(''),
    doNeuralNet = cms.bool(False),
    doLikelihood = cms.bool(False),
    endcapClusterShapeAssociation = cms.InputTag("islandBasicClusters","islandEndcapShapeAssoc"),
    doPtdrId = cms.bool(True),
    doCutBased = cms.bool(False),
    electronIDAssociationLabel = cms.string(''),
    electronProducer = cms.string('pixelMatchGsfElectrons'),
    electronIDLabel = cms.string(''),
    barrelClusterShapeAssociation = cms.InputTag("hybridSuperClusters","hybridShapeAssoc"),
    algo_psets = cms.VPSet(cms.PSet(
        PTDR_ID
    ), cms.PSet(
        CutBased_ID
    ))
)


