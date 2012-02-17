import FWCore.ParameterSet.Config as cms
import copy

from Configuration.EventContent.EventContent_cff import *

tauAnalysisEventContent = copy.deepcopy(AODSIMEventContent)

#tauAnalysisEventContent.outputCommands.extend(RecoEcalRECO.outputCommands)
#tauAnalysisEventContent.outputCommands.extend(RecoParticleFlowRECO.outputCommands)

#--------------------------------------------------------------------------------
# keep all ECAL + HCAL recHits for computation of IsoDeposits
#--------------------------------------------------------------------------------

allEcalRecHits = cms.PSet(
    outputCommands = cms.untracked.vstring(
        'keep *_ecalRecHit_EcalRecHitsEB_*',
        'keep *_ecalRecHit_EcalRecHitsEE_*',
        'keep *_ecalRecHit_EcalRecHitsES_*'
    )
)
tauAnalysisEventContent.outputCommands.extend(allEcalRecHits.outputCommands)

allHcalRecHits = cms.PSet(
    outputCommands = cms.untracked.vstring(
        'keep *_hbhereco_*_*',
        'keep *_hfreco_*_*',
        'keep *_horeco_*_*'
    )
)
tauAnalysisEventContent.outputCommands.extend(allHcalRecHits.outputCommands)

pfJets = cms.PSet(
    outputCommands = cms.untracked.vstring(
        'keep *_iterativeCone5PFJets_*_*',
    )
)
tauAnalysisEventContent.outputCommands.extend(pfJets.outputCommands)

#--------------------------------------------------------------------------------
# keep special track collections from the Z->tautau embedded data samples
#--------------------------------------------------------------------------------
embeddedTracks = cms.PSet(
    outputCommands = cms.untracked.vstring(
        'keep *_tmfTracks_*_*'
    )
)
tauAnalysisEventContent.outputCommands.extend(embeddedTracks.outputCommands)

#--------------------------------------------------------------------------------
# keep TrackExtra objects for computation of electron anti-photon conversion vetos
#--------------------------------------------------------------------------------

#trackExtras = cms.PSet(
#    outputCommands = cms.untracked.vstring(
#        'keep recoTrackExtras_generalTracks_*_*',
#        'keep recoTrackExtras_electronGsfTracks_*_*',
#        'keep recoGsfTrackExtras_electronGsfTracks_*_*',
#		'keep *_ckfInOutTracksFromConversions_*_*',
#		'keep *_ckfOutInTracksFromConversions_*_*'
#    )
#)
#tauAnalysisEventContent.outputCommands.extend(trackExtras.outputCommands)


