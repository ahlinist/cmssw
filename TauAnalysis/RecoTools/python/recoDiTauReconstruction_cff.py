import FWCore.ParameterSet.Config as cms

from TauAnalysis.CandidateTools.diCandidatePairProducer_cfi import diTauProducer
recoDiTau = diTauProducer.clone()
recoDiTau.srcLeg1 = 'selectedLayer1Muons'
recoDiTau.srcLeg2 = 'selectedLayer1Taus'
recoDiTau.srcMET = 'selectedLayer1METs'

# PAT filters
from PhysicsTools.PatAlgos.selectionLayer1.tauMinFilter_cfi import minLayer1Taus
recoAtLeastOneTau = minLayer1Taus.clone()
recoAtLeastOneTau.minNumber = 1

from PhysicsTools.PatAlgos.selectionLayer1.muonMinFilter_cfi import minLayer1Muons
recoAtLeastOneMuon = minLayer1Muons.clone()
recoAtLeastOneMuon.minNumber = 1

recoDiTauReconstruction = cms.Sequence(
    recoAtLeastOneTau + 
    recoAtLeastOneMuon +
    recoDiTau
    )
