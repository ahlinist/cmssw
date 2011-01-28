import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.RecoTools.elecTauPairZeeHypothesis_cff import *

#--------------------------------------------------------------------------------
# produce data-formats providing information about compatibility of electron + tau-jet pairs
# with hypothesis of being a pair of electrons resulting from a Z --> e+ e- decay
#--------------------------------------------------------------------------------

elecTauPairZeeHypothesesForAHtoElecTau = elecTauPairZeeHypotheses.clone(
    diCandidatePairSource = cms.InputTag('selectedElecTauPairsForAHtoElecTauPzetaDiffCumulative')
)

elecTauPairVisMassHypothesesForAHtoElecTau = elecTauPairVisMassHypotheses.clone(
    diCandidatePairSource = elecTauPairZeeHypothesesForAHtoElecTau.diCandidatePairSource
)
elecTauPairVisMassHypothesesForAHtoElecTau.ZllHypotheses[0].src = \
  cms.InputTag('elecTauPairZeeHypothesesForAHtoElecTau')

produceElecTauPairZeeHypothesesForAHtoElecTau = cms.Sequence(
   elecTauPairZeeHypothesesForAHtoElecTau
  * elecTauPairVisMassHypothesesForAHtoElecTau
)

elecTauPairZeeHypothesesForAHtoElecTauLooseElectronIsolation = elecTauPairZeeHypothesesForAHtoElecTau.clone(
    diCandidatePairSource = cms.InputTag('selectedElecTauPairsForAHtoElecTauPzetaDiffLooseElectronIsolationCumulative')
)

elecTauPairVisMassHypothesesForAHtoElecTauLooseElectronIsolation = elecTauPairVisMassHypothesesForAHtoElecTau.clone(
    diCandidatePairSource = elecTauPairZeeHypothesesForAHtoElecTauLooseElectronIsolation.diCandidatePairSource
)
elecTauPairVisMassHypothesesForAHtoElecTauLooseElectronIsolation.ZllHypotheses[0].src = \
  cms.InputTag('elecTauPairZeeHypothesesForAHtoElecTauLooseElectronIsolation')

produceElecTauPairZeeHypothesesForAHtoElecTauLooseElectronIsolation = cms.Sequence(
   elecTauPairZeeHypothesesForAHtoElecTauLooseElectronIsolation
  * elecTauPairVisMassHypothesesForAHtoElecTauLooseElectronIsolation
)
