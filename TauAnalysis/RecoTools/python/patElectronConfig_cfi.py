import FWCore.ParameterSet.Config as cms
import copy

from PhysicsTools.PatAlgos.recoLayer0.electronId_cff import *
from PhysicsTools.PatAlgos.recoLayer0.electronIsolation_cff import *
from PhysicsTools.PatAlgos.recoLayer0.aodReco_cff import *
from PhysicsTools.PatAlgos.triggerLayer0.trigMatchSequences_cff import *
from PhysicsTools.PatAlgos.producersLayer1.electronProducer_cfi import *
from PhysicsTools.PatAlgos.cleaningLayer1.electronCleaner_cfi import *

#--------------------------------------------------------------------------------  
# PAT layer 0 electron configuration parameters
#--------------------------------------------------------------------------------

# add HLT electron trigger to PAT trigger match sequence
patTrigMatchElectron = cms.Sequence( electronTrigMatchHLT1Electron )
patTrigMatch._seq = patTrigMatch._seq * patHLT1Electron * patTrigMatchElectron

#--------------------------------------------------------------------------------  
# PAT layer 1 electron configuration parameters
#--------------------------------------------------------------------------------

#
# Track isolation
#
allLayer1Electrons.isolation.tracker.src = cms.InputTag("eleIsoDepositTk")
allLayer1Electrons.isolation.tracker.deltaR = cms.double(0.6)
allLayer1Electrons.isolation.tracker.vetos = cms.vstring(
    '0.015',         # inner radius veto cone (was 0.015)
    'Threshold(0.3)' # threshold on individual track pt
)
allLayer1Electrons.isolation.tracker.skipDefaultVeto = cms.bool(True)
#
# ECAL isolation
#
allLayer1Electrons.isolation.ecal.src = cms.InputTag("eleIsoDepositEcalFromHits")
allLayer1Electrons.isolation.ecal.deltaR = cms.double(0.6)
allLayer1Electrons.isolation.ecal.vetos = cms.vstring(
    'EcalBarrel:0.045', 
    'EcalBarrel:RectangularEtaPhiVeto(-0.02,0.02,-0.5,0.5)',
    'EcalEndcaps:0.1',                         #0.07
    'EcalEndcaps:RectangularEtaPhiVeto(-0.05,0.05,-0.5,0.5)',
    'EcalBarrel:ThresholdFromTransverse(0.12)', #0.08
    'EcalEndcaps:ThresholdFromTransverse(0.3)'
)
allLayer1Electrons.isolation.ecal.skipDefaultVeto = cms.bool(True)
#
# HCAL isolation
#
allLayer1Electrons.isolation.hcal.src = cms.InputTag("eleIsoDepositHcalFromTowers")
allLayer1Electrons.isolation.hcal.deltaR = cms.double(0.6)
#
# add IsoDeposit objects for Track, ECAL and HCAL based isolation
#
allLayer1Electrons.isoDeposits = cms.PSet(
   tracker         = allLayer1Electrons.isolation.tracker.src,
   ecal            = allLayer1Electrons.isolation.ecal.src,
   hcal            = allLayer1Electrons.isolation.hcal.src,
   particle        = cms.InputTag("pfeleIsoDepositPFCandidates"),
   chargedparticle = cms.InputTag("pfeleIsoChDepositPFCandidates"),
   neutralparticle = cms.InputTag("pfeleIsoNeDepositPFCandidates"),
   gammaparticle   = cms.InputTag("pfeleIsoGaDepositPFCandidates")
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

# do not remove electrons overlapping with muons
# (instead, leave this removal to the subsequent selector stage)
cleanLayer1Electrons.checkOverlaps = cms.PSet()
