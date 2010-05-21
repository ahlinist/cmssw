import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# produce combinations of electron + tau-jet pairs
#--------------------------------------------------------------------------------

allElecTauPairs = cms.EDProducer("PATElecTauPairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('selectedPatElectronsForElecTauConversionVetoCumulative'),
    srcLeg2 = cms.InputTag('selectedPatTausForElecTauMuonVetoCumulative'),
    dRmin12 = cms.double(0.3),
    srcMET = cms.InputTag('patMETs'),
    srcPrimaryVertex = cms.InputTag("offlinePrimaryVerticesWithBS"),
	#    srcBeamSpot = cms.InputTag("offlineBeamSpot"),                             
    srcGenParticles = cms.InputTag('genParticles'),                                   
    recoMode = cms.string(""),
    scaleFuncImprovedCollinearApprox = cms.string('1'),                             
    verbosity = cms.untracked.int32(0)
)

produceElecTauPairs = cms.Sequence( allElecTauPairs )

# define additional collections of electron + tau-jet candidates
# with loose track and ECAL isolation applied on electron leg
# (NOTE: to be used for the purpose of factorizing efficiencies
#        of electron isolation from other event selection criteria,
#        in order to avoid problems with limited Monte Carlo statistics)

allElecTauPairsLooseElectronIsolation = allElecTauPairs.clone()
allElecTauPairsLooseElectronIsolation.srcLeg1 = cms.InputTag('selectedPatElectronsForElecTauConversionVetoLooseIsolationCumulative')

produceElecTauPairsLooseElectronIsolation = cms.Sequence( allElecTauPairsLooseElectronIsolation )

produceElecTauPairsAll = cms.Sequence( produceElecTauPairs * produceElecTauPairsLooseElectronIsolation )
