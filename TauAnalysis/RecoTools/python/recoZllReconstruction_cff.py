import FWCore.ParameterSet.Config as cms

# generic
from TauAnalysis.CandidateTools.diTauProducer_cfi import diTauProducer
recoZll = diTauProducer.clone()
recoZll.hadronicTaus = 'selectedLayer1Muons'
recoZll.leptonicTaus = 'selectedLayer1Muons'
recoZll.METs = ''
recoZll.metMode = 1
recoZll.useLeadingTaus = False

# PAT filters
from PhysicsTools.PatAlgos.selectionLayer1.muonMinFilter_cfi import minLayer1Muons
recoAtLeastTwoMuons = minLayer1Muons.clone()
recoAtLeastTwoMuons.minNumber = 2


recoZllReconstruction = cms.Sequence(
    recoAtLeastTwoMuons * 
    recoZll
    )


