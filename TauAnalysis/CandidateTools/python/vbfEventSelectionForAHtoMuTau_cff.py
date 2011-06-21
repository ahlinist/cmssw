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
    cut = cms.string('dEta > 3.5'),
    filter = cms.bool(False)
)

selectedVBFEventHypothesesForAHtoMuTauTagJetMass350 = cms.EDFilter("PATMuTauPairVBFEventSelector",
    cut = cms.string('mjj > 350'),
    filter = cms.bool(False)
)

vbfEventHypothesesSelConfiguratorForAHtoMuTau = objSelConfigurator(
    [ selectedVBFEventHypothesesForAHtoMuTauTagJetEt30,
      selectedVBFEventHypothesesForAHtoMuTauTagJetOpposHemisphere,
      selectedVBFEventHypothesesForAHtoMuTauTagJetDEta35,
      selectedVBFEventHypothesesForAHtoMuTauTagJetMass350 ],
    src = "allVBFEventHypothesesForAHtoMuTau",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectVBFEventHypothesesForAHtoMuTau = vbfEventHypothesesSelConfiguratorForAHtoMuTau.configure(pyNameSpace = locals())

# define additional collections of VBF event hypotheses for muon + tau-jet candidates
# with loose track and ECAL isolation applied on muon leg
# (NOTE: to be used for the purpose of factorizing efficiencies
#        of muon isolation from other event selection criteria,
#        in order to avoid problems with limited Monte Carlo statistics)

selectedVBFEventHypothesesForAHtoMuTauTagJetEt30LooseMuonIsolation = \
  copy.deepcopy(selectedVBFEventHypothesesForAHtoMuTauTagJetEt30)

selectedVBFEventHypothesesForAHtoMuTauTagJetOpposHemisphereLooseMuonIsolation = \
  copy.deepcopy( selectedVBFEventHypothesesForAHtoMuTauTagJetOpposHemisphere)

selectedVBFEventHypothesesForAHtoMuTauTagJetDEta35LooseMuonIsolation = \
  copy.deepcopy(selectedVBFEventHypothesesForAHtoMuTauTagJetDEta35)

selectedVBFEventHypothesesForAHtoMuTauTagJetMass350LooseMuonIsolation = \
  copy.deepcopy( selectedVBFEventHypothesesForAHtoMuTauTagJetMass350)

vbfEventHypothesesSelConfiguratorForAHtoMuTauLooseMuonIsolation = objSelConfigurator(
    [ selectedVBFEventHypothesesForAHtoMuTauTagJetEt30LooseMuonIsolation,
      selectedVBFEventHypothesesForAHtoMuTauTagJetOpposHemisphereLooseMuonIsolation,
      selectedVBFEventHypothesesForAHtoMuTauTagJetDEta35LooseMuonIsolation,
      selectedVBFEventHypothesesForAHtoMuTauTagJetMass350LooseMuonIsolation ],
    src = "allVBFEventHypothesesForAHtoMuTauLooseMuonIsolation",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectVBFEventHypothesesForAHtoMuTauLooseMuonIsolation = \
  vbfEventHypothesesSelConfiguratorForAHtoMuTauLooseMuonIsolation.configure(pyNameSpace = locals())
