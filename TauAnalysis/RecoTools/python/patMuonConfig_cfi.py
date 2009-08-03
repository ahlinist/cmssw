import FWCore.ParameterSet.Config as cms

#MB from PhysicsTools.PatAlgos.recoLayer0.muonIsolation_cff import *
#MB trigger stuff has been redone with 31X, so it is commented out. It should modified to follow PAT modification
#MB from PhysicsTools.PatAlgos.triggerLayer0.trigMatchSequences_cff import *
from PhysicsTools.PatAlgos.producersLayer1.muonProducer_cfi import *
from PhysicsTools.PatAlgos.cleaningLayer1.muonCleaner_cfi import *

#--------------------------------------------------------------------------------  
# PAT layer 0 muon configuration parameters
#--------------------------------------------------------------------------------

# add isolated HLT muon trigger to PAT trigger match sequence
#MB trigger stuff has been redone with 31X, so it is commented out. It should modified to follow PAT modification
#MB patTrigMatch._seq = patTrigMatch._seq * patTrigMatchHLT1MuonIso

#--------------------------------------------------------------------------------  
# PAT layer 1 muon configuration parameters
#--------------------------------------------------------------------------------

# increase size of muon isolation cone from default of deltaR = 0.3 to 0.6
allLayer1Muons.isolation.tracker.deltaR = cms.double(0.6)

allLayer1Muons.isolation.ecal.deltaR = cms.double(0.6)

allLayer1Muons.isolation.hcal.deltaR = cms.double(0.6)

allLayer1Muons.isolation.user.deltaR = cms.double(0.6)
#
# add IsoDeposit objects for Track, ECAL and HCAL based isolation
#
allLayer1Muons.isoDeposits = cms.PSet(
   tracker         = allLayer1Muons.isolation.tracker.src,
   ecal            = allLayer1Muons.isolation.ecal.src,
   hcal            = allLayer1Muons.isolation.hcal.src,
   particle        = cms.InputTag("pfmuIsoDepositPFCandidates"),
   chargedparticle = cms.InputTag("pfmuIsoChDepositPFCandidates"),
   neutralparticle = cms.InputTag("pfmuIsoNeDepositPFCandidates"),
   gammaparticle   = cms.InputTag("pfmuIsoGaDepositPFCandidates")
)
#
# enable matching to HLT trigger information;
# match offline reconstructed muons to isolated and non-isolated HLT muon paths
#
#MB allLayer1Muons.addTrigMatch = cms.bool(True)
allLayer1Muons.addTrigMatch = cms.bool(False)
#MB allLayer1Muons.trigPrimMatch = cms.VInputTag(
#MB    cms.InputTag("muonTrigMatchHLT1MuonNonIso"),
#MB    cms.InputTag("muonTrigMatchHLT1MuonIso")
#MB )
#
# enable matching to generator level information
#
allLayer1Muons.addGenMatch = cms.bool(True)
