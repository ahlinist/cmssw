import FWCore.ParameterSet.Config as cms
import copy

from PhysicsTools.PatAlgos.patEventContent_cff import *

#--------------------------------------------------------------------------------
# per default, drop everything that is not specifically kept
#--------------------------------------------------------------------------------
patTupleEventContent = cms.PSet(
    outputCommands = cms.untracked.vstring('drop *')
)

#--------------------------------------------------------------------------------
# keep clean leptons and jets
#--------------------------------------------------------------------------------
patTupleEventContent.outputCommands.extend(
    [ 'keep *_cleanLayer1Electrons*_*_*',
      'keep *_cleanLayer1Muons*_*_*',
      'keep *_cleanLayer1Taus*_*_*',
      'keep *_cleanLayer1Jets*_*_*' ]
)

#--------------------------------------------------------------------------------
# required for Z->ee hypothesis; to be removed
#--------------------------------------------------------------------------------
patTupleEventContent.outputCommands.extend(
     [ 'keep *_iterativeCone5CaloJets_*_*',
       'keep *_iterativeCone5PFJets_*_*',
	   'keep *_generalTracks_*_*', 
	   'keep *_genElectronsFromZs_*_*', 
	   'keep *_pixelMatchGsfElectrons_*_*', 
	   'keep *_pixelMatchGsfFit_*_*' ]
)

#--------------------------------------------------------------------------------
# required by photon conversion rejection (to be removed for 3_1_x)
#--------------------------------------------------------------------------------
patTupleEventContent.outputCommands.extend(
      [ 'keep *_generalTracks_*_*',
	    'keep recoTrackExtras_pixelMatchGsfFit_*_*']
)

#--------------------------------------------------------------------------------
# required by pf*HistManager
#--------------------------------------------------------------------------------
patTupleEventContent.outputCommands.extend(
    [ 'keep *_pfAll*_*_*', 
	  'keep *_particleFlow_*_*',]
)

#--------------------------------------------------------------------------------
# used as analysis selection starting point
#--------------------------------------------------------------------------------
patTupleEventContent.outputCommands.extend(
	 [ 'keep *_genPhaseSpaceEventInfo_*_*', ]
)

#--------------------------------------------------------------------------------
# used as analysis selection variables
#--------------------------------------------------------------------------------
patTupleEventContent.outputCommands.extend(
	[ 'keep *_selectedPrimaryVertexHighestPtTrackSum_*_*',
	  'keep *_selectedPrimaryVertexQuality_*_*',
	  'keep *_selectedPrimaryVertexPosition_*_*'
	]
)

#--------------------------------------------------------------------------------
# required by genPhaseSpaceEventInfoHistManager
#--------------------------------------------------------------------------------
patTupleEventContent.outputCommands.extend(
    [ 'keep *_iterativeCone5GenJets_*_*', 
      'keep *_genParticlesFromZs_*_*'
	]
)

#--------------------------------------------------------------------------------
# required by jetHistManager
#--------------------------------------------------------------------------------
patTupleEventContent.outputCommands.extend(
    [ 'keep *_selectedLayer1JetsAntiOverlapWithLeptonsVetoCumulative_*_*' ] )

#--------------------------------------------------------------------------------
# required by metHistManager
#--------------------------------------------------------------------------------
patTupleEventContent.outputCommands.extend(
    [ 'keep *_layer1METs_*_*' ] )

#--------------------------------------------------------------------------------
# required by metTopologyHistManager
#--------------------------------------------------------------------------------
patTupleEventContent.outputCommands.extend(
    [ 'keep *_metTopologies_*_*' ] )

#--------------------------------------------------------------------------------
# required by lepton histogram managers
#--------------------------------------------------------------------------------
patTupleEventContent.outputCommands.extend(
    [ 'keep *_genParticles_*_*',
	  'keep *_tauGenJets_*_*' ] 
)

#--------------------------------------------------------------------------------
# required by triggerHistManager
#--------------------------------------------------------------------------------
patTupleEventContent.outputCommands.extend(
     [ 'keep *_TriggerResults_*_*' ] )


