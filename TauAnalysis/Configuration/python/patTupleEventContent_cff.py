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
    [ 'keep *_cleanPatElectrons*_*_*',
      'keep *_cleanPatMuons*_*_*',
      'keep *_cleanPatTaus*_*_*',
      'keep *_cleanPatJets*_*_*' ]
)

#--------------------------------------------------------------------------------
# required for Z->ee hypothesis; to be removed
#--------------------------------------------------------------------------------
patTupleEventContent.outputCommands.extend(
     [ 'keep *_ak5CaloJets_*_*',
       'keep *_ak5PFJets_*_*',
       'keep *_generalTracks_*_*', 
       'keep *_genElectronsFromZs_*_*' ]
)

#--------------------------------------------------------------------------------
# required by photon conversion rejection
#--------------------------------------------------------------------------------
patTupleEventContent.outputCommands.extend(
      [ 'keep *_generalTracks_*_*',
		'keep *_gsfElectrons_*_*',
		'keep *_electronGsfTracks_*_*',
		'keep *_cleanPatPhotons*_*_*',
		'keep *_photonCore_*_*',
		'keep *_photons_*_*',
		'keep *_conversions_*_*',
		'keep *_ckfInOutTracksFromConversions_*_*',
		'keep *_ckfOutInTracksFromConversions_*_*']
)

#--------------------------------------------------------------------------------
# required by pf*HistManager
#--------------------------------------------------------------------------------
patTupleEventContent.outputCommands.extend(
    [ 'keep *_pfAll*_*_*', 
      'keep *_particleFlow_*_*',
	  'keep *_offlinePrimaryVerticesWithBS_*_*' ]
)

#--------------------------------------------------------------------------------
# used as analysis selection starting point
#--------------------------------------------------------------------------------
patTupleEventContent.outputCommands.extend(
    [ 'keep *_genPhaseSpaceEventInfo_*_*', 
      'keep *_genElectronsFromZs_*_*',
      'keep *_genMuonsFromZs_*_*',
      'keep *_genTausFromZs_*_*' ]
)

#--------------------------------------------------------------------------------
# used as analysis selection variables
#--------------------------------------------------------------------------------
patTupleEventContent.outputCommands.extend(
    [ 'keep *_selectedPrimaryVertexHighestPtTrackSum_*_*',
      'keep *_selectedPrimaryVertexQuality_*_*',
      'keep *_selectedPrimaryVertexPosition_*_*' ]
)

#--------------------------------------------------------------------------------
# required for estimation of systematic uncertainties
#--------------------------------------------------------------------------------
patTupleEventContent.outputCommands.extend(
    [ 'keep *_genParticles_*_*',
      'keep GenEventInfoProduct_generator_*_*' ] 
)

#--------------------------------------------------------------------------------
# required by genPhaseSpaceEventInfoHistManager
#--------------------------------------------------------------------------------
patTupleEventContent.outputCommands.extend(
    [ 'keep *_ak5GenJets_*_*', 
      'keep *_genParticlesFromZs_*_*' ]
)

#--------------------------------------------------------------------------------
# required by jetHistManager
#--------------------------------------------------------------------------------
patTupleEventContent.outputCommands.extend(
    [ 'keep *_selectedPatJetsEt20Cumulative_*_*' ]
)

#--------------------------------------------------------------------------------
# required by metHistManager
#--------------------------------------------------------------------------------
patTupleEventContent.outputCommands.extend(
    [ 'keep *_patMETs_*_*',
      'keep *_patPFMETs_*_*',
      'keep *_metsignificance_*_*' ]
)

#--------------------------------------------------------------------------------
# required by metTopologyHistManager
#--------------------------------------------------------------------------------
patTupleEventContent.outputCommands.extend(
    [ 'keep *_metTopologies_*_*' ]
)

#--------------------------------------------------------------------------------
# required by lepton histogram managers
#--------------------------------------------------------------------------------
patTupleEventContent.outputCommands.extend(
    [ 'keep *_tauGenJets_*_*' ] 
)

#--------------------------------------------------------------------------------
# required by triggerHistManager
#--------------------------------------------------------------------------------
patTupleEventContent.outputCommands.extend(
     [ 'keep *_TriggerResults_*_*',
       'keep *_hltGtDigis_*_*',
       'keep *_gtDigis_*_*',
       'keep *_hltL1GtObjectMap_*_*' ]
)

#--------------------------------------------------------------------------------
# required by event-dump plugins
#--------------------------------------------------------------------------------
patTupleEventContent.outputCommands.extend(
     [ 'keep *_genMetTrue_*_*' ]
)

#--------------------------------------------------------------------------------
# required by di-tau producer plugins
#--------------------------------------------------------------------------------
patTupleEventContent.outputCommands.extend(
     [ 'keep *_offlineBeamSpot_*_*' ]
)

#--------------------------------------------------------------------------------
# required by tau recoil energy
#--------------------------------------------------------------------------------
patTupleEventContent.outputCommands.extend(
    [ 'keep *_towerMaker_*_*',
      'keep *_patJets*_*_*' ]
)


patTupleEventContent.outputCommands.extend(
    [ 'keep *_caloEventShapeVars_*_*',
      'keep *_pfEventShapeVars_*_*',
	  'keep *_pfNoPileUp_*_*']
)
