import FWCore.ParameterSet.Config as cms

from TauAnalysis.CandidateTools.diCandidatePairProducer_cfi import diTauProducer
recoDiTau = diTauProducer.clone()
#Be careful in standard PAT_v2 'selectedLayer1' collections are not cleaned
recoDiTau.srcLeg1 = 'selectedPatMuons'
recoDiTau.srcLeg2 = 'selectedPatTaus'
#Be careful in standard PAT_v2 MET collection is called 'layer1METs'
recoDiTau.srcMET = 'layer1METs'

# PAT filters
from PhysicsTools.PatAlgos.selectionLayer1.tauCountFilter_cfi import countPatTaus
recoAtLeastOneTau = countPatTaus.clone()
recoAtLeastOneTau.minNumber = 1
recoAtLeastOneTau.maxNumber = 999999 #not changed, only for information
recoAtLeastOneTau.src = recoDiTau.srcLeg2 #same collection as for 'recoDiTau'

from PhysicsTools.PatAlgos.selectionLayer1.muonCountFilter_cfi import countPatMuons
recoAtLeastOneMuon = countPatMuons.clone()
recoAtLeastOneMuon.minNumber = 1
recoAtLeastOneMuon.maxNumber = 999999 #not changed, only for information
recoAtLeastOneMuon.src = recoDiTau.srcLeg1 #same collection as for 'recoDiTau'

from PhysicsTools.PatAlgos.selectionLayer1.electronCountFilter_cfi import countPatElectrons
recoAtLeastOneElectron = countPatElectrons.clone()
recoAtLeastOneElectron.minNumber = 1
recoAtLeastOneElectron.maxNumber = 999999 #not changed, only for information
#recoAtLeastOneElectron.src = recoDiTau.srcLeg1 #same collection as for 'recoDiTau'

# DiTau filter
from TauAnalysis.CandidateTools.candidateCountFilter_cfi import candidateCountFilter
recoAtLeastOneDiTau = candidateCountFilter.clone()
recoAtLeastOneDiTau.minNumber = 1
recoAtLeastOneDiTau.src = 'recoDiTau'

recoDiTauReconstruction = cms.Sequence(
    #recoAtLeastOneTau + 
    #recoAtLeastOneMuon +
    recoDiTau
    + recoAtLeastOneDiTau
    )
