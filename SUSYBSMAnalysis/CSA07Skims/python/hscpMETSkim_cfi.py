import FWCore.ParameterSet.Config as cms

from RecoTracker.TrackProducer.RefitterWithMaterial_cff import *
from RecoTracker.DeDx.dedxHitsFromRefitter_cfi import *
from SUSYBSMAnalysis.HSCP.hscpFilter_cfi import *
from RecoTracker.DeDx.dedxTruncated40_cfi import *
from SUSYBSMAnalysis.CSA07Skims.hscpHLTPaths_cfi import *
hscpMETSeq = cms.Sequence(hscpMETHLTPath+TrackRefitter+dedxHitsFromRefitter+dedxTruncated40+hscpFilter)
TrackRefitter.TrajectoryInEvent = True
dedxTruncated40.trackDeDxHits = 'dedxHitsFromRefitter'

