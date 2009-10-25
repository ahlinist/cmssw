import FWCore.ParameterSet.Config as cms

# generic
from TauAnalysis.CandidateTools.diCandidatePairProducer_cfi import diTauProducer
recoZll = diTauProducer.clone()
#Be careful in standard PAT_v2 'selectedLayer1Muons' are not cleaned. 
recoZll.srcLeg1 = 'selectedLayer1Muons'
recoZll.srcLeg2 = 'selectedLayer1Muons'
recoZll.srcMET = ''
recoZll.recoMode = ''
recoZll.useLeadingTausOnly = False

# PAT filters
from PhysicsTools.PatAlgos.selectionLayer1.muonCountFilter_cfi import countLayer1Muons
recoAtLeastTwoMuons = countLayer1Muons.clone()
recoAtLeastTwoMuons.minNumber = 2
recoAtLeastTwoMuons.maxNumber = 999999 #not changed, only for information
recoAtLeastTwoMuons.src = recoZll.srcLeg1 #same collection as for 'recoZll'


recoZllReconstruction = cms.Sequence(
    recoAtLeastTwoMuons * 
    recoZll
    )


