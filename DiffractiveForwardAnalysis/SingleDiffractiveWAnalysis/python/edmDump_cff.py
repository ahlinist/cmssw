import FWCore.ParameterSet.Config as cms

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.castorGen_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.castorTower_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.hfTower_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.xiTower_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.pileUpInfo_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.selectGoodTracks_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.selectTracksAssociatedToPV_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.trackMultiplicity_cfi import *

xiTowerNoMET = xiTower.clone(UseMETInfo = False)
castorGenNoThreshold = castorGen.clone(CASTORParticleEnergyThreshold = 0.0)
selectTracksAssociatedToPV.src = cms.InputTag("selectGoodTracks")
selectTracksAwayFromPV = selectTracksAssociatedToPV.clone()
selectTracksAwayFromPV.MinDistanceFromVertex = 1.2
selectTracksAwayFromPV.MaxDistanceFromVertex = 999.9
trackMultiplicityAssociatedToPV = trackMultiplicity.clone(TracksTag = "selectTracksAssociatedToPV")
trackMultiplicityAwayFromPV = trackMultiplicity.clone(TracksTag = "selectTracksAwayFromPV")

edmDumpTracksAssociatedToPV = cms.Sequence(selectGoodTracks*selectTracksAssociatedToPV*trackMultiplicityAssociatedToPV) 
edmDumpTracksAwayFromPV = cms.Sequence(selectGoodTracks*selectTracksAwayFromPV*trackMultiplicityAwayFromPV)
edmDumpTracks = cms.Sequence(edmDumpTracksAssociatedToPV+edmDumpTracksAwayFromPV)

edmDumpAll = cms.Sequence(castorGen + 
                          castorGenNoThreshold + 
                          castorTower +
                          hfTower + 
                          xiTower +
                          xiTowerNoMET +
                          trackMultiplicity +
                          edmDumpTracks + 
                          pileUpInfo)
edmDumpAllNoPileUp = cms.Sequence(castorGen +
                          castorGenNoThreshold +
                          castorTower +
                          hfTower +
                          xiTower +
                          xiTowerNoMET +
                          trackMultiplicity +
                          edmDumpTracks)
