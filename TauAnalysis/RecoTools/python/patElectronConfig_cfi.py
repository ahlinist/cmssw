import FWCore.ParameterSet.Config as cms
import copy

from PhysicsTools.PatAlgos.recoLayer0.electronId_cff import *
from PhysicsTools.PatAlgos.recoLayer0.electronIsolation_cff import *
from PhysicsTools.PatAlgos.cleaningLayer0.electronCleaner_cfi import *
from PhysicsTools.PatAlgos.triggerLayer0.trigMatchSequences_cff import *
from PhysicsTools.PatAlgos.producersLayer1.electronProducer_cfi import *

#--------------------------------------------------------------------------------  
# PAT layer 0 electron configuration parameters
#--------------------------------------------------------------------------------

# add HLT electron trigger to PAT trigger match sequence
patTrigMatchElectron = cms.Sequence( electronTrigMatchHLT1Electron )
patTrigMatch._seq = patTrigMatch._seq * patHLT1Electron * patTrigMatchElectron

# compite ECAL isolation from individual crystals,
# not from clusters
patAODElectronIsolationLabels = cms.VInputTag(
    cms.InputTag("eleIsoDepositTk"),
    cms.InputTag("eleIsoDepositEcalFromHits"),
    cms.InputTag("eleIsoDepositHcalFromTowers")
)

patAODElectronIsolations.associations = patAODElectronIsolationLabels

layer0ElectronIsolations.associations = patAODElectronIsolationLabels

allLayer0Electrons.isolation = cms.PSet()
allLayer0Electrons.bitsToIgnore = cms.vstring('')

patElectronIds.associations = cms.VInputTag(
    cms.InputTag("elecIdCutBasedRobust"),
    cms.InputTag("elecIdCutBasedLoose"),
    cms.InputTag("elecIdCutBasedTight")
)

#--------------------------------------------------------------------------------  
# PAT layer 1 electron configuration parameters
#--------------------------------------------------------------------------------

# switch input of PAT layer 1 electron reconstruction to output of PAT layer 1 cleaning
# CV: for some reason, the default is '', which does not seem to exist in the event ?
allLayer1Electrons.electronSource = cms.InputTag("allLayer0Electrons")

#
# Track isolation
#
allLayer1Electrons.isolation.tracker.src = cms.InputTag("layer0ElectronIsolations", "eleIsoDepositTk")
allLayer1Electrons.isolation.tracker.deltaR = cms.double(0.6)
allLayer1Electrons.isolation.tracker.vetos = cms.vstring(
    '0.015',         # inner radius veto cone (was 0.015)
    'Threshold(0.3)' # threshold on individual track pt
)
allLayer1Electrons.isolation.tracker.skipDefaultVeto = cms.bool(True)
#
# ECAL isolation
#
allLayer1Electrons.isolation.ecal.src = cms.InputTag("layer0ElectronIsolations", "eleIsoDepositEcalFromHits")
allLayer1Electrons.isolation.ecal.deltaR = cms.double(0.6)
allLayer1Electrons.isolation.ecal.vetos = cms.vstring(
    'EcalBarrel:0.045', 
    'EcalBarrel:RectangularEtaPhiVeto(-0.02,0.02,-0.5,0.5)',
    'EcalEndcaps:0.01',                         #0.07
    'EcalEndcaps:RectangularEtaPhiVeto(-0.05,0.05,-0.5,0.5)',
    'EcalBarrel:ThresholdFromTransverse(0.12)', #0.08
    'EcalEndcaps:ThresholdFromTransverse(0.3)'
)
allLayer1Electrons.isolation.ecal.skipDefaultVeto = cms.bool(True)
#
# HCAL isolation
#
allLayer1Electrons.isolation.hcal.src = cms.InputTag("layer0ElectronIsolations", "eleIsoDepositHcalFromTowers")
allLayer1Electrons.isolation.hcal.deltaR = cms.double(0.6)
#
# add IsoDeposit objects for Track, ECAL and HCAL based isolation
#
allLayer1Electrons.isoDeposits = cms.PSet(
   tracker = allLayer1Electrons.isolation.tracker.src,
   ecal    = allLayer1Electrons.isolation.ecal.src,
   hcal    = allLayer1Electrons.isolation.hcal.src
)
#
# add electron Id. flags
#
allLayer1Electrons.addElectronID = cms.bool(True)
allLayer1Electrons.electronIDSources = cms.PSet(
   robust = cms.InputTag("elecIdCutBasedRobust"),
   loose  = cms.InputTag("elecIdCutBasedLoose"),
   tight  = cms.InputTag("elecIdCutBasedTight")        
)
#
# enable matching to HLT trigger information
#
allLayer1Electrons.addTrigMatch = cms.bool(True)
allLayer1Electrons.trigPrimMatch = cms.VInputTag(
    cms.InputTag("electronTrigMatchHLT1Electron")
)
#
# enable matching to generator level information
#
allLayer1Electrons.addGenMatch = cms.bool(True)

allLayer1Electrons.genParticleMatch = cms.InputTag("electronMatch")

