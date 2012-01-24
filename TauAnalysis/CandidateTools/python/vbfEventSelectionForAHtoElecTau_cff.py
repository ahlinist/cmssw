import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.CandidateTools.tools.objSelConfigurator import *

#--------------------------------------------------------------------------------  
# produce collections of VBF event hypotheses passing selection criteria
# specific to VBF Higgs --> tau+ tau- --> electron + tau-jet analysis
#--------------------------------------------------------------------------------

selectedVBFEventHypothesesForAHtoElecTauTagJetEt30 = cms.EDFilter("PATElecTauPairVBFEventSelector",
    cut = cms.string('tagJet1.et > 30. & tagJet2.et > 30.'),
    filter = cms.bool(False)
)

selectedVBFEventHypothesesForAHtoElecTauTagJetOpposHemisphere = cms.EDFilter("PATElecTauPairVBFEventSelector",
    cut = cms.string('(tagJet1.eta*tagJet2.eta) < 0.'),
    filter = cms.bool(False)
)

selectedVBFEventHypothesesForAHtoElecTauTagJetDEta35 = cms.EDFilter("PATElecTauPairVBFEventSelector",
    cut = cms.string('dEta > 4.0'),
    filter = cms.bool(False)
)

selectedVBFEventHypothesesForAHtoElecTauTagJetMass350 = cms.EDFilter("PATElecTauPairVBFEventSelector",
    cut = cms.string('mjj > 400'),
    filter = cms.bool(False)
)

selectedVBFEventHypothesesForAHtoElecTau3rdJetVeto = cms.EDFilter("PATElecTauPairVBFEventSelector",
    cut = cms.string('centralJets.size < 1'),
    filter = cms.bool(False)
)

vbfEventHypothesesSelConfiguratorForAHtoElecTau = objSelConfigurator(
    [ selectedVBFEventHypothesesForAHtoElecTauTagJetEt30,
      selectedVBFEventHypothesesForAHtoElecTauTagJetOpposHemisphere,
      selectedVBFEventHypothesesForAHtoElecTauTagJetDEta35,
      selectedVBFEventHypothesesForAHtoElecTauTagJetMass350,
      selectedVBFEventHypothesesForAHtoElecTau3rdJetVeto ],
    src = "allVBFEventHypothesesForAHtoElecTau",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectVBFEventHypothesesForAHtoElecTau = vbfEventHypothesesSelConfiguratorForAHtoElecTau.configure(pyNameSpace = locals())
