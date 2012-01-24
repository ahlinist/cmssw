import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.CandidateTools.tools.objProdConfigurator import *

#--------------------------------------------------------------------------------
# produce VBF event hypotheses
#--------------------------------------------------------------------------------

allVBFEventHypothesesForAHtoElecTau = cms.EDProducer("PATElecTauPairVBFEventProducer",
    srcTagJets          = cms.InputTag('selectedPatJetsForAHtoElecTauJetTagCumulative'),
    srcCentralJets      = cms.InputTag('selectedPatJetsForAHtoElecTauJetTagCumulative'),
                                       
    srcDiTaus           = cms.InputTag('selectedElecTauPairsPzetaDiffCumulative'),
                                           
    dEtaMinTagJet       = cms.double(2.5),
    massMinTagJet       = cms.double(250.),
    dRmin12TagJet       = cms.double(0.5),

    etaMarginCentralJet = cms.double(0.),
    dRmin12CentralJet   = cms.double(0.5)
)

vbfEventProdConfiguratorForAHtoElecTau = objProdConfigurator(
    allVBFEventHypothesesForAHtoElecTau,
    pyModuleName = __name__
)

produceVBFEventHypothesesForAHtoElecTau = \
  vbfEventProdConfiguratorForAHtoElecTau.configure(pyNameSpace = locals())
