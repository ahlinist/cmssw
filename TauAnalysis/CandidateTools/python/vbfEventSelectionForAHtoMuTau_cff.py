import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.CandidateTools.tools.objSelConfigurator import *

#--------------------------------------------------------------------------------  
# produce collections of VBF event hypotheses passing selection criteria
# specific to VBF Higgs --> tau+ tau- --> muon + tau-jet analysis
#--------------------------------------------------------------------------------

selectedVBFEventHypothesesForAHtoMuTauTagJetEt30 = cms.EDFilter("PATMuTauPairVBFEventSelector",
    cut = cms.string('tagJet1.et > 30. & tagJet2.et > 30.'),
    filter = cms.bool(False)
)

selectedVBFEventHypothesesForAHtoMuTauTagJetOpposHemisphere = cms.EDFilter("PATMuTauPairVBFEventSelector",
    cut = cms.string('(tagJet1.eta*tagJet2.eta) < 0.'),
    filter = cms.bool(False)
)

selectedVBFEventHypothesesForAHtoMuTauTagJetDEta35 = cms.EDFilter("PATMuTauPairVBFEventSelector",
    cut = cms.string('dEta > 4.0'),
    filter = cms.bool(False)
)

selectedVBFEventHypothesesForAHtoMuTauTagJetMass350 = cms.EDFilter("PATMuTauPairVBFEventSelector",
    cut = cms.string('mjj > 400'),
    filter = cms.bool(False)
)

selectedVBFEventHypothesesForAHtoMuTau3rdJetVeto = cms.EDFilter("PATMuTauPairVBFEventSelector",
    cut = cms.string('centralJets.size < 1'),
    filter = cms.bool(False)
)

vbfEventHypothesesSelConfiguratorForAHtoMuTau = objSelConfigurator(
    [ selectedVBFEventHypothesesForAHtoMuTauTagJetEt30,
      selectedVBFEventHypothesesForAHtoMuTauTagJetOpposHemisphere,
      selectedVBFEventHypothesesForAHtoMuTauTagJetDEta35,
      selectedVBFEventHypothesesForAHtoMuTauTagJetMass350,
      selectedVBFEventHypothesesForAHtoMuTau3rdJetVeto ],
    src = "allVBFEventHypothesesForAHtoMuTau",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectVBFEventHypothesesForAHtoMuTau = vbfEventHypothesesSelConfiguratorForAHtoMuTau.configure(pyNameSpace = locals())

vbfEventHypothesesSelConfiguratorForAHtoMuTauLooseMuonIsolation = objSelConfigurator(
    [ selectedVBFEventHypothesesForAHtoMuTauTagJetEt30,
      selectedVBFEventHypothesesForAHtoMuTauTagJetOpposHemisphere,
      selectedVBFEventHypothesesForAHtoMuTauTagJetDEta35,
      selectedVBFEventHypothesesForAHtoMuTauTagJetMass350,
      selectedVBFEventHypothesesForAHtoMuTau3rdJetVeto ],
    src = "allVBFEventHypothesesForAHtoMuTauLooseMuonIsolation",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectVBFEventHypothesesForAHtoMuTauLooseMuonIsolation = vbfEventHypothesesSelConfiguratorForAHtoMuTauLooseMuonIsolation.configure(pyNameSpace = locals())
