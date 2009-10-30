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
# keep TrackExtra objects for computation of electron anti-photon conversion vetos
#--------------------------------------------------------------------------------

generalTrackExtras = cms.PSet(
    outputCommands = cms.untracked.vstring(
        'keep recoTrackExtras_generalTracks_*_*',
        'keep recoTrackExtras_electronGsfTracks_*_*',
        'keep recoGsfTrackExtras_electronGsfTracks_*_*'
    )
)
tauAnalysisEventContent.outputCommands.extend(generalTrackExtras.outputCommands)
