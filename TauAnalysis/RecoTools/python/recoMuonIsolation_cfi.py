import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# compute particle flow based IsoDeposits
#--------------------------------------------------------------------------------

from PhysicsTools.PFCandProducer.pfMuonIsolation_cff import *
pfmuIsoDepositPFCandidates.src = cms.InputTag("muons")
pfmuIsoChDepositPFCandidates.src = pfmuIsoDepositPFCandidates.src
pfmuIsoNeDepositPFCandidates.src = pfmuIsoDepositPFCandidates.src
pfmuIsoGaDepositPFCandidates.src = pfmuIsoDepositPFCandidates.src
pfMuons.src = pfmuIsoDepositPFCandidates.src

pfMuonIsol = cms.Sequence( pfmuIsoDepositPFCandidates
                          * pfmuIsoChDepositPFCandidates
                          * pfmuIsoNeDepositPFCandidates
                          * pfmuIsoGaDepositPFCandidates )

recoMuonIsolation = cms.Sequence( pfMuonIsol )
