import FWCore.ParameterSet.Config as cms

##### event content for heavy-ion analysis objects
from Configuration.EventContent.EventContentHeavyIons_cff import *

#jets
jetContent = cms.PSet(
    outputCommands = cms.untracked.vstring(
    'keep double*_*CaloJets_*_*',
    'drop recoCaloJets_*_*_*',
    'keep recoGenJets_*_*_*',
    'keep patJets_*_*_*'
    )
  )

jetContentExtended = jetContent.clone()
jetContentExtended.outputCommands.extend(cms.untracked.vstring('keep *_caloTowers_*_*',
                                                               'keep *_towerMaker_*_*'))

#tracks
trkContent = cms.PSet(
    outputCommands = cms.untracked.vstring(
      'keep *_offlineBeamSpot_*_*',
      'keep recoTracks_hiSelectedTracks_*_*',
      'keep recoTracks_hiPixel3PrimTracks_*_*' # low-fake selection to lower pt?
      )
    )

#muons
muonContent = cms.PSet(
    outputCommands = cms.untracked.vstring(
      'keep patMuons_selected*_*_*'
      )
    )

muonContentExtended = muonContent.clone()
muonContentExtended.outputCommands.extend(RecoMuonAOD.outputCommands)

#photons
photonContent = cms.PSet(
    outputCommands = cms.untracked.vstring(
      'keep patPhotons_selected*_*_*'
      )
    )

photonContentExtended = photonContent.clone()
photonContentExtended.outputCommands.extend(RecoEgammaAOD.outputCommands)

#correlations
corrContent = cms.PSet(
    outputCommands = cms.untracked.vstring( 
      'keep recoRecoChargedCandidates_allTracks_*_*',
      'keep recoRecoChargedCandidates_allPxltracks_*_*'
      )
    )

#common
hiCommon = cms.PSet(
    outputCommands = cms.untracked.vstring('drop *',
      'keep *_TriggerResults_*_HLT',
      'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*',
      'keep recoVertexs_hiSelectedVertex_*_*',                              
      'keep *_heavyIon_*_*',
      'keep *_hiCentrality_*_*'
    )
)

##### combinations for specific skims

# HI PAG skim
hiAnalysisSkimContent = hiCommon.clone()
hiAnalysisSkimContent.outputCommands.extend(jetContentExtended.outputCommands)
hiAnalysisSkimContent.outputCommands.extend(trkContent.outputCommands)
hiAnalysisSkimContent.outputCommands.extend(muonContent.outputCommands)
hiAnalysisSkimContent.outputCommands.extend(photonContent.outputCommands)
hiAnalysisSkimContent.outputCommands.extend(corrContent.outputCommands)

# [highpt] skim
jetTrkSkimContent = hiCommon.clone()
jetTrkSkimContent.outputCommands.extend(jetContentExtended.outputCommands)
jetTrkSkimContent.outputCommands.extend(trkContent.outputCommands)

# [dilepton] skim
muonTrkSkimContent = hiCommon.clone()
muonTrkSkimContent.outputCommands.extend(muonContentExtended.outputCommands)
muonTrkSkimContent.outputCommands.extend(trkContent.outputCommands)


