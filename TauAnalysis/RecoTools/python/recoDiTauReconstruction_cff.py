import FWCore.ParameterSet.Config as cms

from TauAnalysis.CandidateTools.diCandidatePairProducer_cfi import diTauProducer
recoDiTau = diTauProducer.clone()
#Be careful in standard PAT_v2 'selectedLayer1' collections are not cleaned
recoDiTau.srcLeg1 = 'cleanLayer1Muons'
recoDiTau.srcLeg2 = 'cleanLayer1Taus'
#Be careful in standard PAT_v2 MET collection is called 'layer1METs'
recoDiTau.srcMET = 'layer1METs'

# PAT filters
from PhysicsTools.PatAlgos.selectionLayer1.tauCountFilter_cfi import countLayer1Taus
recoAtLeastOneTau = countLayer1Taus.clone()
recoAtLeastOneTau.minNumber = 1
recoAtLeastOneTau.maxNumber = 999999 #not changed, only for information
recoAtLeastOneTau.src = recoDiTau.srcLeg2 #same collection as for 'recoDiTau'

from PhysicsTools.PatAlgos.selectionLayer1.muonCountFilter_cfi import countLayer1Muons
recoAtLeastOneMuon = countLayer1Muons.clone()
recoAtLeastOneMuon.minNumber = 1
recoAtLeastOneMuon.maxNumber = 999999 #not changed, only for information
recoAtLeastOneMuon.src = recoDiTau.srcLeg1 #same collection as for 'recoDiTau'


recoDiTauReconstruction = cms.Sequence(
    recoAtLeastOneTau + 
    recoAtLeastOneMuon +
    recoDiTau
    )
