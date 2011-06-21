import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------
# produce VBF event hypotheses
#--------------------------------------------------------------------------------

allVBFEventHypothesesForAHtoMuTau = cms.EDProducer("PATMuTauPairVBFEventProducer",
    srcTagJets          = cms.InputTag('selectedPatTagJetsForVBFEt20'),
    srcCentralJets      = cms.InputTag('selectedPatCentralJetsForVBFEt15'),
                                       
    srcDiTaus           = cms.InputTag('selectedMuTauPairsForAHtoMuTauPzetaDiffCumulative'),
                                           
    dEtaMinTagJet       = cms.double(2.5),
    massMinTagJet       = cms.double(250.),
    dRmin12TagJet       = cms.double(0.5),

    etaMarginCentralJet = cms.double(0.),
    dRmin12CentralJet   = cms.double(0.5)
)

allVBFEventHypothesesForAHtoMuTauLooseMuonIsolation = allVBFEventHypothesesForAHtoMuTau.clone(
    srcDiTaus           = cms.InputTag('selectedMuTauPairsForAHtoMuTauPzetaDiffLooseMuonIsolationCumulative')
)

