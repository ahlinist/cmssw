import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# compute particle flow based IsoDeposits
# names and settings taken from 22X, to be tuned
#--------------------------------------------------------------------------------

from PhysicsTools.PFCandProducer.Isolation.pfMuonIsolation_cff import *
muonCollection = "muons"
pfmuIsoDepositPFCandidates = isoDepositReplace(muonCollection,"particleFlow")
pfmuIsoChDepositPFCandidates = isoDepositReplace(muonCollection,"pfAllChargedHadrons")
pfmuIsoNeDepositPFCandidates = isoDepositReplace(muonCollection,"pfAllNeutralHadrons")
pfmuIsoGaDepositPFCandidates = isoDepositReplace(muonCollection,"pfAllPhotons")


pfMuonIsolCandidates = cms.Sequence( pfmuIsoDepositPFCandidates
                                     * pfmuIsoChDepositPFCandidates
                                     * pfmuIsoNeDepositPFCandidates
                                     * pfmuIsoGaDepositPFCandidates )

pfMuIsoDeposit = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(
        cms.PSet(
            src = cms.InputTag("pfmuIsoDepositPFCandidates"),
            deltaR = cms.double(0.3),
            weight = cms.string('1'),
            vetos = cms.vstring('0.01','Threshold(0.5)'),
            skipDefaultVeto = cms.bool(True),
            mode = cms.string('sum')
        )
    )
)
pfMuIsoChDeposit = pfMuIsoDeposit.clone()
pfMuIsoChDeposit.deposits.src = 'pfmuIsoChDepositPFCandidates'
pfMuIsoNeDeposit = pfMuIsoDeposit.clone()
pfMuIsoNeDeposit.deposits.src = 'pfmuIsoNeDepositPFCandidates'
pfMuIsoGaDeposit = pfMuIsoDeposit.clone()
pfMuIsoGaDeposit.deposits.src = 'pfmuIsoGaDepositPFCandidates'

muonIsoDeposits =  cms.Sequence( pfMuIsoDeposit
                                 * pfMuIsoChDeposit
                                 * pfMuIsoNeDeposit
                                 * pfMuIsoGaDeposit )

recoMuonIsolation = cms.Sequence( pfMuonIsolCandidates
                                  * muonIsoDeposits )
