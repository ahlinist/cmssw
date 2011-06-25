import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.CandidateTools.tools.objProdConfigurator import *

#--------------------------------------------------------------------------------
# produce VBF event hypotheses
#--------------------------------------------------------------------------------

allVBFEventHypothesesForAHtoMuTau = cms.EDProducer("PATMuTauPairVBFEventProducer",
    srcTagJets          = cms.InputTag('selectedPatTagJetsForVBFEt20Cumulative'),
    srcCentralJets      = cms.InputTag('selectedPatCentralJetsForVBFEt15Cumulative'),
                                       
    srcDiTaus           = cms.InputTag('selectedMuTauPairsForAHtoMuTauPzetaDiffCumulative'),
                                           
    dEtaMinTagJet       = cms.double(2.5),
    massMinTagJet       = cms.double(250.),
    dRmin12TagJet       = cms.double(0.5),

    etaMarginCentralJet = cms.double(0.),
    dRmin12CentralJet   = cms.double(0.5)
)

vbfEventProdConfiguratorForAHtoMuTau = objProdConfigurator(
    allVBFEventHypothesesForAHtoMuTau,
    pyModuleName = __name__
)

produceVBFEventHypothesesForAHtoMuTau = \
  vbfEventProdConfiguratorForAHtoMuTau.configure(pyNameSpace = locals())

allVBFEventHypothesesForAHtoMuTauLooseMuonIsolation = allVBFEventHypothesesForAHtoMuTau.clone(
    srcDiTaus = cms.InputTag('selectedMuTauPairsForAHtoMuTauPzetaDiffLooseMuonIsolationCumulative')
)

vbfEventProdConfiguratorForAHtoMuTauLooseMuonIsolation = objProdConfigurator(
    allVBFEventHypothesesForAHtoMuTauLooseMuonIsolation,
    pyModuleName = __name__
)

produceVBFEventHypothesesForAHtoMuTauLooseMuonIsolation = \
  vbfEventProdConfiguratorForAHtoMuTauLooseMuonIsolation.configure(pyNameSpace = locals())


