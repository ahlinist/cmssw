import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.RecoTools.muTauPairZmumuHypothesis_cff import *

#--------------------------------------------------------------------------------
# produce data-formats providing information about compatibility of muon + tau-jet pairs
# with hypothesis of being a pair of muons resulting from a Z --> mu+ mu- decay
#--------------------------------------------------------------------------------

muTauPairZmumuHypothesesForAHtoMuTau = muTauPairZmumuHypotheses.clone(
    diCandidatePairSource = cms.InputTag('selectedMuTauPairsForAHtoMuTauCollinearApproxZmassVetoCumulative')
)

muTauPairVisMassHypothesesForAHtoMuTau = muTauPairVisMassHypotheses.clone(
    diCandidatePairSource = muTauPairZmumuHypothesesForAHtoMuTau.diCandidatePairSource
)
muTauPairVisMassHypothesesForAHtoMuTau.ZllHypotheses[0].src = \
  cms.InputTag('muTauPairZmumuHypothesesForAHtoMuTau')

produceMuTauPairZmumuHypothesesForAHtoMuTau = cms.Sequence(
   muTauPairZmumuHypothesesForAHtoMuTau
  * muTauPairVisMassHypothesesForAHtoMuTau
)

muTauPairZmumuHypothesesForAHtoMuTauLooseMuonIsolation = muTauPairZmumuHypothesesForAHtoMuTau.clone(
    diCandidatePairSource = cms.InputTag('selectedMuTauPairsForAHtoMuTauCollinearApproxZmassVetoLooseMuonIsolationCumulative')
)

muTauPairVisMassHypothesesForAHtoMuTauLooseMuonIsolation = muTauPairVisMassHypothesesForAHtoMuTau.clone(
    diCandidatePairSource = muTauPairZmumuHypothesesForAHtoMuTauLooseMuonIsolation.diCandidatePairSource
)
muTauPairVisMassHypothesesForAHtoMuTauLooseMuonIsolation.ZllHypotheses[0].src = \
  cms.InputTag('muTauPairZmumuHypothesesForAHtoMuTauLooseMuonIsolation')

produceMuTauPairZmumuHypothesesForAHtoMuTauLooseMuonIsolation = cms.Sequence(
   muTauPairZmumuHypothesesForAHtoMuTauLooseMuonIsolation
  * muTauPairVisMassHypothesesForAHtoMuTauLooseMuonIsolation
)
