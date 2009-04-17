import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.patEventContent_cff import *

#--------------------------------------------------------------------------------
# per default, drop everything that is not specifically kept
#--------------------------------------------------------------------------------

patTupleEventContent = cms.PSet(
    outputCommands = cms.untracked.vstring('drop *')
)

#--------------------------------------------------------------------------------
# keep PAT layer 1 objects
#--------------------------------------------------------------------------------

patTupleEventContent.outputCommands.extend(patEventContent)
patTupleEventContent.outputCommands.extend(patExtraAodEventContent)
patTupleEventContent.outputCommands.extend(patTriggerEventContent)

#--------------------------------------------------------------------------------
# keep collections of selected PAT layer 1 particles
#--------------------------------------------------------------------------------

patTupleEventContent.outputCommands.extend(
    [ 'keep *_selectedLayer1Electrons*_*_*',
      'keep *_selectedLayer1Muons*_*_*',
      'keep *_selectedLayer1Taus*_*_*',
      'keep *_selectedLayer1Jets*_*_*' ]
)

#--------------------------------------------------------------------------------
# keep di-tau objects
#--------------------------------------------------------------------------------

patTupleEventContent.outputCommands.extend(
    [ 'keep *_allElecMuPairs*_*_*',
      'keep *_selectedElecMuPairs*_*_*',
      'keep *_allElecTauPairs*_*_*',
      'keep *_selectedElecTauPairs*_*_*',
      'keep *_allMuTauPairs*_*_*',
      'keep *_selectedMuTauPairs*_*_*',
      'keep *_allDiTauPairs*_*_*',
      'keep *_selectedDiTauPairs*_*_*' ]
)

#--------------------------------------------------------------------------------
# keep generator level tau-jets
#--------------------------------------------------------------------------------

patTupleEventContent.outputCommands.extend(
    [ 'keep *_tauGenJets*_*_*', ]
)

#--------------------------------------------------------------------------------
# keep boolean flags indicating whether or not an event
# passes or fails the event selection criteria
#--------------------------------------------------------------------------------

patTupleEventContent.outputCommands.extend(
    [ 'keep bool_*_*_*', ]
)

