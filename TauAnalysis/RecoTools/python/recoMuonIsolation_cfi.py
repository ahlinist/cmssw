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

#--------------------------------------------------------------------------------
# read IsoDeposits and convert to ValueMap<IsoDeposit> keyed to muon candidate
#--------------------------------------------------------------------------------

muonIsolationValueMap = cms.EDFilter("MultipleIsoDepositsToValueMaps",
    collection   = cms.InputTag("muons"),
    associations = cms.VInputTag(
        cms.InputTag("muIsoDepositCalByAssociatorTowers", "ecal"),
        cms.InputTag("muIsoDepositCalByAssociatorTowers", "hcal"),
        cms.InputTag("muIsoDepositCalByAssociatorTowers", "ho"),
        cms.InputTag("muIsoDepositTk"),
        cms.InputTag("muIsoDepositJets"),
        cms.InputTag("pfmuIsoDepositPFCandidates"),
        cms.InputTag("pfmuIsoChDepositPFCandidates"),
        cms.InputTag("pfmuIsoNeDepositPFCandidates"),
        cms.InputTag("pfmuIsoGaDepositPFCandidates")
    )
)

recoMuonIsolation = cms.Sequence( pfMuonIsol * muonIsolationValueMap )
