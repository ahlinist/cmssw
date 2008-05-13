import FWCore.ParameterSet.Config as cms

from SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi import *
import SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi
assoc2secStepTk = SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi.trackingParticleRecoTrackAsssociation.clone()
import SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi
assoc2thStepTk = SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi.trackingParticleRecoTrackAsssociation.clone()
import SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi
assoc2GsfTracks = SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi.trackingParticleRecoTrackAsssociation.clone()
import SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi
assocOutInConversionTracks = SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi.trackingParticleRecoTrackAsssociation.clone()
import SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi
assocInOutConversionTracks = SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi.trackingParticleRecoTrackAsssociation.clone()
assoc2secStepTk.label_tr = 'secStep'
assoc2thStepTk.label_tr = 'thStep'
assoc2GsfTracks.label_tr = 'pixelMatchGsfFit'
assocOutInConversionTracks.label_tr = 'ckfOutInTracksFromConversions'
assocInOutConversionTracks.label_tr = 'ckfInOutTracksFromConversions'

