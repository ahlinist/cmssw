import FWCore.ParameterSet.Config as cms

# generic
from TauAnalysis.CandidateTools.diCandidatePairProducer_cfi import diTauProducer
recoZll = diTauProducer.clone()
recoZll.srcLeg1 = 'selectedLayer1Muons'
recoZll.srcLeg2 = 'selectedLayer1Muons'
recoZll.srcMET = ''
recoZll.recoMode = ''
recoZll.useLeadingTausOnly = False

# PAT filters
from PhysicsTools.PatAlgos.selectionLayer1.muonMinFilter_cfi import minLayer1Muons
recoAtLeastTwoMuons = minLayer1Muons.clone()
recoAtLeastTwoMuons.minNumber = 2


recoZllReconstruction = cms.Sequence(
    recoAtLeastTwoMuons * 
    recoZll
    )


