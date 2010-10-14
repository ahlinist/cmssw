### ########################################
### Changed by:                          ###
### Nicola Pozzobon                      ###
### UNIPD                                ###
### 2010, August                         ###
###                                      ###
### Added feature:                       ###
### Possibility to have a Tracklet VTX   ###
### which is different from the default  ###
### one (0,0,z).                         ###
### Choice is made with 'customLabel'    ###
### and 'BeamSpotLabel'. Set them to be  ###
### equal to each other and to "custom"  ###
### to force the beamspot position,      ###
### otherwise the standard choice is     ###
### made (offlineBeamSpot).              ###
### NOTE these comments are not so       ###
### accurate...                          ###
### ########################################

import FWCore.ParameterSet.Config as cms

TrackletsFromSimHits = cms.EDFilter("TrackletBuilder_PSimHit_",
    minPtThreshold = cms.double(2.0),
    ipWidth = cms.double(15.0),
	fastPhiCut = cms.double(0.1),
    GlobalStubs = cms.InputTag("GlobalStubsFromSimHits"),
    customLabel = cms.InputTag("custom"),
    BeamSpotLabel = cms.InputTag("offlineBeamSpot"),
    BeamSpotSetX = cms.double(0.0),
    BeamSpotSetY = cms.double(0.0),
    BeamSpotSetZ = cms.double(0.0),
)

TrackletsFromPixelDigis = cms.EDFilter("TrackletBuilder_PixelDigi_",
    minPtThreshold = cms.double(2.0),
    ipWidth = cms.double(15.0),
	fastPhiCut = cms.double(0.1),
    GlobalStubs = cms.InputTag("GlobalStubsFromPixelDigis"),
    customLabel = cms.InputTag("custom"),
    BeamSpotLabel = cms.InputTag("offlineBeamSpot"),
    BeamSpotSetX = cms.double(0.0),
    BeamSpotSetY = cms.double(0.0),
    BeamSpotSetZ = cms.double(0.0),
)

#TrackletsFromTrackTriggerHits = cms.EDFilter("TrackletBuilder_TTHit_",
#    minPtThreshold = cms.double(2.0),
#    ipWidth = cms.double(15.0),
#	fastPhiCut = cms.double(0.1),
#    GlobalStubs = cms.InputTag("GlobalStubsFromTrackTriggerHits"),
#    customLabel = cms.InputTag("custom"),
#    BeamSpotLabel = cms.InputTag("offlineBeamSpot"),
#    BeamSpotSetX = cms.double(0.0),
#    BeamSpotSetY = cms.double(0.0),
#    BeamSpotSetZ = cms.double(0.0),
#)

